#pragma once
#include "stdafx.h"
#include "Render.h"
#include <fstream>
#include "ms3dspec.h"
namespace D3D11_Framework
{
	/// <summary>
	/// Класс статичного меша
	/// </summary>
	class StaticMesh
	{
	public:
		StaticMesh();

		bool Init(Render* render, const wchar_t* name);
		/// <summary>
		/// отрисовка
		/// </summary>
		void Draw(CXMMATRIX viewMatrix);
		void Close();

		void Translate(float x, float y, float z);
		void Rotate(float angle, float x, float y, float z);
		void Scale(float x, float y, float z);
		void Identity();

		void* operator new(size_t i) { return _aligned_malloc(i, 16); }
		void operator delete(void* p) {_aligned_free(p); }
		
	private:
		bool _loadMS3DFile(const wchar_t* name);
		bool _loadTextures(const wchar_t* name);
		bool _initShader(wchar_t* vsFilename, wchar_t* psFilename);

		void _renderBuffers() const;
		void _setShaderParameters(CXMMATRIX viewMatrix);
		void _renderShader();

		D3D11_Framework::Render* _render;

		ID3D11Buffer* _vertexBuffer;
		ID3D11Buffer* _indexBuffer;
		ID3D11VertexShader* _vertexShader;
		ID3D11PixelShader* _pixelShader;
		ID3D11InputLayout* _layout;
		ID3D11Buffer* _pConstantBuffer;
		ID3D11SamplerState* _sampleState;
		ID3D11ShaderResourceView* _texture;

		XMMATRIX _objMatrix;
		unsigned short _indexCount;
		//float _rot;
	};

}