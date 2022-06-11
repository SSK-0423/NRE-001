#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

MYRESULT Mesh::LoadMesh(const char* meshPath)
{
    // FBXローダーのインスタンス生成
    FBXLoader& loader = FBXLoader::Instance();

    // 初期化
    MYRESULT result = loader.Init(meshPath);

    if (result == MYRESULT::FAILED)
    {
        return MYRESULT::FAILED;
    }

    return MYRESULT::SUCCESS;
}

void Mesh::Draw(RenderingContext& renderContext)
{
}
