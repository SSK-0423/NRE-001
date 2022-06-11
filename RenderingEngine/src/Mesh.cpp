#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

MYRESULT Mesh::LoadMesh(const char* meshPath)
{
    // FBX���[�_�[�̃C���X�^���X����
    FBXLoader& loader = FBXLoader::Instance();

    // ������
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
