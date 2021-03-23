#pragma once
#include "RenderModel.h"
#include <fstream>
#include "ms3dspec.h"

using namespace DirectX;

class RenderModel;

class StaticMesh
{
public:
	StaticMesh();

	bool Init(RenderModel* render, const wchar_t* name);
	void Render();
	void Close();
private:
	bool _loadMS3DFile(const wchar_t* name);
	bool _loadTextures(const wchar_t* name);
	bool _initShader(wchar_t* vsFilename, wchar_t* psFilename);

	void _renderBuffers() const;
	void _setShaderParameters();
	void _renderShader();

	RenderModel* _render;

	ID3D11Buffer* _vertexBuffer;
	ID3D11Buffer* _indexBuffer;
	ID3D11VertexShader* _vertexShader;
	ID3D11PixelShader* _pixelShader;
	ID3D11InputLayout* _layout;
	ID3D11Buffer* _pConstantBuffer;
	ID3D11SamplerState* _sampleState;
	ID3D11ShaderResourceView* _texture;

	XMMATRIX objM;
	unsigned short _indexCount;
	float _rot;

};

