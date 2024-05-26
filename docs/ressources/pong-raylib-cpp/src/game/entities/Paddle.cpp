#include "Paddle.hpp"
#include "globals.hpp"


Paddle::Paddle(const float positionX)
    : _size({GetReelValue(10.0f), GetReelValue(100.0f)}),
      _position({positionX, (GAME_HEIGHT / 2.0f) - (_size.y / 2.0f)}),
      _speed(GetReelValue(900.0f))
{}


// === Accessors ===
Vector2 Paddle::GetPosition() const {
    return _position;
}

float Paddle::GetSpeed() const {
    return _speed;
}

Rectangle Paddle::GetRectangle() const {
    return {_position.x, _position.y, _size.x, _size.y};
}


// === Movement & Logic ===
void Paddle::Update()
{
    // Update paddle 1 position
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))   // Raylib assumes that your keyboard is QWERTY, here KEY_W means KEY_Z on AZERTY keyboard
        MoveUp();
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
        MoveDown();
}


// === Mutators ===
void Paddle::SetPosition(const Vector2& newPosition) {
    _position.x = newPosition.x;
    _position.y = newPosition.y;
}

void Paddle::MoveUp() {
    // Vérifie si le paddle atteint le bord supérieur de l'écran
    if (_position.y - _speed * GetFrameTime() > 10.0f) {
        _position.y -= _speed * GetFrameTime();
    } else {
        _position.y = 10.0f;
    }
}

void Paddle::MoveDown() {
    // Vérifie si le paddle atteint le bord inférieur de l'écran
    if (_position.y + _size.y + _speed * GetFrameTime() < WINDOW_HEIGHT - 10.0f) {
        _position.y += _speed * GetFrameTime();
    } else {
        _position.y = WINDOW_HEIGHT - _size.y - 10.0f;
    }
}

void Paddle::SetSpeed(const float newSpeed) {
    _speed = newSpeed;
}

void Paddle::Reset(const float positionX)
{
    _size = {GetReelValue(10.0f), GetReelValue(100.0f)};
    _position = {(positionX), (GAME_HEIGHT / 2.0f) - (_size.y / 2.0f)};
    _speed = GetReelValue(900.0f);
}

// === Rendering ===
void Paddle::Render() const {
    DrawRectangle(_position.x, _position.y, _size.x, _size.y, BLACK);
}