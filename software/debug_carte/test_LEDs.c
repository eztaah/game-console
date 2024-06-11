/*
 * File:   test_boutons.c
 * Author: samac
 *
 * Created on 11 juin 2024, 10:00
 */

#include "../game-console-software/src/engine.h"

void main(void) {
    e_init_game_console();
    // Les LEDs clignotent à tour de rôle
    while(1){
        e_toggle_led(DEBUG_LED_1);
        e_toggle_led(DEBUG_LED_2);
        e_sleep_ms(1000);
        e_toggle_led(DEBUG_LED_2);
        e_toggle_led(DEBUG_LED_3);
        e_sleep_ms(1000);
        e_toggle_led(DEBUG_LED_3);
        e_toggle_led(DEBUG_LED_4);
        e_sleep_ms(1000);
        e_toggle_led(DEBUG_LED_4);
        e_toggle_led(DEBUG_LED_1);
        e_sleep_ms(1000);
    }
}
