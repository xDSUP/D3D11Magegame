#pragma once

namespace D3D11_Framework
{
	//------------------------------------------------------------------

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

	private:
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