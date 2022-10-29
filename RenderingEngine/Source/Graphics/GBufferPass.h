#pragma once
#include <memory>
#include <vector>
#include <unordered_map>
#include <d3dx12.h>

#include "EngineUtility.h"

namespace NamelessEngine {
	class Actor;
	namespace DX12API {
		class RenderTarget;
		class RootSignature;
		class GraphicsPipelineState;
		class Texture;
	}
}

namespace NamelessEngine::Graphics {
	enum class GBUFFER_TYPE {
		COLOR = 0,
		NORMAL,
		WORLD_POS,
		METAL_ROUGH_REFLECT,
		GBUFFER_TYPE_NUM,
	};

	class GBufferPass {
	public:
		GBufferPass();
		~GBufferPass();

	private:
		CD3DX12_VIEWPORT _viewport;
		CD3DX12_RECT _scissorRect;
		std::unique_ptr<DX12API::RootSignature> _rootSignature = nullptr;
		std::unique_ptr<DX12API::GraphicsPipelineState> _pipelineState = nullptr;

		Utility::RESULT CreateGraphicsPipelineState(ID3D12Device& device);
		Utility::RESULT CreateRootSignature(ID3D12Device& device);
		Utility::RESULT CreateRenderTarget(ID3D12Device& device);

		std::unordered_map<GBUFFER_TYPE, std::unique_ptr<DX12API::RenderTarget>> _renderTargets;
	public:
		Utility::RESULT Init();
		void Render(std::vector<Actor*>& actors);
		DX12API::Texture& GetGBuffer(GBUFFER_TYPE type);
	};
}