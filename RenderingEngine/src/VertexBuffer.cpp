#include "VertexBuffer.h"

HRESULT VertexBuffer::CreateVertexBufferAndView(
	ID3D12Device& device, const std::vector<DirectX::XMFLOAT3>& vertex)
{
	// �o�b�t�@�[����
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

	// �r���[����
	_vertexBufferView.BufferLocation = _vertexBuffer->GetGPUVirtualAddress();	// �o�b�t�@��GPU���̉��z�A�h���X
	_vertexBufferView.SizeInBytes = bufferSize;	                                // ���_�̑S�T�C�Y
	_vertexBufferView.StrideInBytes = sizeof(vertex[0]);	                    // 1���_������̃T�C�Y

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
	// ���_���L�^
	_vertexNum = vertex.size();

	// �o�b�t�@�[�ƃr���[����
	if (FAILED(CreateVertexBufferAndView(device, vertex))) { return MYRESULT::FAILED; }
	// �}�b�v����
	if (FAILED(MapVertexBuffer(vertex))) { return MYRESULT::FAILED; }

	return MYRESULT::SUCCESS;
}