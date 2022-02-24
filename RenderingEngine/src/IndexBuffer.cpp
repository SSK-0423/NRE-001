#include "IndexBuffer.h"

HRESULT IndexBuffer::CreateIndexBufferAndView(ID3D12Device& device, const std::vector<UINT>& index)
{
	CD3DX12_HEAP_PROPERTIES heapProp(D3D12_HEAP_TYPE_UPLOAD);
	UINT bufferSize = static_cast<UINT>(sizeof(index[0]) * index.size());
	CD3DX12_RESOURCE_DESC resDesc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	HRESULT result = device.CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(_indexBuffer.ReleaseAndGetAddressOf()));

	if (FAILED(result)) { return result; }

	// ビュー生成
	_indexBufferView.BufferLocation = _indexBuffer->GetGPUVirtualAddress();
	_indexBufferView.SizeInBytes = bufferSize;
	_indexBufferView.Format = DXGI_FORMAT_R16_UINT;

	return result;
}

HRESULT IndexBuffer::MapIndexBuffer(const std::vector<UINT>& index)
{
	HRESULT result = _indexBuffer->Map(0, nullptr, (void**)&_indexMap);
	if (FAILED(result)) { return result; }

	std::copy(std::begin(index), std::end(index), _indexMap);

	_indexBuffer->Unmap(0, nullptr);

	return result;
}

MYRESULT IndexBuffer::Create(ID3D12Device& device, const std::vector<UINT>& index)
{
	// インデックス数記録
	_indexNum = static_cast<UINT>(index.size());

	// インデックスバッファーとビュー生成
	if (FAILED(CreateIndexBufferAndView(device, index))) { return MYRESULT::FAILED; }
	// マップ処理
	if (FAILED(MapIndexBuffer(index))) { return MYRESULT::FAILED; }

	return MYRESULT::SUCCESS;
}
