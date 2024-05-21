#pragma once
#include "ball.hpp"
#include "paddle.hpp"
#include "brick.hpp"
#include <string>


const unsigned int numberOfColumns {15};
const unsigned int numberOfLines {6};

const unsigned int xGapBetweenBricks {8};
const unsigned int yGapBetweenBricks {5};


class Game
{
public:
    Game();
    void Update();
    void Draw();

private:
    void InitBricks();
    void ManageCollisionBallWall();
    void ManageCollisionBallPaddle();
    void ManageCollisionBallBrick();
    int unsigned GetBestScore();
    void SetBestScore(int score_);
    void GameOver();
    void Restart();

    Ball ball;
    Paddle paddle;
    Brick bricks[numberOfLines * numberOfColumns];

    bool running;
    int unsigned score;
    int unsigned bestScore;
};
