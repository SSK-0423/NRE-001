#include "Mesh.h"
#include "ConstantBuffer.h"
#include "Texture.h"
#include "ShaderResourceViewDesc.h"

using namespace NamelessEngine::Utility;

namespace NamelessEngine::Graphics
{
	Mesh::Mesh()
	{
	}
	Mesh::~Mesh()
	{
	}
	RESULT Mesh::CreateVertexBuffer(ID3D12Device& device, MeshData& data)
	{
		if (_vertexBuffer != nullptr) { delete _vertexBuffer; }
		_vertexBuffer = new DX12API::VertexBuffer();

		RESULT result = _vertexBuffer->Create(
			device, &data.vertices[0], SizeofVector(data.vertices), sizeof(MeshVertex));
		if (result == RESULT::FAILED) { return result; }

		return RESULT::SUCCESS;
	}
	RESULT Mesh::CreateIndexBuffer(ID3D12Device& device, MeshData& data)
	{
		if (_indexBuffer != nullptr) { delete _indexBuffer; }
		_indexBuffer = new DX12API::IndexBuffer();

		RESULT result = _indexBuffer->Create(device, data.indices);
		if (result == RESULT::FAILED) { return result; }
	}
	RESULT Mesh::CreateGraphicsPipelineState(ID3D12Device& device, MeshData& data)
	{
		// ���[�g�V�O�l�`������
		if (_rootSignature != nullptr) delete _rootSignature;
		_rootSignature = new DX12API::RootSignature();

		RESULT result = _rootSignature->Create(device, data.rootSignatureData);
		if (result == RESULT::FAILED) { return result; }

		// ���[�g�V�O�l�`���ƃV�F�[�_�[�Z�b�g
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineState = {};
		pipelineState.pRootSignature = &_rootSignature->GetRootSignature();
		pipelineState.VS = CD3DX12_SHADER_BYTECODE();
		pipelineState.PS = CD3DX12_SHADER_BYTECODE();

		// �T���v���}�X�N�ݒ�
		pipelineState.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		// �u�����h
		pipelineState.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

		// ���X�^���C�Y�ݒ�
		pipelineState.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		pipelineState.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;

		// �C���v�b�g���C�A�E�g�̐ݒ�
		pipelineState.InputLayout.pInputElementDescs = data.inputLayout.GetData();
		pipelineState.InputLayout.NumElements = data.inputLayout.GetSize();
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
		_graphicsPipelineState = new DX12API::GraphicsPipelineState();
		return _graphicsPipelineState->Create(device, pipelineState);

		return RESULT::SUCCESS;
	}

	RESULT Mesh::CreateDescriptorHeap(ID3D12Device& device)
	{
		if (_descriptorHeap != nullptr) { delete _descriptorHeap; }

		_descriptorHeap = new DX12API::DescriptorHeapCBV_SRV_UAV();

		return _descriptorHeap->Create(device);
	}

	RESULT Mesh::Create(ID3D12Device& device, MeshData& data)
	{
		RESULT result = CreateVertexBuffer(device, data);
		if (result == RESULT::FAILED) { return result; }

		result = CreateIndexBuffer(device, data);
		if (result == RESULT::FAILED) { return result; }

		result = CreateGraphicsPipelineState(device, data);
		if (result == RESULT::FAILED) { return result; }

		result = CreateDescriptorHeap(device);
		if (result == RESULT::FAILED) { return result; }

		return RESULT::SUCCESS;
	}

	void Mesh::Draw(DX12API::RenderingContext& renderContext)
	{
		renderContext.SetGraphicsRootSignature(*_rootSignature);
		renderContext.SetPipelineState(*_graphicsPipelineState);
		renderContext.SetDescriptorHeap(*_descriptorHeap);
		renderContext.SetVertexBuffer(0, *_vertexBuffer);
		renderContext.SetIndexBuffer(*_indexBuffer);
		renderContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		renderContext.DrawIndexedInstanced(_indexBuffer->GetIndexNum(), 1);
	}
	void Mesh::SetConstantBuffer(ID3D12Device& device, DX12API::ConstantBuffer& constantBuffer, const int& registerNo)
	{
	}
	void Mesh::SetTexture(ID3D12Device& device, DX12API::Texture& texture, DX12API::ShaderResourceViewDesc& desc, const int& registerNo)
	{
	}
	size_t MeshData::GetRenderTargetNum() const
	{
		for (size_t idx = 0; idx < colorFormats.size(); idx++)
		{
			if (colorFormats[idx] == DXGI_FORMAT_UNKNOWN)
				return idx;
		}
	}
}
