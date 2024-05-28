/*
 * File:   engine.c
 * Author: kabay
 *
 * Created on 28 May 2024, 09:47
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "bit_settings.h"
#include "spi_pic18f.h"
#include "ili9341.h"
#include "font.h"
#include "bitmap.h"
#include "time_delay.h"

#include "engine.h"

void renderRectangle(uint16_t old_left, uint16_t old_top, uint16_t old_right, uint16_t old_bottom,
                     uint16_t new_left, uint16_t new_top, uint16_t new_right, uint16_t new_bottom,
                     uint16_t clear_color, uint16_t fill_color) {
    // Clear the regions not overlapped
    // Clear left strip
    if (new_left > old_left) {
        TFT_Box(old_left, old_top, new_left - 1, old_bottom, clear_color);
    }
    // Clear right strip
    if (new_right < old_right) {
        TFT_Box(new_right + 1, old_top, old_right, old_bottom, clear_color);
    }
    // Clear top strip
    if (new_top > old_top) {
        TFT_Box(old_left, old_top, old_right, new_top - 1, clear_color);
    }
    // Clear bottom strip
    if (new_bottom < old_bottom) {
        TFT_Box(old_left, new_bottom + 1, old_right, old_bottom, clear_color);
    }

    // Draw the new ball position as a black square
    TFT_Box(new_left, new_top, new_right, new_bottom, fill_color);
}

void initBuzzer(void){
    TRISCbits.TRISC2 = 0;   // RC2 est la sortie PWM, sur laquelle est connect? le buzzer
    T2CON = 0b00000110;     // Postscaler = 1, Timer ON, Prescaler = 16
    CCP1CON = 0b00001100;   // mode PWM, avec 1 seule sortie sur RC2
}

void impactSound(void){
    PR2 = 35;           // fr?quence_PWM = (1MHz)/4 /prescaler / 36 = 440 Hz
    CCPR1L = 18;        // rapport cyclique = 50%
    _delay(125000);     // 125000 * Tcy = 125000 * 4us = 0.125 sec
    stopBuzzer();
}

void stopBuzzer(void){
    PR2 = 0;
    CCPR1L = 0;
}