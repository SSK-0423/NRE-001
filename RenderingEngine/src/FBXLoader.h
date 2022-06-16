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
	/// FBX_SDK������
	/// </summary>
	/// <returns></returns>
	bool Init();

	/// <summary>
	/// ���b�V���m�[�h���W
	/// </summary>
	/// <param name="node"></param>
	/// <param name="list"></param>
	void CollectMeshNode(FbxNode* node, std::map<std::string, FbxNode*>& list);

	/// <summary>
	/// ���b�V������
	/// </summary>
	/// <param name="nodeName"></param>
	/// <param name="mesh"></param>
	void CreateMesh(const char* nodeName, FbxMesh* mesh);

	// TODO: map�����s�K�v�Ȃ�폜
	void CreateMesh2(const char* nodeName, FbxMesh* mesh);

	// ���b�V���̒��_���X�g
	std::vector<MeshVertex> meshVertices;
	// TODO: map�����s�K�v�Ȃ�폜
	std::map<std::string, std::vector<MeshVertex>> meshVertices2;

	// ���b�V���̃C���f�b�N�X���X�g
	std::vector<unsigned int> meshIndices;
	// TODO: map�����s�K�v�Ȃ�폜
	std::map<std::string, std::vector<unsigned int>> meshIndices2;
};