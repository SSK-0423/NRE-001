#pragma once
#include <array>
#include <vector>
#include <dxgi1_4.h>
#pragma comment(lib,"dxgi.lib")

#include "IComponent.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RootSignature.h"
#include "GraphicsPipelineState.h"
#include "DescriptorHeapCBV_SRV_UAV.h"
#include "RenderingContext.h"
#include "ShaderLibrary.h"
#include "InputLayout.h"

#include "EngineUtility.h"

namespace NamelessEngine::Component {
	struct MeshVertex {
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 uv;
	};

	struct MeshData {
		std::vector<MeshVertex> vertices;
		std::vector<unsigned int> indices;
	};

	class Mesh : public IComponent {
	public:
		Mesh();
		~Mesh();
	private:
		DX12API::VertexBuffer* _vertexBuffer = nullptr;
		DX12API::IndexBuffer* _indexBuffer = nullptr;
		DX12API::DescriptorHeapCBV_SRV_UAV* _descriptorHeap = nullptr;

		Utility::RESULT CreateVertexBuffer(ID3D12Device& device, MeshData& data);
		Utility::RESULT CreateIndexBuffer(ID3D12Device& device, MeshData& data);
		Utility::RESULT CreateDescriptorHeap(ID3D12Device& device);

	public:
		Utility::RESULT Create(ID3D12Device& device, MeshData& data);
		Utility::RESULT Create(ID3D12Device& device, std::vector<MeshData> data);
		void Update(float deltatime);
		void Draw(DX12API::RenderingContext& renderContext);

		void SetConstantBuffer(
			ID3D12Device& device, DX12API::ConstantBuffer& constantBuffer,
			const int& registerNo = DX12API::DescriptorHeapCBV_SRV_UAV::NEXT_REGISTER);
		void SetTexture(
			ID3D12Device& device, DX12API::Texture& texture,
			DX12API::ShaderResourceViewDesc desc, 
			const int& registerNo = DX12API::DescriptorHeapCBV_SRV_UAV::NEXT_REGISTER);

	};
}