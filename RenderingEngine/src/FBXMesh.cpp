#include "FBXMesh.h"
#include "ShaderResourceViewDesc.h"

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
	SafetyDelete<VertexBuffer>(_vertexBuffer);
	SafetyDelete<IndexBuffer>(_indexBuffer);
	SafetyDelete<GraphicsPipelineState>(_graphicsPipelineState);
	SafetyDelete<RootSignature>(_rootSignature);
}

MYRESULT FBXMesh::CreateDescriptorHeap(ID3D12Device& device)
{
	if (_descriptorHeap != nullptr) { delete _descriptorHeap; }

	_descriptorHeap = new DescriptorHeapCBV_SRV_UAV();
	return _descriptorHeap->Create(device);
}

void FBXMesh::SetMaterial(IMaterial& material)
{
	// ここでマテリアルを適用する
	// ・フォンモデルのコンスタントバッファー登録
	// ・テクスチャ登録など
	material.ApplyMaterial(*this);
}

MYRESULT FBXMesh::CreateAsChild(Dx12GraphicsEngine& graphicsEngine, FBXMeshData& meshData)
{
	ID3D12Device& device = graphicsEngine.Device();

	// 頂点バッファ―生成
	MYRESULT result = CreateVertexBuffer(device, meshData);
	if (result == MYRESULT::FAILED) { return result; }

	// インデックスバッファー生成
	result = CreateIndexBuffer(device, meshData);
	if (result == MYRESULT::FAILED) { return result; }

	// ディスクリプタヒープ生成
	result = CreateDescriptorHeap(device);
	if (result == MYRESULT::FAILED) { return result; }

	// マテリアルセット
	SetMaterial(*meshData.material);

	return MYRESULT::SUCCESS;
}

MYRESULT FBXMesh::LoadFBX(Dx12GraphicsEngine& graphicsEngine, FBXMeshCreateData& meshCreateData)
{
	// ローダー用意
	FBXLoader* _fbxLoader = new FBXLoader();

	FBXLoadData loadData;
	loadData.meshPath = meshCreateData.modelPath;
	loadData.materialType = meshCreateData.materialType;
	loadData.textureFolderPath = meshCreateData.textureFolderPath;
	loadData.baseColorName = meshCreateData.baseColorName;
	loadData.metallicName = meshCreateData.metallicName;
	loadData.roughnessName = meshCreateData.roughnessName;
	loadData.normalName = meshCreateData.normalName;
	loadData.occlusionName = meshCreateData.occlusionName;

	// FBX読み込み
	if (!_fbxLoader->Load(_meshDataList, loadData))
	{
		return MYRESULT::FAILED;
	}

	// グラフィクスパイプラインステート生成
	MYRESULT result = CreateGraphicsPipelineState(graphicsEngine.Device(), meshCreateData);
	if (result == MYRESULT::FAILED) { return result; }
	// ディスクリプタヒープ生成
	result = CreateDescriptorHeap(graphicsEngine.Device());
	if (result == MYRESULT::FAILED) { return result; }

	// マテリアルを分けるためにマテリアル毎にFBXMeshを生成
	_childs.resize(_meshDataList.size());
	for (size_t idx = 0; idx < _childs.size(); idx++) {

		FBXMesh* child = new FBXMesh();
		result = child->CreateAsChild(graphicsEngine, _meshDataList[idx]);
		if (result == MYRESULT::FAILED) { return result; }

		_childs[idx] = child;
	}

	delete _fbxLoader;

	return MYRESULT::SUCCESS;
}

void FBXMesh::CommonDraw(RenderingContext& renderContext)
{
	renderContext.SetGraphicsRootSignature(*_rootSignature);

	renderContext.SetPipelineState(*_graphicsPipelineState);
}

