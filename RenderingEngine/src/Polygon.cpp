#include "Polygon.h"

using namespace MyFrameWork;

MYRESULT Polygon::CreateGraphicsPipelineState(ID3D12Device& device, const PolygonData& data)
{
	/// 必要なモノ
	/// 頂点/ピクセルシェーダー
	/// ルートシグネチャ
	/// 頂点レイアウト

	// ルートシグネチャ生成
	MYRESULT result = _rootSignature.Create(device, data.rootSignatureData);
	if (MYRESULT::FAILED == result) { return result; }
	
	// ルートシグネチャとシェーダーセット
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineState = {};
	pipelineState.pRootSignature = &_rootSignature.GetRootSignature();
	pipelineState.VS = CD3DX12_SHADER_BYTECODE(&data.vertexShader.GetShader());
	pipelineState.PS = CD3DX12_SHADER_BYTECODE(&data.pixelShader.GetShader());

	// サンプルマスク設定
	pipelineState.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// ブレンド
	pipelineState.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

	// ラスタライズ設定
	pipelineState.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	pipelineState.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	// インプットレイアウトの設定
	pipelineState.InputLayout.pInputElementDescs = &data.inputLayout[0];
	pipelineState.InputLayout.NumElements = static_cast<UINT>(data.inputLayout.size());
	pipelineState.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
	pipelineState.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// レンダーターゲットの設定
	pipelineState.NumRenderTargets = data.renderTargetNum;
	for (size_t idx = 0; idx < data.renderTargetNum; idx++)
	{
		pipelineState.RTVFormats[idx] = DXGI_FORMAT_R8G8B8A8_UNORM;
	}

	// アンチエイリアシングのためのサンプル数設定
	pipelineState.SampleDesc.Count = 1;	    // サンプリングは1ピクセルにつき1
	pipelineState.SampleDesc.Quality = 0;	// クオリティは最低

	// グラフィックスパイプラインステート生成
	return _graphicsPipelineState.Create(device, pipelineState);
}

MYRESULT Polygon::Create(ID3D12Device& device, const PolygonData& data)
{
	_vertexBuffer = data.vertexBuffer;
	_indexBuffer = data.indexBuffer;

	return CreateGraphicsPipelineState(device, data);
}

void Polygon::Draw(RenderingContext& renderContext)
{
	renderContext.SetGraphicsRootSignature(_rootSignature);
	
	if(_descriptorHeap != nullptr)
		renderContext.SetDescriptorHeap(*_descriptorHeap);
	
	renderContext.SetPipelineState(_graphicsPipelineState);
	renderContext.SetVertexBuffer(0, _vertexBuffer);
	renderContext.SetIndexBuffer(_indexBuffer);
	renderContext.SetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	renderContext.DrawIndexedInstanced(_indexBuffer.GetIndexNum(), 1);
}

void MyFrameWork::Polygon::SetDescriptorHeap(DescriptorHeapCBV_SRV_UAV& descriptorHeap)
{
	_descriptorHeap = &descriptorHeap;
}
