/*
 * File:   rendering.c
 * Author: eztaah, MatteoPerez, MokhmadGUIRIEV and 21KEBY
 */

#include "../engine.h"


#define TFT_CS             LATAbits.LATA1
#define TRIS_CS            TRISAbits.TRISA1

#define TFT_RES            LATAbits.LATA2
#define TRIS_RES           TRISAbits.TRISA2

#define TFT_DC             LATAbits.LATA3
#define TRIS_DC            TRISAbits.TRISA3

#define TFT_BL             LATEbits.LATE0
#define TRIS_BL            TRISEbits.TRISE0

#define TRIS_SCK1   TRISCbits.TRISC3         
#define TRIS_SDO1   TRISCbits.TRISC5
#define TRIS_SDI1   TRISCbits.TRISC4

#define COMMAND_MODE 0
#define DATA_MODE 1

extern int16_t last_frame_duration;

// Global variables
const unsigned char *letter_font;
const unsigned char *letter_font2;
unsigned char letter_width = 0;
unsigned char letter_height = 0;
unsigned char letter_spacing = 0;

char last_frame_duration_text[30] = {0};
char fps_text[30] = {0};


//                             INTERNAL FUNCTIONS
//==============================================================================
// Lowest level function
//==============================================================================
void spi_write(uint8_t data){    
    SSP1BUF = data;
    while(!SSP1STATbits.BF) {};  
    data = SSP1BUF;
}

//==============================================================================
// This function writes a command.
//==============================================================================
void write_command(uint8_t command){   
    TFT_CS = 0;
    TFT_DC = COMMAND_MODE; // When DCX = �0�, command is selected.
    spi_write(command);
    TFT_CS = 1;
}

//==============================================================================
// This function writes a Parameter.
//==============================================================================
void write_data(uint8_t data){   
    TFT_CS = 0;
    TFT_DC = DATA_MODE; // When DCX = �1�, data is selected.
    spi_write(data);
    TFT_CS = 1;
}

//==============================================================================
// This function is used to define area of frame memory where MCU can access.
// x1: Set start column address. 
// x2: Set end column address. 
// y1: Set start page address.
// y2: Set end page address.
//==============================================================================
void set_column_page(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2)
{      
    TFT_CS = 0;
    TFT_DC = COMMAND_MODE; 
    spi_write(0x2A);
    
    TFT_DC = DATA_MODE; 
    spi_write(x1 >> 8); 
    spi_write(x1 & 0xFF);
    spi_write(x2 >> 8); 
    spi_write(x2 & 0xFF);
    
    TFT_DC = COMMAND_MODE; 
    spi_write(0x2B);
    
    TFT_DC = DATA_MODE; 
    spi_write(y1 >> 8); 
    spi_write(y1 & 0xFF); 
    spi_write(y2 >> 8); 
    spi_write(y2 & 0xFF);
    
    TFT_DC = COMMAND_MODE; 
    spi_write(0x2C);
    
    TFT_CS = 1;
}

