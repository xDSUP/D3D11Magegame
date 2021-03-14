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
		std::wstring caption;	///< заголовок окна
		int width;				///< ширина клиентской части окна
		int height;				///< высота клиентской части окна
		bool resizing;
	};

	class Window
	{
	public:
		Window();

		static Window* Get() { return _wndthis; }

		// Создать окно
		bool Create(const DescWindow& desc);

		// Обработка событий окна
		void RunEvent();

		// Закрыть окно.
		void Close();

		void SetInputManager(InputManager* inputmgr);

		HWND GetHWND() const { return _hwnd; }
		int GetLeft() const { return _desc.posx; }
		int GetTop() const { return _desc.posy; }
		int GetWidth() const { return _desc.width; }
		int GetHeight() const { return _desc.height; }
		// Вернуть заголовок окна
		const std::wstring& GetCaption() const { return _desc.caption; }

		// сообщает, было ли сообщение о выходе
		bool IsExit() const { return _isExit; }
		// сообщает об активности окна
		bool IsActive() const { return _active; }
		// сообщает об изменении окна
		// предупреждение: после вызова оповещает окно об обработке события
		bool IsResize()
		{
			bool ret = _isResize;
			_isResize = false;
			return ret;
		}

		// обработка событий
		LRESULT WndProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
	private:
		void _UpdateWindowState();

		static Window* _wndthis;

		DescWindow _desc;	// описание окна
		InputManager* _inputManager;
		HWND _hwnd;		// дескриптор окна	
		bool _isExit;		// флаг сообщающий о событии выхода	
		bool _active;		// окно активно?
		bool _minimized;
		bool _maximized;
		bool _isResize;	// если окно изменило размер
	};

	// обработка событий
	static LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam);

	//------------------------------------------------------------------
}


