#include "DrawMeshSample.h"

MYRESULT DrawMeshSample::Init(Dx12GraphicsEngine& graphicsEngine, AppWindow& window)
{
    MYRESULT result = mesh.LoadMesh("");
    
    if (result == MYRESULT::FAILED) {
        return MYRESULT::FAILED;
    }
    return MYRESULT::SUCCESS;
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
