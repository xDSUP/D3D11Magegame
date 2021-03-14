#pragma once

#include "Window.h"
#include "Render.h"
#include "InputManager.h"
#include "Log.h"



namespace D3D11_Framework
{
	//------------------------------------------------------------------

	class Framework
	{
	public:
		Framework();
		~Framework();

		/**
		 * \brief инициализация
		 * \return 
		 */
		bool Init();
		/**
		 * \brief основной цикл программы
		 */
		void Run();
		void Close();

		void SetRender(Render* render) { _render = render; }
		void AddInputListener(InputListener* listener);
	protected:
		
		/**
		 * \brief расчёт одного кадра
		 * \return 
		 */
		bool _frame();

		Window* _wnd;
		Render* _render;
		InputManager* _input;
		Log _log;
		bool _init;		// если было инициализировано
	};

	//------------------------------------------------------------------
}