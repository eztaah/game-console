/*
 * File:   ili9341.c
 * Author: 
 * 
 * Graphical display TFT 240x320. 
 * Driver ILI9341.
 * Interface: SPI.
 * Version 2.0
 */
#include <xc.h>
#include "spi_pic18f.h"
#include "ili9341.h"
#include "time_delay.h"

//==============================================================================
// Declaration of global variables.
//==============================================================================
const unsigned char *font, *font2;
unsigned char width, height, letter_spacing, dot_size = 0, frame_memory = TFT_VERTICAL; 
uint16_t tft_x = TFT_W - 1;
//==============================================================================
// This function initializes the driver ILI9341.
//==============================================================================
void TFT_Init(void) {
    TRIS_CS = 0; 
    TRIS_DC = 0; 
    TRIS_RES = 0;
    TFT_CS = 1; 
    TFT_DC = 0; 
    TFT_RES = 0;

    TFT_Reset();
    TFT_WriteCommand(0xCB);   // Power control A (CBh)
    TFT_WriteParameter(0x39); 
    TFT_WriteParameter(0x2C); 
    TFT_WriteParameter(0x00); 
    TFT_WriteParameter(0x34); 
    TFT_WriteParameter(0x02); 

    TFT_WriteCommand(0xCF);   // Power control B (CFh)
    TFT_WriteParameter(0x00); 
    TFT_WriteParameter(0XC1); 
    TFT_WriteParameter(0X30); 

    TFT_WriteCommand(0xE8);   // Driver timing control A (E8h)  
    TFT_WriteParameter(0x85); 
    TFT_WriteParameter(0x00); 
    TFT_WriteParameter(0x78); 

    TFT_WriteCommand(0xEA);   // Driver timing control B (EAh)  
    TFT_WriteParameter(0x00); 
    TFT_WriteParameter(0x00); 

    TFT_WriteCommand(0xED);   // Power on sequence control (EDh) 
    TFT_WriteParameter(0x64); 
    TFT_WriteParameter(0x03); 
    TFT_WriteParameter(0X12); 
    TFT_WriteParameter(0X81); 

    TFT_WriteCommand(0xF7);   // Pump ratio control (F7h)
    TFT_WriteParameter(0x20); 

    TFT_WriteCommand(0xC0);   // Power Control 1 (C0h) 
    TFT_WriteParameter(0x23); // VRH[5:0] 

    TFT_WriteCommand(0xC1);   // Power Control 2 (C1h)
    TFT_WriteParameter(0x10); // SAP[2:0]; BT[3:0] 

    TFT_WriteCommand(0xC5);   // Set the VCOMH voltage. 
    TFT_WriteParameter(0x3E);
    TFT_WriteParameter(0x28); 

    TFT_WriteCommand(0xC7);   // VCOM Control 2(C7h)
    TFT_WriteParameter(0x86);  

    TFT_WriteCommand(0x36);   // Memory Access Control (36h)
    TFT_WriteParameter(TFT_VERTICAL);

    TFT_WriteCommand(0x3A);   // COLMOD: Pixel Format Set (3Ah)    
    TFT_WriteParameter(0x55); 

    TFT_WriteCommand(0xB1);   // Frame Rate Control (In Normal Mode/Full Colors) (B1h)  
    TFT_WriteParameter(0x00);  
    TFT_WriteParameter(0x18); 

    TFT_WriteCommand(0xB6);   // Display Function Control (B6h) 
    TFT_WriteParameter(0x08); 
    TFT_WriteParameter(0x82);
    TFT_WriteParameter(0x27);  

    TFT_WriteCommand(0xF2);   // Enable 3G (F2h)
    TFT_WriteParameter(0x00); 

    TFT_WriteCommand(0x26);   // Gamma Set (26h)
    TFT_WriteParameter(0x01); 

    TFT_WriteCommand(0xE0);   //Positive Gamma Correction (E0h)
    TFT_WriteParameter(0x0F); 
    TFT_WriteParameter(0x31); 
    TFT_WriteParameter(0x2B); 
    TFT_WriteParameter(0x0C); 
    TFT_WriteParameter(0x0E); 
    TFT_WriteParameter(0x08); 
    TFT_WriteParameter(0x4E); 
    TFT_WriteParameter(0xF1); 
    TFT_WriteParameter(0x37); 
    TFT_WriteParameter(0x07); 
    TFT_WriteParameter(0x10); 
    TFT_WriteParameter(0x03); 
    TFT_WriteParameter(0x0E); 
    TFT_WriteParameter(0x09); 
    TFT_WriteParameter(0x00); 

    TFT_WriteCommand(0XE1);   // Negative Gamma Correction (E1h)
    TFT_WriteParameter(0x00); 
    TFT_WriteParameter(0x0E); 
    TFT_WriteParameter(0x14); 
    TFT_WriteParameter(0x03); 
    TFT_WriteParameter(0x11); 
    TFT_WriteParameter(0x07); 
    TFT_WriteParameter(0x31); 
    TFT_WriteParameter(0xC1); 
    TFT_WriteParameter(0x48); 
    TFT_WriteParameter(0x08); 
    TFT_WriteParameter(0x0F); 
    TFT_WriteParameter(0x0C); 
    TFT_WriteParameter(0x31); 
    TFT_WriteParameter(0x36); 
    TFT_WriteParameter(0x0F); 

    TFT_WriteCommand(0x11);   // Exit Sleep
    Delay_ms(150);            // Delay of 150ms
    TFT_WriteCommand(0x29);   // Display ON (29h)
    TFT_FillScreen(WHITE);
}

