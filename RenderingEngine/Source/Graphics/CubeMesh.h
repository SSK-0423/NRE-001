#pragma once

#include <array>
#include <DirectXMath.h>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RootSignature.h"
#include "GraphicsPipelineState.h"

#include "Shader.h"

#include "DescriptorHeapCBV_SRV_UAV.h"
#include "ConstantBuffer.h"
#include "Texture.h"

#include "RenderingContext.h"

#include "EngineUtility.h"

#include "ShaderResourceViewDesc.h"

namespace NamelessEngine::Graphics
{
	struct CubeMeshData {
		DX12API::Shader vertexShader;	                            // ���_�V�F�[�_�[
		DX12API::Shader pixelShader;								// �s�N�Z���V�F�[�_�[
		DX12API::RootSignatureData rootSignatureData;				// ���[�g�V�O�l�`��
		std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;			// ���_���C�A�E�g
		std::array<DXGI_FORMAT, D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT> colorFormats = {
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		DXGI_FORMAT_UNKNOWN,
		};	// �����_�[�^�[�Q�b�g�̃J���[�t�H�[�}�b�g

		size_t GetRenderTargetNum() const;
	};

	class CubeMesh {
	public:
		CubeMesh();
		~CubeMesh();

	private:
		struct CubeVertex {
			DirectX::XMFLOAT3 position;
			DirectX::XMFLOAT3 normal;
			DirectX::XMFLOAT2 uv;
		};

		std::vector<CubeVertex> _vertices;
		std::vector<unsigned int> _indices;

		void CreateVertexAttributesAndIndicesData(CubeMeshData& data);

		DX12API::VertexBuffer* _vertexBuffer = nullptr;
		Utility::RESULT CreateVertexBuffer(ID3D12Device& devic);

		DX12API::IndexBuffer* _indexBuffer = nullptr;
		Utility::RESULT CreateIndexBuffer(ID3D12Device& device);

		DX12API::GraphicsPipelineState* _graphicsPipelineState = nullptr;
		Utility::RESULT CreateGraphicsPipelineState(ID3D12Device& device, CubeMeshData& data);

		DX12API::RootSignature* _rootSignature = nullptr;

		DX12API::DescriptorHeapCBV_SRV_UAV* _descriptorHeap = nullptr;
		Utility::RESULT CreateDescriptorHeap(ID3D12Device& device);

	public:
		Utility::RESULT Create(ID3D12Device& device, CubeMeshData& data);

		void Draw(DX12API::RenderingContext& renderContext);

		void SetConstantBuffer(
			ID3D12Device& device, DX12API::ConstantBuffer& constantBuffer,
			const int& registerNo = DX12API::DescriptorHeapCBV_SRV_UAV::NEXT_REGISTER);

		void SetTexture(
			ID3D12Device& device, DX12API::Texture& texture, DX12API::ShaderResourceViewDesc desc,
			const int& registerNo = DX12API::DescriptorHeapCBV_SRV_UAV::NEXT_REGISTER);
	};
}