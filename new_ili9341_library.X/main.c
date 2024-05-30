/*
 * File:   main.c
 * Author:  
 */

#include "engine.h"

typedef struct Ball {
    Vector2i position;
    Vector2i speed;
    int16_t size;
} Ball;

void main(void) {
    init_game_console();
    
    // Ball creation
    Ball ball;
    ball.position.x = SCREEN_WIDTH / 2;
    ball.position.y = SCREEN_HEIGHT / 2;
    ball.speed.x = 1;
    ball.speed.y = 1;
    ball.size = 30;
    
    // first render
    fill_screen(GREEN);
    draw_rectangle(ball.position.x, ball.position.y, ball.size, ball.size, MAGENTA);
    
    draw_rectangle(30, 30, 250, 50, BLUE);
    

    // GAME LOOP
    while(1){
        // store the old ball position
        Vector2i old_ball_position = {ball.position.x, ball.position.y};
        
        // Move the ball
        ball.position.x += ball.speed.x;
        ball.position.y += ball.speed.y;

        // Collision detection and correction
        if (ball.position.y <= 0) {
            ball.position.y = 0;
            ball.speed.y *= -1;
        } 
        else if (ball.position.y >= SCREEN_HEIGHT - ball.size) {
            ball.position.y = SCREEN_HEIGHT - ball.size;
            ball.speed.y *= -1;
        }
        if (ball.position.x <= 0) {
            ball.position.x = 0;
            ball.speed.x *= -1;
        } 
        else if (ball.position.x >= SCREEN_WIDTH - ball.size) {
            ball.position.x = SCREEN_WIDTH - ball.size;
            ball.speed.x *= -1;
        }
        
        // Draw on the screen
        // void draw_moving_rectangle(Vector2i new_position, Vector2i old_position, Vector2i size, uint16_t color, uint16_t background_color);
        draw_moving_rectangle(ball.position, old_ball_position, ball.size, ball.size, BLACK, WHITE);
        
        sleep_ms(100);
    }
}
