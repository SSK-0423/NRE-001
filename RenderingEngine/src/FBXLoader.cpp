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

void FBXLoader::CollectMeshNode(FbxNode* node, std::map<std::string, FbxNode*>& list)
{
	// �m�[�h�Ƀ��b�V���A�g���r���[�g���܂܂�Ă���ꍇ�̓��X�g�ɒǉ�����
	for (size_t i = 0; i < node->GetNodeAttributeCount(); i++) {

		FbxNodeAttribute* attribute = node->GetNodeAttributeByIndex(i);

		// Attribute�����b�V���Ȃ�ǉ�
		if (attribute->GetAttributeType() == FbxNodeAttribute::EType::eMesh) {
			list[node->GetName()] = node;
			break;
		}
	}

	// �q�m�[�h�̃��b�V����T��
	for (size_t i = 0; i < node->GetChildCount(); i++) {
		CollectMeshNode(node->GetChild(i), list);
	}
}

void FBXLoader::CreateMesh(const char* nodeName, FbxMesh* mesh)
{
	// ���_�o�b�t�@�̎擾 
	FbxVector4* vertices = mesh->GetControlPoints();
	// �C���f�b�N�X�o�b�t�@�̎擾
	int* indices = mesh->GetPolygonVertices();
	// ���_���W�̐�(���_��)�̎擾
	unsigned long long polygonVertexCount = mesh->GetPolygonVertexCount();

	for (size_t idx = 0; idx < polygonVertexCount; idx++) {

		MeshVertex vertex;

		// �C���f�b�N�X�o�b�t�@���璸�_�ԍ����擾
		int index = indices[idx];

		// ���_���W���X�g������W���擾����
		vertex.position.x = -vertices[index][0]; // FBX�͉E��n DirectX�͍���n�Ȃ̂�X���W�𔽓]
		vertex.position.y = vertices[index][1];
		vertex.position.z = vertices[index][2];

		// �ǉ�
		meshVertices.push_back(vertex);
	}

	FbxArray<FbxVector4> normals;
	// �@�����X�g�擾
	mesh->GetPolygonVertexNormals(normals);

	// �@���ݒ�
	for (size_t idx = 0; idx < normals.Size(); idx++) {
		meshVertices[idx].normal.x = (float)-normals[idx][0];
		meshVertices[idx].normal.y = (float)normals[idx][1];
		meshVertices[idx].normal.z = (float)normals[idx][2];
	}

	// �|���S�����̎擾
	int polygonCount = mesh->GetPolygonCount();

	// �|���S���̐������A�ԂƂ��ĕۑ�����
	for (unsigned int i = 0; i < polygonCount; i++) {
		// �C���f�b�N�X�o�b�t�@�̏��Ԃŕ���ł���̂ł��̂܂�
		// ����n�΍�
		meshIndices.push_back(i * 3 + 2);
		meshIndices.push_back(i * 3 + 1);
		meshIndices.push_back(i * 3);
	}
}

void FBXLoader::CreateMesh2(const char* nodeName, FbxMesh* mesh)
{
	// ���_�o�b�t�@�̎擾
	FbxVector4* vertices = mesh->GetControlPoints();
	// �C���f�b�N�X�o�b�t�@�̎擾
	int* indices = mesh->GetPolygonVertices();
	// ���_���W�̐�(���_��)�̎擾
	unsigned long long polygonVertexCount = mesh->GetPolygonVertexCount();

	for (size_t idx = 0; idx < polygonVertexCount; idx++) {

		MeshVertex vertex;
		
		// �C���f�b�N�X�o�b�t�@���璸�_�ԍ����擾
		int index = indices[idx];

		// ���_���W���X�g������W���擾����
		vertex.position.x = -vertices[index][0]; // FBX�͉E��n DirectX�͍���n�Ȃ̂�X���W�𔽓]
		vertex.position.y = vertices[index][1];
		vertex.position.z = vertices[index][2];

		// �ǉ�
		meshVertices2[nodeName].push_back(vertex);
	}

	FbxArray<FbxVector4> normals;
	// �@�����X�g�擾
	mesh->GetPolygonVertexNormals(normals);

	// �@���ݒ�
	for (size_t idx = 0; idx < normals.Size(); idx++) {
		meshVertices2[nodeName][idx].normal.x = (float)-normals[idx][0];
		meshVertices2[nodeName][idx].normal.y = (float)normals[idx][1];
		meshVertices2[nodeName][idx].normal.z = (float)normals[idx][2];
	}

	// �|���S�����̎擾
	int polygonCount = mesh->GetPolygonCount();

	// �|���S���̐������A�ԂƂ��ĕۑ�����
	for (unsigned int i = 0; i < polygonCount; i++) {
		// �C���f�b�N�X�o�b�t�@�̏��Ԃŕ���ł���̂ł��̂܂�
		// ����n�΍�
		meshIndices2[nodeName].push_back(i * 3 + 2);
		meshIndices2[nodeName].push_back(i * 3 + 1);
		meshIndices2[nodeName].push_back(i * 3);
	}
}

bool FBXLoader::Load(const char* meshPath)
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

	// �|���S�����O�p�`�ɂ���
	FbxGeometryConverter converter(fbxManager);
	converter.Triangulate(fbxScene, true);

	// ���[�g�m�[�h�擾
	FbxNode* rootNode = fbxScene->GetRootNode();
	if (rootNode == nullptr) {
		return false;
	}

	// ���b�V��Node��T��
	CollectMeshNode(rootNode, meshNodeList);

	for (auto data : meshNodeList) {
		// mesh����
		CreateMesh(data.first.c_str(), data.second->GetMesh());
		CreateMesh2(data.first.c_str(), data.second->GetMesh());
	}

	return true;
}

std::vector<MeshVertex>& FBXLoader::GetVertices()
{
	return meshVertices;
}

std::map<std::string, std::vector<MeshVertex>>& FBXLoader::GetMapVertices()
{
	return meshVertices2;
}

std::vector<unsigned int>& FBXLoader::GetIndices()
{
	return meshIndices;
}

std::map<std::string, std::vector<unsigned int>>& FBXLoader::GetMapIndices()
{
	return meshIndices2;
}
