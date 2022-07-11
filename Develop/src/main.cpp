#include "Dx12Application.h"
#include "EngineUtility.h"

#include "DrawPolygonSample.h"
#include "TextureMappingSample.h"
#include "MultiPassRenderingSample.h"
#include "MultiRenderTargetSample.h"
#include "DepthBufferSample.h"
#include "DrawMeshSample.h"
#include "PBRSample.h"

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

	// PBRサンプル
	PBRSample pbrApp;

	Dx12Application app(texMapApp);
	if (app.Init() == MYRESULT::FAILED)
	{
		MessageBox(NULL, L"アプリケーションの初期化に失敗しました。", L"エラーメッセージ", MB_OK);
		return -1;
	}
	app.Run();
	app.End();
	return 0;
}