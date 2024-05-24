/*
 * File:   spi_pic18f.c
 * Author: http://pic18fxx.blogspot.com 
 */
#include <xc.h>
#include "spi_pic18f.h"

void SPI1_Init(void){
TRIS_SCK1 = 0; TRIS_SDO1 = 0; TRIS_SDI1 = 1; TRIS_SS1 = 0;
SSP1STAT = 0xC0;
SSP1CON1 = 0x21;

// test pour augmenter le baude rate
// BAUDCON1 = 0b00000000;
// SPBRGH1 = 0b10000000;
// SPBRGH2 = 0b11111111;
}

void SPI1_Close(void){
SSP1CON1 &= 0xDF;
}

void SPI1_Write(unsigned char data){    
//SSP1CON1bits.WCOL = 0;
SSP1BUF = data;
while(!SSP1STATbits.BF);  
data = SSP1BUF;
}

unsigned char SPI1_Read(void){    
SSP1BUF = 0x00;
while(!SSP1STATbits.BF);
return SSP1BUF; 
}