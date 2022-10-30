#include "PBRSample.h"
#include "PBRScene.h"
#include "PBRRenderer.h"

using namespace NamelessEngine::Component;
using namespace NamelessEngine::DX12API;
using namespace NamelessEngine::Core;
using namespace NamelessEngine::Utility;
using namespace NamelessEngine::Graphics;

PBRSample::PBRSample()
{
}

PBRSample::~PBRSample()
{
}

NamelessEngine::Utility::RESULT PBRSample::Init(
	NamelessEngine::Core::Dx12GraphicsEngine& graphicsEngine, NamelessEngine::Core::AppWindow& window)
{
	_renderer = std::make_unique<PBRRenderer>();
	RESULT result = _renderer->Init();
	_scene = std::make_unique<PBRScene>();
	result = _scene->Init();
	return result;
}

void PBRSample::Update(float deltaTime)
{
	_scene->Update(deltaTime);
	_renderer->Update(deltaTime);
}

void PBRSample::Draw(NamelessEngine::Core::Dx12GraphicsEngine& graphicsEngine)
{
	_renderer->Render(*_scene);
}

void PBRSample::Final()
{
}
