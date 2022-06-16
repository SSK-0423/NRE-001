#pragma once
#include <DirectXMath.h>
#include <d3d12.h>
#include <vector>
#include <map>

/// <summary>
/// メッシュの頂点構造体
/// </summary>
struct MeshVertex {
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 normal;
};

class IMeshLoader {
public:
	/// <summary>
	/// メッシュ読み込み
	/// </summary>
	/// <param name="meshPath">メッシュファイルへのパス</param>
	/// <returns>成功:true 失敗:false</returns>
	virtual bool Load(const char* meshPath) = 0;

	// TODO:戻り値の最適化
	virtual std::vector<MeshVertex>& GetVertices() = 0;

	// TODO:戻り値の最適化
	virtual std::vector<unsigned int>& GetIndices() = 0;
};