#pragma once
#include "GameComponent.h"
#include "SimpleMath.h"

class GameComponent;

class RectangleComponent : public GameComponent {
protected:
	ID3D11InputLayout* layout = nullptr;
	ID3D11VertexShader* vertexShader = nullptr;
	ID3DBlob* vertexBC = nullptr;
	ID3D11RasterizerState* rastState = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;
	ID3DBlob* pixelBC = nullptr;

	std::vector<unsigned int> indeces;
	std::vector<DirectX::XMFLOAT4> points;
	D3D11_BUFFER_DESC vertexBufDesc; // Vertex buffer descriptor
	D3D11_SUBRESOURCE_DATA vertexData;
	D3D11_BUFFER_DESC indexBufDesc;
	D3D11_SUBRESOURCE_DATA indexData;
	CD3D11_RASTERIZER_DESC rastDesc;
	D3D11_FILL_MODE fillMode;
	ID3D11Buffer* vb = nullptr;
	ID3D11Buffer* ib = nullptr;
	ID3D11Buffer* cb = nullptr;
	UINT strides[1];
	UINT offsets[1];

	DirectX::SimpleMath::Vector4 offset;

public:
	RectangleComponent();

	void Initialize();
	void Update();
	void Draw();
	void Reload();
	void DestroyResources();

	void SetPosition(float x, float y);
	void SetPosition(DirectX::SimpleMath::Vector2 pos);
	virtual void SetY(float y);
	void SetX(float x);
	float GetX() const;
	float GetY() const;
	DirectX::SimpleMath::Vector2 GetPosition() const;
};
