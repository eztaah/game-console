/*
 * File:   test_boutons.c
 * Author: samac
 *
 * Created on 11 juin 2024, 10:00
 */

#include "engine.h"

void main(void) {
    TRISBbits.TRISB3 = 0;
    TRISBbits.TRISB4 = 0;
    TRISBbits.TRISB5 = 0;
    LATBbits.LATB3 = 0;
    LATBbits.LATB4 = 0;
    LATBbits.LATB5 = 0;
    // Toutes les LEDs s'allument
    while(1){
        LATBbits.LATB3 = 1;
        LATBbits.LATB4 = 1;
        LATBbits.LATB5 = 1;
    }
}
