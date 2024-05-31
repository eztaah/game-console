#include "engine.h"


typedef struct Ball {
    int16_t size;
    Vector2i position;
    Vector2i speed;
} Ball;

typedef struct Paddle {
    int16_t width;
    int16_t height;
    Vector2i position;
    int16_t speed;
} Paddle;


void run_pong_game(void) {  
    
    // INITIALISATION
    set_target_fps(20);
    
    // Ball creation
    Ball ball;
    ball.size = 15;
    ball.position.x = (SCREEN_WIDTH / 2) - (ball.size / 2);
    ball.position.y = (SCREEN_HEIGHT / 2) - (ball.size / 2);
    ball.speed.x = 6;
    ball.speed.y = 6;
    
    // Player paddle creation
    Paddle player_paddle;
    player_paddle.width = 6;
    player_paddle.height = 60;
    player_paddle.position.x =  15 + (player_paddle.width / 2);
    player_paddle.position.y = (SCREEN_HEIGHT / 2) - (player_paddle.height / 2);
    player_paddle.speed = 7;
    
    // Bot paddle creation
    Paddle bot_paddle;
    bot_paddle.width = 60;
    bot_paddle.height = 6;
    bot_paddle.position.x = (SCREEN_WIDTH - 15) - (bot_paddle.width / 2);
    bot_paddle.position.y = (SCREEN_HEIGHT / 2) - (bot_paddle.height / 2);
    bot_paddle.speed = 7
    
    // first render
    fill_screen(WHITE);
    draw_rectangle(ball.position.x, ball.position.y, ball.size, ball.size, BLACK);

    
    // GAME LOOP
    while(!game_should_stop()){
                
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
        draw_fps(5, 90);

        draw_moving_rectangle(ball.position.x, ball.position.y, old_ball_position.x, old_ball_position.y, ball.size, ball.size, BLACK, WHITE);
    }
}
