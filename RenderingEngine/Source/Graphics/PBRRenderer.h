#pragma once
#include "IRenderer.h"
#include "GBufferPass.h"
#include "LightingPass.h"
#include "SkyBoxPass.h"
#include "BlendPass.h"

#include "EngineUtility.h"

#include <d3dx12.h>
#include <memory>

namespace NamelessEngine {
	namespace DX12API {
		class Texture;
	}
	namespace Scene {
		class Camera;
		class Scene;
	}
}

namespace NamelessEngine::Graphics {
	class PBRRenderer : public IRenderer {
	public:
		PBRRenderer();
		~PBRRenderer();

	private:
		GBufferPass _gbufferPass;
		LightingPass _lightingPass;
		SkyBoxPass _skyBoxPass;
		BlendPass _blendPass;
		std::unique_ptr<DX12API::Texture> _cubeTexture;

		// ImGuiÉpÉâÉÅÅ[É^
		float _baseColor[3];
		float _roughness;
		float _metallic;

	public:
		Utility::RESULT Init(Scene::Scene& scene) override;
		void Update(float deltatime, Scene::Scene& scene) override;
		void Render(Scene::Scene& scene) override;
	};
}