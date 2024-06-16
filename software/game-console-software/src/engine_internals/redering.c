/*
 * File:   rendering.c
 * Author: eztaah, MatteoPerez, MokhmadGUIRIEV and 21KEBY
 */

#include "../engine.h"
#include "internal.h"
#include "font.h"


#define TFT_CS             LATAbits.LATA1
#define TRIS_CS            TRISAbits.TRISA1

#define TFT_RES            LATAbits.LATA2
#define TRIS_RES           TRISAbits.TRISA2

#define TFT_DC             LATAbits.LATA3
#define TRIS_DC            TRISAbits.TRISA3

#define TFT_BL             LATEbits.LATE0
#define TRIS_BL            TRISEbits.TRISE0

#define TFT_VERTICAL       0x88
#define TFT_HORIZONTAL     0xE8
#define TFT_VERTICAL_BMP   0x08

#define TFT_W 320
#define TFT_H 240

// SPI
#define TRIS_SCK1   TRISCbits.TRISC3         
#define TRIS_SDO1   TRISCbits.TRISC5
#define TRIS_SDI1   TRISCbits.TRISC4
//#define TRIS_SS1    TRISAbits.TRISA5


//==============================================================================
// Declaration of global variables.
//==============================================================================
const unsigned char *font, *font2;
unsigned char width, height, letter_spacing, dot_size = 0; 

char last_frame_duration_text[30] = {0};
char fps_text[30] = {0};




