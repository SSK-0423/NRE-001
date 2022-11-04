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
		result = _iblPass.Init();
		if (result == RESULT::FAILED) { return result; }
		result = _skyBoxPass.Init();
		if (result == RESULT::FAILED) { return result; }
		result = _blendPass.Init();
		if (result == RESULT::FAILED) { return result; }

		// キューブテクスチャ生成
		_environment = std::make_unique<DX12API::Texture>();
		result = _environment->CreateCubeTextureFromDDS(
			Dx12GraphicsEngine::Instance(), L"res/clarens_night_01/clarens_night_01EnvHDR.dds");
		if (result == RESULT::FAILED) { return result; }

		_specularLD = std::make_unique<DX12API::Texture>();
		result = _specularLD->CreateCubeTextureFromDDS(
			Dx12GraphicsEngine::Instance(), L"res/clarens_night_01/clarens_night_01SpecularHDR.dds");
		if (result == RESULT::FAILED) { return result; }

		_diffuseLD = std::make_unique<DX12API::Texture>();
		result = _diffuseLD->CreateCubeTextureFromDDS(
			Dx12GraphicsEngine::Instance(), L"res/clarens_night_01/clarens_night_01DiffuseHDR.dds");
		if (result == RESULT::FAILED) { return result; }

		_DFG = std::make_unique<DX12API::Texture>();
		result = _DFG->CreateTextureFromDDS(
			Dx12GraphicsEngine::Instance(), L"res/clarens_night_01/clarens_night_01Brdf.dds");
		if (result == RESULT::FAILED) { return result; }

		// 各レンダリングパスに必要なリソースをセット
		for (size_t index = 0; index < static_cast<size_t>(GBUFFER_TYPE::GBUFFER_TYPE_NUM); index++) {

			Texture& gbuffer = _gbufferPass.GetGBuffer(static_cast<GBUFFER_TYPE>(index));
			_lightingPass.SetGBuffer(static_cast<GBUFFER_TYPE>(index), gbuffer);
			_iblPass.SetGBuffer(static_cast<GBUFFER_TYPE>(index), gbuffer);
		}

		_iblPass.SetIBLTextures(*_specularLD, *_diffuseLD, *_DFG);
		_iblPass.SetLightedTexture(_lightingPass.GetOffscreenTexture());

		_skyBoxPass.SetCubeTexture(*_environment);
		_skyBoxPass.SetCamera(scene.GetCamera());

		_blendPass.SetLightedTexture(_iblPass.GetOffscreenTexture());
		_blendPass.SetRenderedSkyBoxTexture(_skyBoxPass.GetOffscreenTexture());
		_blendPass.SetDepthTexture(_gbufferPass.GetGBuffer(GBUFFER_TYPE::DEPTH));
	}
	void PBRRenderer::Update(float deltatime, Scene::Scene& scene)
	{
		_lightingParam.eyePosition = scene.GetCamera().GetTransform().Position();
		_lightingPass.UpdateParamData(_lightingParam);
		
		_directionalLight.color.x = _dLightColor[0];
		_directionalLight.color.y = _dLightColor[1];
		_directionalLight.color.z = _dLightColor[2];
		_directionalLight.direction.x = _dLightDirection[0];
		_directionalLight.direction.y = _dLightDirection[1];
		_directionalLight.direction.z = _dLightDirection[2];
		_lightingPass.UpdateDirectionalLight(_directionalLight);

		_iblParam.eyePosition = scene.GetCamera().GetTransform().Position();
		_iblPass.UpdateParamData(_iblParam);

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
			ImGui::SetNextWindowPos(ImVec2(650, 0));
			ImGui::Begin("Physically Based Rendering", 0, ImGuiWindowFlags_NoMove);
			ImGui::SetWindowSize(ImVec2(375, AppWindow::GetWindowSize().cy), ImGuiCond_Once);

			// GBufferパス関連
			ImGui::ColorPicker3("BaseColor", _baseColor, ImGuiColorEditFlags_::ImGuiColorEditFlags_InputRGB);
			ImGui::SliderFloat("Roughness", &_roughness, 0.f, 1.f);
			ImGui::SliderFloat("Metallic", &_metallic, 0.f, 1.f);
			// ライティングパス関連
			ImGui::ColorPicker3("LightColor", _dLightColor, ImGuiColorEditFlags_::ImGuiColorEditFlags_InputRGB);
			ImGui::SliderFloat3("LightDirection", _dLightDirection, -1.f, 1.f);
			ImGui::SliderFloat("LightIntensity", &_directionalLight.intensity, 0.f, 10.f);
			ImGui::RadioButton("CookTorrance", &_lightingParam.brdfModel, 0);
			ImGui::SameLine();
			ImGui::RadioButton("GGX(Trowbridge-Reitz)", &_lightingParam.brdfModel, 1);
			// IBLパス関連
			ImGui::SliderFloat("IBL_Intensity", &_iblParam.lightIntensity, 0.f, 10.f);
			ImGui::End();
			ImGui::Render();
		}

		// 1. GBuffer出力パス
		_gbufferPass.Render(scene.GetMeshActors());
		// 2. ライティングパス
		_lightingPass.Render();
		// 3. IBLパス
		_iblPass.Render();
		// 4. スカイボックスパス
		_skyBoxPass.Render();
		// 5. ライティング結果とスカイボックス描画結果を合成する
		_blendPass.Render();
	}
}
