#pragma once
#include "RectangleComponent.h"
#include "SimpleMath.h"

class RacketComponent :
    public RectangleComponent
{
public:
    DirectX::SimpleMath::Rectangle CollisionBox;
    float Speed;
    float currentSpeed;
    RacketComponent();
    void Update() override;
    void SetY(float y) override;
};

