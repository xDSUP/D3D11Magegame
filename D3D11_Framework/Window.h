#pragma once

#include "InputManager.h"

namespace D3D11_Framework
{
	class InputManager;
	//------------------------------------------------------------------

	//class InputManager;

	struct DescWindow
	{
		DescWindow() :
			caption(L""),
			width(640),
			height(480),
			posx(200),
			posy(20),
			resizing(true)
		{}

		int posx;
		int posy;
		std::wstring caption;	///< ��������� ����
		int width;				///< ������ ���������� ����� ����
		int height;				///< ������ ���������� ����� ����
		bool resizing;
	};

	class Window
	{
	public:
		Window();

		static Window* Get() { return _wndthis; }

		// ������� ����
		bool Create(const DescWindow& desc);

		// ��������� ������� ����
		void RunEvent();

		// ������� ����.
		void Close();

		void SetInputManager(InputManager* inputmgr);

		HWND GetHWND() const { return _hwnd; }
		int GetLeft() const { return _desc.posx; }
		int GetTop() const { return _desc.posy; }
		int GetWidth() const { return _desc.width; }
		int GetHeight() const { return _desc.height; }
		// ������� ��������� ����
		const std::wstring& GetCaption() const { return _desc.caption; }

		// ��������, ���� �� ��������� � ������
		bool IsExit() const { return _isExit; }
		// �������� �� ���������� ����
		bool IsActive() const { return _active; }
		// �������� �� ��������� ����
		// ��������������: ����� ������ ��������� ���� �� ��������� �������
		bool IsResize()
		{
			bool ret = _isResize;
			_isResize = false;
			return ret;
		}

		// ��������� �������
		LRESULT WndProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
	private:
		void _UpdateWindowState();

		static Window* _wndthis;

		DescWindow _desc;	// �������� ����
		InputManager* _inputManager;
		HWND _hwnd;		// ���������� ����	
		bool _isExit;		// ���� ���������� � ������� ������	
		bool _active;		// ���� �������?
		bool _minimized;
		bool _maximized;
		bool _isResize;	// ���� ���� �������� ������
	};

	// ��������� �������
	static LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam);

	//------------------------------------------------------------------
}


