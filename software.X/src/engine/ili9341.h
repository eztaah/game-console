/*
 * File:   ili9341.h
 * Author: http://pic18fxx.blogspot.com 
 *
 * Graphical display TFT 240x320. 
 * Driver ILI9341.
 * Interface: SPI.
 * Version 2.0
 */
#ifndef __ILI9341_H
#define __ILI9341_H
//==============================================================================
// Connection of module TFT.
//
// VCC      <<<>>> 3.3 V.
// GND      <<<>>> GND.
// TFT_CS   <<<>>> You can change this pin.
// TFT_REST <<<>>> You can change this pin.
// TFT_D/C  <<<>>> You can change this pin.
// TFT_SDI  <<<>>> Connect to SDO MCU.
// TFT_SCK  <<<>>> Connect to SCK MCU.
// TFT_SDO  <<<>>> Connect to SDI MCU.
// LED      <<<>>> 3.3 V (Use a 10-ohm resistor).
//==============================================================================
#define TFT_CS             LATDbits.LATD0
#define TRIS_CS            TRISDbits.TRISD0

#define TFT_RES            LATDbits.LATD1
#define TRIS_RES           TRISDbits.TRISD1

#define TFT_DC             LATDbits.LATD2
#define TRIS_DC            TRISDbits.TRISD2

//==============================================================================
// These are the defines to select the memory access control.
//==============================================================================
#define TFT_VERTICAL       0x88
#define TFT_HORIZONTAL     0xE8
#define TFT_VERTICAL_BMP   0x08

//==============================================================================
// Define number of pixels in width and height.
// New name for data types.
//==============================================================================
#define TFT_W 320
#define TFT_H 240

//==============================================================================
// Function Prototypes.
//==============================================================================
// SPI
#define TRIS_SCK1   TRISCbits.TRISC3         
#define TRIS_SDO1   TRISCbits.TRISC5
#define TRIS_SDI1   TRISCbits.TRISC4
#define TRIS_SS1    TRISAbits.TRISA5
void SPI1_Init(void);
void SPI1_Close(void);
void SPI1_Write(unsigned char data);
unsigned char SPI1_Read(void);


// SCREEN
void TFT_Init(void);
void TFT_Reset(void);
void TFT_WriteCommand(unsigned char command);
void TFT_WriteParameter(unsigned char parameter);
void TFT_ColumnPage(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2);
unsigned char TFT_MemoryAccessControl(unsigned char frame_memory_);
uint16_t TFT_RGBConvert(uint16_t red, uint16_t green, uint16_t blue);
//==============================================================================
void TFT_Pixel(uint16_t x, uint16_t y, uint16_t color);
void TFT_Box(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void TFT_FillScreen(uint16_t color);

void TFT_SetFont(const unsigned char *font_, unsigned char letterspacing);
unsigned char TFT_CharHeight(void);
void TFT_WriteChar(unsigned char c, uint16_t x, uint16_t y, uint16_t color1, uint16_t color2);
void TFT_ConstText(const char *buffer, uint16_t x, uint16_t y, uint16_t color1, uint16_t color2);
void TFT_Text(char *buffer, uint16_t x, uint16_t y, uint16_t color1, uint16_t color2);

#endif