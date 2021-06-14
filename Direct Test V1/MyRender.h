#pragma once

#include "AsimpMesh.h"
#include "Camera.h"
#include "D3D11_Framework.h"
#include "Frustum.h"
#include "Model.h"
#include "ModelList.h"
#include "Timer.h"

using namespace D3D11Framework;

class MyRender : public Render
{
public:
	MyRender();
	bool Init();
	bool Draw();
	void Close();

	void SetMoveLeftCam(bool moveLeftCam)
	{
		this->moveLeftCam = moveLeftCam;
	}

	void SetMoveRightCam(bool moveRightCam)
	{
		this->moveRightCam = moveRightCam;
	}

	void SetMoveForwardCam(bool moveForwardCam)
	{
		this->moveForwardCam = moveForwardCam;
	}

	void SetMoveBackCam(bool moveBackCam)
	{
		this->moveBackCam = moveBackCam;
	}

	void SetMoveUpCam(bool moveUpCam)
	{
		this->moveUpCam = moveUpCam;
	}

	void SetMoveDownCam(bool moveDownCam)
	{
		this->moveDownCam = moveDownCam;
	}

	void SetTurnLeftCam(bool turnLeftCam)
	{
		this->turnLeftCam = turnLeftCam;
	}

	void SetTurnRightCam(bool turnRightCam)
	{
		this->turnRightCam = turnRightCam;
	}

	Camera* GetCam()
	{
		return &cam;
	}

private:
	ModelList	modelList;
	Camera		cam;
	Frustum		frustum;
	Timer		timer;

	Model* model;
	Model* player;
	StaticMesh* mesh;

	BitmapFont* font;
	Text*		text;

	bool		moveLeftCam;
	bool		moveRightCam;
	bool		moveForwardCam;
	bool		moveBackCam;
	bool		moveUpCam;
	bool		moveDownCam;
	bool		turnLeftCam;
	bool		turnRightCam;
	
	void handleCamMove();
};