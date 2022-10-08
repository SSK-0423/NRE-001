#include "VertexBuffer.h"

using namespace NamelessEngine::Utility;

namespace NamelessEngine::DX12API
{
	HRESULT VertexBuffer::CreateVertexBufferAndView(ID3D12Device& device, const UINT& sizeInBytes, const UINT& stribeInBytes)
	{
		// �o�b�t�@�[����
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

		// �r���[����
		_vertexBufferView.BufferLocation = _vertexBuffer->GetGPUVirtualAddress();	// �o�b�t�@��GPU���̉��z�A�h���X
		_vertexBufferView.SizeInBytes = sizeInBytes;	                                // ���_�̑S�T�C�Y
		_vertexBufferView.StrideInBytes = stribeInBytes;	                    // 1���_������̃T�C�Y

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
		// ���_���L�^
		_vertexNum = sizeInBytes / stribeInBytes;

		// �o�b�t�@�[�ƃr���[����
		if (FAILED(CreateVertexBufferAndView(device, sizeInBytes, stribeInBytes))) { return MYRESULT::FAILED; }
		// �}�b�v����
		if (FAILED(MapVertexBuffer(vertexData, sizeInBytes))) { return MYRESULT::FAILED; }

		return MYRESULT::SUCCESS;
	}
}
