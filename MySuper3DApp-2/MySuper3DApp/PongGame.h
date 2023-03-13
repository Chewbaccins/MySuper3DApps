#pragma once
#include "Game.h"
#include "RacketComponent.h"
#include "BallComponent.h"
#include "CircleComponent.h"

class PongGame :
    public Game
{
protected:
	void Update() override;
	unsigned s1;
	unsigned s2;
	IDXGISurface* D2DBackBuff;
public:
	unsigned ballHits;
	std::chrono::steady_clock::time_point ContinueTime;
	RacketComponent* racket1;
	RacketComponent* racket2;
	//BallComponent* ball;
	CircleComponent* ball;
	//GameState state;
	PongGame(LPCWSTR name, int screenWidth, int screenHeight);
	static void CreateInstance(LPCWSTR name, int screenWidth, int screenHeight);
	void Draw() override;
	void Initialize() override;
	void Run() override;
	void ConfigureGameObjects();

	void IncScore(bool p);
};

