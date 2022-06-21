#include "Mesh.h"
#include "IMeshLoader.h"

size_t MeshData::GetRenderTargetNum() const
{
	for (size_t idx = 0; idx < colorFormats.size(); idx++)
	{
		if (colorFormats[idx] == DXGI_FORMAT_UNKNOWN)
			return idx;
	}
}

Mesh::Mesh() : _loader(nullptr)
{
}

Mesh::~Mesh()
{
	delete _loader;
}

MYRESULT Mesh::LoadMesh(const char* meshPath, ID3D12Device& device, MeshData& meshData)
{
	// ローダー用意
	_loader = new FBXLoader();

	// FBX読み込み
	if (!_loader->Load(meshPath))
	{
		return MYRESULT::FAILED;
	}

	// 頂点バッファ―生成
	MYRESULT result = CreateVertexBuffers(device);
	if (result == MYRESULT::FAILED) { return result; }

	// インデックスバッファー生成
	result = CreateIndexBuffers(device);
	if (result == MYRESULT::FAILED) { return result; }

	// グラフィクスパイプラインステート生成
	result = CreateGraphicsPipelineState(device, meshData);
	if (result == MYRESULT::FAILED) { return result; }

	return MYRESULT::SUCCESS;
}

void Mesh::Draw(RenderingContext& renderContext)
{
	renderContext.SetGraphicsRootSignature(_rootSignature);

	if (_descriptorHeap != nullptr)
	    renderContext.SetDescriptorHeap(*_descriptorHeap);

	renderContext.SetPipelineState(_graphicsPipelineState);
	renderContext.SetVertexBuffer(0, _vertexBuffer);
	renderContext.SetIndexBuffer(_indexBuffer);
	renderContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	renderContext.DrawIndexedInstanced(_indexBuffer.GetIndexNum(), 1);
}

void Mesh::SetDescriptorHeap(DescriptorHeapCBV_SRV_UAV& descriptorHeap)
{
	_descriptorHeap = &descriptorHeap;
}

MYRESULT Mesh::CreateVertexBuffers(ID3D12Device& device)
{
	std::vector<MeshVertex>& vertices = _loader->GetVertices();
	MYRESULT result = _vertexBuffer.Create(
		device, &vertices[0], SizeofVector(vertices), sizeof(MeshVertex));
	if (result == MYRESULT::FAILED) { return result; }

	return MYRESULT::SUCCESS;
}

MYRESULT Mesh::CreateIndexBuffers(ID3D12Device& device)
{
	std::vector<unsigned int>& indices = _loader->GetIndices();
	MYRESULT result = _indexBuffer.Create(device, indices);
	if (result == MYRESULT::FAILED) { return result; }

	return MYRESULT::SUCCESS;
}

MYRESULT Mesh::CreateGraphicsPipelineState(ID3D12Device& device, MeshData& data)
{
	// ルートシグネチャ生成
	MYRESULT result = _rootSignature.Create(device, data.rootSignatureData);
	if (result == MYRESULT::FAILED) { return result; }

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

	// デプスステンシル設定
	pipelineState.DepthStencilState.DepthEnable = true;
	pipelineState.DepthStencilState.StencilEnable = false;
	pipelineState.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	pipelineState.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	pipelineState.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// レンダーターゲットの設定
	pipelineState.NumRenderTargets = data.GetRenderTargetNum();

	for (size_t idx = 0; idx < pipelineState.NumRenderTargets; idx++)
	{
		pipelineState.RTVFormats[idx] = data.colorFormats[idx];
	}

	// アンチエイリアシングのためのサンプル数設定
	pipelineState.SampleDesc.Count = 1;	    // サンプリングは1ピクセルにつき1
	pipelineState.SampleDesc.Quality = 0;	// クオリティは最低

	// グラフィックスパイプラインステート生成
	return _graphicsPipelineState.Create(device, pipelineState);
}

//
// 実装案
// 1. Mesh側でバッファー生成
// メリット
// ・FBXLoaderがVertexBuffer/IndexBufferクラスに依存しなくなるので、独立性が増して色々な実装に対応できる
// デメリット
// ・Mesh側のFBXLoaderへの依存が強くなる
// ・他形式のファイルを読み込む際に対応できない
// 2. FBXLoader側でバッファー生成
// メリット
// ・メッシュ側の処理が簡素になる
// ・他形式を読み込むローダークラスへの切り替えが容易
// デメリット
// ・VertexBuffer/IndexBufferクラスに依存する形になる
// ・独立性が損なわれ、部品としての使いまわしがしづらくなる
//