#pragma once
#include "GameState.hpp"
#include "Game.hpp"
#include "raylib.hpp"
#include <memory>
#include <vector>
#include <raylib.hpp>


class MenuState : public GameState
{
public:
    MenuState(Game* game);

    void OnEnter() override;
    void OnExit() override;
    void Update() override;
    void Render() override;

private: 
    void _ManageKeyPressedMainMenu();
    void _ManageKeyPressedSettings();

    Game* _game;
    int _cursorPosition;
    int _state;

    std::vector<Texture2D> _mainMenuTextures;
    std::vector<Texture2D> _settingsTextures;
};