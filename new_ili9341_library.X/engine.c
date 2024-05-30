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
// global
int16_t target_fps = 60;

// internal
int16_t target_dt;
char time_passed_text[30] = {0};
char fps_text[30] = {0};



// ---------- INTERNALS ---------- 
void __interrupt() timer0_ISR(void){
    if (INTCONbits.TMR0IF == 1){     // v?rifie si l'interruption est bien provoqu?e par le timer0
        exit_car_error("Le timer a overflow, ce n'est pas normal");
        INTCONbits.TMR0IF = 0;      // flag d'interruption effac�
    }
}

void init_timer(void){
    T0CON = 0x02;           // TIMER0: OFF, 16-bit, cadenc? par Tcy (1MHz/4), prescaler = 8
                            // donc f = (1MHz/4) / 8 = 31.250 kHz
    INTCONbits.TMR0IF = 0;
    INTCONbits.TMR0IE = 1;
};

void start_timer(void) {
    TMR0 = 0x0;                 // on met le timer � 0
    T0CONbits.TMR0ON = 1;       // allume le timer
}

int16_t get_timer_value(void) {
    T0CONbits.TMR0ON = 0;     // On arrete le timer
    
    uint16_t timer_value = TMR0;
    int16_t time_passed = (int16_t)(timer_value * 0.0016f);
    
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
    
    // Init buttons
    TRISBbits.TRISB0 = 1;   // Configurer TRISB0 comme une entr�e (bouton)
    TRISAbits.TRISA4 = 1;   // Configurer TRISA4 comme une entr�e (bouton)
}

void set_target_fps(const int16_t fps)
{
    target_fps = fps;
    
    target_dt = 1000 / fps;
}

void mystery_function() {
    // R�cuperer la valeur du timer
    int16_t time_passed = get_timer_value();
    sprintf(time_passed_text, "dt: %d", time_passed);
    sprintf(fps_text, "fps: %d", (uint16_t)((1.f/time_passed)*1000));
    
    // On attends si on est au dessus de 60 fps
    if (time_passed < target_dt) {
        sleep_ms(target_dt - time_passed);
    }
    
    // restart the timer
    start_timer();
}

void sleep_ms(int16_t duration)
{
    uint16_t count = safe_convert(duration, "1111");
    Delay_ms(count);
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


void draw_moving_rectangle(Vector2i new_position, Vector2i old_position, int16_t width, int16_t height, uint16_t color, uint16_t background_color) {
    
    uint16_t new_position_x = safe_convert(new_position.x, "81");
    uint16_t new_position_y = safe_convert(new_position.y, "82");
    uint16_t old_position_x = safe_convert(old_position.x, "83");
    uint16_t old_position_y = safe_convert(new_position.y, "84");
    uint16_t width2 = safe_convert(width, "85");
    uint16_t height2 = safe_convert(height, "86");
    
    // Calcul de la zone de chevauchement
    int right = old_position_x + width;
    if (new_position_x + width < right) {
        right = new_position_x + width;
    }
    int bottom = old_position_y + height;
    if (new_position_y + height < bottom) {
        bottom = new_position_y + height;
    }

    // Dessin des nouvelles zones
    if (new_position_x < old_position_x) {
        TFT_Box(new_position_x, new_position_y, old_position_x, new_position_y + height, color);
    }
    if (new_position_x > old_position_x) {
        TFT_Box(old_position_x + width, new_position_y, new_position_x + width, new_position_y + height, color);
    }
    if (new_position_y < old_position_y) {
        TFT_Box(new_position_x, new_position_y, new_position_x + width, old_position_y, color);
    }
    if (new_position_y > old_position_y) {
        TFT_Box(new_position_x, old_position_y + height, new_position_x + width, new_position_y + height, color);
    }

    // Nettoyage des anciennes zones
    if (old_position_x < new_position_x) {
        TFT_Box(old_position_x, old_position_y, new_position_x, old_position_y + height, background_color);
    }
    if (old_position_x > new_position_x) {
        TFT_Box(right, old_position_y, old_position_x + width, old_position_y + height, background_color);
    }
    if (old_position_y < new_position_y) {
        TFT_Box(old_position_x, old_position_y, old_position_x + width, new_position_y, background_color);
    }
    if (old_position_y > new_position_y) {
        TFT_Box(old_position_x , bottom, old_position_x + width, old_position_y + height, background_color);
    }
}



//void draw_moving_rectangle(int16_t old_left, int16_t old_top, int16_t old_right, int16_t old_bottom,
//                    int16_t new_left, int16_t new_top, int16_t new_right, int16_t new_bottom,
//                    int16_t clear_color, int16_t fill_color)
//{
//    // Clear the regions not overlapped
//    // Clear left strip
//    if (new_left > old_left) {
//        TFT_Box(safe_convert(old_left), safe_convert(old_top), safe_convert(new_left - 1), safe_convert(old_bottom), safe_convert(clear_color));
//    }
//    // Clear right strip
//    if (new_right < old_right) {
//        TFT_Box(safe_convert(new_right + 1), safe_convert(old_top), safe_convert(old_right), safe_convert(old_bottom), safe_convert(clear_color));
//    }
//    // Clear top strip
//    if (new_top > old_top) {
//        TFT_Box(safe_convert(old_left), safe_convert(old_top), safe_convert(old_right), safe_convert(new_top - 1), safe_convert(clear_color));
//    }
//    // Clear bottom strip
//    if (new_bottom < old_bottom) {
//        TFT_Box(safe_convert(old_left), safe_convert(new_bottom + 1), safe_convert(old_right), safe_convert(old_bottom), safe_convert(clear_color));
//    }
//
//    // Draw the new rectangle position as a filled box
//    TFT_Box(safe_convert(new_left), safe_convert(new_top), safe_convert(new_right), safe_convert(new_bottom), safe_convert(fill_color));
//}


void draw_text(schar__t *text, int16_t x, int16_t y, uint16_t color1, uint16_t color2)
{
    //  void TFT_Text(schar_t *buffer, uint16_t x, uint16_t y, uint16_t color1, uint16_t color2)
    uint16_t x1 = safe_convert(x, "67");
    uint16_t y1 = safe_convert(y, "68");
    TFT_Text(text, x1, y1, color1, color2);
}



// ---------- AUDIO ---------- 
void init_buzzer(void){
    TRISCbits.TRISC2 = 0;   // RC2 est la sortie PWM, sur laquelle est connect? le buzzer
    T2CON = 0b00000110;     // Postscaler = 1, Timer ON, Prescaler = 16
    CCP1CON = 0b00001100;   // mode PWM, avec 1 seule sortie sur RC2
}

void play_impact_sound(void){
    PR2 = 35;           // fr?quence_PWM = (1MHz)/4 /prescaler / 36 = 440 Hz
    CCPR1L = 18;        // rapport cyclique = 50%
    _delay(125000);     // 125000 * Tcy = 125000 * 4us = 0.125 sec
    stop_buzzer();
}

void stop_buzzer(void){
    PR2 = 0;
    CCPR1L = 0;
}



