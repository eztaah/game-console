#include "Game.hpp"  
#include "WindowManager.hpp"
#include "raylib.hpp"


int main()
{
    WindowManager window = WindowManager(1366, 768, "pong");

    Game game = Game(window);
    
    while (!WindowShouldClose())
    {
        if (IsWindowResized()) 
        {
            window.ManageWindowResizing(GetScreenWidth(), GetScreenHeight());   // Gère la redimention de la fenetre de jeu
            game.SetMenuState();
        }
        game.Update();
        game.Render();
    }

    CloseWindow();
    return 0;
}