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
	MYRESULT LoadMesh(const char* modelPath);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="renderContext"></param>
	void Draw(RenderingContext& renderContext);
};