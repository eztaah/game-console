/*
 * File:   main.c
 * Author: http://pic18fxx.blogspot.com  
 */
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "bit_settings.h"
#include "spi_pic18f.h"
#include "ili9341.h"
#include "bitmap.h"
#include "time_delay.h"

void main(void) {
    int i;
    char buffer1[20];
    OSCCON = 0b01110000;
    OSCTUNEbits.PLLEN = 1; // turn on the PLL 64 MHz
    ANSELA = 0; ANSELB = 0; ANSELC = 0; ANSELD = 0; ANSELE = 0;
    PORTA  = 0; PORTB  = 0; PORTC  = 0; PORTD  = 0; PORTE  = 0;
    TRISA  = 0; TRISB  = 0; TRISC  = 0; TRISD  = 0; TRISE  = 0;
    SPI1_Close();
    SPI1_Init();
    TFT_Init();
    
    TFT_Box(33, 150, 63, 180, RED);
    TFT_Box(103, 150, 133, 180, GREEN);
    TFT_Box(173, 150, 203, 180, BLUE);

    while(1){}
}
