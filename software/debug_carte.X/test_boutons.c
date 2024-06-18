/*
 * File:   test_boutons.c
 * Author: samac
 *
 * Created on 11 juin 2024, 10:00
 */

#include "./engine.h"

void main() {
    e_init_game_console(BLACK);
    while(1){
        if(e_is_button_down(BUTTON_UP)) {
            e_turn_on_debug_led(DEBUG_LED_1);
        }
        if(e_is_button_down(BUTTON_DOWN)) {
            e_turn_on_debug_led(DEBUG_LED_2);
        }
        if(e_is_button_down(BUTTON_RIGHT)) {
            e_turn_on_debug_led(DEBUG_LED_3);
        }
        if(e_is_button_down(BUTTON_LEFT)) {
            e_turn_on_debug_led(DEBUG_LED_4);
        }
        if(e_is_button_down(BUTTON_A)) {
            e_turn_on_debug_led(DEBUG_LED_1);
            e_turn_on_debug_led(DEBUG_LED_2);
        }
        if(e_is_button_down(BUTTON_B)) {
            e_turn_on_debug_led(DEBUG_LED_3);
            e_turn_on_debug_led(DEBUG_LED_4);
        }
        if(e_is_button_down(BUTTON_HOME)) {
            e_turn_on_debug_led(DEBUG_LED_1);
            e_turn_on_debug_led(DEBUG_LED_2);
            e_turn_on_debug_led(DEBUG_LED_3);
            e_turn_on_debug_led(DEBUG_LED_4);
        }
        else{
            e_turn_off_debug_led(DEBUG_LED_1);
            e_turn_off_debug_led(DEBUG_LED_2);
            e_turn_off_debug_led(DEBUG_LED_3);
            e_turn_off_debug_led(DEBUG_LED_4);
        }
    }
}
