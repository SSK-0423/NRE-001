#include "FBXLoader.h"

FBXLoader::FBXLoader()
{
}

FBXLoader::~FBXLoader()
{
}

MYRESULT FBXLoader::Init(const char* meshPath)
{
	// FbxManager生成
	FbxManager* fbxManager = FbxManager::Create();
	if (fbxManager == nullptr) {
		return MYRESULT::FAILED;
	}

	// FbxImporter生成
	FbxImporter* fbxImporter = FbxImporter::Create(fbxManager, "Importer");
	if (fbxImporter == nullptr) {
		fbxManager->Destroy();
		return MYRESULT::FAILED;
	}

	// FbxScene生成
	FbxScene* fbxScene = FbxScene::Create(fbxManager, "Scene");
	if (fbxScene == nullptr) {
		fbxImporter->Destroy();
		fbxManager->Destroy();
		return MYRESULT::FAILED;
	}

	// ファイルを初期化する
	if (fbxImporter->Initialize(meshPath) == false) {
		fbxImporter->Destroy();
		fbxScene->Destroy();
		fbxManager->Destroy();
		return MYRESULT::FAILED;
	}

	// インポート
	if (fbxImporter->Import(fbxScene) == false) {
		fbxImporter->Destroy();
		fbxScene->Destroy();
		fbxManager->Destroy();
		return MYRESULT::FAILED;
	}

	// 不要なものは破棄
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
