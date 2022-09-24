#include "CubeGeometry.h"

CubeGeometry::CubeGeometry()
{
}

CubeGeometry::~CubeGeometry()
{
	SafetyDelete<VertexBuffer>(_vertexBuffer);
	SafetyDelete<IndexBuffer>(_indexBuffer);
	SafetyDelete<GraphicsPipelineState>(_graphicsPipelineState);
	SafetyDelete<RootSignature>(_rootSignature);
}

void CubeGeometry::CreateVertexAttributesAndIndicesData(CubeGeometryData& data)
{
	std::vector<DirectX::XMFLOAT3> vertices = {
		// positiveX
		DirectX::XMFLOAT3(0.5,-0.5, -0.5),
		DirectX::XMFLOAT3(0.5, 0.5, -0.5),
		DirectX::XMFLOAT3(0.5,-0.5,  0.5),
		DirectX::XMFLOAT3(0.5, 0.5,  0.5),
		// negativeX
		DirectX::XMFLOAT3(-0.5,-0.5,-0.5),
		DirectX::XMFLOAT3(-0.5, 0.5,-0.5),
		DirectX::XMFLOAT3(-0.5,-0.5, 0.5),
		DirectX::XMFLOAT3(-0.5, 0.5, 0.5),

		// positiveY
		DirectX::XMFLOAT3(-0.5, 0.5,-0.5),
		DirectX::XMFLOAT3(0.5, 0.5,-0.5),
		DirectX::XMFLOAT3(-0.5, 0.5, 0.5),
		DirectX::XMFLOAT3(0.5, 0.5, 0.5),
		// negativeY
		DirectX::XMFLOAT3(-0.5,-0.5,-0.5),
		DirectX::XMFLOAT3(0.5,-0.5,-0.5),
		DirectX::XMFLOAT3(-0.5,-0.5, 0.5),
		DirectX::XMFLOAT3(0.5,-0.5, 0.5),

		// positiveZ
		DirectX::XMFLOAT3(-0.5,-0.5, 0.5),
		DirectX::XMFLOAT3(0.5,-0.5, 0.5),
		DirectX::XMFLOAT3(-0.5, 0.5, 0.5),
		DirectX::XMFLOAT3(0.5, 0.5, 0.5),
		// negativeZ
		DirectX::XMFLOAT3(-0.5,-0.5,-0.5),
		DirectX::XMFLOAT3(0.5,-0.5,-0.5),
		DirectX::XMFLOAT3(-0.5, 0.5,-0.5),
		DirectX::XMFLOAT3(0.5, 0.5,-0.5),
	};

	std::vector<DirectX::XMFLOAT3> normals = {
		// positiveX
		DirectX::XMFLOAT3(1,0,0),
		DirectX::XMFLOAT3(1,0,0),
		DirectX::XMFLOAT3(1,0,0),
		DirectX::XMFLOAT3(1,0,0),
		// negativeX
		DirectX::XMFLOAT3(-1,0,0),
		DirectX::XMFLOAT3(-1,0,0),
		DirectX::XMFLOAT3(-1,0,0),
		DirectX::XMFLOAT3(-1,0,0),

		// positiveY
		DirectX::XMFLOAT3(0,1,0),
		DirectX::XMFLOAT3(0,1,0),
		DirectX::XMFLOAT3(0,1,0),
		DirectX::XMFLOAT3(0,1,0),
		// negativeY
		DirectX::XMFLOAT3(0,-1,0),
		DirectX::XMFLOAT3(0,-1,0),
		DirectX::XMFLOAT3(0,-1,0),
		DirectX::XMFLOAT3(0,-1,0),

		// positiveZ
		DirectX::XMFLOAT3(0,0,1),
		DirectX::XMFLOAT3(0,0,1),
		DirectX::XMFLOAT3(0,0,1),
		DirectX::XMFLOAT3(0,0,1),
		// negativeZ
		DirectX::XMFLOAT3(0,0,-1),
		DirectX::XMFLOAT3(0,0,-1),
		DirectX::XMFLOAT3(0,0,-1),
		DirectX::XMFLOAT3(0,0,-1),
	};

	std::vector<DirectX::XMFLOAT2> uv = {
		// positiveX
		DirectX::XMFLOAT2(0, 0),     //����
		DirectX::XMFLOAT2(0, 1),     //����
		DirectX::XMFLOAT2(1, 0),     //�E��
		DirectX::XMFLOAT2(1, 1),     //�E��
		// negativeX
		DirectX::XMFLOAT2(0, 0),     //����
		DirectX::XMFLOAT2(0, 1),     //����
		DirectX::XMFLOAT2(1, 0),     //�E��
		DirectX::XMFLOAT2(1, 1),     //�E��
		// positiveY
		DirectX::XMFLOAT2(0, 0),     //����
		DirectX::XMFLOAT2(1, 0),     //�E��
		DirectX::XMFLOAT2(0, 1),     //����
		DirectX::XMFLOAT2(1, 1),     //�E��
		// negativeY
		DirectX::XMFLOAT2(0, 0),     //����
		DirectX::XMFLOAT2(1, 0),     //�E��
		DirectX::XMFLOAT2(0, 1),     //����
		DirectX::XMFLOAT2(1, 1),     //�E��
		// positiveZ
		DirectX::XMFLOAT2(0, 0),     //����
		DirectX::XMFLOAT2(1, 0),     //�E��
		DirectX::XMFLOAT2(0, 1),     //����
		DirectX::XMFLOAT2(1, 1),     //�E��
		// negativeZ
		DirectX::XMFLOAT2(0, 0),     //����
		DirectX::XMFLOAT2(1, 0),     //�E��
		DirectX::XMFLOAT2(0, 1),     //����
		DirectX::XMFLOAT2(1, 1)      //�E��
	};

	_vertices.resize(vertices.size());
	for (size_t idx = 0; idx < vertices.size(); idx++) {
		_vertices[idx].position = vertices[idx];
		_vertices[idx].normal = normals[idx];
		_vertices[idx].uv = uv[idx];
	}

	std::vector<unsigned int> indices = {
		// positiveX
		0,1,3,
		0,3,2,
		// negativeX
		4,5,7,
		4,7,6,
		// positiveY
		8,10,11,
		8,11,9,
		// negativeY
		12,14,15,
		12,15,13,
		// positiveZ
		16,18,19,
		16,19,17,
		// negativeZ
		20,22,23,
		20,23,21
	};
	_indices = indices;
}

