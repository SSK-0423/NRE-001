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
/// <summary>
/// FBXメッシュを読み込むのに必要な情報をまとめた構造体
/// </summary>
struct FBXLoadData {
	const char* meshPath;
	MATERIAL_TYPE materialType;
	std::wstring textureFolderPath;

	std::wstring baseColorName = L"";
	std::wstring metallicName = L"";
	std::wstring roughnessName = L"";
	std::wstring normalName = L"";
	std::wstring occlusionName = L"";
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
	/// <param name="meshDataList">読み込んだメッシュの情報を格納するリスト</param>
	/// <param name="data">FBXを読み込むのに必要な情報をまとめた構造体</param>
	/// <returns>true:成功 false:失敗</returns>
	bool Load(std::vector<FBXMeshData>& meshDataList, FBXLoadData& data);

private:
	std::map<std::string, IMaterial*> _materials;

	/// <summary>
	/// FBX_SDK初期化
	/// </summary>
	/// <returns></returns>
	bool Init();

	bool LoadPhongMaterial(FbxSurfaceMaterial* material, std::wstring textureFolderPath);

	bool LoadPBRMaterial(
		FbxSurfaceMaterial* material, std::wstring textureFolderPath,
		std::wstring baseColorTextureName, std::wstring metallicTextureName,
		std::wstring roughnessTextureName, std::wstring normalTextureName,
		std::wstring occlusionTextureName);

	bool LoadMaterial(FBXLoadData& data);

	FBXMeshData CreateMesh(FbxMesh* mesh);

	void LoadVertices(FBXMeshData& meshData, FbxMesh* mesh);

	void LoadIndices(FBXMeshData& meshData, FbxMesh* mesh);

	void LoadNormals(FBXMeshData& meshData, FbxMesh* mesh);

	void SetMaterial(FBXMeshData& meshData, FbxMesh* mesh);

	void LoadUV(FBXMeshData& meshData, FbxMesh* mesh);

	std::wstring GetTextureFileName(FbxFileTexture* texture);
};