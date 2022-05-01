#pragma once

#include "DescriptorHeapRTV.h"
#include "DescriptorHeapCBV_SRV_UAV.h"
#include "RenderTargetBuffer.h"

#include "Texture.h"

#include "EngineUtility.h"

struct RenderTargetData
{
	RenderTargetBufferData renderTargetBufferData;	// �����_�[�^�[�Q�b�g�o�b�t�@�[�p�f�[�^
};

/// <summary>
/// �����_�[�^�[�Q�b�g�N���X
/// </summary>
class RenderTarget
{
private:
	Texture _renderTargetTexture;				// �I�t�X�N���[���e�N�X�`��
	DescriptorHeapCBV_SRV_UAV _textureHeap;		// �I�t�X�N���[���e�N�X�`���p�q�[�v

	RenderTargetBuffer _renderTargetBuffer;		// �����_�[�^�[�Q�b�g�o�b�t�@�[
	DescriptorHeapRTV _rtvHeap;					// �����_�[�^�[�Q�b�g�p�q�[�v

	RenderTargetData _renderTargetData;			// �����_�[�^�[�Q�b�g�f�[�^

public:
	/// <summary>
	/// �����_�[�^�[�Q�b�g����
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	/// <returns></returns>
	MYRESULT Create(ID3D12Device& device, RenderTargetData& renderTargetData);

	/// <summary>
	/// �����_�����O�J�n
	/// </summary>
	/// <param name="renderContext">�����_�����O�R���e�L�X�g</param>
	void BeginRendering(RenderingContext& renderContext, CD3DX12_VIEWPORT& viewport, CD3DX12_RECT& scissorRect);

	/// <summary>
	/// �����_�����O�I��
	/// </summary>
	/// <param name="renderContext">�����_�����O�R���e�L�X�g</param>
	void EndRendering(RenderingContext& renderContext);

	/// <summary>
	/// �����_�[�^�[�Q�b�g�̃e�N�X�`���擾
	/// </summary>
	/// <returns>�����_�[�^�[�Q�b�g�e�N�X�`��</returns>
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