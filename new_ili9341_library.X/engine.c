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
int16_t running = TRUE;
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
    
    // init font
    TFT_SetFont(Courier_New_Bold_20, 1);
    
    // Init timer0
    init_timer();
    
    // init buzzer 
    init_buzzer();
    
    // Init buttons
    TRISAbits.TRISA4 = 1;   // Configurer TRISA4 comme une entrï¿½e (bouton)
    TRISBbits.TRISB0 = 1;   // Configurer TRISB0 comme une entrï¿½e (bouton)
    TRISBbits.TRISB1 = 1;   // Configurer TRISB0 comme une entrï¿½e (bouton)
    TRISBbits.TRISB2 = 1;   // Configurer TRISA4 comme une entrï¿½e (bouton)
    TRISBbits.TRISB3 = 1;   // Configurer TRISB0 comme une entrï¿½e (bouton)
    TRISBbits.TRISB4 = 1;   // Configurer TRISA4 comme une entrï¿½e (bouton)
    TRISBbits.TRISB5 = 1;   // Configurer TRISB0 comme une entrï¿½e (bouton)
    TRISBbits.TRISB6 = 1;   // Configurer TRISA4 comme une entrï¿½e (bouton)
    // mettre les boutons à 0
    PORTAbits.RA4 = 0;
    PORTBbits.RB0 = 0;
    PORTBbits.RB1 = 0;
    PORTBbits.RB2 = 0;
    PORTBbits.RB3 = 0;
    PORTBbits.RB4 = 0;
    PORTBbits.RB5 = 0;
    PORTBbits.RB6 = 0;
}

void set_target_fps(const int16_t fps)
{
    target_fps = fps;
    target_dt = 1000 / fps;
}

