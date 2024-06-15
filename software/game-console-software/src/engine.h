/*
 * File:   engine.h
 * Author: eztaah, MatteoPerez, MokhmadGUIRIEV and 21KEBY
 */

#ifndef ENGINE_H
#define	ENGINE_H

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>

#include "engine_internals/bitmap.h"


//==============================================================================
// DEFINES
//==============================================================================
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define TRUE 1
#define FALSE 0

// colors
#define BLACK              0b0000000000000000
#define BRIGHTBLUE         0b0000000000011111
#define BRIGHTGREEN        0b0000011111100000
#define BRIGHTCYAN         0b0000011111111111
#define BRIGHTRED          0b1111100000000000
#define BRIGHTMAGENTA      0b1111100000011111
#define BRIGHTYELLOW       0b1111111111100000
#define BLUE               0b0000000000010000
#define GREEN              0b0000010000000000
#define CYAN               0b0000010000010000
#define RED                0b1000000000000000
#define MAGENTA            0b1000000000010000
#define BROWN              0b1111110000000000
#define LIGHTGRAY          0b1000010000010000
#define DARKGRAY           0b0100001000001000
#define LIGHTBLUE          0b1000010000011111
#define LIGHTGREEN         0b1000011111110000
#define LIGHTCYAN          0b1000011111111111
#define LIGHTRED           0b1111110000010000
#define LIGHTMAGENTA       0b1111110000011111
#define YELLOW             0b1111111111110000
#define WHITE              0b1111111111111111
#define GRAY0       	   0b1110011100011100
#define GRAY1         	   0b1100011000011000   
#define GRAY2              0b1010010100010100   
#define GRAY3              0b1000010000010000
#define GRAY4              0b0110001100001100
#define GRAY5              0b0100001000001000
#define GRAY6	           0b0010000100000100

// vectors
typedef struct Vector2i {
    int16_t x;
    int16_t y;
} Vector2i;

typedef struct Vector2u {
    uint16_t x;
    uint16_t y;
} Vector2u;

typedef struct Vector2f {
    float x;
    float y;
} Vector2f;

// buttons
typedef enum {
    BUTTON_UP,
    BUTTON_DOWN,
    BUTTON_RIGHT,
    BUTTON_LEFT,
    BUTTON_A,
    BUTTON_B,
    BUTTON_HOME
} Button;

// fonts
typedef enum {
    Courier_New_Bold_8,
    Courier_New_Bold_10,
    Courier_New_Bold_14,
    Courier_New_Bold_20,
} Font;

// LEDs
typedef enum {
    DEBUG_LED_1,
    DEBUG_LED_2,
    DEBUG_LED_3,
    DEBUG_LED_4
} LEDs;

// EEPROM adresses
#define EEPROM_ADRESS_PONG 0x0
#define EEPROM_ADRESS_TETRIS 0x2
#define EEPROM_ADRESS_SNAKE 0x4

//==============================================================================
// USEFUL FUNCTIONS 
//==============================================================================
void     e_exit_with_error(const char message[]);           // Display error message on screen and halt execution
uint16_t e_safe_convert(int16_t value, char error_token[]); // Safely convert int16_t to uint16_t, display error if negative


//==============================================================================
// CORE FUNCTIONS (core.c)
//==============================================================================
// base
void    e_init_game_console(void);              // Initialize the game console
int16_t e_game_should_stop(void);               // Check if home_button is pressed
void    e_set_target_fps(const int16_t fps);    // Set target FPS (maximum)
void    e_exit_game(void);                         // Will exit the game next time e_game_should_stop is called
// time
void    e_sleep_ms(int16_t duration);           // Wait for X ms
void    e_sleep_us(int16_t duration);           // Wait for X us
float   e_get_frame_time(void);                 // Get time in seconds for last frame drawn

int16_t e_generate_rd_nb(int16_t min, int16_t max);         // Generate random number

void e_turn_on_led(int16_t led);    // Lights on one of the LEDs
void e_turn_off_led(int16_t led);   // Lights off one of the LEDs
void e_toggle_led(int16_t led);     // Toggles one of the LEDs

void    e_write_eeprom(uint8_t adress, uint8_t data);
uint8_t e_read_eeprom(uint8_t adress);

//==============================================================================
// INPUT MANAGEMENT FUNCTIONS  (inputs.c)
//==============================================================================
int8_t e_is_button_down(int8_t button);            // Check if a button is being pressed
int8_t e_is_button_pressed(int8_t button);

//==============================================================================
// RENDERING FUNCTIONS  (rendering.c)
//==============================================================================
void e_fill_screen(uint16_t color);             // Fill the screen with a specific color
//shape
void e_draw_rectangle(int16_t pos_x, int16_t pos_y, int16_t width, int16_t height, uint16_t color);       // Draw a color-filled rectangle
void e_draw_moving_rectangle(int16_t new_pos_x, int16_t new_pos_y, int16_t old_pos_x, int16_t old_pos_y,
                           int16_t width, int16_t height, uint16_t color, uint16_t background_color);   // Only draw the moving part of the rectangle
// text
void e_set_font(int16_t font);
void e_draw_fps(int16_t pos_x, int16_t pos_y);  // Draw current FPS
void e_draw_const_text(const char *text, int16_t x, int16_t y, uint16_t color1, uint16_t color2);   // Draw const text (using chosen font)
void e_draw_text(char *text, int16_t x, int16_t y, uint16_t color1, uint16_t color2);               // Draw text (using chosen font)

// icon
void e_draw_icon(const uint16_t *buffer, int16_t x, int16_t y, 
                 uint16_t original_width, uint16_t original_height, uint16_t zoom);

//==============================================================================
// AUDIO FUNCTIONS   (audio.c)
//==============================================================================
void e_play_A3(int16_t duration);         // Play note A3 (La 3) for specified duration in ms
void e_play_B3(int16_t duration);         // Play note B3 (Si 3) for specified duration in ms
void e_play_C4(int16_t duration);         // Play note C4 (Do 4) for specified duration in ms
void e_play_D4(int16_t duration);         // Play note D4 (R� 4) for specified duration in ms
void e_play_D_4(int16_t duration);        // Play note D#4 (R� di�se 4) for specified duration in ms
void e_play_E4(int16_t duration);         // Play note E4 (Mi 4) for specified duration in ms
void e_play_G4(int16_t duration);         // Play note G4 (Sol 4) for specified duration in ms
void e_play_A_4(int16_t duration);        // Play note A#4 (La di�se 4) for specified duration in ms

#endif

