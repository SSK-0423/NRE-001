#include "Mesh.h"
#include "ConstantBuffer.h"
#include "Texture.h"
#include "ShaderResourceViewDesc.h"

using namespace NamelessEngine::Utility;

namespace NamelessEngine::Component
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

		return _vertexBuffer->Create(
			device, &data.vertices[0], SizeofVector(data.vertices), sizeof(MeshVertex));
	}
	RESULT Mesh::CreateIndexBuffer(ID3D12Device& device, MeshData& data)
	{
		if (_indexBuffer != nullptr) { delete _indexBuffer; }
		_indexBuffer = new DX12API::IndexBuffer();

		return _indexBuffer->Create(device, data.indices);
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

		result = CreateDescriptorHeap(device);
		if (result == RESULT::FAILED) { return result; }

		return RESULT::SUCCESS;
	}

	Utility::RESULT Mesh::Create(ID3D12Device& device, std::vector<MeshData> data)
	{
		return Utility::RESULT::SUCCESS;
	}

	void Mesh::Update(float deltatime)
	{
	}

	void Mesh::Draw(DX12API::RenderingContext& renderContext)
	{
		renderContext.SetDescriptorHeap(*_descriptorHeap);
		renderContext.SetVertexBuffer(0, *_vertexBuffer);
		renderContext.SetIndexBuffer(*_indexBuffer);
		renderContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		renderContext.DrawIndexedInstanced(_indexBuffer->GetIndexNum(), 1);
	}
	void Mesh::SetConstantBuffer(ID3D12Device& device, DX12API::ConstantBuffer& constantBuffer, const int& registerNo)
	{
		_descriptorHeap->RegistConstantBuffer(device, constantBuffer, registerNo);
	}
	void Mesh::SetTexture(ID3D12Device& device, DX12API::Texture& texture, DX12API::ShaderResourceViewDesc desc, const int& registerNo)
	{
		_descriptorHeap->RegistShaderResource(device, texture, desc, registerNo);
	}
}
