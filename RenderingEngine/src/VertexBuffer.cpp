#include "VertexBuffer.h"

HRESULT VertexBuffer::CreateVertexBufferAndView(
	ID3D12Device& device, const std::vector<DirectX::XMFLOAT3>& vertex)
{
	// バッファー生成
	CD3DX12_HEAP_PROPERTIES heapProp(D3D12_HEAP_TYPE_UPLOAD);
	UINT bufferSize = sizeof(vertex[0]) * vertex.size();
	CD3DX12_RESOURCE_DESC resDesc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

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
	_vertexBufferView.SizeInBytes = bufferSize;	                                // 頂点の全サイズ
	_vertexBufferView.StrideInBytes = sizeof(vertex[0]);	                    // 1頂点当たりのサイズ

	return result;
}

HRESULT VertexBuffer::MapVertexBuffer(const std::vector<DirectX::XMFLOAT3>& vertex)
{
	HRESULT result = _vertexBuffer->Map(0, nullptr, (void**)&_vertMap);
	if (FAILED(result)) { return result; }

	std::copy(std::begin(vertex), std::end(vertex), _vertMap);
	//std::memcpy((void*)_vertMap, (void*)&vertex[0],_vertexBufferView.SizeInBytes);
	
	_vertexBuffer->Unmap(0, nullptr);

	return result;
}

MYRESULT VertexBuffer::Create(ID3D12Device& device, const std::vector<DirectX::XMFLOAT3>& vertex)
{
	// 頂点数記録
	_vertexNum = vertex.size();

	// バッファーとビュー生成
	if (FAILED(CreateVertexBufferAndView(device, vertex))) { return MYRESULT::FAILED; }
	// マップ処理
	if (FAILED(MapVertexBuffer(vertex))) { return MYRESULT::FAILED; }

	return MYRESULT::SUCCESS;
}