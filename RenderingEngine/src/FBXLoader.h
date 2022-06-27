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

struct FBXMaterial {
	FBXMaterial()
	{
		for (int i = 0; i < _countof(ambient); i++)
		{
			ambient[i] = 1.0f;
			diffuse[i] = 1.0f;
			specular[i] = 1.0f;
		}
		alpha = 0.f;
		//TextureKeyWord = "";
		//TextureName = "";
	}

	void SetAmbient(float r, float g, float b, float factor)
	{
		ambient[0] = r;
		ambient[1] = g;
		ambient[2] = b;
		ambient[3] = factor;
	}

	void SetDiffuse(float r, float g, float b, float factor)
	{
		diffuse[0] = r;
		diffuse[1] = g;
		diffuse[2] = b;
		diffuse[3] = factor;
	}

	void SetSpecular(float r, float g, float b, float factor)
	{
		specular[0] = r;
		specular[1] = g;
		specular[2] = b;
		specular[3] = factor;
	}

	float ambient[4];
	float diffuse[4];
	float specular[4];
	float alpha;	// これなに？Shiness?
	//std::string TextureKeyWord;
	//std::string TextureName;
};

class FBXLoader {
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

	std::map<std::string, FBXMaterial> _materials;

	/// <summary>
	/// FBX_SDK初期化
	/// </summary>
	/// <returns></returns>
	bool Init();

	FBXMeshData CreateMesh(FbxMesh* mesh);

	void LoadVertices(FBXMeshData& meshData, FbxMesh* mesh);

	void LoadIndices(FBXMeshData& meshData, FbxMesh* mesh);

	void LoadNormals(FBXMeshData& meshData, FbxMesh* mesh);

	void LoadMaterial(FbxSurfaceMaterial* material);
};