#include "MyRender.h"

#include "Util.h"


MyRender::MyRender()
{
	mesh = nullptr;
	font = nullptr;
	text = nullptr;
	leftcam = rightcam = false;
}

bool MyRender::Init()
{
	mesh = new StaticMesh(this);
	if (!mesh->Init(w("sphere.ms3d")))
		return false;

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

bool MyRender::Draw()
{
	timer.Frame();
	cam.TurnLeft(leftcam);
	cam.TurnRight(rightcam);
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

	Log::Get()->Debug("Сфер на экране %d", renderCount);
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