//==============================================================================
// This function draws a character on the TFT.
// c: character to be written. 
// x: x position. Valid values: 0..240 
// y: y position. Valid values: 0..320 
// color1: Top color.
// color2: Bottom color.
//==============================================================================
void write_character(unsigned char c, uint16_t x, uint16_t y, uint16_t color, uint16_t background_color)
{
    // Initialisation
    unsigned char i, j, k;
    uint16_t p;
    p = c - 32;
    p = p * 4;
    p = p + 8; 
    letter_font = letter_font2;                
    letter_font += p;                  
    letter_width = *letter_font;
    letter_font += 2; 
    p = *letter_font;
    letter_font -= 1; 
    p = (p << 8) | *letter_font;
    letter_font = letter_font2;
    letter_font += p;
    i = letter_height;
    
    // Prepare la zone qui va etre dessinee
    set_column_page(x, x + letter_width + letter_spacing - 1, y, y + letter_height);
    
    // On dessine
    TFT_CS = 0; 
    TFT_DC = DATA_MODE; 
    while (i) {
        j = letter_width;
        while(j) {
            if(j>0){
                if(*letter_font & 0x01){
                    spi_write(color>>8);
                    spi_write(color&0xFF);
                } else {
                    spi_write(background_color>>8);
                    spi_write(background_color&0xFF);
                }
                j--;
            } else {
                letter_font++;
                break;
            }
            if (j>0) {
                if (*letter_font&0x02){
                    spi_write(color>>8);
                    spi_write(color&0xFF);
                } else {
                    spi_write(background_color>>8);
                    spi_write(background_color&0xFF);
                }
                j--;
            } else {
                letter_font++;
                break;
            }
            if (j>0) {
                if(*letter_font&0x04) {
                    spi_write(color>>8);
                    spi_write(color&0xFF);
                } else {
                    spi_write(background_color>>8);
                    spi_write(background_color&0xFF);
                }
                j--;
            } else {
                letter_font++;
                break;
            }
            if (j>0) {
                if (*letter_font&0x08) {
                    spi_write(color>>8);
                    spi_write(color&0xFF);
                } else {
                    spi_write(background_color>>8);
                    spi_write(background_color&0xFF);
                }
                j--;
            } else {
                letter_font++;
                break;
            }
            if (j>0) {
                if (*letter_font&0x10) {
                    spi_write(color>>8);
                    spi_write(color&0xFF);
                } else {
                    spi_write(background_color>>8);
                    spi_write(background_color&0xFF);
                }
                j--;
            } else {
                letter_font++;
                break;
            }
            if (j>0) {
                if (*letter_font&0x20) {
                    spi_write(color>>8);
                    spi_write(color&0xFF);
                } else {
                    spi_write(background_color>>8);
                    spi_write(background_color&0xFF);
                }
                j--;
            } else {
                letter_font++;
                break;
            }
            if (j>0) {
                if(*letter_font&0x40) {
                    spi_write(color>>8);
                    spi_write(color&0xFF);
                } else {
                    spi_write(background_color>>8);
                    spi_write(background_color&0xFF);
                }
                j--;
            } else {
                letter_font++;
                break;
            }
            if (j>0) {
                if (*letter_font&0x80) {
                    spi_write(color>>8);
                    spi_write(color&0xFF);
                } else {
                    spi_write(background_color>>8);
                    spi_write(background_color&0xFF);
                }
                j--;
            } else {
                letter_font++;
                break;
            }
            letter_font++;
        }
        
        for(k = 0; k < letter_spacing; k++) {
            spi_write(background_color >> 8); 
            spi_write(background_color & 0xFF);
        }
        i--;
    }
    TFT_CS = 1;
}

