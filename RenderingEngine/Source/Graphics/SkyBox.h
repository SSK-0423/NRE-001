#pragma once

#include <d3dx12.h>
#include <memory>
#include <string>

#include "EngineUtility.h"

namespace NamelessEngine {
	namespace Core {
		class Dx12GraphicsEngine;
	}
	namespace DX12API {
		class RenderingContext;
		class Texture;
	}
	namespace Component {
		class Mesh;
	}
}

namespace NamelessEngine::Graphics {
	class SkyBox {
	public:
		SkyBox();
		~SkyBox();
	private:
		std::unique_ptr<DX12API::Texture> _cubeTexture;
		std::unique_ptr<Component::Mesh> _cubeMesh;

	public:
		Utility::RESULT Create(Core::Dx12GraphicsEngine& graphicsEngine, const std::wstring& texturePass);

		void Draw(DX12API::RenderingContext& renderContext);
	};
}