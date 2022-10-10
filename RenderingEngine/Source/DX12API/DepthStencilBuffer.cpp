#include "DepthStencilBuffer.h"
#include "d3dx12.h"
#include <stdio.h>

using namespace NamelessEngine::Utility;

namespace NamelessEngine::DX12API
{
	DepthStencilBufferData::DepthStencilBufferData(
		const DXGI_FORMAT& depthFormat, const UINT& width, const UINT& height,
		const float& clearDepth, const UINT8& clearStencil)
		: depthFormat(depthFormat), width(width), height(height), clearDepth(clearDepth),
		clearStencil(clearStencil)
	{
	}

	RESULT DepthStencilBuffer::Create(ID3D12Device& device, const DepthStencilBufferData& data)
	{
		_depthStencilBufferData = data;

		CD3DX12_HEAP_PROPERTIES heapProp(D3D12_HEAP_TYPE_DEFAULT);
		CD3DX12_RESOURCE_DESC resDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R32_TYPELESS, data.width, data.height);
		resDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
		CD3DX12_CLEAR_VALUE clearValue(data.depthFormat, data.clearDepth, data.clearStencil);

		HRESULT result = device.CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&clearValue,
			IID_PPV_ARGS(_dsvBuffer.ReleaseAndGetAddressOf()));

		if (FAILED(result)) { return RESULT::FAILED; }

		return RESULT::SUCCESS;
	}
}