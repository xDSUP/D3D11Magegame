#include "CubeTransformRender.h"


CubeTransformRender::CubeTransformRender():
	_pVertexBuffer(nullptr),
	_pVertexLayout(nullptr),
	_pVertexShader(nullptr),
	_pPixelShader(nullptr),
	_pIndexBuffer(nullptr),
	_pConstantBuffer(nullptr),
	_pTextureRV(nullptr),
	_pSamplerState(nullptr),
	_rot(0.01),
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
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,12, D3D11_INPUT_PER_VERTEX_DATA, 0}
		//{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
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
	{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
	{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
	{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
	{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
	{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT2(1.0f, 1.0f) },
	{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT2(0.0f, 1.0f) },
	{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
	{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT2(1.0f, 0.0f) },
	{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f) },
	{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 1.0f) },
	};

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 24;
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
		6,4,5,
		7,4,6,
		11,9,8,
		10,9,11,
		14,12,13,
		15,12,14,
		19,17,16,
		18,17,19,
		22,20,21,
		23,20,22
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

	hr = D3DX11CreateShaderResourceViewFromFile(_device,
		L"texture.png", NULL, NULL, &_pTextureRV, NULL);
	if (FAILED(hr)) return false;

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = _device->CreateSamplerState(&sampDesc, &_pSamplerState);
	if (FAILED(hr)) return false;
	
	_world1 = XMMatrixIdentity();
	_world2 = XMMatrixIdentity();

	XMVECTOR Eye = XMVectorSet(0.0f, 3.0f, -8.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	_view = XMMatrixLookAtLH(Eye, At, Up);

	float width = 640.0f;
	float height = 480.0f;
	_projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, width / height, 0.01f, 1000.0f);

	return true;
}

bool CubeTransformRender::Draw()
{
	//Update();
	_rot += .0005f;
	if (_rot > 6.26f)
		_rot = 0.0f;

	XMVECTOR rotaxis = XMVectorSet(0, 1, 0, 0);
	XMMATRIX rotation = XMMatrixRotationAxis(rotaxis, _rot);
	XMMATRIX translation = XMMatrixTranslation(0, 0, 4);

	_world1 = translation * rotation;
	rotation = XMMatrixRotationAxis(rotaxis, -_rot);
	XMMATRIX scale = XMMatrixScaling(1.3f, 1.3f, 1.3f);

	_world2 = rotation * scale;

	XMMATRIX WVP = _world1 * _view * _projection;
	ConstantBuffer cb;
	cb.MVP = XMMatrixTranspose(WVP);
	
	_context->UpdateSubresource(_pConstantBuffer, 0,
		NULL, &cb, 0, 0);

	_context->VSSetShader(_pVertexShader, NULL, 0);
	_context->VSSetConstantBuffers(0, 1, &_pConstantBuffer);
	_context->PSSetShader(_pPixelShader, NULL, 0);
	_context->PSSetShaderResources(0, 1, &_pTextureRV);
	_context->PSSetSamplers(0, 1, &_pSamplerState);
	_context->DrawIndexed(36, 0, 0);

	WVP = _world2 * _view * _projection;
	cb.MVP = XMMatrixTranspose(WVP);
	_context->UpdateSubresource(_pConstantBuffer, 0,
		NULL, &cb, 0, 0);
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
	_RELEASE(_pSamplerState);
	_RELEASE(_pTextureRV);
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
	
	//_world2 =  mTraslate;
}


