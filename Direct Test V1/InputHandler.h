#pragma once

#include "D3D11_Framework.h"
#include "MyRender.h"

using namespace D3D11Framework;

class InputHandler : public InputListener
{
public:
	InputHandler(MyRender* render)
	{
		this->render = render;
	}
	
	bool MouseMove(const MouseEvent& arg) override;
	bool KeyPressed(const KeyEvent& arg) override;
	bool KeyReleased(const KeyEvent& arg) override;

private:
	MyRender* render;
};