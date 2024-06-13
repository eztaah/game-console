#include "../games/snake.h"
#include "../engine.h"

int run_snake_game(void) {
    Snake snake;
    e_init_game_console();
    e_set_target_fps(13);

    initialize_game(&snake);

    while (!e_game_should_stop()) {
        update1_game(&snake);
        render_game(&snake);
    }

    return 0;
}

void update1_game(Snake *snake) {
    if (snake->game_over) {
        // Effacer le texte "GAME OVER" et r initialiser le fond
        e_fill_screen(BLACK);
        e_set_font(Courier_New_Bold_20);
        e_draw_text("GAME OVER", SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT / 2 - 20, RED, BLACK);

        while(1){
            // R initialiser les scores et les autres param tres
            if (e_is_button_down(BUTTON_A)) {
                if (snake->current_score > read_best_score()) {
                    change_best_score(snake->current_score);
                }
                reset_game(snake);  // R initialise le jeu si BUTTON_A est press 
                snake->game_over = 0;  // Revenir   l' tat normal
                return;
            }
        }
    }
    
    // Sauvegarde de la derni re position de la queue
    snake->last_tail_position = snake->position[snake->length - 1];
    
    // Mise   jour de la position du serpent
    Vector2i new_head = snake->position[0];

    switch (snake->direction) {
        case 0: new_head.y -= SNAKE_BLOCK_SIZE; break;  // Haut
        case 1: new_head.x += SNAKE_BLOCK_SIZE; break;  // Droite
        case 2: new_head.y += SNAKE_BLOCK_SIZE; break;  // Bas
        case 3: new_head.x -= SNAKE_BLOCK_SIZE; break;  // Gauche
    }
    
    // Collision avec les murs ou avec lui-m me
    if (check_collision(snake)) {
        snake->game_over = 1;  // Met   jour l' tat de game over
        e_fill_screen(BLACK);
        e_set_font(Courier_New_Bold_20);
        e_draw_text("GAME OVER", SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT / 2 - 20, RED, BLACK);
        return;
    }

    // D placement du serpent
    for (int i = snake->length - 1; i > 0; i--) {
        snake->position[i] = snake->position[i - 1];
    }
    snake->position[0] = new_head;
    
    // Calcul de la zone de collision de la t te du serpent
    int snake_head_x1 = new_head.x;
    int snake_head_y1 = new_head.y;
    int snake_head_x2 = new_head.x + SNAKE_BLOCK_SIZE;
    int snake_head_y2 = new_head.y + SNAKE_BLOCK_SIZE;

    // Calcul de la zone de collision de la pomme
    int apple_x1 = snake->apple_position.x;
    int apple_y1 = snake->apple_position.y;
    int apple_x2 = snake->apple_position.x + SNAKE_BLOCK_SIZE;
    int apple_y2 = snake->apple_position.y + SNAKE_BLOCK_SIZE;

    // V rification de l'intersection des rectangles
    if ((snake_head_x1 < apple_x2) && (snake_head_x2 > apple_x1) &&
        (snake_head_y1 < apple_y2) && (snake_head_y2 > apple_y1)) {
        snake->current_score++;
        // Affichage du score
        char score_text[10];
        sprintf(score_text, "%d", snake->current_score);
        e_set_font(Courier_New_Bold_8);
        e_draw_text(score_text, 10, 10, WHITE, BLACK);
        if (snake->length < MAX_SNAKE_LENGTH) {
            snake->position[snake->length] = new_head;
            snake->length++;
        }
        place_apple(snake); // Replace la pomme
    }


    // Contr les du jeu
    if (e_is_button_down(BUTTON_LEFT) && snake->direction != 1) snake->direction = 3;
    if (e_is_button_down(BUTTON_RIGHT) && snake->direction != 3) snake->direction = 1;
    if (e_is_button_down(BUTTON_UP) && snake->direction != 2) snake->direction = 0;
    if (e_is_button_down(BUTTON_DOWN) && snake->direction != 0) snake->direction = 2;
}

void render_game(Snake *snake) {

    // Efface l'ancienne position de la queue
    e_draw_rectangle(snake->last_tail_position.x, snake->last_tail_position.y, SNAKE_BLOCK_SIZE, SNAKE_BLOCK_SIZE, BRIGHTGREEN);
    
    // Dessiner la pomme
    e_draw_rectangle(snake->apple_position.x, snake->apple_position.y, 6, 6, RED);
    
    // Dessin du serpent
    for (int i = 0; i < snake->length; i++) {
        e_draw_rectangle(snake->position[i].x, snake->position[i].y, SNAKE_BLOCK_SIZE, SNAKE_BLOCK_SIZE, BLACK);
    }
}

