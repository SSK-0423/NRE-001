#include "RenderTarget.h"

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

	// オフスクリーンテクスチャバッファー生成
	_renderTargetTexture.CreateTextureFromRenderTarget(_renderTargetBuffer);

	// オフスクリーンテクスチャ用ヒープ生成
	result = _textureHeap.Create(device);
	if (result == MYRESULT::FAILED) { return result; }

	// テクスチャとして登録
	_textureHeap.RegistShaderResource(device, _renderTargetTexture);

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
	renderContext.SetRenderTarget(&rtvHandle, nullptr);

	// 画面を指定色でクリア
	renderContext.ClearRenderTarget(rtvHandle, _renderTargetData.renderTargetBufferData.clearColor, 0, nullptr);

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
	}

	// レンダーターゲットセット
	renderContext.SetRenderTargets(length, rtvHandles, nullptr);

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
