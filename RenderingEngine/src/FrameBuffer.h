#pragma once
#include "RenderTarget.h"
#include "DescriptorHeapRTV.h"

/// <summary>
/// フレームバッファクラス
/// </summary>
class FrameBuffer
{
public:
	static constexpr UINT BUFFERCOUNT = 2;

	FrameBuffer() = default;
	~FrameBuffer() = default;

private:
	RenderTarget _frameBuffers[BUFFERCOUNT];	// フレームバッファ
	DescriptorHeapRTV _frameRTVHeap;			// フレームバッファ用ヒープ

public:
	/// <summary>
	/// フレームバッファ生成
	/// </summary>
	/// <returns>MYRESULT::SUCCESS: 成功 MYRESULT::FAILED: 失敗</returns>
	MYRESULT Create(ID3D12Device& device, IDXGISwapChain4& swapchain, const RenderTargetInitData& initData);

	/// <summary>
	/// フレームバッファ用ヒープ取得
	/// </summary>
	/// <returns></returns>
	DescriptorHeapRTV& GetDescriptorHeap() {
		return _frameRTVHeap;
	}
};