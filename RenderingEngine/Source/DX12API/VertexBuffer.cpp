#include "VertexBuffer.h"

using namespace NamelessEngine::Utility;

namespace NamelessEngine::DX12API
{
	HRESULT VertexBuffer::CreateVertexBufferAndView(ID3D12Device& device, const UINT& sizeInBytes, const UINT& stribeInBytes)
	{
		// バッファー生成
		CD3DX12_HEAP_PROPERTIES heapProp(D3D12_HEAP_TYPE_UPLOAD);
		CD3DX12_RESOURCE_DESC resDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeInBytes);

		HRESULT result = device.CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(_vertexBuffer.ReleaseAndGetAddressOf()));

		if (FAILED(result)) { return result; }

		// ビュー生成
		_vertexBufferView.BufferLocation = _vertexBuffer->GetGPUVirtualAddress();	// バッファのGPU側の仮想アドレス
		_vertexBufferView.SizeInBytes = sizeInBytes;	                                // 頂点の全サイズ
		_vertexBufferView.StrideInBytes = stribeInBytes;	                    // 1頂点当たりのサイズ

		return result;
	}

	HRESULT VertexBuffer::MapVertexBuffer(void* vertexData, const UINT& sizeInBytes)
	{
		BYTE* mappedData;
		HRESULT result = _vertexBuffer->Map(0, nullptr, (void**)&mappedData);
		if (FAILED(result)) { return result; }

		std::memcpy((void*)mappedData, vertexData, static_cast<size_t>(sizeInBytes));

		_vertexBuffer->Unmap(0, nullptr);

		return result;
	}

	MYRESULT VertexBuffer::Create(
		ID3D12Device& device, void* vertexData, const UINT& sizeInBytes, const UINT& stribeInBytes)
	{
		// 頂点数記録
		_vertexNum = sizeInBytes / stribeInBytes;

		// バッファーとビュー生成
		if (FAILED(CreateVertexBufferAndView(device, sizeInBytes, stribeInBytes))) { return MYRESULT::FAILED; }
		// マップ処理
		if (FAILED(MapVertexBuffer(vertexData, sizeInBytes))) { return MYRESULT::FAILED; }

		return MYRESULT::SUCCESS;
	}
}
