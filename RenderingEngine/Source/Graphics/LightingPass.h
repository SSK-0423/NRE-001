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
	class ConstantBuffer;
	class DescriptorHeapCBV_SRV_UAV;
}

namespace NamelessEngine::Graphics {
	enum class GBUFFER_TYPE;

	class LightingPass {
	public:
		LightingPass();
		~LightingPass();

	private:
		struct ParameterCBuff {
			DirectX::XMFLOAT3 eyePosition;
		};
		ParameterCBuff _paramData;

		CD3DX12_VIEWPORT _viewport;
		CD3DX12_RECT _scissorRect;
		
		std::unique_ptr<DX12API::RootSignature> _rootSignature = nullptr;
		Utility::RESULT CreateRootSignature(ID3D12Device& device);
		std::unique_ptr<DX12API::GraphicsPipelineState> _pipelineState = nullptr;
		Utility::RESULT CreateGraphicsPipelineState(ID3D12Device& device);
		std::unique_ptr<DX12API::RenderTarget> _renderTarget;
		Utility::RESULT CreateRenderTarget(ID3D12Device& device);
		
		std::unordered_map<GBUFFER_TYPE, DX12API::Texture&> _gbuffers;

		std::unique_ptr<DX12API::ConstantBuffer> _paramBuffer;
		Utility::RESULT CreateParamBuffer(ID3D12Device& device);

		std::unique_ptr<DX12API::DescriptorHeapCBV_SRV_UAV> _descriptorHeap = nullptr;
		Utility::RESULT CreateDescriptorHeap(ID3D12Device& device);

	public:
		Utility::RESULT Init();
		void Render();
		void SetGBuffer(GBUFFER_TYPE type, DX12API::Texture& texture);
		void SetCubeTexture(DX12API::Texture& texture);
	};
}