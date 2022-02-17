#include "DescriptorHeapRTV.h"

HRESULT DescriptorHeapRTV::CreateDescriptorHeap(ID3D12Device& device)
{
    D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
    heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    heapDesc.NumDescriptors = 1;
    heapDesc.NodeMask = 0;
    heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

    HRESULT result = device.CreateDescriptorHeap(
        &heapDesc, IID_PPV_ARGS(_rtvHeap.ReleaseAndGetAddressOf()));
    if (FAILED(result)) { return result; }
    
    return result;
}

MYRESULT DescriptorHeapRTV::Create(ID3D12Device& device)
{
    // ハンドルのインクリメントサイズ取得
    _handleIncrimentSize = device.GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    
    // ディスクリプタヒープ生成
    if (FAILED(CreateDescriptorHeap(device))) {
        return MYRESULT::FAILED;
    }
    
    return MYRESULT::SUCCESS;
}