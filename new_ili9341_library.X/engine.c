/*
 * File:   engine.c
 * Author: kabay
 *
 * Created on 28 May 2024, 09:47
 */


#include "bit_settings.h"
#include "spi_pic18f.h"
#include "ili9341.h"
#include "bitmap.h"
#include "font.h"
#include "time_delay.h"

#include "engine.h"


// Helper functions
void exit_car_error(const char message[])
{
    TFT_FillScreen(BROWN);
    TFT_ConstText(message, 10, 10, WHITE, BLACK);
    while(1) {};
}

uint16_t safe_convert(int16_t value, char error_token[]) {
    if (value < 0) {
        exit_car_error("E: ");
    }
    return (uint16_t)value;
}


// ---------- VARIABLES ----------
int16_t target_fps = 60;
int16_t target_dt = 17;
int16_t last_frame_duration = 0;
char last_frame_duration_text[30] = {0};
char fps_text[30] = {0};



// ---------- INTERNALS ---------- 
void __interrupt() timer0_ISR(void){
    if (INTCONbits.TMR0IF == 1){     // v?rifie si l'interruption est bien provoqu?e par le timer0
        exit_car_error("Le timer a overflow, ce n'est pas normal");
        INTCONbits.TMR0IF = 0;      // flag d'interruption effacï¿½
    }
}

void init_timer(void){
    T0CON = 0x02;           // TIMER0: OFF, 16-bit, cadenc? par Tcy (1MHz/4), prescaler = 8
                            // donc f = (1MHz/4) / 8 = 31.250 kHz
    INTCONbits.TMR0IF = 0;
    INTCONbits.TMR0IE = 1;
};

void start_timer(void) {
    TMR0 = 0x0;                 // on met le timer ï¿½ 0
    T0CONbits.TMR0ON = 1;       // allume le timer
}

int16_t get_timer_value(void) {
    T0CONbits.TMR0ON = 0;     // On arrete le timer
    
    uint16_t timer_value = TMR0;
    int16_t time_passed = (int16_t)(timer_value * 0.0016f);   // car microc en 64MHz
    
    return time_passed;
}



// ---------- CORE ----------
void init_game_console(void)
{
    // Init screen 
    OSCCON = 0b01110000;
    OSCTUNEbits.PLLEN = 1; // turn on the PLL 64 MHz
    ANSELA = 0; ANSELB = 0; ANSELC = 0; ANSELD = 0; ANSELE = 0;
    PORTA  = 0; PORTB  = 0; PORTC  = 0; PORTD  = 0; PORTE  = 0;
    TRISA  = 0; TRISB  = 0; TRISC  = 0; TRISD  = 0; TRISE  = 0;
    SPI1_Close();
    SPI1_Init();
    TFT_Init();
    
//    while(1) {
//        Delay_ms(1000);
//        TFT_Box(10, 10, 20, 20, BLUE);
//        Delay_ms(1000);
//        TFT_Box(10, 10, 20, 20, GREEN);
//    }

    
    // init font
    TFT_SetFont(Courier_New_Bold_20, 1);
    
    // Init timer0
    init_timer();
    
    // Init buttons
    TRISBbits.TRISB0 = 1;   // Configurer TRISB0 comme une entrï¿½e (bouton)
    TRISAbits.TRISA4 = 1;   // Configurer TRISA4 comme une entrï¿½e (bouton)
}

void set_target_fps(const int16_t fps)
{
    target_fps = fps;
    target_dt = 1000 / fps;
}

void mystery_function() {
    // Rï¿½cuperer la valeur du timer
    last_frame_duration = get_timer_value();
    
    // Si on est au dessus de target_fps
    if (last_frame_duration < target_dt) {
        sleep_ms(target_dt - last_frame_duration);
        last_frame_duration = target_dt;
    }
    
    // restart the timer
    start_timer();
}

void sleep_ms(int16_t duration)
{
    uint16_t count = safe_convert(duration, "1111");
    Delay_ms(count);
}

float get_frame_time(void)
{
    return (float)last_frame_duration / 1000;
}

// ---------- RENDERING ----------
void fill_screen(uint16_t color)
{
    TFT_FillScreen(color);
}

void draw_rectangle(int16_t pos_x, int16_t pos_y, int16_t width, int16_t height, uint16_t color)
{
    uint16_t x1 = safe_convert(pos_x, "1");
    uint16_t y1 = safe_convert(pos_y, "2");
    uint16_t x2 = safe_convert(pos_x + width, "3");
    uint16_t y2 = safe_convert(pos_y + height, "4");

    TFT_Box(x1, y1, x2, y2, color);
}


