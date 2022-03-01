#include <d3dx12.h>
#include "DescriptorHeapCBV_SRV_UAV.h"

HRESULT DescriptorHeapCBV_SRV_UAV::CreateDescriptorHeap(ID3D12Device& device)
{
    D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
    heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    heapDesc.NodeMask = 0;
    heapDesc.NumDescriptors = MAX_CBV_DESCRIPTOR_NUM + MAX_SRV_DESCRIPTOR_NUM + MAX_UAV_DESCRIPTOR_NUM;
    heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

    HRESULT result = device.CreateDescriptorHeap(
        &heapDesc, IID_PPV_ARGS(_descriptorHeap.ReleaseAndGetAddressOf()));
    if (FAILED(result)) { return result; }
    
    return result;
}

MYRESULT DescriptorHeapCBV_SRV_UAV::Create(ID3D12Device& device)
{
    // ディスクリプタヒープ生成
    if (FAILED(CreateDescriptorHeap(device))) { return MYRESULT::FAILED; }

    return MYRESULT::SUCCESS;
}

void DescriptorHeapCBV_SRV_UAV::RegistShaderResource(ID3D12Device& device, Texture& texture)
{
}

void DescriptorHeapCBV_SRV_UAV::RegistConstantBuffer(ID3D12Device& device, ConstantBuffer& constantBuffer)
{
}
