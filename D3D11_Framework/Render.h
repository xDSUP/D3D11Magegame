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
		 * \brief Инициализация DirectX
		 * \param hwnd 
		 * \return 
		 */
		bool CreateDevice(HWND hwnd);
		
		/**
		 * \brief очищает экран
		 */
		void BeginFrame();
		/**
		 * \brief меняет буферы
		 */
		void EndFrame();
		/**
		 * \brief очищает ресурсы
		 */
		void Shutdown();
		
		virtual bool Init(HWND hwnd) = 0;
		virtual bool Draw() = 0;
		virtual void Close() = 0;

	protected:
		D3D_DRIVER_TYPE _driverType;
		D3D_FEATURE_LEVEL _featureLevel;
		/**
		 * \brief содержит методы для создания ресурсов.
		 */
		ID3D11Device* _device;
		/**
		 * \brief  отвечает за смену буферов
		 */
		IDXGISwapChain* _swapChain;

		/**
		 * immediate (непосредственный)
		 * \brief используется приложением для выполнения рендеринга в буфер
		 */
		ID3D11DeviceContext* _context;
		/**
		 * \brief бъект нашего заднего буфера в котором мы будем рисовать нашу сцену.
		 */
		ID3D11RenderTargetView* _renderTargetView;
	};

	//------------------------------------------------------------------
}