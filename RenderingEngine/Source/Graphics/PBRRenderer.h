#pragma once
#include "IRenderer.h"
#include "GBufferPass.h"
#include "LightingPass.h"
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
		std::unique_ptr<DX12API::Texture> _cubeTexture;

	public:
		Utility::RESULT Init() override;
		void Render(Scene::Scene& scene) override;
	};
}