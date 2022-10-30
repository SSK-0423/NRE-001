#include <d3dx12.h>

#include "Dx12GraphicsEngine.h"

#include "PBRRenderer.h"
#include "InputLayout.h"
#include "ShaderLibrary.h"
#include "Texture.h"

#include "Scene.h"
#include "Camera.h"

using namespace NamelessEngine::Core;
using namespace NamelessEngine::DX12API;
using namespace NamelessEngine::Utility;

namespace NamelessEngine::Graphics
{
	PBRRenderer::PBRRenderer()
	{

	}
	PBRRenderer::~PBRRenderer()
	{
	}
	Utility::RESULT PBRRenderer::Init()
	{
		RESULT result = _gbufferPass.Init();
		if (result == RESULT::FAILED) { return result; }
		result = _lightingPass.Init();
		if (result == RESULT::FAILED) { return result; }

		// ライティングパスにGBufferをセット
		for (size_t index = 0; index < static_cast<size_t>(GBUFFER_TYPE::GBUFFER_TYPE_NUM); index++) {
			_lightingPass.SetGBuffer(
				static_cast<GBUFFER_TYPE>(index),
				_gbufferPass.GetGBuffer(static_cast<GBUFFER_TYPE>(index)));
		}

		// キューブテクスチャ生成
		_cubeTexture = std::make_unique<DX12API::Texture>();
		result = _cubeTexture->CreateCubeTextureFromDDS(
			Dx12GraphicsEngine::Instance(), L"res/SanFrancisco3/SanFrancisco3_cube.dds");
		if (result == RESULT::FAILED) { return result; }
		_lightingPass.SetCubeTexture(*_cubeTexture);
	}
	void PBRRenderer::Update(float deltatime)
	{
	}
	void PBRRenderer::Render(Scene::Scene& scene)
	{
		// バッファ更新
		_lightingPass.SetEyePosition(scene.GetCamera().GetTransform().Position());
		_lightingPass.UpdateParamData();

		// 1. GBuffer出力パス
		_gbufferPass.Render(scene.GetMeshActors());
		// 2. ライティングパス
		_lightingPass.Render();
		// 3. ポストエフェクトパス
		// _postEffectPass.Render(_meshActors);
		// 4. GUIパス
		// _guiPass.Render(_guiActors);
	}
}