void draw_moving_rectangle(Vector2i new_position, Vector2i old_position, int16_t width, int16_t height, uint16_t color, uint16_t background_color) 
{
    // Calcul de la zone de chevauchement
    int left = old_position.x;
    if (new_position.x > left) left = new_position.x;
    int right = old_position.x + width;
    if (new_position.x + width < right) right = new_position.x + width;
    int top = old_position.y;
    if (new_position.y > top) top = new_position.y;
    int bottom = old_position.y + height;
    if (new_position.y + height < bottom) bottom = new_position.y + height;

    // Dessin des nouvelles zones
    if (new_position.x < old_position.x) {
        TFT_Box(safe_convert(new_position.x, "0"), safe_convert(new_position.y, "0"), safe_convert(old_position.x, "0"), safe_convert(new_position.y + height, "0"), color);
    }
    if (new_position.x > old_position.x) {
        TFT_Box(safe_convert(old_position.x + width, "0"), safe_convert(new_position.y, "0"), safe_convert(new_position.x + width, "0"), safe_convert(new_position.y + height, "0"), color);
    }
    if (new_position.y < old_position.y) {
        TFT_Box(safe_convert(new_position.x, "0"), safe_convert(new_position.y, "0"), safe_convert(new_position.x + width, "0"), safe_convert(old_position.y, "0"), color);
    }
    if (new_position.y > old_position.y) {
        TFT_Box(safe_convert(new_position.x, "0"), safe_convert(old_position.y + height, "0"), safe_convert(new_position.x + width, "0"), safe_convert(new_position.y + height, "0"), color);
    }

    // Nettoyage des anciennes zones
    if (old_position.x < new_position.x) {
        TFT_Box(safe_convert(old_position.x, "0"), safe_convert(old_position.y, "0"), safe_convert(new_position.x, "0"), safe_convert(old_position.y + height, "0"), background_color);
    }
    if (old_position.x > new_position.x) {
        TFT_Box(safe_convert(right, "0"), safe_convert(old_position.y, "0"), safe_convert(old_position.x + width, "0"), safe_convert(old_position.y + height, "0"), background_color);
    }
    if (old_position.y < new_position.y) {
        TFT_Box(safe_convert(old_position.x, "0"), safe_convert(old_position.y, "0"), safe_convert(old_position.x + width, "0"), safe_convert(new_position.y, "0"), background_color);
    }
    if (old_position.y > new_position.y) {
        TFT_Box(safe_convert(old_position.x, "0"), safe_convert(bottom, "0"), safe_convert(old_position.x + width, "0"), safe_convert(old_position.y + height, "0"), background_color);
    }
}

void draw_fps(int16_t pos_x, int16_t pos_y)
{
    sprintf(last_frame_duration_text, "dt: %d", last_frame_duration);
    sprintf(fps_text, "fps: %d", (uint16_t)((1.f/last_frame_duration)*1000));
    
    draw_text(last_frame_duration_text, pos_x, pos_y, GREEN, BLACK);
    draw_text(fps_text, pos_x, pos_y + 30, GREEN, BLACK);
}

void draw_text(char *text, int16_t x, int16_t y, uint16_t color1, uint16_t color2)
{
    //  void TFT_Text(char *buffer, uint16_t x, uint16_t y, uint16_t color1, uint16_t color2)
    uint16_t x1 = safe_convert(x, "67");
    uint16_t y1 = safe_convert(y, "68");
    TFT_Text(text, x1, y1, color1, color2);
}



// ---------- AUDIO ---------- 
void init_buzzer(void){
    TRISCbits.TRISC1 = 0;   // RC1 est la sortie PWM, sur laquelle est connect? le buzzer
    T2CON = 0b00000110;     // Postscaler = 1, Timer ON, Prescaler = 16
    CCP2CON = 0b00001100;   // mode PWM, avec 1 seule sortie sur RC1
}
// La 3
void play_A3(int16_t duration){
    PR2 = 180;
    CCPR2L = 90;
    sleep_ms(duration);
    //stop_buzzer();
}
// Si 3
void play_B3(int16_t duration){
    PR2 = 160;
    CCPR2L = 80;
    sleep_ms(duration);
    //stop_buzzer();
}
// Do 4
void play_C4(int16_t duration){
    PR2 = 151;
    CCPR2L = 75;
    sleep_ms(duration);
    //stop_buzzer();
}
// Ré 4
void play_D4(int16_t duration){
    PR2 = 135;
    CCPR2L = 67;
    sleep_ms(duration);
    //stop_buzzer();
}
//
void play_D_4(int16_t duration){
    PR2 = 127;
    CCPR2L = 63;
    sleep_ms(duration);
    //stop_buzzer();
}
// Mi 4
void play_E4(int16_t duration){
    PR2 = 120;
    CCPR2L = 60;
    sleep_ms(duration);
    //stop_buzzer();
}
// Sol 4
void play_G4(int16_t duration){
    PR2 = 101;
    CCPR2L = 50;
    sleep_ms(duration);
    //stop_buzzer();
}
//
void play_A_4(int16_t duration){
    PR2 = 86;
    CCPR2L = 43;
    sleep_ms(duration);
    //stop_buzzer();
}

void play_tetris(void){
    play_E4(400);
    play_B3(200);
    play_C4(200);
    play_D4(400);
    play_C4(200);
    play_B3(200);
    play_A3(400);
    play_A3(200);
    stop_buzzer();
    play_C4(200);
    play_E4(400);
    stop_buzzer();
}

void play_game_over(void){
    play_G4(400);
    play_G4(400);
    play_G4(400);
    play_D_4(150);
    play_A_4(100);
    play_G4(400);
    play_D_4(150);
    play_A_4(100);
    play_G4(400);
    stop_buzzer();
}

void stop_buzzer(void){
    PR2 = 0;
    CCPR2L = 0;
}



