#include "Dx12GraphicsEngine.h"

Dx12GraphicsEngine::Dx12GraphicsEngine()
{
}

Dx12GraphicsEngine::~Dx12GraphicsEngine()
{
}

Dx12GraphicsEngine& Dx12GraphicsEngine::Instance()
{
	static Dx12GraphicsEngine inst;
	return inst;
}

MYRESULT Dx12GraphicsEngine::Init()
{
	MYRESULT result = MYRESULT::SUCCESS;
	
	return result;
}
