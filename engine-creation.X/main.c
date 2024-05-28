#include "engine.h"

//typedef struct Ball {
//    Vector x;
//    int16_t y;
//} Vector2i;

void main(void)
{
    // Initialisation
    init_game_console();
    set_target_fps(3);
    
    // Variables declaration
    int16_t score = 0; // Initialisation du score
    char scoreText[30] = {0}; // Buffer pour le texte du score
    
    // test code buzzer
    init_buzzer();
    stop_buzzer();
    play_impact_sound();
    
    // first rendering
    fill_screen(BLACK);
    sprintf(scoreText, "%d", score);
    
    // Ball creation
    Vector2i ball_position = {SCREEN_WIDTH/2, SCREEN_HEIGHT/2};
    Vector2i ball_speed = {5, 5};
    int16_t ball_size = 10;
    
    // Paddle creation
    Vector2i paddle_position = {SCREEN_WIDTH/2, SCREEN_HEIGHT - 20};
    int16_t paddle_speed = 7;
    Vector2i paddle_size = {60, 6};
    
    // Bot Paddle creation
    Vector2i bot_paddle_position = {SCREEN_WIDTH/2, SCREEN_HEIGHT - 280};
    int16_t bot_paddle_speed = 7;
    Vector2i bot_paddle_size = {60, 6};
    
    // Game loop
    while(1) {
        mystery_function();
        
        // ---------------------- LOGIC ---------------------
        // Store the old positions
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
        
        
        // Move the ball
        ball_position.x += ball_speed.x;
        ball_position.y += ball_speed.y;
        // Move the player paddle if necessay
        if(PORTBbits.RB0 == 1 && paddle_position.x - paddle_size.x/2 >= 7) {
            paddle_position.x -= paddle_speed;
        }
        if(PORTAbits.RA4 == 1 && paddle_position.x + paddle_size.x/2 <= SCREEN_WIDTH) {
            paddle_position.x += paddle_speed;
        }
        // Move the bot paddle
        bot_paddle_position.x = ball_position.x;
        
        
        // New positions
        int16_t ball_new_left = ball_position.x - ball_size/2;
        int16_t ball_new_top = ball_position.y - ball_size/2;
        int16_t ball_new_right = ball_position.x + ball_size/2;
        int16_t ball_new_bottom = ball_position.y + ball_size/2;
        
        int16_t paddle_new_left = paddle_position.x - paddle_size.x/2;
        int16_t paddle_new_top = paddle_position.y - paddle_size.y/2;
        int16_t paddle_new_right = paddle_position.x + paddle_size.x/2;
        int16_t paddle_new_bottom = paddle_position.y + paddle_size.y/2;
        
        int16_t bot_paddle_new_left = bot_paddle_position.x - bot_paddle_size.x/2;
        int16_t bot_paddle_new_top = bot_paddle_position.y - bot_paddle_size.y/2;
        int16_t bot_paddle_new_right = bot_paddle_position.x + bot_paddle_size.x/2;
        int16_t bot_paddle_new_bottom = bot_paddle_position.y + bot_paddle_size.y/2;
        
        
        // Bloquage du paddle du bot sur le cot� gauche
        if (bot_paddle_new_left <= 7) {
            bot_paddle_new_left = 7;
            bot_paddle_position.x = bot_paddle_size.x/2 + 7;
        }

        // Collision detection and correction
        if (ball_new_top <= 0) {
            ball_new_top = 0;
            ball_position.y = ball_size/2;
            ball_speed.y *= -1;
        } else if (ball_new_bottom >= SCREEN_HEIGHT) {
            fill_screen(RED);
            fill_screen(BLACK);
            draw_const_text("GAME OVER", 60, 130, RED, BLACK);
            while(1) {};
        }
        if (ball_new_left <= 0) {
            ball_new_left = 0;
            ball_position.x = ball_size/2;
            ball_speed.x *= -1;
        } else if (ball_new_right >= SCREEN_WIDTH) {
            ball_new_right = SCREEN_WIDTH;
            ball_position.x = SCREEN_WIDTH - ball_size/2;
            ball_speed.x *= -1;
        }
        
        // collision ball paddle
        if ((ball_new_bottom >= paddle_new_top) &&
            (ball_new_left < paddle_new_right) &&
            (ball_new_right > paddle_new_left)) {
            ball_speed.y *= -1;  // Inverse la direction verticale de la balle
            ball_position.y = paddle_new_top - ball_size;  // Repositionne la balle pour �viter les chevauchements
            ball_new_bottom = ball_position.y;
            
            score += 1;  // Augmente le score
            sprintf(scoreText, "%d", score);  // Met � jour le texte du score
        }
        
        // collision ball bot paddle
        if ((ball_new_top <= bot_paddle_new_bottom) &&
            (ball_new_right > bot_paddle_new_left) &&
            (ball_new_left < bot_paddle_new_right)) {
            ball_speed.y *= -1;  // Inverse la direction verticale de la balle
            ball_position.y = bot_paddle_new_bottom + ball_size;  // Repositionne la balle pour �viter les chevauchements
            ball_new_top = ball_position.y;
            ball_speed.x += 1;
            ball_speed.y += 1;
        }
        
        
        
        // ---------------------- RENDERING ---------------------
        // Draw the score
        draw_text(scoreText, 110, 10, RED, BLACK);
        
        // Draw fps
        draw_fps(5, 90);
        
        // Draw the ball
        draw_rectangle(ball_old_left, ball_old_top, ball_old_right, ball_old_bottom,
                     ball_new_left, ball_new_top, ball_new_right, ball_new_bottom,
                     BLACK, RED);
       
        // draw the paddle
        draw_rectangle(paddle_old_left, paddle_old_top, paddle_old_right, paddle_old_bottom,
             paddle_new_left, paddle_new_top, paddle_new_right, paddle_new_bottom,
             BLACK, RED);
        
        // draw the bot paddle
        draw_rectangle(bot_paddle_old_left, bot_paddle_old_top, bot_paddle_old_right, bot_paddle_old_bottom,
             bot_paddle_new_left, bot_paddle_new_top, bot_paddle_new_right, bot_paddle_new_bottom,
             BLACK, RED);
    }
}
