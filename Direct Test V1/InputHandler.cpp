#include "InputHandler.h"


bool InputHandler::KeyPressed(const KeyEvent& arg)
{
	printf("key press %c\n", arg.wc);
	switch(arg.code)
	{
	case KEY_Q:
		render->SetTurnLeftCam(true);
		break;
	case KEY_E:
		render->SetTurnRightCam(true);
		break;
	case KEY_W:
		render->SetMoveForwardCam(true);
		break;
	case KEY_S:
		render->SetMoveBackCam(true);
		break;
	case KEY_D:
		render->SetMoveRightCam(true);
		break;
	case KEY_A:
		render->SetMoveLeftCam(true);
		break;
	case KEY_SHIFT:
		render->SetMoveDownCam(true);
		break;
	case KEY_SPACE:
		render->SetMoveUpCam(true);
		break;
	}
	return true;
}

bool InputHandler::KeyReleased(const KeyEvent& arg)
{
	switch (arg.code)
	{
	case KEY_Q:
		render->SetTurnLeftCam(false);
		break;
	case KEY_E:
		render->SetTurnRightCam(false);
		break;
	case KEY_W:
		render->SetMoveForwardCam(false);
		break;
	case KEY_S:
		render->SetMoveBackCam(false);
		break;
	case KEY_D:
		render->SetMoveRightCam(false);
		break;
	case KEY_A:
		render->SetMoveLeftCam(false);
		break;
	case KEY_SHIFT:
		render->SetMoveDownCam(false);
		break;
	case KEY_SPACE:
		render->SetMoveUpCam(false);
		break;
	}
	return true;
}

bool InputHandler::MouseMove(const MouseEvent& arg)
	{
		printf("mouse %d - %d\n", arg.x, arg.y);
		return false;
	}
