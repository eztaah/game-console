#include "../engine.h"


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

void play_game_over_music(void){
    e_play_G4(400);
    e_play_G4(400);
    e_play_G4(400);
    e_play_D_4(150);
    e_play_A_4(100);
    e_play_G4(400);
    e_play_D_4(150);
    e_play_A_4(100);
    e_play_G4(400);
}

void e_draw_moving_rectangle(int16_t new_pos_x, int16_t new_pos_y, int16_t old_pos_x, int16_t old_pos_y,
                             int16_t width, int16_t height, uint16_t color, uint16_t background_color) 
{
    // Calculate the old rectangle boundaries
    int16_t old_left = old_pos_x;
    int16_t old_top = old_pos_y;
    int16_t old_right = old_pos_x + width - 1;
    int16_t old_bottom = old_pos_y + height - 1;

    // Calculate the new rectangle boundaries
    int16_t new_left = new_pos_x;
    int16_t new_top = new_pos_y;
    int16_t new_right = new_pos_x + width - 1;
    int16_t new_bottom = new_pos_y + height - 1;

    // Clear the regions not overlapped
    // Clear left strip
    if (new_left > old_left) {
        e_draw_rectangle(old_left, old_top, new_left - old_left, height, background_color);
    }
    // Clear right strip
    if (new_right < old_right) {
        e_draw_rectangle(new_right + 1, old_top, old_right - new_right, height, background_color);
    }
    // Clear top strip
    if (new_top > old_top) {
        e_draw_rectangle(old_left, old_top, width, new_top - old_top, background_color);
    }
    // Clear bottom strip
    if (new_bottom < old_bottom) {
        e_draw_rectangle(old_left, new_bottom + 1, width, old_bottom - new_bottom, background_color);
    }

    // Draw the new rectangle position
    e_draw_rectangle(new_left, new_top, width, height, color);
}


void run_pong_game(void) {  
    
    // INITIALISATION
    e_set_target_fps(60);
    
    //play_init_console_sound();
    e_fill_screen(BLACK);
    
    // Ball creation
    Ball ball;
    ball.size = 10;
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
    bot_paddle.width = 6;
    bot_paddle.height = 60;
    bot_paddle.position.x = (SCREEN_WIDTH - 15) - (bot_paddle.width / 2);
    bot_paddle.position.y = (SCREEN_HEIGHT / 2) - (bot_paddle.height / 2);
    bot_paddle.speed = 7;
    
    // first render
    e_fill_screen(BLACK);
    e_draw_rectangle(ball.position.x, ball.position.y, ball.size, ball.size, WHITE);
    e_sleep_ms(1000);

    
    // GAME LOOP
    while(!e_game_should_stop()) {
        // store old ball, player_paddle and bot_paddle positions
        Vector2i old_ball_position = {ball.position.x, ball.position.y};
        Vector2i old_player_paddle_position = {player_paddle.position.x, player_paddle.position.y};
        Vector2i old_bot_paddle_position = {bot_paddle.position.x, bot_paddle.position.y};

        // Move the ball
        ball.position.x += ball.speed.x;
        ball.position.y += ball.speed.y;
        
        // move player_paddle
        if(e_is_button_down(BUTTON_UP) && (player_paddle.position.y > player_paddle.speed)) {
            player_paddle.position.y -= player_paddle.speed;
        }
        if (e_is_button_down(BUTTON_DOWN) && (player_paddle.position.y + player_paddle.height + player_paddle.speed < SCREEN_HEIGHT)) {
            player_paddle.position.y += player_paddle.speed;
        }
        
        // move bot_paddle
        if ((ball.position.y > 0) && (ball.position.y < SCREEN_HEIGHT - ball.size - bot_paddle.height)) {
            bot_paddle.position.y = ball.position.y;
        }

        // Collision detection and correction
        if (ball.position.y <= 0) {
            ball.position.y = 0;
            ball.speed.y *= -1;
            e_play_A3(50);
        } 
        else if (ball.position.y >= SCREEN_HEIGHT - ball.size) {
            ball.position.y = SCREEN_HEIGHT - ball.size;
            ball.speed.y *= -1;
            e_play_A3(50);
        }
        if (ball.position.x <= 0) {
            e_fill_screen(BLACK);
            e_draw_const_text("GAME OVER", 90, 110, RED, BLACK);
            play_game_over_music();
            return;
        } 
        else if (ball.position.x >= SCREEN_WIDTH - ball.size) {
            ball.position.x = SCREEN_WIDTH - ball.size;
            ball.speed.x *= -1;
            e_play_A3(50);
        }
        
        // Player paddle collision
        if (ball.position.x <= player_paddle.position.x + player_paddle.width && 
            ball.position.y + ball.size >= player_paddle.position.y && 
            ball.position.y <= player_paddle.position.y + player_paddle.height) {
            ball.position.x = player_paddle.position.x + player_paddle.width;
            ball.speed.x *= -1;
            e_play_A3(50);
        }

        // Bot paddle collision
        if (ball.position.x + ball.size >= bot_paddle.position.x && 
            ball.position.y + ball.size >= bot_paddle.position.y && 
            ball.position.y <= bot_paddle.position.y + bot_paddle.height) {
            ball.position.x = bot_paddle.position.x - ball.size;
            ball.speed.x *= -1;
            e_play_A3(50);
        }
       
        // Draw on the screen
        // void e_draw_moving_rectangle(Vector2i new_position, Vector2i old_position, Vector2i size, uint16_t color, uint16_t background_color);
        e_draw_fps(110, 170);
        
        e_draw_moving_rectangle(ball.position.x, ball.position.y, old_ball_position.x, old_ball_position.y, ball.size, ball.size, WHITE, BLACK);

        e_draw_moving_rectangle(player_paddle.position.x, player_paddle.position.y, old_player_paddle_position.x, old_player_paddle_position.y, player_paddle.width, player_paddle.height, WHITE, BLACK);

        e_draw_moving_rectangle(bot_paddle.position.x, bot_paddle.position.y, old_bot_paddle_position.x, old_bot_paddle_position.y, bot_paddle.width, bot_paddle.height, WHITE, BLACK);
    }
}
