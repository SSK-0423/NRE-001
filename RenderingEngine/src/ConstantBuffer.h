#pragma once
#include <d3d12.h>
#include <wrl.h>

#pragma comment(lib,"d3d12.lib")

#include "EngineUtility.h"

/// <summary>
/// コンスタントバッファ―クラス
/// </summary>
class ConstantBuffer {
public:
	ConstantBuffer() = default;
	~ConstantBuffer() = default;

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> _constantBuffer = nullptr;

	/// <summary>
	/// コンスタントバッファ―生成
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="bufferSize">バッファーサイズ
	/// <returns></returns>
	HRESULT CreateConstantBuffer(ID3D12Device& device, const UINT& bufferSize);

	/// <summary>
	/// マップ処理
	/// </summary>
	/// <param name="data">デバイス</param>
	/// <param name="bufferSize">バッファーサイズ</param>
	/// <returns></returns>
	HRESULT MapConstantBuffer(void* data, const UINT& bufferSize);

public:
	/// <summary>
	/// コンスタントバッファ―生成
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="data">バッファーに入れるデータ</param>
	/// <param name="bufferSize">バッファーサイズ</param>
	/// <returns>MYRESULT::SUCCESS: 成功 MYRESULT::FAILED: 失敗</returns>
	MYRESULT Create(ID3D12Device& device, void* data, const UINT& bufferSize);

	/// <summary>
	/// バッファー取得
	/// </summary>
	/// <returns>バッファ―</returns>
	ID3D12Resource& GetBuffer() {
		return *_constantBuffer.Get();
	}
};