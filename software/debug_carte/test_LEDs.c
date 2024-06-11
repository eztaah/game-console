/*
 * File:   test_boutons.c
 * Author: samac
 *
 * Created on 11 juin 2024, 10:00
 */

#include "engine.h"

void main(void) {
    e_init_game_console();
    // Les LEDs clignotent à tour de rôle
    while(1){
        LATBbits.LATB5 = 0;
        LATBbits.LATB2 = 1;
        e_sleep_ms(1000);
        LATBbits.LATB2 = 0;
        LATBbits.LATB3 = 1;
        e_sleep_ms(1000);
        LATBbits.LATB3 = 0;
        LATBbits.LATB4 = 1;
        e_sleep_ms(1000);
        LATBbits.LATB4 = 0;
        LATBbits.LATB5 = 1;
        e_sleep_ms(1000);
    }
}
