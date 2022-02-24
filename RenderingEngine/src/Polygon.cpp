#include "Polygon.h"

using namespace MyFrameWork;

MYRESULT Polygon::CreateGraphicsPipelineState(ID3D12Device& device, const PolygonData& data)
{
	/// 必要なモノ
	/// 頂点/ピクセルシェーダー
	/// ルートシグネチャ
	/// 頂点レイアウト
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineState = {};
	pipelineState.pRootSignature = &data._rootSignature.GetRootSignature();
	pipelineState.VS = CD3DX12_SHADER_BYTECODE(&data._vertexShader.GetShader());
	pipelineState.PS = CD3DX12_SHADER_BYTECODE(&data._pixelShader.GetShader());

	// サンプルマスク設定
	pipelineState.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// ブレンド
	pipelineState.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

	// ラスタライズ設定
	pipelineState.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	pipelineState.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	// インプットレイアウトの設定
	pipelineState.InputLayout.pInputElementDescs = &data._inputLayout[0];
	pipelineState.InputLayout.NumElements = static_cast<UINT>(data._inputLayout.size());
	pipelineState.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
	pipelineState.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// レンダーターゲットの設定
	pipelineState.NumRenderTargets = 1;
	pipelineState.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

	// アンチエイリアシングのためのサンプル数設定
	pipelineState.SampleDesc.Count = 1;	    // サンプリングは1ピクセルにつき1
	pipelineState.SampleDesc.Quality = 0;	// クオリティは最低

	// グラフィックスパイプラインステート生成
	return _graphicsPipelineState.Create(device, pipelineState);
}

MYRESULT Polygon::Create(ID3D12Device& device, const PolygonData& data)
{
	_vertexBuffer = data._vertexBuffer;
	_indexBuffer = data._indexBuffer;
	
	return CreateGraphicsPipelineState(device, data);
}

void Polygon::Draw(RenderingContext& renderContext)
{
	renderContext.SetPipelineState(_graphicsPipelineState);
	renderContext.SetVertexBuffer(0, _vertexBuffer);
	renderContext.SetIndexBuffer(_indexBuffer);
	renderContext.SetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	renderContext.DrawIndexedInstanced(_indexBuffer.GetIndexNum(), 1);
}
