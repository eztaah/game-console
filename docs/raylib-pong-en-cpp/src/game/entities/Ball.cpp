#include "globals.hpp"
#include "Ball.hpp"
#include <string>


Ball::Ball(const Color& color)
    : _isActive(false),
      _size(GetReelValue(14.0f)),
      _position({0.0f, 0.0f}),
      _speedCoeff(1.0f),
      _speed({GetReelValue(600.0f), GetReelValue(-300.0f)}),
      _color(color)
{}


// === Movmement & Logic ===
void Ball::Update()
{
    if(_isActive)
    {
        _position.x += _speedCoeff * _speed.x * GetFrameTime();   // * GetFrameTime() permet de rendre le déplacement de la balle constant
        _position.y += _speedCoeff * _speed.y * GetFrameTime();
    }
}

void Ball::HandleBounceTop()
{
    _position.y = 0.0f;    // Prevent bug when the ball go outside the screen
    _speed.y *= -1;
}

void Ball::HandleBounceBottom()
{
    _position.y = GAME_HEIGHT - _size;
    _speed.y *= -1;
}

void Ball::HandleBounceLeft()
{
    _position.x = 30.0f;
    _speed.x *= -1;     // bounce

    _speedCoeff += 0.05;     // increase speed

    // if(mismatch <= 0)
    //     _speed.y = -10 * abs(mismatch);
    // else 
    //     _speed.y = 10 * abs(mismatch);
}

void Ball::HandleBounceRight()
{ 
    _position.x = GAME_WIDTH - 30.0f - _size;
    _speed.x *= -1;     // bounce

    // if(mismatch <= 0)
    //     _speed.y = -5 * abs(mismatch);
    // else 
    //     _speed.y = 5 * abs(mismatch);
}


// === Accessors ===
Vector2 Ball::GetPosition() const {
    return _position;
}

Vector2 Ball::GetSpeed() const {
    return _speed;
}

float Ball::GetSize() const {
    return _size;
}

Rectangle Ball::GetRectangle() const {
    return {_position.x, _position.y, _size, _size};
}

bool Ball::IsActive() const {
    return _isActive;
}


// === Mutators ===
void Ball::SetPosition(const Vector2& newPosition) {
    _position.x = newPosition.x;
    _position.y = newPosition.y;
}

void Ball::SetSpeed(const Vector2& newSpeed) {
    _speed = newSpeed;
}

void Ball::Reset()
{
    _size = GetReelValue(14.0f);
    _position = {(GAME_WIDTH/2.0f) - (_size/2), (GAME_HEIGHT/2.0f) - (_size/2)};
    _speed = {GetReelValue(600.0f), GetReelValue(-400.0f)};
    _speedCoeff = 1.0f;
}

void Ball::Activate() {
    _isActive = true;
}

void Ball::Desactivate() {
    _isActive = false;
}


// === Rendering ===
void Ball::Render() const {
    DrawRectangle(_position.x, _position.y, _size, _size, _color);
}