#pragma once
#include "RenderTarget.h"
#include "DescriptorHeapRTV.h"
#include <cassert>

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
	/// ディスクリプターヒープの先頭ハンドル取得
	/// </summary>
	/// <returns></returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetRtvCpuDescriptorHandleForHeapStart() {
		return _frameRTVHeap.GetCPUDescriptorHandleForHeapStart();
	}

	/// <summary>
	/// ディスクリプタヒープハンドルのインクリメントサイズ取得
	/// </summary>
	/// <returns></returns>
	UINT GetHandleIncrimentSize() {
		return _frameRTVHeap.GetHandleIncrimentSize();
	}
	
	ID3D12Resource& GetRengerTargetBuffer(size_t index) {
		assert(index < BUFFERCOUNT);
		return _frameBuffers[index].GetRenderTargetBuffer();
	}

	ID3D12Resource& GetDepthBuffer(size_t index) {
		assert(index < BUFFERCOUNT);
		return _frameBuffers[index].GetDepthBuffer();
	}
};