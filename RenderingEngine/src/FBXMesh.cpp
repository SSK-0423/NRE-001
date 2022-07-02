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
	SafetyDelete<VertexBuffer>(_vertexBuffer);
	SafetyDelete<IndexBuffer>(_indexBuffer);
	SafetyDelete<Texture>(_texture);
	SafetyDelete<ConstantBuffer>(_materialConstantBuffer);
	SafetyDelete<GraphicsPipelineState>(_graphicsPipelineState);
	SafetyDelete<RootSignature>(_rootSignature);
}

MYRESULT FBXMesh::CreateDescriptorHeap(ID3D12Device& device)
{
	if (_descriptorHeap != nullptr) { delete _descriptorHeap; }

	_descriptorHeap = new DescriptorHeapCBV_SRV_UAV();
	return _descriptorHeap->Create(device);
}

MYRESULT FBXMesh::CreateTexture(
	Dx12GraphicsEngine& graphicsEngine, FBXMeshData& meshData, std::wstring textureFolderPath)
{
	// �e�N�X�`�����Ȃ��Ȃ�X�L�b�v
	if (meshData.material.textureName == L"") { return MYRESULT::SUCCESS; }

	if (_texture != nullptr) { delete _texture; }
	_texture = new Texture();

	std::wstring texturePath = textureFolderPath + L"/" + meshData.material.textureName;
	MYRESULT result = _texture->CreateTextureFromWIC(graphicsEngine, texturePath);
	if (result == MYRESULT::FAILED) { return MYRESULT::FAILED; }

	_descriptorHeap->RegistShaderResource(graphicsEngine.Device(), *_texture);

	return MYRESULT::SUCCESS;
}

MYRESULT FBXMesh::CreateMaterialConsnantBuffer(ID3D12Device& device)
{
	if (_materialConstantBuffer != nullptr) { delete _materialConstantBuffer; }
	_materialConstantBuffer = new ConstantBuffer();

	MYRESULT result = _materialConstantBuffer->Create(device, &_material, sizeof(MaterialBuff));
	if (result == MYRESULT::FAILED) { return MYRESULT::FAILED; }

	_descriptorHeap->RegistConstantBuffer(device, *_materialConstantBuffer);

	return MYRESULT::SUCCESS;
}

MYRESULT FBXMesh::CreateAsChild(Dx12GraphicsEngine& graphicsEngine, ChildMeshCreateData& meshData)
{
	ID3D12Device& device = graphicsEngine.Device();

	// ���_�o�b�t�@�\����
	MYRESULT result = CreateVertexBuffer(device, meshData.meshData);
	if (result == MYRESULT::FAILED) { return result; }

	// �C���f�b�N�X�o�b�t�@�[����
	result = CreateIndexBuffer(device, meshData.meshData);
	if (result == MYRESULT::FAILED) { return result; }

	// �f�B�X�N���v�^�q�[�v����
	result = CreateDescriptorHeap(device);
	if (result == MYRESULT::FAILED) { return result; }

	// �}�e���A���p�R���X�^���g�o�b�t�@�[����
	result = CreateMaterialConsnantBuffer(device);
	if (result == MYRESULT::FAILED) { return result; }

	// �}�e���A���Z�b�g
	SetMaterial(meshData.meshData.material);

	// �e�N�X�`������
	result = CreateTexture(graphicsEngine, meshData.meshData, meshData.textureFolderPath);
	if (result == MYRESULT::FAILED) { return result; }

	return MYRESULT::SUCCESS;
}

MYRESULT FBXMesh::LoadFBX(Dx12GraphicsEngine& graphicsEngine, FBXMeshCreateData& meshCreateData)
{
	// ���[�_�[�p��
	FBXLoader* _fbxLoader = new FBXLoader();

	// FBX�ǂݍ���
	if (!_fbxLoader->Load(meshCreateData.modelPath, _meshDataList))
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

		ChildMeshCreateData data;
		data.meshData = _meshDataList[idx];
		data.textureFolderPath = meshCreateData.textureFolderPath;

		FBXMesh* child = new FBXMesh();
		result = child->CreateAsChild(graphicsEngine, data);
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
	_material.shiness = material.shiness;

	_materialConstantBuffer->UpdateData(&_material);
}

void FBXMesh::SetConstantBuffer(ID3D12Device& device, ConstantBuffer& constantBuffer)
{
	_descriptorHeap->RegistConstantBuffer(device, constantBuffer);
	for (auto child : _childs) {
		child->SetConstantBuffer(device, constantBuffer);
	}
}

void FBXMesh::SetTexture(ID3D12Device& device, Texture& texture)
{
	_descriptorHeap->RegistShaderResource(device, texture);
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