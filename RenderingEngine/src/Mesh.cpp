#include "Mesh.h"
#include "IMeshLoader.h"
Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

MYRESULT Mesh::LoadMesh(const char* meshPath, ID3D12Device& device)
{
    // ���[�_�[�p��
    IMeshLoader* loader = new FBXLoader();

    // FBX�ǂݍ���

    if (!loader->Load(meshPath))
    {
        return MYRESULT::FAILED;
    }
    
    // ���_�o�b�t�@�\����
    MYRESULT result = CreateVertexBuffers(device);
    if (result == MYRESULT::FAILED) { return result; }

    // �C���f�b�N�X�o�b�t�@�[����
    result = CreateIndexBuffers(device);
    if (result == MYRESULT::FAILED) { return result; }

    return MYRESULT::SUCCESS;
}

void Mesh::Draw(RenderingContext& renderContext)
{
}

MYRESULT Mesh::CreateVertexBuffers(ID3D12Device& device)
{
    return MYRESULT::SUCCESS;
}

MYRESULT Mesh::CreateIndexBuffers(ID3D12Device& device)
{
    return MYRESULT::SUCCESS;
}

//
// ������
// 1. Mesh���Ńo�b�t�@�[����
// �����b�g
// �EFBXLoader��VertexBuffer/IndexBuffer�N���X�Ɉˑ����Ȃ��Ȃ�̂ŁA�Ɨ����������ĐF�X�Ȏ����ɑΉ��ł���
// �f�����b�g
// �EMesh����FBXLoader�ւ̈ˑ��������Ȃ�
// �E���`���̃t�@�C����ǂݍ��ލۂɑΉ��ł��Ȃ�
// 2. FBXLoader���Ńo�b�t�@�[����
// �����b�g
// �E���b�V�����̏������ȑf�ɂȂ�
// �E���`����ǂݍ��ރ��[�_�[�N���X�ւ̐؂�ւ����e��
// �f�����b�g
// �EVertexBuffer/IndexBuffer�N���X�Ɉˑ�����`�ɂȂ�
// �E�Ɨ��������Ȃ��A���i�Ƃ��Ă̎g���܂킵�����Â炭�Ȃ�
//