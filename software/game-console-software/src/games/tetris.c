#include <stdlib.h> // Pour rand()
#include "../engine.h"

// add colors
#define BRIGHTBLUE         0b0000000000011111
#define BRIGHTGREEN        0b0000011111100000
#define BRIGHTCYAN         0b0000011111111111
#define BRIGHTRED          0b1111100000000000
#define BRIGHTMAGENTA      0b1111100000011111
#define BRIGHTYELLOW       0b1111111111100000

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define BLOCK_SIZE 12

// Calculer les d�calages pour centrer le terrain de jeu
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
    Tetromino next;
    int is_over;
    int score;
} TetrisGame;

void init_game(TetrisGame *game);
void update_game(TetrisGame *game);
void draw_game(TetrisGame *game);
void draw_next_piece(const Tetromino *piece, int offsetX, int offsetY);
Tetromino get_random_piece(void);
int is_valid_position(TetrisGame *game, Tetromino *piece);
void move_piece_left(TetrisGame *game);
void move_piece_right(TetrisGame *game);
void rotate_piece(TetrisGame *game);
void move_piece_down(TetrisGame *game);
void check_lines(TetrisGame *game);
void clear_line(TetrisGame *game, int line);
int is_game_over(TetrisGame *game);

// ------------------------------------------------------------

int update_next_piece_display;

Tetromino get_random_piece(void) {
    Tetromino piece;
    int random = rand() % 7; // G�n�re un nombre entre 0 et 6

    switch (random) {
        case 0: // O (Cube)
            piece.type = O;
            piece.blocks[0] = (Point){4, 0};
            piece.blocks[1] = (Point){5, 0};
            piece.blocks[2] = (Point){4, 1};
            piece.blocks[3] = (Point){5, 1};
            break;
        case 1: // L
            piece.type = L;
            piece.blocks[0] = (Point){4, 0};
            piece.blocks[1] = (Point){4, 1};
            piece.blocks[2] = (Point){4, 2};
            piece.blocks[3] = (Point){5, 2};
            break;
        case 2: // J
            piece.type = J;
            piece.blocks[0] = (Point){4, 2};
            piece.blocks[1] = (Point){5, 0};
            piece.blocks[2] = (Point){5, 1};
            piece.blocks[3] = (Point){5, 2};
            break;
        case 3: // I
            piece.type = I;
            piece.blocks[0] = (Point){3, 1};
            piece.blocks[1] = (Point){4, 1};
            piece.blocks[2] = (Point){5, 1};
            piece.blocks[3] = (Point){6, 1};
            break;
        case 4: // T
            piece.type = T;
            piece.blocks[0] = (Point){4, 0};
            piece.blocks[1] = (Point){4, 1};
            piece.blocks[2] = (Point){4, 2};
            piece.blocks[3] = (Point){5, 1};
            break;
        case 5: // S
            piece.type = S;
            piece.blocks[0] = (Point){4, 1};
            piece.blocks[1] = (Point){5, 1};
            piece.blocks[2] = (Point){5, 2};
            piece.blocks[3] = (Point){6, 2};
            break;
        case 6: // Z
            piece.type = Z;
            piece.blocks[0] = (Point){4, 2};
            piece.blocks[1] = (Point){5, 2};
            piece.blocks[2] = (Point){5, 1};
            piece.blocks[3] = (Point){6, 1};
            break;
    }

    return piece;
}

