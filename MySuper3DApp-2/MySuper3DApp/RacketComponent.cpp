#include "RacketComponent.h"
#include "Game.h"

using namespace DirectX;

RacketComponent::RacketComponent() : RectangleComponent()
{
	fillMode = D3D11_FILL_SOLID;
	currentSpeed = 0;
	points.insert(points.end(), {
		SimpleMath::Vector4(0.02f, 0.15f, 0.5f, 1.0f),	SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		SimpleMath::Vector4(-0.02f, -0.15f, 0.5f, 1.0f),	SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		SimpleMath::Vector4(0.02f, -0.15f, 0.5f, 1.0f),	SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		SimpleMath::Vector4(-0.02f, 0.15f, 0.5f, 1.0f),	SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	});
	indeces.insert(indeces.end(), { 0, 1, 2, 1, 0, 3 });
	Speed = 1.4f;
	CollisionBox = SimpleMath::Rectangle(-0.02f * 800, -0.15f * 800, 0.04f * 800, 0.3f * 800);
}

void RacketComponent::Update()
{
	CollisionBox.x = -0.02f * 800 + offset.x * 800;
	CollisionBox.y = -0.15f * 800 + offset.y * 800;
	RectangleComponent::Update();
}

void RacketComponent::SetY(float y)
{
	currentSpeed = (currentSpeed + (GetY() - y) / Game::instance->deltaTime)/2;
	RectangleComponent::SetY(y);
}
