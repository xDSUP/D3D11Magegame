#include "stdafx.h"
#include "Window.h"
#include "InputManager.h"
#include "Log.h"

namespace D3D11_Framework
{
	Window* Window::_wndthis = nullptr;
	
	Window::Window(void) :
		_inputManager(nullptr),
		_hwnd(0),
		_isExit(false),
		_active(true),
		_minimized(false),
		_maximized(false),
		_isResize(false)
	{
		if (!_wndthis)
			_wndthis = this;
		else
			sLog->Err("Window уже был создан");
	}

	bool Window::Create(const DescWindow& desc)
	{
		sLog->Debug("Window Create");
		_desc = desc;

		WNDCLASSEXW wnd;

		wnd.cbSize = sizeof(WNDCLASSEXW);
		wnd.style = CS_HREDRAW | CS_VREDRAW;
		wnd.lpfnWndProc = StaticWndProc;
		wnd.cbClsExtra = 0;
		wnd.cbWndExtra = 0;
		wnd.hInstance = 0;
		wnd.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wnd.hIconSm = wnd.hIcon;
		wnd.hCursor = LoadCursor(0, IDC_ARROW);
		wnd.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wnd.lpszMenuName = NULL;
		wnd.lpszClassName = L"D3D11F";
		wnd.cbSize = sizeof(WNDCLASSEX);

		if (!RegisterClassEx(&wnd))
		{
			sLog->Err("Ќе удалось зарегистрировать окно");
			return false;
		}

		RECT rect = { 0, 0, _desc.width, _desc.height };
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW | WS_VISIBLE, FALSE);

		long lwidth = rect.right - rect.left;
		long lheight = rect.bottom - rect.top;

		long lleft = (long)_desc.posx;
		long ltop = (long)_desc.posy;

		_hwnd = CreateWindowEx(
			NULL, 
			L"D3D11F", 
			_desc.caption.c_str(), 
			WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
			lleft, ltop, 
			lwidth, lheight, 
			NULL, NULL, 
			NULL, NULL);

		if (!_hwnd)
		{
			sLog->Err("Ќе удалось создать окно");
			return false;
		}

		ShowWindow(_hwnd, SW_SHOW);
		UpdateWindow(_hwnd);

		return true;
	}

	void Window::RunEvent()
	{
		MSG msg;
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE ))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void Window::Close()
	{
		if (_hwnd)
			DestroyWindow(_hwnd);
		_hwnd = nullptr;
		sLog->Debug("Window Close");
	}

	void Window::SetInputManager(InputManager* inputmgr)
	{
		_inputManager = inputmgr;
		_UpdateWindowState();
	}

	LRESULT Window::WndProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (nMsg)
		{
		case WM_CREATE:
			return 0;
		case WM_CLOSE:
			_isExit = true;
			return 0;
		case WM_ACTIVATE:
			if (LOWORD(wParam) != WA_INACTIVE)
				_active = true;
			else
				_active = false;
			return 0;
		case WM_MOVE:
			_desc.posx = LOWORD(lParam);
			_desc.posy = HIWORD(lParam);
			_UpdateWindowState();
			return 0;
		case WM_SIZE:
			if (!_desc.resizing)
				return 0;
			_desc.width = LOWORD(lParam);
			_desc.height = HIWORD(lParam);
			_isResize = true;
			if (wParam == SIZE_MINIMIZED)
			{
				_active = false;
				_minimized = true;
				_maximized = false;
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				_active = true;
				_minimized = false;
				_maximized = true;
			}
			else if (wParam == SIZE_RESTORED)
			{
				if (_minimized)
				{
					_active = true;
					_minimized = false;
				}
				else if (_maximized)
				{
					_active = true;
					_maximized = false;
				}
			}
			_UpdateWindowState();
			return 0;
		case WM_MOUSEMOVE: case WM_LBUTTONUP: case WM_LBUTTONDOWN: case WM_MBUTTONUP: case WM_MBUTTONDOWN: case WM_RBUTTONUP: case WM_RBUTTONDOWN: case WM_MOUSEWHEEL: case WM_KEYDOWN: case WM_KEYUP:
			if (_inputManager)
				_inputManager->Run(nMsg, wParam, lParam);
			return 0;
		}

		return DefWindowProcW(hwnd, nMsg, wParam, lParam);
	}

	void Window::_UpdateWindowState()
	{
		RECT clientRect;
		clientRect.left = _desc.posx;
		clientRect.right = _desc.width;
		clientRect.top = _desc.posy;
		clientRect.bottom = _desc.height;
		if (_inputManager)
			_inputManager->SetWinRect(clientRect);
	}

	LRESULT __stdcall StaticWndProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
	{
		return Window::Get()->WndProc(hwnd, nMsg, wParam, lParam);
	}
}
