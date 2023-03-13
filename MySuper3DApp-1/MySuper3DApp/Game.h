#pragma once
#include <windows.h>
#include <WinUser.h>
#include <wrl.h>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include "GameComponent.h"
#include "DisplayWin32.h"
#include <iostream>
#include <chrono>
#include <vector>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

class DisplayWin32;
class GameComponent;

class Game {
private:
	LPCWSTR	name; // Name of the game application
	std::shared_ptr<DisplayWin32> display; // WinApi display
	Microsoft::WRL::ComPtr<ID3D11Device> device; // The device interface represents a virtual adapter and it is used to create resources
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context; // Interface represents a device context which generates rendering commands
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain; // Interface implements one or more "IDXGISurface" for storing rendered data before presenting it to an output
	DXGI_SWAP_CHAIN_DESC swapDesc; // Descriptor, that describes swap chain
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backTex; // Pointer to the back buffer interface
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv; // Identifies the render-target subresources that can be accessed during rendering (Back buffer?)
	D3D11_VIEWPORT viewport;

	bool isExitRequested; // For main application cycle

	Game(LPCWSTR name);
	void CreateBackBuffer();

protected:
	void UpdateInternal();
	void DestroyResources();
	void PrepareResources();
	void Initialize();
	void PrepareFrame();
	void Update();
	void Draw();
	void EndFrame();

public:
	static Game* instance; // Singleton
	std::vector<GameComponent*> components;
	std::chrono::time_point<std::chrono::steady_clock>* startTime = nullptr;
	std::chrono::time_point<std::chrono::steady_clock>* prevTime = nullptr;
	float totalTime = 0;
	unsigned int frameCount;
	int mouseX = 0;
	int mouseY = 0;

	static Game* CreateInstance(LPCWSTR name);
	void RestoreTargets();
	void Run(int screenWidth, int screenHeight);
	void Exit();

	LRESULT MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);


	std::shared_ptr<DisplayWin32> GetDisplay();

	Microsoft::WRL::ComPtr<ID3D11Device> GetDevice();

	Microsoft::WRL::ComPtr <ID3D11DeviceContext> GetContext();

	Microsoft::WRL::ComPtr <IDXGISwapChain> GetSwapChain();


	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> GetRTV();

	float GetTotalTime();
	void SetTotalTime(float totalTime);

	unsigned int GetFrameCount();
	void SetFrameCount(unsigned int frameCount);
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
