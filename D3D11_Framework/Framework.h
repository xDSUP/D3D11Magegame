#pragma once

#include "Window.h"
#include "Render.h"
#include "InputManager.h"
#include "Log.h"

namespace D3D11_Framework
{
	//------------------------------------------------------------------

	struct FrameworkDesc
	{
		DescWindow wnd;
		Render* render;
	};
	
	class Framework
	{
	public:
		Framework();
		~Framework();

		/**
		 * \brief инициализация
		 * \return 
		 */
		bool Init(const FrameworkDesc& desc);
		/**
		 * \brief основной цикл программы
		 */
		void Run();
		void Close();

		void AddInputListener(InputListener* listener);

		// оставлено для поддержки совместимости со старыми версиями)
		
	protected:
		
		/**
		 * \brief расчёт одного кадра
		 * \return 
		 */
		bool _frame();

		FrameworkDesc _desc;
		Window* _wnd;
		Render* _render;
		InputManager* _input;
		Log _log;
		bool _init;		// если было инициализировано
	};

	//------------------------------------------------------------------
}