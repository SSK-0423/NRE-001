#include <d3dx12.h>

#include "Dx12GraphicsEngine.h"

#include "PBRRenderer.h"
#include "InputLayout.h"
#include "ShaderLibrary.h"
#include "Texture.h"

#include "Scene.h"
#include "Camera.h"

#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"

#include "Actor.h"

#include "Material.h"

using namespace NamelessEngine::Core;
using namespace NamelessEngine::DX12API;
using namespace NamelessEngine::Utility;
using namespace NamelessEngine::Component;

namespace NamelessEngine::Graphics
{
	PBRRenderer::PBRRenderer()
	{

	}
	PBRRenderer::~PBRRenderer()
	{
	}
	Utility::RESULT PBRRenderer::Init(Scene::Scene& scene)
	{
		RESULT result = _gbufferPass.Init();
		if (result == RESULT::FAILED) { return result; }
		result = _lightingPass.Init();
		if (result == RESULT::FAILED) { return result; }
		result = _skyBoxPass.Init();
		if (result == RESULT::FAILED) { return result; }
		result = _blendPass.Init();
		if (result == RESULT::FAILED) { return result; }

		// キューブテクスチャ生成
		_cubeTexture = std::make_unique<DX12API::Texture>();
		result = _cubeTexture->CreateCubeTextureFromDDS(
			Dx12GraphicsEngine::Instance(), L"res/SanFrancisco3/SanFrancisco3_cube.dds");
		if (result == RESULT::FAILED) { return result; }

		// ライティングパスにGBufferをセット
		for (size_t index = 0; index < static_cast<size_t>(GBUFFER_TYPE::GBUFFER_TYPE_NUM); index++) {
			_lightingPass.SetGBuffer(
				static_cast<GBUFFER_TYPE>(index),
				_gbufferPass.GetGBuffer(static_cast<GBUFFER_TYPE>(index)));
		}
		_lightingPass.SetCubeTexture(*_cubeTexture);

		_skyBoxPass.SetCubeTexture(*_cubeTexture);
		_skyBoxPass.SetCamera(scene.GetCamera());

		_blendPass.SetLightedTexture(_lightingPass.GetOffscreenTexture());
		_blendPass.SetRenderedSkyBoxTexture(_skyBoxPass.GetOffscreenTexture());
		_blendPass.SetDepthTexture(_gbufferPass.GetGBuffer(GBUFFER_TYPE::DEPTH));
	}
	void PBRRenderer::Update(float deltatime, Scene::Scene& scene)
	{
		_lightingPass.SetEyePosition(scene.GetCamera().GetTransform().Position());
		_lightingPass.UpdateParamData();

		for (auto meshActor : scene.GetMeshActors()) {
			Material* material = meshActor->GetComponent<Material>();
			material->SetBaseColor(_baseColor[0], _baseColor[1], _baseColor[2]);
			material->SetRoughness(_roughness);
			material->SetMetallic(_metallic);
		};
	}
	void PBRRenderer::Render(Scene::Scene& scene)
	{
		// Imguiレンダー
		{
			ImGui::SetNextWindowPos(ImVec2(634, 0));
			ImGui::Begin("Physically Based Rendering", 0, ImGuiWindowFlags_NoMove);
			ImGui::SetWindowSize(ImVec2(400, 500), ImGuiCond_::ImGuiCond_FirstUseEver);

			// ラフネス
			ImGui::ColorPicker3("BaseColor", _baseColor, ImGuiColorEditFlags_::ImGuiColorEditFlags_InputRGB);
			ImGui::SliderFloat("Roughness", &_roughness, 0.f, 1.f);
			ImGui::SliderFloat("Metallic", &_metallic, 0.f, 1.f);
			ImGui::End();
			ImGui::Render();
		}

		// 1. GBuffer出力パス
		_gbufferPass.Render(scene.GetMeshActors());
		// 2. ライティングパス
		_lightingPass.Render();
		// 3. スカイボックスパス
		_skyBoxPass.Render();
		// 4. 最終パス？
		_blendPass.Render();

		// 3. ポストエフェクトパス
		// _postEffectPass.Render(_meshActors);
		// 4. GUIパス
		// _guiPass.Render(_guiActors);
	}
}
