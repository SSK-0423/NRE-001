#pragma once
#include <fbxsdk.h>
#include <map>
#include <vector>
#include <DirectXMath.h>

#include "IMaterial.h"

/// <summary>
/// FBX���b�V���̒��_�f�[�^
/// </summary>
struct FBXMeshVertex {
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 uv;
};

/// <summary>
/// FBX���b�V�������i�[����\����
/// FBXMesh�N���X�����̏������ɒ��_�o�b�t�@�[������}�e���A�����̃Z�b�g���s��
/// </summary>
struct FBXMeshData {
	std::vector<FBXMeshVertex> vertices;	// ���_
	std::vector<unsigned int> indices;		// ���_�C���f�b�N�X
	IMaterial* material = nullptr;			// �������}�e���A��
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
	/// FBX�t�@�C����ǂݍ���
	/// </summary>
	/// <param name="meshPath">.fbx�ւ̃p�X</param>
	/// <param name="textureFolderPath">�ǂݍ��ރ��f���̃e�N�X�`��������t�H���_�ւ̃p�X</param>
	/// <param name="materialType">�g�p����}�e���A���^�C�v</param>
	/// <param name="meshDataList">�ǂݍ��񂾃��b�V���̏����i�[���郊�X�g</param>
	/// <returns>true:���� false:���s</returns>
	bool Load(
		const char* meshPath, std::wstring textureFolderPath,
		MATERIAL_TYPE materialType, std::vector<FBXMeshData>& meshDataList);

private:
	std::map<std::string, IMaterial*> _materials;

	/// <summary>
	/// FBX_SDK������
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