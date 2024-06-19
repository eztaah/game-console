/*
 * File:   test_boutons.c
 * Author: eztaah, MatteoPerez, MokhmadGUIRIEV and 21KEBY
 */

#include "./engine.h"

void main1(void) {
    e_init_game_console(BLACK);
    // Les LEDs clignotent � tour de r�le
    while(1){
        e_toggle_debug_led(DEBUG_LED_1);
        e_toggle_debug_led(DEBUG_LED_2);
        e_sleep_ms(1000);
        e_toggle_debug_led(DEBUG_LED_2);
        e_toggle_debug_led(DEBUG_LED_3);
        e_sleep_ms(1000);
        e_toggle_debug_led(DEBUG_LED_3);
        e_toggle_debug_led(DEBUG_LED_4);
        e_sleep_ms(1000);
        e_toggle_debug_led(DEBUG_LED_4);
        e_toggle_debug_led(DEBUG_LED_1);
        e_sleep_ms(1000);
    }
}
