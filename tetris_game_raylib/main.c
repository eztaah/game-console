/*******************************************************************************************
*
*   raylib - classic game: tetris
*
*   Sample game developed by Marc Palau and Ramon Santamaria
*
*   This game has been created using raylib v1.3 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2015 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Some Defines
//----------------------------------------------------------------------------------
#define SQUARE_SIZE             13

#define GRID_HORIZONTAL_SIZE    10
#define GRID_VERTICAL_SIZE      15

#define LATERAL_SPEED           10
#define TURNING_SPEED           12
#define FAST_FALL_AWAIT_COUNTER 30

#define FADING_TIME             33

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef enum GridSquare { EMPTY, MOVING, FULL, BLOCK, FADING } GridSquare;

//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------
static const int screenWidth = 320;
static const int screenHeight = 240;

static bool gameOver = false;
static bool pause = false;

// Matrices
static GridSquare grid [GRID_HORIZONTAL_SIZE][GRID_VERTICAL_SIZE];
static GridSquare piece [4][4];
static GridSquare incomingPiece [4][4];

// Theese variables keep track of the active piece position
static int piecePositionX = 0;
static int piecePositionY = 0;

// Game parameters
static Color fadingColor;
//static int fallingSpeed;           // In frames

static bool beginPlay = true;      // This var is only true at the begining of the game, used for the first matrix creations
static bool pieceActive = false;
static bool detection = false;
static bool lineToDelete = false;

// Statistics
static int level = 1;
static int lines = 0;

// Counters
static int gravityMovementCounter = 0;
static int lateralMovementCounter = 0;
static int turnMovementCounter = 0;
static int fastFallMovementCounter = 0;

static int fadeLineCounter = 0;

// Based on level
static int gravitySpeed = 30;

//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------
static void InitGame(void);         // Initialize game
static void UpdateGame(void);       // Update game (one frame)
static void DrawGame(void);         // Draw game (one frame)
static void UnloadGame(void);       // Unload game
static void UpdateDrawFrame(void);  // Update and Draw (one frame)

// Additional module functions
static bool Createpiece();
static void GetRandompiece();
static void ResolveFallingMovement(bool *detection, bool *pieceActive);
static bool ResolveLateralMovement();
static bool ResolveTurnMovement();
static void CheckDetection(bool *detection);
static void CheckCompletion(bool *lineToDelete);
static int DeleteCompleteLines();

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization (Note windowTitle is unused on Android)
    //---------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "classic game: tetris");

    InitGame();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update and Draw
        //----------------------------------------------------------------------------------
        UpdateDrawFrame();
        //----------------------------------------------------------------------------------
    }
#endif
    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadGame();         // Unload loaded data (textures, sounds, models...)

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//--------------------------------------------------------------------------------------
// Game Module Functions Definition
//--------------------------------------------------------------------------------------

// Initialize game variables
void InitGame(void)
{
    // Initialize game statistics
    level = 1;
    lines = 0;

    fadingColor = BLACK;

    piecePositionX = 0;
    piecePositionY = 0;

    pause = false;

    beginPlay = true;
    pieceActive = false;
    detection = false;
    lineToDelete = false;

    // Counters
    gravityMovementCounter = 0;
    lateralMovementCounter = 0;
    turnMovementCounter = 0;
    fastFallMovementCounter = 0;

    fadeLineCounter = 0;
    gravitySpeed = 30;

    // Initialize grid matrices
    for (int i = 0; i < GRID_HORIZONTAL_SIZE; i++)
    {
        for (int j = 0; j < GRID_VERTICAL_SIZE; j++)
        {
            if ((j == GRID_VERTICAL_SIZE - 1) || (i == 0) || (i == GRID_HORIZONTAL_SIZE - 1)) grid[i][j] = BLOCK;
            else grid[i][j] = EMPTY;
        }
    }

    // Initialize incoming piece matrices
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j< 4; j++)
        {
            incomingPiece[i][j] = EMPTY;
        }
    }
}

// Update game (one frame)
void UpdateGame(void)
{
    if (!gameOver)
    {
        if (IsKeyPressed('P')) pause = !pause;

        if (!pause)
        {
            if (!lineToDelete)
            {
                if (!pieceActive)
                {
                    // Get another piece
                    pieceActive = Createpiece();

                    // We leave a little time before starting the fast falling down
                    fastFallMovementCounter = 0;
                }
                else    // Piece falling
                {
                    // Counters update
                    fastFallMovementCounter++;
                    gravityMovementCounter++;
                    lateralMovementCounter++;
                    turnMovementCounter++;

                    // We make sure to move if we've pressed the key this frame
                    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT)) lateralMovementCounter = LATERAL_SPEED;
                    if (IsKeyPressed(KEY_UP)) turnMovementCounter = TURNING_SPEED;

                    // Fall down
                    if (IsKeyDown(KEY_DOWN) && (fastFallMovementCounter >= FAST_FALL_AWAIT_COUNTER))
                    {
                        // We make sure the piece is going to fall this frame
                        gravityMovementCounter += gravitySpeed;
                    }

                    if (gravityMovementCounter >= gravitySpeed)
                    {
                        // Basic falling movement
                        CheckDetection(&detection);

                        // Check if the piece has collided with another piece or with the boundings
                        ResolveFallingMovement(&detection, &pieceActive);

                        // Check if we fullfilled a line and if so, erase the line and pull down the the lines above
                        CheckCompletion(&lineToDelete);

                        gravityMovementCounter = 0;
                    }

                    // Move laterally at player's will
                    if (lateralMovementCounter >= LATERAL_SPEED)
                    {
                        // Update the lateral movement and if success, reset the lateral counter
                        if (!ResolveLateralMovement()) lateralMovementCounter = 0;
                    }

                    // Turn the piece at player's will
                    if (turnMovementCounter >= TURNING_SPEED)
                    {
                        // Update the turning movement and reset the turning counter
                        if (ResolveTurnMovement()) turnMovementCounter = 0;
                    }
                }

                // Game over logic
                for (int j = 0; j < 2; j++)
                {
                    for (int i = 1; i < GRID_HORIZONTAL_SIZE - 1; i++)
                    {
                        if (grid[i][j] == FULL)
                        {
                            gameOver = true;
                        }
                    }
                }
            }
            else
            {
                // Animation when deleting lines
                fadeLineCounter++;

                if (fadeLineCounter%8 < 4) fadingColor = BLACK;
                else fadingColor = WHITE;

                if (fadeLineCounter >= FADING_TIME)
                {
                    int deletedLines = 0;
                    deletedLines = DeleteCompleteLines();
                    fadeLineCounter = 0;
                    lineToDelete = false;

                    lines += deletedLines;
                }
            }
        }
    }
    else
    {
        if (IsKeyPressed(KEY_B))
        {
            InitGame();
            gameOver = false;
        }
    }
}

// Draw game (one frame)
void DrawGame()
{
    BeginDrawing();

        ClearBackground(BLACK);

        if (!gameOver)
        {
            // Draw gameplay area
            Vector2 offset;
            offset.x = screenWidth/2 - (GRID_HORIZONTAL_SIZE*SQUARE_SIZE/2) - 50;
            offset.y = screenHeight/2 - ((GRID_VERTICAL_SIZE - 1)*SQUARE_SIZE/2) + SQUARE_SIZE*2;

            offset.y -= 50;     // NOTE: Harcoded position!

            int controller = offset.x;
        
            for (int j = 0; j < GRID_VERTICAL_SIZE; j++)
            {
                for (int i = 0; i < GRID_HORIZONTAL_SIZE; i++)
                {
                    // Draw each square of the grid
                    if (grid[i][j] == EMPTY)
                    {  
                        DrawLine(offset.x, offset.y + SQUARE_SIZE*2, offset.x + SQUARE_SIZE, offset.y, BLANK );
                        DrawLine(offset.x, offset.y + SQUARE_SIZE*2, offset.x, offset.y + SQUARE_SIZE, BLANK );
                        DrawLine(offset.x + SQUARE_SIZE, offset.y + SQUARE_SIZE*2, offset.x + SQUARE_SIZE, offset.y + SQUARE_SIZE, BLANK );
                        DrawLine(offset.x, offset.y + SQUARE_SIZE*3, offset.x + SQUARE_SIZE, offset.y + SQUARE_SIZE, BLANK );
                        offset.x += SQUARE_SIZE;
                    }
                    else if (grid[i][j] == FULL)
                    {    
                        DrawRectangle(offset.x, offset.y + SQUARE_SIZE*2, SQUARE_SIZE, SQUARE_SIZE, BLUE);
                        DrawLine(offset.x, offset.y + SQUARE_SIZE*2, offset.x + SQUARE_SIZE,  offset.y + SQUARE_SIZE*2, LIGHTGRAY); 
                        DrawLine(offset.x, offset.y + SQUARE_SIZE*2, offset.x,  offset.y + SQUARE_SIZE*3, LIGHTGRAY);
                        offset.x += SQUARE_SIZE;
                    }
                    else if (grid[i][j] == MOVING)
                    {     
                        DrawRectangle(offset.x, offset.y + SQUARE_SIZE*2, SQUARE_SIZE, SQUARE_SIZE, BLUE); 
                        DrawLine(offset.x, offset.y + SQUARE_SIZE*2, offset.x + SQUARE_SIZE,  offset.y + SQUARE_SIZE*2, LIGHTGRAY); 
                        DrawLine(offset.x, offset.y + SQUARE_SIZE*2, offset.x,  offset.y + SQUARE_SIZE*3, LIGHTGRAY);
                        offset.x += SQUARE_SIZE;
                    }
                    else if (grid[i][j] == BLOCK)
                    {
                        DrawRectangle(offset.x, offset.y + SQUARE_SIZE*2, SQUARE_SIZE, SQUARE_SIZE, DARKGRAY);
                        DrawRectangle(0, offset.y - SQUARE_SIZE*14, 320, SQUARE_SIZE, DARKGRAY);
                        DrawRectangle(offset.x, offset.y - SQUARE_SIZE*13, 320 - offset.x, SQUARE_SIZE, DARKGRAY);
                        DrawRectangle(0, 0, 44, 240, DARKGRAY);
                        DrawRectangle(253, 0, 67, 240, DARKGRAY);
                        DrawRectangle(175, 122, 82, 118, DARKGRAY);
                        DrawRectangle(44, 226, 131, 14, DARKGRAY);

                        dessinLigneHorizontale();
                        dessinLigneVerticale();
                        dessinBordureJeuEtInfo();
                        dessinPicXel();
                
                        offset.x += SQUARE_SIZE;
                    }
                    else if (grid[i][j] == FADING)
                    {
                        DrawRectangle(offset.x, offset.y + SQUARE_SIZE*2, SQUARE_SIZE, SQUARE_SIZE, fadingColor);
                        offset.x += SQUARE_SIZE;
                    }
                }

                offset.x = controller;
                offset.y += SQUARE_SIZE;
            }

            // Draw incoming piece (hardcoded)
            offset.x = GetScreenWidth()/2 + 28;
            offset.y = GetScreenWidth()/2 - 108;

            int controler = offset.x;

            for (int j = 0; j < 4; j++)
            {
                for (int i = 0; i < 4; i++)
                {
                    if (incomingPiece[i][j] == EMPTY)
                    {
                        DrawLine(offset.x, offset.y, offset.x + SQUARE_SIZE, offset.y, BLANK );
                        DrawLine(offset.x, offset.y, offset.x, offset.y + SQUARE_SIZE, BLANK );
                        DrawLine(offset.x + SQUARE_SIZE, offset.y, offset.x + SQUARE_SIZE, offset.y + SQUARE_SIZE, BLANK );
                        DrawLine(offset.x, offset.y + SQUARE_SIZE, offset.x + SQUARE_SIZE, offset.y + SQUARE_SIZE, BLANK );
                        offset.x += SQUARE_SIZE;
                    }
                    else if (incomingPiece[i][j] == MOVING)
                    {
                        DrawRectangle(offset.x, offset.y + 7, SQUARE_SIZE, SQUARE_SIZE, BLUE);
                        DrawLine(offset.x, offset.y + 7, offset.x + SQUARE_SIZE,  offset.y + 7, LIGHTGRAY); 
                        DrawLine(offset.x, offset.y + 7, offset.x,  offset.y + SQUARE_SIZE + 7, LIGHTGRAY);
                        offset.x += SQUARE_SIZE;
                    }
                }

                offset.x = controler;
                offset.y += SQUARE_SIZE;
            }
            
            // DrawRectangle
            DrawText("INCOMING: ", GetScreenWidth()/2 + 28, GetScreenWidth()/2 - 112, 6, WHITE);
            dessinLines();
            DrawText(TextFormat("LINES: %03i", lines), GetScreenWidth()/2 + 26, 164, 6, WHITE);

            if (pause) {
                DrawText("GAME PAUSED", screenWidth/2 - 93, screenHeight/2 - 40, 12, RED);
            }
        }
        else {
            ClearBackground(BLACK);
            DrawText("PRESS [B] TO PLAY AGAIN", GetScreenWidth()/2 - MeasureText("PRESS [B] TO PLAY AGAIN", 14)/2, GetScreenHeight()/2 - 50, 14, WHITE);
        }

    EndDrawing();
}

// Unload game variables
void UnloadGame(void)
{
    // TODO: Unload all dynamic loaded data (textures, sounds, models...)
}

// Update and Draw (one frame)
void UpdateDrawFrame(void)
{
    UpdateGame();
    DrawGame();
}

//--------------------------------------------------------------------------------------
// Additional module functions
//--------------------------------------------------------------------------------------
static bool Createpiece()
{
    piecePositionX = (int)((GRID_HORIZONTAL_SIZE - 4)/2);
    piecePositionY = 0;

    // If the game is starting and you are going to create the first piece, we create an extra one
    if (beginPlay)
    {
        GetRandompiece();
        beginPlay = false;
    }

    // We assign the incoming piece to the actual piece
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j< 4; j++)
        {
            piece[i][j] = incomingPiece[i][j];
        }
    }

    // We assign a random piece to the incoming one
    GetRandompiece();

    // Assign the piece to the grid
    for (int i = piecePositionX; i < piecePositionX + 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (piece[i - (int)piecePositionX][j] == MOVING) grid[i][j] = MOVING;
        }
    }

    return true;
}

static void GetRandompiece()
{
    int random = GetRandomValue(0, 6);

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            incomingPiece[i][j] = EMPTY;
        }
    }

    switch (random)
    {
        case 0: { incomingPiece[1][1] = MOVING; incomingPiece[2][1] = MOVING; incomingPiece[1][2] = MOVING; incomingPiece[2][2] = MOVING; } break;    //Cube
        case 1: { incomingPiece[1][0] = MOVING; incomingPiece[1][1] = MOVING; incomingPiece[1][2] = MOVING; incomingPiece[2][2] = MOVING; } break;    //L
        case 2: { incomingPiece[1][2] = MOVING; incomingPiece[2][0] = MOVING; incomingPiece[2][1] = MOVING; incomingPiece[2][2] = MOVING; } break;    //L inversa
        case 3: { incomingPiece[0][1] = MOVING; incomingPiece[1][1] = MOVING; incomingPiece[2][1] = MOVING; incomingPiece[3][1] = MOVING; } break;    //Recta
        case 4: { incomingPiece[1][0] = MOVING; incomingPiece[1][1] = MOVING; incomingPiece[1][2] = MOVING; incomingPiece[2][1] = MOVING; } break;    //Creu tallada
        case 5: { incomingPiece[1][1] = MOVING; incomingPiece[2][1] = MOVING; incomingPiece[2][2] = MOVING; incomingPiece[3][2] = MOVING; } break;    //S
        case 6: { incomingPiece[1][2] = MOVING; incomingPiece[2][2] = MOVING; incomingPiece[2][1] = MOVING; incomingPiece[3][1] = MOVING; } break;    //S inversa
    }
}

static void ResolveFallingMovement(bool *detection, bool *pieceActive)
{
    // If we finished moving this piece, we stop it
    if (*detection)
    {
        for (int j = GRID_VERTICAL_SIZE - 2; j >= 0; j--)
        {
            for (int i = 1; i < GRID_HORIZONTAL_SIZE - 1; i++)
            {
                if (grid[i][j] == MOVING)
                {
                    grid[i][j] = FULL;
                    *detection = false;
                    *pieceActive = false;
                }
            }
        }
    }
    else    // We move down the piece
    {
        for (int j = GRID_VERTICAL_SIZE - 2; j >= 0; j--)
        {
            for (int i = 1; i < GRID_HORIZONTAL_SIZE - 1; i++)
            {
                if (grid[i][j] == MOVING)
                {
                    grid[i][j+1] = MOVING;
                    grid[i][j] = EMPTY;
                }
            }
        }

        piecePositionY++;
    }
}

static bool ResolveLateralMovement()
{
    bool collision = false;

    // Piece movement
    if (IsKeyDown(KEY_LEFT))        // Move left
    {
        // Check if is possible to move to left
        for (int j = GRID_VERTICAL_SIZE - 2; j >= 0; j--)
        {
            for (int i = 1; i < GRID_HORIZONTAL_SIZE - 1; i++)
            {
                if (grid[i][j] == MOVING)
                {
                    // Check if we are touching the left wall or we have a full square at the left
                    if ((i-1 == 0) || (grid[i-1][j] == FULL)) collision = true;
                }
            }
        }

        // If able, move left
        if (!collision)
        {
            for (int j = GRID_VERTICAL_SIZE - 2; j >= 0; j--)
            {
                for (int i = 1; i < GRID_HORIZONTAL_SIZE - 1; i++)             // We check the matrix from left to right
                {
                    // Move everything to the left
                    if (grid[i][j] == MOVING)
                    {
                        grid[i-1][j] = MOVING;
                        grid[i][j] = EMPTY;
                    }
                }
            }

            piecePositionX--;
        }
    }
    else if (IsKeyDown(KEY_RIGHT))  // Move right
    {
        // Check if is possible to move to right
        for (int j = GRID_VERTICAL_SIZE - 2; j >= 0; j--)
        {
            for (int i = 1; i < GRID_HORIZONTAL_SIZE - 1; i++)
            {
                if (grid[i][j] == MOVING)
                {
                    // Check if we are touching the right wall or we have a full square at the right
                    if ((i+1 == GRID_HORIZONTAL_SIZE - 1) || (grid[i+1][j] == FULL))
                    {
                        collision = true;

                    }
                }
            }
        }

        // If able move right
        if (!collision)
        {
            for (int j = GRID_VERTICAL_SIZE - 2; j >= 0; j--)
            {
                for (int i = GRID_HORIZONTAL_SIZE - 1; i >= 1; i--)             // We check the matrix from right to left
                {
                    // Move everything to the right
                    if (grid[i][j] == MOVING)
                    {
                        grid[i+1][j] = MOVING;
                        grid[i][j] = EMPTY;
                    }
                }
            }

            piecePositionX++;
        }
    }

    return collision;
}

static bool ResolveTurnMovement()
{
    // Input for turning the piece
    if (IsKeyDown(KEY_UP))
    {
        GridSquare aux;
        bool checker = false;

        // Check all turning possibilities
        if ((grid[piecePositionX + 3][piecePositionY] == MOVING) &&
            (grid[piecePositionX][piecePositionY] != EMPTY) &&
            (grid[piecePositionX][piecePositionY] != MOVING)) checker = true;

        if ((grid[piecePositionX + 3][piecePositionY + 3] == MOVING) &&
            (grid[piecePositionX + 3][piecePositionY] != EMPTY) &&
            (grid[piecePositionX + 3][piecePositionY] != MOVING)) checker = true;

        if ((grid[piecePositionX][piecePositionY + 3] == MOVING) &&
            (grid[piecePositionX + 3][piecePositionY + 3] != EMPTY) &&
            (grid[piecePositionX + 3][piecePositionY + 3] != MOVING)) checker = true;

        if ((grid[piecePositionX][piecePositionY] == MOVING) &&
            (grid[piecePositionX][piecePositionY + 3] != EMPTY) &&
            (grid[piecePositionX][piecePositionY + 3] != MOVING)) checker = true;

        if ((grid[piecePositionX + 1][piecePositionY] == MOVING) &&
            (grid[piecePositionX][piecePositionY + 2] != EMPTY) &&
            (grid[piecePositionX][piecePositionY + 2] != MOVING)) checker = true;

        if ((grid[piecePositionX + 3][piecePositionY + 1] == MOVING) &&
            (grid[piecePositionX + 1][piecePositionY] != EMPTY) &&
            (grid[piecePositionX + 1][piecePositionY] != MOVING)) checker = true;

        if ((grid[piecePositionX + 2][piecePositionY + 3] == MOVING) &&
            (grid[piecePositionX + 3][piecePositionY + 1] != EMPTY) &&
            (grid[piecePositionX + 3][piecePositionY + 1] != MOVING)) checker = true;

        if ((grid[piecePositionX][piecePositionY + 2] == MOVING) &&
            (grid[piecePositionX + 2][piecePositionY + 3] != EMPTY) &&
            (grid[piecePositionX + 2][piecePositionY + 3] != MOVING)) checker = true;

        if ((grid[piecePositionX + 2][piecePositionY] == MOVING) &&
            (grid[piecePositionX][piecePositionY + 1] != EMPTY) &&
            (grid[piecePositionX][piecePositionY + 1] != MOVING)) checker = true;

        if ((grid[piecePositionX + 3][piecePositionY + 2] == MOVING) &&
            (grid[piecePositionX + 2][piecePositionY] != EMPTY) &&
            (grid[piecePositionX + 2][piecePositionY] != MOVING)) checker = true;

        if ((grid[piecePositionX + 1][piecePositionY + 3] == MOVING) &&
            (grid[piecePositionX + 3][piecePositionY + 2] != EMPTY) &&
            (grid[piecePositionX + 3][piecePositionY + 2] != MOVING)) checker = true;

        if ((grid[piecePositionX][piecePositionY + 1] == MOVING) &&
            (grid[piecePositionX + 1][piecePositionY + 3] != EMPTY) &&
            (grid[piecePositionX + 1][piecePositionY + 3] != MOVING)) checker = true;

        if ((grid[piecePositionX + 1][piecePositionY + 1] == MOVING) &&
            (grid[piecePositionX + 1][piecePositionY + 2] != EMPTY) &&
            (grid[piecePositionX + 1][piecePositionY + 2] != MOVING)) checker = true;

        if ((grid[piecePositionX + 2][piecePositionY + 1] == MOVING) &&
            (grid[piecePositionX + 1][piecePositionY + 1] != EMPTY) &&
            (grid[piecePositionX + 1][piecePositionY + 1] != MOVING)) checker = true;

        if ((grid[piecePositionX + 2][piecePositionY + 2] == MOVING) &&
            (grid[piecePositionX + 2][piecePositionY + 1] != EMPTY) &&
            (grid[piecePositionX + 2][piecePositionY + 1] != MOVING)) checker = true;

        if ((grid[piecePositionX + 1][piecePositionY + 2] == MOVING) &&
            (grid[piecePositionX + 2][piecePositionY + 2] != EMPTY) &&
            (grid[piecePositionX + 2][piecePositionY + 2] != MOVING)) checker = true;

        if (!checker)
        {
            aux = piece[0][0];
            piece[0][0] = piece[3][0];
            piece[3][0] = piece[3][3];
            piece[3][3] = piece[0][3];
            piece[0][3] = aux;

            aux = piece[1][0];
            piece[1][0] = piece[3][1];
            piece[3][1] = piece[2][3];
            piece[2][3] = piece[0][2];
            piece[0][2] = aux;

            aux = piece[2][0];
            piece[2][0] = piece[3][2];
            piece[3][2] = piece[1][3];
            piece[1][3] = piece[0][1];
            piece[0][1] = aux;

            aux = piece[1][1];
            piece[1][1] = piece[2][1];
            piece[2][1] = piece[2][2];
            piece[2][2] = piece[1][2];
            piece[1][2] = aux;
        }

        for (int j = GRID_VERTICAL_SIZE - 2; j >= 0; j--)
        {
            for (int i = 1; i < GRID_HORIZONTAL_SIZE - 1; i++)
            {
                if (grid[i][j] == MOVING)
                {
                    grid[i][j] = EMPTY;
                }
            }
        }

        for (int i = piecePositionX; i < piecePositionX + 4; i++)
        {
            for (int j = piecePositionY; j < piecePositionY + 4; j++)
            {
                if (piece[i - piecePositionX][j - piecePositionY] == MOVING)
                {
                    grid[i][j] = MOVING;
                }
            }
        }

        return true;
    }

    return false;
}

static void CheckDetection(bool *detection)
{
    for (int j = GRID_VERTICAL_SIZE - 2; j >= 0; j--)
    {
        for (int i = 1; i < GRID_HORIZONTAL_SIZE - 1; i++)
        {
            if ((grid[i][j] == MOVING) && ((grid[i][j+1] == FULL) || (grid[i][j+1] == BLOCK))) *detection = true;
        }
    }
}

static void CheckCompletion(bool *lineToDelete)
{
    int calculator = 0;

    for (int j = GRID_VERTICAL_SIZE - 2; j >= 0; j--)
    {
        calculator = 0;
        for (int i = 1; i < GRID_HORIZONTAL_SIZE - 1; i++)
        {
            // Count each square of the line
            if (grid[i][j] == FULL)
            {
                calculator++;
            }

            // Check if we completed the whole line
            if (calculator == GRID_HORIZONTAL_SIZE - 2)
            {
                *lineToDelete = true;
                calculator = 0;
                // points++;

                // Mark the completed line
                for (int z = 1; z < GRID_HORIZONTAL_SIZE - 1; z++)
                {
                    grid[z][j] = FADING;
                }
            }
        }
    }
}

static int DeleteCompleteLines()
{
    int deletedLines = 0;

    // Erase the completed line
    for (int j = GRID_VERTICAL_SIZE - 2; j >= 0; j--)
    {
        while (grid[1][j] == FADING)
        {
            for (int i = 1; i < GRID_HORIZONTAL_SIZE - 1; i++)
            {
                grid[i][j] = EMPTY;
            }

            for (int j2 = j-1; j2 >= 0; j2--)
            {
                for (int i2 = 1; i2 < GRID_HORIZONTAL_SIZE - 1; i2++)
                {
                    if (grid[i2][j2] == FULL)
                    {
                        grid[i2][j2+1] = FULL;
                        grid[i2][j2] = EMPTY;
                    }
                    else if (grid[i2][j2] == FADING)
                    {
                        grid[i2][j2+1] = FADING;
                        grid[i2][j2] = EMPTY;
                    }
                }
            }

             deletedLines++;
        }
    }

    return deletedLines;
}

static void dessinLigneVerticale()
{
    DrawLine(6, 0, 6, 240, BLACK);
    DrawLine(19, 0, 19, 240, BLACK);
    DrawLine(32, 0, 32, 240, BLACK);
    DrawLine(45, 0, 45, 240, BLACK);
    DrawLine(58, 0, 58, 240, BLACK);
    DrawLine(162, 0, 162, 240, BLACK);
    DrawLine(175, 0, 175, 240, BLACK);
    DrawLine(253, 0, 253, 240, BLACK);
    DrawLine(266, 0, 266, 240, BLACK);
    DrawLine(279, 0, 279, 240, BLACK);
    DrawLine(292, 0, 292, 240, BLACK);
    DrawLine(305, 0, 305, 240, BLACK);
    DrawLine(318, 0, 318, 240, BLACK);

    DrawLine(71, 0, 71, 30, BLACK);
    DrawLine(84, 0, 84, 30, BLACK);
    DrawLine(97, 0, 97, 30, BLACK);
    DrawLine(110, 0, 110, 30, BLACK);
    DrawLine(123, 0, 123, 30, BLACK);
    DrawLine(136, 0, 136, 30, BLACK);
    DrawLine(149, 0, 149, 30, BLACK);
    DrawLine(188, 0, 188, 30, BLACK);
    DrawLine(201, 0, 201, 30, BLACK);
    DrawLine(214, 0, 214, 30, BLACK);
    DrawLine(227, 0, 227, 30, BLACK);
    DrawLine(240, 0, 240, 30, BLACK);

    DrawLine(71, 213, 71, 240, BLACK);
    DrawLine(84, 213, 84, 240, BLACK);
    DrawLine(97, 213, 97, 240, BLACK);
    DrawLine(110, 213, 110, 240, BLACK);
    DrawLine(123, 213, 123, 240, BLACK);
    DrawLine(136, 213, 136, 240, BLACK);
    DrawLine(149, 213, 149, 240, BLACK);

    DrawLine(188, 122, 188, 240, BLACK);
    DrawLine(201, 122, 201, 240, BLACK);
    DrawLine(214, 122, 214, 240, BLACK);
    DrawLine(227, 122, 227, 240, BLACK);
    DrawLine(240, 122, 240, 240, BLACK);
}


static void dessinLigneHorizontale()
{
    DrawLine(0, 213, 320, 213, BLACK);
    DrawLine(0, 226, 320, 226, BLACK);
    DrawLine(0, 239, 320, 239, BLACK);
    DrawLine(0, 30, 320, 30, BLACK);
    DrawLine(0, 17, 320, 17, BLACK);
    DrawLine(0, 4, 320, 4, BLACK);

    DrawLine(0, 200, 58, 200, BLACK);
    DrawLine(0, 187, 58, 187, BLACK);
    DrawLine(0, 174, 58, 174, BLACK);
    DrawLine(0, 161, 58, 161, BLACK);
    DrawLine(0, 148, 58, 148, BLACK);
    DrawLine(0, 135, 58, 135, BLACK);
    DrawLine(0, 122, 58, 122, BLACK);
    DrawLine(0, 109, 58, 109, BLACK);
    DrawLine(0, 96, 58, 96, BLACK);
    DrawLine(0, 83, 58, 83, BLACK);
    DrawLine(0, 70, 58, 70, BLACK);
    DrawLine(0, 57, 58, 57, BLACK);
    DrawLine(0, 44, 58, 44, BLACK);

    DrawLine(162, 200, 175, 200, BLACK);
    DrawLine(162, 187, 175, 187, BLACK);
    DrawLine(162, 174, 175, 174, BLACK);
    DrawLine(162, 161, 175, 161, BLACK);
    DrawLine(162, 148, 175, 148, BLACK);
    DrawLine(162, 135, 175, 135, BLACK);
    DrawLine(162, 122, 253, 122, BLACK);
    DrawLine(162, 109, 175, 109, BLACK);
    DrawLine(162, 96, 175, 96, BLACK);
    DrawLine(162, 83, 175, 83, BLACK);
    DrawLine(162, 70, 175, 70, BLACK);
    DrawLine(162, 57, 175, 57, BLACK);
    DrawLine(162, 44, 175, 44, BLACK);

    DrawLine(175, 200, 320, 200, BLACK);
    DrawLine(175, 187, 320, 187, BLACK);
    DrawLine(175, 174, 320, 174, BLACK);
    DrawLine(175, 161, 320, 161, BLACK);
    DrawLine(175, 148, 320, 148, BLACK);
    DrawLine(175, 135, 320, 135, BLACK);
    DrawLine(175, 122, 320, 122, BLACK);
    DrawLine(253, 109, 320, 109, BLACK);
    DrawLine(253, 96, 320, 96, BLACK);
    DrawLine(253, 83, 320, 83, BLACK);
    DrawLine(253, 70, 320, 70, BLACK);
    DrawLine(253, 57, 320, 57, BLACK);
    DrawLine(253, 44, 320, 44, BLACK);
}

static void dessinBordureJeuEtInfo()
{
    DrawRectangle(175, 31, 77, 4, LIGHTGRAY);
    DrawRectangle(175, 31, 4, 91, LIGHTGRAY);
    DrawRectangle(175, 118, 77, 4, LIGHTGRAY);
    DrawRectangle(248, 31, 4, 91, LIGHTGRAY);
    DrawLine(179, 35, 248, 35, SKYBLUE);
    DrawLine(179, 117, 248, 117, SKYBLUE);
    DrawLine(179, 35, 179, 117, SKYBLUE);
    DrawLine(248, 35, 248, 117, SKYBLUE);

    DrawRectangle(53, 26, 4, 188, LIGHTGRAY);
    DrawRectangle(162, 26, 4, 188, LIGHTGRAY);
    DrawRectangle(53, 26, 109, 4, LIGHTGRAY);
    DrawRectangle(53, 210, 109, 4, LIGHTGRAY);

    DrawLine(53, 0, 53, 214, BLACK);
    DrawLine(166, 0, 166, 214, BLACK);
    DrawLine(53, 214, 166, 214, BLACK);

    DrawLine(58, 30, 58, 210, SKYBLUE);
    DrawLine(162, 30, 162, 210, SKYBLUE);
    DrawLine(57, 30, 162, 30, SKYBLUE);
    DrawLine(57, 210, 162, 210, SKYBLUE);
}

static void dessinPicXel()
{
    DrawRectangle(58, 0, 102, 26, BLACK);
    DrawRectangle(53, 0, 110, 4, LIGHTGRAY);
    DrawRectangle(53, 0, 5, 26, LIGHTGRAY);
    DrawRectangle(160, 0, 5, 26, LIGHTGRAY);

    DrawLine(53, 0, 165, 0, BLACK);
    DrawLine(58, 4, 160, 4, SKYBLUE);
    DrawLine(58, 26, 160, 26, SKYBLUE);
    DrawLine(58, 4, 58, 26, SKYBLUE);
    DrawLine(160, 4, 160, 26, SKYBLUE); 

    DrawText("T", 68, 5, 25, BLUE);
    DrawText("e", 85, 5, 25, RED);
    DrawText("t", 100, 5, 25, ORANGE);
    DrawText("r", 113, 5, 25, GREEN);
    DrawText("i", 129, 5, 25, PINK);
    DrawText("s", 135, 5, 25, YELLOW);
}

static void dessinLines()
{
    DrawRectangle(175, 148, 77, 40, BLACK);

    DrawRectangle(175, 149, 4, 38, LIGHTGRAY);
    DrawRectangle(175, 149, 77, 4, LIGHTGRAY);
    DrawRectangle(248, 149, 4, 38, LIGHTGRAY);
    DrawRectangle(175, 183, 77, 4, LIGHTGRAY);

    DrawLine(179, 153, 248, 153, SKYBLUE);
    DrawLine(179, 183, 248, 183, SKYBLUE);
    DrawLine(179, 153, 179, 183, SKYBLUE);
    DrawLine(248, 153, 248, 183, SKYBLUE);

}