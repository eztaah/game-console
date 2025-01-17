/**
  TMR0 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    tmr0.c

  @Summary
    This is the generated driver implementation file for the TMR0 driver using MPLAB� Code Configurator

  @Description
    This source file provides APIs for TMR0.
    Generation Information :
        Product Revision  :  MPLAB� Code Configurator - v2.10.3
        Device            :  PIC16F1937
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 v1.34
        MPLAB             :  MPLAB X 2.26
*/

/*
Copyright (c) 2013 - 2015 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*/

/**
  Section: Included Files
*/

#include <xc.h>
#include "tmr0.h"
#include "../app.h"
#include "../Buttons.h"

/**
  Section: Global Variables Definitions
*/

volatile uint8_t timer0ReloadVal;

/**
  Section: TMR0 APIs
*/

void TMR0_Initialize(void)
{
    // Set TMR0 to the options selected in the User Interface

    // PSA assigned; PS 1:256; TMRSE Increment_lo_hi; mask the nWPUEN and INTEDG bits
    OPTION_REG = (OPTION_REG & 0xC0) | 0xC7 & 0x3F; 

    // TMR0 0; 
    TMR0 = 0x00;

    // Load the TMR value to reload variable
    timer0ReloadVal= 0;

    // Clear Interrupt flag before enabling the interrupt
    INTCONbits.TMR0IF = 0;

    // Enabling TMR0 interrupt
    INTCONbits.TMR0IE = 1;
}


uint8_t TMR0_ReadTimer(void)
{
    uint8_t readVal;

    readVal = TMR0;

    return readVal;
}

void TMR0_WriteTimer(uint8_t timerVal)
{
    // Write to the Timer0 register
    TMR0 = timerVal;
}

void TMR0_Reload(void)
{
    // Write to the Timer0 register
    TMR0 = timer0ReloadVal;
}

void TMR0_ISR(void)
{

    // clear the TMR0 interrupt flag
    INTCONbits.TMR0IF = 0;

    TMR0 = timer0ReloadVal;


    // add your TMR0 interrupt custom code
      if (tmrCount)                     // If counter Enabled by application
		  {
	    	    tmrCount--;                         // The software timer is running
                    }
    	  CheckButtons();                           // Go scan & process buttons

             
    	  TMR0IF = 0;                         // Interrupt has been serviced
}                                         // If Timer0 Interrupt occured
/**
  End of File
*/