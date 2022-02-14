#include "Dx12Application.h"
#include "EngineUtility.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Dx12Application app;
	if (app.Init() == MYRESULT::FAILED)
	{
		return -1;
	}
	app.Run();
	app.End();
	return 0;
}