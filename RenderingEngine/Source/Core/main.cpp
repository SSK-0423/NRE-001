#include "../NamelessEngine.h"
#include "Dx12Application.h"
#include "PBRSample.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	PBRSample pbrApp;
	Dx12Application app(pbrApp);
	if (app.Init() == NamelessEngine::Utility::RESULT::FAILED)
	{
		MessageBox(NULL, L"�A�v���P�[�V�����̏������Ɏ��s���܂����B", L"�G���[���b�Z�[�W", MB_OK);
		return -1;
	}
	app.Run();
	app.End();
	return 0;
}