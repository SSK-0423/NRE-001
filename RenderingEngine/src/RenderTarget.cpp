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

void RenderTarget::BeginRendering(RenderingContext& renderContext)
{
}

void RenderTarget::EndRendering(RenderingContext& renderContext)
{
}

void RenderTarget::Draw(RenderingContext& renderContext)
{
}
