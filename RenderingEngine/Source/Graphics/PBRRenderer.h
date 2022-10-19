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
		// ルートシグネチャ
		DX12API::RootSignature* _rootSignature = nullptr;
		// パイプライン
		DX12API::GraphicsPipelineState* _graphicsPipelineState = nullptr;
		// パイプラインとルートシグネチャ生成
		Utility::RESULT CreateGraphicsPipelineState(ID3D12Device& device);

	public:
		void Render(std::vector<Actor*>& _meshActors, std::vector<Actor*>& _guiActors);
	};
}