#include "../NamelessEngine.h"
#include "Dx12Application.h"
#include "PBRSample.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	PBRSample pbrApp;
	Dx12Application app(pbrApp);
	if (app.Init() == NamelessEngine::Utility::RESULT::FAILED)
	{
		MessageBox(NULL, L"アプリケーションの初期化に失敗しました。", L"エラーメッセージ", MB_OK);
		return -1;
	}
	app.Run();
	app.End();
	return 0;
}