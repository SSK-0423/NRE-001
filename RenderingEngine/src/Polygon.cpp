#include "Polygon.h"

MYRESULT Polygon::CreateGraphicsPipelineState()
{
	/// 必要なモノ
	/// 頂点/ピクセルシェーダー
	/// ルートシグネチャ
	/// 頂点レイアウト
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineState = {};

	return MYRESULT::SUCCESS;
}

MYRESULT Polygon::Create(ID3D12Device& device)
{
	return MYRESULT::SUCCESS;
}

void Polygon::Draw(RenderingContext& renderContext)
{
	renderContext.SetVertexBuffer(0, _vertexBuffer);
	renderContext.SetIndexBuffer(_indexBuffer);
	renderContext.SetPipelineState(_graphicsPipelineState);
	renderContext.DrawIndexedInstanced(_indexBuffer.GetIndexNum(), 1);
}
