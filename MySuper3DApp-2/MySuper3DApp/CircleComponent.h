#pragma once
#include "RectangleComponent.h"
#include "CircleComponent.h"

class PongGame;

class CircleComponent :
    public RectangleComponent
{
private:
    PongGame* pGame;
    float radius;
    unsigned int pointsAmount;
    void GenRndDirection();
public:
    float Speed;
    DirectX::SimpleMath::Vector2 Direction;
    CircleComponent();
    void Update() override;
    DirectX::SimpleMath::Rectangle CollisionBox;
};

