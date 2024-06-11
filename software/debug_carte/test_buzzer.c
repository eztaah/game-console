/*
 * File:   test_buzzer.c
 * Author: samac
 *
 * Created on 11 juin 2024, 11:41
 */

#include "engine.h"

void main(void) {
    _e_init_buzzer(void);
    while(1){
        e_play_A_4(500);
        e_play_D4(250);
        e_play_A3(500);
    }
}
