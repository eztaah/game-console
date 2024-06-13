#ifndef SNAKE_H
#define	SNAKE_H

#include "../engine.h"

// D finitions des dimensions du serpent
#define INITIAL_SNAKE_LENGTH 5
#define MAX_SNAKE_LENGTH 50
#define SNAKE_BLOCK_SIZE 10

#define BORDER_THICKNESS_TOP 25
#define BORDER_THICKNESS_OTHER 1


// Structure pour d finir le serpent
typedef struct {
    Vector2i position[MAX_SNAKE_LENGTH];
    Vector2i last_tail_position;
    Vector2i apple_position;
    uint16_t length;
    int current_score;  
    int16_t direction;  // 0: up, 1: right, 2: down, 3: left
    int game_over;      //  tat de game over
} Snake;

void run_snake_game(void);

void initialize_game(Snake *snake);
void update1_game(Snake *snake);
void render_game(Snake *snake);

int check_collision(Snake *snake);

void reset_game(Snake *snake);

void place_apple(Snake *snake);

int read_best_score(void);
void change_best_score(int score);



#endif


