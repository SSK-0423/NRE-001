#include "ShadowingPass.h"

#include "Dx12GraphicsEngine.h"

#include "RootSignature.h"
#include "ShaderLibrary.h"
#include "RenderTarget.h"
#include "ConstantBuffer.h"
#include "GraphicsPipelineState.h"
#include "DescriptorHeapCBV_SRV_UAV.h"
#include "ConstantBuffer.h"
#include "Texture.h"
#include "ShaderResourceViewDesc.h"

#include "Camera.h"

constexpr UINT WORLDPOS_INDEX = 0;
constexpr UINT SHADOWMAP_INDEX = 1;
constexpr UINT LIGHTVIEWPROJ_INDEX = 0;
constexpr UINT BIAS_INDEX = 1;

using namespace NamelessEngine::Core;
using namespace NamelessEngine::DX12API;
using namespace NamelessEngine::Utility;

namespace NamelessEngine::Graphics {
	ShadowingPass::ShadowingPass()
		: _rootSignature(new RootSignature()), _pipelineState(new GraphicsPipelineState()),
		_descriptorHeap(new DescriptorHeapCBV_SRV_UAV()), _renderTarget(new RenderTarget()),
		_biasBuffer(new ConstantBuffer())
	{
	}
	ShadowingPass::~ShadowingPass()
	{
	}
	Utility::RESULT ShadowingPass::CreateRenderTarget(ID3D12Device& device)
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
	Utility::RESULT ShadowingPass::CreateRootSignature(ID3D12Device& device)
	{
		RootSignatureData rootSigData;
		rootSigData._descRangeData.cbvDescriptorNum = 2;
		rootSigData._descRangeData.srvDescriptorNum = 2;

		Utility::RESULT result = _rootSignature->Create(device, rootSigData);
		if (result == Utility::RESULT::FAILED) { return result; }
		return Utility::RESULT::SUCCESS;
	}
	Utility::RESULT ShadowingPass::CreateGraphicsPipelineState(ID3D12Device& device)
	{
		// ���[�g�V�O�l�`���ƃV�F�[�_�[�Z�b�g
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineState = {};
		pipelineState.pRootSignature = &_rootSignature->GetRootSignature();
		pipelineState.VS = CD3DX12_SHADER_BYTECODE(&ShaderLibrary::Instance().GetShader("ShadowVS")->GetShader());
		pipelineState.PS = CD3DX12_SHADER_BYTECODE(&ShaderLibrary::Instance().GetShader("ShadowPS")->GetShader());

		// �T���v���}�X�N�ݒ�
		pipelineState.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		// �u�����h
		pipelineState.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

		// ���X�^���C�Y�ݒ�
		pipelineState.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		pipelineState.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

		// �C���v�b�g���C�A�E�g�̐ݒ�
		pipelineState.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
		pipelineState.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		// �f�v�X�X�e���V���ݒ�
		pipelineState.DepthStencilState.DepthEnable = false;
		pipelineState.DepthStencilState.StencilEnable = false;

		// �����_�[�^�[�Q�b�g�̐ݒ�
		pipelineState.NumRenderTargets = 1;
		pipelineState.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

		// �A���`�G�C���A�V���O�̂��߂̃T���v�����ݒ�
		pipelineState.SampleDesc.Count = 1;			// �T���v�����O��1�s�N�Z���ɂ�1
		pipelineState.SampleDesc.Quality = 0;		// �N�I���e�B�͍Œ�

		return _pipelineState->Create(device, pipelineState);
	}
	Utility::RESULT ShadowingPass::CreateDescriptorHeap(ID3D12Device& device)
	{
		return _descriptorHeap->Create(device);
	}
	Utility::RESULT ShadowingPass::CreateBuffer(ID3D12Device& device)
	{
		return _biasBuffer->Create(device, nullptr, sizeof(float));
	}
	Utility::RESULT ShadowingPass::Init()
	{
		ID3D12Device& device = Dx12GraphicsEngine::Instance().Device();

		RESULT result = CreateRootSignature(device);
		if (result == RESULT::FAILED) {
			MessageBox(NULL, L"���[�g�V�O�l�`���������s", L"�G���[���b�Z�[�W", MB_OK);
		}
		result = CreateGraphicsPipelineState(device);
		if (result == RESULT::FAILED) {
			MessageBox(NULL, L"�O���t�B�N�X�p�C�v���C���������s", L"�G���[���b�Z�[�W", MB_OK);
		}
		result = CreateRenderTarget(device);
		if (result == RESULT::FAILED) {
			MessageBox(NULL, L"�����_�[�^�[�Q�b�g�������s", L"�G���[���b�Z�[�W", MB_OK);
		}
		result = CreateDescriptorHeap(device);
		if (result == RESULT::FAILED) {
			MessageBox(NULL, L"�f�B�X�N���v�^�q�[�v�������s", L"�G���[���b�Z�[�W", MB_OK);
		}
		result = CreateBuffer(device);
		if (result == RESULT::FAILED) {
			MessageBox(NULL, L"�o�b�t�@�[�������s", L"�G���[���b�Z�[�W", MB_OK);
		}

		_descriptorHeap->RegistConstantBuffer(device, *_biasBuffer.get(), BIAS_INDEX);

		SIZE windowSize = AppWindow::GetWindowSize();

		_viewport = CD3DX12_VIEWPORT(
			0.f, 0.f, static_cast<float>(windowSize.cx), static_cast<float>(windowSize.cy));
		_scissorRect = CD3DX12_RECT(0, 0, windowSize.cx, windowSize.cy);

		return result;
	}
	void ShadowingPass::Render()
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
	void ShadowingPass::SetWorldPosTexture(DX12API::Texture& texture)
	{
		ShaderResourceViewDesc desc(texture, false);
		_descriptorHeap->RegistShaderResource(Dx12GraphicsEngine::Instance().Device(), texture, desc, WORLDPOS_INDEX);
	}
	void ShadowingPass::SetShadowMap(DX12API::Texture& texture)
	{
		ShaderResourceViewDesc desc(texture, false);
		_descriptorHeap->RegistShaderResource(Dx12GraphicsEngine::Instance().Device(), texture, desc, SHADOWMAP_INDEX);
	}
	void ShadowingPass::SetLightViewProjBuffer(DX12API::ConstantBuffer& buffer)
	{
		_descriptorHeap->RegistConstantBuffer(Dx12GraphicsEngine::Instance().Device(), buffer, 0);
	}
	void ShadowingPass::UpdateBias(float bias)
	{
		_biasBuffer->UpdateData(&bias);
	}
	const SIZE_T ShadowingPass::GetShadowMapHandlePtr()
	{
		auto gpuHandle = _descriptorHeap->GetGPUDescriptorHandleForHeapStart();
		gpuHandle.ptr += _descriptorHeap->GetHandleIncrimentSize();

		return _descriptorHeap->GetGPUHandle().ptr;
		//return _descriptorHeap->GetGPUDescriptorHandleForHeapStartSRV().ptr;
		//return _descriptorHeap->GetSRVHandle(SHADOWMAP_INDEX).ptr;
	}
	DX12API::Texture& ShadowingPass::GetShadowFactorTexture()
	{
		return _renderTarget->GetRenderTargetTexture();
	}
}
