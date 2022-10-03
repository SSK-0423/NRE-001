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
	// �����Ń}�e���A����K�p����
	// �E�t�H�����f���̃R���X�^���g�o�b�t�@�[�o�^
	// �E�e�N�X�`���o�^�Ȃ�
	material.ApplyMaterial(*this);
}

MYRESULT FBXMesh::CreateAsChild(Dx12GraphicsEngine& graphicsEngine, FBXMeshData& meshData)
{
	ID3D12Device& device = graphicsEngine.Device();

	// ���_�o�b�t�@�\����
	MYRESULT result = CreateVertexBuffer(device, meshData);
	if (result == MYRESULT::FAILED) { return result; }

	// �C���f�b�N�X�o�b�t�@�[����
	result = CreateIndexBuffer(device, meshData);
	if (result == MYRESULT::FAILED) { return result; }

	// �f�B�X�N���v�^�q�[�v����
	result = CreateDescriptorHeap(device);
	if (result == MYRESULT::FAILED) { return result; }

	// �}�e���A���Z�b�g
	SetMaterial(*meshData.material);

	return MYRESULT::SUCCESS;
}

MYRESULT FBXMesh::LoadFBX(Dx12GraphicsEngine& graphicsEngine, FBXMeshCreateData& meshCreateData)
{
	// ���[�_�[�p��
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

	// FBX�ǂݍ���
	if (!_fbxLoader->Load(_meshDataList, loadData))
	{
		return MYRESULT::FAILED;
	}

	// �O���t�B�N�X�p�C�v���C���X�e�[�g����
	MYRESULT result = CreateGraphicsPipelineState(graphicsEngine.Device(), meshCreateData);
	if (result == MYRESULT::FAILED) { return result; }
	// �f�B�X�N���v�^�q�[�v����
	result = CreateDescriptorHeap(graphicsEngine.Device());
	if (result == MYRESULT::FAILED) { return result; }

	// �}�e���A���𕪂��邽�߂Ƀ}�e���A������FBXMesh�𐶐�
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
	/* �}�e���A���؂�ւ� */
	//
	// ���j�Fb1�ɓ����R���X�^���g�o�b�t�@�[�r���[��؂�ւ�
	// ����FCBV�̃f�B�X�N���v�^�e�[�u���͂P�̂�
	// b0�̒l�͕s�ρAb1�̒l�̂ݕύX������
	// ������1�FFBXMesh�p�̃��[�g�V�O�l�`���𐶐�����
	// �����b�g
	// �E�V���h�E�}�b�v�������ɃV���h�E�}�b�v�p�̃T���v���[���K�v�ɂȂ�
	// �E���[�g�V�O�l�`����
	// �f�����b�g
	// �E�������Ɏg���\���̂̒��g��Polygon��Sprite�ƈقȂ�A���[�g�V�O�l�`���n�̃N���X������������邱�ƂɂȂ�
	// �f�B�t�@�[�h�����_�����O��������ƂȂ��Ă킩��Ȃ��̂ŁA�Ƃ肠����������
	// ������2�F���b�V�����Ƃ�FBXMesh�𐶐����A�}�e���A�������݂̂�ύX����
	// �����b�g
	// �E�K�w���b�V���A�j���\�V�����ɂ��Ή��ł�����
	// 
	// 
	// 
	// 
	// ���[�g�p�����[�^�̐ݒ�ŁAb0,b1���p�ӂ���Ă���
	// �f�B�X�N���v�^�q�[�v�ɂ�CBV��64����
	// 1. �}�e���A��������CBV�𐶐�����
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
	// ���_�o�b�t�@�[����
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
	// �C���f�b�N�X�o�b�t�@�[����
	std::vector<unsigned int>& indices = meshData.indices;

	if (_indexBuffer != nullptr) { delete _indexBuffer; }
	_indexBuffer = new IndexBuffer();

	MYRESULT result = _indexBuffer->Create(device, indices);
	if (result == MYRESULT::FAILED) { return result; }

	return MYRESULT::SUCCESS;
}

MYRESULT FBXMesh::CreateGraphicsPipelineState(ID3D12Device& device, FBXMeshCreateData& data)
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