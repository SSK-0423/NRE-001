#pragma once
#include "RenderTarget.h"
#include "DescriptorHeapRTV.h"

/// <summary>
/// �t���[���o�b�t�@�N���X
/// </summary>
class FrameBuffer
{
public:
	static constexpr UINT BUFFERCOUNT = 2;

	FrameBuffer() = default;
	~FrameBuffer() = default;

private:
	RenderTarget _frameBuffers[BUFFERCOUNT];	// �t���[���o�b�t�@
	DescriptorHeapRTV _frameRTVHeap;			// �t���[���o�b�t�@�p�q�[�v

public:
	/// <summary>
	/// �t���[���o�b�t�@����
	/// </summary>
	/// <returns>MYRESULT::SUCCESS: ���� MYRESULT::FAILED: ���s</returns>
	MYRESULT Create(ID3D12Device& device, IDXGISwapChain4& swapchain, const RenderTargetInitData& initData);

	/// <summary>
	/// �t���[���o�b�t�@�p�q�[�v�擾
	/// </summary>
	/// <returns></returns>
	DescriptorHeapRTV& GetDescriptorHeap() {
		return _frameRTVHeap;
	}
};