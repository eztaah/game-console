#ifndef SNAKE_H
#define	SNAKE_H

#include "../engine.h"

// D�finitions des dimensions du serpent
#define INITIAL_SNAKE_LENGTH 5
#define MAX_SNAKE_LENGTH 20
#define SNAKE_BLOCK_SIZE 10

// Structure pour d�finir le serpent
typedef struct {
    Vector2i position[MAX_SNAKE_LENGTH];
    uint16_t length;
    int16_t direction;  // 0: up, 1: right, 2: down, 3: left
    int game_over;      // Ajout d'une variable pour g�rer l'�tat de game over
} Snake;


int run_snake_game(void);

void initialize_game(Snake *snake);
void update1_game(Snake *snake);
void render_game(Snake *snake);
int check_collision(Snake *snake);
void reset_game(Snake *snake);
void initialize_game(Snake *snake);

#endif

