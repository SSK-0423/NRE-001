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
	// ���[�_�[�p��
	_loader = new FBXLoader();

	// FBX�ǂݍ���
	if (!_loader->Load(meshPath))
	{
		return MYRESULT::FAILED;
	}

	// ���_�o�b�t�@�\����
	MYRESULT result = CreateVertexBuffers(device);
	if (result == MYRESULT::FAILED) { return result; }

	// �C���f�b�N�X�o�b�t�@�[����
	result = CreateIndexBuffers(device);
	if (result == MYRESULT::FAILED) { return result; }

	// �O���t�B�N�X�p�C�v���C���X�e�[�g����
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
	// ���[�g�V�O�l�`������
	MYRESULT result = _rootSignature.Create(device, data.rootSignatureData);
	if (result == MYRESULT::FAILED) { return result; }

	// ���[�g�V�O�l�`���ƃV�F�[�_�[�Z�b�g
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineState = {};
	pipelineState.pRootSignature = &_rootSignature.GetRootSignature();
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
	return _graphicsPipelineState.Create(device, pipelineState);
}

//
// ������
// 1. Mesh���Ńo�b�t�@�[����
// �����b�g
// �EFBXLoader��VertexBuffer/IndexBuffer�N���X�Ɉˑ����Ȃ��Ȃ�̂ŁA�Ɨ����������ĐF�X�Ȏ����ɑΉ��ł���
// �f�����b�g
// �EMesh����FBXLoader�ւ̈ˑ��������Ȃ�
// �E���`���̃t�@�C����ǂݍ��ލۂɑΉ��ł��Ȃ�
// 2. FBXLoader���Ńo�b�t�@�[����
// �����b�g
// �E���b�V�����̏������ȑf�ɂȂ�
// �E���`����ǂݍ��ރ��[�_�[�N���X�ւ̐؂�ւ����e��
// �f�����b�g
// �EVertexBuffer/IndexBuffer�N���X�Ɉˑ�����`�ɂȂ�
// �E�Ɨ��������Ȃ��A���i�Ƃ��Ă̎g���܂킵�����Â炭�Ȃ�
//