int16_t game_should_stop() {
    // CLOSE THE GAME IF NEEDED
    if (running == FALSE) {
        return TRUE;
    }
    
    // GESTION DES FPS
    // Rï¿½cuperer la valeur du timer
    last_frame_duration = get_timer_value();
    
    // Si on est au dessus de target_fps
    if (last_frame_duration < target_dt) {
        sleep_ms(target_dt - last_frame_duration);
        last_frame_duration = target_dt;
    }
    
    // restart the timer
    start_timer();
    
    return FALSE;
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


int16_t is_button_pressed(int16_t button)
{
    switch (button) {
        case BUTTON_UP:
            return !PORTBbits.RB2;     // 1 si pressé, et 0 sinon
            break;
        case BUTTON_DOWN:
            return !PORTBbits.RB3;
            break;
        case BUTTON_RIGHT:
            return !PORTBbits.RB1;
            break;
        case BUTTON_LEFT:
            return !PORTBbits.RB0;
            break;
        case BUTTON_A:
            return !PORTBbits.RB4;
            break;
        case BUTTON_B:
            return !PORTBbits.RB5;
            break;
        case BUTTON_HOME:
            return !PORTBbits.RB6;
            break;
        default:
            return -1;
            break;
    }
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


void draw_moving_rectangle(int16_t new_pos_x, int16_t new_pos_y, int16_t old_pos_x, int16_t old_pos_y,
                           int16_t width, int16_t height, uint16_t color, uint16_t background_color) {
    // Calculate the old and new rectangle boundaries
    uint16_t old_left = safe_convert(old_pos_x, "1");
    uint16_t old_top = safe_convert(old_pos_y, "2");
    uint16_t old_right = safe_convert(old_pos_x + width - 1, "3");
    uint16_t old_bottom = safe_convert(old_pos_y + height - 1, "4");

    uint16_t new_left = safe_convert(new_pos_x, "5");
    uint16_t new_top = safe_convert(new_pos_y, "6");
    uint16_t new_right = safe_convert(new_pos_x + width - 1, "7");
    uint16_t new_bottom = safe_convert(new_pos_y + height - 1, "8");

    // Clear the regions not overlapped
    // Clear left strip
    if (new_left > old_left) {
        TFT_Box(old_left, old_top, new_left - 1, old_bottom, background_color);
    }
    // Clear right strip
    if (new_right < old_right) {
        TFT_Box(new_right + 1, old_top, old_right, old_bottom, background_color);
    }
    // Clear top strip
    if (new_top > old_top) {
        TFT_Box(old_left, old_top, old_right, new_top - 1, background_color);
    }
    // Clear bottom strip
    if (new_bottom < old_bottom) {
        TFT_Box(old_left, new_bottom + 1, old_right, old_bottom, background_color);
    }

    // Draw the new rectangle position
    TFT_Box(new_left, new_top, new_right, new_bottom, color);
}


//void draw_moving_rectangle(int16_t new_pos_x, int16_t new_pos_y, int16_t old_pos_x, int16_t old_pos_y,
//                           int16_t width, int16_t height, uint16_t color, uint16_t background_color)
//{
//    // Calcul de la zone de chevauchement
//    int left = old_pos_x;
//    if (new_pos_x > left) left = new_pos_x;
//    int right = old_pos_x + width;
//    if (new_pos_x + width < right) right = new_pos_x + width;
//    int top = old_pos_y;
//    if (new_pos_y > top) top = new_pos_y;
//    int bottom = old_pos_y + height;
//    if (new_pos_y + height < bottom) bottom = new_pos_y + height;
//
//    // Dessin des nouvelles zones
//    if (new_pos_x < old_pos_x) {
//        TFT_Box(safe_convert(new_pos_x, "0"), safe_convert(new_pos_y, "0"), safe_convert(old_pos_x, "0"), safe_convert(new_pos_y + height, "0"), color);
//    }
//    if (new_pos_x > old_pos_x) {
//        TFT_Box(safe_convert(old_pos_x + width, "0"), safe_convert(new_pos_y, "0"), safe_convert(new_pos_x + width, "0"), safe_convert(new_pos_y + height, "0"), color);
//    }
//    if (new_pos_y < old_pos_y) {
//        TFT_Box(safe_convert(new_pos_x, "0"), safe_convert(new_pos_y, "0"), safe_convert(new_pos_x + width, "0"), safe_convert(old_pos_y, "0"), color);
//    }
//    if (new_pos_y > old_pos_y) {
//        TFT_Box(safe_convert(new_pos_x, "0"), safe_convert(old_pos_y + height, "0"), safe_convert(new_pos_x + width, "0"), safe_convert(new_pos_y + height, "0"), color);
//    }
//
//    // Nettoyage des anciennes zones
//    if (old_pos_x < new_pos_x) {
//        TFT_Box(safe_convert(old_pos_x, "0"), safe_convert(old_pos_y, "0"), safe_convert(new_pos_x, "0"), safe_convert(old_pos_y + height, "0"), background_color);
//    }
//    if (old_pos_x > new_pos_x) {
//        TFT_Box(safe_convert(right, "0"), safe_convert(old_pos_y, "0"), safe_convert(old_pos_x + width, "0"), safe_convert(old_pos_y + height, "0"), background_color);
//    }
//    if (old_pos_y < new_pos_y) {
//        TFT_Box(safe_convert(old_pos_x, "0"), safe_convert(old_pos_y, "0"), safe_convert(old_pos_x + width, "0"), safe_convert(new_pos_y, "0"), background_color);
//    }
//    if (old_pos_y > new_pos_y) {
//        TFT_Box(safe_convert(old_pos_x, "0"), safe_convert(bottom, "0"), safe_convert(old_pos_x + width, "0"), safe_convert(old_pos_y + height, "0"), background_color);
//    }
//}

void draw_fps(int16_t pos_x, int16_t pos_y)
{
    sprintf(last_frame_duration_text, "dt: %d", last_frame_duration);
    sprintf(fps_text, "fps: %d", (uint16_t)((1.f/last_frame_duration)*1000));
    
    draw_text(last_frame_duration_text, pos_x, pos_y, GREEN, BLACK);
    draw_text(fps_text, pos_x, pos_y + 30, GREEN, BLACK);
}

void draw_const_text(const char *text, int16_t x, int16_t y, uint16_t color1, uint16_t color2)
{
    //  void TFT_Text(char *buffer, uint16_t x, uint16_t y, uint16_t color1, uint16_t color2)
    uint16_t x1 = safe_convert(x, "67");
    uint16_t y1 = safe_convert(y, "68");
    TFT_ConstText(text, x1, y1, color1, color2);
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
    
    PR2 = 0;
    CCPR2L = 0;
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



