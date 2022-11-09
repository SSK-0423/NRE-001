#include "GBufferPass.h"

#include "Dx12GraphicsEngine.h"

#include "RootSignature.h"
#include "GraphicsPipelineState.h"
#include "ShaderLibrary.h"
#include "InputLayout.h"
#include "RenderTarget.h"

#include "Actor.h"

using namespace NamelessEngine::Core;
using namespace NamelessEngine::DX12API;
using namespace NamelessEngine::Utility;

namespace NamelessEngine::Graphics
{
	GBufferPass::GBufferPass()
		: _rootSignature(new RootSignature()), _pipelineState(new GraphicsPipelineState())
	{
	}
	GBufferPass::~GBufferPass()
	{
	}
	void GBufferPass::Render(std::vector<Actor*>& actors)
	{
		RenderingContext& renderContext = Dx12GraphicsEngine::Instance().GetRenderingContext();
		renderContext.SetPipelineState(*_pipelineState);
		renderContext.SetGraphicsRootSignature(*_rootSignature);

		// 深度はカラーのレンダーターゲットに含まれているのでレンダーターゲット数は-1する
		RenderTarget renderTargets[static_cast<UINT>(GBUFFER_TYPE::GBUFFER_TYPE_NUM) - 1];
		for (size_t index = 0; index < _countof(renderTargets); index++) {
			renderTargets[index] = *_renderTargets[static_cast<GBUFFER_TYPE>(index)];
		}

		RenderTarget::BeginMultiRendering(
			renderTargets, _countof(renderTargets), renderContext, _viewport, _scissorRect);
		{
			if (!actors.empty()) {
				for (auto mesh : actors) {
					mesh->Draw(renderContext);
				}
			}
		}
		RenderTarget::EndMultiRendering(renderTargets, _countof(renderTargets), renderContext);
	}

	Utility::RESULT GBufferPass::CreateRootSignature(ID3D12Device& device)
	{
		RootSignatureData rootSigData;
		rootSigData._descRangeData.cbvDescriptorNum = 3;
		rootSigData._descRangeData.srvDescriptorNum = 1;

		Utility::RESULT result = _rootSignature->Create(device, rootSigData);
		if (result == Utility::RESULT::FAILED) { return result; }
	}
	Utility::RESULT GBufferPass::CreateRenderTarget(ID3D12Device& device)
	{
		SIZE windowSize = AppWindow::GetWindowSize();

		RenderTargetData data;
		data.depthStencilBufferData.width = windowSize.cx;
		data.depthStencilBufferData.height = windowSize.cy;

		data.renderTargetBufferData.clearColor[0] = 0.f;
		data.renderTargetBufferData.clearColor[1] = 0.f;
		data.renderTargetBufferData.clearColor[2] = 0.f;
		data.renderTargetBufferData.clearColor[3] = 1.f;
		data.renderTargetBufferData.colorFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
		data.renderTargetBufferData.width = windowSize.cx;
		data.renderTargetBufferData.height = windowSize.cy;
		data.useDepth = true;	// 深度は一つでよいのでカラー出力のレンダーターゲットのみで使用する

		_renderTargets[GBUFFER_TYPE::COLOR] = std::make_unique<DX12API::RenderTarget>();
		_renderTargets[GBUFFER_TYPE::NORMAL] = std::make_unique<DX12API::RenderTarget>();
		_renderTargets[GBUFFER_TYPE::WORLD_POS] = std::make_unique<DX12API::RenderTarget>();
		_renderTargets[GBUFFER_TYPE::METAL_ROUGH_REFLECT] = std::make_unique<DX12API::RenderTarget>();

		RESULT result = _renderTargets[GBUFFER_TYPE::COLOR]->Create(device, data);
		if (result == RESULT::FAILED) { return result; }

		data.useDepth = false;
		result = _renderTargets[GBUFFER_TYPE::NORMAL]->Create(device, data);
		if (result == RESULT::FAILED) { return result; }

		result = _renderTargets[GBUFFER_TYPE::WORLD_POS]->Create(device, data);
		if (result == RESULT::FAILED) { return result; }

		result = _renderTargets[GBUFFER_TYPE::METAL_ROUGH_REFLECT]->Create(device, data);
		if (result == RESULT::FAILED) { return result; }

		return result;
	}
	Texture& GBufferPass::GetGBuffer(GBUFFER_TYPE type)
	{
		if (type == GBUFFER_TYPE::DEPTH) {
			return _renderTargets[GBUFFER_TYPE::COLOR]->GetDepthStencilTexture();
		}
		return _renderTargets[type]->GetRenderTargetTexture();
	}
	Utility::RESULT GBufferPass::Init()
	{
		ID3D12Device& device = Dx12GraphicsEngine::Instance().Device();

		RESULT result = CreateRootSignature(device);
		if (result == RESULT::FAILED) {
			MessageBox(NULL, L"ルートシグネチャ生成失敗", L"エラーメッセージ", MB_OK);
		}
		result = CreateGraphicsPipelineState(device);
		if (result == RESULT::FAILED) {
			MessageBox(NULL, L"グラフィクスパイプライン生成失敗", L"エラーメッセージ", MB_OK);
		}
		result = CreateRenderTarget(device);
		if (result == RESULT::FAILED) {
			MessageBox(NULL, L"レンダーターゲット生成失敗", L"エラーメッセージ", MB_OK);
		}

		SIZE windowSize = AppWindow::GetWindowSize();

		_viewport = CD3DX12_VIEWPORT(
			0.f, 0.f, static_cast<float>(windowSize.cx), static_cast<float>(windowSize.cy));
		_scissorRect = CD3DX12_RECT(0, 0, windowSize.cx, windowSize.cy);

		return result;
	}
	Utility::RESULT GBufferPass::CreateGraphicsPipelineState(ID3D12Device& device)
	{
		// ルートシグネチャとシェーダーセット
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineState = {};
		pipelineState.pRootSignature = &_rootSignature->GetRootSignature();
		pipelineState.VS = CD3DX12_SHADER_BYTECODE(&ShaderLibrary::Instance().GetShader("GBufferVS")->GetShader());
		pipelineState.PS = CD3DX12_SHADER_BYTECODE(&ShaderLibrary::Instance().GetShader("GBufferPS")->GetShader());

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
		pipelineState.NumRenderTargets = 4;
		pipelineState.RTVFormats[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;	// カラー
		pipelineState.RTVFormats[1] = DXGI_FORMAT_R32G32B32A32_FLOAT;	// 法線
		pipelineState.RTVFormats[2] = DXGI_FORMAT_R32G32B32A32_FLOAT;	// ワールド座標
		pipelineState.RTVFormats[3] = DXGI_FORMAT_R32G32B32A32_FLOAT;	// メタリック/ラフネス/リフレクション

		// アンチエイリアシングのためのサンプル数設定
		pipelineState.SampleDesc.Count = 1;			// サンプリングは1ピクセルにつき1
		pipelineState.SampleDesc.Quality = 0;		// クオリティは最低

		return _pipelineState->Create(device, pipelineState);
	}
}
