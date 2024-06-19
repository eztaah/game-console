/*
 * File:   test_buzzer.c
 * Author: eztaah, MatteoPerez, MokhmadGUIRIEV and 21KEBY
 */

#include "../game-console-software/src/engine.h"

void main8(void) {
    e_init_game_console(BLACK);
    while(1){
        e_play_A_4(500);
        e_play_D4(250);
        e_play_A3(500);
    }
}
