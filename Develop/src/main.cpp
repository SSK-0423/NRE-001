#include "Dx12Application.h"
#include "EngineUtility.h"

#include "DrawPolygonSample.h"
#include "TextureMappingSample.h"
#include "MultiPassRenderingSample.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// ポリゴン描画サンプル
	DrawPolygonSample drawPolygonApp;

	// テクスチャマッピングサンプル
	TextureMappingSample texMapApp;

	// マルチパスレンダリングサンプル
	MultiPassRendering multiPassApp;

	Dx12Application app(multiPassApp);
	if (app.Init() == MYRESULT::FAILED)
	{
		return -1;
	}
	app.Run();
	app.End();
	return 0;
}