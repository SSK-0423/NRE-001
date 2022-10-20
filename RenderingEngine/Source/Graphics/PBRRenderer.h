#pragma once
#include "IRenderer.h"
#include "GraphicsPipelineState.h"
#include "RootSignature.h"

#include <d3dx12.h>

namespace NamelessEngine::Graphics {
	class PBRRenderer : public IRenderer {
	public:
		PBRRenderer(unsigned int window_width, unsigned int window_height);
		~PBRRenderer();

	private:
		// �r���[�|�[�g
		CD3DX12_VIEWPORT _viewport;
		// �V�U�[��`
		CD3DX12_RECT _scissorRect;
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