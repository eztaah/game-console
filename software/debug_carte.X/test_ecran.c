/*
 * File:   test_ecran.c
 * Author: eztaah, MatteoPerez, MokhmadGUIRIEV and 21KEBY
 */

#include "../game-console-software/src/engine.h"

void main(void) {
    e_init_game_console(BLACK);
    while(1){
        e_fill_screen(RED);
        e_fill_screen(YELLOW);
        e_fill_screen(GREEN);
    }
}
