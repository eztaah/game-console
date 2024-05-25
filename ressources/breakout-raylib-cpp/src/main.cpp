/*

add comments

*/

#include "game.hpp"
#include <raylib.hpp>
#include <iostream>


int main()
{
    InitWindow(700, 800, "Breakout");
    SetWindowState(FLAG_VSYNC_HINT);

    Game game = Game();

    while (!WindowShouldClose())
    { 
        game.Update();
        game.Draw();
    }
    CloseWindow();
    return 0;
}