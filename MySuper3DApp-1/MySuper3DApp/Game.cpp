#include "Game.h"

Game* Game::instance = nullptr;

/*
* Static method for handling user input
*/
LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam) {
	return Game::instance->MessageHandler(hwnd, umessage, wparam, lparam);
}

LRESULT Game::MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam) {
	switch (umessage) 
	{
	case WM_KEYDOWN:
	{
		// If a key is pressed send it to the input object so it can record that state.
		std::cout << "Key: " << static_cast<unsigned int>(wparam) << std::endl;

		if (static_cast<unsigned int>(wparam) == 27) PostQuitMessage(0);
		return 0;
	}
	case WM_MOUSEMOVE:
	{
		const POINTS pt = MAKEPOINTS(lparam);
		mouseX = pt.x;
		mouseY = pt.y;
	}
	default:
		{
			return DefWindowProc(hwnd, umessage, wparam, lparam);
		}
	}
}

/* Singleton method*/
Game::Game(LPCWSTR name) {
	this->name = name;
}

Game* Game::CreateInstance(LPCWSTR name) {
	if (!instance)
		instance = new Game(name);

	return instance;
}

void Game::CreateBackBuffer() {

}

/*
* Method for preparing all "Game" resources
*/
void Game::PrepareResources() {
	
	const int featureLevelsNumber = 1;
	D3D_FEATURE_LEVEL featureLevels[featureLevelsNumber] = { D3D_FEATURE_LEVEL_11_1 };

	swapDesc = {}; // Init swap chain descriptor
	// BufferDesc describes the backbuffer display mode
	swapDesc.BufferCount = 2; // Number of buffers in the swap chain (double or triple buffering)
	swapDesc.BufferDesc.Width = display->GetScreenWidth(); // Resolution width
	swapDesc.BufferDesc.Height = display->GetScreenHeight(); // Resolution height
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // Display format (32-bit unsigned normalized integer format supporting 8 bits per channel, including the alpha channel)
	swapDesc.BufferDesc.RefreshRate.Numerator = 60; // Refresh rate in hertz numerator
	swapDesc.BufferDesc.RefreshRate.Denominator = 1; // Refresh rate in hertz denominator (for representing integer it = 1)
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; // Scanline drawing mode (indicating the method the raster uses to create an image on a surface)
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; // Scaling mode (indicating how an image is stretched to fit a given monitor's resolution)
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // Describes the surface usage and CPU access options for the back buffer
	swapDesc.OutputWindow = display->GetHWnd(); // Handle to the output window. This member must not be NULL.
	swapDesc.Windowed = true; // Fullscreen or not
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // Describes options for handling the contents of the presentation buffer after presenting a surface
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // Options for swap-chain behavior
	// Describes multi-sampling parameters; Works only with DXGI_SWAP_EFFECT_DISCARD 
	swapDesc.SampleDesc.Count = 1; // Number of multisamples per pixel
	swapDesc.SampleDesc.Quality = 0; // The image quality level. The higher the quality, the lower the performance

	// Creates a device that represents the display adapter and a swap chain used for rendering
	auto res = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		featureLevels, // Determine the order of feature levels to attempt to create
		featureLevelsNumber, // The number of elements in feature levels array
		D3D11_SDK_VERSION,
		&swapDesc,
		&swapChain,
		&device,
		nullptr, // Feature level for device
		&context);

	if (FAILED(res)) {
		// Well, that was unexpected
	}

	res = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backTex.GetAddressOf()); // Accesses one of the buffers of the back buffer chain
	res = device->CreateRenderTargetView(backTex.Get(), nullptr, rtv.GetAddressOf());

	viewport = {};
	viewport.Width = static_cast<float>(display->GetScreenWidth());
	viewport.Height = static_cast<float>(display->GetScreenHeight());
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;
}

/*
* Method for initializing all "GameComponent" items in vector
*/
void Game::Initialize() {
	// Compile shaders
	for (auto component : components)
		component->Initialize();
}

void Game::PrepareFrame() {
	context->ClearState(); // Reset parameters to default

	context->OMSetRenderTargets(1, rtv.GetAddressOf(), nullptr);

	context->RSSetViewports(1, &viewport);
	//context->RSSetState(rastState); // In TriangleComponent Update Method

	float color[] = { Game::instance->GetTotalTime(), 0.1f, 0.1f, 1.0f };
	context->ClearRenderTargetView(rtv.Get(), color);
}

/*
* Method for updating all "GameComponent" items in vector
*/
void Game::Update() {
	for (auto component : components)
		component->Update();
}

/*
* Method for drawing all "GameComponent" items in vector
*/
void Game::Draw() {
	for (auto component : components)
		component->Draw();
}

void Game::RestoreTargets() {

}

/*
* Method for actually presenting graphics
*/
void Game::EndFrame() {
	context->OMSetRenderTargets(0, nullptr, nullptr);

	swapChain->Present(1, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0); // Show what we've drawn
}

/*
* Internal method, that updates all items and called each frame
*/
void Game::UpdateInternal() {
	auto curTime = std::chrono::steady_clock::now();
	float deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(curTime - *prevTime).count() / 1000000.0f;
	*prevTime = curTime;

	totalTime += deltaTime;
	frameCount++;

	if (totalTime > 1.0f) {
		float fps = frameCount / totalTime;

		totalTime -= 1.0f;

		WCHAR text[256];
		swprintf_s(text, TEXT("FPS: %f"), fps);
		//SetWindowText(display->GetHWnd(), text);

		frameCount = 0;
	}

	WCHAR text[256];
	swprintf_s(text, TEXT("Pos (%d;%d)"), mouseX, mouseY);
	SetWindowText(display->GetHWnd(), text);

	PrepareFrame();

	Update();

	Draw();

	RestoreTargets();

	EndFrame();
}

void Game::Run(int screenWidth, int screenHeight) {
	display = std::make_shared<DisplayWin32>(name, screenWidth, screenHeight, WndProc);

	PrepareResources();

	Initialize();

	isExitRequested = false;

	startTime = new std::chrono::time_point<std::chrono::steady_clock>();
	prevTime = new std::chrono::time_point<std::chrono::steady_clock>();
	*startTime = std::chrono::steady_clock::now();
	*prevTime = *startTime;
	totalTime = 0;
	frameCount = 0;

	MSG msg = {};
	while (!isExitRequested) {
		// Handle the windows messages.
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if (msg.message == WM_QUIT) {
			isExitRequested = true;
		}

		UpdateInternal();
	}

	delete startTime;
	delete prevTime;
	DestroyResources();
}

void Game::Exit() {

}

void Game::DestroyResources() {
	for (auto component : components)
		component->DestroyResources();
}


std::shared_ptr<DisplayWin32> Game::GetDisplay() {
	return display;
}

Microsoft::WRL::ComPtr<ID3D11Device> Game::GetDevice() {
	return device;
}

Microsoft::WRL::ComPtr<ID3D11RenderTargetView> Game::GetRTV() {
	return rtv;
}

Microsoft::WRL::ComPtr <IDXGISwapChain> Game::GetSwapChain() {
	return swapChain;
}

Microsoft::WRL::ComPtr <ID3D11DeviceContext> Game::GetContext() {
	return context;
}

float Game::GetTotalTime() {
	return totalTime;
}

void Game::SetTotalTime(float totalTime) {
	this->totalTime = totalTime;
}

unsigned int Game::GetFrameCount() {
	return frameCount;
}

void Game::SetFrameCount(unsigned int frameCount) {
	this->frameCount = frameCount;
}
