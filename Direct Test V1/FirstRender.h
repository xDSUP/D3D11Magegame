#pragma once

#include "D3D11_Framework.h"

using namespace D3D11_Framework;


class FirstRender : public Render
{
public:

	FirstRender();
	bool Init(HWND hwnd) override;
	bool Draw() override;
	void Close() override;
	

private:
	HRESULT _compileShaderFromFile(WCHAR* FileName, LPCSTR EntryPoint, LPCSTR ShaderModel, ID3DBlob** ppBlobOut);

	ID3D11Buffer* _vertexBuffer;
	ID3D11InputLayout* _vertexLayout;
	ID3D11VertexShader* _vertexShader;
	ID3D11PixelShader* _pixelShader;
};

