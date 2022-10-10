#include "PBRSample.h"
#include "ShaderLibrary.h"

using namespace NamelessEngine::DX12API;
using namespace NamelessEngine::Core;

NamelessEngine::Utility::RESULT PBRSample::Init(NamelessEngine::Core::Dx12GraphicsEngine& graphicsEngine, NamelessEngine::Core::AppWindow& window)
{
    ShaderLibrary& library = ShaderLibrary::Instance();
    Shader* shader = library.GetShader("GeometryVS");
    return NamelessEngine::Utility::RESULT::SUCCESS;
}

void PBRSample::Update(float deltaTime)
{
}

void PBRSample::Draw(NamelessEngine::Core::Dx12GraphicsEngine& graphicsEngine)
{
}

void PBRSample::Final()
{
}
