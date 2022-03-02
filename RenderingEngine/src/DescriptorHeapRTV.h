#pragma once
#include <d3d12.h>
#include <wrl.h>

#pragma comment(lib,"d3d12.lib")

#include "EngineUtility.h"
#include "RenderTargetBuffer.h"

/// <summary>
/// レンダーターゲットビュー用のディスクリプタヒープ
/// </summary>
class DescriptorHeapRTV
{
public:
	DescriptorHeapRTV() = default;
	~DescriptorHeapRTV() = default;

	static constexpr UINT MAXDESCRIPTORNUM = 8;							// 登録可能なディスクリプタ数

private:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> _rtvHeap = nullptr;	// ディスクリプタヒープ
	SIZE_T _handleIncrimentSize = 0;										// ハンドルのインクリメントサイズ
	UINT _registedRTVNum = 0;									// ヒープに登録されたディスクリプタ数
	UINT _nextHandleLocation = 1;

	/// <summary>
	/// ディスクリプタヒープ生成
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <returns></returns>
	HRESULT CreateDescriptorHeap(ID3D12Device& device);

public:
	/// <summary>
	/// RTV用ディスクリプタヒープ生成 これを実行することでディスクリプタヒープが扱えるようになる
	/// </summary>
	/// <param name="device"></param>
	/// <returns>MYRESULT::SUCCESS: 成功 MYRESULT::FAILED: 失敗</returns>
	MYRESULT Create(ID3D12Device& device);

	/// <summary>
	/// CPUのディスクリプタヒープの先頭ハンドルを取得
	/// この関数を実行するとGetNextCPUDescriptorHandleで取得できるハンドルが2番目に初期化される
	/// </summary>
	/// <returns></returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandleForHeapStart() {
		_nextHandleLocation = 1;
		return _rtvHeap->GetCPUDescriptorHandleForHeapStart();
	}

	/// <summary>
	/// 次のディスクリプタのハンドル取得
	/// 初期は2番目のハンドルが取得できる
	/// 登録された数を超えたハンドルを取得しようとすると登録された中の最後のハンドルを返す
	/// </summary>
	/// <returns></returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetNextCPUDescriptorHandle();

	/// <summary>
	/// ディスクリプタヒープハンドルのインクリメントサイズ取得
	/// </summary>
	/// <returns></returns>
	SIZE_T GetHandleIncrimentSize() {
		return _handleIncrimentSize;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	UINT GetRegistedDescriptorNum() {
		return _registedRTVNum;
	}

	/// <summary>
	/// ディスクリプタ登録
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="buffer">レンダーターゲットバッファー</param>
	void RegistDescriptor(ID3D12Device& device, RenderTargetBuffer& buffer);
};