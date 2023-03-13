#include "RectangleComponent.h"

RectangleComponent::RectangleComponent() {
	// Creating set of points
	/*points.insert(points.end(),
		{
			DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
			DirectX::XMFLOAT4(-0.5f, -0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
			DirectX::XMFLOAT4(0.5f, -0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
			DirectX::XMFLOAT4(-0.5f, 0.5f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		});*/
}

void RectangleComponent::Initialize() {
	D3D_SHADER_MACRO Shader_Macros[] = { "TEST", "1", "TCOLOR", "float4(0.0f, 1.0f, 0.0f, 1.0f)", nullptr, nullptr };
	ID3DBlob* errorVertexCode = nullptr;
	ID3DBlob* errorPixelCode = nullptr;

	// Compile vertex shader
	auto res = D3DCompileFromFile(L"./Shaders/MyVeryFirstShader.hlsl",
		nullptr /*macros*/,
		nullptr /*include*/,
		"VSMain",
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vertexBC,
		&errorVertexCode);

	if (FAILED(res)) {
		// If the shader failed to compile it should have written something to the error message.
		if (errorVertexCode) {
			char* compileErrors = (char*)(errorVertexCode->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(Game::instance->GetDisplay()->GetHWnd(), L"MyVeryFirstShader.hlsl", L"Missing Shader File", MB_OK);
		}

		return;
	}

	// Compile pixel shader
	res = D3DCompileFromFile(L"./Shaders/MyVeryFirstShader.hlsl",
		Shader_Macros /*macros*/,
		nullptr /*include*/,
		"PSMain",
		"ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&pixelBC,
		&errorPixelCode);

	if (FAILED(res)) {
		// If the shader failed to compile it should have written something to the error message.
		if (errorPixelCode) {
			char* compileErrors = (char*)(errorPixelCode->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(Game::instance->GetDisplay()->GetHWnd(), L"MyVeryFirstShader.hlsl", L"Missing Shader File", MB_OK);
		}

		return;
	}

	Game::instance->GetDevice()->CreateVertexShader(
		vertexBC->GetBufferPointer(),
		vertexBC->GetBufferSize(),
		nullptr, &vertexShader
	);

	Game::instance->GetDevice()->CreatePixelShader(
		pixelBC->GetBufferPointer(),
		pixelBC->GetBufferSize(),
		nullptr, &pixelShader
	);

	D3D11_INPUT_ELEMENT_DESC inputElements[] = {
		D3D11_INPUT_ELEMENT_DESC {
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0},
		D3D11_INPUT_ELEMENT_DESC {
			"COLOR",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA, // Per vertex or per instance
			0}
	};

	Game::instance->GetDevice()->CreateInputLayout(
		inputElements,
		2,
		vertexBC->GetBufferPointer(),
		vertexBC->GetBufferSize(),
		&layout);

	vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = 0;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.ByteWidth = sizeof(DirectX::XMFLOAT4) * points.size();

	vertexData = {};
	vertexData.pSysMem = points.data();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	Game::instance->GetDevice()->CreateBuffer(&vertexBufDesc, &vertexData, &vb);

	indexBufDesc = {};
	indexBufDesc.ByteWidth = sizeof(int) * indeces.size();
	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.CPUAccessFlags = 0;
	indexBufDesc.MiscFlags = 0;
	indexBufDesc.StructureByteStride = 0;

	indexData = {};
	indexData.pSysMem = indeces.data();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	Game::instance->GetDevice()->CreateBuffer(&indexBufDesc, &indexData, &ib);

	strides[0] = { 32 };
	offsets[0] = { 0 };

	D3D11_BUFFER_DESC constBufDesc = {};
	constBufDesc.Usage = D3D11_USAGE_DEFAULT;
	constBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufDesc.CPUAccessFlags = 0;
	constBufDesc.MiscFlags = 0;
	constBufDesc.StructureByteStride = 0;
	constBufDesc.ByteWidth = sizeof(DirectX::SimpleMath::Vector4);

	Game::instance->GetDevice()->CreateBuffer(&constBufDesc, nullptr, &cb);

	rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE; // Try to change
	rastDesc.FillMode = fillMode; // Try to change

	res = Game::instance->GetDevice()->CreateRasterizerState(&rastDesc, &rastState);

	Game::instance->GetContext()->RSSetState(rastState);
}

void RectangleComponent::Update() {
	Game::instance->GetContext()->UpdateSubresource(cb, 0, 0, &offset, 0, 0);
}

void RectangleComponent::Draw() {
	Game::instance->GetContext()->RSSetState(rastState);

	Game::instance->GetContext()->IASetInputLayout(layout);
	Game::instance->GetContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Game::instance->GetContext()->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);
	Game::instance->GetContext()->IASetVertexBuffers(0, 1, &vb, strides, offsets);
	Game::instance->GetContext()->VSSetShader(vertexShader, nullptr, 0);
	Game::instance->GetContext()->VSSetConstantBuffers(0, 1, &cb);
	Game::instance->GetContext()->PSSetShader(pixelShader, nullptr, 0);

	Game::instance->GetContext()->DrawIndexed(indeces.size(), 0, 0); // Main function for draw (DrawCall)
}

void RectangleComponent::Reload() {

}

void RectangleComponent::DestroyResources() {

}

void RectangleComponent::SetPosition(float x, float y)
{
	offset.x = x;
	offset.y = y;
}

void RectangleComponent::SetPosition(DirectX::SimpleMath::Vector2 pos)
{
	offset.x = pos.x;
	offset.y = pos.y;
}

void RectangleComponent::SetY(float y)
{
	offset.y = y;
}

void RectangleComponent::SetX(float x)
{
	offset.x = x;
}

float RectangleComponent::GetX() const
{
	return offset.x;
}

float RectangleComponent::GetY() const
{
	return offset.y;
}

DirectX::SimpleMath::Vector2 RectangleComponent::GetPosition() const
{
	return DirectX::SimpleMath::Vector2(offset.x, offset.y);
}
