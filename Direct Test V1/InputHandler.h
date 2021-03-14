#pragma once
#include <iostream>

#include "D3D11_Framework.h"

using namespace D3D11_Framework;

class InputHandler : public InputListener
{
public:
	bool MouseMove(const MouseEvent& arg) override;
	bool KeyPressed(const KeyEvent& arg) override;
};