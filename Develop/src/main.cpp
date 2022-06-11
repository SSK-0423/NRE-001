#include "Dx12Application.h"
#include "EngineUtility.h"

#include "DrawPolygonSample.h"
#include "TextureMappingSample.h"
#include "MultiPassRenderingSample.h"
#include "MultiRenderTargetSample.h"
#include "DepthBufferSample.h"
#include "DrawMeshSample.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// ポリゴン描画サンプル
	DrawPolygonSample drawPolygonApp;

	// テクスチャマッピングサンプル
	TextureMappingSample texMapApp;

	// マルチパスレンダリングサンプル
	MultiPassRendering multiPassApp;

	// マルチレンダーターゲットサンプル
	MultiRenderTargetSample multiRenderApp;

	// 深度バッファーサンプル
	DepthBufferSample depthBufferApp;

	// メッシュ描画サンプル
	DrawMeshSample drawMeshApp;

	Dx12Application app(drawMeshApp);
	if (app.Init() == MYRESULT::FAILED)
	{
		return -1;
	}
	app.Run();
	app.End();
	return 0;
}