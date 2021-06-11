#include "MyRender.h"

int main()
{
	Framework framework;

	MyRender *render = new MyRender();

	FrameworkDesc desc;
	desc.render = render;

	framework.Init(desc);
	framework.Run();
	framework.Close();

	return 0;
}