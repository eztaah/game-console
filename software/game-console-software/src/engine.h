/*
 * File:   engine.h
 * Author: eztaah, MatteoPerez, MokhmadGUIRIEV and 21KEBY
 */

#ifndef ENGINE_H
#define	ENGINE_H

#include <xc.h>
#include <stdio.h>    // > sprintf()

//==============================================================================
// DEFINITIONS
//==============================================================================
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define TRUE 1
#define FALSE 0

// colors
#define BLACK              0b0000000000000000
#define BLUE               0b0000000000010000
#define GREEN              0b0000010000000000
#define RED                0b1000000000000000
#define YELLOW             0b1111111111110000
#define WHITE              0b1111111111111111
#define MAGENTA            0b1000000000010000
#define BROWN              0b1111110000000000
#define CYAN               0b0000010000010000

// EEPROM adresses
#define EEPROM_ADRESS_PONG 0x0
#define EEPROM_ADRESS_TETRIS 0x2
#define EEPROM_ADRESS_SNAKE 0x4

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

// fonts
extern const uint8_t Courier_New_Bold_8[];
extern const uint8_t Courier_New_Bold_10[];
extern const uint8_t Courier_New_Bold_14[];
extern const uint8_t Courier_New_Bold_20[];

// icons
extern const uint16_t controller_image[];

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

// LEDs
typedef enum {
    DEBUG_LED_1,
    DEBUG_LED_2,
    DEBUG_LED_3,
    DEBUG_LED_4
} LEDs;



//==============================================================================
// CORE FUNCTIONS (core.c)
//==============================================================================
// base
void    e_init_game_console(uint16_t initial_screen_color); // Initialize game console
uint8_t e_game_should_stop(void);                      // Check if the game should stop
void    e_set_target_fps(const int16_t fps);           // Set target FPS (maximum)
void    e_throw_error(const char message[]);           // Display error and halt
void    e_exit_game(void);                             // Will exit the game next frame
int16_t e_generate_rd_nb(int16_t min, int16_t max);    // Generate random number
// time
void    e_sleep_ms(int16_t duration);                  // Wait for X ms
void    e_sleep_us(int16_t duration);                  // Wait for X us
float   e_get_frame_time(void);                        // Get last frame duration
// LEDs
void e_turn_on_debug_led(int16_t led);                 // Turn on specific LED
void e_turn_off_debug_led(int16_t led);                // Turn off specific LED
void e_toggle_debug_led(int16_t led);                  // Toggle specific LED
// eeprom
void    e_write_eeprom(uint8_t adress, uint8_t data);  // Write data to EEPROM
uint8_t e_read_eeprom(uint8_t adress);                 // Read data from EEPROM

//==============================================================================
// INPUT MANAGEMENT FUNCTIONS  (inputs.c)
//==============================================================================
int8_t e_is_button_down(int8_t button);     // Check if button is down
int8_t e_is_button_pressed(int8_t button);  // Check if button is pressed

//==============================================================================
// RENDERING FUNCTIONS  (rendering.c)
//==============================================================================
// base
uint16_t e_rgb_convert(uint16_t red, uint16_t green, uint16_t blue); // Convert RGB to color format
void e_fill_screen(uint16_t color);           // Fill the screen with a specific color
//shape
void e_draw_pixel(int16_t pos_x, int16_t pos_y, uint16_t color);   // Draw pixel at specific position
void e_draw_rectangle(int16_t pos_x, int16_t pos_y, int16_t width, int16_t height, uint16_t color); // Draw a color-filled rectangle
// text
void e_draw_text(const char *buffer, int16_t pos_x, int16_t pos_y, const uint8_t* font, uint16_t color, uint16_t background_color);  // Draw text
void e_draw_fps(int16_t pos_x, int16_t pos_y, const uint8_t* font);  // Draw current FPS
// icon
void e_draw_icon(const uint16_t *buffer, int16_t pos_x, int16_t pos_y, int16_t original_width, int16_t original_height, uint16_t zoom); // Draw icon

//==============================================================================
// AUDIO FUNCTIONS   (audio.c)
//==============================================================================
void e_play_A3(int16_t duration);         // Play note A3 (La 3)
void e_play_B3(int16_t duration);         // Play note B3 (Si 3)
void e_play_C4(int16_t duration);         // Play note C4 (Do 4)
void e_play_D4(int16_t duration);         // Play note D4 (R� 4)
void e_play_D_4(int16_t duration);        // Play note D#4 (R� di�se 4)
void e_play_E4(int16_t duration);         // Play note E4 (Mi 4)
void e_play_G4(int16_t duration);         // Play note G4 (Sol 4)
void e_play_A_4(int16_t duration);        // Play note A#4 (La di�se 4)

//==============================================================================
// LORA FUNCTIONS   (lora.c)
//==============================================================================
void send_text(uint8_t txMsg[]);
void send_score(int score);
void send_best_score(uint8_t address_best_score);

#endif
