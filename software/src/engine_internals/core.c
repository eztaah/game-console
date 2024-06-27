/*
 * File:   engine.c
 * Author: eztaah, MatteoPerez, MokhmadGUIRIEV and 21KEBY
 */

#include <string.h>
#include "bit_settings.h"
#include "../engine.h"

void _e_init_screen(uint16_t color);
void _e_init_buzzer(void);
void _e_stop_buzzer(void);
void _e_init_buttons(void);

// Global variables
int16_t want_to_exit_game = FALSE;
int16_t target_fps = 60;
int16_t target_dt = 17;
int16_t last_frame_duration = 0;


//                          INTERNAL FUNCTIONS
//==============================================================================
// This function initializes TIMER0 with specific settings for clock and prescaler.
//==============================================================================
void _e_init_timer(void){
    T0CON = 0x02;           // TIMER0: OFF, 16-bit, cadenc? par Tcy (1MHz/4), prescaler = 8
                            // donc f = (1MHz/4) / 8 = 31.250 kHz
    INTCONbits.TMR0IF = 0;
    INTCONbits.TMR0IE = 1;
};

//==============================================================================
// This function starts TIMER0 by setting its operation flag to ON.
//==============================================================================
void _e_start_timer(void) {
    TMR0 = 0x0;                 // on met le timer � 0
    T0CONbits.TMR0ON = 1;       // allume le timer
}

//==============================================================================
// This function stops TIMER0 and calculates the time passed based on its value.
// Returns the time passed in milliseconds as calculated from TIMER0's count.
//==============================================================================
int16_t _e_get_timer_value(void) 
{    
    uint16_t timer_value = TMR0;
    int16_t time_passed = (int16_t)(timer_value * 0.0016f);   // car microc en 64MHz
    
    return time_passed;
}

//==============================================================================
// This function initiates the LEDs ports as output and turn the LEDs off.
//==============================================================================
void _e_init_LEDs(void){
    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB3 = 0;
    TRISBbits.TRISB4 = 0;
    TRISBbits.TRISB5 = 0;
    LATBbits.LATB2 = 0;
    LATBbits.LATB3 = 0;
    LATBbits.LATB4 = 0;
    LATBbits.LATB5 = 0;
}

//==============================================================================
// Interrupt function.
//==============================================================================
void __interrupt() timer0_ISR(void)
{
    if (INTCONbits.TMR0IF == 1){     // v?rifie si l'interruption est bien provoqu?e par le timer0
        e_throw_error("Le timer a overflow, ce n'est pas normal");
        INTCONbits.TMR0IF = 0;      // flag d'interruption effac�
    }
}


//                             CORE FUNCTIONS
//==============================================================================
// This function initializes the hardware and settings for the game console.
//==============================================================================
void e_init_game_console(uint16_t initial_screen_color)
{
    OSCCON = 0b01110000;
    OSCTUNEbits.PLLEN = 1; // turn on the PLL 64 MHz
    ANSELA = 0; 
    ANSELB = 0; 
    ANSELC = 0; 
    ANSELD = 0; 
    ANSELE = 0;
    PORTA  = 0; 
    PORTB  = 0; 
    PORTC  = 0; 
    PORTD  = 0; 
    PORTE  = 0;
    TRISA  = 0; 
    TRISB  = 0; 
    TRISC  = 0; 
    TRISD  = 0; 
    TRISE  = 0;
    
    // Init screen 
    _e_init_screen(initial_screen_color);
    
    // Init timer0
    _e_init_timer();
    
    // init buzzer 
    _e_init_buzzer();
    
    // Init buttons
    _e_init_buttons();
    
    //Init LEDs
    _e_init_LEDs();
}

//==============================================================================
// This function checks if the game loop should stop based on the running flag.
// Returns TRUE if the game should stop, otherwise FALSE.
//==============================================================================
uint8_t e_game_should_stop(void)
{
    // CLOSE THE GAME IF NEEDED
    if (want_to_exit_game == TRUE || e_is_button_pressed(BUTTON_HOME)) {
        want_to_exit_game = FALSE;
        return TRUE;
    }

    // GESTION DES FPS
    // R�cuperer la valeur du timer
    last_frame_duration = _e_get_timer_value();
    T0CONbits.TMR0ON = 0;     // On arrete le timer
    
    // Si on est au dessus de target_fps
    if (last_frame_duration < target_dt) {
        e_sleep_ms(target_dt - last_frame_duration);
        last_frame_duration = target_dt;
    }
    
    // restart the timer
    _e_start_timer();
    
    return FALSE;
}

//==============================================================================
// This function sets the target frames per second for the game rendering.
// fps: The desired frame rate per second.
//==============================================================================
void e_set_target_fps(const int16_t fps)
{
    if (fps < 0) {
        e_throw_error("fps needs to be a positive number");
    }
    target_fps = fps;
    target_dt = 1000 / fps;
}

