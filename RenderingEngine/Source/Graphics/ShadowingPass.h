#pragma once

#include <memory>
#include <d3dx12.h>

#include "EngineUtility.h"

namespace NamelessEngine {
	class Actor;
	namespace DX12API {
		class RenderTarget;
		class RootSignature;
		class GraphicsPipelineState;
		class Texture;
		class ConstantBuffer;
		class DescriptorHeapCBV_SRV_UAV;
	}
}

namespace NamelessEngine::Graphics {
	class ShadowingPass {
	public:
		ShadowingPass();
		~ShadowingPass();
	private:
		CD3DX12_VIEWPORT _viewport;
		CD3DX12_RECT _scissorRect;
		std::unique_ptr<DX12API::RenderTarget> _renderTarget = nullptr;
		std::unique_ptr<DX12API::RootSignature> _rootSignature = nullptr;
		std::unique_ptr<DX12API::GraphicsPipelineState> _pipelineState = nullptr;

		Utility::RESULT CreateRenderTarget(ID3D12Device& device);
		Utility::RESULT CreateRootSignature(ID3D12Device& device);
		Utility::RESULT CreateGraphicsPipelineState(ID3D12Device& device);
		Utility::RESULT CreateDescriptorHeap(ID3D12Device& device);
	public:
		Utility::RESULT Init();
		void Render();

	};
}