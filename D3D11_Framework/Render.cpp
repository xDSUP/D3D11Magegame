#include "stdafx.h"
#include "Log.h"
#include "macros.h"

#include "Render.h"
#include "StaticMesh.h"

namespace D3D11_Framework
{
	//------------------------------------------------------------------

	Render::Render()
	{
		_driverType = D3D_DRIVER_TYPE_NULL;
		_featureLevel = D3D_FEATURE_LEVEL_11_0;
		_device = nullptr;
		_context = nullptr;
		_swapChain = nullptr;
		_renderTargetView = nullptr;
		_pDepthStencil = nullptr;
		_pDepthStencilView = nullptr;
		_pDepthStencilState = nullptr;
		_pDepthDisabledStencilState = nullptr;
	}

	Render::~Render()
	{}

	/**
	* \brief
	*
	��� ������������� Direct3D ��������:
	1. ������� ����������� swap chain, �������� ��������� DXGI_SWAP_CHAIN_DESC
	2. �������� ���������� ID3D11Device � IDXGISwapChain ��������� ������� D3D11CreateDeviceAndSwapChain
	3. �������� render target ��� ������� ������ swap chain
	4. ������� ����� �������/������� (stencil) � ������� ��� � depth/stencil view
	5. ������������ render target � depth/stencil view � �������� ���������� (rendering pipeline), ����� Direct3D ��� �� ������������.
	6. ���������� ������� (viewport).
	 */
	bool Render::CreateDevice(HWND hwnd)
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
		ZeroMemory(&sd, sizeof(sd));	// ������� ���������
		sd.BufferCount = 1;				// � ��� ���� ������ �����
		sd.BufferDesc.Width = width;	// ������������ ������ ������
		sd.BufferDesc.Height = height;	// � ������
		sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // ������ �������
		sd.BufferDesc.RefreshRate.Numerator = 60; // ������� ���������� ������
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // ���������� ������
		sd.OutputWindow = hwnd;			// ���������� ����
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;				// ������������� ������� �����

		for (UINT i = 0; i < numDriverTypes; i++)
		{
			_driverType = driverTypes[i];
			hr = D3D11CreateDeviceAndSwapChain(
				NULL, _driverType, NULL,
				createDeviceFlags, featureLevels,
				numFeatureLevels,
				D3D11_SDK_VERSION,
				&sd, &_swapChain, &_device,
				&_featureLevel, &_context
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

		// ������� �� ������� ��������,
		D3D11_TEXTURE2D_DESC descDepth;
		ZeroMemory(&descDepth, sizeof(descDepth));
		descDepth.Width = width;
		descDepth.Height = height;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		hr = _device->CreateTexture2D(&descDepth, NULL, &_pDepthStencil);
		if (FAILED(hr))
			return false;
		// �������� �������
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		hr = _device->CreateDepthStencilState(&depthStencilDesc, &_pDepthStencilState);
		if (FAILED(hr))
			return false;

		depthStencilDesc.DepthEnable = false;
		hr = _device->CreateDepthStencilState(&depthStencilDesc, &_pDepthDisabledStencilState);
		if (FAILED(hr))
			return false;

		//��������� ��� ����� � ������� � �������� ��� ����� ��������.
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = descDepth.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		hr = _device->CreateDepthStencilView(_pDepthStencil, &descDSV, &_pDepthStencilView);
		if (FAILED(hr))
			return false;

		_context->OMSetRenderTargets(1, &_renderTargetView, _pDepthStencilView);

		// ����� �� ��������� ���� �������,
		// � ������� ���������� ����� �������� ��������� ��������.
		// ��� ���� ����� �������
		_context->OMSetRenderTargets(1, &_renderTargetView, _pDepthStencilView);

		// ������� ���������� ���� ���������.
		// ����� �� ��������� ��� �������� ���� ������� �� ����� 0,0 (��������� TopLeftX � TopLeftY)
		D3D11_VIEWPORT vp;
		vp.Width = (FLOAT)width;
		vp.Height = (FLOAT)height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		_context->RSSetViewports(1, &vp);
		
		_projection = XMMatrixPerspectiveFovLH(0.4f * 3.14f,
			(float)width / height, 1.0f, 1000.0f);

		return Init(hwnd);
	}

	void Render::BeginFrame()
	{
		TurnZBufferOn();
		// �������, �������, �����, �����
		float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f };
		// ����� ������ ����� � ������� ��� �������� ������.
		_context->ClearRenderTargetView(_renderTargetView, ClearColor);
		// ������� ����� ������� ( ��������� ��� ���������)
		_context->ClearDepthStencilView(_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	void Render::EndFrame()
	{
		// ����� ������ ����� � ������� ��� �� ����� 
		_swapChain->Present(0, 0);
		// � �������� ����� ������ ������ � ������ ���
		// ��������� ����� ������������ � ����.
	}

	void Render::Shutdown()
	{
		Close();
		
		if (_context)
			_context->ClearState();

		_RELEASE(_renderTargetView);
		_RELEASE(_swapChain);
		_RELEASE(_context);
		_RELEASE(_device);
		_RELEASE(_pDepthStencil);
		_RELEASE(_pDepthStencilView);
		_RELEASE(_pDepthStencilState);
		_RELEASE(_pDepthDisabledStencilState);
	}

	void Render::TurnZBufferOn()
	{
		_context->OMSetDepthStencilState(_pDepthStencilState, 1);
	}

	void Render::TurnZBufferOff()
	{
		_context->OMSetDepthStencilState(_pDepthDisabledStencilState, 1);
	}

	HRESULT Render::_compileShaderFromFile(const wchar_t* FileName, LPCSTR EntryPoint, LPCSTR ShaderModel,
	                                       ID3DBlob** ppBlobOut)
	{
		HRESULT hr = S_OK;

		DWORD ShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
		ShaderFlags |= D3DCOMPILE_DEBUG;
#endif

		ID3DBlob* pErrorBlob;
		hr = D3DX11CompileFromFile(FileName, NULL, NULL, EntryPoint, ShaderModel, ShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL);
		if (FAILED(hr) && pErrorBlob != NULL)
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());

		_RELEASE(pErrorBlob);
		return hr;
		
	}

	//------------------------------------------------------------------
}