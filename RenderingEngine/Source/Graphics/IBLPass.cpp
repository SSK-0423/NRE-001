#include "IBLPass.h"
#include "GBufferPass.h"

#include "RenderTarget.h"
#include "Texture.h"
#include "ConstantBuffer.h"
#include "ShaderResourceViewDesc.h"
#include "RootSignature.h"
#include "GraphicsPipelineState.h"
#include "ShaderLibrary.h"
#include "InputLayout.h"

#include "Dx12GraphicsEngine.h"

using namespace NamelessEngine::Core;
using namespace NamelessEngine::DX12API;
using namespace NamelessEngine::Utility;

constexpr UINT LIGHTED_INDEX = 5;
constexpr UINT DFG_INDEX = 6;
constexpr UINT SPECULAR_LD_INDEX = 7;
constexpr UINT DIFFUSE_LD_INDEX = 8;
constexpr UINT SHADOWFACT_INDEX = 9;

namespace NamelessEngine::Graphics
{
	IBLPass::IBLPass()
		: _rootSignature(new RootSignature()), _pipelineState(new GraphicsPipelineState()),
		_descriptorHeap(new DescriptorHeapCBV_SRV_UAV()), _paramBuffer(new ConstantBuffer()),
		_debugParamBuffer(new ConstantBuffer()), _renderTarget(new RenderTarget())
	{
	}
	IBLPass::~IBLPass()
	{
	}

	Utility::RESULT IBLPass::Init()
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
		result = CreateParamBuffer(device);
		if (result == RESULT::FAILED) {
			MessageBox(NULL, L"パラメーター用コンスタントバッファー生成失敗", L"エラーメッセージ", MB_OK);
		}
		_descriptorHeap->RegistConstantBuffer(device, *_paramBuffer, 0);
		_descriptorHeap->RegistConstantBuffer(device, *_debugParamBuffer, 1);

		SIZE windowSize = AppWindow::GetWindowSize();

		_viewport = CD3DX12_VIEWPORT(
			0.f, 0.f, static_cast<float>(windowSize.cx), static_cast<float>(windowSize.cy));
		_scissorRect = CD3DX12_RECT(0, 0, windowSize.cx, windowSize.cy);

