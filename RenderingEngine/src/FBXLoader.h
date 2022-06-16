#pragma once
#include <fbxsdk.h>
#include <map>
#include <vector>
#include <DirectXMath.h>

#include "IMeshLoader.h"

class FBXLoader : public IMeshLoader{
public:
	FBXLoader();
	~FBXLoader();

	FbxManager* fbxManager;
	FbxImporter* fbxImporter;
	FbxScene* fbxScene;

public:
	bool Load(const char* meshPath) override;

	std::vector<MeshVertex>& GetVertices() override;

	std::vector<unsigned int>& GetIndices() override;

private:
	std::map<std::string, FbxNode*> meshNodeList;

	/// <summary>
	/// FBX_SDK初期化
	/// </summary>
	/// <returns></returns>
	bool Init();

	/// <summary>
	/// メッシュノード収集
	/// </summary>
	/// <param name="node"></param>
	/// <param name="list"></param>
	void CollectMeshNode(FbxNode* node, std::map<std::string, FbxNode*>& list);

	/// <summary>
	/// メッシュ生成
	/// </summary>
	/// <param name="nodeName"></param>
	/// <param name="mesh"></param>
	void CreateMesh(const char* nodeName, FbxMesh* mesh);

	// TODO: map化が不必要なら削除
	void CreateMesh2(const char* nodeName, FbxMesh* mesh);

	// メッシュの頂点リスト
	std::vector<MeshVertex> meshVertices;
	// TODO: map化が不必要なら削除
	std::map<std::string, std::vector<MeshVertex>> meshVertices2;

	// メッシュのインデックスリスト
	std::vector<unsigned int> meshIndices;
	// TODO: map化が不必要なら削除
	std::map<std::string, std::vector<unsigned int>> meshIndices2;
};