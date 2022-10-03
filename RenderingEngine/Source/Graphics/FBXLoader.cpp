#include "FBXLoader.h"
#include "EngineUtility.h"
#include "PhongMaterial.h"
#include "PBRMaterial.h"

#include <Windows.h>
#include <cassert>

namespace NamelessEngine::Graphics
{
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
		_materials.clear();
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
		LoadUV(meshData, mesh);
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

	void FBXLoader::SetMaterial(FBXMeshData& meshData, FbxMesh* mesh)
	{
		// マテリアルが無ければ終わり
		if (mesh->GetElementMaterialCount() == 0) {
			meshData.material = nullptr;
			return;
		}

		// FbxMesh側のマテリアル情報取得
		FbxLayerElementMaterial* material = mesh->GetElementMaterial();
		int index = material->GetIndexArray().GetAt(0);
		FbxSurfaceMaterial* surfaceMaterial = mesh->GetNode()->GetSrcObject<FbxSurfaceMaterial>(index);

		if (surfaceMaterial != nullptr) {
			for (auto& mat : _materials) {
				if (mat.first == surfaceMaterial->GetName()) {
					meshData.material = mat.second;
				}
			}
		}
		else {
			meshData.material = nullptr;
		}
	}

	bool FBXLoader::LoadPhongMaterial(FbxSurfaceMaterial* material, std::wstring textureFolderPath)
	{
		PhongMaterial* phongMaterial = new PhongMaterial();

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
				phongMaterial->SetShininess(prop.Get<FbxDouble>());
			}
			else {
				phongMaterial->SetShininess(1.f);
			}

			FbxDouble3 color = materialColors[static_cast<int>(MATERIAL::DIFFUSE)];
			FbxDouble factor = factors[static_cast<int>(MATERIAL::DIFFUSE)];
			phongMaterial->SetDiffuse(
				static_cast<float>(color[0]),
				static_cast<float>(color[1]),
				static_cast<float>(color[2]),
				static_cast<float>(factor));

			color = materialColors[static_cast<int>(MATERIAL::AMBIENT)];
			factor = factors[static_cast<int>(MATERIAL::AMBIENT)];
			phongMaterial->SetAmbient(
				static_cast<float>(color[0]),
				static_cast<float>(color[1]),
				static_cast<float>(color[2]),
				static_cast<float>(factor));

			color = materialColors[static_cast<int>(MATERIAL::SPECULAR)];
			factor = factors[static_cast<int>(MATERIAL::SPECULAR)];
			phongMaterial->SetSpecular(
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
			phongMaterial->SetDiffuse(
				static_cast<float>(color[0]),
				static_cast<float>(color[1]),
				static_cast<float>(color[2]),
				static_cast<float>(factor));

			color = materialColors[static_cast<int>(MATERIAL::AMBIENT)];
			factor = factors[static_cast<int>(MATERIAL::AMBIENT)];
			phongMaterial->SetAmbient(
				static_cast<float>(color[0]),
				static_cast<float>(color[1]),
				static_cast<float>(color[2]),
				static_cast<float>(factor));
		}

		// テクスチャ読み込み
		// Diffuseプロパティを取得
		prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
		FbxFileTexture* texture = nullptr;
		int textureNum = prop.GetSrcObjectCount<FbxFileTexture>();
		// シングルテクスチャの場合
		if (textureNum > 0) {
			texture = prop.GetSrcObject<FbxFileTexture>(0);
		}
		// マルチテクスチャの場合
		else {
			int layerNum = prop.GetSrcObjectCount<FbxLayeredTexture>();
			if (layerNum > 0) {
				// TODO:マルチテクスチャへの対応
				texture = prop.GetSrcObject<FbxFileTexture>();
			}
		}

		// ファイルネームのみ取得
		phongMaterial->SetTextureName(GetTextureFileName(texture));
		// テクスチャフォルダへのパスセット
		phongMaterial->SetTextureFolderPath(textureFolderPath);
		// マテリアルを利用可能な状態にする
		if (phongMaterial->Commit() == Utility::MYRESULT::FAILED) { return false; }

		// マテリアルリストに登録
		_materials[material->GetName()] = phongMaterial;

