#include "stdafx.h"
#include "InputManager.h"
#include "InputCodes.h"
#include "InputListener.h"
#include "Log.h"

namespace D3D11_Framework
{
	void InputManager::Init()
	{
		_mouseWheel = _curx = _cury = 0;
		sLog->Debug("InputManager Init");
	}

	void InputManager::Close()
	{
		if (!_listener.empty())
			_listener.clear();
		sLog->Debug("InputManager Close");
	}

	void InputManager::Run(const UINT& msg, WPARAM wParam, LPARAM lParam)
	{
		if(_listener.empty())
			return;

		eKeyCodes KeyIndex;
		wchar_t buffer[1];
		BYTE lpKeyState[256];

		_EventCursor();// событие движения мыши
		switch (msg)
		{
		case WM_KEYDOWN:
			KeyIndex = static_cast<eKeyCodes>(wParam);
			GetKeyboardState(lpKeyState);
			ToUnicode(wParam, HIWORD(lParam) & 0xFF, lpKeyState, buffer, 1, 0);
			_EventKey(KeyIndex, buffer[0], true);
			break;
		case WM_KEYUP:
			KeyIndex = static_cast<eKeyCodes>(wParam);
			GetKeyboardState(lpKeyState);
			ToUnicode(wParam, HIWORD(lParam) & 0xFF, lpKeyState, buffer, 1, 0);
			_EventKey(KeyIndex, buffer[0], false);
			break;
		case WM_LBUTTONDOWN:
			_EventMouse(MOUSE_LEFT, true);
			break;
		case WM_LBUTTONUP:
			_EventMouse(MOUSE_LEFT, false);
			break;
		case WM_RBUTTONDOWN:
			_EventMouse(MOUSE_RIGHT, true);
			break;
		case WM_RBUTTONUP:
			_EventMouse(MOUSE_RIGHT, false);
			break;
		case WM_MBUTTONDOWN:
			_EventMouse(MOUSE_MIDDLE, true);
			break;
		case WM_MBUTTONUP:
			_EventMouse(MOUSE_MIDDLE, false);
			break;
		case WM_MOUSEWHEEL:
			_EventMouseWheel((short)GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA);
			break;
		}
	}

	void InputManager::AddListener(InputListener* listener)
	{
		_listener.push_back(listener);
	}

	void InputManager::SetWinRect(const RECT& winrect)
	{
		_windowRect.left = winrect.left;
		_windowRect.right = winrect.right;
		_windowRect.top = winrect.top;
		_windowRect.bottom = winrect.bottom;
	}

	void InputManager::_EventCursor()
	{
		POINT Position;
		GetCursorPos(&Position);	// получаем текущую позицию курсора

		Position.x -= _windowRect.left;
		Position.y -= _windowRect.top;

		if (_curx == Position.x && _cury == Position.y)
			return;

		_curx = Position.x;
		_cury = Position.y;

		for (auto it = _listener.begin(); it != _listener.end(); ++it)
		{
			if (!(*it))
				continue;
			else if ((*it)->MouseMove(MouseEvent(_curx, _cury)) == true)
				return;
		}
	}

	void InputManager::_EventMouse(const eMouseKeyCodes keyCode, bool press)
	{
		for (auto it = _listener.begin(); it != _listener.end(); ++it)
		{
			if (!(*it))
				continue;
			// кнопка нажата
			if (press == true)
			{
				if ((*it)->MousePressed(MouseEventClick(keyCode, _curx, _cury)) == true)
					return;
			}
			// кнопка отпущена
			else
			{
				if ((*it)->MouseReleased(MouseEventClick(keyCode, _curx, _cury)) == true)
					return;
			}
		}
	}

	void InputManager::_EventMouseWheel(short Value)
	{
		if (_mouseWheel == Value)
			return;

		_mouseWheel = Value;

		for (auto it = _listener.begin(); it != _listener.end(); ++it)
		{
			if (!(*it))
				continue;
			else if ((*it)->MouseWheel(MouseEventWheel(_mouseWheel, _curx, _cury)) == true)
				return;
		}
	}

	void InputManager::_EventKey(const eKeyCodes KeyCode, const wchar_t ch, bool press)
	{
		for (auto it = _listener.begin(); it != _listener.end(); ++it)
		{
			if (!(*it))
				continue;
			// кнопка нажата
			if (press == true)
			{
				if ((*it)->KeyPressed(KeyEvent(ch, KeyCode)) == true)
					return;
			}
			// кнопка отпущена
			else
			{
				if ((*it)->KeyReleased(KeyEvent(ch, KeyCode)) == true)
					return;
			}
		}
	}
}
