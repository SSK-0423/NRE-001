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
	D3D12_VERTEX_BUFFER_VIEW _vbView;	                            // 頂点バッファービュー
	DirectX::XMFLOAT3* _vertMap = nullptr;	                        // マップ先
	UINT _vertexNum = 0;

	/// <summary>
	/// 頂点バッファー生成とビュー生成
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <returns></returns>
	HRESULT CreateVertexBufferAndView(ID3D12Device& device, const std::vector<DirectX::XMFLOAT3>& vertex);

	/// <summary>
	/// マップ処理
	/// </summary>
	/// <param name="vertex">頂点データ</param>
	/// <returns></returns>
	HRESULT VertexBufferMap(const std::vector<DirectX::XMFLOAT3>& vertex);

public:
	/// <summary>
	/// 頂点バッファーを生成する
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="vertex">頂点リスト</param>
	/// <returns></returns>
	MYRESULT Create(ID3D12Device& device, const std::vector<DirectX::XMFLOAT3>& vertex);

	/// <summary>
	/// 頂点バッファービュー取得
	/// </summary>
	/// <returns>頂点バッファービュー</returns>
	const D3D12_VERTEX_BUFFER_VIEW& GetView() const {
		return _vbView;
	}

	UINT GetVertexNum() const {
		return _vertexNum;
	}
};