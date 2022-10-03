#pragma once

#include "EngineUtility.h"

namespace NamelessEngine::DX12API
{
	struct RenderTargetData
	{
		RenderTargetBufferData renderTargetBufferData;	// レンダーターゲットバッファー用データ
		DepthStencilBufferData depthStencilBufferData;	// デプスステンシルバッファー用データ
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

		Texture _depthStencilTexture;				// デプスステンシルテクスチャ
		DepthStencilBuffer _depthStencilBuffer;		// デプスステンシルバッファー
		DescriptorHeapDSV _dsvHeap;					// デプスステンシル用ヒープ

		RenderTargetData _renderTargetData;			// レンダーターゲットデータ

	public:
		/// <summary>
		/// レンダーターゲット生成
		/// </summary>
		/// <param name="device">デバイス</param>
		/// <returns></returns>
		Utility::MYRESULT Create(ID3D12Device& device, RenderTargetData& renderTargetData);

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
		/// デプスステンシルのテクスチャ取得
		/// </summary>
		/// <returns>デプスステンシルテクスチャ</returns>
		Texture& GetDepthStencilTexture() {
			return _depthStencilTexture;
		}

		/// <summary>
		/// マルチレンダーターゲットでのレンダリング開始
		/// </summary>
		/// <param name="renderTargets"></param>
		/// <param name="renderContext"></param>
		/// <param name="viewport"></param>
		/// <param name="scissorRect"></param>
		static void BeginMultiRendering(
			RenderTarget* renderTargets, const size_t& length, RenderingContext& renderContext,
			CD3DX12_VIEWPORT& viewport, CD3DX12_RECT& scissorRect);

		/// <summary>
		/// マルチレンダーターゲットでのレンダリング終了
		/// </summary>
		/// <param name="renderTargets"></param>
		/// <param name="length"></param>
		/// <param name="renderContext"></param>
		/// <param name="viewport"></param>
		/// <param name="scissorRect"></param>
		static void EndMultiRendering(
			RenderTarget* renderTargets, const size_t& length, RenderingContext& renderContext,
			CD3DX12_VIEWPORT& viewport, CD3DX12_RECT& scissorRect);
	};
}