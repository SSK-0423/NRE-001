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

void FBXLoader::CollectMeshNode(FbxNode* node, std::map<std::string, FbxNode*>& list)
{
	// ノードにメッシュアトリビュートが含まれている場合はリストに追加する
	for (size_t i = 0; i < node->GetNodeAttributeCount(); i++) {

		FbxNodeAttribute* attribute = node->GetNodeAttributeByIndex(i);

		// Attributeがメッシュなら追加
		if (attribute->GetAttributeType() == FbxNodeAttribute::EType::eMesh) {
			list[node->GetName()] = node;
			break;
		}
	}

	// 子ノードのメッシュを探す
	for (size_t i = 0; i < node->GetChildCount(); i++) {
		CollectMeshNode(node->GetChild(i), list);
	}
}

void FBXLoader::CreateMesh(const char* nodeName, FbxMesh* mesh)
{
	// 頂点バッファの取得 
	FbxVector4* vertices = mesh->GetControlPoints();
	// インデックスバッファの取得
	int* indices = mesh->GetPolygonVertices();
	// 頂点座標の数(頂点数)の取得
	unsigned long long polygonVertexCount = mesh->GetPolygonVertexCount();

	for (size_t idx = 0; idx < polygonVertexCount; idx++) {

		MeshVertex vertex;

		// インデックスバッファから頂点番号を取得
		int index = indices[idx];

		// 頂点座標リストから座標を取得する
		vertex.position.x = -vertices[index][0]; // FBXは右手系 DirectXは左手系なのでX座標を反転
		vertex.position.y = vertices[index][1];
		vertex.position.z = vertices[index][2];

		// 追加
		meshVertices.push_back(vertex);
	}

	FbxArray<FbxVector4> normals;
	// 法線リスト取得
	mesh->GetPolygonVertexNormals(normals);

	// 法線設定
	for (size_t idx = 0; idx < normals.Size(); idx++) {
		meshVertices[idx].normal.x = (float)-normals[idx][0];
		meshVertices[idx].normal.y = (float)normals[idx][1];
		meshVertices[idx].normal.z = (float)normals[idx][2];
	}

	// ポリゴン数の取得
	int polygonCount = mesh->GetPolygonCount();

	// ポリゴンの数だけ連番として保存する
	for (unsigned int i = 0; i < polygonCount; i++) {
		// インデックスバッファの順番で並んでいるのでそのまま
		// 左手系対策
		meshIndices.push_back(i * 3 + 2);
		meshIndices.push_back(i * 3 + 1);
		meshIndices.push_back(i * 3);
	}
}

void FBXLoader::CreateMesh2(const char* nodeName, FbxMesh* mesh)
{
	// 頂点バッファの取得
	FbxVector4* vertices = mesh->GetControlPoints();
	// インデックスバッファの取得
	int* indices = mesh->GetPolygonVertices();
	// 頂点座標の数(頂点数)の取得
	unsigned long long polygonVertexCount = mesh->GetPolygonVertexCount();

	for (size_t idx = 0; idx < polygonVertexCount; idx++) {

		MeshVertex vertex;
		
		// インデックスバッファから頂点番号を取得
		int index = indices[idx];

		// 頂点座標リストから座標を取得する
		vertex.position.x = -vertices[index][0]; // FBXは右手系 DirectXは左手系なのでX座標を反転
		vertex.position.y = vertices[index][1];
		vertex.position.z = vertices[index][2];

		// 追加
		meshVertices2[nodeName].push_back(vertex);
	}

	FbxArray<FbxVector4> normals;
	// 法線リスト取得
	mesh->GetPolygonVertexNormals(normals);

	// 法線設定
	for (size_t idx = 0; idx < normals.Size(); idx++) {
		meshVertices2[nodeName][idx].normal.x = (float)-normals[idx][0];
		meshVertices2[nodeName][idx].normal.y = (float)normals[idx][1];
		meshVertices2[nodeName][idx].normal.z = (float)normals[idx][2];
	}

	// ポリゴン数の取得
	int polygonCount = mesh->GetPolygonCount();

	// ポリゴンの数だけ連番として保存する
	for (unsigned int i = 0; i < polygonCount; i++) {
		// インデックスバッファの順番で並んでいるのでそのまま
		// 左手系対策
		meshIndices2[nodeName].push_back(i * 3 + 2);
		meshIndices2[nodeName].push_back(i * 3 + 1);
		meshIndices2[nodeName].push_back(i * 3);
	}
}

bool FBXLoader::Load(const char* meshPath)
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

	// ポリゴンを三角形にする
	FbxGeometryConverter converter(fbxManager);
	converter.Triangulate(fbxScene, true);

	// ルートノード取得
	FbxNode* rootNode = fbxScene->GetRootNode();
	if (rootNode == nullptr) {
		return false;
	}

	// メッシュNodeを探す
	CollectMeshNode(rootNode, meshNodeList);

	for (auto data : meshNodeList) {
		// mesh生成
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
