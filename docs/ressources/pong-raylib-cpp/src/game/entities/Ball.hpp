#pragma once
#include <raylib.hpp>


class Ball
{
public:
    Ball(const Color& color);

    // === Movmement & Logic ===
    virtual void Update();
    void HandleBounceTop();
    void HandleBounceBottom();
    void HandleBounceLeft();
    void HandleBounceRight();

    // === Accessors ===
    Vector2 GetPosition() const;
    Vector2 GetSpeed() const;
    float GetSize() const;
    Rectangle GetRectangle() const;
    bool IsActive() const;

    // === Mutators ===
    void SetPosition(const Vector2& newPosition);
    void SetSpeed(const Vector2& newSpeed);
    void Reset();
    void Activate();
    void Desactivate();

    // === Rendering ===
    void Render() const;

private:
    bool _isActive;
    float _size;
    Vector2 _position;
    float _speedCoeff;
    Vector2 _speed;
    Color _color;
};
