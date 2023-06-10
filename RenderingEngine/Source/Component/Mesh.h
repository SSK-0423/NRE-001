#pragma once
#include <memory>
#include <array>
#include <vector>
#include <dxgi1_4.h>
#pragma comment(lib,"dxgi.lib")

#include "IComponent.h"
#include "Material.h"

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
		DirectX::XMFLOAT4 tangent;
		DirectX::XMFLOAT2 uv;
	};

	struct MeshData {
		std::vector<MeshVertex> vertices;
		std::vector<unsigned int> indices;
		int materialIndex;
	};

	struct SubMesh {
	public:
		SubMesh();
		~SubMesh();

	private:
		DX12API::VertexBuffer* _vertexBuffer = nullptr;
		DX12API::IndexBuffer* _indexBuffer = nullptr;
		DX12API::DescriptorHeapCBV_SRV_UAV* _descriptorHeap = nullptr;
		int _materialIndex;

		Utility::RESULT CreateVertexBuffer(ID3D12Device& device, MeshData& data);
		Utility::RESULT CreateIndexBuffer(ID3D12Device& device, MeshData& data);
		Utility::RESULT CreateDescriptorHeap(ID3D12Device& device);

	public:
		Utility::RESULT Create(ID3D12Device& device, MeshData& data);

		void SetConstantBuffer(
			ID3D12Device& device, DX12API::ConstantBuffer& constantBuffer,
			const int& registerNo = DX12API::DescriptorHeapCBV_SRV_UAV::NEXT_REGISTER);
		void SetTexture(
			ID3D12Device& device, DX12API::Texture& texture,
			DX12API::ShaderResourceViewDesc desc,
			const int& registerNo = DX12API::DescriptorHeapCBV_SRV_UAV::NEXT_REGISTER);

		friend class Mesh;
	};

	class Mesh : public IComponent {
	public:
		Mesh();
		~Mesh();
	private:
		std::vector<SubMesh> _submeshes;
		std::vector<Material> _materials;

	public:
		Utility::RESULT Create(ID3D12Device& device, MeshData& data);
		Utility::RESULT CreateSphere(ID3D12Device& device, unsigned int stackNum, unsigned int sectorNum, float radius);
		Utility::RESULT CreateCube(ID3D12Device& device);
		Utility::RESULT CreateFromGLB(ID3D12Device& device, const std::string& path);

		void Update(float deltatime);
		void Draw(DX12API::RenderingContext& renderContext);

		void SetConstantBufferOnAllSubMeshes(
			ID3D12Device& device, DX12API::ConstantBuffer& constantBuffer,
			const int& registerNo = DX12API::DescriptorHeapCBV_SRV_UAV::NEXT_REGISTER);
		void SetTextureOnAllSubMeshes(
			ID3D12Device& device, DX12API::Texture& texture,
			DX12API::ShaderResourceViewDesc desc,
			const int& registerNo = DX12API::DescriptorHeapCBV_SRV_UAV::NEXT_REGISTER);

	};
}