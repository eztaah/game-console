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
    ball.speed.x = 5;
    ball.speed.y = 5;
    ball.size = 30;
    
    draw_rectangle(ball.position.x, ball.position.y, ball.size, ball.size, BLACK);
    draw_text("hello everyone", 20, 10, BLACK, WHITE);
    


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
        else if (ball.position.y >= SCREEN_HEIGHT) {
            ball.position.y = SCREEN_WIDTH;
            ball.speed.y *= -1;
        }
        if (ball.position.x <= 0) {
            ball.position.x = 0;
            ball.speed.x *= -1;
        } 
        else if (ball.position.x >= SCREEN_WIDTH) {
            ball.position.x = SCREEN_WIDTH;
            ball.speed.x *= -1;
        }
        
        // Draw on the screen
        draw_moving_rectangle()
    }
}
