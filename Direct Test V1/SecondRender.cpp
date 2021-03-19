#include "SecondRender.h"

#include <d3dcompiler.h>



SecondRender::SecondRender()
{
	_pPixelShader = nullptr;
	_pPixelShader = nullptr;
	_pVertexLayout = nullptr;
	_pVertexBuffer = nullptr;

	_pIndexBuffer = nullptr;
	_pConstantBuffer = nullptr;
}

bool SecondRender::Init(HWND hwnd)
{
	HRESULT hr = S_OK;
	ID3DBlob* pVSBlob = NULL;

	// Компиляция вершинного шейдера
	hr = _compileShaderFromFile(L"shader.fx", "VS", "vs_4_0", &pVSBlob);
	if (FAILED(hr))
	{
		sLog->Err("Невозможно скомпилировать файл shader.fx. Пожалуйста, запустите данную программу из папки, содержащей этот файл");
		return false;
	}

	// Создание вершинного шейдера
	hr = _device->CreateVertexShader(
		pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), NULL, &_pVertexShader
	);
	if (FAILED(hr))
	{
		sLog->Err("Не удалось создать вершинный шейдер");
		_RELEASE(pVSBlob);
		return false;
	}
	// определение входного формата
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0,
			DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
			D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,
			0, 12,
			D3D11_INPUT_PER_VERTEX_DATA, 0
		}
	};

	UINT numElements = ARRAYSIZE(layout);
	hr = _device->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &_pVertexLayout);
	_RELEASE(pVSBlob);
	if (FAILED(hr))
		return false;
	_context->IASetInputLayout(_pVertexLayout);

	ID3DBlob* pPSBlob = NULL;
	hr = _compileShaderFromFile(L"shader.fx", "PS", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		sLog->Err("Невозможно скомпилировать файл shader.fx. Пожалуйста, запустите данную программу из папки, содержащей этот файл");
		return false;
	}

	hr = _device->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &_pPixelShader);
	_RELEASE(pPSBlob);
	if (FAILED(hr))
		return false;

	SimpleVertex vertices[] =
	{
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) }
	};
	
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 8;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;
	hr = _device->CreateBuffer(&bd, &InitData, &_pVertexBuffer);
	if (FAILED(hr))
		return false;

	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	_context->IASetVertexBuffers(0, 1, &_pVertexBuffer, &stride, &offset);

	WORD indices[] =
	{
		3,1,0,
		2,1,3,

		0,5,4,
		1,5,0,

		3,4,7,
		0,4,3,

		1,6,5,
		2,6,1,

		2,7,6,
		3,7,2,

		6,4,5,
		7,4,6,
	};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * 36;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = indices;
	hr = _device->CreateBuffer(&bd, &InitData, &_pIndexBuffer);
	if (FAILED(hr))
		return false;

	_context->IASetIndexBuffer(_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = _device->CreateBuffer(&bd, NULL, &_pConstantBuffer);
	if (FAILED(hr))
		return false;

	_world = XMMatrixIdentity();


	XMVECTOR Eye = XMVectorSet(0.0f, 1.0f, -5.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	_view = XMMatrixLookAtLH(Eye, At, Up);

	float width = 640.0f;
	float height = 480.0f;
	_projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, width / height, 0.01f, 100.0f);
	
	return  true;
}

void SecondRender::Close()
{
	_RELEASE(_pIndexBuffer);
	_RELEASE(_pConstantBuffer);
	_RELEASE(_pVertexBuffer);
	_RELEASE(_pVertexLayout);
	_RELEASE(_pVertexShader);
	_RELEASE(_pPixelShader);
}

bool SecondRender::Draw()
{
	static float t = 0.0f;
	static DWORD dwTimeStart = 0;
	DWORD dwTimeCur = GetTickCount();
	if (dwTimeStart == 0)
		dwTimeStart = dwTimeCur;
	t = (dwTimeCur - dwTimeStart) / 1000.0f;

	_world = XMMatrixRotationY(t) * XMMatrixRotationX(t);

	ConstantBuffer cb;
	cb.mWorld = XMMatrixTranspose(_world);
	cb.mView = XMMatrixTranspose(_view);
	cb.mProjection = XMMatrixTranspose(_projection);
	_context->UpdateSubresource(_pConstantBuffer, 0, NULL, &cb, 0, 0);
	_context->VSSetShader(_pVertexShader, NULL, 0);
	_context->VSSetConstantBuffers(0, 1, &_pConstantBuffer);
	_context->PSSetShader(_pPixelShader, NULL, 0);
	_context->DrawIndexed(36, 0, 0);

	return true;
}

HRESULT SecondRender::_compileShaderFromFile(const wchar_t* fileName, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** blobOut)
{
	HRESULT hr = S_OK;

	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	shaderFlags |= D3DCOMPILE_DEBUG;
#endif

	//D3DCompileFromFile()
	ID3DBlob* errorBlob;
	hr = D3DX11CompileFromFileW(
		fileName, NULL, NULL,
		entryPoint, shaderModel,
		shaderFlags, 0, NULL, blobOut,
		&errorBlob, NULL
	);
	if (FAILED(hr) && errorBlob != NULL)
		OutputDebugStringA((char*)errorBlob->GetBufferPointer());
	_RELEASE(errorBlob);
	return hr;
}
