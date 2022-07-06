#pragma once
#include <fbxsdk.h>
#include <map>
#include <vector>
#include <DirectXMath.h>

#include "IMaterial.h"

/// <summary>
/// FBXメッシュの頂点データ
/// </summary>
struct FBXMeshVertex {
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 uv;
};

/// <summary>
/// FBXメッシュ情報を格納する構造体
/// FBXMeshクラスがこの情報を元に頂点バッファー生成やマテリアル情報のセットを行う
/// </summary>
struct FBXMeshData {
	std::vector<FBXMeshVertex> vertices;	// 頂点
	std::vector<unsigned int> indices;		// 頂点インデックス
	IMaterial* material = nullptr;			// 実装中マテリアル
};

class FBXLoader {
public:
	FBXLoader();
	~FBXLoader();

	FbxManager* fbxManager;
	FbxImporter* fbxImporter;
	FbxScene* fbxScene;

public:
	/// <summary>
	/// FBXファイルを読み込む
	/// </summary>
	/// <param name="meshPath">.fbxへのパス</param>
	/// <param name="textureFolderPath">読み込むモデルのテクスチャがあるフォルダへのパス</param>
	/// <param name="materialType">使用するマテリアルタイプ</param>
	/// <param name="meshDataList">読み込んだメッシュの情報を格納するリスト</param>
	/// <returns>true:成功 false:失敗</returns>
	bool Load(
		const char* meshPath, std::wstring textureFolderPath,
		MATERIAL_TYPE materialType, std::vector<FBXMeshData>& meshDataList);

private:
	std::map<std::string, IMaterial*> _materials;

	/// <summary>
	/// FBX_SDK初期化
	/// </summary>
	/// <returns></returns>
	bool Init();

	bool LoadPhongMaterial(FbxSurfaceMaterial* material, std::wstring textureFolderPath);

	bool LoadPBRMaterial(FbxSurfaceMaterial* material, std::wstring textureFolderPath);

	bool LoadMaterial(MATERIAL_TYPE materialType, std::wstring textureFolderPath);

	FBXMeshData CreateMesh(FbxMesh* mesh);

	void LoadVertices(FBXMeshData& meshData, FbxMesh* mesh);

	void LoadIndices(FBXMeshData& meshData, FbxMesh* mesh);

	void LoadNormals(FBXMeshData& meshData, FbxMesh* mesh);

	void SetMaterial(FBXMeshData& meshData, FbxMesh* mesh);


	void LoadUV(FBXMeshData& meshData, FbxMesh* mesh);

	std::wstring GetTextureFileName(FbxFileTexture* texture);
};