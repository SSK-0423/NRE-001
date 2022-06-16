#include "DrawMeshSample.h"

MYRESULT DrawMeshSample::Init(Dx12GraphicsEngine& graphicsEngine, AppWindow& window)
{
	// ���b�V���ǂݍ���
	MYRESULT result = mesh.LoadMesh(
		"res/stanford-bunny-fbx/stanford-bunny.fbx", graphicsEngine.Device());

	if (result == MYRESULT::FAILED) {
		return MYRESULT::FAILED;
	}
	return MYRESULT::SUCCESS;
}

void DrawMeshSample::Update(float deltaTime)
{
}

void DrawMeshSample::Draw(Dx12GraphicsEngine& graphicsEngine)
{
	// �����_�����O�R���e�L�X�g�擾
	RenderingContext& renderContext = graphicsEngine.GetRenderingContext();

}

void DrawMeshSample::Final()
{
}
