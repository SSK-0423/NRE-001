#include "LightingPass.h"
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

constexpr UINT CUBETEX_INDEX = 4;

namespace NamelessEngine::Graphics
{
	LightingPass::LightingPass()
		: _rootSignature(new RootSignature()), _pipelineState(new GraphicsPipelineState()),
		_descriptorHeap(new DescriptorHeapCBV_SRV_UAV()), _paramBuffer(new ConstantBuffer())
	{
	}

	LightingPass::~LightingPass()
	{
	}
	Utility::RESULT LightingPass::CreateRootSignature(ID3D12Device& device)
	{
		RootSignatureData rootSigData;
		rootSigData._descRangeData.cbvDescriptorNum = 1;
		// �J���[�A�@���A�L���[�u�}�b�vUV�A���^���b�N�E���t�l�X�A�L���[�u�e�N�X�`��
		rootSigData._descRangeData.srvDescriptorNum = 5;

		Utility::RESULT result = _rootSignature->Create(device, rootSigData);
		if (result == Utility::RESULT::FAILED) { return result; }
		return Utility::RESULT::SUCCESS;
	}
	Utility::RESULT LightingPass::CreateGraphicsPipelineState(ID3D12Device& device)
	{
		// ���[�g�V�O�l�`���ƃV�F�[�_�[�Z�b�g
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineState = {};
		pipelineState.pRootSignature = &_rootSignature->GetRootSignature();
		pipelineState.VS = CD3DX12_SHADER_BYTECODE(&ShaderLibrary::Instance().GetShader("PBRVS")->GetShader());
		pipelineState.PS = CD3DX12_SHADER_BYTECODE(&ShaderLibrary::Instance().GetShader("PBRPS")->GetShader());

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
		pipelineState.DepthStencilState.DepthEnable = true;
		pipelineState.DepthStencilState.StencilEnable = false;
		pipelineState.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		pipelineState.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		pipelineState.DSVFormat = DXGI_FORMAT_D32_FLOAT;

		// �����_�[�^�[�Q�b�g�̐ݒ�
		pipelineState.NumRenderTargets = 1;
		pipelineState.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

		// �A���`�G�C���A�V���O�̂��߂̃T���v�����ݒ�
		pipelineState.SampleDesc.Count = 1;			// �T���v�����O��1�s�N�Z���ɂ�1
		pipelineState.SampleDesc.Quality = 0;		// �N�I���e�B�͍Œ�

		return _pipelineState->Create(device, pipelineState);
	}
	Utility::RESULT LightingPass::CreateRenderTarget(ID3D12Device& device)
	{
		return Utility::RESULT::SUCCESS;
	}
	Utility::RESULT LightingPass::CreateParamBuffer(ID3D12Device& device)
	{
		return _paramBuffer->Create(device, &_paramData, sizeof(ParameterCBuff));
	}
	Utility::RESULT LightingPass::CreateDescriptorHeap(ID3D12Device& device)
	{
		return _descriptorHeap->Create(device);
	}

	Utility::RESULT LightingPass::Init()
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
		result = CreateParamBuffer(device);
		if (result == RESULT::FAILED) {
			MessageBox(NULL, L"�p�����[�^�[�p�R���X�^���g�o�b�t�@�[�������s", L"�G���[���b�Z�[�W", MB_OK);
		}

		SIZE windowSize = AppWindow::GetWindowSize();

		_viewport = CD3DX12_VIEWPORT(
			0.f, 0.f, static_cast<float>(windowSize.cx), static_cast<float>(windowSize.cy));
		_scissorRect = CD3DX12_RECT(0, 0, windowSize.cx, windowSize.cy);

		return result;
	}

	void LightingPass::Render()
	{
		RenderingContext& renderContext = Dx12GraphicsEngine::Instance().GetRenderingContext();
		renderContext.SetGraphicsRootSignature(*_rootSignature);
		renderContext.SetPipelineState(*_pipelineState);

		Dx12GraphicsEngine::Instance().SetFrameRenderTarget(_viewport, _scissorRect);
		{
			renderContext.SetDescriptorHeap(*_descriptorHeap);
			renderContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			renderContext.DrawInstanced(4, 1, 0, 0);
		}
	}
	void LightingPass::SetGBuffer(GBUFFER_TYPE type, DX12API::Texture& texture)
	{
		ShaderResourceViewDesc desc(texture);
		// 0: �J���[, 1: �@��, 2: �L���[�u�}�b�vUV, 3: ���^���b�N�E���t�l�X
		_descriptorHeap->RegistShaderResource(
			Dx12GraphicsEngine::Instance().Device(), texture, desc, static_cast<int>(type));
	}
	void LightingPass::SetCubeTexture(DX12API::Texture& texture)
	{
		ShaderResourceViewDesc desc(texture, true);
		_descriptorHeap->RegistShaderResource(
			Dx12GraphicsEngine::Instance().Device(), texture, desc, CUBETEX_INDEX);
	}
}
