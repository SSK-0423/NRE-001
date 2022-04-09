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

void RenderTarget::BeginRendering(RenderingContext& renderContext)
{
}

void RenderTarget::EndRendering(RenderingContext& renderContext)
{
}

void RenderTarget::Draw(RenderingContext& renderContext)
{
}
