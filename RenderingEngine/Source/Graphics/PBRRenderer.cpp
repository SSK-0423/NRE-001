#include "d3dx12.h"

#include "PBRRenderer.h"
#include "InputLayout.h"
#include "ShaderLibrary.h"
#include "Dx12GraphicsEngine.h"

using namespace NamelessEngine::Core;
using namespace NamelessEngine::DX12API;
using namespace NamelessEngine::Utility;

namespace NamelessEngine::Graphics
{
	PBRRenderer::PBRRenderer()
	{
		// ライティングパスにGBufferをセット
		for (size_t index = 0; index < static_cast<size_t>(GBUFFER_TYPE::GBUFFER_TYPE_NUM); index++) {
			_lightingPass.SetGBuffer(
				static_cast<GBUFFER_TYPE>(index),
				_gbufferPass.GetGBuffer(static_cast<GBUFFER_TYPE>(index)));
		}
	}
	PBRRenderer::~PBRRenderer()
	{
	}
	void PBRRenderer::Render(std::vector<Actor*>& _meshActors, std::vector<Actor*>& _guiActors)
	{
		// 1. GBuffer出力パス
		_gbufferPass.Render(_meshActors);
		// 2. ライティングパス
		_lightingPass.Render();
		// 3. ポストエフェクトパス
		// _postEffectPass.Render(_meshActors);
		// 4. GUIパス
		// _guiPass.Render(_guiActors);
	}
}
