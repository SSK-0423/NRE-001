#pragma once

#include "IComponent.h"

namespace NamelessEngine::Component {
	struct DirectionalLight
	{
		float direction[3] = { 0.f,1.f,0.f };
		float intensity = 1.f;
		float color[4] = { 1.f,1.f,1.f,1.f };
	};
	// TODO: ポイントライト、スポットライト
	struct PointLight {

	};

	struct SpotLight {

	};

	enum class LightType {
		DIRECTIONAL,
		POINT,
		SPOT,
		TYPE_NUM
	};

	struct LightSource : public IComponent {
	public:
		LightSource();
		~LightSource();

		DirectionalLight directionalLight;
		PointLight pointLight;
		SpotLight spotLight;
	};
}