//==============================================================================
// This function resets the display TFT.
//==============================================================================
void TFT_Reset(void){
    TFT_CS = 1;        
    TFT_RES = 0; 
    Delay_ms(50);
    TFT_RES = 1; 
    Delay_ms(150);
}

//==============================================================================
// This function writes a command.
//==============================================================================
void TFT_WriteCommand(unsigned char command){   
    TFT_CS = 0;
    TFT_DC = 0; // When DCX = ’0’, command is selected.
    SPI1_Write(command);
    TFT_CS = 1;
}

//==============================================================================
// This function writes a Parameter.
//==============================================================================
void TFT_WriteParameter(unsigned char parameter){   
    TFT_CS = 0;
    TFT_DC = 1; // When DCX = ’1’, data is selected.
    SPI1_Write(parameter);
    TFT_CS = 1;
}

//==============================================================================
// This function is used to define area of frame memory where MCU can access.
// x1: Set start column address. 
// x2: Set end column address. 
// y1: Set start page address.
// y2: Set end page address.
//==============================================================================
void TFT_ColumnPage(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2){      
    TFT_CS = 0;
    TFT_DC = 0; 
    SPI1_Write(0x2A);
    TFT_DC = 1; 
    SPI1_Write(x1 >> 8); 
    SPI1_Write(x1 & 0xFF); 
    SPI1_Write(x2 >> 8); 
    SPI1_Write(x2 & 0xFF);
    TFT_DC = 0; 
    SPI1_Write(0x2B);
    TFT_DC = 1; 
    SPI1_Write(y1 >> 8); 
    SPI1_Write(y1 & 0xFF); 
    SPI1_Write(y2 >> 8); 
    SPI1_Write(y2 & 0xFF);
    TFT_DC = 0; 
    SPI1_Write(0x2C);
    TFT_CS = 1;
}