void init_game(TetrisGame *game) {
    // Initialiser les cellules de jeu � vide
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            game->board[y][x] = EMPTY;
        }
    }
    
    // R�initialiser le score
    game->score = 0;

    // Dessiner l'arri�re-plan de la surface de jeu en noir
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            e_draw_rectangle(OFFSET_X + x * BLOCK_SIZE, OFFSET_Y + y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, BLACK);
        }
    }

    // Dessiner le fond autour de la zone de jeu en blanc
    // Dessiner � gauche de la zone de jeu
    for (int y = OFFSET_Y; y < OFFSET_Y + BOARD_HEIGHT * BLOCK_SIZE; y++) {
        for (int x = 0; x < OFFSET_X; x++) {
            e_draw_rectangle(x, y, 1, 1, WHITE);
        }
    }
    // Dessiner � droite de la zone de jeu
    for (int y = OFFSET_Y; y < OFFSET_Y + BOARD_HEIGHT * BLOCK_SIZE; y++) {
        for (int x = OFFSET_X + BOARD_WIDTH * BLOCK_SIZE; x < SCREEN_WIDTH; x++) {
            e_draw_rectangle(x, y, 1, 1, WHITE);
        }
    }

    // Initialiser la premi�re pi�ce, la pi�ce pr�cedente et la pi�ce prochaine
    game->current = get_random_piece();
    game->next = get_random_piece();
    update_next_piece_display = 1;
    game->previous = game->current;
}


int is_valid_position(TetrisGame *game, Tetromino *piece) {
    for (int i = 0; i < 4; i++) {
        int x = piece->blocks[i].x;
        int y = piece->blocks[i].y;
        
        // V�rifier les limites du plateau
        if (x < 0 || x >= BOARD_WIDTH || y < 0 || y >= BOARD_HEIGHT) {
            return 0;
        }

        // V�rifier les chevauchements avec les pi�ces d�j� plac�es
        if (game->board[y][x] != EMPTY) {
            return 0;
        }
    }
    return 1;
}

void clear_line(TetrisGame *game, int line) {
    // Faire clignoter la ligne remplie en noir et blanc
    for (int i = 0; i < 5; i++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            uint16_t color = (i % 2 == 0) ? WHITE : BLACK;
            e_draw_rectangle(OFFSET_X + x * BLOCK_SIZE, OFFSET_Y + line * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, color);
        }
        e_sleep_ms(100); // Attendre 100 ms entre les clignotements
    }

    // Supprimer la ligne et d�placer les lignes au-dessus vers le bas
    for (int y = line; y > 0; y--) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            game->board[y][x] = game->board[y - 1][x];
            // Effacer visuellement l'ancienne position au-dessus
            e_draw_rectangle(OFFSET_X + x * BLOCK_SIZE, OFFSET_Y + (y + 1) * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, BLACK);
            // Si le bloc n'est pas vide, dessiner le bloc d�plac�
            if (game->board[y][x] != EMPTY) {
                e_draw_rectangle(OFFSET_X + x * BLOCK_SIZE, OFFSET_Y + y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, BRIGHTBLUE);
            } else {
                // Assurer que les espaces devenus vides sont aussi effac�s
                e_draw_rectangle(OFFSET_X + x * BLOCK_SIZE, OFFSET_Y + y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, BLACK);
            }
        }
    }

    // La ligne la plus haute doit �tre effac�e car elle n'a pas �t� trait�e dans la boucle
    for (int x = 0; x < BOARD_WIDTH; x++) {
        game->board[0][x] = EMPTY;
        e_draw_rectangle(OFFSET_X + x * BLOCK_SIZE, OFFSET_Y, BLOCK_SIZE, BLOCK_SIZE, BLACK);
    }
    
    // Mettre � jour le score pour chaque ligne effac�e
    game->score += 1;  // Augmente de 1 pour chaque ligne compl�te
    e_play_bip_sound();
}


void check_lines(TetrisGame *game) {
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        int is_full = 1;
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (game->board[y][x] == EMPTY) {
                is_full = 0;
                break;
            }
        }

        if (is_full) {
            clear_line(game, y);
        }
    }
}

