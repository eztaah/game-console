/*
 * File:   test_buzzer.c
 * Author: samac
 *
 * Created on 11 juin 2024, 11:41
 */

#include "../game-console-software/src/engine.h"

void main4(void) {
    e_init_game_console();
    while(1){
        e_play_A_4(500);
        e_play_D4(250);
        e_play_A3(500);
    }
}