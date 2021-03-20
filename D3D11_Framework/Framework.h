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
		 * \brief �������������
		 * \return 
		 */
		bool Init(const FrameworkDesc& desc);
		/**
		 * \brief �������� ���� ���������
		 */
		void Run();
		void Close();

		void AddInputListener(InputListener* listener);

		// ��������� ��� ��������� ������������� �� ������� ��������)
		
	protected:
		
		/**
		 * \brief ������ ������ �����
		 * \return 
		 */
		bool _frame();

		FrameworkDesc _desc;
		Window* _wnd;
		Render* _render;
		InputManager* _input;
		Log _log;
		bool _init;		// ���� ���� ����������������
	};

	//------------------------------------------------------------------
}