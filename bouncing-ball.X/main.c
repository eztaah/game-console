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


void renderRectangle(int old_left, int old_top, int old_right, int old_bottom,
                     int new_left, int new_top, int new_right, int new_bottom,
                     uint16_t clear_color, uint16_t fill_color) {
    // Clear the regions not overlapped
    // Clear left strip
    if (new_left > old_left) {
        TFT_Box(old_left, old_top, new_left - 1, old_bottom, clear_color);
    }
    // Clear right strip
    if (new_right < old_right) {
        TFT_Box(new_right + 1, old_top, old_right, old_bottom, clear_color);
    }
    // Clear top strip
    if (new_top > old_top) {
        TFT_Box(old_left, old_top, old_right, new_top - 1, clear_color);
    }
    // Clear bottom strip
    if (new_bottom < old_bottom) {
        TFT_Box(old_left, new_bottom + 1, old_right, old_bottom, clear_color);
    }

    // Draw the new ball position as a black square
    TFT_Box(new_left, new_top, new_right, new_bottom, fill_color);
}



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

    
    TFT_SetFont(Courier_New_Bold_20, 1);
    TFT_SetDotSize(1);
    
    // score 
    int score = 0; // Initialisation du score
    char scoreText[30] = {0}; // Buffer pour le texte du score
    
    // first rendering
    TFT_FillScreen(BLACK);
    sprintf(scoreText, "%d", score);
    
    typedef struct Vector2 {
        int x;
        int y;
    } Vector2;
    
    // Setup boutons 
    TRISBbits.TRISB0 = 1;   // Configurer TRISB0 comme une entrï¿½e (bouton)
    TRISAbits.TRISA4 = 1;   // Configurer TRISA4 comme une entrï¿½e (bouton)

    // Ball creation
    Vector2 ball_position = {TFT_W/2, TFT_H/2};
    Vector2 ball_speed = {3, 3};
    int ball_size = 10;
    
    // Paddle creation
    Vector2 paddle_position = {TFT_W/2, TFT_H - 20};
    uint8_t paddle_speed = 7;
    Vector2 paddle_size = {60, 6};
    
    // Game loop
    while(1) {
        // Store the old ball position
        int ball_old_left = ball_position.x - ball_size/2;
        int ball_old_top = ball_position.y - ball_size/2;
        int ball_old_right = ball_position.x + ball_size/2;
        int ball_old_bottom = ball_position.y + ball_size/2;
        
        int paddle_old_left = paddle_position.x - paddle_size.x/2;
        int paddle_old_top = paddle_position.y - paddle_size.y/2;
        int paddle_old_right = paddle_position.x + paddle_size.x/2;
        int paddle_old_bottom = paddle_position.y + paddle_size.y/2;

        // Move the ball (Check p.106 pour les interruptions)
        ball_position.x += ball_speed.x;
        ball_position.y += ball_speed.y;
        
  
        if(PORTBbits.RB0 == 1) {
            paddle_position.x -= paddle_speed;
        }
        
        if(PORTAbits.RA4 == 1) {
            paddle_position.x += paddle_speed;
        }

        // New ball position
        int ball_new_left = ball_position.x - ball_size/2;
        int ball_new_top = ball_position.y - ball_size/2;
        int ball_new_right = ball_position.x + ball_size/2;
        int ball_new_bottom = ball_position.y + ball_size/2;
        
        int paddle_new_left = paddle_position.x - paddle_size.x/2;
        int paddle_new_top = paddle_position.y - paddle_size.y/2;
        int paddle_new_right = paddle_position.x + paddle_size.x/2;
        int paddle_new_bottom = paddle_position.y + paddle_size.y/2;

        // Collision detection and correction
        if (ball_new_top <= 0) {
            ball_new_top = 0;
            ball_position.y = ball_size/2;
            ball_speed.y *= -1;
        } else if (ball_new_bottom >= TFT_H) {
            TFT_FillScreen(RED);
            TFT_FillScreen(BLACK);
            TFT_ConstText("GAME OVER", 60, 130, RED, BLACK);
            while(1) {};
        }
        if (ball_new_left <= 0) {
            ball_new_left = 0;
            ball_position.x = ball_size/2;
            ball_speed.x *= -1;
        } else if (ball_new_right >= TFT_W) {
            ball_new_right = TFT_W;
            ball_position.x = TFT_W - ball_size/2;
            ball_speed.x *= -1;
        }
        
        
        // collision ball paddle
        if ((ball_new_bottom >= paddle_new_top) &&
            (ball_new_left < paddle_new_right) &&
            (ball_new_right > paddle_new_left)) {
            ball_speed.y *= -1;  // Inverse la direction verticale de la balle
            ball_position.y = paddle_new_top - ball_size;  // Repositionne la balle pour éviter les chevauchements
            ball_new_bottom = ball_position.y;
            
            score += 1;  // Augmente le score
            sprintf(scoreText, "%d", score);  // Met à jour le texte du score
        }


        
        
        // RENDER ON THE SCREEN
        TFT_Text(scoreText, 110, 10, RED, BLACK);
        // Draw the ball
        renderRectangle(ball_old_left, ball_old_top, ball_old_right, ball_old_bottom,
                     ball_new_left, ball_new_top, ball_new_right, ball_new_bottom,
                     BLACK, RED);
       
        // draw the paddle
        renderRectangle(paddle_old_left, paddle_old_top, paddle_old_right, paddle_old_bottom,
             paddle_new_left, paddle_new_top, paddle_new_right, paddle_new_bottom,
             BLACK, RED);
                
        // Wait until the next frame (here 60fps)
        Delay_ms(16);
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
