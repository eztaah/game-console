#include "tetris.h"
#include "../engine.h"
#include <stdlib.h> // Pour rand()

Tetromino get_random_piece(void) {
    Tetromino piece;
    int random = rand() % 7; // Génère un nombre entre 0 et 6

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
    // Initialiser les cellules de jeu à vide
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            game->board[y][x] = EMPTY;
        }
    }

    // Dessiner l'arrière-plan de la surface de jeu en noir
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            e_draw_rectangle(OFFSET_X + x * BLOCK_SIZE, OFFSET_Y + y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, BLACK);
        }
    }

    // Dessiner le fond autour de la zone de jeu en blanc
    // Dessiner au-dessus de la zone de jeu
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        for (int y = 0; y < OFFSET_Y; y++) {
            e_draw_rectangle(x, y, 1, 1, WHITE);
        }
    }
    // Dessiner en dessous de la zone de jeu
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        for (int y = OFFSET_Y + BOARD_HEIGHT * BLOCK_SIZE; y < SCREEN_HEIGHT; y++) {
            e_draw_rectangle(x, y, 1, 1, WHITE);
        }
    }
    // Dessiner à gauche de la zone de jeu
    for (int y = OFFSET_Y; y < OFFSET_Y + BOARD_HEIGHT * BLOCK_SIZE; y++) {
        for (int x = 0; x < OFFSET_X; x++) {
            e_draw_rectangle(x, y, 1, 1, WHITE);
        }
    }
    // Dessiner à droite de la zone de jeu
    for (int y = OFFSET_Y; y < OFFSET_Y + BOARD_HEIGHT * BLOCK_SIZE; y++) {
        for (int x = OFFSET_X + BOARD_WIDTH * BLOCK_SIZE; x < SCREEN_WIDTH; x++) {
            e_draw_rectangle(x, y, 1, 1, WHITE);
        }
    }

    // Initialiser la première pièce
    game->current = get_random_piece();
    game->previous = game->current;
}


int is_valid_position(TetrisGame *game, Tetromino *piece) {
    for (int i = 0; i < 4; i++) {
        int x = piece->blocks[i].x;
        int y = piece->blocks[i].y;
        
        // Vérifier les limites du plateau
        if (x < 0 || x >= BOARD_WIDTH || y < 0 || y >= BOARD_HEIGHT) {
            return 0;
        }

        // Vérifier les chevauchements avec les pièces déjà placées
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

    // Supprimer la ligne et déplacer les lignes au-dessus vers le bas
    for (int y = line; y > 0; y--) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            game->board[y][x] = game->board[y - 1][x];
            // Effacer visuellement l'ancienne position au-dessus
            e_draw_rectangle(OFFSET_X + x * BLOCK_SIZE, OFFSET_Y + (y + 1) * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, BLACK);
            // Si le bloc n'est pas vide, dessiner le bloc déplacé
            if (game->board[y][x] != EMPTY) {
                e_draw_rectangle(OFFSET_X + x * BLOCK_SIZE, OFFSET_Y + y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, BRIGHTBLUE);
            } else {
                // Assurer que les espaces devenus vides sont aussi effacés
                e_draw_rectangle(OFFSET_X + x * BLOCK_SIZE, OFFSET_Y + y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, BLACK);
            }
        }
    }

    // La ligne la plus haute doit être effacée car elle n'a pas été traitée dans la boucle
    for (int x = 0; x < BOARD_WIDTH; x++) {
        game->board[0][x] = EMPTY;
        e_draw_rectangle(OFFSET_X + x * BLOCK_SIZE, OFFSET_Y, BLOCK_SIZE, BLOCK_SIZE, BLACK);
    }
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
    // Copier la pièce actuelle dans l'ancienne position
    game->previous = game->current;

    // Faire tomber la pièce d'une ligne
    for (int i = 0; i < 4; i++) {
        game->current.blocks[i].y += 1;
    }

    // Si la nouvelle position n'est pas valide, restaurer l'ancienne position
    if (!is_valid_position(game, &game->current)) {
        for (int i = 0; i < 4; i++) {
            game->current.blocks[i].y -= 1;
        }
        
        // Fixer la pièce sur le plateau
        for (int i = 0; i < 4; i++) {
            Point p = game->current.blocks[i];
            game->board[p.y][p.x] = game->current.type;
        }

        // Vérifier les lignes complètes
        check_lines(game);

        // Générer une nouvelle pièce
        game->current = get_random_piece();
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
    // Effacer la pièce à l'ancienne position
    erase_piece(&game->previous);

    // Dessiner la pièce à la nouvelle position
    draw_piece(&game->current, BRIGHTRED);

    // Dessiner le plateau de jeu
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (game->board[y][x] != EMPTY) {
                e_draw_rectangle(OFFSET_X + x * BLOCK_SIZE, OFFSET_Y + y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, BRIGHTBLUE);
            }
        }
    }
}

void move_piece_left(TetrisGame *game) {
    // Copier la pièce actuelle dans l'ancienne position
    game->previous = game->current;
    
    // Effacer l'ancienne position de la pièce
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

    // Dessiner la pièce à la nouvelle position
    draw_piece(&game->current, BRIGHTRED);
}

void move_piece_right(TetrisGame *game) {
    // Copier la pièce actuelle dans l'ancienne position
    game->previous = game->current;

    // Effacer l'ancienne position de la pièce
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

    // Dessiner la pièce à la nouvelle position
    draw_piece(&game->current, BRIGHTRED);
}

void rotate_piece(TetrisGame *game) {
    // Copier la pièce actuelle dans l'ancienne position
    game->previous = game->current;

    // Effacer l'ancienne position de la pièce
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

    // Dessiner la pièce à la nouvelle position
    draw_piece(&game->current, BRIGHTRED);
}

void move_piece_down(TetrisGame *game) {
    // Copier la pièce actuelle dans l'ancienne position
    game->previous = game->current;

    // Effacer l'ancienne position de la pièce
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

    // Dessiner la pièce à la nouvelle position
    draw_piece(&game->current, BRIGHTRED);
}

int run_tetris_game(void) {
    e_init_game_console();
    e_set_target_fps(5);

    TetrisGame game;
    init_game(&game);

    while (!e_game_should_stop()) {
        // Gérer les entrées utilisateur pour déplacer les pièces
        if (e_is_button_pressed(BUTTON_LEFT)) {
            move_piece_left(&game);
        }
        if (e_is_button_pressed(BUTTON_RIGHT)) {
            move_piece_right(&game);
        }
        if (e_is_button_pressed(BUTTON_A)) { // Bouton pour tourner la pièce
            rotate_piece(&game);
        }
        if (e_is_button_pressed(BUTTON_DOWN)) { // Bouton pour déplacer la pièce rapidement vers le bas
            move_piece_down(&game);
        }

        // Mettre à jour et dessiner le jeu
        update_game(&game);
        draw_game(&game);

        e_sleep_ms(16);
    }

    return 0;
}