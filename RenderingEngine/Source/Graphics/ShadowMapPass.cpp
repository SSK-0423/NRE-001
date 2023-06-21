#include "ShadowMapPass.h"

#include "Dx12GraphicsEngine.h"

#include "RootSignature.h"
#include "ShaderLibrary.h"
#include "InputLayout.h"
#include "RenderTarget.h"
#include "ConstantBuffer.h"
#include "GraphicsPipelineState.h"

#include "LightSource.h" 
#include "Mesh.h"

#include "Camera.h"

#include "Actor.h"

#include <DirectXMath.h>

using namespace DirectX;
using namespace NamelessEngine::Core;
using namespace NamelessEngine::DX12API;
using namespace NamelessEngine::Component;
using namespace NamelessEngine::Utility;

namespace NamelessEngine::Graphics {
	ShadowMapPass::ShadowMapPass()
		: _rootSignature(new RootSignature()), _pipelineState(new GraphicsPipelineState()),
		_lightViewProjBuffer(new ConstantBuffer())
	{
	}
	ShadowMapPass::~ShadowMapPass()
	{
	}
	Utility::RESULT ShadowMapPass::CreateGraphicsPipelineState(ID3D12Device& device)
	{
		// ルートシグネチャとシェーダーセット
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineState = {};
		pipelineState.pRootSignature = &_rootSignature->GetRootSignature();
		pipelineState.VS = CD3DX12_SHADER_BYTECODE(&ShaderLibrary::Instance().GetShader("ShadowMapVS")->GetShader());
		pipelineState.PS = CD3DX12_SHADER_BYTECODE(&ShaderLibrary::Instance().GetShader("ShadowMapPS")->GetShader());

		// サンプルマスク設定
		pipelineState.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		// ブレンド
		pipelineState.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

		// ラスタライズ設定
		pipelineState.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		pipelineState.RasterizerState.CullMode = D3D12_CULL_MODE_FRONT;

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
		pipelineState.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;	// カラー

		// アンチエイリアシングのためのサンプル数設定
		pipelineState.SampleDesc.Count = 1;			// サンプリングは1ピクセルにつき1
		pipelineState.SampleDesc.Quality = 0;		// クオリティは最低

		return _pipelineState->Create(device, pipelineState);
	}
	Utility::RESULT ShadowMapPass::CreateBuffer(ID3D12Device& device)
	{
		return _lightViewProjBuffer->Create(device, nullptr, sizeof(XMMATRIX));
	}
	Utility::RESULT ShadowMapPass::CreateRootSignature(ID3D12Device& device)
	{
		RootSignatureData rootSigData;
		rootSigData._descRangeData.cbvDescriptorNum = 3;
		rootSigData._descRangeData.srvDescriptorNum = 5;

		Utility::RESULT result = _rootSignature->Create(device, rootSigData);
		if (result == Utility::RESULT::FAILED) { return result; }

		return result;
	}
	Utility::RESULT ShadowMapPass::CreateRenderTarget(ID3D12Device& device)
	{
		SIZE windowSize = AppWindow::GetWindowSize();

		RenderTargetData data;
		data.depthStencilBufferData.width = windowSize.cx;
		data.depthStencilBufferData.height = windowSize.cy;

		data.renderTargetBufferData.clearColor[0] = 0.f;
		data.renderTargetBufferData.clearColor[1] = 0.f;
		data.renderTargetBufferData.clearColor[2] = 0.f;
		data.renderTargetBufferData.clearColor[3] = 1.f;
		data.renderTargetBufferData.colorFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		data.renderTargetBufferData.width = windowSize.cx;
		data.renderTargetBufferData.height = windowSize.cy;
		data.useDepth = true;

		_renderTarget = std::make_unique<DX12API::RenderTarget>();

		RESULT result = _renderTarget->Create(device, data);
		if (result == RESULT::FAILED) { return result; }

		return result;
	}
	Utility::RESULT ShadowMapPass::Init()
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
		result = CreateBuffer(device);
		if (result == RESULT::FAILED) {
			MessageBox(NULL, L"バッファー生成失敗", L"エラーメッセージ", MB_OK);
		}

		SIZE windowSize = AppWindow::GetWindowSize();

		_viewport = CD3DX12_VIEWPORT(
			0.f, 0.f, static_cast<float>(windowSize.cx), static_cast<float>(windowSize.cy));
		_scissorRect = CD3DX12_RECT(0, 0, windowSize.cx, windowSize.cy);

		return result;
	}
	void ShadowMapPass::UpdateLightViewProj(Scene::Camera& camera, const Component::DirectionalLight& directionalLight)
	{
		XMFLOAT3 lightDir = XMFLOAT3(directionalLight.direction);
		XMStoreFloat3(&lightDir, XMVector3Normalize(XMLoadFloat3(&lightDir)));
		if (XMVector3Equal(XMLoadFloat3(&lightDir), XMVectorZero())) {
			lightDir.y = 0.00001f;
		}

		XMFLOAT3 pos = camera.GetTransform().Position();
		float length = 0;
		XMStoreFloat(&length, XMVector3Length(XMLoadFloat3(&pos)));

		XMFLOAT3 lightPos = XMFLOAT3(0, 0, 0);
		XMStoreFloat3(&lightPos, XMVector3Normalize(XMLoadFloat3(&lightDir)) * 150.f);

		XMFLOAT3 upDir = XMFLOAT3A(0, 0, 1);
		XMMATRIX view =
			XMMatrixLookToLH(XMLoadFloat3(&lightPos), -1.f * XMLoadFloat3(&lightDir), XMLoadFloat3(&upDir));
		XMMATRIX proj = XMMatrixOrthographicLH(300.f, 300.f, camera.cameraNear, 500.f);
		XMMATRIX lightViewProj = view * proj;

		_lightViewProjBuffer->UpdateData(&lightViewProj);
	}
	void ShadowMapPass::Render(std::vector<Actor*>& actors)
	{
		RenderingContext& renderContext = Dx12GraphicsEngine::Instance().GetRenderingContext();
		renderContext.SetPipelineState(*_pipelineState);
		renderContext.SetGraphicsRootSignature(*_rootSignature);

		_renderTarget->BeginRendering(renderContext, _viewport, _scissorRect);
		{
			if (!actors.empty()) {
				for (auto mesh : actors) {
					mesh->Draw(renderContext);
				}
			}
		}
		_renderTarget->EndRendering(renderContext);
	}
	DX12API::Texture& ShadowMapPass::GetShadowMap()
	{
		return _renderTarget->GetDepthStencilTexture();
	}
	DX12API::ConstantBuffer& ShadowMapPass::GetLightViewProjBuffer()
	{
		return *_lightViewProjBuffer.get();
	}
}