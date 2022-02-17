#include "RenderingContext.h"
#include "d3dx12.h"

RenderingContext::RenderingContext()
{
}

RenderingContext::~RenderingContext()
{

}

void RenderingContext::Init(ID3D12GraphicsCommandList& cmdList)
{
	_cmdList = &cmdList;
}

void RenderingContext::Close()
{
	_cmdList->Close();
}

void RenderingContext::Reset(ID3D12CommandAllocator& cmdAlloc)
{
	_cmdList->Reset(&cmdAlloc, nullptr);
}

void RenderingContext::TransitionResourceState(ID3D12Resource& resource, D3D12_RESOURCE_STATES presentState, D3D12_RESOURCE_STATES afterState)
{
	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		&resource, presentState, afterState);
	_cmdList->ResourceBarrier(1, &barrier);
}

void RenderingContext::SetRenderTargets(RenderTarget& renderTarget, BOOL descriptorContinuous)
{
}
