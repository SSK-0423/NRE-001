#pragma once
#include <memory>
#include <unordered_map>
#include <d3dx12.h>
#include <DirectXMath.h>
#include "EngineUtility.h"

namespace NamelessEngine {
	namespace DX12API {
		class RenderTarget;
		class RootSignature;
		class GraphicsPipelineState;
		class Texture;
		class ConstantBuffer;
		class DescriptorHeapCBV_SRV_UAV;
	}
	namespace Component {
		struct DirectionalLight;
		struct PointLight;
		struct SpotLight;
	}
}

namespace NamelessEngine::Graphics {
	enum class GBUFFER_TYPE;

	enum class BRDF_MODEL {
		COOK_TORRANCE,
		GGX,
	};
	struct LightingParam {
		DirectX::XMFLOAT3 eyePosition;
		int brdfModel;
	};

	class LightingPass {
	public:
		LightingPass();
		~LightingPass();

	private:
		LightingParam _paramData;

		CD3DX12_VIEWPORT _viewport;
		CD3DX12_RECT _scissorRect;

		std::unique_ptr<DX12API::RootSignature> _rootSignature = nullptr;
		Utility::RESULT CreateRootSignature(ID3D12Device& device);
		std::unique_ptr<DX12API::GraphicsPipelineState> _pipelineState = nullptr;
		Utility::RESULT CreateGraphicsPipelineState(ID3D12Device& device);
		std::unique_ptr<DX12API::RenderTarget> _renderTarget = nullptr;
		Utility::RESULT CreateRenderTarget(ID3D12Device& device);

		std::unordered_map<GBUFFER_TYPE, DX12API::Texture&> _gbuffers;

		std::unique_ptr<DX12API::ConstantBuffer> _paramBuffer;
		std::unique_ptr<DX12API::ConstantBuffer> _directionalLightBuffer;
		Utility::RESULT CreateBuffer(ID3D12Device& device);

		std::unique_ptr<DX12API::DescriptorHeapCBV_SRV_UAV> _descriptorHeap = nullptr;
		Utility::RESULT CreateDescriptorHeap(ID3D12Device& device);

	public:
		Utility::RESULT Init();
		void UpdateParamData(LightingParam& param);
		void UpdateDirectionalLight(Component::DirectionalLight& directionalLight);
		void Render();
		void SetGBuffer(GBUFFER_TYPE type, DX12API::Texture& texture);
		DX12API::Texture& GetOffscreenTexture();
	};
}