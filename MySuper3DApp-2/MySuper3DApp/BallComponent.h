#pragma once
#include "RectangleComponent.h"
#include "BallComponent.h"

class PongGame;

class BallComponent :
    public RectangleComponent
{
private:
    PongGame* pGame;
    void GenRndDirection();
public:
    float Speed;
    DirectX::SimpleMath::Vector2 Direction;
    BallComponent();
    void Update() override;
};

