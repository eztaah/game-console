/*
 * File:   test_boutons.c
 * Author: samac
 *
 * Created on 11 juin 2024, 10:00
 */

#include "engine.h"

void main() {
    e_init_game_console();
    while(1){
        if(e_is_button_pressed(BUTTON_UP)) {
            LATBbits.LATB2 = 1;
        }
        if(e_is_button_pressed(BUTTON_DOWN)) {
            LATBbits.LATB3 = 1;
        }
        if(e_is_button_pressed(BUTTON_RIGHT)) {
            LATBbits.LATB4 = 1;
        }
        if(e_is_button_pressed(BUTTON_LEFT)) {
            LATBbits.LATB5 = 1;
        }
        if(e_is_button_pressed(BUTTON_A)) {
            LATBbits.LATB2 = 1;
            LATBbits.LATB3 = 1;
        }
        if(e_is_button_pressed(BUTTON_B)) {
            LATBbits.LATB4 = 1;
            LATBbits.LATB5 = 1;
        }
        if(e_is_button_pressed(BUTTON_HOME)) {
            LATBbits.LATB2 = 1;
            LATBbits.LATB3 = 1;
            LATBbits.LATB4 = 1;
            LATBbits.LATB5 = 1;
        }
        else{
            LATBbits.LATB2 = 0;
            LATBbits.LATB3 = 0;
            LATBbits.LATB4 = 0;
            LATBbits.LATB5 = 0;
        }
    }
}
