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
	PBRRenderer::PBRRenderer(unsigned int window_width, unsigned int window_height)
	{
		Utility::RESULT result = CreateGraphicsPipelineState(Dx12GraphicsEngine::Instance().Device());
		if (result == Utility::RESULT::FAILED) {
			MessageBox(NULL, L"PBRレンダラーパイプライン生成失敗", L"エラーメッセージ", MB_OK);
		}

		_scissorRect = CD3DX12_RECT(0, 0, window_width, window_height);
		_viewport = CD3DX12_VIEWPORT(
			0.f, 0.f, static_cast<FLOAT>(window_width), static_cast<FLOAT>(window_height));
	}
	PBRRenderer::~PBRRenderer()
	{
		SafetyDelete<GraphicsPipelineState>(_graphicsPipelineState);
		SafetyDelete<RootSignature>(_rootSignature);
	}
	Utility::RESULT PBRRenderer::CreateGraphicsPipelineState(ID3D12Device& device)
	{
		// ルートシグネチャ生成
		if (_rootSignature != nullptr) delete _rootSignature;
		_rootSignature = new DX12API::RootSignature();

		RootSignatureData rootSigData;
		rootSigData._descRangeData.cbvDescriptorNum = 2;
		rootSigData._descRangeData.srvDescriptorNum = 1; // base,metal,rough,ao,normal,cubetex

		Utility::RESULT result = _rootSignature->Create(device, rootSigData);
		if (result == Utility::RESULT::FAILED) { return result; }

		// ルートシグネチャとシェーダーセット
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineState = {};
		pipelineState.pRootSignature = &_rootSignature->GetRootSignature();
		pipelineState.VS = CD3DX12_SHADER_BYTECODE(&ShaderLibrary::Instance().GetShader("GeometryVS")->GetShader());
		pipelineState.PS = CD3DX12_SHADER_BYTECODE(&ShaderLibrary::Instance().GetShader("GeometryPS")->GetShader());

		// サンプルマスク設定
		pipelineState.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		// ブレンド
		pipelineState.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

		// ラスタライズ設定
		pipelineState.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		pipelineState.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;

		// インプットレイアウトの設定
		InputLayout layout = InputLayout::DefaultLayout();
		pipelineState.InputLayout.pInputElementDescs = layout.GetData();
		pipelineState.InputLayout.NumElements = layout.GetSize();
		pipelineState.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
		pipelineState.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		// デプスステンシル設定
		pipelineState.DepthStencilState.DepthEnable = true;
		pipelineState.DepthStencilState.StencilEnable = false;
		pipelineState.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		pipelineState.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		pipelineState.DSVFormat = DXGI_FORMAT_D32_FLOAT;

		// レンダーターゲットの設定
		pipelineState.NumRenderTargets = 1;
		pipelineState.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

		// アンチエイリアシングのためのサンプル数設定
		pipelineState.SampleDesc.Count = 1;	    // サンプリングは1ピクセルにつき1
		pipelineState.SampleDesc.Quality = 0;		// クオリティは最低

		// グラフィックスパイプラインステート生成
		if (_graphicsPipelineState != nullptr) delete _graphicsPipelineState;
		_graphicsPipelineState = new DX12API::GraphicsPipelineState();

		return _graphicsPipelineState->Create(device, pipelineState);
	}
	void PBRRenderer::Render(std::vector<Actor*>& _meshActors, std::vector<Actor*>& _guiActors)
	{
		RenderingContext& renderContext = Dx12GraphicsEngine::Instance().GetRenderingContext();
		renderContext.SetViewport(_viewport);
		renderContext.SetScissorRect(_scissorRect);
		renderContext.SetGraphicsRootSignature(*_rootSignature);
		renderContext.SetPipelineState(*_graphicsPipelineState);

		// メッシュ描画
		if (!_meshActors.empty()) {
			for (auto mesh : _meshActors) {
				mesh->Draw(renderContext);
			}
		}

		// GUI描画
		if (!_guiActors.empty()) {
			for (auto gui : _guiActors) {
				gui->Draw(renderContext);
			}
		}
	}
}
