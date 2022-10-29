#pragma once
#include <DirectXMath.h>
#include <Windows.h>
#include "IComponent.h"

namespace NamelessEngine {
	namespace DX12API {
		class ConstantBuffer;
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

	class Material : public IComponent {
	public:
		Material();
		~Material();

	private:
		struct MaterialCBuff {
			DirectX::XMFLOAT4 baseColor;
			float metallic;
			float roughness;
			BOOL useReflection;
		};
		MaterialCBuff _materialData;

		DX12API::ConstantBuffer* _buffer = nullptr;

	public:
		void Update(float deltatime) override;
		void Draw(DX12API::RenderingContext& renderContext) override;

		void SetBaseColor(float r, float g, float b, float a = 1);
		void SetMetallic(float metallic);
		void SetRoughness(float roughness);
		void SetUseReflection(BOOL useReflection);

		DX12API::ConstantBuffer& GetConstantBuffer();
	};
}