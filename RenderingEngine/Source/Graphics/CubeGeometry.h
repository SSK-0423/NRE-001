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
	struct CubeGeometryData {
		DX12API::Shader vertexShader;	                            // 頂点シェーダー
		DX12API::Shader pixelShader;									// ピクセルシェーダー
		DX12API::RootSignatureData rootSignatureData;				// ルートシグネチャ
		std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;	// 頂点レイアウト
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

	class CubeGeometry {
	public:
		CubeGeometry();
		~CubeGeometry();

	private:
		struct CubeVertex {
			DirectX::XMFLOAT3 position;
			DirectX::XMFLOAT3 normal;
			DirectX::XMFLOAT2 uv;
		};

		std::vector<CubeVertex> _vertices;
		std::vector<unsigned int> _indices;

		void CreateVertexAttributesAndIndicesData(CubeGeometryData& data);

		DX12API::VertexBuffer* _vertexBuffer = nullptr;
		Utility::MYRESULT CreateVertexBuffer(ID3D12Device& devic);

		DX12API::IndexBuffer* _indexBuffer = nullptr;
		Utility::MYRESULT CreateIndexBuffer(ID3D12Device& device);

		DX12API::GraphicsPipelineState* _graphicsPipelineState = nullptr;
		Utility::MYRESULT CreateGraphicsPipelineState(ID3D12Device& device, CubeGeometryData& data);

		DX12API::RootSignature* _rootSignature = nullptr;

		DX12API::DescriptorHeapCBV_SRV_UAV* _descriptorHeap = nullptr;
		Utility::MYRESULT CreateDescriptorHeap(ID3D12Device& device);

	public:
		Utility::MYRESULT Create(ID3D12Device& device, CubeGeometryData& data);

		void Draw(DX12API::RenderingContext& renderContext);

		void SetConstantBuffer(
			ID3D12Device& device, DX12API::ConstantBuffer& constantBuffer,
			const int& registerNo = DX12API::DescriptorHeapCBV_SRV_UAV::_NEXT_REGISTER);

		void SetTexture(
			ID3D12Device& device, DX12API::Texture& texture, DX12API::ShaderResourceViewDesc desc,
			const int& registerNo = DX12API::DescriptorHeapCBV_SRV_UAV::_NEXT_REGISTER);
	};
}