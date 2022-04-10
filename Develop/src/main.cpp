#include "Dx12Application.h"
#include "EngineUtility.h"

#include "DrawPolygonSample.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	DrawPolygonSample drawPolygonApp;
	Dx12Application app(drawPolygonApp);
	if (app.Init() == MYRESULT::FAILED)
	{
		return -1;
	}
	app.Run();
	app.End();
	return 0;
}