//==============================================================================
// This function fills the entire TFT screen with a single color.
// color: The color to fill the screen with.
//==============================================================================
void _e_init_screen(uint16_t color)
{
    // SPI1_Close
    SSP1CON1 &= 0xDF;
    
    // SPI1_Init
    TRIS_SCK1 = 0; 
    TRIS_SDO1 = 0; 
    TRIS_SDI1 = 1; 
    SSP1STAT = 0xC0;
    SSP1CON1 = 0x21;
    
    // ---------- TFT Init --------------------------
    TRIS_CS = 0; 
    TRIS_DC = 0; 
    TRIS_RES = 0;
    TRIS_BL = 0;
    TFT_CS = 1; 
    TFT_DC = COMMAND_MODE; 
    TFT_RES = 0;
    TFT_BL = 1;

    // Reset the screen 
    TFT_CS = 1;        
    TFT_RES = 0; 
    e_sleep_ms(50);
    TFT_RES = 1; 
    e_sleep_ms(150);
    
    // Start sending commands
    write_command(0xCB);   // Power control A (CBh)
    write_data(0x39); 
    write_data(0x2C); 
    write_data(0x00); 
    write_data(0x34); 
    write_data(0x02); 

    write_command(0xCF);   // Power control B (CFh)
    write_data(0x00); 
    write_data(0XC1); 
    write_data(0X30); 

    write_command(0xE8);   // Driver timing control A (E8h)  
    write_data(0x85); 
    write_data(0x00); 
    write_data(0x78); 

    write_command(0xEA);   // Driver timing control B (EAh)  
    write_data(0x00); 
    write_data(0x00); 

    write_command(0xED);   // Power on sequence control (EDh) 
    write_data(0x64); 
    write_data(0x03); 
    write_data(0X12); 
    write_data(0X81); 

    write_command(0xF7);   // Pump ratio control (F7h)
    write_data(0x20); 

    write_command(0xC0);   // Power Control 1 (C0h) 
    write_data(0x23); // VRH[5:0] 

    write_command(0xC1);   // Power Control 2 (C1h)
    write_data(0x10); // SAP[2:0]; BT[3:0] 

    write_command(0xC5);   // Set the VCOMH voltage. 
    write_data(0x3E);
    write_data(0x28); 

    write_command(0xC7);   // VCOM Control 2(C7h)
    write_data(0x86);  

    write_command(0x36);   // Memory Access Control (36h)
    write_data(0xE8);      // Indicate that we want the screen to update horizontally

    write_command(0x3A);   // COLMOD: Pixel Format Set (3Ah)    
    write_data(0x55); 

    write_command(0xB1);   // Frame Rate Control (In Normal Mode/Full Colors) (B1h)  
    write_data(0x00);  
    write_data(0x18); 

    write_command(0xB6);   // Display Function Control (B6h) 
    write_data(0x08); 
    write_data(0x82);
    write_data(0x27);  

    write_command(0xF2);   // Enable 3G (F2h)
    write_data(0x00); 

    write_command(0x26);   // Gamma Set (26h)
    write_data(0x01); 

    write_command(0xE0);   //Positive Gamma Correction (E0h)
    write_data(0x0F); 
    write_data(0x31); 
    write_data(0x2B); 
    write_data(0x0C); 
    write_data(0x0E); 
    write_data(0x08); 
    write_data(0x4E); 
    write_data(0xF1); 
    write_data(0x37); 
    write_data(0x07); 
    write_data(0x10); 
    write_data(0x03); 
    write_data(0x0E); 
    write_data(0x09); 
    write_data(0x00); 

    write_command(0XE1);   // Negative Gamma Correction (E1h)
    write_data(0x00); 
    write_data(0x0E); 
    write_data(0x14); 
    write_data(0x03); 
    write_data(0x11); 
    write_data(0x07); 
    write_data(0x31); 
    write_data(0xC1); 
    write_data(0x48); 
    write_data(0x08); 
    write_data(0x0F); 
    write_data(0x0C); 
    write_data(0x31); 
    write_data(0x36); 
    write_data(0x0F); 

    write_command(0x11);   // Exit Sleep
    e_sleep_ms(150);            // Delay of 150ms
    write_command(0x29);   // Display ON (29h)
    e_fill_screen(color);
}

//                              RENDERING
//==============================================================================
// This function is used to convert 24 bpp color data to 5-6-5 RGB format.
// 16 bit/pixel color order (R:5-bit, G:6-bit, B:5-bit), 65,536 colors.
// 8-8-8 to to 5-6-5 conversion.
//==============================================================================
uint16_t e_rgb_convert(uint16_t red, uint16_t green, uint16_t blue)
{
    uint16_t color = 0;
    red = (red & 0xF8) << 8; 
    green = (green & 0xFC) << 3; 
    blue = (blue & 0xF8) >> 3;
    color = red | green | blue;
    return color;
}

//==============================================================================
// This function fills the entire TFT screen with a single color.
// color: The color to fill the screen with.
//==============================================================================
void e_fill_screen(uint16_t color)
{
    uint8_t DH = color >> 8;        // On stocke la partie haute des bits de color dans DH
    uint8_t DL = color & 0xFF;      // On stocke la partie basse des bits de color dans DL
    
    set_column_page(0, SCREEN_WIDTH - 1, 0, SCREEN_HEIGHT - 1); 
    
    TFT_CS = 0; 
    TFT_DC = DATA_MODE;
    for(uint16_t i = 0; i < SCREEN_HEIGHT; i++) {
        for (uint16_t j = 0; j < SCREEN_WIDTH; j++) {
            spi_write(DH);
            spi_write(DL);
        }
    }
    TFT_CS = 1;
}

//==============================================================================
// This function draws a pixel on TFT.
// pos_x: x position. Valid values: 0..240 
// pos_y: y position. Valid values: 0..320 
// color: color parameter.
//==============================================================================
void e_draw_pixel(int16_t pos_x, int16_t pos_y, uint16_t color)
{
    if (pos_x < 0 && pos_y < 0) {
        // Setup the pixel before drawing
        set_column_page((uint16_t)pos_x, (uint16_t)pos_x, (uint16_t)pos_y, (uint16_t)pos_y);

        // Draw the pixel on the screen 
        TFT_CS = 0;    
        TFT_DC = DATA_MODE; 
        spi_write(color >> 8); 
        spi_write(color & 0xFF);
        TFT_CS = 1;   
    }
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
        
        // On prepare la zone qui va etre dessin�e
        set_column_page((uint16_t)pos_x, (uint16_t)(pos_x + width), (uint16_t)pos_y, (uint16_t)(pos_y + height));
        
        // On affiche
        TFT_CS = 0; 
        TFT_DC = DATA_MODE;
        for(int16_t i = pos_y; i <= pos_y + height; i++) {
            for (int16_t j = pos_x; j <= pos_x + width; j++) {
                spi_write(DH);
                spi_write(DL);        
            }
        }
        TFT_CS = 1;
    }
}

