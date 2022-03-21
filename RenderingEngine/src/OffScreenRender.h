#pragma once
#include "DescriptorHeapCBV_SRV_UAV.h"
#include "Texture.h"
#include "RootSignature.h"
#include "GraphicsPipelineState.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Polygon.h"

#include "EngineUtility.h"

#include <d3dx12.h>

class RenderingContext;

/// <summary>
/// オフスクリーンレンダー生成用データ
/// </summary>
struct OffScreenRenderData
{
	RenderTargetBufferData renderTargetData;	// レンダーターゲットデータ
	RootSignature rootSignature;                // ルートシグネチャ
	ShaderData vertexShaderData;                // 頂点シェーダーデータ
	ShaderData pixelShaderData;                 // ピクセルシェーダーデータ
	CD3DX12_VIEWPORT viewport;                  // ビューポート
	CD3DX12_RECT scissorRect;                   // シザー矩形
};

/// <summary>
/// オフスクリーンレンダークラス
/// </summary>
class OffScreenRender
{
private:
	DescriptorHeapCBV_SRV_UAV _offscreenHeap;	        // オフスクリーンテクスチャ用ヒープ
	Texture _offscreenTexture;	                        // オフスクリーンテクスチャ

	DescriptorHeapRTV _offscreenRTVHeap;	            // オフスクリーンレンダー用ヒープ
	RenderTargetBuffer _offscreenRender;	            // オフスクリーンレンダー

	RootSignature _offscreenRootSignature;	            // オフスクリーン用ルートシグネチャ(今は未使用)
	GraphicsPipelineState _offscreenGraphicsPipeline;	// オフスクリーン用パイプライン

	VertexBuffer _offscreenPolygonVB;					// オフスクリーン用頂点バッファー
	IndexBuffer _offscreenPolygonIB;					// オフスクリーン用インデックスバッファー
	MyFrameWork::Polygon _offscreenPolygon;				// オフスクリーンポリゴン
														// これに前パスのレンダリング結果をテクスチャマップする
	Shader _offscreenVS;	                            // オフスクリーンポリゴン用頂点シェーダー
	Shader _offscreenPS;	                            // オフスクリーンポリゴン用ピクセルシェーダー

	OffScreenRenderData _offScreenRenderData;

	/// <summary>
	/// オフスクリーンポリゴン生成
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="offScreenData">オフスクリーンデータ</param>
	/// <returns>MYRESULT::SUCCESS: 成功 MYRESULT::FAILED: 失敗</returns>
	MYRESULT CreateOffScreenPolygon(ID3D12Device& device, const OffScreenRenderData& offScreenData);

public:
	/// <summary>
	/// オフスクリーンレンダー生成
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="offScreenRenderData">オフスクリーンレンダーデータ</param>
	/// <returns>MYRESULT::SUCCESS: 成功 MYRESULT::FAILED: 失敗</returns>
	MYRESULT Create(ID3D12Device& device, const OffScreenRenderData& offScreenRenderData);

	/// <summary>
	/// レンダリング開始 オフスクリーンレンダリングを開始する際に呼ぶ
	/// </summary>
	void BeginRendering(RenderingContext& renderContext);
	/// <summary>
	/// レンダリング終了
	/// </summary>
	void EndRendering(RenderingContext& renderContext);
	/// <summary>
	/// 次フレームでレンダーターゲットとして使用できるようにする
	/// </summary>
	void NextPass(RenderingContext& renderContext);
	/// <summary>
	/// レンダリング結果をテクスチャマッピングしたポリゴン描画
	/// </summary>
	/// <param name="renderContext"></param>
	void Draw(RenderingContext& renderContext);

	DescriptorHeapCBV_SRV_UAV& GetDescriptorHeap() { return _offscreenHeap; }
};