void FBXMesh::Draw(RenderingContext& renderContext)
{
	Draw(renderContext, true);
	/* マテリアル切り替え */
	//
	// 方針：b1に入れるコンスタントバッファービューを切り替え
	// 現状：CBVのディスクリプタテーブルは１つのみ
	// b0の値は不変、b1の値のみ変更したい
	// 実装案1：FBXMesh用のルートシグネチャを生成する
	// メリット
	// ・シャドウマップ実装時にシャドウマップ用のサンプラーが必要になる
	// ・ルートシグネチャが
	// デメリット
	// ・初期化に使う構造体の中身がPolygonやSpriteと異なり、ルートシグネチャ系のクラスがもう一つ増えることになる
	// ディファードレンダリングやったことなくてわかんないので、とりあえずこっち
	// 実装案2：メッシュごとにFBXMeshを生成し、マテリアル部分のみを変更する
	// メリット
	// ・階層メッシュアニメ―ションにも対応できそう
	// 
	// 
	// 
	// 
	// ルートパラメータの設定で、b0,b1が用意されている
	// ディスクリプタヒープにはCBVが64個ある
	// 1. マテリアル数分のCBVを生成する
	//
}

void FBXMesh::Draw(RenderingContext& renderContext, bool isRootMesh)
{
	if (isRootMesh) {
		CommonDraw(renderContext);
	}
	else {
		renderContext.SetDescriptorHeap(*_descriptorHeap);
		renderContext.SetVertexBuffer(0, *_vertexBuffer);
		renderContext.SetIndexBuffer(*_indexBuffer);
		renderContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		renderContext.DrawIndexedInstanced(_indexBuffer->GetIndexNum(), 1);
	}

	for (auto child : _childs) {
		child->Draw(renderContext, false);
	}
}

void FBXMesh::SetConstantBuffer(ID3D12Device& device, ConstantBuffer& constantBuffer, const int& registerNo)
{
	_descriptorHeap->RegistConstantBuffer(device, constantBuffer, registerNo);
	for (auto child : _childs) {
		child->SetConstantBuffer(device, constantBuffer);
	}
}

void FBXMesh::SetTexture(ID3D12Device& device, Texture& texture, const int& registerNo)
{
	ShaderResourceViewDesc desc(texture);
	_descriptorHeap->RegistShaderResource(device, texture, desc, registerNo);
}

MYRESULT FBXMesh::CreateVertexBuffer(ID3D12Device& device, FBXMeshData& meshData)
{
	// 頂点バッファー生成
	std::vector<FBXMeshVertex>& vertices = meshData.vertices;

	if (_vertexBuffer != nullptr) { delete _vertexBuffer; }
	_vertexBuffer = new VertexBuffer();

	MYRESULT result = _vertexBuffer->Create(
		device, &vertices[0], SizeofVector(vertices), sizeof(FBXMeshVertex));
	if (result == MYRESULT::FAILED) { return result; }

	return MYRESULT::SUCCESS;
}

MYRESULT FBXMesh::CreateIndexBuffer(ID3D12Device& device, FBXMeshData& meshData)
{
	// インデックスバッファー生成
	std::vector<unsigned int>& indices = meshData.indices;

	if (_indexBuffer != nullptr) { delete _indexBuffer; }
	_indexBuffer = new IndexBuffer();

	MYRESULT result = _indexBuffer->Create(device, indices);
	if (result == MYRESULT::FAILED) { return result; }

	return MYRESULT::SUCCESS;
}

MYRESULT FBXMesh::CreateGraphicsPipelineState(ID3D12Device& device, FBXMeshCreateData& data)
{
	// ルートシグネチャ生成
	if (_rootSignature != nullptr) delete _rootSignature;
	_rootSignature = new RootSignature();

	MYRESULT result = _rootSignature->Create(device, data.rootSignatureData);
	if (result == MYRESULT::FAILED) { return result; }

	// ルートシグネチャとシェーダーセット
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineState = {};
	pipelineState.pRootSignature = &_rootSignature->GetRootSignature();
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
	if (_graphicsPipelineState != nullptr) delete _graphicsPipelineState;
	_graphicsPipelineState = new GraphicsPipelineState();
	return _graphicsPipelineState->Create(device, pipelineState);
}