#pragma once

#include "MyRender.h"
#include "Util.h"


MyRender::MyRender(): frustum(), timer(), player(nullptr),
                      moveForwardCam(false), moveBackCam(false),
                      moveUpCam(false), moveDownCam(false),
                      turnLeftCam(false), turnRightCam(false)
{
	mesh = nullptr;
	model = nullptr;
	font = nullptr;
	text = nullptr;
	moveLeftCam = moveRightCam = false;
}

bool MyRender::Init()
{
	mesh = new MeshMS3D(this);
	if (!mesh->Init(w("sphere.ms3d")))
		return false;

	model = new Model(this);
	model->Init(("labyrinth1.obj"));

	player = new Model(this);
	player->Init("grim.obj");
	
	
	font = new BitmapFont(this);
	if(!font->Init("font.fnt"))
		return false;

	text = new Text(this, font);
	if(!text->Init(L"", false, 20))
		return false;

	if(!modelList.Init(300))
		return false;

	if(!timer.Init())
		return false;

	cam.SetPosition(0.0f, 0.0f, -35.0f);
	return true;
}

void MyRender::handleCamMove()
{
	cam.TurnLeft(turnLeftCam);
	cam.TurnRight(turnRightCam);
	cam.MoveDown(moveDownCam);
	cam.MoveUp(moveUpCam);
	cam.MoveBack(moveBackCam);
	cam.MoveForward(moveForwardCam);
	cam.MoveLeft(moveLeftCam);
	cam.MoveRight(moveRightCam);
}

bool MyRender::Draw()
{
	timer.Frame();
	handleCamMove();
	cam.Render(timer.GetTime());

	XMMATRIX viewMatrix = cam.GetViewMatrix();
	frustum.ConstructFrustum(1000, m_Projection, viewMatrix);

	int modelCount = modelList.GetModelCount();
	int renderCount = 0;

	for (int i = 0; i < modelCount; i++)
	{
		float x, y, z;
		modelList.GetData(i, x, y, z);
		bool renderModel = frustum.CheckSphere(x, y, z, 1);
		if(renderModel)
		{
			mesh->Identity();
			mesh->Translate(x, y, z);
			mesh->Draw(viewMatrix);

			renderCount++;
		}
	}
	model->Identity();
	model->Scale(30, 30, 30);
	model->Draw(viewMatrix);

	player->Identity();
	player->Scale(0.05, 0.05, 0.05);
	player->Draw(viewMatrix);
	

	TurnZBufferOff();
	TurnOnAlphaBlending();

	std::wstring t = L"Сфер на экране: " + intToStr(renderCount);
	text->SetText(t);
	text->Draw(1.0f, 1.0f, 1.0f, 20.0f, 20.0f);

	

	TurnZBufferOn();
	TurnOffAlphaBlending();

	return true;
}

void MyRender::Close()
{
	_CLOSE(font);
	_CLOSE(text);
	_CLOSE(mesh);
	modelList.Close();
}

