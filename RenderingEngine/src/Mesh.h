#pragma once

#include "EngineUtility.h"
#include "RenderingContext.h"
#include "FBXLoader.h"

/// <summary>
/// メッシュクラス
/// </summary>
class Mesh {
public:
	Mesh();
	~Mesh();
public:
	/// <summary>
	/// メッシュ読み込み
	/// </summary>
	/// <param name="modelPath">ファイルパス</param>
	/// <returns></returns>
	MYRESULT LoadMesh(const char* modelPath,ID3D12Device& device);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="renderContext"></param>
	void Draw(RenderingContext& renderContext);

private:

	MYRESULT CreateVertexBuffers(ID3D12Device& device);

	MYRESULT CreateIndexBuffers(ID3D12Device& device);
};