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
		// ���C�e�B���O�p�X��GBuffer���Z�b�g
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
		// 1. GBuffer�o�̓p�X
		_gbufferPass.Render(_meshActors);
		// 2. ���C�e�B���O�p�X
		_lightingPass.Render();
		// 3. �|�X�g�G�t�F�N�g�p�X
		// _postEffectPass.Render(_meshActors);
		// 4. GUI�p�X
		// _guiPass.Render(_guiActors);
	}
}
