#include "FBXLoader.h"

FBXLoader::FBXLoader()
{
}

FBXLoader::~FBXLoader()
{
}

MYRESULT FBXLoader::Init(const char* meshPath)
{
	// FbxManager����
	FbxManager* fbxManager = FbxManager::Create();
	if (fbxManager == nullptr) {
		return MYRESULT::FAILED;
	}

	// FbxImporter����
	FbxImporter* fbxImporter = FbxImporter::Create(fbxManager, "Importer");
	if (fbxImporter == nullptr) {
		fbxManager->Destroy();
		return MYRESULT::FAILED;
	}

	// FbxScene����
	FbxScene* fbxScene = FbxScene::Create(fbxManager, "Scene");
	if (fbxScene == nullptr) {
		fbxImporter->Destroy();
		fbxManager->Destroy();
		return MYRESULT::FAILED;
	}

	// �t�@�C��������������
	if (fbxImporter->Initialize(meshPath) == false) {
		fbxImporter->Destroy();
		fbxScene->Destroy();
		fbxManager->Destroy();
		return MYRESULT::FAILED;
	}

	// �C���|�[�g
	if (fbxImporter->Import(fbxScene) == false) {
		fbxImporter->Destroy();
		fbxScene->Destroy();
		fbxManager->Destroy();
		return MYRESULT::FAILED;
	}

	// �s�v�Ȃ��͔̂j��
	if (fbxImporter != nullptr) {
		fbxImporter->Destroy();
	}
	if (fbxScene != nullptr) {
		fbxScene->Destroy();
	}
	if (fbxManager != nullptr) {
		fbxManager->Destroy();
	}

	return MYRESULT::SUCCESS;
}
