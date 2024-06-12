#include "../games/snake.h"
#include "../engine.h"

int run_snake_game(void) {
    Snake snake;
    e_init_game_console();
    e_set_target_fps(10);

    initialize_game(&snake);
    e_fill_screen(GREEN);

    while (!e_game_should_stop()) {
        update1_game(&snake);
        render_game(&snake);
        e_sleep_ms(100);  // Délai pour contrôler la vitesse du jeu
    }

    return 0;
}

void update1_game(Snake *snake) {
    if (snake->game_over) {
        if (e_is_button_pressed(BUTTON_A)) {
            reset_game(snake);  // Réinitialise le jeu si BUTTON_A est pressé
        }
        return;  // Empêche toute autre mise à jour tant que le jeu est en état de game over
    }
    
    // Mise à jour de la position du serpent
    Vector2i new_head = snake->position[0];

    switch (snake->direction) {
        case 0: new_head.y -= SNAKE_BLOCK_SIZE; break;  // Haut
        case 1: new_head.x += SNAKE_BLOCK_SIZE; break;  // Droite
        case 2: new_head.y += SNAKE_BLOCK_SIZE; break;  // Bas
        case 3: new_head.x -= SNAKE_BLOCK_SIZE; break;  // Gauche
    }

    // Collision avec les murs ou avec lui-même
    if (check_collision(snake)) {
        snake->game_over = 1;  // Met à jour l'état de game over
        e_fill_screen(BLACK);
        e_set_font(Courier_New_Bold_20);
        e_draw_text("GAME OVER", SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT / 2 - 20, RED, BLACK);
        return;
    }

    // Déplacement du serpent
    for (int i = snake->length - 1; i > 0; i--) {
        snake->position[i] = snake->position[i - 1];
    }
    snake->position[0] = new_head;

    // Contrôles du jeu
    if (e_is_button_pressed(BUTTON_LEFT) && snake->direction != 1) snake->direction = 3;
    if (e_is_button_pressed(BUTTON_RIGHT) && snake->direction != 3) snake->direction = 1;
    if (e_is_button_pressed(BUTTON_UP) && snake->direction != 2) snake->direction = 0;
    if (e_is_button_pressed(BUTTON_DOWN) && snake->direction != 0) snake->direction = 2;
}

void render_game(Snake *snake) {
    // Dessin du serpent
    for (int i = 0; i < snake->length; i++) {
        e_draw_rectangle(snake->position[i].x, snake->position[i].y, SNAKE_BLOCK_SIZE, SNAKE_BLOCK_SIZE, BLACK);
    }
}

int check_collision(Snake *snake) {
    Vector2i head = snake->position[0];
    // Vérification des collisions avec les murs
    if (head.x < 0 || head.x >= SCREEN_WIDTH || head.y < 0 || head.y >= SCREEN_HEIGHT) {
        return 1;
    }
    // Vérification des collisions avec le corps
    for (int i = 1; i < snake->length; i++) {
        if (head.x == snake->position[i].x && head.y == snake->position[i].y) {
            return 1;
        }
    }
    return 0;
}

void reset_game(Snake *snake) {
    snake->length = INITIAL_SNAKE_LENGTH;
    snake->direction = 1;  // Commence en se déplaçant vers la droite
    snake->game_over = 0;  // Réinitialise l'état de game over

    for (int i = 0; i < snake->length; i++) {
        snake->position[i].x = SCREEN_WIDTH / 2 - i * SNAKE_BLOCK_SIZE;
        snake->position[i].y = SCREEN_HEIGHT / 2;
    }
}

void initialize_game(Snake *snake) {
    reset_game(snake);  // Utilise reset_game pour initialiser le jeu
}

    