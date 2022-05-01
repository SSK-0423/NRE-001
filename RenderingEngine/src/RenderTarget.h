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
	void BeginRendering(RenderingContext& renderContext, CD3DX12_VIEWPORT& viewport, CD3DX12_RECT& scissorRect);

	/// <summary>
	/// レンダリング終了
	/// </summary>
	/// <param name="renderContext">レンダリングコンテキスト</param>
	void EndRendering(RenderingContext& renderContext);

	/// <summary>
	/// レンダーターゲットのテクスチャ取得
	/// </summary>
	/// <returns>レンダーターゲットテクスチャ</returns>
	Texture& GetRenderTargetTexture() {
		return _renderTargetTexture;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="renderTargets"></param>
	/// <param name="renderContext"></param>
	/// <param name="viewport"></param>
	/// <param name="scissorRect"></param>
	static void BeginMultiRendering(
		RenderTarget* renderTargets, const size_t& length, RenderingContext& renderContext,
		CD3DX12_VIEWPORT& viewport, CD3DX12_RECT& scissorRect);

	static void EndMultiRendering(
		RenderTarget* renderTargets, const size_t& length, RenderingContext& renderContext,
		CD3DX12_VIEWPORT& viewport, CD3DX12_RECT& scissorRect);
};