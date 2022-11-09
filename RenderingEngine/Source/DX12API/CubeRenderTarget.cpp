#include "CubeRenderTarget.h"

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
	RESULT CubeRenderTarget::Create(ID3D12Device& device, CubeRenderTargetData& renderTargetData)
	{
		_renderTargetData = renderTargetData;

		// レンダーターゲット生成
		RESULT result = _renderTargetBuffer.Create(device, renderTargetData.renderTargetBufferData);
		if (result == RESULT::FAILED) { return result; }

		// レンダーターゲットヒープ生成
		result = _rtvHeap.Create(device);
		if (result == RESULT::FAILED) { return result; }

		// レンダーターゲットビュー生成
		_rtvHeap.RegistDescriptor(
			device, _renderTargetBuffer, _renderTargetData.renderTargetBufferData.colorFormat);

		// オフスクリーンテクスチャバッファー生成
		_renderTargetTexture.CreateTextureFromRenderTarget(_renderTargetBuffer);

		// テクスチャ用ヒープ生成
		result = _textureHeap.Create(device);
		if (result == RESULT::FAILED) { return result; }

		// テクスチャとして登録
		ShaderResourceViewDesc renderDesc(_renderTargetTexture);
		_textureHeap.RegistShaderResource(device, _renderTargetTexture, renderDesc);

		// デプスステンシルバッファー生成
		result = _depthStencilBuffer.Create(device, renderTargetData.depthStencilBufferData);
		if (result == RESULT::FAILED) { return result; }

		// デプスステンシル用ディスクリプタヒープ生成
		result = _dsvHeap.Create(device);
		if (result == RESULT::FAILED) { return result; }

		// デプスステンシルビュー生成
		_dsvHeap.RegistDescriptor(device, _depthStencilBuffer);

		// デプスステンシルテクスチャバッファー生成
		_depthStencilTexture.CreateTextureFromDepthStencil(_depthStencilBuffer);

		// テクスチャとして登録
		ShaderResourceViewDesc depthDesc(_depthStencilTexture);
		_textureHeap.RegistShaderResource(device, _depthStencilTexture, depthDesc);

		return RESULT::SUCCESS;
	}

	void CubeRenderTarget::BeginRendering(RenderingContext& renderContext, CD3DX12_VIEWPORT& viewport, CD3DX12_RECT& scissorRect)
	{
		// レンダーターゲットに移行
		renderContext.TransitionResourceState(
			_renderTargetBuffer.GetBuffer(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			D3D12_RESOURCE_STATE_RENDER_TARGET);

		// レンダーターゲットセット
		auto rtvHandle = _rtvHeap.GetCPUDescriptorHandleForHeapStart();

		if (_renderTargetData.useDepth) {

			// 深度バッファー
			auto dsvHandle = _dsvHeap.GetCPUDescriptorHandleForHeapStart();

			renderContext.SetRenderTarget(&rtvHandle, &dsvHandle);

			// デプスステンシルバッファーをクリア
			renderContext.ClearDepthStencilView(
				dsvHandle, D3D12_CLEAR_FLAG_DEPTH,
				_renderTargetData.depthStencilBufferData.clearDepth,
				_renderTargetData.depthStencilBufferData.clearStencil, 0, nullptr);
		}
		else {
			renderContext.SetRenderTarget(&rtvHandle, nullptr);
		}

		// 画面を指定色でクリア
		renderContext.ClearRenderTarget(rtvHandle, _renderTargetData.renderTargetBufferData.clearColor, 0, nullptr);

		// ビューポート、シザー矩形セット
		renderContext.SetViewport(viewport);
		renderContext.SetScissorRect(scissorRect);
	}

	void CubeRenderTarget::EndRendering(RenderingContext& renderContext)
	{
		// ピクセルシェーダーリソースへ移行
		renderContext.TransitionResourceState(
			_renderTargetBuffer.GetBuffer(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	}

	void CubeRenderTarget::BeginMultiRendering(
		CubeRenderTarget* renderTargets, const size_t& length, RenderingContext& renderContext,
		CD3DX12_VIEWPORT& viewport, CD3DX12_RECT& scissorRect)
	{
		// ハンドル
		D3D12_CPU_DESCRIPTOR_HANDLE* rtvHandles = new D3D12_CPU_DESCRIPTOR_HANDLE[length];
		D3D12_CPU_DESCRIPTOR_HANDLE* dsvHandle = nullptr;

		for (size_t idx = 0; idx < length; idx++)
		{
			// レンダーターゲットに移行
			renderContext.TransitionResourceState(
				renderTargets[idx]._renderTargetBuffer.GetBuffer(),
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				D3D12_RESOURCE_STATE_RENDER_TARGET);

			rtvHandles[idx] = renderTargets[idx]._rtvHeap.GetCPUDescriptorHandleForHeapStart();

			// 画面を指定色でクリア
			renderContext.ClearRenderTarget(
				rtvHandles[idx], renderTargets[idx]._renderTargetData.renderTargetBufferData.clearColor, 0, nullptr);

			// デプスステンシルは1つしかセットできないので、最初に見つかったものを使用する
			if (renderTargets[idx]._renderTargetData.useDepth && dsvHandle == nullptr) {
				dsvHandle = new D3D12_CPU_DESCRIPTOR_HANDLE();
				auto handle = renderTargets[idx]._dsvHeap.GetCPUDescriptorHandleForHeapStart();
				// GetCPUDescriptorHandleForHeapStartの戻り値が値渡しなので、
				// スコープを抜ける際にdsvHandleがnullとなるのを防ぐためにコピーする
				memcpy_s(
					dsvHandle,
					sizeof(D3D12_CPU_DESCRIPTOR_HANDLE),
					&handle,
					sizeof(D3D12_CPU_DESCRIPTOR_HANDLE));

				// デプスステンシルバッファーをクリア
				renderContext.ClearDepthStencilView(
					*dsvHandle, D3D12_CLEAR_FLAG_DEPTH,
					renderTargets[idx]._renderTargetData.depthStencilBufferData.clearDepth,
					renderTargets[idx]._renderTargetData.depthStencilBufferData.clearStencil, 0, nullptr);
			}
		}

		// レンダーターゲットセット
		renderContext.SetRenderTargets(length, rtvHandles, dsvHandle);

		// ビューポート、シザー矩形セット
		renderContext.SetViewport(viewport);
		renderContext.SetScissorRect(scissorRect);
	}

	void CubeRenderTarget::EndMultiRendering(CubeRenderTarget* renderTargets, const size_t& length, RenderingContext& renderContext)
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