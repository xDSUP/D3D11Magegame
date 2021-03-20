#include "CubeTransformRender.h"


CubeTransformRender::CubeTransformRender():
_pVertexBuffer(nullptr),
_pVertexLayout(nullptr),
_pVertexShader(nullptr),
_pPixelShader(nullptr),
 _pIndexBuffer(nullptr),
 _pConstantBuffer(nullptr),
 _world1(), _world2(),
 _view(), _projection()
{
}

bool CubeTransformRender::Init(HWND hwnd)
{
	HRESULT hr = S_OK;
	ID3DBlob* pVSBlob = NULL;
	hr = _compileShaderFromFile(L"shader.fx", "VS", "vs_4_0", &pVSBlob);
	if (FAILED(hr))
	{
		sLog->Err("Невозможно скомпилировать файл shader.fx. Пожалуйста, запустите данную программу из папки, содержащей этот файл");
		return false;
	}

	hr = _device->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &_pVertexShader);
	if (FAILED(hr))	{ _RELEASE(pVSBlob); return false; }


	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	hr = _device->CreateInputLayout(layout, numElements, 
		pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &_pVertexLayout);
	_RELEASE(pVSBlob);
	if (FAILED(hr)) return false;

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
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
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

	_world1 = XMMatrixIdentity();
	_world2 = XMMatrixIdentity();
	_world3 = XMMatrixIdentity();

	XMVECTOR Eye = XMVectorSet(0.0f, 1.0f, -5.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	_view = XMMatrixLookAtLH(Eye, At, Up);

	float width = 640.0f;
	float height = 480.0f;
	_projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, width / height, 0.01f, 100.0f);

	return true;
}

bool CubeTransformRender::Draw()
{
	Update();

	ConstantBuffer cb1;
	cb1.mWorld = XMMatrixTranspose(_world1);
	cb1.mView = XMMatrixTranspose(_view);
	cb1.mProjection = XMMatrixTranspose(_projection);
	_context->UpdateSubresource(_pConstantBuffer, 0,
		NULL, &cb1, 0, 0);

	_context->VSSetShader(_pVertexShader, NULL, 0);
	_context->VSSetConstantBuffers(0, 1, &_pConstantBuffer);
	_context->PSSetShader(_pPixelShader, NULL, 0);
	_context->DrawIndexed(36, 0, 0);

	ConstantBuffer cb2;
	cb2.mWorld = XMMatrixTranspose(_world2);
	cb2.mView = XMMatrixTranspose(_view);
	cb2.mProjection = XMMatrixTranspose(_projection);
	_context->UpdateSubresource(_pConstantBuffer, 0,
		NULL, &cb2, 0, 0);
	_context->DrawIndexed(36, 0, 0);

	ConstantBuffer cb3;
	cb3.mWorld = XMMatrixTranspose(_world3);
	cb3.mView = XMMatrixTranspose(_view);
	cb3.mProjection = XMMatrixTranspose(_projection);
	_context->UpdateSubresource(_pConstantBuffer, 0,
		NULL, &cb3, 0, 0);
	_context->DrawIndexed(36, 0, 0);
	return true;
}

void CubeTransformRender::Close()
{
	_RELEASE(_pConstantBuffer);
	_RELEASE(_pVertexBuffer);
	_RELEASE(_pIndexBuffer);
	_RELEASE(_pVertexLayout);
	_RELEASE(_pVertexShader);
	_RELEASE(_pPixelShader);
}

void CubeTransformRender::Update()
{
	static float t = 0.0f;
	static DWORD timeStart = 0;
	DWORD timeCur = GetTickCount();
	if (timeStart == 0)
		timeStart = timeCur;
	t = (timeCur - timeStart) / 1000.0f;

	_world1 = XMMatrixRotationY(t) * XMMatrixRotationX(t);
	
	XMMATRIX mScale = XMMatrixScaling(0.3f, 0.3f, 0.3f);
	XMMATRIX mSpin = XMMatrixRotationZ(-t);
	XMMATRIX mTraslate = XMMatrixTranslation(-4.0f, 0.0f, 0.0f);
	XMMATRIX mOrbit = XMMatrixRotationZ(-t * 2.0f);//*XMMatrixRotationX(t * 2.0f);

	_world2 = mScale * mSpin * mTraslate * mOrbit;
	
	_world3 = mScale * mSpin * XMMatrixTranslation(4.0f, 0.0f, 0.0f) * XMMatrixRotationY(-t * 2.0f);;
	//_world2 =  mTraslate;
}


