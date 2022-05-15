#include "DepthBufferSample.h"
#include "Dx12GraphicsEngine.h"

MYRESULT DepthBufferSample::Init(Dx12GraphicsEngine& graphicsEngine, AppWindow& window)
{
    MYRESULT result = _dsvHeap.Create(graphicsEngine.Device());

    return result;
}

void DepthBufferSample::Update(float deltaTime)
{
}

void DepthBufferSample::Draw(Dx12GraphicsEngine& graphicsEngine)
{
}

void DepthBufferSample::Final()
{
}