		return result;
	}

	Utility::RESULT IBLPass::CreateRootSignature(ID3D12Device& device)
	{
		RootSignatureData rootSigData;
		rootSigData._descRangeData.cbvDescriptorNum = 2;
		rootSigData._descRangeData.srvDescriptorNum = 10;
		rootSigData._samplerData.samplerFilter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		rootSigData._samplerData.addressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		rootSigData._samplerData.addressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;

		Utility::RESULT result = _rootSignature->Create(device, rootSigData);
		if (result == Utility::RESULT::FAILED) { return result; }
		return Utility::RESULT::SUCCESS;
	}
	Utility::RESULT IBLPass::CreateGraphicsPipelineState(ID3D12Device& device)
	{
		// ルートシグネチャとシェーダーセット
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineState = {};
		pipelineState.pRootSignature = &_rootSignature->GetRootSignature();
		pipelineState.VS = CD3DX12_SHADER_BYTECODE(&ShaderLibrary::Instance().GetShader("IBLVS")->GetShader());
		pipelineState.PS = CD3DX12_SHADER_BYTECODE(&ShaderLibrary::Instance().GetShader("IBLPS")->GetShader());

		// サンプルマスク設定
		pipelineState.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		// ブレンド
		pipelineState.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

		// ラスタライズ設定
		pipelineState.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		pipelineState.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

		// インプットレイアウトの設定
		pipelineState.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
		pipelineState.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		// デプスステンシル設定
		pipelineState.DepthStencilState.DepthEnable = false;
		pipelineState.DepthStencilState.StencilEnable = false;

		// レンダーターゲットの設定
		pipelineState.NumRenderTargets = 1;
		pipelineState.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

		// アンチエイリアシングのためのサンプル数設定
		pipelineState.SampleDesc.Count = 1;			// サンプリングは1ピクセルにつき1
		pipelineState.SampleDesc.Quality = 0;		// クオリティは最低

		return _pipelineState->Create(device, pipelineState);
	}
	Utility::RESULT IBLPass::CreateRenderTarget(ID3D12Device& device)
	{
		SIZE windowSize = AppWindow::GetWindowSize();

		RenderTargetData data;
		data.renderTargetBufferData.width = windowSize.cx;
		data.renderTargetBufferData.height = windowSize.cy;
		data.renderTargetBufferData.colorFormat = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		data.depthStencilBufferData.width = windowSize.cx;
		data.depthStencilBufferData.height = windowSize.cy;
		data.useDepth = false;

		return _renderTarget->Create(device, data);
	}
	Utility::RESULT IBLPass::CreateParamBuffer(ID3D12Device& device)
	{
		if (_paramBuffer->Create(device, &_paramData, sizeof(IBLParam)) == Utility::RESULT::FAILED) {
			return Utility::RESULT::FAILED;
		}

		return _debugParamBuffer->Create(device, &_debugParamData, sizeof(DebugParam));
	}
	Utility::RESULT IBLPass::CreateDescriptorHeap(ID3D12Device& device)
	{
		return _descriptorHeap->Create(device);
	}

	void IBLPass::UpdateParamData(IBLParam& param, DebugParam& debugParam)
	{
		_paramData = param;
		_paramBuffer->UpdateData(&_paramData);
		_debugParamData = debugParam;
		_debugParamBuffer->UpdateData(&_debugParamData);
	}
	void IBLPass::Render()
	{
		RenderingContext& renderContext = Dx12GraphicsEngine::Instance().GetRenderingContext();

		renderContext.SetPipelineState(*_pipelineState);
		renderContext.SetGraphicsRootSignature(*_rootSignature);

		_renderTarget->BeginRendering(renderContext, _viewport, _scissorRect);
		{
			renderContext.SetDescriptorHeap(*_descriptorHeap);
			renderContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			renderContext.DrawInstanced(4, 1, 0, 0);
		}
		_renderTarget->EndRendering(renderContext);
	}
	void IBLPass::SetGBuffer(GBUFFER_TYPE type, DX12API::Texture& texture)
	{
		// 深度は扱わないのでセットしない
		if (type == GBUFFER_TYPE::DEPTH) return;

		ShaderResourceViewDesc desc(texture);
		_descriptorHeap->RegistShaderResource(
			Dx12GraphicsEngine::Instance().Device(), texture, desc, static_cast<int>(type));
	}
	void IBLPass::SetLightedTexture(DX12API::Texture& texture)
	{
		ShaderResourceViewDesc desc(texture, false);
		_descriptorHeap->RegistShaderResource(
			Dx12GraphicsEngine::Instance().Device(), texture, desc, LIGHTED_INDEX);
	}
	void IBLPass::SetIBLTextures(
		DX12API::Texture& specularLD, DX12API::Texture& diffuseLD, DX12API::Texture& dfg)
	{
		ShaderResourceViewDesc specDesc(specularLD, true);
		_descriptorHeap->RegistShaderResource(
			Dx12GraphicsEngine::Instance().Device(), specularLD, specDesc, SPECULAR_LD_INDEX);

		ShaderResourceViewDesc diffDesc(diffuseLD, true);
		_descriptorHeap->RegistShaderResource(
			Dx12GraphicsEngine::Instance().Device(), diffuseLD, diffDesc, DIFFUSE_LD_INDEX);

		ShaderResourceViewDesc dfgDesc(dfg, false);
		_descriptorHeap->RegistShaderResource(
			Dx12GraphicsEngine::Instance().Device(), dfg, dfgDesc, DFG_INDEX);
	}
	void IBLPass::SetShadowFactorTex(DX12API::Texture& texture)
	{
		ShaderResourceViewDesc desc(texture);
		_descriptorHeap->RegistShaderResource(
			Dx12GraphicsEngine::Instance().Device(), texture, desc, SHADOWFACT_INDEX);
	}
	DX12API::Texture& IBLPass::GetOffscreenTexture()
	{
		return _renderTarget->GetRenderTargetTexture();
	}
}
