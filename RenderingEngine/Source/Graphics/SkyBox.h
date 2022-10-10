#pragma once

#include "d3dx12.h"

#include "CubeMesh.h"
#include "Texture.h"

#include "Dx12GraphicsEngine.h"

#include "RenderingContext.h"
#include "EngineUtility.h"

namespace NamelessEngine::Graphics {
	class SkyBox {
	public:
		SkyBox();
		~SkyBox();
	private:
		CubeMesh _cube;
		DX12API::Texture _cubeTexture;

	public:
		Utility::RESULT Create(Core::Dx12GraphicsEngine& graphicsEngine, const std::wstring& texturePass);

		void Draw(DX12API::RenderingContext& renderContext);
	};
}