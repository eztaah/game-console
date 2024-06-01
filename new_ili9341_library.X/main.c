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
    draw_const_text("PIC18 GAME CONSOLE", 42, 80, BLACK, WHITE);
    play_game_over();
    
    draw_const_text("loading", 90, 200, WHITE, BLACK);
    sleep_ms(500);

    draw_const_text("loading.", 90, 200, WHITE, BLACK);
    sleep_ms(500);

    draw_const_text("loading..", 90, 200, WHITE, BLACK);
    sleep_ms(500);

    draw_const_text("loading...", 90, 200, WHITE, BLACK);
    sleep_ms(500);
    draw_rectangle(90, 200, 150, 30, BLACK);
    
    fill_screen(BLACK);
    
//    while(1) {
//        int16_t variable1 = PORTBbits.RB0;
//        if (is_button_pressed(BUTTON_UP)) {
//            fill_screen(BLUE);
//        }
//        else if (is_button_pressed(BUTTON_DOWN)) {
//            fill_screen(GREEN);
//        }
//        else if (is_button_pressed(BUTTON_RIGHT)) {
//            fill_screen(MAGENTA);
//        }
//        else if (is_button_pressed(BUTTON_LEFT)) {
//            fill_screen(BROWN);
//        }
//        else if (is_button_pressed(BUTTON_A)) {
//            fill_screen(RED);
//        }
//        else if (is_button_pressed(BUTTON_B)) {
//            fill_screen(LIGHTBLUE);
//        }
//        else if (is_button_pressed(BUTTON_HOME)) {
//            fill_screen(CYAN);
//        }
//        else {
//            fill_screen(BLACK);
//        }
//    }
    
    int16_t cursor_position = 0;
    int16_t last_button_pressed = -1;
    
    while (1) {
        if (is_button_pressed(BUTTON_UP)) {
            if (cursor_position == 0) {
                cursor_position = 2;
            } else {
                cursor_position--;
            }
            last_button_pressed = BUTTON_UP;
        }

        if (is_button_pressed(BUTTON_DOWN)) {
            if (cursor_position == 2) {
                cursor_position = 0;
            } else {
                cursor_position++;
            }
            last_button_pressed = BUTTON_DOWN;
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
        draw_const_text("Choose a game : ", 50, 20, WHITE, BLACK);
        
        if (cursor_position == 0) {
           draw_const_text("pong", 110, 70, BLACK, WHITE);
           draw_const_text("tetris", 110, 100, WHITE, BLACK);
           draw_const_text("snake", 110, 130, WHITE, BLACK);
        }

        else if (cursor_position == 1) {
           draw_const_text("pong", 110, 70, WHITE, BLACK);
           draw_const_text("tetris", 110, 100, BLACK, WHITE);
           draw_const_text("snake", 110, 130, WHITE, BLACK);
        }

        else if (cursor_position == 2) {
           draw_const_text("pong", 110, 70, WHITE, BLACK);
           draw_const_text("tetris", 110, 100, WHITE, BLACK);
           draw_const_text("snake", 110, 130, BLACK, WHITE);
        }    
        
        
        if (last_button_pressed == BUTTON_DOWN) {
            while(is_button_pressed(BUTTON_DOWN)) {}
            last_button_pressed = -1;
        }
        else if (last_button_pressed == BUTTON_UP) {
            while(is_button_pressed(BUTTON_UP)) {}
            last_button_pressed = -1;
        }
    }

      
}
