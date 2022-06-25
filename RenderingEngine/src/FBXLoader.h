#pragma once
#include <fbxsdk.h>
#include <map>
#include <vector>
#include <DirectXMath.h>

struct FBXMeshVertex {
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 normal;
};

struct FBXMeshData {
	std::vector<FBXMeshVertex> vertices;	// 頂点
	std::vector<unsigned int> indices;		// 頂点インデックス
};

class FBXLoader{
public:
	FBXLoader();
	~FBXLoader();

	FbxManager* fbxManager;
	FbxImporter* fbxImporter;
	FbxScene* fbxScene;

public:
	bool Load(const char* meshPath, std::vector<FBXMeshData>& meshDataList);

private:
	std::vector<FBXMeshData> _meshDataList;

	/// <summary>
	/// FBX_SDK初期化
	/// </summary>
	/// <returns></returns>
	bool Init();

	FBXMeshData CreateMesh(FbxMesh* mesh);

	// 頂点読み込み 
	void LoadVertices(FBXMeshData& meshData, FbxMesh* mesh);

	// インデックスバッファ読み込み
	void LoadIndices(FBXMeshData& meshData, FbxMesh* mesh);

	// 法線読み込み
	void LoadNormals(FBXMeshData& meshData, FbxMesh* mesh);

	// マテリアル読み込み
	//void LoadMaterial();
};