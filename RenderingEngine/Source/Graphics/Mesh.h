#pragma once
#include <vector>
#include <DirectXMath.h>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RootSignature.h"
#include "GraphicsPipelineState.h"
#include "DescriptorHeapCBV_SRV_UAV.h"
#include "RenderingContext.h"

#include "EngineUtility.h"

namespace NamelessEngine::Graphics {
	struct MeshVertex {
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 uv;
	};

	struct MeshData {
		std::vector<MeshVertex> _vertices;
		std::vector<unsigned int> _indices;
	};

	class Mesh {
	public:
		Mesh();
		~Mesh();
	private:
		MeshData _meshData;

		DX12API::VertexBuffer* _vertexBuffer = nullptr;
		DX12API::IndexBuffer* _indexBuffer = nullptr;
		DX12API::GraphicsPipelineState* _graphicsPipelineState = nullptr;
		DX12API::RootSignature* _rootSignature = nullptr;
		DX12API::DescriptorHeapCBV_SRV_UAV* _descriptorHeap = nullptr;

		Utility::RESULT CreateVertexBuffer(ID3D12Device& device, MeshData& data);
		Utility::RESULT CreateIndexBuffer(ID3D12Device& device, MeshData& data);
		Utility::RESULT CreateGraphicsPipelineState(ID3D12Device& device, MeshData& data);
		Utility::RESULT CreateDescriptorHeap(ID3D12Device& device);

	public:
		Utility::RESULT Create(ID3D12Device& device, MeshData& data);

		void Draw(DX12API::RenderingContext& renderContext);

		void SetConstantBuffer(
			ID3D12Device& device, DX12API::ConstantBuffer& constantBuffer,
			const int& registerNo = DX12API::DescriptorHeapCBV_SRV_UAV::NEXT_REGISTER);

		void SetTexture(
			ID3D12Device& device, DX12API::Texture& texture, DX12API::ShaderResourceViewDesc& desc,
			const int& registerNo = DX12API::DescriptorHeapCBV_SRV_UAV::NEXT_REGISTER);
	};
}