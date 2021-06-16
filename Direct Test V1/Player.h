#pragma once
#include "MovableGameObject.h"
#include "Model.h"
#include "Util.h"

class Player : public MovableGameObject
{
public:
	explicit Player()
		:   model(nullptr), moveForward(false), moveBack(false),
			turnLeft(false), turnRight(false)
	{
		
	}

	

	bool InitModel(Render* render, const char* path)
	{
		model = new Model(render);
		this->render = render;
		
		torchLight.position = XMFLOAT3(0.0f, 0.0f, -10.0f);
		torchLight.ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
		torchLight.diffuse = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
		torchLight.specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
		torchLight.att = XMFLOAT3(0.0f, 0.1f, 0.0f);
		//m_PointLight.color = XMFLOAT3(0.5f, 0.1f, 0.0f);
		torchLight.range = 10.0f;
		
		return model->Init(path);
	}

	Model* GetModel()
	{
		return model;
	}

	void Draw(CXMMATRIX view) override
	{
		model->Identity();
		UpdatePosition();
		model->Scale(0.05, 0.05, 0.05);

		float rad = convertDegreeToRad(rot.y);
		model->Rotate(rad, 0, 1, 0);
		if(abs(forwardMoveSpeed - backMoveSpeed) >= 0.000000000001)
		{
			pos.x += (forwardMoveSpeed - backMoveSpeed) * sinf(rad);
			pos.z += (forwardMoveSpeed - backMoveSpeed) * cosf(rad);
		}
		model->Translate(pos.x , pos.y, pos.z);
		torchLight.position.x = pos.x + 1;
		torchLight.position.y = pos.y + 3;
		torchLight.position.z = pos.z + 1;
		
		//sLog->Debug("Xpos:%lf | Ypos:%lf | Zpos: %lf rad: %lf", pos.x, pos.y, pos.z, rot.y);
		render->GetPointLights()[0].position = this->GetTorchLight()->position;
		model->Draw(view);
	}


	void SetMoveForward(bool moveForward)
	{
		this->moveForward = moveForward;
	}

	void SetMoveBack(bool moveBack)
	{
		this->moveBack = moveBack;
	}

	void SetTurnLeft(bool turnLeft)
	{
		this->turnLeft = turnLeft;
	}

	void SetTurnRight(bool turnRight)
	{
		this->turnRight = turnRight;
	}

	PointLight* GetTorchLight()
	{
		return &torchLight;
	}
	
	void UpdatePosition()
	{
		TurnRight(turnRight);
		TurnLeft(turnLeft);
		MoveBack(moveBack);
		MoveForward(moveForward);
	}

private:
	PointLight	torchLight;
	Render* render;
	Model*		model;
	bool		moveForward;
	bool		moveBack;
	bool		turnLeft;
	bool		turnRight;
};

