/*
 * File:   main.c
 * Author:  
 */

#include "engine.h"

int16_t cursor_position;

void run_pong_game(void);
void run_tetris_game(void);
void run_snake_game(void);


void play_init_music(void)
{
    e_play_G4(400);
    e_play_G4(400);
    e_play_G4(400);
    e_play_D_4(150);
    e_play_A_4(100);
    e_play_G4(400);
    e_play_D_4(150);
    e_play_A_4(100);
    e_play_G4(400);
}

void init_menu()
{
    e_set_target_fps(20);
    e_set_font(Courier_New_Bold_20);
    cursor_position = 0;
    e_fill_screen(BLACK);
}

void main(void) {
    e_init_game_console();
    init_menu();
    
    e_draw_const_text("PIC18 GAME CONSOLE", 42, 80, BLACK, WHITE);
    e_fill_screen(BLACK);
    
    while (1) {
        if (e_is_button_pressed(BUTTON_UP)) {
            if (cursor_position == 0) {
                cursor_position = 2;
            } else {
                cursor_position--;
            }
        }

        if (e_is_button_pressed(BUTTON_DOWN)) {
            if (cursor_position == 2) {
                cursor_position = 0;
            } else {
                cursor_position++;
            }
        }

        if (e_is_button_pressed(BUTTON_A)) {
            switch (cursor_position) {
                case 0:
                    run_pong_game();
                    init_menu();
                    break;
                case 1:
                    run_tetris_game();
                    init_menu();
                    break;
                case 2:
                    run_snake_game();
                    init_menu();
                    break;
            }
        }
        
        // DRAW
        e_draw_const_text("Choose a game : ", 50, 20, WHITE, BLACK);
        
        if (cursor_position == 0) {
           e_draw_const_text("pong", 110, 70, BLACK, WHITE);
           e_draw_const_text("tetris", 110, 100, WHITE, BLACK);
           e_draw_const_text("snake", 110, 130, WHITE, BLACK);
        }

        else if (cursor_position == 1) {
           e_draw_const_text("pong", 110, 70, WHITE, BLACK);
           e_draw_const_text("tetris", 110, 100, BLACK, WHITE);
           e_draw_const_text("snake", 110, 130, WHITE, BLACK);
        }

        else if (cursor_position == 2) {
           e_draw_const_text("pong", 110, 70, WHITE, BLACK);
           e_draw_const_text("tetris", 110, 100, WHITE, BLACK);
           e_draw_const_text("snake", 110, 130, BLACK, WHITE);
        }    
    }   
}
