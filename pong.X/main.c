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

#include "engine.h"


uint16_t temps_de_la_frame = 0;

#define DEBUG_MODE 1

void initialiser_timer(void){
    T0CON = 0x02;           // TIMER0: OFF, 16-bit, cadenc? par Tcy (1MHz/4), prescaler = 8
                            // donc f = (1MHz/4) / 8 = 31.250 kHz

    INTCONbits.TMR0IF = 0;
    INTCONbits.TMR0IE = 1;
    // T0CONbits.TMR0ON = ON;
};

void lancer_le_timer(void) {
    TMR0 = 0x0;
    T0CONbits.TMR0ON = 1;
}

uint16_t recuperer_valeur_timer(uint16_t target_delay) {
    T0CONbits.TMR0ON = 0;     // On arrete le timer
    
    uint16_t timer_value = TMR0;
    uint16_t time_passed = (uint16_t)(timer_value * 0.0016f);
    
    if (time_passed < target_delay) {
        //Delay_ms(target_delay - time_passed);
        return time_passed;
    }
    else {
        return time_passed;
    }
}

void __interrupt() timer0_ISR(void){
    if (INTCONbits.TMR0IF == 1){     // v?rifie si l'interruption est bien provoqu?e par le timer0
        printf("Le timer a overflow, ce n'est pas normal");
        exit(1);
        INTCONbits.TMR0IF = 0;      // flag d'interruption effac?
    }
}