void update_game(TetrisGame *game) {
    game->previous = game->current;

    for (int i = 0; i < 4; i++) {
        game->current.blocks[i].y += 1;
    }

    if (!is_valid_position(game, &game->current)) {
        for (int i = 0; i < 4; i++) {
            game->current.blocks[i].y -= 1;
        }
        for (int i = 0; i < 4; i++) {
            Point p = game->current.blocks[i];
            game->board[p.y][p.x] = game->current.type;
        }

        check_lines(game);

        game->current = game->next;        // La pi�ce actuelle devient la pi�ce suivante
        game->next = get_random_piece();   // G�n�rer une nouvelle pi�ce suivante
        update_next_piece_display = 1;
        
        // V�rifier le game over apr�s la g�n�ration d'une nouvelle pi�ce
        if (is_game_over(game)) {
            game->is_over = 1; // D�finir l'�tat de game over
        }
    }
}

void draw_piece(const Tetromino *piece, uint16_t color) {
    for (int i = 0; i < 4; i++) {
        Point p = piece->blocks[i];
        e_draw_rectangle(OFFSET_X + p.x * BLOCK_SIZE, OFFSET_Y + p.y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, color);
    }
}

void erase_piece(const Tetromino *piece) {
    draw_piece(piece, BLACK);
}

void draw_game(TetrisGame *game) {
    // Effacer la pi�ce � l'ancienne position
    erase_piece(&game->previous);

    // Dessiner la pi�ce � la nouvelle position
    draw_piece(&game->current, BRIGHTRED);

    // Dessiner le plateau de jeu
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (game->board[y][x] != EMPTY) {
                e_draw_rectangle(OFFSET_X + x * BLOCK_SIZE, OFFSET_Y + y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, BRIGHTBLUE);
            }
        }
    }

    // Afficher le score
    char score_text[20];
    sprintf(score_text, "Score: %d", game->score);
    e_draw_text(score_text, OFFSET_X + BOARD_WIDTH * BLOCK_SIZE + 10, 20, Courier_New_Bold_10, WHITE, BLACK);
    
    if (update_next_piece_display) {
        // Effacer l'ancienne pi�ce suivante
        e_draw_rectangle(OFFSET_X + BOARD_WIDTH * BLOCK_SIZE + 10, 50, 50, 50, BLACK);

        // Afficher la pi�ce suivante
        char next_text[] = "Next Piece:";
        e_draw_text(next_text, OFFSET_X + BOARD_WIDTH * BLOCK_SIZE + 10, 40, Courier_New_Bold_10, WHITE, BLACK);
        draw_next_piece(&game->next, OFFSET_X + BOARD_WIDTH * BLOCK_SIZE + 10, 60);
        update_next_piece_display = 0;
    }
}

void draw_next_piece(const Tetromino *piece, int offsetX, int offsetY) {
    for (int i = 0; i < 4; i++) {
        Point p = piece->blocks[i];
        // Dessinez chaque bloc de la pi�ce suivante, ajust� selon le besoin
        e_draw_rectangle(offsetX + (p.x - 4) * 10 + 10, offsetY + p.y * 10, 10, 10, BRIGHTRED);
    }
}

void move_piece_left(TetrisGame *game) {
    // Copier la pi�ce actuelle dans l'ancienne position
    game->previous = game->current;
    
    // Effacer l'ancienne position de la pi�ce
    erase_piece(&game->previous);

    Tetromino moved = game->current;
    for (int i = 0; i < 4; i++) {
        moved.blocks[i].x -= 1;
    }
    if (is_valid_position(game, &moved)) {
        game->current = moved;
    } else {
        // Si la nouvelle position n'est pas valide, restaurer l'ancienne position
        game->current = game->previous;
    }

    // Dessiner la pi�ce � la nouvelle position
    draw_piece(&game->current, BRIGHTRED);
}

void move_piece_right(TetrisGame *game) {
    // Copier la pi�ce actuelle dans l'ancienne position
    game->previous = game->current;

    // Effacer l'ancienne position de la pi�ce
    erase_piece(&game->previous);

    Tetromino moved = game->current;
    for (int i = 0; i < 4; i++) {
        moved.blocks[i].x += 1;
    }
    if (is_valid_position(game, &moved)) {
        game->current = moved;
    } else {
        // Si la nouvelle position n'est pas valide, restaurer l'ancienne position
        game->current = game->previous;
    }

    // Dessiner la pi�ce � la nouvelle position
    draw_piece(&game->current, BRIGHTRED);
}

