#include "FBXMesh.h"

using namespace DirectX;

size_t FBXMeshCreateData::GetRenderTargetNum() const
{
	for (size_t idx = 0; idx < colorFormats.size(); idx++)
	{
		if (colorFormats[idx] == DXGI_FORMAT_UNKNOWN)
			return idx;
	}
}

FBXMesh::FBXMesh()
{
}

FBXMesh::~FBXMesh()
{
}

MYRESULT FBXMesh::CreateDescriptorHeap(ID3D12Device& device)
{
	return _descriptorHeap.Create(device);
}

MYRESULT FBXMesh::CreateMaterialConsnantBuffer(ID3D12Device& device)
{
	MYRESULT result = _materialConstantBuffer.Create(device, &_material, sizeof(MaterialBuff));
	if (result == MYRESULT::FAILED) { return MYRESULT::FAILED; }

	_descriptorHeap.RegistConstantBuffer(device, _materialConstantBuffer);

	return MYRESULT::SUCCESS;
}

MYRESULT FBXMesh::LoadFBX(ID3D12Device& device, FBXMeshCreateData& meshCreateData)
{
	// ローダー用意
	_fbxLoader = new FBXLoader();

	// FBX読み込み
	if (!_fbxLoader->Load(meshCreateData.modelPath, _meshDataList))
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
	result = CreateGraphicsPipelineState(device, meshCreateData);
	if (result == MYRESULT::FAILED) { return result; }

	// ディスクリプタヒープ生成
	result = CreateDescriptorHeap(device);
	if (result == MYRESULT::FAILED) { return result; }

	// マテリアル用コンスタントバッファー生成
	result = CreateMaterialConsnantBuffer(device);
	if (result == MYRESULT::FAILED) { return result; }

	return MYRESULT::SUCCESS;
}

void FBXMesh::Draw(RenderingContext& renderContext)
{
	renderContext.SetGraphicsRootSignature(_rootSignature);

	renderContext.SetDescriptorHeap(_descriptorHeap);

	renderContext.SetPipelineState(_graphicsPipelineState);

	// メッシュ数分描画
	for (size_t idx = 0; idx < _meshDataList.size(); idx++) {
		// メッシュごとのマテリアルセット
		SetMaterial(_meshDataList[idx].material);
		renderContext.SetVertexBuffer(0, _vertexBuffers[idx]);
		renderContext.SetIndexBuffer(_indexBuffers[idx]);
		renderContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		renderContext.DrawIndexedInstanced(_indexBuffers[idx].GetIndexNum(), 1);
	}
}

void FBXMesh::SetMaterial(const FBXMaterial& material)
{
	// アンビエントセット
	XMStoreFloat4(&_material.ambient,
		XMVectorSet(material.ambient[0], material.ambient[1], material.ambient[2], material.ambient[3]));
	// ディフューズ
	XMStoreFloat4(&_material.diffuse,
		XMVectorSet(material.diffuse[0], material.diffuse[1], material.diffuse[2], material.diffuse[3]));
	// スペキュラー
	XMStoreFloat4(&_material.specular,
		XMVectorSet(material.specular[0], material.specular[1], material.specular[2], material.specular[3]));
	// アルファ
	_material.alpha = material.alpha;

	_materialConstantBuffer.UpdateData(&_material);
}

void FBXMesh::SetConstantBuffer(ID3D12Device& device, ConstantBuffer& constantBuffer)
{
	_descriptorHeap.RegistConstantBuffer(device, constantBuffer);
}

void FBXMesh::SetTexture(ID3D12Device& device, Texture& texture)
{
	_descriptorHeap.RegistShaderResource(device, texture);
}

MYRESULT FBXMesh::CreateVertexBuffers(ID3D12Device& device)
{
	// メッシュ数分の頂点バッファーメモリ確保
	_vertexBuffers.resize(_meshDataList.size());

	// メッシュ数分の頂点バッファー生成
	for (size_t idx = 0; idx < _meshDataList.size(); idx++) {
		std::vector<FBXMeshVertex>& vertices = _meshDataList[idx].vertices;

		MYRESULT result = _vertexBuffers[idx].Create(
			device, &vertices[0], SizeofVector(vertices), sizeof(FBXMeshVertex));
		if (result == MYRESULT::FAILED) { return result; }
	}

	return MYRESULT::SUCCESS;
}

MYRESULT FBXMesh::CreateIndexBuffers(ID3D12Device& device)
{
	// メッシュ数分のインデックスバッファーメモリ確保
	_indexBuffers.resize(_meshDataList.size());

	// メッシュ数分のインデックスバッファー生成
	for (size_t idx = 0; idx < _meshDataList.size(); idx++) {
		std::vector<unsigned int>& indices = _meshDataList[idx].indices;

		MYRESULT result = _indexBuffers[idx].Create(device, indices);
		if (result == MYRESULT::FAILED) { return result; }
	}

	return MYRESULT::SUCCESS;
}

MYRESULT FBXMesh::CreateGraphicsPipelineState(ID3D12Device& device, FBXMeshCreateData& data)
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