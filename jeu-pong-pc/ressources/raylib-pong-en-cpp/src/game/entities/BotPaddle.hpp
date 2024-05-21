#pragma once
#include "Paddle.hpp" 


class BotPaddle : public Paddle
{
public:
    BotPaddle(const float positionX);
    void Update(const float botDefensePosition);
};