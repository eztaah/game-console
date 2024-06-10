#ifndef TETRIS_H
#define TETRIS_H

#include "../engine.h"

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define BLOCK_SIZE 12

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

// Calculer les décalages pour centrer le terrain de jeu
#define OFFSET_X ((SCREEN_WIDTH - (BOARD_WIDTH * BLOCK_SIZE)) / 2)
#define OFFSET_Y ((SCREEN_HEIGHT - (BOARD_HEIGHT * BLOCK_SIZE)) / 2)

typedef enum { EMPTY, O, L, J, I, T, S, Z } BlockType;

typedef struct {
    int x, y;
} Point;

typedef struct {
    BlockType type;
    Point blocks[4];
} Tetromino;

typedef struct {
    BlockType board[BOARD_HEIGHT][BOARD_WIDTH];
    Tetromino current;
    Tetromino previous;
} TetrisGame;

void init_game(TetrisGame *game);
void update_game(TetrisGame *game);
void draw_game(TetrisGame *game);

Tetromino get_random_piece(void);
int is_valid_position(TetrisGame *game, Tetromino *piece);

void move_piece_left(TetrisGame *game);
void move_piece_right(TetrisGame *game);
void rotate_piece(TetrisGame *game);
void move_piece_down(TetrisGame *game);

void check_lines(TetrisGame *game);
void clear_line(TetrisGame *game, int line);

int run_tetris_game(void);

#endif // TETRIS_H