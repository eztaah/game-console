#include "GameOverState.hpp"
#include "globals.hpp"


GameOverState::GameOverState(Game* game)
    : _game(game),
     _texture()
{}


void GameOverState::OnEnter()
{
    _texture = LoadTexture("./assets/textures/game-over.png");
}

void GameOverState::OnExit()
{

}

void GameOverState::Update()
{
    if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER))
        _game->SetMenuState();
}

void GameOverState::Render()
{
    // Draw image
    float scale = static_cast<float>(GAME_WIDTH) / static_cast<float>(_texture.width);
    DrawTextureEx(_texture, {0.0f, 0.0f}, 0.0f, scale, WHITE); 

    // Draw score
    std::string scoreStr = std::to_string(_game->GetScore());   // Convert score to a string
    DrawText(scoreStr.c_str(), (GAME_WIDTH / 2) + GetReelValue(80.0f), GetReelValue(343.0f), GetReelValue(40.0f), BLACK);     // Display the score
}