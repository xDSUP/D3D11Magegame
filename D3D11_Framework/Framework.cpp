#include "stdafx.h"
#include "Framework.h"
#include "macros.h"
#include "Log.h"


namespace D3D11_Framework
{
	//------------------------------------------------------------------

	
	//------------------------------------------------------------------
	Framework::Framework():
		_wnd(nullptr),
		_render(nullptr),
		_input(nullptr),
		_init(false)
	{
	}

	Framework::~Framework()
	{
	}

	bool Framework::Init()
	{
		_wnd = new Window();
		_input = new InputManager();
		if(!_wnd || !_input)
		{
			sLog->Err("Ќе удалось выдерилть пам€ть");
			return false;
		}
		_input->Init();

		DescWindow desc;
		if (!_wnd->Create(desc))
		{
			sLog->Err("Ќе удалось создать окно");
			return false;
		}
		_wnd->SetInputManager(_input);

		if (!_render->CreateDevice(_wnd->GetHWND()))
		{
			sLog->Err("Ќе удалось создать рендер");
			return false;
		}

		_init = true;
		return true;
	}

	void Framework::Run()
	{
		if (_init)
			while (_frame());
	}

	void Framework::Close()
	{
		_init = false;
		_render->Shutdown();
		_DELETE(_render);
		_CLOSE(_wnd);
		_CLOSE(_input);
	}

	void Framework::AddInputListener(InputListener* listener)
	{
		if (_input)
			_input->AddListener(listener);
	}

	bool Framework::_frame()
	{
		// обрабатываем событи€ окна
		_wnd->RunEvent();
		// если окно неактивно - завершаем кадр
		if (!_wnd->IsActive())
			return true;

		// если окно было закрыто, завершаем работу движка
		if (_wnd->IsExit())
			return false;

		// если окно изменило размер
		if (_wnd->IsResize())
		{
		}
		_render->BeginFrame();
		// если не отрисовалось, завершаем работу
		if (!_render->Draw())
			return false;
		_render->EndFrame();
		
		return true;
	}
}
