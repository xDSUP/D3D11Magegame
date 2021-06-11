#include "InputHandler.h"


bool InputHandler::KeyPressed(const KeyEvent& arg)
{
	printf("key press %c\n", arg.wc);
	if (arg.code == KEY_LEFT)
		render->setLeftCam(true);
	else if (arg.code == KEY_RIGHT)
		render->setRightCam(true);
	return true;
}

bool InputHandler::KeyReleased(const KeyEvent& arg)
{
	if (arg.code == KEY_LEFT)
		render->setLeftCam(false);
	else if (arg.code == KEY_RIGHT)
		render->setRightCam(false);
	return true;
}

bool InputHandler::MouseMove(const MouseEvent& arg)
	{
		printf("mouse %d - %d\n", arg.x, arg.y);
		return false;
	}
