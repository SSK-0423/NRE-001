#include <d3dx12.h>

#include "ConstantBuffer.h"

using namespace Microsoft::WRL;

namespace NamelessEngine::DX12API
{
	HRESULT ConstantBuffer::CreateConstantBuffer(ID3D12Device& device, const UINT& bufferSize)
	{
		CD3DX12_HEAP_PROPERTIES heapProp(D3D12_HEAP_TYPE_UPLOAD);
		CD3DX12_RESOURCE_DESC resDesc = CD3DX12_RESOURCE_DESC::Buffer(
			Utility::AlignmentedSize(bufferSize, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT));

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

	Utility::MYRESULT ConstantBuffer::Create(ID3D12Device& device, void* data, const UINT& bufferSize)
	{
		// バッファーサイズ取得
		_bufferSize = Utility::AlignmentedSize(bufferSize, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT);

		// バッファー生成
		if (FAILED(CreateConstantBuffer(device, bufferSize))) { return Utility::MYRESULT::FAILED; }
		// マップ処理
		if (FAILED(MapConstantBuffer(data, bufferSize))) { return Utility::MYRESULT::FAILED; }

		return Utility::MYRESULT::SUCCESS;
	}

}