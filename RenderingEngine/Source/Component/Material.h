#pragma once
#include <DirectXMath.h>
#include <Windows.h>
#include "IComponent.h"

namespace NamelessEngine {
	namespace DX12API {
		class ConstantBuffer;
		class Texture;
		class RenderingContext;
	}
}

namespace NamelessEngine::Component {
	enum class MATERIAL_TEXTURE_TYPE {
		BASECOLOR = 0,
		METALLIC,
		ROUGHNESS,
		NORMAL,
		OCCULUSION,
		TYPE_NUM
	};

	struct Material : public IComponent {
	public:
		Material();
		~Material();

		struct MaterialCBuff {
			DirectX::XMFLOAT4 baseColor;
			float metallic;
			float roughness;
		};
		
		MaterialCBuff materialData;

		DirectX::XMFLOAT4 baseColor;
		float metallic;
		float roughness;

		DX12API::Texture* baseColorTexture = nullptr;
		DX12API::Texture* metallicTexture = nullptr;
		DX12API::Texture* roughnessTexture = nullptr;
		DX12API::Texture* normalTexture = nullptr;
		DX12API::Texture* occulusionTexture = nullptr;

	private:
		DX12API::ConstantBuffer* _buffer = nullptr;

	public:
		void Update(float deltatime) override;
		void Draw(DX12API::RenderingContext& renderContext) override;
		void Build();

		DX12API::ConstantBuffer& GetConstantBuffer();
	};
}