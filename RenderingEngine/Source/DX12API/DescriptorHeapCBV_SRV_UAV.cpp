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

		auto cpuHandle = _descriptorHeap->GetCPUDescriptorHandleForHeapStart();
		auto gpuHandle = _descriptorHeap->GetGPUDescriptorHandleForHeapStart();

		// GPUDescriptorHandleも一緒にインクリメントする必要ある？？

		if (registerNo == NEXT_REGISTER)	// 登録されているリソース数の次のレジスタ
		{
			cpuHandle.ptr += _handleIncrimentSize * (static_cast<SIZE_T>(_registedSRVNum) + _MAX_CBV_DESCRIPTOR_NUM);
			gpuHandle.ptr += _handleIncrimentSize * (static_cast<SIZE_T>(_registedSRVNum) + _MAX_CBV_DESCRIPTOR_NUM);
		}
		else                                // 指定されたレジスタ
		{
			cpuHandle.ptr += _handleIncrimentSize * (static_cast<SIZE_T>(registerNo) + _MAX_CBV_DESCRIPTOR_NUM);
			gpuHandle.ptr += _handleIncrimentSize * (static_cast<SIZE_T>(registerNo) + _MAX_CBV_DESCRIPTOR_NUM);
		}

		device.CreateShaderResourceView(&texture.GetBuffer(), &desc.desc, cpuHandle);

		_registedSRVNum++;
	}

	void DescriptorHeapCBV_SRV_UAV::RegistConstantBuffer(
		ID3D12Device& device, ConstantBuffer& constantBuffer, const int& registerNo)
	{
		assert(registerNo < static_cast<int>(_MAX_CBV_DESCRIPTOR_NUM) && registerNo >= NEXT_REGISTER);

		auto cpuHandle = _descriptorHeap->GetCPUDescriptorHandleForHeapStart();
		auto gpuHandle = _descriptorHeap->GetGPUDescriptorHandleForHeapStart();

		if (registerNo == NEXT_REGISTER)	// 登録されているリソース数の次のレジスタ
		{
			cpuHandle.ptr += _handleIncrimentSize * _registedCBVNum;
			gpuHandle.ptr += _handleIncrimentSize * _registedCBVNum;
		}
		else                                // 指定されたレジスタ
		{
			cpuHandle.ptr += _handleIncrimentSize * registerNo;
			gpuHandle.ptr += _handleIncrimentSize * registerNo;
		}

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = constantBuffer.GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = constantBuffer.GetBufferSize();

		device.CreateConstantBufferView(&cbvDesc, cpuHandle);

		_registedCBVNum++;
	}

	void DescriptorHeapCBV_SRV_UAV::RegistUnorderedAccessResource(
		ID3D12Device& device, UnorderedAccessResource& unorderedAccessResource, const int& registerNo)
	{
		assert(registerNo < static_cast<int>(_MAX_CBV_DESCRIPTOR_NUM) && registerNo >= NEXT_REGISTER);
		// Todo:必要になったら実装
	}
}
