#pragma once

#include <array>

#include "EngineUtility.h"
#include "Polygon.h"
#include "DescriptorHeapCBV_SRV_UAV.h"
#include "RenderingContext.h"
#include "Dx12GraphicsEngine.h"
#include "Texture.h"

namespace NamelessEngine::Graphics
{
	// スプライトに設定できる最大テクスチャ数
	const int MAX_TEXTURE = 16;

	/// <summary>
	/// スプライト生成用データ
	/// </summary>
	struct SpriteData {
		std::array<DX12API::Texture*, MAX_TEXTURE> textures = { nullptr };			        // テクスチャ
		std::array<const std::wstring*, MAX_TEXTURE> texturePaths = { nullptr };	// テクスチャパス(現在はWIC想定)
		DX12API::ShaderData vertexShaderData;				                                // 頂点シェーダー生成データ
		DX12API::ShaderData pixelShaderData;					                                // ピクセルシェーダー生成データ
		DX12API::RootSignatureData rootSignatureData;			                            // ルートシグネチャ生成データ
		unsigned int width = 0;	                                                    // スプライト幅(ピクセル単位)
		unsigned int height = 0;	                                                // スプライト高(ピクセル単位)

		std::array<DXGI_FORMAT, D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT> colorFormats = {
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_UNKNOWN,
			DXGI_FORMAT_UNKNOWN,
			DXGI_FORMAT_UNKNOWN,
			DXGI_FORMAT_UNKNOWN,
			DXGI_FORMAT_UNKNOWN,
			DXGI_FORMAT_UNKNOWN,
			DXGI_FORMAT_UNKNOWN,
		};	// レンダーターゲットのカラーフォーマット
	};

	/// <summary>
	/// スプライトクラス
	/// </summary>
	class Sprite {
	public:
		Sprite() = default;
		~Sprite() = default;

	private:
		// 固定
		DX12API::VertexBuffer _vertexBuffer;			    // 頂点バッファー
		DX12API::IndexBuffer _indexBuffer;			    // インデックスバッファー

		DX12API::Shader _vertexShader;				    // 頂点シェーダー
		DX12API::Shader _pixelShader;				    // ピクセルシェーダー

		Polygon _polygon;		    // ポリゴン

		DX12API::DescriptorHeapCBV_SRV_UAV _textureHeap;	// テクスチャ用ヒープ

		/// <summary>
		/// ポリゴン初期化
		/// </summary>
		/// <param name="device"></param>
		/// <returns></returns>
		Utility::RESULT CreatePolygon(ID3D12Device& device, SpriteData& spriteData);

		/// <summary>
		/// テクスチャリソース生成
		/// </summary>
		/// <param name="graphicsEngine">グラフィックスエンジン</param>
		/// <param name="device">デバイス</param>
		/// <param name="spriteData">スプライトデータ</param>
		/// <returns></returns>
		Utility::RESULT CreateTextureResource(
			Core::Dx12GraphicsEngine& graphicsEngine, ID3D12Device& device, SpriteData& spriteData);

	public:
		/// <summary>
		/// スプライト生成
		/// </summary>
		/// <param name="device"></param>
		/// <param name="spriteData"></param>
		/// <returns></returns>
		Utility::RESULT Create(Core::Dx12GraphicsEngine& graphcisEngine, SpriteData& spriteData);

		/// <summary>
		/// 描画
		/// </summary>
		/// <param name="renderContext"></param>
		void Draw(DX12API::RenderingContext& renderContext);
	};
}