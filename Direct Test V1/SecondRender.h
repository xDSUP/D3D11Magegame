#pragma once
#include "D3D11_Framework.h"
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

class SecondRender : public Render
{
public:
	SecondRender();
	bool Init(HWND hwnd) override;
	void Close() override;
	bool Draw() override;

	// чтобы был кусок памяти выравненный на 16 байт
void * operator new(size_t i){
	return _aligned_malloc(i, 16);
}

void operator delete(void* p){
	_aligned_free(p);
}
private:
	HRESULT _compileShaderFromFile(const wchar_t* FileName, LPCSTR EntryPoint, LPCSTR ShaderModel, ID3DBlob** ppBlobOut);

	ID3D11Buffer* _pVertexBuffer;
	ID3D11InputLayout* _pVertexLayout;
	ID3D11VertexShader* _pVertexShader;
	ID3D11PixelShader* _pPixelShader;

	XMMATRIX _world;
	XMMATRIX _view;
	XMMATRIX _projection;

	ID3D11Buffer* _pIndexBuffer;
	ID3D11Buffer* _pConstantBuffer;
};

