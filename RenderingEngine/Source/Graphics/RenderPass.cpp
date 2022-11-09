//#include "RenderPass.h"
//#include "Dx12GraphicsEngine.h"
//#include "RootSignature.h"
//#include "GraphicsPipelineState.h"
//
//using namespace NamelessEngine::Core;
//using namespace NamelessEngine::Utility;
//
//namespace NamelessEngine::Graphics
//{
//	RenderPass::RenderPass()
//		: _rootSignature(new DX12API::RootSignature()), _pipelineState(new DX12API::GraphicsPipelineState())
//	{
//	}
//	Utility::RESULT RenderPass::Init()
//	{
//		ID3D12Device& device = Dx12GraphicsEngine::Instance().Device();
//
//		RESULT result = CreateRootSignature(device);
//		if (result == RESULT::FAILED) {
//			MessageBox(NULL, L"���[�g�V�O�l�`���������s", L"�G���[���b�Z�[�W", MB_OK);
//		}
//		result = CreateGraphicsPipelineState(device);
//		if (result == RESULT::FAILED) {
//			MessageBox(NULL, L"�O���t�B�N�X�p�C�v���C���������s", L"�G���[���b�Z�[�W", MB_OK);
//		}
//		result = CreateRenderTarget(device);
//		if (result == RESULT::FAILED) {
//			MessageBox(NULL, L"�����_�[�^�[�Q�b�g�������s", L"�G���[���b�Z�[�W", MB_OK);
//		}
//
//		return result;
//	}
//}
