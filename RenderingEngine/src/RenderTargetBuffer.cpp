#include "RenderTargetBuffer.h"
#include "d3dx12.h"

RenderTargetBufferData::RenderTargetBufferData(
	const DXGI_FORMAT& colorFormat, const UINT& width, const UINT& height, const float(&clearColor)[4])
	: _colorFormat(colorFormat), _width(width), _height(height)
{
	_clearColor[0] = clearColor[0];
	_clearColor[1] = clearColor[1];
	_clearColor[2] = clearColor[2];
	_clearColor[3] = clearColor[3];
}

MYRESULT RenderTargetBuffer::Create(ID3D12Device& device, const RenderTargetBufferData& data)
{
	CD3DX12_HEAP_PROPERTIES heapProp(D3D12_HEAP_TYPE_DEFAULT);
	CD3DX12_RESOURCE_DESC resDesc = CD3DX12_RESOURCE_DESC::Tex2D(data._colorFormat, data._width, data._height);
	resDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
	CD3DX12_CLEAR_VALUE clearValue(data._colorFormat, data._clearColor);

	HRESULT result = device.CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_PRESENT,
		&clearValue,
		IID_PPV_ARGS(_rtvBuffer.ReleaseAndGetAddressOf()));

	if (FAILED(result)) { return MYRESULT::FAILED; }

	return MYRESULT::SUCCESS;
}

MYRESULT RenderTargetBuffer::Create(ID3D12Device& device, IDXGISwapChain4& swapchain, const size_t& index)
{
	HRESULT result = swapchain.GetBuffer(index, IID_PPV_ARGS(_rtvBuffer.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { return MYRESULT::FAILED; }

	return MYRESULT::SUCCESS;
}
