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

		// レンダーターゲット生成
		MYRESULT result = _renderTargetBuffer.Create(device, renderTargetData.renderTargetBufferData);
		if (result == MYRESULT::FAILED) { return result; }

		// レンダーターゲットヒープ生成
		result = _rtvHeap.Create(device);
		if (result == MYRESULT::FAILED) { return result; }

		// レンダーターゲットビュー生成
		_rtvHeap.RegistDescriptor(device, _renderTargetBuffer);

		// デプスステンシルバッファー生成
		result = _depthStencilBuffer.Create(device, renderTargetData.depthStencilBufferData);
		if (result == MYRESULT::FAILED) { return result; }

		// デプスステンシル用ディスクリプタヒープ生成
		result = _dsvHeap.Create(device);
		if (result == MYRESULT::FAILED) { return result; }

		// デプスステンシルビュー生成
		_dsvHeap.RegistDescriptor(device, _depthStencilBuffer);

		// オフスクリーンテクスチャバッファー生成
		_renderTargetTexture.CreateTextureFromRenderTarget(_renderTargetBuffer);

		// デプスステンシルテクスチャバッファー生成
		_depthStencilTexture.CreateTextureFromDepthStencil(_depthStencilBuffer);

		// テクスチャ用ヒープ生成
		result = _textureHeap.Create(device);
		if (result == MYRESULT::FAILED) { return result; }

		// テクスチャとして登録
		ShaderResourceViewDesc renderDesc(_renderTargetTexture);
		ShaderResourceViewDesc depthDesc(_depthStencilTexture);

		_textureHeap.RegistShaderResource(device, _renderTargetTexture, renderDesc);
		_textureHeap.RegistShaderResource(device, _depthStencilTexture, depthDesc);

		return MYRESULT::SUCCESS;
	}

	void RenderTarget::BeginRendering(RenderingContext& renderContext, CD3DX12_VIEWPORT& viewport, CD3DX12_RECT& scissorRect)
	{
		// レンダーターゲットに移行
		renderContext.TransitionResourceState(
			_renderTargetBuffer.GetBuffer(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			D3D12_RESOURCE_STATE_RENDER_TARGET);

		// レンダーターゲットセット
		auto rtvHandle = _rtvHeap.GetCPUDescriptorHandleForHeapStart();

		// 深度バッファー
		auto dsvHandle = _dsvHeap.GetCPUDescriptorHandleForHeapStart();

		renderContext.SetRenderTarget(&rtvHandle, &dsvHandle);

		// 画面を指定色でクリア
		renderContext.ClearRenderTarget(rtvHandle, _renderTargetData.renderTargetBufferData.clearColor, 0, nullptr);

		// デプスステンシルバッファーをクリア
		renderContext.ClearDepthStencilView(
			dsvHandle, D3D12_CLEAR_FLAG_DEPTH,
			_renderTargetData.depthStencilBufferData.clearDepth,
			_renderTargetData.depthStencilBufferData.clearStencil, 0, nullptr);

		// ビューポート、シザー矩形セット
		renderContext.SetViewport(viewport);
		renderContext.SetScissorRect(scissorRect);
	}

	void RenderTarget::EndRendering(RenderingContext& renderContext)
	{
		// ピクセルシェーダーリソースへ移行
		renderContext.TransitionResourceState(
			_renderTargetBuffer.GetBuffer(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	}

	void RenderTarget::BeginMultiRendering(
		RenderTarget* renderTargets, const size_t& length, RenderingContext& renderContext,
		CD3DX12_VIEWPORT& viewport, CD3DX12_RECT& scissorRect)
	{
		// ハンドル
		D3D12_CPU_DESCRIPTOR_HANDLE* rtvHandles = new D3D12_CPU_DESCRIPTOR_HANDLE[length];
		D3D12_CPU_DESCRIPTOR_HANDLE* dsvHandles = new D3D12_CPU_DESCRIPTOR_HANDLE[length];

		for (size_t idx = 0; idx < length; idx++)
		{
			// レンダーターゲットに移行
			renderContext.TransitionResourceState(
				renderTargets[idx]._renderTargetBuffer.GetBuffer(),
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				D3D12_RESOURCE_STATE_RENDER_TARGET);

			rtvHandles[idx] = renderTargets[idx]._rtvHeap.GetCPUDescriptorHandleForHeapStart();
			dsvHandles[idx] = renderTargets[idx]._dsvHeap.GetCPUDescriptorHandleForHeapStart();

			// 画面を指定色でクリア
			renderContext.ClearRenderTarget(
				rtvHandles[idx], renderTargets[idx]._renderTargetData.renderTargetBufferData.clearColor, 0, nullptr);

			// デプスステンシルバッファーをクリア
			renderContext.ClearDepthStencilView(
				dsvHandles[idx], D3D12_CLEAR_FLAG_DEPTH,
				renderTargets[idx]._renderTargetData.depthStencilBufferData.clearDepth,
				renderTargets[idx]._renderTargetData.depthStencilBufferData.clearStencil, 0, nullptr);
		}

		// レンダーターゲットセット
		renderContext.SetRenderTargets(length, rtvHandles, &dsvHandles[0]);

		// ビューポート、シザー矩形セット
		renderContext.SetViewport(viewport);
		renderContext.SetScissorRect(scissorRect);

	}

	void RenderTarget::EndMultiRendering(RenderTarget* renderTargets, const size_t& length, RenderingContext& renderContext, CD3DX12_VIEWPORT& viewport, CD3DX12_RECT& scissorRect)
	{
		for (size_t idx = 0; idx < length; idx++)
		{
			// ピクセルシェーダーリソースへ移行
			renderContext.TransitionResourceState(
				renderTargets[idx]._renderTargetBuffer.GetBuffer(),
				D3D12_RESOURCE_STATE_RENDER_TARGET,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		}
	}

}