/*
 * File:   test_boutons.c
 * Author: samac
 *
 * Created on 11 juin 2024, 10:00
 */

#include "engine.h"

void main(void) {
    // Bouton UP
    TRISAbits.TRISA5 = 1;
    PORTAbits.RA5 = 0;
    // Bouton DOWN
    TRISEbits.TRISE2 = 1;
    PORTEbits.RE2 = 0;
    // Bouton RIGHT
    TRISAbits.TRISA7 = 1;
    PORTAbits.RA7 = 0;
    // Bouton LEFT
    TRISAbits.TRISA4 = 1;
    PORTAbits.RA4 = 0;
    // Bouton A
    TRISBbits.TRISB0 = 1;
    PORTBbits.RB0 = 0;
    // Bouton B
    TRISDbits.TRISD5 = 1;
    PORTDbits.RD5 = 0;
    // Bouton HOME
    TRISDbits.TRISD2 = 1;
    PORTDbits.RD2 = 0;
    
    TRISBbits.TRISB3 = 0;
    TRISBbits.TRISB4 = 0;
    TRISBbits.TRISB5 = 0;
    LATBbits.LATB3 = 0;
    LATBbits.LATB4 = 0;
    LATBbits.LATB5 = 0;
    
    while(1){
        if(e_is_button_pressed(BUTTON_UP)) {
            LATBbits.LATB3 = 1;
        }
        if(e_is_button_pressed(BUTTON_DOWN)) {
            LATBbits.LATB4 = 1;
        }
        if(e_is_button_pressed(BUTTON_RIGHT)) {
            LATBbits.LATB5 = 1;
        }
        if(e_is_button_pressed(BUTTON_LEFT)) {
            LATBbits.LATB3 = 1;
            LATBbits.LATB4 = 1;
        }
        if(e_is_button_pressed(BUTTON_A)) {
            LATBbits.LATB4 = 1;
            LATBbits.LATB5 = 1;
        }
        if(e_is_button_pressed(BUTTON_B)) {
            LATBbits.LATB3 = 1;
            LATBbits.LATB5 = 1;
        }
        if(e_is_button_pressed(BUTTON_HOME)) {
            LATBbits.LATB3 = 0;
            LATBbits.LATB4 = 0;
            LATBbits.LATB5 = 0;
        }
    }
}
