//#pragma once
//#include <d3dx12.h>
//#include <memory>
//
//#include "EngineUtility.h"
//
//namespace NamelessEngine {
//	class Actor;
//
//	namespace DX12API {
//		class RootSignature;
//		class GraphicsPipelineState;
//	}
//}
//
//namespace NamelessEngine::Graphics {
//	class RenderPass {
//	public:
//		RenderPass();
//		virtual ~RenderPass() = default;
//		
//		Utility::RESULT Init();
//
//		virtual void Render(std::vector<Actor*>& actors) = 0;
//	protected:
//		// オフスクリーンテクスチャが必要
//		CD3DX12_VIEWPORT _viewport;
//		CD3DX12_RECT _scissorRect;
//		std::unique_ptr<DX12API::RootSignature> _rootSignature = nullptr;
//		std::unique_ptr<DX12API::GraphicsPipelineState> _pipelineState = nullptr;
//		
//		virtual Utility::RESULT CreateRootSignature(ID3D12Device& device) = 0;
//		virtual Utility::RESULT CreateGraphicsPipelineState(ID3D12Device& device) = 0;
//		virtual Utility::RESULT CreateRenderTarget(ID3D12Device& device) = 0;
//	};
//}