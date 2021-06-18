#pragma once
#include "Model.h"
#include "MovableGameObject.h"
#include "Util.h"

class FireBall :
    public MovableGameObject
{
public:
	void Init()
	{
		light.position = XMFLOAT3(0.0f, 0.0f, 0.0f);
		light.ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		light.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		light.specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		light.att = XMFLOAT3(0.1f, 0.6f, 1.0f);
		//m_PointLight.color = XMFLOAT3(0.5f, 0.1f, 0.0f);
		light.range = 10.0f;
	}
	
	void Draw(CXMMATRIX view) override
	{
		model->Draw(view);
	}

	void UpdatePos(float frameTime)
	{
		lifeTime -= frameTime;
		this->frameTime = frameTime;
		MoveForward(true);
		model->Identity();
		float rad = convertDegreeToRad(rot.y);
		if (abs(forwardMoveSpeed) >= 0.000000000001)
		{
			pos.x += (forwardMoveSpeed)*sinf(rad);
			pos.z += (forwardMoveSpeed)*cosf(rad);
		}
		light.position = pos;
		model->Translate(pos.x, pos.y, pos.z);
	}

	PointLight	light;
	StaticMesh* model;
	float lifeTime;
private:
	
};

