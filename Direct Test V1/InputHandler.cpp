#include "InputHandler.h"


	bool InputHandler::KeyPressed(const KeyEvent& arg)
	{
		printf("key press %c\n", arg.wc);
		return false;
	}
	bool InputHandler::MouseMove(const MouseEvent& arg)
	{
		printf("mouse %d - %d\n", arg.x, arg.y);
		return false;
	}
