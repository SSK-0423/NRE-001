#pragma once
#include "IRenderer.h"
#include "GBufferPass.h"
#include "LightingPass.h"

#include <d3dx12.h>

namespace NamelessEngine::Graphics {
	class PBRRenderer : public IRenderer {
	public:
		PBRRenderer();
		~PBRRenderer();

	private:
		GBufferPass _gbufferPass;
		LightingPass _lightingPass;
	public:
		void Render(std::vector<Actor*>& _meshActors, std::vector<Actor*>& _guiActors);
	};
}