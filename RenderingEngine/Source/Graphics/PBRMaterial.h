#pragma once
#include <string>
#include <unordered_map>

#include "IMaterial.h"
#include "Texture.h"

#include "Dx12GraphicsEngine.h"

namespace NamelessEngine::Graphics
{
	enum class PBRTEXTURETYPE
	{
		BASE_COLOR = 0,
		METALLIC,
		ROUGHNESS,
		NORMAL,
		OCCULUSION,	// アンビエントオクルージョン
		TYPE_NUM
	};

	/// <summary>
	/// PBR対応マテリアルクラス
	/// </summary>
	class PBRMaterial : public IMaterial {
	public:
		PBRMaterial();
		~PBRMaterial();
		PBRMaterial(const PBRMaterial& inst);

	private:
		std::wstring _textureFolderPath = L"";


		std::unordered_map<PBRTEXTURETYPE, std::wstring> _pbrTextureNames;
		std::unordered_map<PBRTEXTURETYPE, DX12API::Texture*> _pbrTextures;

		DX12API::Texture _dummyTexture;	// テクスチャが指定されていない場合のダミー用テクスチャ

		/// <summary>
		/// テクスチャが指定されていない場合の
		/// ダミーとなる真っ白なテクスチャ生成
		/// </summary>
		/// <returns></returns>
		Utility::RESULT CreateDammyTexture(Core::Dx12GraphicsEngine& graphicsEngine);

		Utility::RESULT LoadPBRTextures(Core::Dx12GraphicsEngine& graphicsEngine);

	public:
		/// <summary>
		/// マテリアルを利用可能な状態にする
		/// </summary>
		/// <returns></returns>
		Utility::RESULT Commit();

		void ApplyMaterial(FBXMesh& mesh) override;

		/// <summary>
		/// 読み込みたいテクスチャが置いてあるフォルダへのパス
		/// </summary>
		/// <param name="folderPath"></param>
		void SetTextureFolderPath(std::wstring folderPath);

		/// <summary>
		/// 読み込みたいテクスチャのファイル名(拡張子付き)セット
		/// </summary>
		/// <param name="type">テクスチャの種類(ベースカラー・メタリック・ラフネスなど)</param>
		/// <param name="path"></param>
		void SetTextureName(PBRTEXTURETYPE type, std::wstring textureName);
	};
}