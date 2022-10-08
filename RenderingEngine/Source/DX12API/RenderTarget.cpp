#include "RenderTarget.h"

#include "DescriptorHeapRTV.h"
#include "DescriptorHeapCBV_SRV_UAV.h"
#include "RenderTargetBuffer.h"

#include "DescriptorHeapDSV.h"
#include "DepthStencilBuffer.h"

#include "Texture.h"
#include "ShaderResourceViewDesc.h"

using namespace NamelessEngine::Utility;

namespace NamelessEngine::DX12API
{
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

		// �f�v�X�X�e���V���o�b�t�@�[����
		result = _depthStencilBuffer.Create(device, renderTargetData.depthStencilBufferData);
		if (result == MYRESULT::FAILED) { return result; }

		// �f�v�X�X�e���V���p�f�B�X�N���v�^�q�[�v����
		result = _dsvHeap.Create(device);
		if (result == MYRESULT::FAILED) { return result; }

		// �f�v�X�X�e���V���r���[����
		_dsvHeap.RegistDescriptor(device, _depthStencilBuffer);

		// �I�t�X�N���[���e�N�X�`���o�b�t�@�[����
		_renderTargetTexture.CreateTextureFromRenderTarget(_renderTargetBuffer);

		// �f�v�X�X�e���V���e�N�X�`���o�b�t�@�[����
		_depthStencilTexture.CreateTextureFromDepthStencil(_depthStencilBuffer);

		// �e�N�X�`���p�q�[�v����
		result = _textureHeap.Create(device);
		if (result == MYRESULT::FAILED) { return result; }

		// �e�N�X�`���Ƃ��ēo�^
		ShaderResourceViewDesc renderDesc(_renderTargetTexture);
		ShaderResourceViewDesc depthDesc(_depthStencilTexture);

		_textureHeap.RegistShaderResource(device, _renderTargetTexture, renderDesc);
		_textureHeap.RegistShaderResource(device, _depthStencilTexture, depthDesc);

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

		// �[�x�o�b�t�@�[
		auto dsvHandle = _dsvHeap.GetCPUDescriptorHandleForHeapStart();

		renderContext.SetRenderTarget(&rtvHandle, &dsvHandle);

		// ��ʂ��w��F�ŃN���A
		renderContext.ClearRenderTarget(rtvHandle, _renderTargetData.renderTargetBufferData.clearColor, 0, nullptr);

		// �f�v�X�X�e���V���o�b�t�@�[���N���A
		renderContext.ClearDepthStencilView(
			dsvHandle, D3D12_CLEAR_FLAG_DEPTH,
			_renderTargetData.depthStencilBufferData.clearDepth,
			_renderTargetData.depthStencilBufferData.clearStencil, 0, nullptr);

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

	void RenderTarget::BeginMultiRendering(
		RenderTarget* renderTargets, const size_t& length, RenderingContext& renderContext,
		CD3DX12_VIEWPORT& viewport, CD3DX12_RECT& scissorRect)
	{
		// �n���h��
		D3D12_CPU_DESCRIPTOR_HANDLE* rtvHandles = new D3D12_CPU_DESCRIPTOR_HANDLE[length];
		D3D12_CPU_DESCRIPTOR_HANDLE* dsvHandles = new D3D12_CPU_DESCRIPTOR_HANDLE[length];

		for (size_t idx = 0; idx < length; idx++)
		{
			// �����_�[�^�[�Q�b�g�Ɉڍs
			renderContext.TransitionResourceState(
				renderTargets[idx]._renderTargetBuffer.GetBuffer(),
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				D3D12_RESOURCE_STATE_RENDER_TARGET);

			rtvHandles[idx] = renderTargets[idx]._rtvHeap.GetCPUDescriptorHandleForHeapStart();
			dsvHandles[idx] = renderTargets[idx]._dsvHeap.GetCPUDescriptorHandleForHeapStart();

			// ��ʂ��w��F�ŃN���A
			renderContext.ClearRenderTarget(
				rtvHandles[idx], renderTargets[idx]._renderTargetData.renderTargetBufferData.clearColor, 0, nullptr);

			// �f�v�X�X�e���V���o�b�t�@�[���N���A
			renderContext.ClearDepthStencilView(
				dsvHandles[idx], D3D12_CLEAR_FLAG_DEPTH,
				renderTargets[idx]._renderTargetData.depthStencilBufferData.clearDepth,
				renderTargets[idx]._renderTargetData.depthStencilBufferData.clearStencil, 0, nullptr);
		}

		// �����_�[�^�[�Q�b�g�Z�b�g
		renderContext.SetRenderTargets(length, rtvHandles, &dsvHandles[0]);

		// �r���[�|�[�g�A�V�U�[��`�Z�b�g
		renderContext.SetViewport(viewport);
		renderContext.SetScissorRect(scissorRect);

	}

	void RenderTarget::EndMultiRendering(RenderTarget* renderTargets, const size_t& length, RenderingContext& renderContext, CD3DX12_VIEWPORT& viewport, CD3DX12_RECT& scissorRect)
	{
		for (size_t idx = 0; idx < length; idx++)
		{
			// �s�N�Z���V�F�[�_�[���\�[�X�ֈڍs
			renderContext.TransitionResourceState(
				renderTargets[idx]._renderTargetBuffer.GetBuffer(),
				D3D12_RESOURCE_STATE_RENDER_TARGET,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		}
	}

}