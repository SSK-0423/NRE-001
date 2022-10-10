#pragma once
#include "Dx12ApplicationImpl.h"

class PBRSample : public Dx12ApplicationImpl
{
public:
	NamelessEngine::Utility::RESULT Init(
		NamelessEngine::Core::Dx12GraphicsEngine& graphicsEngine, 
		NamelessEngine::Core::AppWindow& window);
	void Update(float deltaTime);
	void Draw(NamelessEngine::Core::Dx12GraphicsEngine& graphicsEngine);
	void Final();

private:
};