MYRESULT CubeGeometry::CreateVertexBuffer(ID3D12Device& device)
{
	if (_vertexBuffer != nullptr) { delete _vertexBuffer; }
	_vertexBuffer = new VertexBuffer();

	MYRESULT result = _vertexBuffer->Create(
		device, &_vertices[0], SizeofVector(_vertices), sizeof(CubeVertex));
	if (result == MYRESULT::FAILED) { return result; }

	return MYRESULT::SUCCESS;
}

MYRESULT CubeGeometry::CreateIndexBuffer(ID3D12Device& device)
{
	if (_indexBuffer != nullptr) { delete _indexBuffer; }
	_indexBuffer = new IndexBuffer();

	MYRESULT result = _indexBuffer->Create(device, _indices);
	if (result == MYRESULT::FAILED) { return result; }

	return MYRESULT::SUCCESS;
}

MYRESULT CubeGeometry::CreateGraphicsPipelineState(ID3D12Device& device, CubeGeometryData& data)
{
	// ���[�g�V�O�l�`������
	if (_rootSignature != nullptr) delete _rootSignature;
	_rootSignature = new RootSignature();

	MYRESULT result = _rootSignature->Create(device, data.rootSignatureData);
	if (result == MYRESULT::FAILED) { return result; }

	// ���[�g�V�O�l�`���ƃV�F�[�_�[�Z�b�g
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineState = {};
	pipelineState.pRootSignature = &_rootSignature->GetRootSignature();
	pipelineState.VS = CD3DX12_SHADER_BYTECODE(&data.vertexShader.GetShader());
	pipelineState.PS = CD3DX12_SHADER_BYTECODE(&data.pixelShader.GetShader());

	// �T���v���}�X�N�ݒ�
	pipelineState.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// �u�����h
	pipelineState.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

	// ���X�^���C�Y�ݒ�
	pipelineState.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	pipelineState.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	// �C���v�b�g���C�A�E�g�̐ݒ�
	pipelineState.InputLayout.pInputElementDescs = &data.inputLayout[0];
	pipelineState.InputLayout.NumElements = static_cast<UINT>(data.inputLayout.size());
	pipelineState.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
	pipelineState.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// �f�v�X�X�e���V���ݒ�
	pipelineState.DepthStencilState.DepthEnable = true;
	pipelineState.DepthStencilState.StencilEnable = false;
	pipelineState.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	pipelineState.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	pipelineState.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// �����_�[�^�[�Q�b�g�̐ݒ�
	pipelineState.NumRenderTargets = data.GetRenderTargetNum();

	for (size_t idx = 0; idx < pipelineState.NumRenderTargets; idx++)
	{
		pipelineState.RTVFormats[idx] = data.colorFormats[idx];
	}

	// �A���`�G�C���A�V���O�̂��߂̃T���v�����ݒ�
	pipelineState.SampleDesc.Count = 1;	    // �T���v�����O��1�s�N�Z���ɂ�1
	pipelineState.SampleDesc.Quality = 0;	// �N�I���e�B�͍Œ�

	// �O���t�B�b�N�X�p�C�v���C���X�e�[�g����
	if (_graphicsPipelineState != nullptr) delete _graphicsPipelineState;
	_graphicsPipelineState = new GraphicsPipelineState();
	return _graphicsPipelineState->Create(device, pipelineState);
}

MYRESULT CubeGeometry::CreateDescriptorHeap(ID3D12Device& device)
{
	if (_descriptorHeap != nullptr) { delete _descriptorHeap; }

	_descriptorHeap = new DescriptorHeapCBV_SRV_UAV();

	return _descriptorHeap->Create(device);
}

MYRESULT CubeGeometry::Create(ID3D12Device& device, CubeGeometryData& data)
{
	CreateVertexAttributesAndIndicesData(data);

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

void CubeGeometry::Draw(RenderingContext& renderContext)
{
	renderContext.SetGraphicsRootSignature(*_rootSignature);
	renderContext.SetPipelineState(*_graphicsPipelineState);
	renderContext.SetDescriptorHeap(*_descriptorHeap);
	renderContext.SetVertexBuffer(0, *_vertexBuffer);
	renderContext.SetIndexBuffer(*_indexBuffer);
	renderContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	renderContext.DrawIndexedInstanced(_indexBuffer->GetIndexNum(), 1);
}

void CubeGeometry::SetConstantBuffer(ID3D12Device& device, ConstantBuffer& constantBuffer, const int& registerNo)
{
	_descriptorHeap->RegistConstantBuffer(device, constantBuffer, registerNo);
}

void CubeGeometry::SetTexture(
	ID3D12Device& device, Texture& texture,
	ShaderResourceViewDesc desc, const int& registerNo)
{
	_descriptorHeap->RegistShaderResource(device, texture, desc, registerNo);
}

size_t CubeGeometryData::GetRenderTargetNum() const
{
	for (size_t idx = 0; idx < colorFormats.size(); idx++)
	{
		if (colorFormats[idx] == DXGI_FORMAT_UNKNOWN)
			return idx;
	}
}
