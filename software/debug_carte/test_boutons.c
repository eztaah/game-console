/*
 * File:   test_boutons.c
 * Author: samac
 *
 * Created on 11 juin 2024, 10:00
 */

#include "../game-console-software/src/engine.h"

void main() {
    e_init_game_console();
    while(1){
        if(e_is_button_pressed(BUTTON_UP)) {
            e_turn_on_led(DEBUG_LED_1);
        }
        if(e_is_button_pressed(BUTTON_DOWN)) {
            e_turn_on_led(DEBUG_LED_2);
        }
        if(e_is_button_pressed(BUTTON_RIGHT)) {
            e_turn_on_led(DEBUG_LED_3);
        }
        if(e_is_button_pressed(BUTTON_LEFT)) {
            e_turn_on_led(DEBUG_LED_4);
        }
        if(e_is_button_pressed(BUTTON_A)) {
            e_turn_on_led(DEBUG_LED_1);
            e_turn_on_led(DEBUG_LED_2);
        }
        if(e_is_button_pressed(BUTTON_B)) {
            e_turn_on_led(DEBUG_LED_3);
            e_turn_on_led(DEBUG_LED_4);
        }
        if(e_is_button_pressed(BUTTON_HOME)) {
            e_turn_on_led(DEBUG_LED_1);
            e_turn_on_led(DEBUG_LED_2);
            e_turn_on_led(DEBUG_LED_3);
            e_turn_on_led(DEBUG_LED_4);
        }
        else{
            e_turn_off_led(DEBUG_LED_1);
            e_turn_off_led(DEBUG_LED_2);
            e_turn_off_led(DEBUG_LED_3);
            e_turn_off_led(DEBUG_LED_4);
        }
    }
}
