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
	// �|���S���`��T���v��
	DrawPolygonSample drawPolygonApp;

	// �e�N�X�`���}�b�s���O�T���v��
	TextureMappingSample texMapApp;

	// �}���`�p�X�����_�����O�T���v��
	MultiPassRendering multiPassApp;

	// �}���`�����_�[�^�[�Q�b�g�T���v��
	MultiRenderTargetSample multiRenderApp;

	// �[�x�o�b�t�@�[�T���v��
	DepthBufferSample depthBufferApp;

	// ���b�V���`��T���v��
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