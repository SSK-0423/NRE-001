#include "d3dx12.h"
#include "RenderingContext.h"
#include "DescriptorHeapRTV.h"
#include "VertexBuffer.h"

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

void RenderingContext::SetRenderTarget(
	const D3D12_CPU_DESCRIPTOR_HANDLE* rtvHandle, const D3D12_CPU_DESCRIPTOR_HANDLE* dsvHandle)
{
	_cmdList->OMSetRenderTargets(1, rtvHandle, false, dsvHandle);
}

void RenderingContext::SetRenderTargets(
	UINT rtvNum, const D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[], const D3D12_CPU_DESCRIPTOR_HANDLE* dsvHandle)
{
	_cmdList->OMSetRenderTargets(rtvNum, rtvHandles, false, dsvHandle);
}

void RenderingContext::ClearRenderTarget(
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, const ColorRGBA& color, UINT numRects, const D3D12_RECT* rects)
{
	float clearColor[4] = { color._color[0],color._color[1], color._color[2], color._color[3] };
	_cmdList->ClearRenderTargetView(rtvHandle, clearColor, numRects, rects);
}

void RenderingContext::ClearRenderTarget(D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, const float* color, UINT numRects, const D3D12_RECT* rects)
{
	_cmdList->ClearRenderTargetView(rtvHandle, color, numRects, rects);
}

void RenderingContext::SetVertexBuffer(UINT startSlot, const VertexBuffer& vertexBuffer)
{
	_cmdList->IASetVertexBuffers(startSlot, 1, &vertexBuffer.GetView());
}