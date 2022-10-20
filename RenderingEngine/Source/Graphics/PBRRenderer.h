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
		// ビューポート
		CD3DX12_VIEWPORT _viewport;
		// シザー矩形
		CD3DX12_RECT _scissorRect;
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