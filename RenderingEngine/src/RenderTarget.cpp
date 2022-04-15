#include "RenderTarget.h"

MYRESULT RenderTarget::Create(ID3D12Device& device, RenderTargetData& renderTargetData)
{
	_renderTargetData = renderTargetData;

	// �����_�[�^�[�Q�b�g����
	MYRESULT result = _renderTargetBuffer.Create(device, renderTargetData.renderTargetBufferData);
	if (result == MYRESULT::FAILED) { return result; }

	// �����_�[�^�[�Q�b�g�q�[�v����
	result = _rtvHeap.Create(device);
	if (result == MYRESULT::FAILED) { return result; }

	// �����_�[�^�[�Q�b�g�r���[����
	_rtvHeap.RegistDescriptor(device, _renderTargetBuffer);

	// �I�t�X�N���[���e�N�X�`���o�b�t�@�[����
	_renderTargetTexture.CreateTextureFromRenderTarget(_renderTargetBuffer);

	// �I�t�X�N���[���e�N�X�`���p�q�[�v����
	result = _textureHeap.Create(device);
	if (result == MYRESULT::FAILED) { return result; }

	// �e�N�X�`���Ƃ��ēo�^
	_textureHeap.RegistShaderResource(device, _renderTargetTexture);

	return MYRESULT::SUCCESS;
}

void RenderTarget::BeginRendering(RenderingContext& renderContext, CD3DX12_VIEWPORT& viewport, CD3DX12_RECT& scissorRect)
{
	// �����_�[�^�[�Q�b�g�Ɉڍs
	renderContext.TransitionResourceState(
		_renderTargetBuffer.GetBuffer(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_STATE_RENDER_TARGET);

	// �����_�[�^�[�Q�b�g�Z�b�g
	auto rtvHandle = _rtvHeap.GetCPUDescriptorHandleForHeapStart();
	renderContext.SetRenderTarget(&rtvHandle, nullptr);

	// ��ʂ��w��F�ŃN���A
	renderContext.ClearRenderTarget(rtvHandle, _renderTargetData.renderTargetBufferData.clearColor, 0, nullptr);

	// �r���[�|�[�g�A�V�U�[��`�Z�b�g
	renderContext.SetViewport(viewport);
	renderContext.SetScissorRect(scissorRect);
}

void RenderTarget::EndRendering(RenderingContext& renderContext)
{
	// �s�N�Z���V�F�[�_�[���\�[�X�ֈڍs
	renderContext.TransitionResourceState(
		_renderTargetBuffer.GetBuffer(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}