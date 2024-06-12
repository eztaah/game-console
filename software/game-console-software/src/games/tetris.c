#include "tetris.h"
#include "../engine.h"
#include <stdlib.h> // Pour rand()

int update_next_piece_display;

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
    
    // Réinitialiser le score
    game->score = 0;

    // Dessiner l'arrière-plan de la surface de jeu en noir
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            e_draw_rectangle(OFFSET_X + x * BLOCK_SIZE, OFFSET_Y + y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, BLACK);
        }
    }

    // Dessiner le fond autour de la zone de jeu en blanc
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

    // Initialiser la première pièce, la pièce précedente et la pièce prochaine
    game->current = get_random_piece();
    game->next = get_random_piece();
    update_next_piece_display = 1;
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
    
    // Mettre à jour le score pour chaque ligne effacée
    game->score += 1;  // Augmente de 1 pour chaque ligne complète
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

        game->current = game->next;        // La pièce actuelle devient la pièce suivante
        game->next = get_random_piece();   // Générer une nouvelle pièce suivante
        update_next_piece_display = 1;
        
        // Vérifier le game over après la génération d'une nouvelle pièce
        if (is_game_over(game)) {
            game->is_over = 1; // Définir l'état de game over
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

    // Afficher le score
    char score_text[20];
    sprintf(score_text, "Score: %d", game->score);
    e_set_font(Courier_New_Bold_10);
    e_draw_text(score_text, OFFSET_X + BOARD_WIDTH * BLOCK_SIZE + 10, 20, WHITE, BLACK);
    
    if (update_next_piece_display) {
        // Effacer l'ancienne pièce suivante
        e_draw_rectangle(OFFSET_X + BOARD_WIDTH * BLOCK_SIZE + 10, 50, 50, 50, BLACK);

        // Afficher la pièce suivante
        char next_text[] = "Next Piece:";
        e_set_font(Courier_New_Bold_10);
        e_draw_text(next_text, OFFSET_X + BOARD_WIDTH * BLOCK_SIZE + 10, 40, WHITE, BLACK);
        draw_next_piece(&game->next, OFFSET_X + BOARD_WIDTH * BLOCK_SIZE + 10, 60);
        update_next_piece_display = 0;
    }
}

void draw_next_piece(const Tetromino *piece, int offsetX, int offsetY) {
    for (int i = 0; i < 4; i++) {
        Point p = piece->blocks[i];
        // Dessinez chaque bloc de la pièce suivante, ajusté selon le besoin
        e_draw_rectangle(offsetX + (p.x - 4) * 10 + 10, offsetY + p.y * 10, 10, 10, BRIGHTRED);
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

int is_game_over(TetrisGame *game) {
    for (int i = 0; i < 4; i++) {
        Point p = game->current.blocks[i];
        if (game->board[p.y][p.x] != EMPTY) {
            return 1; // Le jeu se termine car il y a un chevauchement.
        }
    }
    return 0;
}

int run_tetris_game(void) {
    e_init_game_console();
    e_set_target_fps(7);

    TetrisGame game;
    game.is_over = 0;
    init_game(&game);

    while (!e_game_should_stop()) {
        if (game.is_over) {
            e_fill_screen(BLACK);
            // Afficher le message de Game Over avec les couleurs spécifiées
            e_set_font(Courier_New_Bold_20);
            e_draw_text("GAME OVER", SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT / 2 - 20, RED, BLACK);
            e_set_font(Courier_New_Bold_10);
            e_draw_text("Press A to restart", SCREEN_WIDTH / 2 - 64, SCREEN_HEIGHT / 2, RED, BLACK);
            while(1){
                if (e_is_button_pressed(BUTTON_A)) {
                    init_game(&game); // Réinitialiser le jeu
                    game.is_over = 0; // Réinitialiser l'état de game over
                    break;
                }
            }
        } else {
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
        }
    }

    return 0;
}