#ifndef SNAKE_H
#define	SNAKE_H

#include "../engine.h"

// Définitions des dimensions du serpent
#define INITIAL_SNAKE_LENGTH 5
#define MAX_SNAKE_LENGTH 50
#define SNAKE_BLOCK_SIZE 10

#define border_thickness 1

// Structure pour définir le serpent
typedef struct {
    Vector2i position[MAX_SNAKE_LENGTH];
    Vector2i last_tail_position;
    uint16_t length;
    int16_t direction;  // 0: up, 1: right, 2: down, 3: left
    int game_over;      // État de game over
} Snake;


int run_snake_game(void);

void initialize_game(Snake *snake);
void update1_game(Snake *snake);
void render_game(Snake *snake);
int check_collision(Snake *snake);
void reset_game(Snake *snake);
void initialize_game(Snake *snake);

#endif

