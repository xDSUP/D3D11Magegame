#pragma once
#include <D3D11_Framework.h>
#include <DirectXMath.h>

using namespace D3D11_Framework;
using namespace DirectX;


struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
};

class CubeTransformRender :
    public Render
{
public:
	CubeTransformRender();
	bool Init(HWND hwnd) override;
	bool Draw() override;
	void Close() override;

	void Update();

	void* operator new(size_t i) { return _aligned_malloc(i, 16); }
	void operator delete(void* p) { _aligned_free(p); }
private:
	ID3D11Buffer* _pVertexBuffer;
	ID3D11InputLayout* _pVertexLayout;
	ID3D11VertexShader* _pVertexShader;
	ID3D11PixelShader* _pPixelShader;

	ID3D11Buffer* _pIndexBuffer;
	ID3D11Buffer* _pConstantBuffer;

	XMMATRIX _world1;
	XMMATRIX _world2;
	XMMATRIX _world3;
	XMMATRIX _view;
	XMMATRIX _projection;
};

