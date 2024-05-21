#pragma once 
#include <raylib.hpp>

class Paddle
{
public:
    Paddle();
    void Update();
    void Draw() const;

    Rectangle GetRectangle();
    void SetXPosition(float x_);
    void SetYPosition(float y_);

private:
    Vector2 position;
    float speed;
    float width;
    float height;
};