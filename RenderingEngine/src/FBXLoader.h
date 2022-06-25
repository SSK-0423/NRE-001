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
	std::vector<FBXMeshVertex> vertices;	// ���_
	std::vector<unsigned int> indices;		// ���_�C���f�b�N�X
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
	/// FBX_SDK������
	/// </summary>
	/// <returns></returns>
	bool Init();

	FBXMeshData CreateMesh(FbxMesh* mesh);

	// ���_�ǂݍ��� 
	void LoadVertices(FBXMeshData& meshData, FbxMesh* mesh);

	// �C���f�b�N�X�o�b�t�@�ǂݍ���
	void LoadIndices(FBXMeshData& meshData, FbxMesh* mesh);

	// �@���ǂݍ���
	void LoadNormals(FBXMeshData& meshData, FbxMesh* mesh);

	// �}�e���A���ǂݍ���
	//void LoadMaterial();
};