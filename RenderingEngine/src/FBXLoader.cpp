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
	SetMaterial(meshData, mesh);

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

void FBXLoader::LoadMaterial(FbxSurfaceMaterial* material)
{
	FBXMaterial fbxMaterial;

	enum class MATERIAL
	{
		DIFFUSE,
		AMBIENT,
		SPECULAR,
		MATERIAL_NUM
	};

	FbxDouble3 materialColors[static_cast<int>(MATERIAL::MATERIAL_NUM)];
	FbxDouble factors[static_cast<int>(MATERIAL::MATERIAL_NUM)];
	FbxProperty prop;

	// FbxSurfacePhongクラスか判定
	if (material->GetClassId().Is(FbxSurfacePhong::ClassId)) {
		// 取得するマテリアルデータ
		const char* colorElements[] = {
			FbxSurfaceMaterial::sDiffuse,
			FbxSurfaceMaterial::sAmbient,
			FbxSurfaceMaterial::sSpecular,
		};

		// 取得するファクター(重み)
		const char* factorElements[] = {
			FbxSurfaceMaterial::sDiffuseFactor,
			FbxSurfaceMaterial::sAmbientFactor,
			FbxSurfaceMaterial::sSpecularFactor,
		};

		for (size_t idx = 0; idx < _countof(colorElements); idx++) {
			prop = material->FindProperty(colorElements[idx]);
			if (prop.IsValid()) {
				materialColors[idx] = prop.Get<FbxDouble3>();
			}
			else {
				materialColors[idx] = FbxDouble3(1.f, 1.f, 1.f);
			}
		}

		for (size_t idx = 0; idx < _countof(factorElements); idx++) {
			prop = material->FindProperty(factorElements[idx]);
			if (prop.IsValid()) {
				factors[idx] = prop.Get<FbxDouble>();
			}
			else {
				factors[idx] = 1.f;
			}
		}

		// Shiness取得
		prop = material->FindProperty(FbxSurfaceMaterial::sShininess);
		if (prop.IsValid()) {
			fbxMaterial.shiness = prop.Get<FbxDouble>();
		}
		else {
			fbxMaterial.shiness = 1.f;
		}

		FbxDouble3 color = materialColors[static_cast<int>(MATERIAL::DIFFUSE)];
		FbxDouble factor = factors[static_cast<int>(MATERIAL::DIFFUSE)];
		fbxMaterial.SetDiffuse(
			static_cast<float>(color[0]),
			static_cast<float>(color[1]),
			static_cast<float>(color[2]),
			static_cast<float>(factor));

		color = materialColors[static_cast<int>(MATERIAL::AMBIENT)];
		factor = factors[static_cast<int>(MATERIAL::AMBIENT)];
		fbxMaterial.SetAmbient(
			static_cast<float>(color[0]),
			static_cast<float>(color[1]),
			static_cast<float>(color[2]),
			static_cast<float>(factor));

		color = materialColors[static_cast<int>(MATERIAL::SPECULAR)];
		factor = factors[static_cast<int>(MATERIAL::SPECULAR)];
		fbxMaterial.SetSpecular(
			static_cast<float>(color[0]),
			static_cast<float>(color[1]),
			static_cast<float>(color[2]),
			static_cast<float>(factor));
	}

	// FbxSurfaceLambertクラスか判定
	else if (material->GetClassId().Is(FbxSurfaceLambert::ClassId)) {
		// 取得するマテリアルカラー
		const char* colorElements[] = {
			FbxSurfaceMaterial::sDiffuse,
			FbxSurfaceMaterial::sAmbient,
		};
		// 取得するファクター(重み)
		const char* factorElements[] = {
			FbxSurfaceMaterial::sDiffuseFactor,
			FbxSurfaceMaterial::sAmbientFactor,
		};

		for (size_t idx = 0; idx < _countof(colorElements); idx++) {
			prop = material->FindProperty(colorElements[idx]);
			if (prop.IsValid()) {
				materialColors[idx] = prop.Get<FbxDouble3>();
			}
			else {
				materialColors[idx] = FbxDouble3(1.f, 1.f, 1.f);
			}
		}

		for (size_t idx = 0; idx < _countof(factorElements); idx++) {
			prop = material->FindProperty(factorElements[idx]);
			if (prop.IsValid()) {
				factors[idx] = prop.Get<FbxDouble>();
			}
			else {
				factors[idx] = 1.f;
			}
		}

		FbxDouble3 color = materialColors[static_cast<int>(MATERIAL::DIFFUSE)];
		FbxDouble factor = factors[static_cast<int>(MATERIAL::DIFFUSE)];
		fbxMaterial.SetDiffuse(
			static_cast<float>(color[0]),
			static_cast<float>(color[1]),
			static_cast<float>(color[2]),
			static_cast<float>(factor));

		color = materialColors[static_cast<int>(MATERIAL::AMBIENT)];
		factor = factors[static_cast<int>(MATERIAL::AMBIENT)];
		fbxMaterial.SetAmbient(
			static_cast<float>(color[0]),
			static_cast<float>(color[1]),
			static_cast<float>(color[2]),
			static_cast<float>(factor));
	}

	_materials[material->GetName()] = fbxMaterial;
}

void FBXLoader::SetMaterial(FBXMeshData& meshData, FbxMesh* mesh)
{
	// マテリアルが無ければ終わり
	if (mesh->GetElementMaterialCount() == 0) {
		//TODO: ない場合のマテリアルをどうするか
		return;
	}

	// FbxMesh側のマテリアル情報取得
	FbxLayerElementMaterial* material = mesh->GetElementMaterial();
	int index = material->GetIndexArray().GetAt(0);
	FbxSurfaceMaterial* surfaceMaterial = mesh->GetNode()->GetSrcObject<FbxSurfaceMaterial>(index);

	if (surfaceMaterial != nullptr) {
		for (auto mat : _materials) {
			if (mat.first == surfaceMaterial->GetName()) {
				meshData.material = mat.second;
			}
		}
	}
	else {
		//TODO: ない場合のマテリアルをどうするか
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
	for (int idx = 0; idx < materialNum; idx++) {
		LoadMaterial(fbxScene->GetSrcObject<FbxSurfaceMaterial>(idx));
	}

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