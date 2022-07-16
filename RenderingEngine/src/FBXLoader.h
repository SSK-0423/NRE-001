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
/// <summary>
/// FBX���b�V����ǂݍ��ނ̂ɕK�v�ȏ����܂Ƃ߂��\����
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
	/// FBX�t�@�C����ǂݍ���
	/// </summary>
	/// <param name="meshDataList">�ǂݍ��񂾃��b�V���̏����i�[���郊�X�g</param>
	/// <param name="data">FBX��ǂݍ��ނ̂ɕK�v�ȏ����܂Ƃ߂��\����</param>
	/// <returns>true:���� false:���s</returns>
	bool Load(std::vector<FBXMeshData>& meshDataList, FBXLoadData& data);

private:
	std::map<std::string, IMaterial*> _materials;

	/// <summary>
	/// FBX_SDK������
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