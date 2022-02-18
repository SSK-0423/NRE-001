#include "RenderTarget.h"
#include "d3dx12.h"

RenderTargetInitData::RenderTargetInitData()
	:_colorFormat(DXGI_FORMAT_R8G8B8A8_UNORM), _depthFormat(DXGI_FORMAT_D32_FLOAT),
	_width(1024), _height(768), _clearDepth(1.f)
{
	_clearColor[0] = 1.f;
	_clearColor[1] = 1.f;
	_clearColor[2] = 1.f;
	_clearColor[3] = 1.f;
}

RenderTargetInitData::RenderTargetInitData(
	const DXGI_FORMAT& colorFormat, const DXGI_FORMAT& depthFormat, const UINT& width, const UINT& height,
	const float(&clearColor)[4], const float& clearDepth)
	: _colorFormat(colorFormat), _depthFormat(depthFormat), _width(width), _height(height), _clearDepth(clearDepth)
{
	_clearColor[0] = clearColor[0];
	_clearColor[1] = clearColor[1];
	_clearColor[2] = clearColor[2];
	_clearColor[3] = clearColor[3];
}

MYRESULT RenderTarget::Create(ID3D12Device& device, const RenderTargetInitData& initData, bool createDepth)
{
	// レンダーターゲット生成
	if (FAILED(CreateRenderTargetBuffer(
		device, initData._colorFormat, initData._width, initData._height, initData._clearColor)))
	{
		return MYRESULT::FAILED;
	}

	if (!createDepth) {
		return MYRESULT::SUCCESS;
	}

	// デプスステンシル生成
	if (FAILED(CreateDepthStencilBuffer(
		device, initData._depthFormat, initData._width, initData._height, initData._clearDepth)))
	{
		return MYRESULT::FAILED;
	}

	return MYRESULT::SUCCESS;
}

MYRESULT RenderTarget::Create(
	ID3D12Device& device, IDXGISwapChain4& swapchain, const RenderTargetInitData& initData, 
	size_t bufferIndex, bool createDepth)
{
	// レンダーターゲット生成
	if (FAILED(CreateRenderTargetBuffer(device, swapchain, bufferIndex)))
	{
		return MYRESULT::FAILED;
	}

	if (!createDepth) {
		return MYRESULT::SUCCESS;
	}

	// デプスステンシル生成
	if (FAILED(CreateDepthStencilBuffer(
		device, initData._depthFormat, initData._width, initData._height, initData._clearDepth)))
	{
		return MYRESULT::FAILED;
	}
	return MYRESULT::SUCCESS;
}

HRESULT RenderTarget::CreateRenderTargetBuffer(
	ID3D12Device& device, const DXGI_FORMAT& format, const UINT& width, const UINT& height,
	const float(&clearColor)[4])
{
	// バッファー生成
	CD3DX12_HEAP_PROPERTIES heapProp(D3D12_HEAP_TYPE_DEFAULT);
	CD3DX12_RESOURCE_DESC resDesc = CD3DX12_RESOURCE_DESC::Tex2D(format, width, height);
	CD3DX12_CLEAR_VALUE clearValue(format, clearColor);

	// 生成
	HRESULT result = device.CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_PRESENT,
		&clearValue,
		IID_PPV_ARGS(_renderTargetBuffer.ReleaseAndGetAddressOf()));

	if (FAILED(result)) { return result; }

	return result;
}

HRESULT RenderTarget::CreateRenderTargetBuffer(ID3D12Device& device, IDXGISwapChain4& swapchain, size_t bufferIndex)
{
	HRESULT result = swapchain.GetBuffer(bufferIndex, IID_PPV_ARGS(_renderTargetBuffer.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { return result; }

	return result;
}

HRESULT RenderTarget::CreateDepthStencilBuffer(
	ID3D12Device& device, const DXGI_FORMAT& format, const UINT& width, const UINT& height,
	const float& clearDepth)
{
	// バッファー生成
	CD3DX12_HEAP_PROPERTIES heapProp(D3D12_HEAP_TYPE_DEFAULT);
	CD3DX12_RESOURCE_DESC resDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, width, height);
	CD3DX12_CLEAR_VALUE clearValue;
	clearValue.Format = DXGI_FORMAT_D32_FLOAT;
	clearValue.DepthStencil.Depth = clearDepth;

	HRESULT result = device.CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_PRESENT,
		&clearValue,
		IID_PPV_ARGS(_depthStencilBuffer.ReleaseAndGetAddressOf()));

	if (FAILED(result)) { return result; }

	return result;
}

ID3D12Resource& RenderTarget::GetRenderTargetBuffer()
{
	return *_renderTargetBuffer.Get();
}

ID3D12Resource& RenderTarget::GetDepthBuffer()
{
	return *_depthStencilBuffer.Get();
}
