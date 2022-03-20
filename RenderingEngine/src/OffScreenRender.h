#pragma once
#include "DescriptorHeapCBV_SRV_UAV.h"
#include "Texture.h"
#include "RootSignature.h"
#include "GraphicsPipelineState.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Polygon.h"

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

public:
	void BeginDraw();
	void EndDraw();
	void NextPass();
};