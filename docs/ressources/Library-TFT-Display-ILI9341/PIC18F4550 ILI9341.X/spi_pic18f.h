/*
 * File:   spi_pic18f.h
 * Author: http://pic18fxx.blogspot.com 
 */
#ifndef SPI_PIC18F_H
#define	SPI_PIC18F_H

#define TRIS_SCK1   TRISBbits.TRISB1         
#define TRIS_SDO1   TRISCbits.TRISC7
#define TRIS_SDI1   TRISBbits.TRISB0
#define TRIS_SS1    TRISAbits.TRISA5

void SPI1_Init(void);
void SPI1_Close(void);
void SPI1_Write(unsigned char data);
unsigned char SPI1_Read(void);

#endif	/* SPI_PIC18F_H */