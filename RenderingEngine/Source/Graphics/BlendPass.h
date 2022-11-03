#pragma once
#include <memory>
#include <unordered_map>
#include <d3dx12.h>
#include <DirectXMath.h>
#include "EngineUtility.h"

namespace NamelessEngine::DX12API {
	class RenderTarget;
	class RootSignature;
	class GraphicsPipelineState;
	class Texture;
	class DescriptorHeapCBV_SRV_UAV;
}

namespace NamelessEngine::Graphics {

	class BlendPass {
	public:
		BlendPass();
		~BlendPass();

	private:
		CD3DX12_VIEWPORT _viewport;
		CD3DX12_RECT _scissorRect;

		std::unique_ptr<DX12API::RootSignature> _rootSignature = nullptr;
		Utility::RESULT CreateRootSignature(ID3D12Device& device);
		std::unique_ptr<DX12API::GraphicsPipelineState> _pipelineState = nullptr;
		Utility::RESULT CreateGraphicsPipelineState(ID3D12Device& device);
		std::unique_ptr<DX12API::RenderTarget> _renderTarget;
		Utility::RESULT CreateRenderTarget(ID3D12Device& device);

		std::unique_ptr<DX12API::DescriptorHeapCBV_SRV_UAV> _descriptorHeap = nullptr;
		Utility::RESULT CreateDescriptorHeap(ID3D12Device& device);

	public:
		Utility::RESULT Init();
		void Render();
		void SetRenderedSkyBoxTexture(DX12API::Texture& texture);
		void SetLightedTexture(DX12API::Texture& texture);
		void SetDepthTexture(DX12API::Texture& texture);
		DX12API::Texture& GetOffscreenTexture();
	};
}