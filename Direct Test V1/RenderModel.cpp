#include "RenderModel.h"

RenderModel::RenderModel()
{
	_mesh = nullptr;
}

bool RenderModel::Init(HWND hwnd)
{
	XMVECTOR Eye = XMVectorSet(0.0f, 0.0f, -2.8f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	_view = XMMatrixLookAtLH(Eye, At, Up);
	
	_projection = XMMatrixPerspectiveFovLH(0.4f * 3.14f,
		(float)640 / 480, 0.1f, 1000.0f);

	_mesh = new StaticMesh();
	if (!_mesh->Init(this, L"mesh.ms3d")) return false;
}

bool RenderModel::Draw()
{
	_mesh->Render();
	return true;
}

void RenderModel::Close()
{
	_CLOSE(_mesh);
}