int check_collision(Snake *snake) {
    Vector2i head = snake->position[0];
    
    // V rification des collisions avec la nouvelle bordure sup rieure
    if (head.x < BORDER_THICKNESS_OTHER || head.x + SNAKE_BLOCK_SIZE > SCREEN_WIDTH - BORDER_THICKNESS_OTHER ||
        head.y < BORDER_THICKNESS_TOP || head.y + SNAKE_BLOCK_SIZE > SCREEN_HEIGHT - BORDER_THICKNESS_OTHER) {
        return 1;
    }
    
    // // V rification des collisions avec le corps
    // for (int i = 1; i < snake->length; i++) {
    //     if (head.x == snake->position[i].x && head.y == snake->position[i].y) {
    //         return 1;
    //     }
    // }
    
    return 0;
}

void reset_game(Snake *snake) {
    snake->length = INITIAL_SNAKE_LENGTH;
    snake->direction = 1;  // Commence en se d pla ant vers la droite
    snake->game_over = 0;  // R initialise l' tat de game over
    snake->current_score = 0;  // R initialise le score actuel   0
    
    e_fill_screen(BRIGHTGREEN);
    
    // Dessiner la bordure sup rieure avec la nouvelle  paisseur
    e_draw_rectangle(0, 0, SCREEN_WIDTH, BORDER_THICKNESS_TOP, BLACK);  // Haut

    // Dessiner les autres bordures avec l' paisseur originale
    e_draw_rectangle(0, SCREEN_HEIGHT - BORDER_THICKNESS_OTHER, SCREEN_WIDTH, BORDER_THICKNESS_OTHER, BLACK);  // Bas
    e_draw_rectangle(0, 0, BORDER_THICKNESS_OTHER, SCREEN_HEIGHT, BLACK);  // Gauche
    e_draw_rectangle(SCREEN_WIDTH - BORDER_THICKNESS_OTHER, 0, BORDER_THICKNESS_OTHER, SCREEN_HEIGHT, BLACK);  // Droite
    
    // Affichage du meilleur score
    char best_score_text[10];
    sprintf(best_score_text, "%d", read_best_score());
    e_set_font(Courier_New_Bold_8);
    e_draw_text(best_score_text, 180, 10, WHITE, BLACK);

    for (int i = 0; i < snake->length; i++) {
        snake->position[i].x = SCREEN_WIDTH / 2 - i * SNAKE_BLOCK_SIZE;
        snake->position[i].y = SCREEN_HEIGHT / 2;
    }

    place_apple(snake);  // Assurez-vous que la pomme est plac e   l'initialisation du jeu
}

void initialize_game(Snake *snake) {
    reset_game(snake);  // Utilise reset_game pour initialiser le jeu
}

void place_apple(Snake *snake) {
    int safe;
    int max_x = (SCREEN_WIDTH - 2 * BORDER_THICKNESS_OTHER) / SNAKE_BLOCK_SIZE; // Calcul du nombre maximum de blocs en x
    int max_y = (SCREEN_HEIGHT - BORDER_THICKNESS_TOP - BORDER_THICKNESS_OTHER) / SNAKE_BLOCK_SIZE; // Calcul du nombre maximum de blocs en y

    do {
        safe = 1;
        snake->apple_position.x = (rand() % max_x) * SNAKE_BLOCK_SIZE;  // Position x est un multiple de SNAKE_BLOCK_SIZE
        snake->apple_position.y = (rand() % max_y) * SNAKE_BLOCK_SIZE + 10 ;    // Position y est un multiple de SNAKE_BLOCK_SIZE

        // V rifiez que la pomme n'apparaisse pas sur le serpent
        for (int i = 0; i < snake->length; i++) {
            if (snake->apple_position.x == snake->position[i].x && snake->apple_position.y == snake->position[i].y) {
                safe = 0;
                break;
            }
        }
        if (snake->apple_position.x <= 9 || snake->apple_position.x >= 231) {
            safe = 0;
        }
        if (snake->apple_position.y <= 29 || snake->apple_position.y >= 311) {
            safe = 0;
        }
    } while (!safe);
}

int read_best_score(void) {
    uint8_t best_score = e_read_eeprom(EEPROM_ADRESS_SNAKE);
    return (int) best_score;
}

void change_best_score(int score) {
    uint8_t data = (uint8_t) score;
    e_write_eeprom(EEPROM_ADRESS_SNAKE, data);
}
