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
/// 仕様:
/// ・メッシュクラスにセットするもの
/// ・PhongMaterialとPBRMaterialは別仕様
/// ・IMaterialクラスのセット関数でメッシュにマテリアルを適用する
/// マテリアルの定義
/// ・オブジェクトの質感を決定するもの
/// ・シェーディング関連の数値
/// ・テクスチャも該当
/// </summary>
class IMaterial {
protected:
	static const size_t _dummyTextureWidth = 16;
	static const size_t _dummyTextureHeight = 16;

public:
	
	/// <summary>
	/// メッシュにマテリアル適用
	/// </summary>
	/// <param name="mesh">FBXメッシュ</param>
	virtual void ApplyMaterial(FBXMesh& mesh) = 0;
};