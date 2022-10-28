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
//			MessageBox(NULL, L"ルートシグネチャ生成失敗", L"エラーメッセージ", MB_OK);
//		}
//		result = CreateGraphicsPipelineState(device);
//		if (result == RESULT::FAILED) {
//			MessageBox(NULL, L"グラフィクスパイプライン生成失敗", L"エラーメッセージ", MB_OK);
//		}
//		result = CreateRenderTarget(device);
//		if (result == RESULT::FAILED) {
//			MessageBox(NULL, L"レンダーターゲット生成失敗", L"エラーメッセージ", MB_OK);
//		}
//
//		return result;
//	}
//}
