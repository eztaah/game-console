#pragma once 
#include <raylib.hpp>

class Ball
{
public:
    Ball();
    void Update();
    void Draw() const;

    Rectangle GetRectangle();
    Vector2 GetSpeed();
    void SetXPosition(float x_);
    void SetYPosition(float y_);
    void SetXSpeed(float x_);
    void SetYSpeed(float y_);

private:
    Vector2 position;
    Vector2 speed;
    float width;
    float height;
};
