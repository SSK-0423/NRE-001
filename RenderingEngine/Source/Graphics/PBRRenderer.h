#pragma once
#include "IRenderer.h"
#include "GraphicsPipelineState.h"
#include "RootSignature.h"

namespace NamelessEngine::Graphics {
	class PBRRenderer : public IRenderer {
	public:
		PBRRenderer();
		~PBRRenderer();

	private:
		// ���[�g�V�O�l�`��
		DX12API::RootSignature* _rootSignature = nullptr;
		// �p�C�v���C��
		DX12API::GraphicsPipelineState* _graphicsPipelineState = nullptr;
		// �p�C�v���C���ƃ��[�g�V�O�l�`������
		Utility::RESULT CreateGraphicsPipelineState(ID3D12Device& device);

	public:
		void Render(std::vector<Actor*>& _meshActors, std::vector<Actor*>& _guiActors);
	};
}