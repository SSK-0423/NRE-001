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

	// ���_�f�[�^����
	// �X�^�b�N���F�c�������̖ʂō\�����邩
	// �c�̒��_���F��[�Ɖ��[������̂ŃX�^�b�N��+1�ɂȂ� �Z�N�^�[���ꏏ
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

			// �@���v�Z
			// ���_�ʒu�͋��̒��S����̈ʒu�x�N�g���Ȃ̂Ŗ@�������Ɠ����ƍl���邱�Ƃ��ł���
			// �܂��A�x�N�g���̒����͋��̔��a�ɂȂ�̂Ŕ��a�Ŋ��邱�ƂŐ��K���ł���
			float nx = x * lengthInv;
			float ny = y * lengthInv;
			float nz = z * lengthInv;
			vertex.normal = DirectX::XMFLOAT3(nx, ny, nz);

			// UV�v�Z
			// ����؂�J���Ē����`�ɂ��邱�Ƃ��C���[�W����Ƃ킩��₷��
			float u = static_cast<float>(sector) / sectorNum;
			float v = static_cast<float>(stack) / stackNum;
			vertex.uv = DirectX::XMFLOAT2(u, v);

			_vertices[stack * (static_cast<size_t>(sectorNum) + 1) + sector] = vertex;
		}
	}
	unsigned int k1 = 0;
	unsigned int k2 = 0;

	// ���_�C���f�b�N�X�f�[�^����
	for (size_t stack = 0; stack < stackNum; stack++) {
		k1 = stack * (static_cast<size_t>(sectorNum) + 1);	// ���݂̃X�^�b�N
		k2 = k1 + sectorNum + 1;							// ���̃X�^�b�N

		for (size_t sector = 0; sector < sectorNum; sector++, k1++, k2++) {
			// �Z�N�^�[���Ƃ�2�̎O�p�`���K�v
			// k1 => k2 => k1+1 ���[��
			if (stack != 0) {
				_indices.push_back(k1);
				_indices.push_back(k2);
				_indices.push_back(k1 + 1);
			}

			// k1+1 => k2 => k2+1 ��[��
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
