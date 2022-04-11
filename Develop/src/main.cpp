#include "Dx12Application.h"
#include "EngineUtility.h"

#include "DrawPolygonSample.h"
#include "TextureMappingSample.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// �|���S���`��T���v��
	DrawPolygonSample drawPolygonApp;

	// �e�N�X�`���}�b�s���O�T���v��
	TextureMappingSample texMapApp;



	Dx12Application app(texMapApp);
	if (app.Init() == MYRESULT::FAILED)
	{
		return -1;
	}
	app.Run();
	app.End();
	return 0;
}