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
// These are some colors that you can use for fonts and graphics.
//==============================================================================
#define BLACK              0b0000000000000000
#define BRIGHTBLUE         0b0000000000011111
#define BRIGHTGREEN        0b0000011111100000
#define BRIGHTCYAN         0b0000011111111111
#define BRIGHTRED          0b1111100000000000
#define BRIGHTMAGENTA      0b1111100000011111
#define BRIGHTYELLOW       0b1111111111100000
#define BLUE               0b0000000000010000
#define GREEN              0b0000010000000000
#define CYAN               0b0000010000010000
#define RED                0b1000000000000000
#define MAGENTA            0b1000000000010000
#define BROWN              0b1111110000000000
#define LIGHTGRAY          0b1000010000010000
#define DARKGRAY           0b0100001000001000
#define LIGHTBLUE          0b1000010000011111
#define LIGHTGREEN         0b1000011111110000
#define LIGHTCYAN          0b1000011111111111
#define LIGHTRED           0b1111110000010000
#define LIGHTMAGENTA       0b1111110000011111
#define YELLOW             0b1111111111110000
#define WHITE              0b1111111111111111

#define GRAY0       	   0b1110011100011100
#define GRAY1         	   0b1100011000011000   
#define GRAY2              0b1010010100010100   
#define GRAY3              0b1000010000010000
#define GRAY4              0b0110001100001100
#define GRAY5              0b0100001000001000
#define GRAY6	           0b0010000100000100

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
#define TFT_W 240
#define TFT_H 320

typedef char                       schar_t;
typedef unsigned char              uchar_t;
//typedef char                     Schar;
//typedef unsigned char            Uchar;
//typedef const char               CSchar;
//typedef const unsigned char      CUchar;
//typedef int16_t                  Sint; 
//typedef uint16_t                 Uint;
//typedef const uint16_t           CUint;

//==============================================================================
// Function Prototypes.
//==============================================================================
void TFT_Init(void);
void TFT_Reset(void);
void TFT_WriteCommand(uchar_t command);
void TFT_WriteParameter(uchar_t parameter);
void TFT_ColumnPage(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2);
unsigned char TFT_MemoryAccessControl(uchar_t frame_memory_);
uint16_t TFT_RGBConvert(uint16_t red, uint16_t green, uint16_t blue);
//==============================================================================
void TFT_Pixel(uint16_t x, uint16_t y, uint16_t color);
void TFT_FillScreen(uint16_t color);
void TFT_Box(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);

void TFT_WriteChar(uchar_t c, uint16_t x, uint16_t y, uint16_t color1, uint16_t color2);
void TFT_Text(schar_t *buffer, uint16_t x, uint16_t y, uint16_t color1, uint16_t color2);

#endif