//==============================================================================
// SPI FUNCTIONS
//==============================================================================
void SPI1_Init(void){
    TRIS_SCK1 = 0; 
    TRIS_SDO1 = 0; 
    TRIS_SDI1 = 1; 
    // TRIS_SS1 = 0;
    SSP1STAT = 0xC0;
    SSP1CON1 = 0x21;
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


//==============================================================================
// This function resets the display TFT.
//==============================================================================
void TFT_Reset(void){
    TFT_CS = 1;        
    TFT_RES = 0; 
    e_sleep_ms(50);
    TFT_RES = 1; 
    e_sleep_ms(150);
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
// This function sets the type of font.
// letterspacing: Letter spacing. Valid values: 1, 2, 3...
//==============================================================================
void TFT_SetFont(const unsigned char *font_, unsigned char letterspacing){
    font2 = font_;
    letter_spacing = letterspacing;
    height = TFT_CharHeight();
}


//==============================================================================
// This function returns the height of character. The size is determined in pixels.
//==============================================================================
unsigned char TFT_CharHeight(void){
    font = font2;
    font += 6;
    return *font;
}



//==============================================================================
// This function draws a character on the TFT.
// c: character to be written. 
// x: x position. Valid values: 0..240 
// y: y position. Valid values: 0..320 
// color1: Top color.
// color2: Bottom color.
//==============================================================================
void TFT_WriteChar(unsigned char c, uint16_t x, uint16_t y, uint16_t color1, uint16_t color2)
{
    unsigned char i, j, k;
    uint16_t p;
    p = c - 32; p = p * 4; p = p + 8;       
    font = font2;                
    font += p;                  
    width = *font;
    font += 2; 
    p = *font;
    font -= 1; 
    p = (p << 8) | *font;
    font = font2;
    font += p;
    i = height;
    TFT_ColumnPage(x, x + width + letter_spacing - 1, y, y + height);
    TFT_CS = 0; 
    TFT_DC = 1; 
    while(i){
        j = width;
        while(j) {
            if(j>0){if(*font&0x01){SPI1_Write(color1>>8);SPI1_Write(color1&0xFF);}else{SPI1_Write(color2>>8);SPI1_Write(color2&0xFF);}j--;}else{font++;break;}
            if(j>0){if(*font&0x02){SPI1_Write(color1>>8);SPI1_Write(color1&0xFF);}else{SPI1_Write(color2>>8);SPI1_Write(color2&0xFF);}j--;}else{font++;break;}
            if(j>0){if(*font&0x04){SPI1_Write(color1>>8);SPI1_Write(color1&0xFF);}else{SPI1_Write(color2>>8);SPI1_Write(color2&0xFF);}j--;}else{font++;break;}
            if(j>0){if(*font&0x08){SPI1_Write(color1>>8);SPI1_Write(color1&0xFF);}else{SPI1_Write(color2>>8);SPI1_Write(color2&0xFF);}j--;}else{font++;break;}
            if(j>0){if(*font&0x10){SPI1_Write(color1>>8);SPI1_Write(color1&0xFF);}else{SPI1_Write(color2>>8);SPI1_Write(color2&0xFF);}j--;}else{font++;break;}
            if(j>0){if(*font&0x20){SPI1_Write(color1>>8);SPI1_Write(color1&0xFF);}else{SPI1_Write(color2>>8);SPI1_Write(color2&0xFF);}j--;}else{font++;break;}
            if(j>0){if(*font&0x40){SPI1_Write(color1>>8);SPI1_Write(color1&0xFF);}else{SPI1_Write(color2>>8);SPI1_Write(color2&0xFF);}j--;}else{font++;break;}
            if(j>0){if(*font&0x80){SPI1_Write(color1>>8);SPI1_Write(color1&0xFF);}else{SPI1_Write(color2>>8);SPI1_Write(color2&0xFF);}j--;}else{font++;break;}
            font++;
        }
        for(k = 0; k < letter_spacing; k++) {
            SPI1_Write(color2 >> 8); 
            SPI1_Write(color2 & 0xFF);
        }
        i--;
    }
    TFT_CS = 1;
}


//==============================================================================
// This function writes text constant on TFT.
// buffer: Pointer to read all the array.
// x: x position. Valid values: 0..240 
// y: y position. Valid values: 0..320 
// color1: Top color.
// color2: Bottom color.
//==============================================================================
void TFT_ConstText(const char *buffer, uint16_t x, uint16_t y, uint16_t color1, uint16_t color2){
while(*buffer)                
    {
     TFT_WriteChar(*buffer, x, y, color1, color2);
     x += width + letter_spacing;
     buffer++;               
    } 
}


//==============================================================================
// This function writes text variable on TFT.
// buffer: Pointer to read all the array.
// x: x position. Valid values: 0..240 
// y: y position. Valid values: 0..320 
// color1: Top color.
// color2: Bottom color.
//==============================================================================
void TFT_Text(char *buffer, uint16_t x, uint16_t y, uint16_t color1, uint16_t color2){
    while(*buffer) {
        TFT_WriteChar(*buffer, x, y, color1, color2);
        x += width + letter_spacing;
        buffer++;               
    } 
}



//==============================================================================
// This function draws a image on a desired location.
// x: x position. 
// y: y position. 
// width: width of the image in pixels.
// height: height of the image in pixels.
//==============================================================================
//==============================================================================
// This function draws an image on a desired location with zoom functionality.
// buffer: pointer to the image data in memory.
// x: x position on the display.
// y: y position on the display.
// width_: width of the image in pixels.
// height_: height of the image in pixels.
// zoom: scaling factor for each pixel (1x1, 2x2, 3x3, etc.)
//==============================================================================
void TFT_Icon(const uint16_t *buffer, uint16_t x, uint16_t y, uint16_t width_, uint16_t height_, uint16_t zoom)
{
    uint16_t i, j, zx, zy;    
    uint16_t new_width = width_ * zoom; // Calculate the original width of the image
    uint16_t new_height = height_ * zoom; // Calculate the original height of the image

    TFT_ColumnPage(x, x + new_width - 1, y, y + new_height - 1);
    TFT_CS = 0; 
    TFT_DC = 1;

    for (i = 0; i < height_; i++) {
        for (zy = 0; zy < zoom; zy++) {  // Repeat the same line zoom times
            const uint16_t *currentLine = buffer + i * width_;
            for (j = 0; j < width_; j++) {
                for (zx = 0; zx < zoom; zx++) {  // Repeat each pixel zoom times
                    SPI1_Write(currentLine[j] >> 8);
                    SPI1_Write(currentLine[j] & 0xFF);
                }
            }
        }
    }
    TFT_CS = 1;
}















//                             INTERNAL FUNCTIONS


//==============================================================================
// This function writes a command.
//==============================================================================
void write_command(unsigned char command){   
    TFT_CS = 0;
    TFT_DC = 0; // When DCX = �0�, command is selected.
    SPI1_Write(command);
    TFT_CS = 1;
}

//==============================================================================
// This function writes a Parameter.
//==============================================================================
void TFT_WriteData(unsigned char parameter){   
    TFT_CS = 0;
    TFT_DC = 1; // When DCX = �1�, data is selected.
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
void TFT_ColumnPage(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2)
{      
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
// This function fills the entire TFT screen with a single color.
// color: The color to fill the screen with.
//==============================================================================
void _e_init_screen(uint16_t color)
{
    SPI1_Close();
    SPI1_Init();
    
    // ------------------------------------
    // TFT_Init
    TRIS_CS = 0; 
    TRIS_DC = 0; 
    TRIS_RES = 0;
    TRIS_BL = 0;
    TFT_CS = 1; 
    TFT_DC = 0; 
    TFT_RES = 0;
    TFT_BL = 1;

    TFT_Reset();
    write_command(0xCB);   // Power control A (CBh)
    TFT_WriteData(0x39); 
    TFT_WriteData(0x2C); 
    TFT_WriteData(0x00); 
    TFT_WriteData(0x34); 
    TFT_WriteData(0x02); 

    write_command(0xCF);   // Power control B (CFh)
    TFT_WriteData(0x00); 
    TFT_WriteData(0XC1); 
    TFT_WriteData(0X30); 

    write_command(0xE8);   // Driver timing control A (E8h)  
    TFT_WriteData(0x85); 
    TFT_WriteData(0x00); 
    TFT_WriteData(0x78); 

    write_command(0xEA);   // Driver timing control B (EAh)  
    TFT_WriteData(0x00); 
    TFT_WriteData(0x00); 

    write_command(0xED);   // Power on sequence control (EDh) 
    TFT_WriteData(0x64); 
    TFT_WriteData(0x03); 
    TFT_WriteData(0X12); 
    TFT_WriteData(0X81); 

    write_command(0xF7);   // Pump ratio control (F7h)
    TFT_WriteData(0x20); 

    write_command(0xC0);   // Power Control 1 (C0h) 
    TFT_WriteData(0x23); // VRH[5:0] 

    write_command(0xC1);   // Power Control 2 (C1h)
    TFT_WriteData(0x10); // SAP[2:0]; BT[3:0] 

    write_command(0xC5);   // Set the VCOMH voltage. 
    TFT_WriteData(0x3E);
    TFT_WriteData(0x28); 

    write_command(0xC7);   // VCOM Control 2(C7h)
    TFT_WriteData(0x86);  

    write_command(0x36);   // Memory Access Control (36h)
    TFT_WriteData(0xE8);      // Indicate that we want the screen to update horizontally

    write_command(0x3A);   // COLMOD: Pixel Format Set (3Ah)    
    TFT_WriteData(0x55); 

    write_command(0xB1);   // Frame Rate Control (In Normal Mode/Full Colors) (B1h)  
    TFT_WriteData(0x00);  
    TFT_WriteData(0x18); 

    write_command(0xB6);   // Display Function Control (B6h) 
    TFT_WriteData(0x08); 
    TFT_WriteData(0x82);
    TFT_WriteData(0x27);  

    write_command(0xF2);   // Enable 3G (F2h)
    TFT_WriteData(0x00); 

    write_command(0x26);   // Gamma Set (26h)
    TFT_WriteData(0x01); 

    write_command(0xE0);   //Positive Gamma Correction (E0h)
    TFT_WriteData(0x0F); 
    TFT_WriteData(0x31); 
    TFT_WriteData(0x2B); 
    TFT_WriteData(0x0C); 
    TFT_WriteData(0x0E); 
    TFT_WriteData(0x08); 
    TFT_WriteData(0x4E); 
    TFT_WriteData(0xF1); 
    TFT_WriteData(0x37); 
    TFT_WriteData(0x07); 
    TFT_WriteData(0x10); 
    TFT_WriteData(0x03); 
    TFT_WriteData(0x0E); 
    TFT_WriteData(0x09); 
    TFT_WriteData(0x00); 

    write_command(0XE1);   // Negative Gamma Correction (E1h)
    TFT_WriteData(0x00); 
    TFT_WriteData(0x0E); 
    TFT_WriteData(0x14); 
    TFT_WriteData(0x03); 
    TFT_WriteData(0x11); 
    TFT_WriteData(0x07); 
    TFT_WriteData(0x31); 
    TFT_WriteData(0xC1); 
    TFT_WriteData(0x48); 
    TFT_WriteData(0x08); 
    TFT_WriteData(0x0F); 
    TFT_WriteData(0x0C); 
    TFT_WriteData(0x31); 
    TFT_WriteData(0x36); 
    TFT_WriteData(0x0F); 

    write_command(0x11);   // Exit Sleep
    e_sleep_ms(150);            // Delay of 150ms
    write_command(0x29);   // Display ON (29h)
    e_fill_screen(color);
}

//                              RENDERING
//==============================================================================
// This function fills the entire TFT screen with a single color.
// color: The color to fill the screen with.
//==============================================================================
void e_fill_screen(uint16_t color)
{
    uint8_t DH = color >> 8;        // On stocke la partie haute des bits de color dans DH
    uint8_t DL = color & 0xFF;      // On stocke la partie basse des bits de color dans DL
    
    TFT_ColumnPage(0, SCREEN_WIDTH - 1, 0, SCREEN_HEIGHT - 1); 
    
    TFT_CS = 0; 
    TFT_DC = 1;
    for(uint16_t i = 0; i < SCREEN_HEIGHT; i++) {
        for (uint16_t j = 0; j < SCREEN_WIDTH; j++) {
            SPI1_Write(DH);
            SPI1_Write(DL);
        }
    }
    TFT_CS = 1;
}

//==============================================================================
// This function draws a rectangle on the TFT display.
// pos_x: The x-coordinate of the top-left corner of the rectangle.
// pos_y: The y-coordinate of the top-left corner of the rectangle.
// width: The width of the rectangle.
// height: The height of the rectangle.
// color: The color of the rectangle.
//==============================================================================
void e_draw_rectangle(int16_t pos_x, int16_t pos_y, int16_t width, int16_t height, uint16_t color)
{   
    if (pos_x >= 0 && pos_y >= 0 && pos_x + width <= SCREEN_WIDTH && pos_y + height <= SCREEN_HEIGHT) {
        // On stocke notre couleur dans DH et DL
        uint8_t DH = color >> 8;
        uint8_t DL = color & 0xFF;
        // On calibre pour afficher
        TFT_ColumnPage((uint16_t)pos_x, (uint16_t)(pos_x + width), (uint16_t)pos_y, (uint16_t)(pos_y + height));
        // On affiche
        TFT_CS = 0; 
        TFT_DC = 1;
        for(int16_t i = pos_y; i <= pos_y + height; i++) {
            for (int16_t j = pos_x; j <= pos_x + width; j++) {
                SPI1_Write(DH);
                SPI1_Write(DL);        
            }
        }
        TFT_CS = 1;
    }
    

}

//==============================================================================
// ????
//==============================================================================
void e_set_font(int16_t font)
{
    switch (font) {
        case Courier_New_Bold_8:
            TFT_SetFont(Courier_New_Bold_8_char, 1);
            break;
        case Courier_New_Bold_10:
            TFT_SetFont(Courier_New_Bold_10_char, 1);
            break;
        case Courier_New_Bold_14:
            TFT_SetFont(Courier_New_Bold_14_char, 1);
            break;
        case Courier_New_Bold_20:
            TFT_SetFont(Courier_New_Bold_20_char, 1);
            break;
    }
}

//==============================================================================
// This function displays the current frame rate and duration of the last frame on the TFT display.
// pos_x: The x-coordinate for text placement.
// pos_y: The y-coordinate for text placement.
//==============================================================================
void e_draw_fps(int16_t pos_x, int16_t pos_y)
{
    sprintf(last_frame_duration_text, "dt: %d", last_frame_duration);
    sprintf(fps_text, "fps: %d", (uint16_t)((1.f/last_frame_duration)*1000));
    
    e_draw_text(last_frame_duration_text, pos_x, pos_y, GREEN, BLACK);
    e_draw_text(fps_text, pos_x, pos_y + 30, GREEN, BLACK);
}

//==============================================================================
// This function displays constant text at a specified position on the TFT display.
// text: The text to display.
// x: The x-coordinate for text placement.
// y: The y-coordinate for text placement.
// color1: The foreground color of the text.
// color2: The background color behind the text.
//==============================================================================
void e_draw_const_text(const char *text, int16_t x, int16_t y, uint16_t color1, uint16_t color2)
{
    //  void TFT_Text(char *buffer, uint16_t x, uint16_t y, uint16_t color1, uint16_t color2)
    //uint16_t x1 = e_safe_convert(x, "67");
    //uint16_t y1 = e_safe_convert(y, "68");
    uint16_t x1 = (uint16_t)x;
    uint16_t y1 = (uint16_t)y;
    TFT_ConstText(text, x1, y1, color1, color2);
}

//==============================================================================
// This function displays variable text at a specified position on the TFT display.
// text: The text to display.
// x: The x-coordinate for text placement.
// y: The y-coordinate for text placement.
// color1: The foreground color of the text.
// color2: The background color behind the text.
//==============================================================================
void e_draw_text(char *text, int16_t x, int16_t y, uint16_t color1, uint16_t color2)
{
    //  void TFT_Text(char *buffer, uint16_t x, uint16_t y, uint16_t color1, uint16_t color2)
    uint16_t x1 = e_safe_convert(x, "67");
    uint16_t y1 = e_safe_convert(y, "68");
    TFT_Text(text, x1, y1, color1, color2);
}

//==============================================================================
// 
//==============================================================================
void e_draw_icon(const uint16_t *buffer, int16_t x, int16_t y, uint16_t original_width, uint16_t original_height, uint16_t zoom)
{
    uint16_t x1 = e_safe_convert(x, "70");
    uint16_t y1 = e_safe_convert(y, "71");
    
    TFT_Icon(buffer, x1, y1, original_width, original_height, zoom);
}

