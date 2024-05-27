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
    
    // Setup boutons 
    TRISBbits.TRISB0 = 1;   // Configurer TRISB0 comme une entrée (bouton)
    TRISAbits.TRISA4 = 1;   // Configurer TRISA4 comme une entrée (bouton)

    // Ball creation
    struct Vector2 ball_position;
    ball_position.x = 240/2;
    ball_position.y = 320/2;
    struct Vector2 ball_speed;
    ball_speed.x = 1;
    ball_speed.y = 1;
    int ball_size = 20;
    
    // Game loop
    while(1) {
        // Store the old ball position
        int old_left = ball_position.x - ball_size/2;
        int old_top = ball_position.y - ball_size/2;
        int old_right = ball_position.x + ball_size/2;
        int old_bottom = ball_position.y + ball_size/2;

        // Move the ball (Check p.106 pour les interruptions)
        if(PORTBbits.RB0 == 1) {
            ball_position.x += ball_speed.x;
            ball_position.y += ball_speed.y;
        }

        // New ball position
        int new_left = ball_position.x - ball_size/2;
        int new_top = ball_position.y - ball_size/2;
        int new_right = ball_position.x + ball_size/2;
        int new_bottom = ball_position.y + ball_size/2;

        // Collision detection and correction
        if (new_top <= 0) {
            new_top = 0;
            ball_position.y = ball_size/2;
            ball_speed.y *= -1;
        } else if (new_bottom >= TFT_H) {
            new_bottom = TFT_H;
            ball_position.y = TFT_H - ball_size/2;
            ball_speed.y *= -1;
        }

        if (new_left <= 0) {
            new_left = 0;
            ball_position.x = ball_size/2;
            ball_speed.x *= -1;
        } else if (new_right >= TFT_W) {
            new_right = TFT_W;
            ball_position.x = TFT_W - ball_size/2;
            ball_speed.x *= -1;
        }

        // Clear the regions not overlapped
        // Clear left strip
        if (new_left > old_left) {
            TFT_Box(old_left, old_top, new_left, old_bottom, WHITE);
        }
        // Clear right strip
        if (new_right < old_right) {
            TFT_Box(new_right, old_top, old_right, old_bottom, WHITE);
        }
        // Clear top strip
        if (new_top > old_top) {
            TFT_Box(old_left, old_top, old_right, new_top, WHITE);
        }
        // Clear bottom strip
        if (new_bottom < old_bottom) {
            TFT_Box(old_left, new_bottom, old_right, old_bottom, WHITE);
        }

        // Draw the new ball position as a black square
        TFT_Box(new_left, new_top, new_right, new_bottom, BLACK);
    }
}



// Ancien code
/*
    // Ball creation
    TFT_SetDotSize(1);
    struct Vector2 ball_position;
    ball_position.x = 240/2;
    ball_position.y = 320/2;
    struct Vector2 ball_speed;
    ball_speed.x = 20;
    ball_speed.y = 20;
    int ball_radius = 10;
    
    int old_position_x;
    int old_position_y;
    
    // Game loop
    while(1){
        // move the ball
        old_position_x = ball_position.x;
        old_position_y = ball_position.y;
                
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
        //TFT_FillScreen(WHITE);
        TFT_CircleFill(old_position_x, old_position_y, ball_radius, WHITE);
        TFT_CircleFill(ball_position.x, ball_position.y, ball_radius, BLACK);
    }
 
*/    
