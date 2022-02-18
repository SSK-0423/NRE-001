#include "FrameBuffer.h"

MYRESULT FrameBuffer::Create(
	ID3D12Device& device, IDXGISwapChain4& swapchain, const RenderTargetInitData& initData)
{
	MYRESULT result;

	// �t���[���o�b�t�@����
	for (size_t idx = 0; idx < _countof(_frameBuffers); idx++) {
		result = _frameBuffers[idx].Create(device, swapchain, initData, idx);
		if (result == MYRESULT::FAILED) { return result; }
	}

	//�t���[���o�b�t�@�p�q�[�v����
	result = _frameRTVHeap.Create(device);
	if (result == MYRESULT::FAILED) { return result; }

	return MYRESULT::SUCCESS;
}