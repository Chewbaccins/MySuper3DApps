#pragma once
#include <windows.h>

class Game;

class DisplayWin32 {
protected:
	LPCWSTR applicationName;
	RECT windowRect;
	HINSTANCE hInstance;
	HWND hWnd;
	WNDCLASSEX wc;
	int screenWidth;
	int screenHeight;

public:
	DisplayWin32(LPCWSTR& applicationName, int screenWidth, int screenHeight, WNDPROC wndProc);
	int GetScreenWidth();
	int GetScreenHeight();
	HWND& GetHWnd();
};
