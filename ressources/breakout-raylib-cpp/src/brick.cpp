#include "brick.hpp"
#include <raylib.hpp>
#include <iostream>


Brick::Brick()
{
	width = 38.0f;
	height = 11.0f;
}


void Brick::Draw() const
{
	DrawRectangle(position.x, position.y, width, height, color);
}


Rectangle Brick::GetRectangle() 
{
	return {position.x, position.y, width, height};
}


void Brick::SetColor(Color color_) 
{
	color = color_;
}


void Brick::SetXPosition(float x_) 
{
	position.x = x_;
}


void Brick::SetYPosition(float y_) 
{
	position.y = y_;
}