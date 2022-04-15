#pragma once

#include <array>

#include "EngineUtility.h"
#include "Polygon.h"
#include "DescriptorHeapCBV_SRV_UAV.h"

class RenderingContext;
class Dx12GraphicsEngine;
class Texture;

// スプライトに設定できる最大テクスチャ数
const int MAX_TEXTURE = 16;

/// <summary>
/// スプライト生成用データ
/// </summary>
struct SpriteData {
	std::array<Texture*, MAX_TEXTURE> textures = { nullptr };			        // テクスチャ
	std::array<const std::wstring*, MAX_TEXTURE> texturePaths = { nullptr };	// テクスチャパス(現在はWIC想定)
	ShaderData vertexShaderData;				                                // 頂点シェーダー生成データ
	ShaderData pixelShaderData;					                                // ピクセルシェーダー生成データ
	RootSignatureData rootSignatureData;			                            // ルートシグネチャ生成データ
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
	VertexBuffer _vertexBuffer;			    // 頂点バッファー
	IndexBuffer _indexBuffer;			    // インデックスバッファー

	Shader _vertexShader;				    // 頂点シェーダー
	Shader _pixelShader;				    // ピクセルシェーダー

	MyFrameWork::Polygon _polygon;		    // ポリゴン

	DescriptorHeapCBV_SRV_UAV _textureHeap;	// テクスチャ用ヒープ

	/// <summary>
	/// ポリゴン初期化
	/// </summary>
	/// <param name="device"></param>
	/// <returns></returns>
	MYRESULT CreatePolygon(ID3D12Device& device, SpriteData& spriteData);

	/// <summary>
	/// テクスチャリソース生成
	/// </summary>
	/// <param name="graphicsEngine">グラフィックスエンジン</param>
	/// <param name="device">デバイス</param>
	/// <param name="spriteData">スプライトデータ</param>
	/// <returns></returns>
	MYRESULT CreateTextureResource(
		Dx12GraphicsEngine& graphicsEngine, ID3D12Device& device, SpriteData& spriteData);

public:
	/// <summary>
	/// スプライト生成
	/// </summary>
	/// <param name="device"></param>
	/// <param name="spriteData"></param>
	/// <returns></returns>
	MYRESULT Create(Dx12GraphicsEngine& graphcisEngine, SpriteData& spriteData);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="renderContext"></param>
	void Draw(RenderingContext& renderContext);
};