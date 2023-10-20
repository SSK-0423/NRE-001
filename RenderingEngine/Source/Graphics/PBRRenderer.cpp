#include <d3dx12.h>

#include "Dx12GraphicsEngine.h"

#include "PBRRenderer.h"
#include "InputLayout.h"
#include "ShaderLibrary.h"
#include "Texture.h"
#include "ShaderResourceViewDesc.h"
#include "DescriptorHeapCBV_SRV_UAV.h"

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
		result = _shadowingPass.Init();
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
			Dx12GraphicsEngine::Instance(), L"Assets/IBL/iblEnvHDR.dds");
		if (result == RESULT::FAILED) { return result; }

		_specularLD = std::make_unique<DX12API::Texture>();
		result = _specularLD->CreateCubeTextureFromDDS(
			Dx12GraphicsEngine::Instance(), L"Assets/IBL/iblSpecularHDR.dds");
		if (result == RESULT::FAILED) { return result; }

		_diffuseLD = std::make_unique<DX12API::Texture>();
		result = _diffuseLD->CreateCubeTextureFromDDS(
			Dx12GraphicsEngine::Instance(), L"Assets/IBL/iblDiffuseHDR.dds");
		if (result == RESULT::FAILED) { return result; }

		_DFG = std::make_unique<DX12API::Texture>();
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
		_iblPass.SetShadowFactorTex(_shadowingPass.GetShadowFactorTexture());
		_iblPass.SetShadowMap(_shadowMapPass.GetShadowMap());

		_shadowingPass.SetWorldPosTexture(_gbufferPass.GetGBuffer(GBUFFER_TYPE::WORLD_POS));
		_shadowingPass.SetShadowMap(_shadowMapPass.GetShadowMap());
		_shadowingPass.SetLightViewProjBuffer(_shadowMapPass.GetLightViewProjBuffer());

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

		auto heap = Dx12GraphicsEngine::Instance().GetImguiDescriptorHeap();
		ShaderResourceViewDesc finalRenderDesc(_blendPass.GetOffscreenTexture());
		heap.RegistShaderResource(device, _blendPass.GetOffscreenTexture(), finalRenderDesc);
		ShaderResourceViewDesc shadowMapDesc(_shadowMapPass.GetShadowMap());
		heap.RegistShaderResource(device, _shadowMapPass.GetShadowMap(), shadowMapDesc);

		return result;
	}
	void PBRRenderer::Update(float deltatime, Scene::Scene& scene)
	{
		_shadowMapPass.UpdateLightViewProj(scene.GetCamera(), _shadowMapParam, _directionalLight);

		_shadowingPass.UpdateBias(_bias);

		_lightingParam.eyePosition = scene.GetCamera().GetTransform().Position();
		_lightingPass.UpdateParamData(_lightingParam);

		_lightingPass.UpdateDirectionalLight(_directionalLight);

		_iblParam.eyePosition = scene.GetCamera().GetTransform().Position();
		_iblPass.UpdateParamData(_iblParam, _debugParam);
	}
	void PBRRenderer::Render(Scene::Scene& scene)
	{
		// 1. シャドウマップ生成パス
		_shadowMapPass.Render(scene.GetMeshActors());
		// 2. GBuffer生成パス
		_gbufferPass.Render(scene.GetMeshActors());
		// 3. シャドウイングパス
		_shadowingPass.Render();
		// 4. ライティングパス
		_lightingPass.Render();
		// 5. IBLパス
		_iblPass.Render();
		// 6. スカイボックスパス
		_skyBoxPass.Render(scene.GetCamera());
		// 7. ライティング結果とスカイボックス描画結果を合成する
		_blendPass.Render();

		// Imguiレンダー
		{
			ImGui::DockSpaceOverViewport(ImGui::GetWindowViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

			// ライティング用エディタ
			//ImGui::SetNextWindowPos(ImVec2(AppWindow::GetWindowSize().cx - ImGui::GetWindowWidth(), 0));
			//ImGui::SetNextWindowSize(ImVec2(400, 475));
			ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.0f, 0.1f, 0.71f, 1.f));
			ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.0f, 0.1f, 0.91f, 1.f));
			ImGui::Begin("Light Property", 0);
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
			ImGui::End();

			// デバッグ用エディタ
			//ImGui::SetNextWindowPos(ImVec2(AppWindow::GetWindowSize().cx - ImGui::GetWindowWidth(), AppWindow::GetWindowSize().cy / 4 * 3));
			ImGui::Begin("G-Buffer", 0);
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

			// シャドウイング用エディタ
			//ImGui::SetNextWindowPos(ImVec2(0, AppWindow::GetWindowSize().cy - ImGui::GetWindowHeight()));
			//ImGui::SetNextWindowSize(ImVec2(1980, 1080));
			ImGui::Begin("Shadow Property");
			ImGui::SliderFloat("LightDistance", &_shadowMapParam.lightDistance, 1.f, scene.GetCamera().cameraFar);
			ImGui::SliderFloat("Near", &_shadowMapParam.nearZ, 0.1f, 10.f);
			ImGui::SliderFloat("Far", &_shadowMapParam.farZ, 1.f, scene.GetCamera().cameraFar);
			ImGui::SliderFloat("ViewWidth", &_shadowMapParam.viewWidth, 1.f, 1000.f);
			ImGui::SliderFloat("ViewHeight", &_shadowMapParam.viewHeight, 1.f, 1000.f);
			ImGui::SliderFloat("Bias", &_bias, 0.f, 0.1f);
			//ImGui::RadioButton("ShadowMap", &_debugParam.debugDrawMode, static_cast<int>(DEBUG_DRAW_MODE::SHADOWMAP));

			auto imguiHeap = Dx12GraphicsEngine::Instance().GetImguiDescriptorHeap();
			// TODO: シャドウマップを表示する
			ImGui::Image((ImTextureID)imguiHeap.GetSRVHandle(1).ptr, ImGui::GetWindowSize());
			ImGui::End();

			// Viewport
			ImGui::Begin("Viewport");
			ImGui::Image((ImTextureID)imguiHeap.GetSRVHandle(0).ptr, ImGui::GetWindowViewport()->WorkSize);
			ImGui::End();

			ImGui::ShowMetricsWindow();

			// パフォーマンス
			//ImGui::SetNextWindowSize(ImVec2(200, 50));
			ImGui::Begin("Performance", 0, ImGuiWindowFlags_None);
			ImGui::Text("FrameRate %.2f", ImGui::GetIO().Framerate);
			ImGui::End();

			ImGui::Begin("SceneHierarchy", 0, ImGuiWindowFlags_None);
			for (auto& actor : scene.GetMeshActors())
			{
				Transform* transform = actor->GetComponent<Transform>();
				DirectX::XMFLOAT3 position = transform->Position();
				DirectX::XMFLOAT3 rotation = transform->DegreeAngle();
				DirectX::XMFLOAT3 scale = transform->Scale();

				if (ImGui::TreeNode(actor->GetActorName().c_str())) {

					ImGui::PushItemWidth(200);
					ImGui::InputFloat3("Position", &position.x);

					ImGui::InputFloat3("Rotation", &rotation.x);

					ImGui::InputFloat3("Scale   ", &scale.x);

					ImGui::TreePop();
				}
				transform->SetPosition(position.x,position.y,position.z);
				transform->SetDegreeAngle(rotation.x, rotation.y, rotation.z);
				transform->SetScalling(scale.x, scale.y, scale.z);
			}
			ImGui::End();

			ImGui::PopStyleColor();
			ImGui::PopStyleColor();

			ImGui::Render();
		}
	}
}
