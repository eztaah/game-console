/*
 * File:   test_ecran.c
 * Author: samac
 *
 * Created on 11 juin 2024, 12:02
 */

#include "../game-console-software/src/engine.h"

void main5(void) {
    e_init_game_console();
    while(1){
        e_fill_screen(RED);
        e_fill_screen(YELLOW);
        e_fill_screen(GREEN);
    }
}
