#include "FirstRender.h"
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXMathVector.inl>

using namespace DirectX;

struct SimpleVertex
{
	XMFLOAT3 Pos;
};

bool FirstRender::Init(HWND hwnd)
{
	HRESULT hr = S_OK;

	// Компиляция вершинного шейдера
	ID3DBlob* VSBlob = NULL;
	hr = _compileShaderFromFile(L"shader.fx", "VS", "vs_4_0", &VSBlob);
	if (FAILED(hr))
	{
		sLog->Err("Невозможно скомпилировать файл shader.fx. Пожалуйста, запустите данную программу из папки, содержащей этот файл");
		return false;
	}

	// Создание вершинного шейдера
	hr = _device->CreateVertexShader(
		VSBlob->GetBufferPointer(),
		VSBlob->GetBufferSize(), NULL, &_vertexShader
	);
	if (FAILED(hr))
	{
		sLog->Err("Не удалось создать вершинный шейдер");
		_RELEASE(VSBlob);
		return false;
	}
	// определение входного формата
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0,
			DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
			D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	UINT numElements = ARRAYSIZE(layout);
	// создание входного формата
	hr = _device->CreateInputLayout(layout, numElements,
		VSBlob->GetBufferPointer(),
		VSBlob->GetBufferSize(), &_vertexLayout);
	_RELEASE(VSBlob);
	if (FAILED(hr))
		return false;

	// установка входного формата
	_context->IASetInputLayout(_vertexLayout);

	// Компиляция пиксельного шейдера
	ID3DBlob* pSBlob = NULL;
	hr = _compileShaderFromFile(L"shader.fx", "PS", "ps_4_0", &pSBlob);
	if (FAILED(hr))
	{
		sLog->Err("Невозможно скомпилировать файл shader.fx. Пожалуйста, запустите данную программу из папки, содержащей этот файл");
		return false;
	}

	// Создание пискельного шейдера
	hr = _device->CreatePixelShader(pSBlob->GetBufferPointer(), pSBlob->GetBufferSize(), NULL, &_pixelShader);
	if (FAILED(hr))
	{
		sLog->Err("Не удалось создать пиксельный шейдер");
		_RELEASE(pSBlob);
		return false;
	}

	// Создание буфера вершин
	SimpleVertex verticles[] = {
		XMFLOAT3(0.0f, 0.5f, 0.5f),
		XMFLOAT3(0.5f, -0.5f, 0.5f),
		XMFLOAT3(-0.5f, -0.5f, 0.5f)
	};

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 3;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = verticles;

	hr = _device->CreateBuffer(&bd, &data, &_vertexBuffer);
	if (FAILED(hr))
		return false;

	// установка вершинного буфера
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	_context->IASetVertexBuffers(0, 1, &_vertexBuffer,
		&stride, &offset);
	// Установка топологии
	_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return true;
}
	


bool FirstRender::Draw()
{
	_context->VSSetShader(_vertexShader, NULL, 0);
	_context->PSSetShader(_pixelShader, NULL, 0);
	_context->Draw(3, 0);
	return true;
}


FirstRender::FirstRender()
{
	_vertexShader = nullptr;
	_vertexLayout = nullptr;
	_vertexBuffer = nullptr;
	_pixelShader = nullptr;
}

void FirstRender::Close()
{
	_RELEASE(_vertexBuffer);
	_RELEASE(_vertexLayout);
	_RELEASE(_vertexShader);
	_RELEASE(_pixelShader);
}

HRESULT FirstRender::_compileShaderFromFile(const wchar_t* fileName, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** blobOut)
{
	HRESULT hr = S_OK;
	//D3DCompileFromFile()
	hr = D3DX11CompileFromFileW(
		fileName, NULL, NULL, 
		entryPoint, shaderModel,
		0, 0, NULL, blobOut, 
		NULL, NULL
	);
	return hr;
}

