#include "FBXLoader.h"
#include <Windows.h>
#include <cassert>

FBXLoader::FBXLoader()
	: fbxManager(nullptr), fbxImporter(nullptr), fbxScene(nullptr)
{
	// ��Ō������K�v����
	if (Init() == false) {
		MessageBox(NULL, L"FBX_SDK�̏������Ɏ��s���܂����B", L"�G���[���b�Z�[�W", MB_OK);
		return;
	}
}

FBXLoader::~FBXLoader()
{
	if (fbxImporter != nullptr) {
		fbxImporter->Destroy();
	}
	if (fbxScene != nullptr) {
		fbxScene->Destroy();
	}
	if (fbxManager != nullptr) {
		fbxManager->Destroy();
	}
}

bool FBXLoader::Init()
{
	// FbxManager����
	fbxManager = FbxManager::Create();
	if (fbxManager == nullptr) {
		return false;
	}

	// FbxImporter����
	fbxImporter = FbxImporter::Create(fbxManager, "Importer");
	if (fbxImporter == nullptr) {
		fbxManager->Destroy();
		return false;
	}

	// FbxScene����
	fbxScene = FbxScene::Create(fbxManager, "Scene");
	if (fbxScene == nullptr) {
		fbxImporter->Destroy();
		fbxManager->Destroy();
		return false;
	}

	return true;
}

FBXMeshData FBXLoader::CreateMesh(FbxMesh* mesh)
{
	FBXMeshData meshData;
	LoadVertices(meshData, mesh);
	LoadIndices(meshData, mesh);
	LoadNormals(meshData, mesh);

	return meshData;
}

void FBXLoader::LoadVertices(FBXMeshData& meshData, FbxMesh* mesh)
{
	// ���_�o�b�t�@�̎擾 
	FbxVector4* vertices = mesh->GetControlPoints();
	// �C���f�b�N�X�o�b�t�@�̎擾
	int* indices = mesh->GetPolygonVertices();
	// ���_���W�̐�(���_��)�̎擾
	unsigned long long polygonVertexCount = mesh->GetPolygonVertexCount();

	for (size_t idx = 0; idx < polygonVertexCount; idx++) {

		FBXMeshVertex vertex;

		// �C���f�b�N�X�o�b�t�@���璸�_�ԍ����擾
		int index = indices[idx];

		// ���_���W���X�g������W���擾����
		vertex.position.x = -vertices[index][0]; // FBX�͉E��n DirectX�͍���n�Ȃ̂�X���W�𔽓]
		vertex.position.y = vertices[index][1];
		vertex.position.z = vertices[index][2];

		// �ǉ�
		meshData.vertices.push_back(vertex);
	}
}

void FBXLoader::LoadIndices(FBXMeshData& meshData, FbxMesh* mesh)
{
	// �|���S�����̎擾
	int polygonCount = mesh->GetPolygonCount();

	// �|���S���̐������A�ԂƂ��ĕۑ�����
	for (unsigned int i = 0; i < polygonCount; i++) {
		// �C���f�b�N�X�o�b�t�@�̏��Ԃŕ���ł���̂ł��̂܂�
		// ����n�΍�
		meshData.indices.push_back(i * 3 + 2);
		meshData.indices.push_back(i * 3 + 1);
		meshData.indices.push_back(i * 3);
	}
}

void FBXLoader::LoadNormals(FBXMeshData& meshData, FbxMesh* mesh)
{
	FbxArray<FbxVector4> normals;
	// �@�����X�g�擾
	mesh->GetPolygonVertexNormals(normals);

	// �@���ݒ�
	for (size_t idx = 0; idx < normals.Size(); idx++) {
		meshData.vertices[idx].normal.x = (float)-normals[idx][0];
		meshData.vertices[idx].normal.y = (float)normals[idx][1];
		meshData.vertices[idx].normal.z = (float)normals[idx][2];
	}
}

bool FBXLoader::Load(const char* meshPath, std::vector<FBXMeshData>& meshDataList)
{
	assert(fbxManager != nullptr && fbxScene != nullptr && fbxImporter != nullptr);

	// �t�@�C��������������
	if (fbxImporter->Initialize(meshPath) == false) {
		fbxImporter->Destroy();
		fbxScene->Destroy();
		fbxManager->Destroy();
		return false;
	}

	// �C���|�[�g
	if (fbxImporter->Import(fbxScene) == false) {
		fbxImporter->Destroy();
		fbxScene->Destroy();
		fbxManager->Destroy();
		return false;
	}

	FbxGeometryConverter converter(fbxManager);
	// �S�Ă�FbxMesh���}�e���A�����Ƃɕ���
	converter.SplitMeshesPerMaterial(fbxScene, true);
	// �|���S�����O�p�`�ɂ���
	converter.Triangulate(fbxScene, true);

	// �}�e���A�����擾
	int materialNum = fbxScene->GetSrcObjectCount<FbxSurfaceMaterial>();

	// FbxMesh�̐����擾
	int meshNum = fbxScene->GetSrcObjectCount<FbxMesh>();

	// ���b�V�������̃������m��
	meshDataList.resize(meshNum);

	// ���b�V������
	for (int idx = 0; idx < meshNum; idx++) {
		meshDataList[idx] = CreateMesh(fbxScene->GetSrcObject<FbxMesh>(idx));
	}

	return true;
}