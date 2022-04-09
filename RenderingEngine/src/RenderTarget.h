#pragma once

#include "DescriptorHeapRTV.h"
#include "DescriptorHeapCBV_SRV_UAV.h"
#include "RenderTargetBuffer.h"

#include "Texture.h"

#include "EngineUtility.h"

struct RenderTargetData
{
	RenderTargetBufferData renderTargetBufferData;	// レンダーターゲットバッファー用データ
};

/// <summary>
/// レンダーターゲットクラス
/// </summary>
class RenderTarget
{
private:
	Texture _renderTargetTexture;				// オフスクリーンテクスチャ
	DescriptorHeapCBV_SRV_UAV _textureHeap;		// オフスクリーンテクスチャ用ヒープ

	RenderTargetBuffer _renderTargetBuffer;		// レンダーターゲットバッファー
	DescriptorHeapRTV _rtvHeap;					// レンダーターゲット用ヒープ

	RenderTargetData _renderTargetData;			// レンダーターゲットデータ

public:
	/// <summary>
	/// レンダーターゲット生成
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <returns></returns>
	MYRESULT Create(ID3D12Device& device, RenderTargetData& renderTargetData);

	/// <summary>
	/// レンダリング開始
	/// </summary>
	/// <param name="renderContext">レンダリングコンテキスト</param>
	void BeginRendering(RenderingContext& renderContext);

	/// <summary>
	/// レンダリング終了
	/// </summary>
	/// <param name="renderContext">レンダリングコンテキスト</param>
	void EndRendering(RenderingContext& renderContext);

	/// <summary>
	/// レンダリング結果をテクスチャマッピングしたポリゴン描画
	/// </summary>
	/// <param name="renderContext">レンダリングコンテキスト</param>
	void Draw(RenderingContext& renderContext);
};