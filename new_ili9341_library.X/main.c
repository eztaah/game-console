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
    ball.size = 10;

    
    
    // GAME LOOP
    while(1)
    {
//        draw_rectangle(ball.position, ball.size, RED);
    }
    
    
    while(1){}
}
