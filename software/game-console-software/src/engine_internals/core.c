/*
 * File:   engine.c
 * Author: eztaah, MatteoPerez, MokhmadGUIRIEV and 21KEBY
 */

#include "bit_settings.h"
#include "internal.h"
#include "../engine.h"


//                              GLOBAL VARIABLES
//==============================================================================
// Internal variable and functions definitions
//==============================================================================
int16_t running = TRUE;
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
    T0CONbits.TMR0ON = 0;     // On arrete le timer
    
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
// This function is the interrupt service routine for TIMER0 overflows.
//==============================================================================
void __interrupt() timer0_ISR(void)
{
    if (INTCONbits.TMR0IF == 1){     // v?rifie si l'interruption est bien provoqu?e par le timer0
        e_exit_with_error("Le timer a overflow, ce n'est pas normal");
        INTCONbits.TMR0IF = 0;      // flag d'interruption effac�
    }
}


//                           USEFUL FUNCTIONS
//==============================================================================
// This function displays an error message on the TFT display and halts the system.
// message: The text string to display on the TFT screen.
//==============================================================================
void e_exit_with_error(const char message[])
{
    e_fill_screen(BROWN);
    //e_draw_const_text(message, 10, 10, WHITE, BLACK);
    while(1) {};
}

//==============================================================================
// This function converts an int16_t value to uint16_t safely, showing an error if negative.
// value: The signed integer value to convert.
// error_token: Identifier for the specific error case.
//==============================================================================
uint16_t e_safe_convert(int16_t value, char error_token[]) {
    if (value < 0) {
        e_exit_with_error("E: ");
    }
    return (uint16_t)value;
}


//                             CORE FUNCTIONS
//==============================================================================
// This function initializes the hardware and settings for the game console.
//==============================================================================
void e_init_game_console(void)
{    
    // Init screen 
    _e_init_screen();
    
    // init font
    e_set_font(Courier_New_Bold_20);
    
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
int16_t e_game_should_stop()
{
    // CLOSE THE GAME IF NEEDED
    if (running == FALSE) {
        return TRUE;
    }
    
    // GESTION DES FPS
    // R�cuperer la valeur du timer
    last_frame_duration = _e_get_timer_value();
    
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
    target_fps = fps;
    target_dt = 1000 / fps;
}

//==============================================================================
// This function creates a delay for a specified number of milliseconds.
// duration: The number of milliseconds to delay.
//==============================================================================
void e_sleep_ms(int16_t duration)
{
    // manage error
    if (duration < 0) {
        e_exit_with_error("error in e_sleep_ms");
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
        e_exit_with_error("error in e_sleep_us");
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
// This function returns a random number between a min and a max value.
//==============================================================================
int16_t e_generate_rd_nb(int16_t min, int16_t max)
{
    uint16_t seed = (uint16_t)_e_get_timer_value();
    srand(seed);
    return min + rand() % (max - min + 1);
}

//==============================================================================
// This function turns on one of the LEDs depending of input "led".
//==============================================================================
void e_turn_on_led(int16_t led)
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
void e_turn_off_led(int16_t led)
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
void e_toggle_led(int16_t led)
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
// 
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
// 
//==============================================================================
uint8_t read_EEPROM(uint8_t address){  
    uint8_t data;
    EEADR = address;       // Adresse de m�moire � lire
    EECON1bits.EEPGD = 0;  // Pointez sur la m�moire DATA
    EECON1bits.CFGS = 0;   // Acc�der � l'EEPROM
    EECON1bits.RD = 1;     // Lecture de l'EEPROM
    data = EEDATA;         // Lire la valeur dans le registre EEDATA
    return data;
}