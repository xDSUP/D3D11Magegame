#pragma once


#include <vector>

#include "Camera.h"
#include "LightHelper.h"
#include "RenderState.h"

namespace D3D11Framework
{
//------------------------------------------------------------------
	
	class Render
	{
		friend class StaticMesh;
		friend class Image;
		friend class BitmapFont;
		friend class Text;
		friend class Shader;
	public:
		Render();
		virtual ~Render();

		bool CreateDevice(HWND hwnd);
		void BeginFrame();
		void EndFrame();
		void Shutdown();

		virtual bool Init() = 0;
		virtual bool Draw() = 0;
		virtual void Close() = 0;

		void TurnZBufferOn();
		void TurnZBufferOff();

		void TurnOnAlphaBlending();
		void TurnOffAlphaBlending();

		ID3D11DeviceContext* GetContext() const
		{
			return m_pImmediateContext;
		}

		ID3D11Device* GetDevice() const
		{
			return m_pd3dDevice;
		}

		DirectionalLight* GetDirectionalLights() 
		{
			return m_DirectionalLights;
		}

		PointLight* GetPointLights()  
		{
			return m_PointLights;
		}

		SpotLight* GetSpotLights() 
		{
			return m_SpotLights;
		}

		int NumDirLight() const
		{
			return numDirLight;
		}

		int NumPointLight() const
		{
			return numPointLight;
		}

		int NumSpotLight() const
		{
			return numSpotLight;
		}

		Camera* GetCam()
		{
			return &cam;
		}
		
		void* operator new(size_t i)
		{
			return _aligned_malloc(i,16);
		}

		void operator delete(void* p)
		{
			_aligned_free(p);
		}

	protected:
		bool m_createdevice();
		bool m_createdepthstencil();
		void m_initmatrix();		
		void m_resize();

		RenderState *m_renderstate;

		DirectionalLight m_DirectionalLights[4];
		PointLight m_PointLights[10];
		SpotLight m_SpotLights[4];
		int numDirLight;
		int numPointLight;
		int numSpotLight;
		
		ID3D11Device *m_pd3dDevice;
		ID3D11DeviceContext *m_pImmediateContext;
		IDXGISwapChain *m_pSwapChain;
		ID3D11RenderTargetView *m_pRenderTargetView;
		ID3D11DepthStencilView *m_pDepthStencilView;
		XMMATRIX m_Ortho;
		XMMATRIX m_Projection;
		HWND m_hwnd;
		unsigned int m_width;
		unsigned int m_height;
		Camera		cam;
	};

//------------------------------------------------------------------
}