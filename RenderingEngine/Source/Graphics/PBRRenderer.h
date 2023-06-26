#pragma once
#include "IRenderer.h"

#include "GBufferPass.h"
#include "ShadowMapPass.h"
#include "ShadowingPass.h"
#include "LightingPass.h"
#include "SkyBoxPass.h"
#include "BlendPass.h"
#include "IBLPass.h"

#include "LightSource.h"

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
		ShadowMapPass _shadowMapPass;
		ShadowingPass _shadowingPass;
		LightingPass _lightingPass;
		IBLPass _iblPass;
		SkyBoxPass _skyBoxPass;
		BlendPass _blendPass;

		std::unique_ptr<DX12API::Texture> _environment;
		std::unique_ptr<DX12API::Texture> _specularLD;
		std::unique_ptr<DX12API::Texture> _diffuseLD;
		std::unique_ptr<DX12API::Texture> _DFG;

		// ���C�e�B���O�p�X�Ŏg�p����o�b�t�@�̍\����
		LightingParam _lightingParam;
		Component::DirectionalLight _directionalLight;
		float _dLightColor[3];
		float _dLightDirection[3];

		// IBL�p�X�Ŏg�p����o�b�t�@�̍\����
		IBLParam _iblParam;
		DebugParam _debugParam;

		// �V���h�E�}�b�v�p�X�̃o�b�t�@
		ShadowMapParam _shadowMapParam;


	public:
		Utility::RESULT Init(Scene::Scene& scene) override;
		void Update(float deltatime, Scene::Scene& scene) override;
		void Render(Scene::Scene& scene) override;
	};
}