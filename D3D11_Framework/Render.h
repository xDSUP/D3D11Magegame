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

		void* operator new(size_t i) { return _aligned_malloc(i, 16); }
		void operator delete(void* p) { _aligned_free(p); }
	
	protected:
		
		/**
		 * \brief загружает код шейдера из файла
		 * \param FileName имя файла
		 * \param EntryPoint название функции
		 * \param ShaderModel модель шейдера
		 * \param ppBlobOut ст-ра, куда грузить
		 * \return 
		 */
		HRESULT _compileShaderFromFile(const wchar_t* FileName, LPCSTR EntryPoint, LPCSTR ShaderModel, ID3DBlob** ppBlobOut);
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

		/**
		 * \brief  текстура которая будет представлять наш буфер глубины
		 */
		ID3D11Texture2D* _pDepthStencil;
		
		/**
		 * \brief отвечает за буфер глубины
		 */
		ID3D11DepthStencilView* _pDepthStencilView;
	};

	//------------------------------------------------------------------
}