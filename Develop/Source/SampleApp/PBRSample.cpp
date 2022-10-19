#include "PBRSample.h"
#include "PBRScene.h"

using namespace NamelessEngine::Component;
using namespace NamelessEngine::DX12API;
using namespace NamelessEngine::Core;
using namespace NamelessEngine::Utility;

NamelessEngine::Utility::RESULT PBRSample::Init(NamelessEngine::Core::Dx12GraphicsEngine& graphicsEngine, NamelessEngine::Core::AppWindow& window)
{
    _scene = new PBRScene();
    RESULT result =  _scene->Init();
    return result;
}

void PBRSample::Update(float deltaTime)
{
    _scene->Update(deltaTime);
}

void PBRSample::Draw(NamelessEngine::Core::Dx12GraphicsEngine& graphicsEngine)
{
    _scene->Draw();
}

void PBRSample::Final()
{
}
