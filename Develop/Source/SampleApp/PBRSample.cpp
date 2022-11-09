#include "PBRSample.h"
#include "PBRScene.h"
#include "PBRRenderer.h"

#include "imgui.h"

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
	// シーン初期化
	_scene = std::make_unique<PBRScene>();
	RESULT result = _scene->Init();
	if (result == RESULT::FAILED) { return result; }

	// レンダラー初期化
	_renderer = std::make_unique<PBRRenderer>();
	result = _renderer->Init(*_scene);
	return result;
}

void PBRSample::Update(float deltaTime)
{
	_scene->Update(deltaTime);
	_renderer->Update(deltaTime, *_scene);
}

void PBRSample::Draw(NamelessEngine::Core::Dx12GraphicsEngine& graphicsEngine)
{
	_renderer->Render(*_scene);
}

void PBRSample::Final()
{
}
