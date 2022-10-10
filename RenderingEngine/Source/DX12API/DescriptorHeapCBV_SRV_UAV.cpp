#include <d3dx12.h>
#include <cassert>
#include "DescriptorHeapCBV_SRV_UAV.h"

#include "Texture.h"
#include "ConstantBuffer.h"

#include "ShaderResourceViewDesc.h"

using namespace NamelessEngine::Utility;

namespace NamelessEngine::DX12API
{
	HRESULT DescriptorHeapCBV_SRV_UAV::CreateDescriptorHeap(ID3D12Device& device)
	{
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		heapDesc.NodeMask = 0;
		heapDesc.NumDescriptors = _MAX_CBV_DESCRIPTOR_NUM + _MAX_SRV_DESCRIPTOR_NUM + _MAX_UAV_DESCRIPTOR_NUM;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

		HRESULT result = device.CreateDescriptorHeap(
			&heapDesc, IID_PPV_ARGS(_descriptorHeap.ReleaseAndGetAddressOf()));
		if (FAILED(result)) { return result; }

		return result;
	}

	RESULT DescriptorHeapCBV_SRV_UAV::Create(ID3D12Device& device)
	{
		// ハンドルのインクリメントサイズ取得
		_handleIncrimentSize = device.GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		// ディスクリプタヒープ生成
		if (FAILED(CreateDescriptorHeap(device))) { return RESULT::FAILED; }

		return RESULT::SUCCESS;
	}

	void DescriptorHeapCBV_SRV_UAV::RegistShaderResource(
		ID3D12Device& device, Texture& texture,
		ShaderResourceViewDesc& desc, const int& registerNo)
	{
		assert(registerNo < static_cast<int>(_MAX_SRV_DESCRIPTOR_NUM) && registerNo >= NEXT_REGISTER);

		auto handle = _descriptorHeap->GetCPUDescriptorHandleForHeapStart();

		if (registerNo == NEXT_REGISTER)	// 登録されているリソース数の次のレジスタ
			handle.ptr += _handleIncrimentSize * (static_cast<SIZE_T>(_registedSRVNum) + _MAX_CBV_DESCRIPTOR_NUM);
		else                                // 指定されたレジスタ
			handle.ptr += _handleIncrimentSize * (static_cast<SIZE_T>(registerNo) + _MAX_CBV_DESCRIPTOR_NUM);

		//D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		//srvDesc.Format = texture.GetImage().format;
		//srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		//srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		//srvDesc.Texture2D.MipLevels = texture.GetTexMetadata().mipLevels;

		device.CreateShaderResourceView(&texture.GetBuffer(), &desc.desc, handle);

		_registedSRVNum++;
	}

	void DescriptorHeapCBV_SRV_UAV::RegistConstantBuffer(
		ID3D12Device& device, ConstantBuffer& constantBuffer, const int& registerNo)
	{
		assert(registerNo < static_cast<int>(_MAX_CBV_DESCRIPTOR_NUM) && registerNo >= NEXT_REGISTER);

		auto handle = _descriptorHeap->GetCPUDescriptorHandleForHeapStart();

		if (registerNo == NEXT_REGISTER)	// 登録されているリソース数の次のレジスタ
			handle.ptr += _handleIncrimentSize * _registedCBVNum;
		else                                // 指定されたレジスタ
			handle.ptr += _handleIncrimentSize * registerNo;

		// Todo:後で実装
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = constantBuffer.GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = constantBuffer.GetBufferSize();

		device.CreateConstantBufferView(&cbvDesc, handle);

		_registedCBVNum++;
	}

	void DescriptorHeapCBV_SRV_UAV::RegistUnorderedAccessResource(
		ID3D12Device& device, UnorderedAccessResource& unorderedAccessResource, const int& registerNo)
	{
		assert(registerNo < static_cast<int>(_MAX_CBV_DESCRIPTOR_NUM) && registerNo >= NEXT_REGISTER);
		// Todo:必要になったら実装
	}
}
