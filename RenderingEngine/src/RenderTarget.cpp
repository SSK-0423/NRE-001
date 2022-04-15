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