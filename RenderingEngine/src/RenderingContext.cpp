#include "d3dx12.h"
#include "RenderingContext.h"
#include "DescriptorHeapRTV.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "GraphicsPipelineState.h"
#include "RootSignature.h"

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

void RenderingContext::TransitionResourceState(
	ID3D12Resource& resource, D3D12_RESOURCE_STATES presentState, D3D12_RESOURCE_STATES afterState)
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

void RenderingContext::SetViewport(const D3D12_VIEWPORT& viewport)
{
	_cmdList->RSSetViewports(1, &viewport);
}

void RenderingContext::SetViewports(UINT viewportNum, const D3D12_VIEWPORT& viewports)
{
	_cmdList->RSSetViewports(viewportNum, &viewports);
}

void RenderingContext::SetScissorRect(const D3D12_RECT& scissorRect)
{
	_cmdList->RSSetScissorRects(1, &scissorRect);
}

void RenderingContext::SetScissorRects(UINT scissorRectNum, const D3D12_RECT& scissorRect)
{
	_cmdList->RSSetScissorRects(scissorRectNum, &scissorRect);
}

void RenderingContext::SetVertexBuffer(UINT startSlot, const VertexBuffer& vertexBuffer)
{
	_cmdList->IASetVertexBuffers(startSlot, 1, &vertexBuffer.GetView());
}

void RenderingContext::SetIndexBuffer(const IndexBuffer& indexBuffer)
{
	_cmdList->IASetIndexBuffer(&indexBuffer.GetView());
}

void RenderingContext::SetPipelineState(GraphicsPipelineState& pipelineState)
{
	_cmdList->SetPipelineState(&pipelineState.GetPipelineState());
}

void RenderingContext::SetGraphicsRootSignature(RootSignature& rootSignature)
{
	_cmdList->SetGraphicsRootSignature(&rootSignature.GetRootSignature());
}

void RenderingContext::SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY primitiveTopology)
{
	_cmdList->IASetPrimitiveTopology(primitiveTopology);
}

void RenderingContext::DrawInstanced(
	UINT vertexNumPerInstance, UINT instanceNum,
	UINT startVertexLocation, UINT startInstanceLocation)
{
	_cmdList->DrawInstanced(vertexNumPerInstance, instanceNum, startVertexLocation, startInstanceLocation);
}

void RenderingContext::DrawIndexedInstanced(UINT indexNumPerInstance, UINT instanceNum, UINT startIndexLocation, INT baseVertexLocation, UINT startInstanceLocation)
{
	_cmdList->DrawIndexedInstanced(
		indexNumPerInstance, instanceNum, startIndexLocation, baseVertexLocation, startInstanceLocation);
}

void RenderingContext::CopyTextureRegion(
	const D3D12_TEXTURE_COPY_LOCATION& src, const D3D12_TEXTURE_COPY_LOCATION& dst, 
	UINT dstX, UINT dstY, UINT dstZ, D3D12_BOX* srcBox)
{
	_cmdList->CopyTextureRegion(&dst, dstX, dstY, dstZ, &src, srcBox);
}