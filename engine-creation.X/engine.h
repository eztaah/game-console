#ifndef ENGINE_H
#define	ENGINE_H

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320

// COLORS
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


// ---------- GLOABAL VARIABLES ----------
extern int16_t target_fps;


// ---------- STRUCTS ----------
typedef struct Vector2i {
    int16_t x;
    int16_t y;
} Vector2i;

typedef struct Vector2u {
    uint16_t x;
    uint16_t y;
} Vector2u;


// ---------- CORE ----------
void e_init_game_console(void);
void e_set_target_fps(const int16_t fps);
void mystery_function(void);

void e_sleep_ms(int16_t duration);


// ---------- RENDERING ----------
void e_fill_screen(uint16_t color);
void e_draw_rectangle(int16_t old_left, int16_t old_top, int16_t old_right, int16_t old_bottom,
                    int16_t new_left, int16_t new_top, int16_t new_right, int16_t new_bottom,
                    int16_t clear_color, int16_t fill_color);
void e_draw_fps(int16_t pos_x, int16_t pos_y);

// text
void e_draw_text(char *buffer, uint16_t x, uint16_t y, uint16_t color1, uint16_t color2);
void e_draw_const_text(const char *buffer, uint16_t x, uint16_t y, uint16_t color1, uint16_t color2);


// ---------- AUDIO ---------- 
void e_init_buzzer(void);
void play_impact_sound(void);
void _e_stop_buzzer(void);

#endif