void main(void)
{
    uint16_t i;
    char buffer1[20];
    OSCCON = 0b01110000;
    OSCTUNEbits.PLLEN = 1; // turn on the PLL 64 MHz
    ANSELA = 0; ANSELB = 0; ANSELC = 0; ANSELD = 0; ANSELE = 0;
    PORTA  = 0; PORTB  = 0; PORTC  = 0; PORTD  = 0; PORTE  = 0;
    TRISA  = 0; TRISB  = 0; TRISC  = 0; TRISD  = 0; TRISE  = 0;
    SPI1_Close();
    SPI1_Init();
    TFT_Init();
    
    initBuzzer();
    stopBuzzer();
    impactSound();
    
    initialiser_timer();

    TFT_SetFont(Courier_New_Bold_20, 1);
    TFT_SetDotSize(1);
    
    // score 
    uint16_t score = 0; // Initialisation du score
    char scoreText[30] = {0}; // Buffer pour le texte du score
    
    uint16_t time_passed = 0;
    char time_passed_text[30] = {0};
    char fps_text[30] = {0};
    
    // first rendering
    TFT_FillScreen(BLACK);
    sprintf(scoreText, "%d", score);
    
    typedef struct Vector2 {
        int16_t x;
        int16_t y;
    } Vector2;
    
    typedef struct uVector2 {
        uint16_t x;
        uint16_t y;
    } uVector2;
    
    
    // Setup boutons 
    TRISBbits.TRISB0 = 1;   // Configurer TRISB0 comme une entrï¿½e (bouton)
    TRISAbits.TRISA4 = 1;   // Configurer TRISA4 comme une entrï¿½e (bouton)

    // Ball creation
    Vector2 ball_position = {TFT_W/2, TFT_H/2};
    uVector2 ball_speed = {5, 5};
    uint16_t ball_size = 10;
    
    // Paddle creation
    Vector2 paddle_position = {TFT_W/2, TFT_H - 20};
    uint16_t paddle_speed = 7;
    Vector2 paddle_size = {60, 6};
    
    // Bot Paddle creation
    Vector2 bot_paddle_position = {TFT_W/2, TFT_H - 280};
    uint16_t bot_paddle_speed = 7;
    Vector2 bot_paddle_size = {60, 6};
    
    // Game loop
    while(1) {
        lancer_le_timer();
        
        // Store the old ball position
        int16_t ball_old_left = ball_position.x - ball_size/2;
        int16_t ball_old_top = ball_position.y - ball_size/2;
        int16_t ball_old_right = ball_position.x + ball_size/2;
        int16_t ball_old_bottom = ball_position.y + ball_size/2;
        
        int16_t paddle_old_left = paddle_position.x - paddle_size.x/2;
        int16_t paddle_old_top = paddle_position.y - paddle_size.y/2;
        int16_t paddle_old_right = paddle_position.x + paddle_size.x/2;
        int16_t paddle_old_bottom = paddle_position.y + paddle_size.y/2;
        
        int16_t bot_paddle_old_left = bot_paddle_position.x - bot_paddle_size.x/2;
        int16_t bot_paddle_old_top = bot_paddle_position.y - bot_paddle_size.y/2;
        int16_t bot_paddle_old_right = bot_paddle_position.x + bot_paddle_size.x/2;
        int16_t bot_paddle_old_bottom = bot_paddle_position.y + bot_paddle_size.y/2;

        // Move the ball (Check p.106 pour les interruptions)
        ball_position.x += ball_speed.x;
        ball_position.y += ball_speed.y;
        
  
        if(PORTBbits.RB0 == 1 && paddle_position.x - paddle_size.x/2 >= 7) {
            paddle_position.x -= paddle_speed;
        }
        
        if(PORTAbits.RA4 == 1 && paddle_position.x + paddle_size.x/2 <= TFT_W) {
            paddle_position.x += paddle_speed;
        }
        
        bot_paddle_position.x = ball_position.x;
        
        // New ball position
        int16_t ball_new_left = ball_position.x - ball_size/2;
        int16_t ball_new_top = ball_position.y - ball_size/2;
        int16_t ball_new_right = ball_position.x + ball_size/2;
        int16_t ball_new_bottom = ball_position.y + ball_size/2;
        
        // New paddle position
        int16_t paddle_new_left = paddle_position.x - paddle_size.x/2;
        int16_t paddle_new_top = paddle_position.y - paddle_size.y/2;
        int16_t paddle_new_right = paddle_position.x + paddle_size.x/2;
        int16_t paddle_new_bottom = paddle_position.y + paddle_size.y/2;
        
        // New bot paddle position
        int16_t bot_paddle_new_left = bot_paddle_position.x - bot_paddle_size.x/2;
        int16_t bot_paddle_new_top = bot_paddle_position.y - bot_paddle_size.y/2;
        int16_t bot_paddle_new_right = bot_paddle_position.x + bot_paddle_size.x/2;
        int16_t bot_paddle_new_bottom = bot_paddle_position.y + bot_paddle_size.y/2;
        
        // Bloquage du paddle du bot sur le coté gauche
        if (bot_paddle_new_left <= 7) {
            bot_paddle_new_left = 7;
            bot_paddle_position.x = bot_paddle_size.x/2 + 7;
        }

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
            ball_position.y = paddle_new_top - ball_size;  // Repositionne la balle pour ï¿½viter les chevauchements
            ball_new_bottom = ball_position.y;
            
            score += 1;  // Augmente le score
            sprintf(scoreText, "%d", score);  // Met ï¿½ jour le texte du score
        }
        
        // collision ball bot paddle
        if ((ball_new_top <= bot_paddle_new_bottom) &&
            (ball_new_right > bot_paddle_new_left) &&
            (ball_new_left < bot_paddle_new_right)) {
            ball_speed.y *= -1;  // Inverse la direction verticale de la balle
            ball_position.y = bot_paddle_new_bottom + ball_size;  // Repositionne la balle pour ï¿½viter les chevauchements
            ball_new_top = ball_position.y;
            ball_speed.x += 1;
            ball_speed.y += 1;
        }
        
        // RENDER ON THE SCREEN
        TFT_Text(scoreText, 110, 10, RED, BLACK);
        
         if (DEBUG_MODE) {
            TFT_Text(time_passed_text, 5, 90, GREEN, BLACK);
            TFT_Text(fps_text, 5, 120, GREEN, BLACK);
        }
        
        // Draw the ball
        renderRectangle(ball_old_left, ball_old_top, ball_old_right, ball_old_bottom,
                     ball_new_left, ball_new_top, ball_new_right, ball_new_bottom,
                     BLACK, RED);
       
        // draw the paddle
        renderRectangle(paddle_old_left, paddle_old_top, paddle_old_right, paddle_old_bottom,
             paddle_new_left, paddle_new_top, paddle_new_right, paddle_new_bottom,
             BLACK, RED);
        
        // draw the bot paddle
        renderRectangle(bot_paddle_old_left, bot_paddle_old_top, bot_paddle_old_right, bot_paddle_old_bottom,
             bot_paddle_new_left, bot_paddle_new_top, bot_paddle_new_right, bot_paddle_new_bottom,
             BLACK, RED);
                
        
        // Rï¿½cuperer la valeur du timer
        time_passed = recuperer_valeur_timer(16);
        sprintf(time_passed_text, "dt: %d", time_passed);
        sprintf(fps_text, "fps: %d", (uint16_t)((1.f/time_passed)*1000));
        
        
        // Wait until the next frame (here 60fps)
        //Delay_ms(16);
    }
}
