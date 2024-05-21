#pragma once
#include <raylib.hpp> 

class Brick
{
public:
    Brick();
    void Draw() const;

    Rectangle GetRectangle();
    void SetColor(Color color_);
    void SetXPosition(float x_);
    void SetYPosition(float y_);

private:
    Vector2 position;
    float height;
    float width;
    Color color;
};
