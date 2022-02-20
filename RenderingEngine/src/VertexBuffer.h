#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <vector>

#pragma comment(lib,"d3d12.lib")

#include "EngineUtility.h"

/// <summary>
/// ���_�o�b�t�@�[�N���X
/// </summary>
class VertexBuffer {
public:
	VertexBuffer() = default;
	~VertexBuffer() = default;

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> _vertBuff = nullptr;	// ���_�o�b�t�@�[
	D3D12_VERTEX_BUFFER_VIEW _vbView;	                        // ���_�o�b�t�@�[�r���[
	DirectX::XMFLOAT3* _vertMap = nullptr;	                    // �}�b�v��

	/// <summary>
	/// ���_�o�b�t�@�[�����ƃr���[����
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	/// <returns></returns>
	HRESULT CreateVertexBufferAndView(ID3D12Device& device, const std::vector<DirectX::XMFLOAT3>& vertex);

	/// <summary>
	/// �}�b�v����
	/// </summary>
	/// <param name="vertex">���_�f�[�^</param>
	/// <returns></returns>
	HRESULT VertexBufferMap(const std::vector<DirectX::XMFLOAT3>& vertex);

public:
	/// <summary>
	/// ���_�o�b�t�@�[�𐶐�����
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	/// <param name="vertex">���_���X�g</param>
	/// <returns></returns>
	MYRESULT Create(ID3D12Device& device, const std::vector<DirectX::XMFLOAT3>& vertex);
};