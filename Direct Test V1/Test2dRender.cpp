#include "Test2dRender.h"


Test2dRender::Test2dRender(): _mesh(nullptr)
{
}

bool Test2dRender::Init(HWND hwnd)
{
	XMVECTOR Eye = XMVectorSet(0.0f, 0.0f, -2.8f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	_view = XMMatrixLookAtLH(Eye, At, Up);

	_mesh = new StaticMesh();
	if (!_mesh->Init(this, L"mesh.ms3d"))
		return false;

	return true;
}

bool Test2dRender::Draw()
{
	static float rot = 0.0f;
	rot += 0.0000005f;
	if (rot > 6.26f)
		rot = 0.0f;

	_mesh->Identity();
	_mesh->Rotate(rot, 0.0, 1.0, 0.0);
	//_mesh->Translate(-1.5, 0.0, 0.0);
	_mesh->Draw(_view);

	//_mesh->Identity();
	//_mesh->Rotate(rot, 0.0, 1.0, 0.0);
	//_mesh->Translate(1.5, 0.0, 0.0);
	//_mesh->Draw(_view);
	return true;
}

void Test2dRender::Close()
{
	_CLOSE(_mesh);
}
