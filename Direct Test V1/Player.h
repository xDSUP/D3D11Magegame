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
		sLog->Debug("Xpos:%lf | Ypos:%lf | Zpos: %lf rad: %lf", pos.x, pos.y, pos.z, rot.y);
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

	void UpdatePosition()
	{
		TurnRight(turnRight);
		TurnLeft(turnLeft);
		MoveBack(moveBack);
		MoveForward(moveForward);
	}

private:
	Model* model;
	bool		moveForward;
	bool		moveBack;
	bool		turnLeft;
	bool		turnRight;
};

