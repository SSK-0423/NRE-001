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

		// �[�x�̓J���[�̃����_�[�^�[�Q�b�g�Ɋ܂܂�Ă���̂Ń����_�[�^�[�Q�b�g����-1����
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
		data.useDepth = true;	// �[�x�͈�ł悢�̂ŃJ���[�o�͂̃����_�[�^�[�Q�b�g�݂̂Ŏg�p����

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

		SIZE windowSize = AppWindow::GetWindowSize();

		_viewport = CD3DX12_VIEWPORT(
			0.f, 0.f, static_cast<float>(windowSize.cx), static_cast<float>(windowSize.cy));
		_scissorRect = CD3DX12_RECT(0, 0, windowSize.cx, windowSize.cy);

		return result;
	}
	Utility::RESULT GBufferPass::CreateGraphicsPipelineState(ID3D12Device& device)
	{
		// ���[�g�V�O�l�`���ƃV�F�[�_�[�Z�b�g
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineState = {};
		pipelineState.pRootSignature = &_rootSignature->GetRootSignature();
		pipelineState.VS = CD3DX12_SHADER_BYTECODE(&ShaderLibrary::Instance().GetShader("GBufferVS")->GetShader());
		pipelineState.PS = CD3DX12_SHADER_BYTECODE(&ShaderLibrary::Instance().GetShader("GBufferPS")->GetShader());

		// �T���v���}�X�N�ݒ�
		pipelineState.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

		// �u�����h
		pipelineState.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

		// ���X�^���C�Y�ݒ�
		pipelineState.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		pipelineState.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;

		// �C���v�b�g���C�A�E�g�̐ݒ�
		InputLayout layout = InputLayout::DefaultLayout();
		pipelineState.InputLayout.pInputElementDescs = layout.GetData();
		pipelineState.InputLayout.NumElements = layout.GetSize();
		pipelineState.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
		pipelineState.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		// �f�v�X�X�e���V���ݒ�
		pipelineState.DepthStencilState.DepthEnable = true;
		pipelineState.DepthStencilState.StencilEnable = false;
		pipelineState.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		pipelineState.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		pipelineState.DSVFormat = DXGI_FORMAT_D32_FLOAT;

		// �����_�[�^�[�Q�b�g�̐ݒ�
		pipelineState.NumRenderTargets = 4;
		pipelineState.RTVFormats[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;	// �J���[
		pipelineState.RTVFormats[1] = DXGI_FORMAT_R32G32B32A32_FLOAT;	// �@��
		pipelineState.RTVFormats[2] = DXGI_FORMAT_R32G32B32A32_FLOAT;	// ���[���h���W
		pipelineState.RTVFormats[3] = DXGI_FORMAT_R32G32B32A32_FLOAT;	// ���^���b�N/���t�l�X/���t���N�V����

		// �A���`�G�C���A�V���O�̂��߂̃T���v�����ݒ�
		pipelineState.SampleDesc.Count = 1;			// �T���v�����O��1�s�N�Z���ɂ�1
		pipelineState.SampleDesc.Quality = 0;		// �N�I���e�B�͍Œ�

		return _pipelineState->Create(device, pipelineState);
	}
}
