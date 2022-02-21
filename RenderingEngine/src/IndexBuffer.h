#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <vector>

#pragma comment(lib,"d3d12.lib")

class IndexBuffer {
public:
	IndexBuffer() = default;
	~IndexBuffer() = default;
private:
	Microsoft::WRL::ComPtr<ID3D12Resource> _indexBuffer = nullptr;	// インデックスバッファー
};