//==============================================================================
// This function sets the memory access control. 
//==============================================================================
unsigned char TFT_MemoryAccessControl(unsigned char frame_memory_){
    if(frame_memory_ == 0){
        return frame_memory;
    }
    TFT_WriteCommand(0x36);
    TFT_WriteParameter(frame_memory_);
    switch(frame_memory_) {
        case TFT_VERTICAL:     tft_x = TFT_W - 1; 
        break;
        case TFT_HORIZONTAL:   tft_x = TFT_H - 1; 
        break;    
        case TFT_VERTICAL_BMP: tft_x = TFT_W - 1; 
        break;
    }
    frame_memory = frame_memory_;
    return frame_memory;
} 

//==============================================================================
// This function is used to convert 24 bpp color data to 5-6-5 RGB format.
// 16 bit/pixel color order (R:5-bit, G:6-bit, B:5-bit), 65,536 colors.
// 8-8-8 to to 5-6-5 conversion.
//==============================================================================
uint16_t TFT_RGBConvert(uint16_t red, uint16_t green, uint16_t blue){
    uint16_t color = 0;
    red = (red & 0xF8) << 8; 
    green = (green & 0xFC) << 3; 
    blue = (blue & 0xF8) >> 3;
    color = red | green | blue;
    return color;
}

//==============================================================================
// This function draws a pixel on TFT.
// x: x position. Valid values: 0..240 
// y: y position. Valid values: 0..320 
// color: color parameter.
//==============================================================================
void TFT_Pixel(uint16_t x, uint16_t y, uint16_t color){
    TFT_CS = 0;
    TFT_DC = 0; 
    SPI1_Write(0x2A);
    TFT_DC = 1; 
    SPI1_Write(x >> 8); 
    SPI1_Write(x & 0xFF); 
    SPI1_Write(x >> 8); 
    SPI1_Write(x & 0xFF);
    TFT_DC = 0; 
    SPI1_Write(0x2B);
    TFT_DC = 1; 
    SPI1_Write(y >> 8); 
    SPI1_Write(y & 0xFF); 
    SPI1_Write(y >> 8); 
    SPI1_Write(y & 0xFF);
    TFT_DC = 0; 
    SPI1_Write(0x2C); 
    TFT_DC = 1; 
    SPI1_Write(color >> 8); 
    SPI1_Write(color & 0xFF);
    TFT_CS = 1;
}


//==============================================================================
// This function fills screen with given color.  
// color: color parameter.
//==============================================================================
void TFT_FillScreen(uint16_t color){    
    unsigned char DH, DL;
    uint16_t i, j;
    DH = color >> 8;
    DL = color & 0xFF;
    switch(frame_memory)
          {
           case TFT_VERTICAL:   
               TFT_ColumnPage(0, TFT_W - 1, 0, TFT_H - 1); 
               break;
           case TFT_HORIZONTAL: 
               TFT_ColumnPage(0, TFT_H - 1, 0, TFT_W - 1); 
               break;    
          }
    TFT_CS = 0; 
    TFT_DC = 1;
    for(i = 0; i < TFT_H; i++)
       {
        for (j = 0; j < TFT_W; j++)
            {
             SPI1_Write(DH);
             SPI1_Write(DL);
            }
       }
    TFT_CS = 1;
}

//==============================================================================
// This function draws a box on TFT.
// x1: x coordinate of the upper left rectangle corner. Valid values: 0..240  
// y1: y coordinate of the upper left rectangle corner. Valid values: 0..320 
// x2: x coordinate of the lower right rectangle corner. Valid values: 0..240 
// y2: y coordinate of the lower right rectangle corner. Valid values: 0..320 
// color: color parameter. 
//==============================================================================
void TFT_Box(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color){
    unsigned char DH, DL;
    uint16_t i, j;
    DH = color >> 8;
    DL = color & 0xFF;
    TFT_ColumnPage(x1, x2, y1, y2);
    TFT_CS = 0; 
    TFT_DC = 1;
    for(i = y1; i <= y2; i++)
       {
        for (j = x1; j <= x2; j++)
            {
                SPI1_Write(DH);
                SPI1_Write(DL);        
            }
       }
    TFT_CS = 1;
}