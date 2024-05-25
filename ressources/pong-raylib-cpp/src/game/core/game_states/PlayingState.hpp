#pragma once
#include "GameState.hpp"
#include "raylib.hpp"
#include "Ball.hpp"
#include "Paddle.hpp"
#include "BotPaddle.hpp"
#include <vector>
#include "Game.hpp"


class PlayingState : public GameState
{
public:
    PlayingState(Game* game);

    void OnEnter() override;
    void OnExit() override;
    void Update() override;
    void Render() override;

private:
    void _HandleCollisions();

    Game* _game;
    Ball _ball;
    Ball _ghostBall;
    Paddle _paddle1;
    BotPaddle _paddle2;

    float _botDefensePosition;
    std::vector<Ball*> _ballsArray;

};