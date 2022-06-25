#include "FBXLoader.h"
#include <Windows.h>
#include <cassert>

FBXLoader::FBXLoader()
	: fbxManager(nullptr), fbxImporter(nullptr), fbxScene(nullptr)
{
	// 後で見直す必要あり
	if (Init() == false) {
		MessageBox(NULL, L"FBX_SDKの初期化に失敗しました。", L"エラーメッセージ", MB_OK);
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
	// FbxManager生成
	fbxManager = FbxManager::Create();
	if (fbxManager == nullptr) {
		return false;
	}

	// FbxImporter生成
	fbxImporter = FbxImporter::Create(fbxManager, "Importer");
	if (fbxImporter == nullptr) {
		fbxManager->Destroy();
		return false;
	}

	// FbxScene生成
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
	// 頂点バッファの取得 
	FbxVector4* vertices = mesh->GetControlPoints();
	// インデックスバッファの取得
	int* indices = mesh->GetPolygonVertices();
	// 頂点座標の数(頂点数)の取得
	unsigned long long polygonVertexCount = mesh->GetPolygonVertexCount();

	for (size_t idx = 0; idx < polygonVertexCount; idx++) {

		FBXMeshVertex vertex;

		// インデックスバッファから頂点番号を取得
		int index = indices[idx];

		// 頂点座標リストから座標を取得する
		vertex.position.x = -vertices[index][0]; // FBXは右手系 DirectXは左手系なのでX座標を反転
		vertex.position.y = vertices[index][1];
		vertex.position.z = vertices[index][2];

		// 追加
		meshData.vertices.push_back(vertex);
	}
}

void FBXLoader::LoadIndices(FBXMeshData& meshData, FbxMesh* mesh)
{
	// ポリゴン数の取得
	int polygonCount = mesh->GetPolygonCount();

	// ポリゴンの数だけ連番として保存する
	for (unsigned int i = 0; i < polygonCount; i++) {
		// インデックスバッファの順番で並んでいるのでそのまま
		// 左手系対策
		meshData.indices.push_back(i * 3 + 2);
		meshData.indices.push_back(i * 3 + 1);
		meshData.indices.push_back(i * 3);
	}
}

void FBXLoader::LoadNormals(FBXMeshData& meshData, FbxMesh* mesh)
{
	FbxArray<FbxVector4> normals;
	// 法線リスト取得
	mesh->GetPolygonVertexNormals(normals);

	// 法線設定
	for (size_t idx = 0; idx < normals.Size(); idx++) {
		meshData.vertices[idx].normal.x = (float)-normals[idx][0];
		meshData.vertices[idx].normal.y = (float)normals[idx][1];
		meshData.vertices[idx].normal.z = (float)normals[idx][2];
	}
}

bool FBXLoader::Load(const char* meshPath, std::vector<FBXMeshData>& meshDataList)
{
	assert(fbxManager != nullptr && fbxScene != nullptr && fbxImporter != nullptr);

	// ファイルを初期化する
	if (fbxImporter->Initialize(meshPath) == false) {
		fbxImporter->Destroy();
		fbxScene->Destroy();
		fbxManager->Destroy();
		return false;
	}

	// インポート
	if (fbxImporter->Import(fbxScene) == false) {
		fbxImporter->Destroy();
		fbxScene->Destroy();
		fbxManager->Destroy();
		return false;
	}

	FbxGeometryConverter converter(fbxManager);
	// 全てのFbxMeshをマテリアルごとに分割
	converter.SplitMeshesPerMaterial(fbxScene, true);
	// ポリゴンを三角形にする
	converter.Triangulate(fbxScene, true);

	// マテリアル数取得
	int materialNum = fbxScene->GetSrcObjectCount<FbxSurfaceMaterial>();

	// FbxMeshの数を取得
	int meshNum = fbxScene->GetSrcObjectCount<FbxMesh>();

	// メッシュ数分のメモリ確保
	meshDataList.resize(meshNum);

	// メッシュ生成
	for (int idx = 0; idx < meshNum; idx++) {
		meshDataList[idx] = CreateMesh(fbxScene->GetSrcObject<FbxMesh>(idx));
	}

	return true;
}