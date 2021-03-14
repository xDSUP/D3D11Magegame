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
			sLog->Err("�� ������� ��������� ������");
			return false;
		}
		_input->Init();

		DescWindow desc;
		if (!_wnd->Create(desc))
		{
			sLog->Err("�� ������� ������� ����");
			return false;
		}
		_wnd->SetInputManager(_input);

		if (!_render->CreateDevice(_wnd->GetHWND()))
		{
			sLog->Err("�� ������� ������� ������");
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
		// ������������ ������� ����
		_wnd->RunEvent();
		// ���� ���� ��������� - ��������� ����
		if (!_wnd->IsActive())
			return true;

		// ���� ���� ���� �������, ��������� ������ ������
		if (_wnd->IsExit())
			return false;

		// ���� ���� �������� ������
		if (_wnd->IsResize())
		{
		}
		_render->BeginFrame();
		// ���� �� ������������, ��������� ������
		if (!_render->Draw())
			return false;
		_render->EndFrame();
		
		return true;
	}
}
