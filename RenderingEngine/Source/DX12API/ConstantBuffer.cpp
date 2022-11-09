#include <d3dx12.h>

#include "ConstantBuffer.h"

using namespace Microsoft::WRL;
using namespace NamelessEngine::Utility;

namespace NamelessEngine::DX12API
{
	HRESULT ConstantBuffer::CreateConstantBuffer(ID3D12Device& device, const UINT& bufferSize)
	{
		CD3DX12_HEAP_PROPERTIES heapProp(D3D12_HEAP_TYPE_UPLOAD);
		CD3DX12_RESOURCE_DESC resDesc = CD3DX12_RESOURCE_DESC::Buffer(
			AlignmentedSize(bufferSize, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT));

		HRESULT result = device.CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(_constantBuffer.ReleaseAndGetAddressOf()));
		if (FAILED(result)) { return result; }

		return result;
	}

	HRESULT ConstantBuffer::MapConstantBuffer(void* data, const UINT& bufferSize)
	{
		HRESULT result = _constantBuffer->Map(0, nullptr, (void**)&_mappedData);
		if (FAILED(result)) { return result; }

		std::memcpy((void*)_mappedData, data, static_cast<size_t>(bufferSize));

		return result;
	}

	RESULT ConstantBuffer::Create(ID3D12Device& device, void* data, const UINT& bufferSize)
	{
		// �o�b�t�@�[�T�C�Y�擾
		_bufferSize = AlignmentedSize(bufferSize, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT);

		// �o�b�t�@�[����
		if (FAILED(CreateConstantBuffer(device, bufferSize))) { return RESULT::FAILED; }
		// �}�b�v����
		if (FAILED(MapConstantBuffer(data, bufferSize))) { return RESULT::FAILED; }

		return RESULT::SUCCESS;
	}

}