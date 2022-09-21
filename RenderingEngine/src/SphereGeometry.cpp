#include "SphereGeometry.h"

constexpr float PI = DirectX::XM_PI;

SphereGeometry::SphereGeometry()
{
}

SphereGeometry::~SphereGeometry()
{
	SafetyDelete<VertexBuffer>(_vertexBuffer);
	SafetyDelete<IndexBuffer>(_indexBuffer);
	SafetyDelete<GraphicsPipelineState>(_graphicsPipelineState);
	SafetyDelete<RootSignature>(_rootSignature);
}

void SphereGeometry::CreateVerticesAndIndicesData(SphereGeometryData& data)
{
	unsigned int stackNum = data.stackNum;
	unsigned int sectorNum = data.sectorNum;
	float radius = data.radius;
	float lengthInv = 1.f / radius;

	float sectorStep = 2.f * PI / static_cast<float>(sectorNum);
	float stackStep = PI / static_cast<float>(stackNum);

	float sectorAngle = 0.f;
	float stackAngle = 0.f;

	_vertices.resize((static_cast<size_t>(stackNum) + 1) * (static_cast<size_t>(sectorNum) + 1));

	// k1--k1+1
	// |  / |
	// | /  |
	// k2--k2+1

	// 頂点データ生成
	// スタック数：縦をいくつの面で構成するか
	// 縦の頂点数：上端と下端があるのでスタック数+1になる セクターも一緒
	for (size_t stack = 0; stack <= stackNum; stack++) {
		stackAngle = PI / 2 - PI * stackStep * stack;
		float xz = radius * cosf(stackAngle);
		float y = radius * sinf(stackAngle);

		for (size_t sector = 0; sector <= sectorNum; sector++) {
			sectorAngle = sector * sectorStep;
			float x = xz * cosf(sectorAngle);
			float z = xz * sinf(sectorAngle);

			SphereVertex vertex;
			vertex.position = DirectX::XMFLOAT3(x, y, z);

			// 法線計算
			// 頂点位置は球の中心からの位置ベクトルなので法線方向と同じと考えることができる
			// また、ベクトルの長さは球の半径になるので半径で割ることで正規化できる
			float nx = x * lengthInv;
			float ny = y * lengthInv;
			float nz = z * lengthInv;
			vertex.normal = DirectX::XMFLOAT3(nx, ny, nz);

			// UV計算
			// 球を切り開いて長方形にすることをイメージするとわかりやすい
			float u = static_cast<float>(sector) / sectorNum;
			float v = static_cast<float>(stack) / stackNum;
			vertex.uv = DirectX::XMFLOAT2(u, v);

			_vertices[stack * (static_cast<size_t>(sectorNum) + 1) + sector] = vertex;
		}
	}
	unsigned int k1 = 0;
	unsigned int k2 = 0;

	// 頂点インデックスデータ生成
	for (size_t stack = 0; stack < stackNum; stack++) {
		k1 = stack * (static_cast<size_t>(sectorNum) + 1);	// 現在のスタック
		k2 = k1 + sectorNum + 1;							// 次のスタック

		for (size_t sector = 0; sector < sectorNum; sector++, k1++, k2++) {
			// セクターごとに2つの三角形が必要
			// k1 => k2 => k1+1 下端も
			if (stack != 0) {
				_indices.push_back(k1);
				_indices.push_back(k2);
				_indices.push_back(k1 + 1);
			}

			// k1+1 => k2 => k2+1 上端も
			if (stack != (stackNum - 1)) {
				_indices.push_back(k1 + 1);
				_indices.push_back(k2);
				_indices.push_back(k2 + 1);
			}
		}
	}
}

MYRESULT SphereGeometry::CreateVertexBuffer(ID3D12Device& device)
{
	if (_vertexBuffer != nullptr) { delete _vertexBuffer; }
	_vertexBuffer = new VertexBuffer();

	MYRESULT result = _vertexBuffer->Create(
		device, &_vertices[0], SizeofVector(_vertices), sizeof(SphereVertex));
	if (result == MYRESULT::FAILED) { return result; }

	return MYRESULT::SUCCESS;
}

MYRESULT SphereGeometry::CreateIndexBuffer(ID3D12Device& device)
{
	if (_indexBuffer != nullptr) { delete _indexBuffer; }
	_indexBuffer = new IndexBuffer();

	MYRESULT result = _indexBuffer->Create(device, _indices);
	if (result == MYRESULT::FAILED) { return result; }

	return MYRESULT::SUCCESS;
}

MYRESULT SphereGeometry::CreateGraphicsPipelineState(ID3D12Device& device, SphereGeometryData& data)
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

MYRESULT SphereGeometry::CreateDescriptorHeap(ID3D12Device& device)
{
	if (_descriptorHeap != nullptr) { delete _descriptorHeap; }

	_descriptorHeap = new DescriptorHeapCBV_SRV_UAV();

	return _descriptorHeap->Create(device);
}

MYRESULT SphereGeometry::Create(ID3D12Device& device, SphereGeometryData& data)
{
	CreateVerticesAndIndicesData(data);

	MYRESULT result = CreateVertexBuffer(device);
	if (result == MYRESULT::FAILED) { return result; }

	result = CreateIndexBuffer(device);
	if (result == MYRESULT::FAILED) { return result; }

	result = CreateGraphicsPipelineState(device, data);
	if (result == MYRESULT::FAILED) { return result; }

	result = CreateDescriptorHeap(device);
	if (result == MYRESULT::FAILED) { return result; }

	return MYRESULT::SUCCESS;
}

void SphereGeometry::Draw(RenderingContext& renderContext)
{
	renderContext.SetGraphicsRootSignature(*_rootSignature);
	renderContext.SetPipelineState(*_graphicsPipelineState);
	renderContext.SetDescriptorHeap(*_descriptorHeap);
	renderContext.SetVertexBuffer(0, *_vertexBuffer);
	renderContext.SetIndexBuffer(*_indexBuffer);
	renderContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	renderContext.DrawIndexedInstanced(_indexBuffer->GetIndexNum(), 1);
}

void SphereGeometry::SetConstantBuffer(ID3D12Device& device, ConstantBuffer& constantBuffer, const int& registerNo)
{
}

void SphereGeometry::SetTexture(ID3D12Device& device, Texture& texture, const int& registerNo)
{
}

size_t SphereGeometryData::GetRenderTargetNum() const
{
	for (size_t idx = 0; idx < colorFormats.size(); idx++)
	{
		if (colorFormats[idx] == DXGI_FORMAT_UNKNOWN)
			return idx;
	}
}
