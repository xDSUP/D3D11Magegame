#pragma once

namespace D3D11_Framework
{
	//------------------------------------------------------------------

//Input Assembler(IA) Stage
//Vertex Shader(VS) Stage
//Hull Shader(HS) Stage
//Tessellator Shader(TS) Stage
//Domain Shader(DS) Stage
//Geometry Shader(GS) Stage
//Stream Output(SO) Stage
//Rasterizer(RS) Stage
//Pixel Shader(PS) Stage
//Output Merger(OM) Stage
	
	class Render
	{
	public:
		Render();
		virtual ~Render();

		
		/**
		 * \brief ������������� DirectX
		 * \param hwnd 
		 * \return 
		 */
		bool CreateDevice(HWND hwnd);
		
		/**
		 * \brief ������� �����
		 */
		void BeginFrame();
		/**
		 * \brief ������ ������
		 */
		void EndFrame();
		/**
		 * \brief ������� �������
		 */
		void Shutdown();
		
		virtual bool Init(HWND hwnd) = 0;
		virtual bool Draw() = 0;
		virtual void Close() = 0;

	protected:
		D3D_DRIVER_TYPE _driverType;
		D3D_FEATURE_LEVEL _featureLevel;
		/**
		 * \brief �������� ������ ��� �������� ��������.
		 */
		ID3D11Device* _device;
		/**
		 * \brief  �������� �� ����� �������
		 */
		IDXGISwapChain* _swapChain;

		/**
		 * immediate (����������������)
		 * \brief ������������ ����������� ��� ���������� ���������� � �����
		 */
		ID3D11DeviceContext* _context;
		/**
		 * \brief ����� ������ ������� ������ � ������� �� ����� �������� ���� �����.
		 */
		ID3D11RenderTargetView* _renderTargetView;
	};

	//------------------------------------------------------------------
}