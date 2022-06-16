#pragma once
#include <DirectXMath.h>
#include <d3d12.h>
#include <vector>
#include <map>

/// <summary>
/// ���b�V���̒��_�\����
/// </summary>
struct MeshVertex {
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 normal;
};

class IMeshLoader {
public:
	/// <summary>
	/// ���b�V���ǂݍ���
	/// </summary>
	/// <param name="meshPath">���b�V���t�@�C���ւ̃p�X</param>
	/// <returns>����:true ���s:false</returns>
	virtual bool Load(const char* meshPath) = 0;

	// TODO:�߂�l�̍œK��
	virtual std::vector<MeshVertex>& GetVertices() = 0;

	// TODO:�߂�l�̍œK��
	virtual std::vector<unsigned int>& GetIndices() = 0;
};