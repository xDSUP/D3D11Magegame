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
		 * \brief �������������
		 * \return 
		 */
		bool Init();
		/**
		 * \brief �������� ���� ���������
		 */
		void Run();
		void Close();

		void SetRender(Render* render) { _render = render; }
		void AddInputListener(InputListener* listener);
	protected:
		
		/**
		 * \brief ������ ������ �����
		 * \return 
		 */
		bool _frame();

		Window* _wnd;
		Render* _render;
		InputManager* _input;
		Log _log;
		bool _init;		// ���� ���� ����������������
	};

	//------------------------------------------------------------------
}