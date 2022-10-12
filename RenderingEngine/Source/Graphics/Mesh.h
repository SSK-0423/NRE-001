#pragma once
#include <vector>
#include <array>
#include <DirectXMath.h>
#include <dxgi1_4.h>

#pragma comment(lib,"dxgi.lib")

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RootSignature.h"
#include "GraphicsPipelineState.h"
#include "DescriptorHeapCBV_SRV_UAV.h"
#include "RenderingContext.h"
#include "ShaderLibrary.h"
#include "InputLayout.h"

#include "EngineUtility.h"

namespace NamelessEngine::Graphics {
	struct MeshVertex {
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 uv;
	};

	struct MeshData {
		std::vector<MeshVertex> vertices;
		std::vector<unsigned int> indices;
		std::unordered_map<DX12API::SHADERTYPE, DX12API::Shader*> shaders;
		DX12API::RootSignatureData rootSignatureData;
		DX12API::InputLayout inputLayout;
		std::array<DXGI_FORMAT, D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT> colorFormats = {
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		};	// レンダーターゲットのカラーフォーマット

		size_t GetRenderTargetNum() const;
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