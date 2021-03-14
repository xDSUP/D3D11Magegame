#pragma once
#include "stdafx.h"
#include "InputCodes.h"

namespace D3D11_Framework
{
	//------------------------------------------------------------------

	class InputListener;

	class InputManager
	{
	public:
		void Init();
		void Close();

		void Run(const UINT& msg, WPARAM wParam, LPARAM lParam);

		void AddListener(InputListener* listener);

		// ���� ����
		void SetWinRect(const RECT& winrect);

	private:
		// ������� �������� ����
		void _EventCursor();
		// ������� ������ ����
		void _EventMouse(const eMouseKeyCodes keyCode, bool press);
		// ������� �������� ��������
		void _EventMouseWheel(short Value);
		// ��������� ������� �������
		void _EventKey(const eKeyCodes KeyCode, const wchar_t ch, bool press);

		std::list<InputListener*> _listener;

		RECT _windowRect;
		int _curx;
		int _cury;
		int _mouseWheel;
	};

	//------------------------------------------------------------------
}