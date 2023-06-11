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
#include "Mesh.h"

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
		result = _shadowMapPass.Init();
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
		//result = _environment->CreateCubeTextureFromDDS(
		//	Dx12GraphicsEngine::Instance(), L"res/clarens_night_01/clarens_night_01EnvHDR.dds");
		result = _environment->CreateCubeTextureFromDDS(
			Dx12GraphicsEngine::Instance(), L"Assets/IBL/iblEnvHDR.dds");
		if (result == RESULT::FAILED) { return result; }

		_specularLD = std::make_unique<DX12API::Texture>();
		//result = _specularLD->CreateCubeTextureFromDDS(
		//	Dx12GraphicsEngine::Instance(), L"res/clarens_night_01/clarens_night_01SpecularHDR.dds");
		result = _specularLD->CreateCubeTextureFromDDS(
			Dx12GraphicsEngine::Instance(), L"Assets/IBL/iblSpecularHDR.dds");
		if (result == RESULT::FAILED) { return result; }

		_diffuseLD = std::make_unique<DX12API::Texture>();
		//result = _diffuseLD->CreateCubeTextureFromDDS(
		//	Dx12GraphicsEngine::Instance(), L"res/clarens_night_01/clarens_night_01DiffuseHDR.dds");
		result = _diffuseLD->CreateCubeTextureFromDDS(
			Dx12GraphicsEngine::Instance(), L"Assets/IBL/iblDiffuseHDR.dds");
		if (result == RESULT::FAILED) { return result; }

		_DFG = std::make_unique<DX12API::Texture>();
		//result = _DFG->CreateTextureFromDDS(
		//	Dx12GraphicsEngine::Instance(), L"res/clarens_night_01/clarens_night_01Brdf.dds");
		result = _DFG->CreateTextureFromDDS(
			Dx12GraphicsEngine::Instance(), L"Assets/IBL/iblBrdf.dds");
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

		ID3D12Device& device = Dx12GraphicsEngine::Instance().Device();
		// ライト行列のバッファーをセット
		for (auto actor : scene.GetMeshActors()) {
			actor->GetComponent<Mesh>()->SetConstantBufferOnAllSubMeshes(
				device, _shadowMapPass.GetLightViewProjBuffer(), 2);
		}
	}
	void PBRRenderer::Update(float deltatime, Scene::Scene& scene)
	{
		_shadowMapPass.UpdateLightViewProj(scene.GetCamera(), _directionalLight);

		_lightingParam.eyePosition = scene.GetCamera().GetTransform().Position();
		_lightingPass.UpdateParamData(_lightingParam);

		_lightingPass.UpdateDirectionalLight(_directionalLight);

		_iblParam.eyePosition = scene.GetCamera().GetTransform().Position();
		_iblPass.UpdateParamData(_iblParam, _debugParam);

		//Material* material = scene.GetMeshActors()[scene.GetMeshActors().size() - 1]->GetComponent<Material>();
		//material->baseColor = DirectX::XMFLOAT4(_baseColor[0], _baseColor[1], _baseColor[2], 1.f);
		//material->roughness = _roughness;
		//material->metallic = _metallic;
		//material->Build();
	}
	void PBRRenderer::Render(Scene::Scene& scene)
	{
		// Imguiレンダー
		{
			ImGui::SetNextWindowPos(ImVec2(900, 0));
			ImGui::Begin("Editor", 0, ImGuiWindowFlags_NoMove);
			ImGui::SetWindowSize(ImVec2(375, AppWindow::GetWindowSize().cy), ImGuiCond_Once);

			// GBufferパス関連
			ImGui::ColorPicker3("BaseColor", _baseColor, ImGuiColorEditFlags_::ImGuiColorEditFlags_InputRGB);
			ImGui::SliderFloat("Roughness", &_roughness, 0.f, 1.f);
			ImGui::SliderFloat("Metallic", &_metallic, 0.f, 1.f);
			// ライティングパス関連
			ImGui::ColorPicker3("LightColor", _directionalLight.color, ImGuiColorEditFlags_::ImGuiColorEditFlags_InputRGB);
			ImGui::SliderFloat3("LightDirection", _directionalLight.direction, -1.f, 1.f);
			ImGui::SliderFloat("LightIntensity", &_directionalLight.intensity, 0.f, 10.f);
			ImGui::RadioButton("CookTorrance", &_lightingParam.brdfModel, static_cast<int>(BRDF_MODEL::COOK_TORRANCE));
			ImGui::SameLine();
			ImGui::RadioButton("GGX(Trowbridge-Reitz)", &_lightingParam.brdfModel, static_cast<int>(BRDF_MODEL::GGX));
			// IBLパス関連
			ImGui::SliderFloat("IBL_Intensity", &_iblParam.lightIntensity, 0.f, 10.f);
			ImGui::Checkbox("IBL Only", &_iblParam.isIBLOnly);

			// デバッグ用(IBLパスに実装)
			ImGui::Text("Debug Menu");
			ImGui::RadioButton("Lighting", &_debugParam.debugDrawMode, static_cast<int>(DEBUG_DRAW_MODE::LIGHTING));
			ImGui::SameLine();
			ImGui::RadioButton("BaseColor", &_debugParam.debugDrawMode, static_cast<int>(DEBUG_DRAW_MODE::BASECOLOR));
			ImGui::SameLine();
			ImGui::RadioButton("Metal", &_debugParam.debugDrawMode, static_cast<int>(DEBUG_DRAW_MODE::METALLIC));
			ImGui::SameLine();
			ImGui::RadioButton("Rough", &_debugParam.debugDrawMode, static_cast<int>(DEBUG_DRAW_MODE::ROUGHNESS));

			ImGui::RadioButton("Normal", &_debugParam.debugDrawMode, static_cast<int>(DEBUG_DRAW_MODE::NORMAL));
			ImGui::SameLine();
			ImGui::RadioButton("EmissiveColor", &_debugParam.debugDrawMode, static_cast<int>(DEBUG_DRAW_MODE::EMISSIVECOLOR));
			ImGui::SameLine();
			ImGui::RadioButton("Occlusion", &_debugParam.debugDrawMode, static_cast<int>(DEBUG_DRAW_MODE::OCCLUSION));

			ImGui::End();
			ImGui::Render();
		}
		// 1. シャドウマップ生成パス
		_shadowMapPass.Render(scene.GetMeshActors());
		// 2. GBuffer生成パス
		_gbufferPass.Render(scene.GetMeshActors());
		// 3. ライティングパス
		_lightingPass.Render();
		// 4. IBLパス
		_iblPass.Render();
		// 5. スカイボックスパス
		_skyBoxPass.Render();
		// 6. ライティング結果とスカイボックス描画結果を合成する
		_blendPass.Render();
	}
}
