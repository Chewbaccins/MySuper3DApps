#pragma once
#include "GameComponent.h"

class GameComponent;

class TriangleComponent : public GameComponent {
protected:
	ID3D11InputLayout* layout = nullptr;
	ID3D11VertexShader* vertexShader = nullptr;
	ID3DBlob* vertexBC = nullptr;
	DirectX::XMFLOAT4* points = nullptr; // Pointer on array of points
	ID3D11RasterizerState* rastState = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;
	ID3DBlob* pixelBC = nullptr;

	
	D3D11_BUFFER_DESC vertexBufDesc; // Vertex buffer descriptor
	D3D11_SUBRESOURCE_DATA vertexData;
	ID3D11Buffer* vb = nullptr;
	D3D11_BUFFER_DESC indexBufDesc;
	D3D11_SUBRESOURCE_DATA indexData;
	CD3D11_RASTERIZER_DESC rastDesc;
	ID3D11Buffer* ib = nullptr;
	UINT strides[1];
	UINT offsets[1];

public:
	TriangleComponent();

	void Initialize();
	void Update();
	void Draw();
	void Reload();
	void DestroyResources();
};
