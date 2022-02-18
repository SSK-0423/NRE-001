#pragma once
#include "RenderTarget.h"
#include "DescriptorHeapRTV.h"
#include <cassert>

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
	/// �f�B�X�N���v�^�[�q�[�v�̐擪�n���h���擾
	/// </summary>
	/// <returns></returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetRtvCpuDescriptorHandleForHeapStart() {
		return _frameRTVHeap.GetCPUDescriptorHandleForHeapStart();
	}

	/// <summary>
	/// �f�B�X�N���v�^�q�[�v�n���h���̃C���N�������g�T�C�Y�擾
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