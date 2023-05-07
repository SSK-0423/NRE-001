#pragma once
#include "Dx12ApplicationImpl.h"
#include <memory>

class PBRSample : public Dx12ApplicationImpl
{
public:
	PBRSample();
	~PBRSample();

	NamelessEngine::Utility::RESULT Init(
		NamelessEngine::Core::Dx12GraphicsEngine& graphicsEngine, 
		NamelessEngine::Core::AppWindow& window);
	void Update(float deltaTime);
	void Draw(NamelessEngine::Core::Dx12GraphicsEngine& graphicsEngine);
	void Final();

private:
	std::unique_ptr<NamelessEngine::Scene::Scene> _scene = nullptr;
	std::unique_ptr<NamelessEngine::Graphics::IRenderer> _renderer = nullptr;
};