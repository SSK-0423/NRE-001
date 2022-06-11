#include "DrawMeshSample.h"
#include <fbxsdk.h>

MYRESULT DrawMeshSample::Init(Dx12GraphicsEngine& graphicsEngine, AppWindow& window)
{
    fbxsdk::FbxManager* fbx_manager = fbxsdk::FbxManager::Create();

    fbx_manager->Destroy();

    return MYRESULT::FAILED;
}

void DrawMeshSample::Update(float deltaTime)
{
}

void DrawMeshSample::Draw(Dx12GraphicsEngine& graphicsEngine)
{
}

void DrawMeshSample::Final()
{
}
