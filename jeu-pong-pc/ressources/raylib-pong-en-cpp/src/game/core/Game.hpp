#pragma once
#include "raylib.hpp"
#include <vector>
#include <memory>
#include "GameState.hpp"
#include "Audiomanager.hpp"
#include "WindowManager.hpp"


class Game
{
public:
    Game(WindowManager& window);
    ~Game();
    void Update();
    void Render();

    // === window ===
    void ToggleFullScreenWindow();

    // === Games states ===
    void SetMenuState();
    void SetPlayingState();
    void SetGameOverState();

    // === Score ===
    unsigned int GetScore() const;
    void IncreaseScore();
    void ResetScore();

    // === Music ===
    void ToggleMusic();

private:
    void _ChangeState(std::unique_ptr<GameState> newState);

    std::unique_ptr<GameState> _currentState;
    int unsigned _score;
    AudioManager _audiomanager;
    WindowManager _window;
};