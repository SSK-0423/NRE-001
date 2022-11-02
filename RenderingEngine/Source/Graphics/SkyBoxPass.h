#pragma once
#include <memory>
#include <d3dx12.h>
#include <DirectXMath.h>

#include "EngineUtility.h"

namespace NamelessEngine {
	class Actor;
	namespace DX12API {
		class RenderTarget;
		class RootSignature;
		class GraphicsPipelineState;
		class Texture;
		class DescriptorHeapCBV_SRV_UAV;
		class ConstantBuffer;
	}
	namespace Component {
		class Mesh;
		class Transform;
	}
	namespace Scene {
		class Camera;
	}
}

namespace NamelessEngine::Graphics {
	class SkyBoxPass {
	public:
		SkyBoxPass();
		~SkyBoxPass();
	private:
		struct ParameterCBuff {
			DirectX::XMFLOAT3 eyePosition;
		};
		ParameterCBuff _paramData;

		CD3DX12_VIEWPORT _viewport;
		CD3DX12_RECT _scissorRect;
		std::unique_ptr<DX12API::RootSignature> _rootSignature = nullptr;
		std::unique_ptr<DX12API::GraphicsPipelineState> _pipelineState = nullptr;

		Utility::RESULT CreateGraphicsPipelineState(ID3D12Device& device);
		Utility::RESULT CreateRootSignature(ID3D12Device& device);

		std::unique_ptr<DX12API::RenderTarget> _renderTarget = nullptr;
		Utility::RESULT CreateRenderTarget(ID3D12Device& device);

		std::unique_ptr<DX12API::ConstantBuffer> _paramBuffer;
		Utility::RESULT CreateParamBuffer(ID3D12Device& device);

		std::unique_ptr<DX12API::DescriptorHeapCBV_SRV_UAV> _descriptorHeap = nullptr;
		Utility::RESULT CreateDescriptorHeap(ID3D12Device& deivce);

		std::unique_ptr<Component::Transform> _transform = nullptr;
		std::unique_ptr<Component::Mesh> _skyBox = nullptr;

	public:
		Utility::RESULT Init();
		void Render();
		void UpdateParamData();
		void SetCubeTexture(DX12API::Texture& texture);
		void SetLightedTexture(DX12API::Texture& texture);
		void SetCamera(Scene::Camera& camera);
	};
}