/*
 * File:   main.c
 * Author:  
 */

#include "engine.h"

#include "pong.h"


void main(void) {
    init_game_console();
    set_target_fps(20);
    
    //play_init_console_sound();
    fill_screen(BLACK);
    draw_const_text("pong game is starting...");
    sleep_ms(2000);
    
    
    run_pong_game();
    
    while(1) {};
}
