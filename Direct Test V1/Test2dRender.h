#pragma once
#include "D3D11_Framework.h"

using namespace D3D11_Framework;

class Test2dRender
	: public Render
{
public:
	Test2dRender();
	bool Init(HWND hwnd) override;
	bool Draw() override;
	void Close() override;
private:
	StaticMesh* _mesh;
	XMMATRIX _view;
};

