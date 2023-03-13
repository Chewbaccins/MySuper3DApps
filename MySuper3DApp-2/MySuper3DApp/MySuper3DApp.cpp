#include "PongGame.h"
#include "RectangleComponent.h"
#include "RacketComponent.h"

int main() {
	PongGame::CreateInstance(L"My Game", 800, 800);
	PongGame::instance->Run();
}
