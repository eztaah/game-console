/*
 * File:   spi_pic18f.c
 * Author: http://pic18fxx.blogspot.com 
 */
#include <xc.h>
#include "spi_pic18f.h"

void SPI1_Init(void){
TRIS_SCK1 = 0; TRIS_SDO1 = 0; TRIS_SDI1 = 1; TRIS_SS1 = 0;
SSPSTAT = 0xC0;
SSPCON1 = 0x20;
}

void SPI1_Close(void){
SSPCON1 &= 0xDF;
}

void SPI1_Write(unsigned char data){
//SSP1CON1bits.WCOL = 0;
SSPBUF = data;
while(!SSPSTATbits.BF);  
data = SSPBUF;
}

unsigned char SPI1_Read(void){    
SSPBUF = 0x00;
while(!SSPSTATbits.BF);
return SSPBUF; 
}