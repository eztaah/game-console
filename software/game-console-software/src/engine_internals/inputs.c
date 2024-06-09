/*
 * File:   inputs.c
 * Author: eztaah, MatteoPerez, MokhmadGUIRIEV and 21KEBY
 */

#include "../engine.h"
#include "internal.h"


//#define TRIS_B_UP           TRISAbits.TRISA5
//#define B_UP                PORTAbits.RA5
//
//#define TRIS_B_DOWN         TRISEbits.TRISE2
//#define B_DOWN              PORTEbits.RE2
//
//#define TRIS_B_RIGHT        TRISAbits.TRISA7
//#define B_RIGHT             PORTAbits.RA7
//
//#define TRIS_B_LEFT         TRISAbits.TRISA4
//#define B_LEFT              PORTAbits.RA4
//
//#define TRIS_B_A            TRISBbits.TRISB0
//#define B_A                 PORTBbits.RB0
//
//#define TRIS_B_B            TRISDbits.TRISD5
//#define B_B                 PORTDbits.RD5
//
//#define TRIS_B_HOME         TRISDbits.TRISD2
//#define B_HOME              PORTDbits.RD2


#define TRIS_B_UP           TRISBbits.TRISB0
#define B_UP                PORTBbits.RB0

#define TRIS_B_DOWN         TRISDbits.TRISD2
#define B_DOWN              PORTDbits.RD2

#define TRIS_B_RIGHT        TRISBbits.TRISB2
#define B_RIGHT             PORTBbits.RB2

#define TRIS_B_LEFT         TRISBbits.TRISB3
#define B_LEFT              PORTBbits.RB3

#define TRIS_B_A            TRISBbits.TRISB4
#define B_A                 PORTBbits.RB4

#define TRIS_B_B            TRISBbits.TRISB5
#define B_B                 PORTBbits.RB5

#define TRIS_B_HOME         TRISAbits.TRISA4
#define B_HOME              PORTAbits.RA4



//==============================================================================
// 
//==============================================================================
void _e_init_buttons(void)
{
    TRIS_B_UP = 1;
    B_UP = 0;
    
    TRIS_B_DOWN = 1;
    B_DOWN = 0;
    
    TRIS_B_RIGHT = 1;
    B_RIGHT = 0;
    
    TRIS_B_LEFT = 1;
    B_LEFT = 0;
    
    TRIS_B_A = 1;
    B_A = 0;
    
    TRIS_B_B = 1;
    B_B = 0;
    
    TRIS_B_HOME = 1;
    B_HOME = 0;
}

//==============================================================================
// This function checks if a specified button is currently pressed.
// button: The button identifier to check.
// Returns 1 if pressed, 0 if not, -1 if an invalid button is checked.
//==============================================================================
int16_t e_is_button_pressed(int16_t button)
{
    switch (button) {
        case BUTTON_UP:
            return !B_UP;     // 1 si press�, et 0 sinon
            break;
        case BUTTON_DOWN:
            return !B_DOWN;
            break;
        case BUTTON_RIGHT:
            return !B_RIGHT;
            break;
        case BUTTON_LEFT:
            return !B_LEFT;
            break;
        case BUTTON_A:
            return !B_A;
            break;
        case BUTTON_B:
            return !B_B;
            break;
        case BUTTON_HOME:
            return !B_HOME;
            break;
        default:
            return -1;
            break;
    }
}