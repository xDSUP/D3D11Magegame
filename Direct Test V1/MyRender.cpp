#pragma once

#include "MyRender.h"
#include "Util.h"


MyRender::MyRender(): frustum(), timer(), player(nullptr),
                      moveForwardCam(false), moveBackCam(false),
                      moveUpCam(false), moveDownCam(false),
                      turnLeftCam(false), turnRightCam(false)
{
	mesh = nullptr;
	labirint = nullptr;
	font = nullptr;
	text = nullptr;
	moveLeftCam = moveRightCam = false;
}

bool MyRender::Init()
{
	mesh = new MeshMS3D(this);
	if (!mesh->Init(w("sphere.ms3d")))
		return false;

	labirint = new Model(this);
	labirint->Init(("maze.obj"));

	player = new Player();
	player->InitModel(this, "grim.obj");
	player->SetSpeedTurn(2);
	player->SetSpeedMove(1);
	player->SetMaxFrameTime(1);
	player->SetPosition(0, 0.2, 0);
	
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

	cam.SetPosition(0.0f, 14.0f, -14.0f);
	cam.SetRotation(1, 0, 0);

	DirectionalLight m_DirLight;
	m_DirLight.ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	m_DirLight.diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_DirLight.specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_DirLight.direction = XMFLOAT3(-0.577f, -0.577f, 0.577f);
	m_DirectionalLights.push_back(m_DirLight);

	PointLight m_PointLight;
	m_PointLight.position = XMFLOAT3(0.0f, 0.0f, -10.0f);
	m_PointLight.ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	m_PointLight.diffuse = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	m_PointLight.specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_PointLight.att = XMFLOAT3(0.0f, 0.1f, 0.0f);
	m_PointLight.range = 25.0f;
	m_PointLights.push_back(m_PointLight);

	SpotLight m_SpotLight;
	m_SpotLight.position = XMFLOAT3(0.0f, 0.0f, -5.0f);
	m_SpotLight.direction = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_SpotLight.ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_SpotLight.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_SpotLight.specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_SpotLight.att = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_SpotLight.spot = 12.0f;
	m_SpotLight.range = 10000.0f;
	m_SpotLights.push_back(m_SpotLight);
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
	labirint->Identity();
	labirint->Scale(2, 2, 2);
	labirint->Draw(viewMatrix);

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

Player* MyRender::GetPlayer()
{
	return player;
}

