#include "Dx12Application.h"
#include "Dx12GraphicsEngine.h"

Dx12Application::Dx12Application()
{
	_graphicsEngine = new Dx12GraphicsEngine();
}

Dx12Application::~Dx12Application()
{
}

MYRESULT Dx12Application::Init()
{
	return MYRESULT::SUCCESS;
}

void Dx12Application::Run()
{
	// �Q�[�����[�v
}

void Dx12Application::End()
{
}