//==============================================================================
// This function displays an error message on the TFT display and halts the system.
// message: The text string to display on the screen.
//==============================================================================
void e_throw_error(const char message[])
{
    e_fill_screen(BROWN);
    e_draw_text(message, 10, 30, Courier_New_Bold_8, WHITE, BLACK);
    while(1) {};
}

//==============================================================================
// Will exit the game the next time e_game_should_stop() is called
//==============================================================================
void e_exit_game(void)
{
    want_to_exit_game = TRUE;
}

//==============================================================================
// This function returns a random number between a min and a max value.
//==============================================================================
int16_t e_generate_rd_nb(int16_t min, int16_t max)
{
    uint16_t seed = (uint16_t)_e_get_timer_value();
    srand(seed);
    return min + rand() % (max - min + 1);
}

//==============================================================================
// This function creates a delay for a specified number of milliseconds.
// duration: The number of milliseconds to delay.
//==============================================================================
void e_sleep_ms(int16_t duration)
{
    // manage error
    if (duration < 0) {
        e_throw_error("error in e_sleep_ms");
    }

    for(int16_t i = 0; i < duration; i++){
        __delay_ms(1);
    }
}

//==============================================================================
// This function creates a delay for a specified number of microseconds.
// duration: The number of microseconds to delay.
//==============================================================================
void e_sleep_us(int16_t duration)
{
    // manage error
    if (duration < 0) {
        e_throw_error("error in e_sleep_us");
    }

    for(int16_t i = 0; i < duration; i++){
        __delay_us(1);
    }
}

//==============================================================================
// This function returns the last frame duration in seconds.
// Returns the time in seconds of the last frame.
//==============================================================================
float e_get_frame_time(void)
{
    return (float)last_frame_duration / 1000;
}

//==============================================================================
// This function turns on one of the LEDs depending of input "led".
//==============================================================================
void e_turn_on_debug_led(int16_t led)
{
    switch (led) {
        case DEBUG_LED_1:
            PORTBbits.RB5 = 1;
            break;
        case DEBUG_LED_2:
            PORTBbits.RB4 = 1;
            break;
        case DEBUG_LED_3:
            PORTBbits.RB3 = 1;
            break;
        case DEBUG_LED_4:
            PORTBbits.RB2 = 1;
            break;
        default:
            break;
    }
}

//==============================================================================
// This function turns off one of the LEDs depending of input "led".
//==============================================================================
void e_turn_off_debug_led(int16_t led)
{
    switch (led) {
        case DEBUG_LED_1:
            PORTBbits.RB5 = 0;
            break;
        case DEBUG_LED_2:
            PORTBbits.RB4 = 0;
            break;
        case DEBUG_LED_3:
            PORTBbits.RB3 = 0;
            break;
        case DEBUG_LED_4:
            PORTBbits.RB2 = 0;
            break;
        default:
            break;
    }
}

//==============================================================================
// This function toggles one of the LEDs depending of input "led".
//==============================================================================
void e_toggle_debug_led(int16_t led)
{
    switch (led) {
        case DEBUG_LED_1:
            if(PORTBbits.RB5 == 0){
                PORTBbits.RB5 = 1;
                break;
            }
            if(PORTBbits.RB5 == 1){
                PORTBbits.RB5 = 0;
                break;
            }        
        case DEBUG_LED_2:
            if(PORTBbits.RB4 == 0){
                PORTBbits.RB4 = 1;
                break;
            }
            if(PORTBbits.RB4 == 1){
                PORTBbits.RB4 = 0;
                break;
            }   
        case DEBUG_LED_3:
            if(PORTBbits.RB3 == 0){
                PORTBbits.RB3 = 1;
                break;
            }
            if(PORTBbits.RB3 == 1){
                PORTBbits.RB3 = 0;
                break;
            }   
        case DEBUG_LED_4:
            if(PORTBbits.RB2 == 0){
                PORTBbits.RB2 = 1;
                break;
            }
            if(PORTBbits.RB2 == 1){
                PORTBbits.RB2 = 0;
                break;
            }   
        default:
            break;
    }
}

//==============================================================================
// This function writes data into the EEPROM
//==============================================================================
void e_write_eeprom(uint8_t address, uint8_t data){
    EEADR = address;
    EEDATA = data;
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;
    EECON1bits.WREN = 1;
    INTCONbits.GIE = 0;
    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1;
    while (EECON1bits.WR);
    INTCONbits.GIE = 1;
    EECON1bits.WREN =0;
}

//==============================================================================
// This function read data from the EEPROM
//==============================================================================
uint8_t e_read_eeprom(uint8_t address){  
    uint8_t data;
    EEADR = address;       // Adresse de m�moire � lire
    EECON1bits.EEPGD = 0;  // Pointez sur la m�moire DATA
    EECON1bits.CFGS = 0;   // Acc�der � l'EEPROM
    EECON1bits.RD = 1;     // Lecture de l'EEPROM
    data = EEDATA;         // Lire la valeur dans le registre EEDATA
    return data;
}