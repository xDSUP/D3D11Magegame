#include "FirstRender.h"

bool FirstRender::Init(HWND hwnd)
{
	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect(hwnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	UINT createDeviceFlags = 0;

#ifdef _DEBUG
	//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);
	
	D3D_FEATURE_LEVEL featureLevels[] = 
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));	// очищаем структуру
	sd.BufferCount = 1;				// у нас один задний буфер
	sd.BufferDesc.Width = width;	// устанвливаем ширину буфера
	sd.BufferDesc.Height = height;	// и высоту
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // формат пикселя
	sd.BufferDesc.RefreshRate.Numerator = 60; // частота обновления экрана
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // назначение буфера
	sd.OutputWindow = hwnd;			// дескриптор окна
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;				// устанавливает оконный режим

	for (UINT i = 0; i < numDriverTypes; i++)
	{
		_driverType = driverTypes[i];
		hr = D3D11CreateDeviceAndSwapChain(
			NULL, _driverType, NULL,
			createDeviceFlags, featureLevels,
			numFeatureLevels, 
			D3D11_SDK_VERSION,
			&sd, &_swapChain, &_device,
			&_featureLevel,	&_context
		);
		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
		return false;

	ID3D11Texture2D* backBuffer = NULL;
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), 
		(LPVOID*)&backBuffer);
	if (FAILED(hr))
		return false;
	
	hr = _device->CreateRenderTargetView(backBuffer, NULL, &_renderTargetView);
	_RELEASE(backBuffer);
	if (FAILED(hr))
		return false;
	// Здесь мы указываем цель рендера,
	// в которую видеокарта будет рисовать полученую картинку.
	_context->OMSetRenderTargets(1, &_renderTargetView, NULL);


	// Вьюпорт определяет зону рисования.
	// Здесь мы указываем что рисовать надо начиная из точки 0,0 (параметры TopLeftX и TopLeftY)
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	_context->RSSetViewports(1, &vp);
	
	return true;
}

bool FirstRender::Draw()
{
	
	return true;
}


FirstRender::FirstRender()
{
	_driverType = D3D_DRIVER_TYPE_NULL;
	_featureLevel = D3D_FEATURE_LEVEL_11_0;
	_device = nullptr;
	_context = nullptr;
	_swapChain = nullptr;
	_renderTargetView = nullptr;
}

void FirstRender::Close()
{
	
}