		return true;
	}

	bool FBXLoader::LoadPBRMaterial(
		FbxSurfaceMaterial* material, std::wstring textureFolderPath,
		std::wstring baseColorTextureName, std::wstring metallicTextureName,
		std::wstring roughnessTextureName, std::wstring normalTextureName, std::wstring occulusionTextureName)
	{
		// PBRテクスチャ読み込み
		PBRMaterial* pbrMaterial = new PBRMaterial();
		pbrMaterial->SetTextureFolderPath(textureFolderPath);
		// マテリアルにテクスチャセット
		pbrMaterial->SetTextureName(PBRTEXTURETYPE::BASE_COLOR, baseColorTextureName);
		pbrMaterial->SetTextureName(PBRTEXTURETYPE::METALLIC, metallicTextureName);
		pbrMaterial->SetTextureName(PBRTEXTURETYPE::ROUGHNESS, roughnessTextureName);
		pbrMaterial->SetTextureName(PBRTEXTURETYPE::NORMAL, normalTextureName);
		pbrMaterial->SetTextureName(PBRTEXTURETYPE::OCCULUSION, occulusionTextureName);
		// マテリアルを利用可能にする
		if (pbrMaterial->Commit() == Utility::MYRESULT::FAILED) { return false; }
		// マテリアルリストに登録
		_materials[material->GetName()] = pbrMaterial;

		return true;
	}

	bool FBXLoader::LoadMaterial(FBXLoadData& data)
	{
		// マテリアル数取得
		int materialNum = fbxScene->GetSrcObjectCount<FbxSurfaceMaterial>();

		// フォンの反射モデルのマテリアル読み込み
		if (data.materialType == MATERIAL_TYPE::PHONG) {
			for (int idx = 0; idx < materialNum; idx++) {
				bool result = LoadPhongMaterial(
					fbxScene->GetSrcObject<FbxSurfaceMaterial>(idx), data.textureFolderPath);
				if (!result) { return false; }
			}
		}

		// PBRマテリアル読み込み
		else if (data.materialType == MATERIAL_TYPE::PBR) {
			for (int idx = 0; idx < materialNum; idx++) {
				bool result = LoadPBRMaterial(
					fbxScene->GetSrcObject<FbxSurfaceMaterial>(idx), data.textureFolderPath,
					data.baseColorName, data.metallicName, data.roughnessName, data.normalName, data.occlusionName);
				if (!result) { return false; }
			}
		}
		return true;
	}

	void FBXLoader::LoadUV(FBXMeshData& meshData, FbxMesh* mesh)
	{
		FbxStringList uvsetNames;
		// UVSetの名前リストを取得
		mesh->GetUVSetNames(uvsetNames);

		FbxArray<FbxVector2> uvBuffer;

		// UVSetの名前からUVsetを取得する
		// 今回はマルチテクスチャには対応しないので最初の名前を使う
		mesh->GetPolygonVertexUVs(uvsetNames.GetStringAt(0), uvBuffer);

		for (int idx = 0; idx < uvBuffer.Size(); idx++) {
			FbxVector2& uv = uvBuffer[idx];

			meshData.vertices[idx].uv.x = static_cast<float>(uv[0]);
			// DirectXは左上原点 テクスチャは左下原点に貼っているのでv値を反転
			meshData.vertices[idx].uv.y = static_cast<float>(1.f - uv[1]);
		}
	}

	std::wstring FBXLoader::GetTextureFileName(FbxFileTexture* texture)
	{
		if (texture == nullptr) { return L""; }

		// 相対パス取得
		std::string path = texture->GetRelativeFileName();

		// 置換
		path = Utility::ReplaceString(path, "\\", "/");

		// 拡張子込みのファイル名取得
		std::string::size_type pos = path.rfind("/");
		// 「/」を除いた文字列取得
		path = path.substr(pos + 1);

		// 文字化け対策
		char* charFileName;
		// 文字コード変換
		FbxUTF8ToAnsi(path.c_str(), charFileName);

		// wstringに変換
		wchar_t wchartFileName[256];
		errno_t err = Utility::charToWchar(charFileName, wchartFileName, 256);
		if (err != 0) {
			return L"";
		}

		return std::wstring(wchartFileName);
	}

	bool FBXLoader::Load(std::vector<FBXMeshData>& meshDataList, FBXLoadData& data)
	{
		assert(fbxManager != nullptr && fbxScene != nullptr && fbxImporter != nullptr);

		// ファイルを初期化する
		if (fbxImporter->Initialize(data.meshPath) == false) {
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

		// マテリアル読み込み
		if (LoadMaterial(data) == false) { return false; }

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
}

///
/// PBRテクスチャ読み込み
/// 仕様
///	・Phongの反射モデル
/// ・PBR
/// ・法線マップ+Phongの反射モデルとか
/// これらを使い分けたい(モデル単位)
/// 実装案1
///		boolで分岐
/// 実装案2
///		テクスチャ単位で読み込み　ファイル名が指定されていないものは読み込まない
/// 