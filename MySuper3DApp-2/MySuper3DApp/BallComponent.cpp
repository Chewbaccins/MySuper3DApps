#include "BallComponent.h"
#include "PongGame.h"

using namespace DirectX;

// line/line collision
static bool LineLineColl(const SimpleMath::Vector2& p1, const SimpleMath::Vector2& p2, const SimpleMath::Vector2& p3, const SimpleMath::Vector2& p4, SimpleMath::Vector2& intersection) {

	// calculate the direction of the lines
	float uA = ((p4.x - p3.x) * (p1.y - p3.y) - (p4.y - p3.y) * (p1.x - p3.x)) / ((p4.y - p3.y) * (p2.x - p1.x) - (p4.x - p3.x) * (p2.y - p1.y));
	float uB = ((p2.x - p1.x) * (p1.y - p3.y) - (p2.y - p1.y) * (p1.x - p3.x)) / ((p4.y - p3.y) * (p2.x - p1.x) - (p4.x - p3.x) * (p2.y - p1.y));

	// if uA and uB are between 0-1, lines are colliding
	if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1)
	{
		intersection.x = (p1.x + (uA * (p2.x - p1.x))) / 800.0f;
		intersection.y = (p1.y + (uA * (p2.y - p1.y))) / 800.0f;
		return true;
	}
	return false;
}

// line/rectangle collision detection
static bool LineRectColl(const SimpleMath::Vector2& p1, const SimpleMath::Vector2& p2, const SimpleMath::Rectangle& rect, bool leftPrimary, SimpleMath::Vector2& intersection)
{
	bool top = LineLineColl(p1, p2, SimpleMath::Vector2(rect.x, rect.y), SimpleMath::Vector2(rect.x + rect.width, rect.y), intersection);
	bool bottom = LineLineColl(p1, p2, SimpleMath::Vector2(rect.x, rect.y + rect.height), SimpleMath::Vector2(rect.x + rect.width, rect.y + rect.height), intersection);
	bool left, right;
	if (leftPrimary)
	{
		right = LineLineColl(p1, p2, SimpleMath::Vector2(rect.x + rect.width, rect.y), SimpleMath::Vector2(rect.x + rect.width, rect.y + rect.height), intersection);
		left = LineLineColl(p1, p2, SimpleMath::Vector2(rect.x, rect.y), SimpleMath::Vector2(rect.x, rect.y + rect.height), intersection);
	}
	else
	{
		left = LineLineColl(p1, p2, SimpleMath::Vector2(rect.x, rect.y), SimpleMath::Vector2(rect.x, rect.y + rect.height), intersection);
		right = LineLineColl(p1, p2, SimpleMath::Vector2(rect.x + rect.width, rect.y), SimpleMath::Vector2(rect.x + rect.width, rect.y + rect.height), intersection);
	}

	if (left || right || top || bottom)
	{
		return true;
	}
	return false;
}

void BallComponent::GenRndDirection()
{
	SimpleMath::Vector2::Lerp(SimpleMath::Vector2(1.0f, 1.0f), SimpleMath::Vector2(1.0f, -1.0f), (float)rand() / RAND_MAX, Direction);
	if ((float)rand() / RAND_MAX < 0.5f)
		Direction.x *= -1;
	Direction.Normalize();
}

BallComponent::BallComponent() : RectangleComponent()
{
	fillMode = D3D11_FILL_WIREFRAME;
	pGame = dynamic_cast<PongGame*>(PongGame::instance);
	points.insert(points.end(), {
		SimpleMath::Vector4(0.03f, 0.03f, 0.5f, 1.0f),	SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		SimpleMath::Vector4(-0.03f, -0.03f, 0.5f, 1.0f),	SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		SimpleMath::Vector4(0.03f, -0.03f, 0.5f, 1.0f),	SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f),
		SimpleMath::Vector4(-0.03f, 0.03f, 0.5f, 1.0f),	SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f),
	});
	Speed = 1.2f;
	GenRndDirection();
}

void BallComponent::Update()
{
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
	else if (LineRectColl(GetPosition() * 800, nextPos * 800, pGame->racket1->CollisionBox, false, nextPos))
	{
		SimpleMath::Vector2::Lerp(SimpleMath::Vector2(1.f, -1.f), SimpleMath::Vector2(1.f, 1.f), abs(GetY() - pGame->racket1->GetY() + 0.1f) / 0.2f, Direction);
		Direction.Normalize();
		nextPos += Direction * Speed * pGame->deltaTime;
		pGame->ballHits += 1;
	}
	else if (LineRectColl(GetPosition() * 800, nextPos * 800, pGame->racket2->CollisionBox, true, nextPos))
	{
		SimpleMath::Vector2::Lerp(SimpleMath::Vector2(-1.f, -1.f), SimpleMath::Vector2(-1.f, 1.f), abs(GetY() - pGame->racket2->GetY() + 0.1f) / 0.2f, Direction);
		Direction.Normalize();
		nextPos += Direction * Speed * pGame->deltaTime;
		pGame->ballHits += 1;
	}
	SetPosition(nextPos);
	RectangleComponent::Update();
}
