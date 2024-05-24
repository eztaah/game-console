/*
 * File:   main.c
 * Author: http://pic18fxx.blogspot.com  
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "bit_settings.h"
#include "spi_pic18f.h"
#include "ili9341.h"
#include "font.h"
#include "bitmap.h"
#include "time_delay.h"

void main(void)
{
    int i;
    char buffer1[20];
    OSCCON = 0b01110000;
    OSCTUNEbits.PLLEN = 1; // turn on the PLL 64 MHz
    ANSELA = 0; ANSELB = 0; ANSELC = 0; ANSELD = 0; ANSELE = 0;
    PORTA  = 0; PORTB  = 0; PORTC  = 0; PORTD  = 0; PORTE  = 0;
    TRISA  = 0; TRISB  = 0; TRISC  = 0; TRISD  = 0; TRISE  = 0;
    SPI1_Close();
    SPI1_Init();
    TFT_Init();
    
    struct Vector2 {
        int x;
        int y;
    };
    
    // Ball creation
    TFT_SetDotSize(1);
    struct Vector2 ball_position;
    ball_position.x = 240/2;
    ball_position.y = 320/2;
    struct Vector2 ball_speed;
    ball_speed.x = 7;
    ball_speed.y = 7;
    int ball_radius = 10;
    
    // Game loop
    while(1){
        // move the ball
        ball_position.x += ball_speed.x;
        ball_position.y += ball_speed.y;
        
        // collision detection
        if (ball_position.y + ball_radius >= TFT_H || ball_position.y - ball_radius <= 0) {
            ball_speed.y *= -1;
        }
        if (ball_position.x + ball_radius >= TFT_W || ball_position.x - ball_radius <= 0) {
            ball_speed.x *= -1;
        }
        
        // draw on the screen
        TFT_FillScreen(WHITE);
        TFT_CircleFill(ball_position.x, ball_position.y, ball_radius, BLACK);
    }
}
