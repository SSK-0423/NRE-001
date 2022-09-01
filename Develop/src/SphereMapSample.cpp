#include "SphereMapSample.h"

MYRESULT SphereMapSample::Init(Dx12GraphicsEngine& graphicsEngine, AppWindow& appWindow)
{
	SphereGeometryData data;
	MYRESULT result = sphere.Create(graphicsEngine.Device(), data);
	if (result == MYRESULT::FAILED) {
		MessageBox(NULL, L"�X�t�B�A�̐����Ɏ��s���܂����B", L"�G���[���b�Z�[�W", MB_OK);
		return result;
	}
	return MYRESULT::SUCCESS;
}

void SphereMapSample::Update(float deltaTime)
{
}

void SphereMapSample::Draw(Dx12GraphicsEngine& graphicsEngine)
{
	RenderingContext& renderContext = graphicsEngine.GetRenderingContext();
	
	sphere.Draw(renderContext);
}

void SphereMapSample::Final()
{
}