void rotate_piece(TetrisGame *game) {
    // Copier la pi�ce actuelle dans l'ancienne position
    game->previous = game->current;

    // Effacer l'ancienne position de la pi�ce
    erase_piece(&game->previous);

    Tetromino rotated = game->current;
    Point pivot = rotated.blocks[1]; // Choisir le second bloc comme pivot
    for (int i = 0; i < 4; i++) {
        int x = rotated.blocks[i].x - pivot.x;
        int y = rotated.blocks[i].y - pivot.y;
        rotated.blocks[i].x = pivot.x - y;
        rotated.blocks[i].y = pivot.y + x;
    }
    if (is_valid_position(game, &rotated)) {
        game->current = rotated;
    } else {
        // Si la nouvelle position n'est pas valide, restaurer l'ancienne position
        game->current = game->previous;
    }

    // Dessiner la pi�ce � la nouvelle position
    draw_piece(&game->current, BRIGHTRED);
}

void move_piece_down(TetrisGame *game) {
    // Copier la pi�ce actuelle dans l'ancienne position
    game->previous = game->current;

    // Effacer l'ancienne position de la pi�ce
    erase_piece(&game->previous);

    Tetromino moved = game->current;
    for (int i = 0; i < 4; i++) {
        moved.blocks[i].y += 1;
    }
    if (is_valid_position(game, &moved)) {
        game->current = moved;
    } else {
        for (int i = 0; i < 4; i++) {
            game->current.blocks[i].y -= 1;
        }
        for (int i = 0; i < 4; i++) {
            Point p = game->current.blocks[i];
            game->board[p.y][p.x] = game->current.type;
        }
        check_lines(game);
        game->current = get_random_piece();
    }

    // Dessiner la pi�ce � la nouvelle position
    draw_piece(&game->current, BRIGHTRED);
}

int is_game_over(TetrisGame *game) {
    for (int i = 0; i < 4; i++) {
        Point p = game->current.blocks[i];
        if (game->board[p.y][p.x] != EMPTY) {
            return 1; // Le jeu se termine car il y a un chevauchement.
        }
    }
    return 0;
}

void run_tetris_game(void) {
    e_set_target_fps(7);

    TetrisGame game;
    game.is_over = 0;
    init_game(&game);

    while (!e_game_should_stop()) {
        if (game.is_over) {
            e_fill_screen(BLACK);
            // Afficher le message de Game Over avec les couleurs sp�cifi�es
            e_draw_text("GAME OVER", SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT / 2 - 20, Courier_New_Bold_20, RED, BLACK);
            e_draw_text("Press A to restart", SCREEN_WIDTH / 2 - 64, SCREEN_HEIGHT / 2, Courier_New_Bold_10, RED, BLACK);
            e_play_game_over_music();
            while(1){
                if (e_is_button_down(BUTTON_A)) {
                    init_game(&game); // R�initialiser le jeu
                    game.is_over = 0; // R�initialiser l'�tat de game over
                    break;
                }
            }
        } else {
            // G�rer les entr�es utilisateur pour d�placer les pi�ces
            if (e_is_button_down(BUTTON_LEFT)) {
                move_piece_left(&game);
            }
            if (e_is_button_down(BUTTON_RIGHT)) {
                move_piece_right(&game);
            }
            if (e_is_button_down(BUTTON_A)) { // Bouton pour tourner la pi�ce
                rotate_piece(&game);
            }
            if (e_is_button_down(BUTTON_DOWN)) { // Bouton pour d�placer la pi�ce rapidement vers le bas
                move_piece_down(&game);
            }

            // Mettre � jour et dessiner le jeu
            update_game(&game);
            draw_game(&game);
        }
    }

    return;
}