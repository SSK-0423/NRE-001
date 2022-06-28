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
	// ���[�_�[�p��
	_fbxLoader = new FBXLoader();

	// FBX�ǂݍ���
	if (!_fbxLoader->Load(meshCreateData.modelPath, _meshDataList))
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
	result = CreateGraphicsPipelineState(device, meshCreateData);
	if (result == MYRESULT::FAILED) { return result; }

	// �f�B�X�N���v�^�q�[�v����
	result = CreateDescriptorHeap(device);
	if (result == MYRESULT::FAILED) { return result; }

	// �}�e���A���p�R���X�^���g�o�b�t�@�[����
	result = CreateMaterialConsnantBuffer(device);
	if (result == MYRESULT::FAILED) { return result; }

	return MYRESULT::SUCCESS;
}

void FBXMesh::Draw(RenderingContext& renderContext)
{
	renderContext.SetGraphicsRootSignature(_rootSignature);

	renderContext.SetDescriptorHeap(_descriptorHeap);

	renderContext.SetPipelineState(_graphicsPipelineState);

	// ���b�V�������`��
	for (size_t idx = 0; idx < _meshDataList.size(); idx++) {
		// ���b�V�����Ƃ̃}�e���A���Z�b�g
		SetMaterial(_meshDataList[idx].material);
		renderContext.SetVertexBuffer(0, _vertexBuffers[idx]);
		renderContext.SetIndexBuffer(_indexBuffers[idx]);
		renderContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		renderContext.DrawIndexedInstanced(_indexBuffers[idx].GetIndexNum(), 1);
	}
}

void FBXMesh::SetMaterial(const FBXMaterial& material)
{
	// �A���r�G���g�Z�b�g
	XMStoreFloat4(&_material.ambient,
		XMVectorSet(material.ambient[0], material.ambient[1], material.ambient[2], material.ambient[3]));
	// �f�B�t���[�Y
	XMStoreFloat4(&_material.diffuse,
		XMVectorSet(material.diffuse[0], material.diffuse[1], material.diffuse[2], material.diffuse[3]));
	// �X�y�L�����[
	XMStoreFloat4(&_material.specular,
		XMVectorSet(material.specular[0], material.specular[1], material.specular[2], material.specular[3]));
	// �A���t�@
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
	// ���b�V�������̒��_�o�b�t�@�[�������m��
	_vertexBuffers.resize(_meshDataList.size());

	// ���b�V�������̒��_�o�b�t�@�[����
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
	// ���b�V�������̃C���f�b�N�X�o�b�t�@�[�������m��
	_indexBuffers.resize(_meshDataList.size());

	// ���b�V�������̃C���f�b�N�X�o�b�t�@�[����
	for (size_t idx = 0; idx < _meshDataList.size(); idx++) {
		std::vector<unsigned int>& indices = _meshDataList[idx].indices;

		MYRESULT result = _indexBuffers[idx].Create(device, indices);
		if (result == MYRESULT::FAILED) { return result; }
	}

	return MYRESULT::SUCCESS;
}

MYRESULT FBXMesh::CreateGraphicsPipelineState(ID3D12Device& device, FBXMeshCreateData& data)
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