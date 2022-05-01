#include "GraphicsPipelineState.h"


MYRESULT GraphicsPipelineState::Create(
	ID3D12Device& device, const D3D12_GRAPHICS_PIPELINE_STATE_DESC& piplineStateDesc)
{
	// グラフィックスパイプラインステート生成
	if (FAILED(CreateGraphicsPipelineState(device, piplineStateDesc))) { return MYRESULT::FAILED; }

	return MYRESULT::SUCCESS;
}

HRESULT GraphicsPipelineState::CreateGraphicsPipelineState(
	ID3D12Device& device, const D3D12_GRAPHICS_PIPELINE_STATE_DESC& piplineStateDesc)
{
	HRESULT result = device.CreateGraphicsPipelineState(
		&piplineStateDesc, IID_PPV_ARGS(_pipelineState.ReleaseAndGetAddressOf()));

	if (FAILED(result)) { return result; }
	
	return result;
}