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
	Microsoft::WRL::ComPtr<ID3D12Resource> _indexBuffer = nullptr;	// �C���f�b�N�X�o�b�t�@�[
	D3D12_INDEX_BUFFER_VIEW _idView;

public:
	/// <summary>
	/// �C���f�b�N�X�o�b�t�@�[�r���[�擾
	/// </summary>
	/// <returns>�C���f�b�N�X�o�b�t�@�[�r���[</returns>
	const D3D12_INDEX_BUFFER_VIEW& GetView() const {
		return _idView;
	}
};