//==============================================================================
// This function writes text constant on TFT.
// buffer: Pointer to read all the array.
// pos_x: x position. Valid values: 0..240 
// pos_y: y position. Valid values: 0..320 
// font: the font
// color: Color of the text
// background_color: The background color.
//==============================================================================
void e_draw_text(const char *buffer, int16_t pos_x, int16_t pos_y, const uint8_t* font, uint16_t color, uint16_t background_color)
{
    int16_t start_x = pos_x;
    int16_t start_y = pos_y;

    // Set font
    letter_font2 = font;
    letter_spacing = 1;
    
    // get the height of the character
    letter_font = letter_font2;
    letter_font += 6;
    letter_height = *letter_font;
    
    // Draw text
    while (*buffer != 0) {
        if (*buffer == '\n') {
            // Move to the next line
            pos_y += letter_height;
            pos_x = start_x;  // Reset x position to the start of the line
        } else {
            // Draw the character if it's not a newline
            write_character(*buffer, (uint16_t)pos_x, (uint16_t)pos_y, color, background_color);
            pos_x += letter_width + letter_spacing;  // Move x position for the next character
        }
        buffer++;  // Move to the next character in the buffer
    }    
}


//==============================================================================
// This function displays the current frame rate and duration of the last frame on the TFT display.
// pos_x: The x-coordinate for text placement.
// pos_y: The y-coordinate for text placement.
// font: The font
//==============================================================================
void e_draw_fps(int16_t pos_x, int16_t pos_y, const uint8_t* font)
{
    sprintf(last_frame_duration_text, "dt: %d", last_frame_duration);
    sprintf(fps_text, "fps: %d", (uint16_t)((1.f/last_frame_duration)*1000));
    
    e_draw_text(last_frame_duration_text, pos_x, pos_y, font, GREEN, BLACK);
    e_draw_text(fps_text, pos_x, pos_y + 30, font, GREEN, BLACK);
}

//==============================================================================
// This function draws an icon on the screen with zoom capability.
// buffer: Pointer to the icon pixel data.
// pos_x: X position to start drawing the icon.
// pos_y: Y position to start drawing the icon.
// original_width: Original width of the icon.
// original_height: Original height of the icon.
// zoom: Zoom factor to scale the icon.
//==============================================================================
void e_draw_icon(const uint16_t *buffer, int16_t pos_x, int16_t pos_y, int16_t original_width, int16_t original_height, uint16_t zoom)
{
    if (original_width > 0 && original_height > 0) {
        // Calculate the new width and height based on the zoom value
        uint16_t new_width = (uint16_t)original_width * zoom;
        uint16_t new_height = (uint16_t)original_height * zoom;
        
        
        if (pos_x > 0 && pos_y > 0 && pos_x + (int16_t)new_width < SCREEN_WIDTH && pos_y + (int16_t)new_height < SCREEN_HEIGHT) {
            // Setup the area before drawing
            set_column_page((uint16_t)pos_x, (uint16_t)(pos_x + (int16_t)new_width - 1), (uint16_t)pos_y, (uint16_t)(pos_y + (int16_t)new_height - 1));
            
            // Draw
            TFT_CS = 0; 
            TFT_DC = DATA_MODE;
            for (int16_t i = 0; i < original_height; i++) {
                for (int16_t zy = 0; zy < zoom; zy++) {  // Repeat the same line zoom times
                    const uint16_t *currentLine = buffer + (uint16_t)i * (uint16_t)original_width;
                    for (int16_t j = 0; j < original_width; j++) {
                        for (int16_t zx = 0; zx < zoom; zx++) {  // Repeat each pixel zoom times
                            spi_write(currentLine[j] >> 8);
                            spi_write(currentLine[j] & 0xFF);
                        }
                    }
                }
            }
            TFT_CS = 1;    
        }
    }
}
