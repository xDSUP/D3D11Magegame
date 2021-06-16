#pragma once
#include "Model.h"
#include "MovableGameObject.h"

class FireBall :
    public MovableGameObject
{
public:
	void Init()
	{
		
	}
	
	void Draw(CXMMATRIX view) override
	{
		model->Translate(pos.x, pos.y, pos.z);
		model->Draw(view);
	}

	void UpdatePos(float frameTime)
	{
		lifeTime -= frameTime;
		this->frameTime = frameTime;
		MoveForward(true);
	}
	
	StaticMesh* model;
	float lifeTime;
private:
	
};

