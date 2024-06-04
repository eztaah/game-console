#include "raylib.h"
#include <stdio.h>
#include <stdbool.h>

//          VARAIABLES GLOBALES
typedef enum {STATE_MENU,STATE_GAME,STATE_OPTION,STATE_JEU_LANCER,STATE_GAMES_MODE}GameState; //On definis une énumération pour les différents états du programme
#define GRID_SIZE 16
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

typedef struct snake_struct
{
    Vector2 position;
    Vector2 velocity;
}snake_struct;

struct snake_struct snake[100]= {0};


//          PROGRAMME PRINCIAPAL

int main(void){
    //INITIALISATION
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "SNAKE"); //(Largeur x, hauteur Y, Nom fenetre )
    bool start = false; //on initialise la variable de debut comme fausse

    //Initialisation du serpent
    struct snake_struct snake[100]= {0};
    int snake_length = 1;

    for(int i=0; i< snake_length; i++){
        snake[i].position.x = 8 + i;
        snake[i].position.y = 8;
    }
    Vector2 collect ={11, 3}; //La balle a collectée 
    
    //Variables de Menu
 
    const char *menuElements[] = {"GAME","OPTIONS","EXIT"};//un tableau char pour les valeurs du MENU de base
    const char *menuOptions[] = {"SNAKE'S COLOR","ELEMENTS COLOR","RETURN"};
    const char *menuGame[] = {"START GAME","GAME MODE","RETURN"}; //Un mode de jeu qui augmente les fps example
    const char *GameModes[] ={"SPEED","MORE APPLES"};

    int menuElements_size = sizeof(menuElements) / sizeof(menuElements[0]);
    int menuOptions_size = sizeof(menuOptions) / sizeof(menuOptions[0]);
    int menuGame_size = sizeof(menuGame) / sizeof(menuGame[0]);

    int player_selected_option = 0;
    
    //On definis l'état initial du programme

    GameState currentState = STATE_MENU;

    //Definir les fps de base a 8
    int base_fps = 60;
    SetTargetFPS(base_fps);

    while(!WindowShouldClose()) //est effectué tant que la fenetre est ouverte
    {   
        //Lancer le jeu avec la barre d'espace uniquement

        // UpdateMusicStream(minecraft);   //ON EST DANS LE MENU DE BASE
        if(currentState==STATE_MENU){
            if(IsKeyPressed(KEY_DOWN)) player_selected_option = (player_selected_option + 1) % 3;
            if(IsKeyPressed(KEY_UP)) player_selected_option = (player_selected_option -1 + 3) % 3;

            // ON SELECTIONNE L'OPTION AVEC ENTER
            if (IsKeyPressed(KEY_ENTER)) {
                if (player_selected_option == 0) currentState = STATE_GAME;
                else if (player_selected_option == 1) currentState = STATE_OPTION;
                else if (player_selected_option == 2) break; // Quitte le programme
            }
        }
        else if (currentState == STATE_GAME){       //ON EST DANS LE MENU DU JEU
            if(IsKeyPressed(KEY_DOWN)) player_selected_option = (player_selected_option + 1) % 3;
            if(IsKeyPressed(KEY_UP)) player_selected_option = (player_selected_option -1 + 3) % 3;

            // ON SELECTIONNE L'OPTION AVEC ENTER
            if (IsKeyPressed(KEY_ENTER)) {
                if (player_selected_option == 0) //On démarre le jeu
                {   
                    currentState = STATE_JEU_LANCER ;
                } 
                else if(player_selected_option ==1) //On change de mode de jeu ?
                {
                    break;
                }
                else if(player_selected_option ==2) // Quitte le programme
                {
                    currentState =STATE_MENU;
                }
            }

        }
        else if(currentState==STATE_OPTION){    //ON EST DANS LES OPTIONS
            if(IsKeyPressed(KEY_DOWN)) player_selected_option = (player_selected_option + 1) % 3;
            if(IsKeyPressed(KEY_UP)) player_selected_option = (player_selected_option -1 + 3) % 3;

            // ON SELECTIONNE L'OPTION AVEC ENTER
             if (IsKeyPressed(KEY_ENTER)) {
                if (player_selected_option == 0) break; 
                else if(player_selected_option ==1) break;
                else if(player_selected_option ==2) currentState =STATE_MENU;
                // Quitte le programme
            }
        }
        else if(currentState==STATE_JEU_LANCER) {    //On va lancer le jeu dans ce state 
            if(IsKeyPressed(KEY_SPACE)) currentState = STATE_GAME; //Un test concluant

            int base_fps = 10;
            SetTargetFPS(base_fps);

            if(IsKeyDown(KEY_S)){
                if(!start)
                    start = true;
            }
            //snake Mouvement
            if(start)
            {
                for(int i = snake_length -1; i > 0;i--){
                    snake[i] = snake[i-1];
                }

                //Deplacement du serpent avec les fleches diretionelles
                if (IsKeyDown(KEY_RIGHT))
                {
                    snake[0].velocity = (Vector2){1, 0};
                }
                else if (IsKeyDown(KEY_LEFT))
                {
                    snake[0].velocity = (Vector2){-1, 0};
                }
                else if (IsKeyDown(KEY_UP))
                {
                    snake[0].velocity = (Vector2){0, -1};
                }
                else if (IsKeyDown(KEY_DOWN))
                {
                    snake[0].velocity = (Vector2){0, 1};
                }
                snake[0].position.x += snake[0].velocity.x;
                snake[0].position.y += snake[0].velocity.y;

            }
            //COLLISIONS,COLLECTE DE LA POMME,ET SCREEN BORDER

            if (snake[0].position.x >= GRID_SIZE)
                snake[0].position.x = 0;
            else if (snake[0].position.x < 0)
                snake[0].position.x = GRID_SIZE - 1;

            if (snake[0].position.y >= GRID_SIZE)
                snake[0].position.y = 0;
            else if (snake[0].position.y < 0)
                snake[0].position.y = GRID_SIZE - 1;

            if (snake[0].position.x == collect.x && snake[0].position.y == collect.y)
            {
                snake_length++;
                collect.x = GetRandomValue(0, GRID_SIZE - 1);
                collect.y = GetRandomValue(0, GRID_SIZE - 1);
            }

            for (int i = 1; i < snake_length; i++)      //APPUIS SUR R POUR RECOMMENCER
            {
                if (snake[0].position.x == snake[i].position.x && snake[0].position.y == snake[i].position.y || IsKeyDown(KEY_R))
                {
                    snake_length = 1;
                    snake[0].position.x = 8;
                    snake[0].position.y = 8;
                    start = false;
                }
            }

        }


        //***************************SECTION DE DESSIN*****************************//

        BeginDrawing(); //On appel la fonction qui lance la session de dessin
        ClearBackground(BLACK); //On balaie le fond d'ecran avec la couleur blanche

        if(currentState==STATE_MENU){
            //A Dessiner sur le Menu pricipal
            for (int i = 0; i < menuElements_size ; i++){
                if(i==player_selected_option){
                    DrawText(TextFormat("> %s <", menuElements[i]), 80, 36 + i * 50, 20,RED);
                } else {
                    DrawText( menuElements[i], 80, 36 + i * 50, 20,DARKGRAY);
                }
            }
        }
        else if(currentState==STATE_GAME){
            //A Dessiner sur le Menu du jeu
            for (int i = 0; i < menuGame_size ; i++){
                if(i==player_selected_option){
                    DrawText(TextFormat("> %s <", menuGame[i]), 80, 36 + i * 50, 20,RED);
                } else {
                    DrawText( menuGame[i], 80, 36 + i * 50, 20,DARKGRAY);
                }
            }
        }
        else if(currentState==STATE_OPTION){
            //A Dessiner sur le Menu options
            for (int i = 0; i < menuOptions_size ; i++){
                if(i==player_selected_option){
                    DrawText(TextFormat("> %s <", menuOptions[i]), 80, 36 + i * 50, 20,RED);
                } else {
                    DrawText( menuOptions[i], 80, 36 + i * 50, 20,DARKGRAY);
                }
            }
        }
        else if(currentState==STATE_JEU_LANCER) {
            //A Dessiner sur le Menu JEU LANCE
            // DrawText(" SNAKE",100,7,26,WHITE);
            DrawText("(SPACE TO RETURN)",2,2,4,RED);
            DrawText("(R : RESTART)",250,2,4,RED);
            //DrawText(TextFormat("TEST de la valeur de true : %d",start),90,190,15,RED);
            // DrawText("PRESS S TO PLAY" ,70,200,16,WHITE);

            for (int i = 0; i < snake_length; ++i)
            {
                Rectangle rect = {
                        snake[i].position.x * SCREEN_WIDTH / GRID_SIZE,
                        snake[i].position.y * SCREEN_HEIGHT / GRID_SIZE,
                        SCREEN_WIDTH / GRID_SIZE,
                        SCREEN_HEIGHT / GRID_SIZE
                };
                //Definir une structre a part pour differencier la tete du corps
                Rectangle rect0 = {
                        snake[0].position.x * SCREEN_WIDTH / GRID_SIZE,
                        snake[0].position.y * SCREEN_HEIGHT / GRID_SIZE,
                        SCREEN_WIDTH / GRID_SIZE,
                        SCREEN_HEIGHT / GRID_SIZE
                };
                DrawRectangleRec(rect0,WHITE);

                if (i % 2 == 1)
                {
                    DrawRectangleRec(rect, GREEN);
                }
                else
                {
                    DrawRectangleRec(rect, GREEN);
                }
            }
            //le dessin de la pomme
            DrawRectangle(collect.x * SCREEN_WIDTH / GRID_SIZE,
                        collect.y * SCREEN_HEIGHT / GRID_SIZE,
                        SCREEN_WIDTH / GRID_SIZE,
                        SCREEN_HEIGHT / GRID_SIZE,
                        RED
            );

        // START MESSAGE
        if (!start)
        {
            DrawText("PRESS S TO PLAY" ,70,200,16,WHITE);
        }

        // SCORE
        char conv[10];
        snprintf(conv, sizeof(conv), "%d", snake_length);

        DrawText(TextFormat("SCORE : %d ",snake_length), 100, 6, 26, WHITE);

        }
        EndDrawing();
    }

    CloseAudioDevice();
    CloseWindow();
    return 0;
}






