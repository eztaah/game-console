/*
 * File:   main.c
 * Author:  
 */

#include "engine.h"

#include "pong.h"


void main(void) {
    init_game_console();
    set_target_fps(20);
    
    fill_screen(BLACK);
    
    int16_t cursor_position = 0;
    
    while (1) {
        if (is_button_pressed(BUTTON_DOWN)) {
            if (cursor_position == 0) {
                cursor_position = 2;
            } else {
                cursor_position--;
            }
        }

        if (is_button_pressed(BUTTON_UP)) {
            if (cursor_position == 2) {
                cursor_position = 0;
            } else {
                cursor_position++;
            }
        }


        if (is_button_pressed(BUTTON_A)) {
            switch (cursor_position) {
                case 0:
                    run_pong_game();
                    fill_screen(BLACK);
                    cursor_position = 0;
                    break;
                case 1:
                    fill_screen(BLUE);
                    sleep_ms(2000);
                    fill_screen(BLACK);
                    cursor_position = 0;
                    break;
                case 2:
                    fill_screen(MAGENTA);
                    sleep_ms(2000);
                    fill_screen(BLACK);
                    cursor_position = 0;
                    break;
            }
        }


        // DRAW
        if (cursor_position == 0) {
           draw_const_text("pong", 50, 20, BLACK, WHITE);
           draw_const_text("tetris", 50, 50, WHITE, BLACK);
           draw_const_text("snake", 50, 80, WHITE, BLACK);
        }

        else if (cursor_position == 1) {
           draw_const_text("pong", 50, 20, WHITE, BLACK);
           draw_const_text("tetris", 50, 50, BLACK, WHITE);
           draw_const_text("snake", 50, 80, WHITE, BLACK);
        }

        else if (cursor_position == 2) {
           draw_const_text("pong", 50, 20, WHITE, BLACK);
           draw_const_text("tetris", 50, 50, WHITE, BLACK);
           draw_const_text("snake", 50, 80, BLACK, WHITE);
        }    
    }

      
}
