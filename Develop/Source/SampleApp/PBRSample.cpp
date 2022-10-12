#include "PBRSample.h"
#include "ShaderLibrary.h"

using namespace NamelessEngine::Component;
using namespace NamelessEngine::DX12API;
using namespace NamelessEngine::Core;

NamelessEngine::Utility::RESULT PBRSample::Init(NamelessEngine::Core::Dx12GraphicsEngine& graphicsEngine, NamelessEngine::Core::AppWindow& window)
{
    ShaderLibrary& library = ShaderLibrary::Instance();
    Shader* shader = library.GetShader("GeometryVS");

    sphere.AddComponent<Transform>();
    sphere.AddComponent<MeshRenderer>();

    skyBox.AddComponent<Transform>();
    skyBox.AddComponent<MeshRenderer>();

    return NamelessEngine::Utility::RESULT::SUCCESS;
}

void PBRSample::Update(float deltaTime)
{
    sphere.Update(deltaTime);
    skyBox.Update(deltaTime);
}

void PBRSample::Draw(NamelessEngine::Core::Dx12GraphicsEngine& graphicsEngine)
{
    sphere.Draw();
    skyBox.Draw();
}

void PBRSample::Final()
{
}
