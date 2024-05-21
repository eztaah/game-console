#include "BotPaddle.hpp"


BotPaddle::BotPaddle(const float positionX)
    : Paddle(positionX)
{}


void BotPaddle::Update(const float botDefensePosition)
{
    // Update paddle 2 position
    if(_position.y < botDefensePosition)
        MoveDown();
    if(_position.y > botDefensePosition)
        MoveUp();
}