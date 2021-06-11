#pragma once

#include "Camera.h"
#include "D3D11_Framework.h"
#include "Frustum.h"
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

	void setLeftCam(bool l)
	{
		leftcam = l;
	}

	void setRightCam(bool l)
	{
		rightcam = l;
	}

private:
	ModelList	modelList;
	Camera		cam;
	Frustum		frustum;
	Timer		timer;

	StaticMesh* mesh;

	BitmapFont* font;
	Text*		text;

	bool		leftcam;
	bool		rightcam;
};