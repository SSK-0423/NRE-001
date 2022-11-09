#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <vector>

#pragma comment(lib,"d3d12.lib")

#include "EngineUtility.h"

namespace NamelessEngine::DX12API
{
	class IndexBuffer {
	public:
		IndexBuffer() = default;
		~IndexBuffer() = default;
	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> _indexBuffer = nullptr;	// インデックスバッファー
		D3D12_INDEX_BUFFER_VIEW _indexBufferView;	                    // インデックスバッファービュー
		UINT* _indexMap = nullptr;	                                    // マップ先
		UINT _indexNum = 0;	                                            // インデックス数

		/// <summary>
		/// インデックスバッファーとビュー生成
		/// </summary>
		/// <param name="device">デバイス</param>
		/// <param name="index">インデックスデータ</param>
		/// <returns></returns>
		HRESULT CreateIndexBufferAndView(ID3D12Device& device, const std::vector<UINT>& index);

		/// <summary>
		/// マップ処理
		/// </summary>
		/// <param name="index">インデックスデータ</param>
		/// <returns></returns>
		HRESULT MapIndexBuffer(const std::vector<UINT>& index);

	public:
		/// <summary>
		/// インデックスバッファーを生成する
		/// </summary>
		/// <param name="device">デバイス</param>
		/// <param name="index">インデックスデータ</param>
		/// <returns></returns>
		Utility::RESULT Create(ID3D12Device& device, const std::vector<UINT>& index);

		/// <summary>
		/// インデックスバッファービュー取得
		/// </summary>
		/// <returns>インデックスバッファービュー</returns>
		const D3D12_INDEX_BUFFER_VIEW& GetView() const {
			return _indexBufferView;
		}

		/// <summary>
		/// インデックス数取得
		/// </summary>
		/// <returns>インデックス数</returns>
		UINT GetIndexNum() const {
			return _indexNum;
		}
	};
}