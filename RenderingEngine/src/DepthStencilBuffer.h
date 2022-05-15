#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

#include "EngineUtility.h"

struct DepthStencilBufferData
{
	UINT width;
	UINT height;
};

/// <summary>
/// デプスステンシルバッファークラス
/// </summary>
class DepthStencilBuffer
{
public:
	DepthStencilBuffer() = default;
	~DepthStencilBuffer() = default;

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> _dsvBuffer = nullptr;

public:
	/// <summary>
	/// デプスステンシルバッファー生成
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="data">デプスステンシルバッファーの設定</param>
	/// <returns>MYRESULT::SUCCESS: 成功 MYRESULT::FAILED: 失敗</returns>
	MYRESULT Create(ID3D12Device& device, const DepthStencilBufferData& data);
	
	/// <summary>
	/// バッファー取得
	/// </summary>
	/// <returns></returns>
	ID3D12Resource& GetBuffer() {
		return *_dsvBuffer.Get();
	}
};

