#pragma once
#include "D3D11_Framework.h"

#define SimpleXMVector(r) XMVectorSet(r, r, r, 0);

using namespace D3D11Framework;
struct Particle
{
    XMFLOAT4 Position, Velocity;
    XMFLOAT4 Color;
    float Life;
};

struct ParticleVertex
{
	XMFLOAT3 pos;
	XMFLOAT2 tex;
};

struct ParticleConstBuf
{
	XMMATRIX WVP;
	XMFLOAT4 color;
	XMFLOAT4 offset;
};


class ParticleGenerator
{

public:

	ParticleGenerator(Render* render)
	{
		this->render = render;
	}

	bool Init(wchar_t* name, int amount, XMFLOAT3 velosity);
	void Draw(CXMMATRIX viewmatrix);
	void Close();
	
	void Update(float frameTime, XMFLOAT3 GameObjectPos, unsigned int newParticles, XMFLOAT3 offset = XMFLOAT3(0,0,0));

	Shader* shader;
	Render* render;
	StaticMesh* model;
	int amount;
	XMFLOAT3 velosity;
	unsigned int VAO;
	std::vector<Particle> particles;

private:
	int FirstUnusedParticle();
	void RespawnParticle(Particle& particle, XMFLOAT3 objectPos, XMFLOAT3 offset);
	void m_RenderBuffers();
	void m_SetShaderParameters(CXMMATRIX WVP, XMFLOAT4 color);
	void m_RenderShader();
	bool m_InitBuffers();

	
	int lastUsedParticle;
	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;
	ID3D11Buffer* m_constantBuffer;
};

