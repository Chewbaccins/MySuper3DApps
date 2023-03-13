#include "CircleComponent.h"
#include "PongGame.h"

using namespace DirectX;

void CircleComponent::GenRndDirection()
{
	SimpleMath::Vector2::Lerp(SimpleMath::Vector2(1.0f, 1.0f), SimpleMath::Vector2(1.0f, -1.0f), (float)rand() / RAND_MAX, Direction);
	//Direction.x = 1;
	if ((float)rand() / RAND_MAX < 0.5f)
		Direction.x *= -1;
	Direction.Normalize();
}

CircleComponent::CircleComponent() : RectangleComponent()
{
	//fillMode = D3D11_FILL_WIREFRAME;
	fillMode = D3D11_FILL_SOLID;
	pointsAmount = 4;
	radius = 0.1f;
	pGame = dynamic_cast<PongGame*>(PongGame::instance);
	points.push_back(DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	points.push_back(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	for (int i = 1; i <= pointsAmount; i++) {
		points.push_back(
			DirectX::XMFLOAT4(
				radius * DirectX::XMScalarSin(DirectX::XM_PI / 4 + DirectX::XM_PI * 2 * (static_cast<float>(i) / pointsAmount)),
				radius * DirectX::XMScalarCos(DirectX::XM_PI / 4 + DirectX::XM_PI * 2 * (static_cast<float>(i) / pointsAmount)),
				0.0f,
				1.0f
			)
		);
		points.push_back(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	}
	for (unsigned int i = 1; i < pointsAmount; i++) {
		indeces.insert(indeces.end(), { 0, i, i + 1 });
	}
	indeces.insert(indeces.end(), { 0, pointsAmount, 1 });

	CollisionBox = SimpleMath::Rectangle(- radius / sqrt(2) * 800, - radius / sqrt(2) * 800, radius * sqrt(2) * 800, radius * sqrt(2) * 800);
	Speed = 1.2f;
	GenRndDirection();
}

void CircleComponent::Update()
{
	CollisionBox.x = -radius / sqrt(2) * 800 + offset.x * 800;
	CollisionBox.y = -radius / sqrt(2) * 800 + offset.y * 800;
	auto nextPos = GetPosition() + Direction * Speed * pGame->deltaTime;

	if (nextPos.y > 1.0f)
		Direction.y = -abs(Direction.y);
	else if (nextPos.y < -1.0f)
		Direction.y = abs(Direction.y);
	else if (nextPos.x < -1.0f || nextPos.x > 1.0f)
	{
		pGame->ContinueTime = std::chrono::steady_clock::now() + std::chrono::seconds(1);
		pGame->IncScore(nextPos.x > 0.0f);
		pGame->ballHits = 0;
		nextPos.x = 0.0f;
		nextPos.y = 0.0f;
		GenRndDirection();
	}
	else if ((Direction.x < 0) && CollisionBox.Intersects(pGame->racket1->CollisionBox))
	{
		SimpleMath::Vector2::Lerp(SimpleMath::Vector2(1.f, -0.3f), SimpleMath::Vector2(1.f, 0.3f), -pGame->racket1->currentSpeed + 0.5f, Direction);
		Direction.Normalize();
		nextPos += Direction * Speed * pGame->deltaTime;
		pGame->ballHits += 1;
		//pGame->components2.push_back(new CircleComponent());
	}
	else if ((Direction.x > 0) && CollisionBox.Intersects(pGame->racket2->CollisionBox))
	{
		SimpleMath::Vector2::Lerp(SimpleMath::Vector2(-1.f, -0.1f), SimpleMath::Vector2(-1.f, 0.1f), - pGame->racket2->currentSpeed + 0.5f, Direction);
		Direction.Normalize();
		nextPos += Direction * Speed * pGame->deltaTime;
		pGame->ballHits += 1;
		pGame->components2.push_back(new CircleComponent());

	}
	SetPosition(nextPos);
	RectangleComponent::Update();
}
