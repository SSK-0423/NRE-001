#include <cassert>

#include "DescriptorHeapRTV.h"
#include "RenderTargetBuffer.h"

using namespace NamelessEngine::Utility;

namespace NamelessEngine::DX12API
{
	HRESULT DescriptorHeapRTV::CreateDescriptorHeap(ID3D12Device& device)
	{
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		heapDesc.NumDescriptors = MAXDESCRIPTORNUM;
		heapDesc.NodeMask = 0;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

		HRESULT result = device.CreateDescriptorHeap(
			&heapDesc, IID_PPV_ARGS(_rtvHeap.ReleaseAndGetAddressOf()));
		if (FAILED(result)) { return result; }

		return result;
	}

	RESULT DescriptorHeapRTV::Create(ID3D12Device& device)
	{
		// ハンドルのインクリメントサイズ取得
		_handleIncrimentSize =
			static_cast<SIZE_T>(device.GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));

		// ディスクリプタヒープ生成
		if (FAILED(CreateDescriptorHeap(device))) { return RESULT::FAILED; }

		return RESULT::SUCCESS;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHeapRTV::GetNextCPUDescriptorHandle()
	{
		// 範囲外参照を防ぐ
		assert(_nextHandleLocation <= _registedRTVNum);

		// 次のハンドルへ
		auto handle = _rtvHeap->GetCPUDescriptorHandleForHeapStart();
		handle.ptr += _nextHandleLocation * _handleIncrimentSize;

		// 次のハンドル位置を指す
		_nextHandleLocation++;

		return handle;
	}

	void DescriptorHeapRTV::RegistDescriptor(
		ID3D12Device& device, RenderTargetBuffer& buffer, DXGI_FORMAT format)
	{
		auto handle = _rtvHeap->GetCPUDescriptorHandleForHeapStart();
		handle.ptr += _registedRTVNum * _handleIncrimentSize;

		// レンダーターゲットビュー設定
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};

		rtvDesc.Format = format;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

		// レンダーターゲットビュー生成
		device.CreateRenderTargetView(&buffer.GetBuffer(), &rtvDesc, handle);

		// 登録済みのディスクリプタ数をインクリメント
		_registedRTVNum++;
	}

}