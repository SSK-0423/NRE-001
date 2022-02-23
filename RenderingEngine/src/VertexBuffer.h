#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <vector>

#pragma comment(lib,"d3d12.lib")

#include "EngineUtility.h"

/// <summary>
/// 頂点バッファークラス
/// </summary>
class VertexBuffer {
public:
	VertexBuffer() = default;
	~VertexBuffer() = default;

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> _vertexBuffer = nullptr;	// 頂点バッファー
	D3D12_VERTEX_BUFFER_VIEW _vertexBufferView;	                            // 頂点バッファービュー
	DirectX::XMFLOAT3* _vertMap = nullptr;	                        // マップ先
	UINT _vertexNum = 0;											// 頂点数

	/// <summary>
	/// 頂点バッファーとビュー生成
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="vertex">頂点データ</param>
	/// <returns></returns>
	HRESULT CreateVertexBufferAndView(ID3D12Device& device, const std::vector<DirectX::XMFLOAT3>& vertex);

	/// <summary>
	/// マップ処理
	/// </summary>
	/// <param name="vertex">頂点データ</param>
	/// <returns></returns>
	HRESULT MapVertexBuffer(const std::vector<DirectX::XMFLOAT3>& vertex);

public:
	/// <summary>
	/// 頂点バッファーを生成する
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="vertex">頂点データ</param>
	/// <returns></returns>
	MYRESULT Create(ID3D12Device& device, const std::vector<DirectX::XMFLOAT3>& vertex);

	/// <summary>
	/// 頂点バッファービュー取得
	/// </summary>
	/// <returns>頂点バッファービュー</returns>
	const D3D12_VERTEX_BUFFER_VIEW& GetView() const {
		return _vertexBufferView;
	}

	/// <summary>
	/// 頂点数取得
	/// </summary>
	/// <returns></returns>
	UINT GetVertexNum() const {
		return _vertexNum;
	}
};