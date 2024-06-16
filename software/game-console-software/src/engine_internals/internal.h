#ifndef INTERNAL_H
#define	INTERNAL_H


// GLOBAL VARIABLES
extern int16_t running;
extern int16_t target_fps;
extern int16_t target_dt;
extern int16_t last_frame_duration;


// redering.c
void _e_init_screen(uint16_t color);

// audio.c
void _e_init_buzzer(void);
void _e_stop_buzzer(void);

// inputs.c
void _e_init_buttons(void);

void _e_init_LEDs(void);


// TFT functions 
void SPI1_Init(void);
void SPI1_Close(void);
void SPI1_Write(unsigned char data);
unsigned char SPI1_Read(void);

void TFT_Init(void);
void TFT_Reset(void);
void TFT_WriteCommand(unsigned char command);
void TFT_WriteParameter(unsigned char parameter);
void TFT_ColumnPage(uint16_t x1, uint16_t x2, uint16_t y1, uint16_t y2);
unsigned char TFT_MemoryAccessControl(unsigned char frame_memory_);
uint16_t TFT_RGBConvert(uint16_t red, uint16_t green, uint16_t blue);

void TFT_Pixel(uint16_t x, uint16_t y, uint16_t color);
void TFT_Box(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void TFT_FillScreen(uint16_t color);

void TFT_SetFont(const unsigned char *font_, unsigned char letterspacing);
unsigned char TFT_CharHeight(void);
void TFT_WriteChar(unsigned char c, uint16_t x, uint16_t y, uint16_t color1, uint16_t color2);
void TFT_ConstText(const char *buffer, uint16_t x, uint16_t y, uint16_t color1, uint16_t color2);
void TFT_Text(char *buffer, uint16_t x, uint16_t y, uint16_t color1, uint16_t color2);



#endif	/* XC_HEADER_TEMPLATE_H */

