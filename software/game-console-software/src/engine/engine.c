/*
 * File:   engine.c
 * Author: eztaah, MatteoPerez, MokhmadGUIRIEV and 21KEBY
 */

#include "bit_settings.h"
#include "ili9341.h"
#include "engine.h"
#include "font.h"


//                              INTERNALS
//==============================================================================
// Internal variable and functions definitions
//==============================================================================
int16_t running = TRUE;
int16_t target_fps = 60;
int16_t target_dt = 17;
int16_t last_frame_duration = 0;
char last_frame_duration_text[30] = {0};
char fps_text[30] = {0};

void _e_init_timer(void);
void _e_start_timer(void);
int16_t _e_get_timer_value(void);
void _e_stop_buzzer(void);
void _e_init_buttons(void);

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
    TFT_FillScreen(BROWN);
    TFT_ConstText(message, 10, 10, WHITE, BLACK);
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
    OSCCON = 0b01110000;
    OSCTUNEbits.PLLEN = 1; // turn on the PLL 64 MHz
    ANSELA = 0; ANSELB = 0; ANSELC = 0; ANSELD = 0; ANSELE = 0;
    PORTA  = 0; PORTB  = 0; PORTC  = 0; PORTD  = 0; PORTE  = 0;
    TRISA  = 0; TRISB  = 0; TRISC  = 0; TRISD  = 0; TRISE  = 0;
    SPI1_Close();
    SPI1_Init();
    TFT_Init();
    
    // init font
    e_set_font(Courier_New_Bold_20);
    
    // Init timer0
    _e_init_timer();
    
    // init buzzer 
    e_init_buzzer();
    
    // Init buttons
    _e_init_buttons();
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
// This function checks if a specified button is currently pressed.
// button: The button identifier to check.
// Returns 1 if pressed, 0 if not, -1 if an invalid button is checked.
//==============================================================================
int16_t e_is_button_pressed(int16_t button)
{
    switch (button) {
        case BUTTON_UP:
            return !PORTBbits.RB2;     // 1 si press�, et 0 sinon
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


//                          REDNERING FUNCTIONS
//==============================================================================
// This function fills the entire TFT screen with a single color.
// color: The color to fill the screen with.
//==============================================================================
void e_fill_screen(uint16_t color)
{
    TFT_FillScreen(color);
}

//==============================================================================
// This function draws a rectangle on the TFT display.
// pos_x: The x-coordinate of the top-left corner of the rectangle.
// pos_y: The y-coordinate of the top-left corner of the rectangle.
// width: The width of the rectangle.
// height: The height of the rectangle.
// color: The color of the rectangle.
//==============================================================================
void e_draw_rectangle(int16_t pos_x, int16_t pos_y, int16_t width, int16_t height, uint16_t color)
{
    uint16_t x1 = e_safe_convert(pos_x, "1");
    uint16_t y1 = e_safe_convert(pos_y, "2");
    uint16_t x2 = e_safe_convert(pos_x + width, "3");
    uint16_t y2 = e_safe_convert(pos_y + height, "4");

    TFT_Box(x1, y1, x2, y2, color);
}

//==============================================================================
// This function updates the position of a rectangle on the TFT display, clearing the old position.
// new_pos_x: The new x-coordinate of the rectangle.
// new_pos_y: The new y-coordinate of the rectangle.
// old_pos_x: The old x-coordinate of the rectangle.
// old_pos_y: The old y-coordinate of the rectangle.
// width: The width of the rectangle.
// height: The height of the rectangle.
// color: The color of the rectangle.
// background_color: The background color to fill the old rectangle area.
//==============================================================================
void e_draw_moving_rectangle(int16_t new_pos_x, int16_t new_pos_y, int16_t old_pos_x, int16_t old_pos_y,
                           int16_t width, int16_t height, uint16_t color, uint16_t background_color) 
{
    // Calculate the old and new rectangle boundaries
    uint16_t old_left = e_safe_convert(old_pos_x, "1");
    uint16_t old_top = e_safe_convert(old_pos_y, "2");
    uint16_t old_right = e_safe_convert(old_pos_x + width - 1, "3");
    uint16_t old_bottom = e_safe_convert(old_pos_y + height - 1, "4");

    uint16_t new_left = e_safe_convert(new_pos_x, "5");
    uint16_t new_top = e_safe_convert(new_pos_y, "6");
    uint16_t new_right = e_safe_convert(new_pos_x + width - 1, "7");
    uint16_t new_bottom = e_safe_convert(new_pos_y + height - 1, "8");

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

//==============================================================================
// ????
//==============================================================================
void e_set_font(int16_t font)
{
    switch (font) {
        case Courier_New_Bold_8:
            TFT_SetFont(Courier_New_Bold_8_char, 1);
            break;
        case Courier_New_Bold_10:
            TFT_SetFont(Courier_New_Bold_10_char, 1);
            break;
        case Courier_New_Bold_14:
            TFT_SetFont(Courier_New_Bold_14_char, 1);
            break;
        case Courier_New_Bold_20:
            TFT_SetFont(Courier_New_Bold_20_char, 1);
            break;
    }
}

//==============================================================================
// This function displays the current frame rate and duration of the last frame on the TFT display.
// pos_x: The x-coordinate for text placement.
// pos_y: The y-coordinate for text placement.
//==============================================================================
void e_draw_fps(int16_t pos_x, int16_t pos_y)
{
    sprintf(last_frame_duration_text, "dt: %d", last_frame_duration);
    sprintf(fps_text, "fps: %d", (uint16_t)((1.f/last_frame_duration)*1000));
    
    e_draw_text(last_frame_duration_text, pos_x, pos_y, GREEN, BLACK);
    e_draw_text(fps_text, pos_x, pos_y + 30, GREEN, BLACK);
}

//==============================================================================
// This function displays constant text at a specified position on the TFT display.
// text: The text to display.
// x: The x-coordinate for text placement.
// y: The y-coordinate for text placement.
// color1: The foreground color of the text.
// color2: The background color behind the text.
//==============================================================================
void e_draw_const_text(const char *text, int16_t x, int16_t y, uint16_t color1, uint16_t color2)
{
    //  void TFT_Text(char *buffer, uint16_t x, uint16_t y, uint16_t color1, uint16_t color2)
    uint16_t x1 = e_safe_convert(x, "67");
    uint16_t y1 = e_safe_convert(y, "68");
    TFT_ConstText(text, x1, y1, color1, color2);
}

//==============================================================================
// This function displays variable text at a specified position on the TFT display.
// text: The text to display.
// x: The x-coordinate for text placement.
// y: The y-coordinate for text placement.
// color1: The foreground color of the text.
// color2: The background color behind the text.
//==============================================================================
void e_draw_text(char *text, int16_t x, int16_t y, uint16_t color1, uint16_t color2)
{
    //  void TFT_Text(char *buffer, uint16_t x, uint16_t y, uint16_t color1, uint16_t color2)
    uint16_t x1 = e_safe_convert(x, "67");
    uint16_t y1 = e_safe_convert(y, "68");
    TFT_Text(text, x1, y1, color1, color2);
}



//                             AUDIO FUNCTIONS
//==============================================================================
// This function initializes the buzzer for audio output by setting up PWM.
// Configures PWM settings and turns the buzzer off initially.
//==============================================================================
void e_init_buzzer(void)
{
    TRISCbits.TRISC1 = 0;   // RC1 est la sortie PWM, sur laquelle est connect? le buzzer
    T2CON = 0b00000110;     // Postscaler = 1, Timer ON, Prescaler = 16
    CCP2CON = 0b00001100;   // mode PWM, avec 1 seule sortie sur RC1
    
    _e_stop_buzzer();
}

//==============================================================================
// This function plays the musical note A3 (La 3) on the buzzer for a specified duration.
// duration: The duration in milliseconds for which to play the note.
//==============================================================================
void e_play_A3(int16_t duration)
{
    PR2 = 180;
    CCPR2L = 90;
    e_sleep_ms(duration);
    _e_stop_buzzer();
}

//==============================================================================
// This function plays the musical note B3 (Si 3) on the buzzer for a specified duration.
// duration: The duration in milliseconds for which to play the note.
//==============================================================================
void e_play_B3(int16_t duration)
{
    PR2 = 160;
    CCPR2L = 80;
    e_sleep_ms(duration);
    _e_stop_buzzer();
}

//==============================================================================
// This function plays the musical note C4 (Do 4) on the buzzer for a specified duration.
// duration: The duration in milliseconds for which to play the note.
//==============================================================================
void e_play_C4(int16_t duration)
{
    PR2 = 151;
    CCPR2L = 75;
    e_sleep_ms(duration);
    _e_stop_buzzer();
}

//==============================================================================
// This function plays the musical note D4 (R� 4) on the buzzer for a specified duration.
// duration: The duration in milliseconds for which to play the note.
//==============================================================================
void e_play_D4(int16_t duration)
{
    PR2 = 135;
    CCPR2L = 67;
    e_sleep_ms(duration);
    _e_stop_buzzer();
}

//==============================================================================
// This function plays the musical note D#4 (R� di�se 4) on the buzzer for a specified duration.
// duration: The duration in milliseconds for which to play the note.
//==============================================================================
void e_play_D_4(int16_t duration)
{
    PR2 = 127;
    CCPR2L = 63;
    e_sleep_ms(duration);
    _e_stop_buzzer();
}

//==============================================================================
// This function plays the musical note E4 (Mi 4) on the buzzer for a specified duration.
// duration: The duration in milliseconds for which to play the note.
//==============================================================================
void e_play_E4(int16_t duration)
{
    PR2 = 120;
    CCPR2L = 60;
    e_sleep_ms(duration);
    _e_stop_buzzer();
}

//==============================================================================
// This function plays the musical note G4 (Sol 4) on the buzzer for a specified duration.
// duration: The duration in milliseconds for which to play the note.
//==============================================================================
void e_play_G4(int16_t duration)
{
    PR2 = 101;
    CCPR2L = 50;
    e_sleep_ms(duration);
    _e_stop_buzzer();
}

//==============================================================================
// This function plays the musical note A#4 (La di�se 4) on the buzzer for a specified duration.
// duration: The duration in milliseconds for which to play the note.
//==============================================================================
void e_play_A_4(int16_t duration)
{
    PR2 = 86;
    CCPR2L = 43;
    e_sleep_ms(duration);
    _e_stop_buzzer();
}


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
// This function stops the buzzer by turning off the PWM output.
//==============================================================================
void _e_stop_buzzer(void)
{
    PR2 = 0;
    CCPR2L = 0;
}

//==============================================================================
// 
//==============================================================================
void _e_init_buttons(void)
{
    TRISAbits.TRISA4 = 1;   // LEFT
    PORTAbits.RA4 = 0;
    
    TRISAbits.TRISA5 = 1;   // UP
    PORTAbits.RA5 = 0;
    
    TRISAbits.TRISA7 = 1;   // RIGHT
    PORTAbits.RA7 = 0;
    
    TRISAbits.TRISA6 = 1;   // LEFT
    PORTAbits.RA6 = 0;
    
    TRISBbits.TRISB0 = 1;   // A
    PORTBbits.RB0 = 0;
    
    TRISDbits.TRISD5 = 1;   // B
    PORTDbits.RD5 = 0;
    
    TRISDbits.TRISD2 = 1;   // HOME
    PORTDbits.RD2 = 0;   
}