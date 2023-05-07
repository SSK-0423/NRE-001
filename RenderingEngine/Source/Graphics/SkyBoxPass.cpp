#include "SkyBoxPass.h"

#include "ShaderResourceViewDesc.h"
#include "DescriptorHeapCBV_SRV_UAV.h"
#include "GraphicsPipelineState.h"
#include "RootSignature.h"
#include "ShaderLibrary.h"
#include "RenderTarget.h"
#include "ConstantBuffer.h"

#include "Mesh.h"
#include "Transform.h"

#include "CubeMesh.h"

#include "Camera.h"

#include "Dx12GraphicsEngine.h"

using namespace NamelessEngine::Core;
using namespace NamelessEngine::DX12API;
using namespace NamelessEngine::Component;
using namespace NamelessEngine::Graphics;
using namespace NamelessEngine::Scene;
using namespace NamelessEngine::Utility;

constexpr UINT CUBETEX_INDEX = 0;

constexpr UINT CAMERA_BUFFER_INDEX = 0;
constexpr UINT TRANSFORM_BUFFER_INDEX = 1;

namespace NamelessEngine::Graphics
{
	SkyBoxPass::SkyBoxPass()
		: _rootSignature(new RootSignature()), _pipelineState(new GraphicsPipelineState()),
		_renderTarget(new RenderTarget()), _descriptorHeap(new DescriptorHeapCBV_SRV_UAV()),
		_skyBox(new Mesh()), _transform(new Transform())
	{
	}
	SkyBoxPass::~SkyBoxPass()
	{
	}

	Utility::RESULT SkyBoxPass::Init()
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
		result = CreateDescriptorHeap(device);
		if (result == RESULT::FAILED) {
			MessageBox(NULL, L"ディスクリプタヒープ生成失敗", L"エラーメッセージ", MB_OK);
		}
		MeshData data = CubeMesh::CreateMeshData();
		result = _skyBox->CreateCube(device);
		if (result == RESULT::FAILED) {
			MessageBox(NULL, L"スカイボックスメッシュ生成失敗", L"エラーメッセージ", MB_OK);
		}

		_transform->SetScalling(1000, 1000, 1000);
		_skyBox->SetConstantBuffer(
			Dx12GraphicsEngine::Instance().Device(), _transform->GetConstantBuffer(), TRANSFORM_BUFFER_INDEX);

		SIZE windowSize = AppWindow::GetWindowSize();

		_viewport = CD3DX12_VIEWPORT(0.f, 0.f, static_cast<float>(windowSize.cx), static_cast<float>(windowSize.cy));
		_scissorRect = CD3DX12_RECT(0, 0, windowSize.cx, windowSize.cy);

		return result;
	}

	Utility::RESULT SkyBoxPass::CreateGraphicsPipelineState(ID3D12Device& device)
	{
		// ルートシグネチャとシェーダーセット
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineState = {};
		pipelineState.pRootSignature = &_rootSignature->GetRootSignature();
		pipelineState.VS = CD3DX12_SHADER_BYTECODE(&ShaderLibrary::Instance().GetShader("SkyBoxVS")->GetShader());
		pipelineState.PS = CD3DX12_SHADER_BYTECODE(&ShaderLibrary::Instance().GetShader("SkyBoxPS")->GetShader());

		// サンプルマスク設定
		pipelineState.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		// ブレンド
		pipelineState.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

		// ラスタライズ設定
		pipelineState.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		pipelineState.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

		// インプットレイアウトの設定
		InputLayout layout = InputLayout::DefaultLayout();
		pipelineState.InputLayout.pInputElementDescs = layout.GetData();
		pipelineState.InputLayout.NumElements = layout.GetSize();
		pipelineState.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
		pipelineState.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		// デプスステンシル設定
		pipelineState.DepthStencilState.DepthEnable = false;
		pipelineState.DepthStencilState.StencilEnable = false;

		// レンダーターゲットの設定
		pipelineState.NumRenderTargets = 1;
		pipelineState.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

		// アンチエイリアシングのためのサンプル数設定
		pipelineState.SampleDesc.Count = 1;			// サンプリングは1ピクセルにつき1
		pipelineState.SampleDesc.Quality = 0;		// クオリティは最低

		return _pipelineState->Create(device, pipelineState);
	}
	Utility::RESULT SkyBoxPass::CreateRootSignature(ID3D12Device& device)
	{
		RootSignatureData rootSigData;
		rootSigData._descRangeData.cbvDescriptorNum = 2;
		// キューブテクスチャ、ライティング済みテクスチャ
		rootSigData._descRangeData.srvDescriptorNum = 2;

		Utility::RESULT result = _rootSignature->Create(device, rootSigData);
		if (result == Utility::RESULT::FAILED) { return result; }
		return Utility::RESULT::SUCCESS;
	}
	Utility::RESULT SkyBoxPass::CreateRenderTarget(ID3D12Device& device)
	{
		SIZE windowSize = AppWindow::GetWindowSize();

		RenderTargetData data;
		data.renderTargetBufferData.width = windowSize.cx;
		data.renderTargetBufferData.height = windowSize.cy;
		data.depthStencilBufferData.width = windowSize.cx;
		data.depthStencilBufferData.height = windowSize.cy;
		data.useDepth = false;

		return _renderTarget->Create(device, data);
	}
	Utility::RESULT SkyBoxPass::CreateDescriptorHeap(ID3D12Device& device)
	{
		return _descriptorHeap->Create(device);
	}

	void SkyBoxPass::Render()
	{
		_transform->Update(0.f);

		RenderingContext& renderContext = Dx12GraphicsEngine::Instance().GetRenderingContext();
		renderContext.SetPipelineState(*_pipelineState);
		renderContext.SetGraphicsRootSignature(*_rootSignature);

		_renderTarget->BeginRendering(renderContext, _viewport, _scissorRect);
		{
			_skyBox->Draw(renderContext);
		}
		_renderTarget->EndRendering(renderContext);
	}
	void SkyBoxPass::SetCubeTexture(DX12API::Texture& texture)
	{
		ShaderResourceViewDesc desc(texture, true);
		_skyBox->SetTexture(
			Dx12GraphicsEngine::Instance().Device(), texture, desc, CUBETEX_INDEX);
		_descriptorHeap->RegistShaderResource(
			Dx12GraphicsEngine::Instance().Device(), texture, desc, CUBETEX_INDEX
		);
	}
	void SkyBoxPass::SetCamera(Scene::Camera& camera)
	{
		_skyBox->SetConstantBuffer(
			Dx12GraphicsEngine::Instance().Device(), camera.GetConstantBuffer(), CAMERA_BUFFER_INDEX);
		_descriptorHeap->RegistConstantBuffer(
			Dx12GraphicsEngine::Instance().Device(), camera.GetConstantBuffer(), CAMERA_BUFFER_INDEX);
	}
	DX12API::Texture& SkyBoxPass::GetOffscreenTexture()
	{
		return _renderTarget->GetRenderTargetTexture();
	}
}
