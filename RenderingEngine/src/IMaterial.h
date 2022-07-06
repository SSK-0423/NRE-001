#pragma once

class FBXMesh;

enum class MATERIAL_TYPE
{
	PHONG,		// フォンの反射モデル
	PBR,		// 物理ベースレンダリング
	TYPE_NUM
};

/// <summary>
/// マテリアルクラスのインターフェース
/// </summary>
class IMaterial {
public:
	
	/// <summary>
	/// メッシュにマテリアル適用
	/// </summary>
	/// <param name="mesh">FBXメッシュ</param>
	virtual void ApplyMaterial(FBXMesh& mesh) = 0;
};