#pragma once
#include "GameState.hpp"
#include "raylib.hpp"
#include "Game.hpp"


class GameOverState : public GameState
{
public:
    GameOverState(Game* game);

    void OnEnter() override;
    void OnExit() override;
    void Update() override;
    void Render() override;

private: 
    Game* _game;
    Texture2D _texture;
};