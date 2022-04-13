#include "Dx12Application.h"
#include "EngineUtility.h"

#include "DrawPolygonSample.h"
#include "TextureMappingSample.h"
#include "MultiPassRenderingSample.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// �|���S���`��T���v��
	DrawPolygonSample drawPolygonApp;

	// �e�N�X�`���}�b�s���O�T���v��
	TextureMappingSample texMapApp;

	// �}���`�p�X�����_�����O�T���v��
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