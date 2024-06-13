/*
 * File:   main.c
 * Author:  
 */



#include "engine.h"
#include "../src/games/pong.h"
#include "../src/games/tetris.h"
#include "../src/games/snake.h"


void play_init_music(void){
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

void main(void) {
    e_init_game_console();
    e_set_target_fps(20);
    
    // init font
    e_set_font(Courier_New_Bold_20);
    
    e_fill_screen(BLACK);
    
    e_draw_const_text("PIC18 GAME CONSOLE", 42, 80, BLACK, WHITE);
    e_fill_screen(BLACK);
    
    int16_t cursor_position = 0;
    int16_t last_button_pressed = -1;
    
    while (1) {
        if (e_is_button_down(BUTTON_UP)) {
            if (cursor_position == 0) {
                cursor_position = 2;
            } else {
                cursor_position--;
            }
            last_button_pressed = BUTTON_UP;
        }

        if (e_is_button_down(BUTTON_DOWN)) {
            if (cursor_position == 2) {
                cursor_position = 0;
            } else {
                cursor_position++;
            }
            last_button_pressed = BUTTON_DOWN;
        }


        if (e_is_button_down(BUTTON_A)) {
            switch (cursor_position) {
                case 0:
                    run_pong_game();
                    e_fill_screen(BLACK);
                    cursor_position = 0;
                    break;
                case 1:
                    run_tetris_game();
                    e_fill_screen(BLACK);
                    cursor_position = 0;
                    break;
                case 2:
                    run_snake_game();
                    e_fill_screen(BLACK);
                    cursor_position = 0;
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
        
        
        if (last_button_pressed == BUTTON_DOWN) {
            while(e_is_button_down(BUTTON_DOWN)) {}
            last_button_pressed = -1;
        }
        else if (last_button_pressed == BUTTON_UP) {
            while(e_is_button_down(BUTTON_UP)) {}
            last_button_pressed = -1;
        }
    }

      
}
