#include "FrameBuffer.h"

MYRESULT FrameBuffer::Create(
	ID3D12Device& device, IDXGISwapChain4& swapchain, const RenderTargetInitData& initData)
{
	MYRESULT result;

	// フレームバッファ生成
	for (size_t idx = 0; idx < _countof(_frameBuffers); idx++) {
		result = _frameBuffers[idx].Create(device, swapchain, initData, idx);
		if (result == MYRESULT::FAILED) { return result; }
	}

	//フレームバッファ用ヒープ生成
	result = _frameRTVHeap.Create(device);
	if (result == MYRESULT::FAILED) { return result; }

	return MYRESULT::SUCCESS;
}