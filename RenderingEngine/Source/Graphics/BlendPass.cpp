#include "BlendPass.h"

#include "ShaderResourceViewDesc.h"
#include "DescriptorHeapCBV_SRV_UAV.h"
#include "GraphicsPipelineState.h"
#include "RootSignature.h"
#include "ShaderLibrary.h"
#include "RenderTarget.h"
#include "Texture.h"

#include "Dx12GraphicsEngine.h"

constexpr UINT SKYBOXTEX_INDEX = 0;
constexpr UINT LIGHTEDTEX_INDEX = 1;
constexpr UINT DEPTHTEX_INDEX = 2;

using namespace NamelessEngine::Core;
using namespace NamelessEngine::DX12API;
using namespace NamelessEngine::Graphics;
using namespace NamelessEngine::Utility;

namespace NamelessEngine::Graphics
{
	BlendPass::BlendPass()
		: _rootSignature(new RootSignature()), _pipelineState(new GraphicsPipelineState()),
		_renderTarget(new RenderTarget()), _descriptorHeap(new DescriptorHeapCBV_SRV_UAV())
	{
	}
	BlendPass::~BlendPass()
	{
	}

	Utility::RESULT BlendPass::Init()
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

		SIZE windowSize = AppWindow::GetWindowSize();
		_viewport = CD3DX12_VIEWPORT(0.f, 0.f, static_cast<float>(windowSize.cx), static_cast<float>(windowSize.cy));
		_scissorRect = CD3DX12_RECT(0, 0, windowSize.cx, windowSize.cy);

		return result;
	}
	Utility::RESULT BlendPass::CreateRootSignature(ID3D12Device& device)
	{
		RootSignatureData rootSigData;
		// �L���[�u�e�N�X�`���A���C�e�B���O�ς݃e�N�X�`��
		rootSigData._descRangeData.srvDescriptorNum = 3;

		Utility::RESULT result = _rootSignature->Create(device, rootSigData);
		if (result == Utility::RESULT::FAILED) { return result; }
		return Utility::RESULT::SUCCESS;
	}
	Utility::RESULT BlendPass::CreateGraphicsPipelineState(ID3D12Device& device)
	{
		// ���[�g�V�O�l�`���ƃV�F�[�_�[�Z�b�g
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineState = {};
		pipelineState.pRootSignature = &_rootSignature->GetRootSignature();
		pipelineState.VS = CD3DX12_SHADER_BYTECODE(&ShaderLibrary::Instance().GetShader("BlendVS")->GetShader());
		pipelineState.PS = CD3DX12_SHADER_BYTECODE(&ShaderLibrary::Instance().GetShader("BlendPS")->GetShader());

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
		pipelineState.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

		// �A���`�G�C���A�V���O�̂��߂̃T���v�����ݒ�
		pipelineState.SampleDesc.Count = 1;			// �T���v�����O��1�s�N�Z���ɂ�1
		pipelineState.SampleDesc.Quality = 0;		// �N�I���e�B�͍Œ�

		return _pipelineState->Create(device, pipelineState);
	}
	Utility::RESULT BlendPass::CreateRenderTarget(ID3D12Device& device)
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
	Utility::RESULT BlendPass::CreateDescriptorHeap(ID3D12Device& device)
	{
		return _descriptorHeap->Create(device);
	}
	void BlendPass::Render()
	{
		RenderingContext& renderContext = Dx12GraphicsEngine::Instance().GetRenderingContext();

		renderContext.SetPipelineState(*_pipelineState);
		renderContext.SetGraphicsRootSignature(*_rootSignature);

		Dx12GraphicsEngine::Instance().SetFrameRenderTarget(_viewport, _scissorRect);
		{
			renderContext.SetDescriptorHeap(*_descriptorHeap);
			renderContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			renderContext.DrawInstanced(4, 1, 0, 0);
		}
	}
	void BlendPass::SetRenderedSkyBoxTexture(DX12API::Texture& texture)
	{
		ShaderResourceViewDesc desc(texture, false);
		_descriptorHeap->RegistShaderResource(
			Dx12GraphicsEngine::Instance().Device(), texture, desc, SKYBOXTEX_INDEX);
	}
	void BlendPass::SetLightedTexture(DX12API::Texture& texture)
	{
		ShaderResourceViewDesc desc(texture, false);
		_descriptorHeap->RegistShaderResource(
			Dx12GraphicsEngine::Instance().Device(), texture, desc, LIGHTEDTEX_INDEX);
	}
	void BlendPass::SetDepthTexture(DX12API::Texture& texture)
	{
		ShaderResourceViewDesc desc(texture, false);
		_descriptorHeap->RegistShaderResource(
			Dx12GraphicsEngine::Instance().Device(), texture, desc, DEPTHTEX_INDEX);
	}
	DX12API::Texture& BlendPass::GetOffscreenTexture()
	{
		return _renderTarget->GetRenderTargetTexture();
	}
}
