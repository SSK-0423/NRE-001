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
	Microsoft::WRL::ComPtr<ID3D12Resource> _vertexBuffer = nullptr;	// ���_�o�b�t�@�[
	D3D12_VERTEX_BUFFER_VIEW _vertexBufferView;	                    // ���_�o�b�t�@�[�r���[
	DirectX::XMFLOAT3* _vertMap = nullptr;	                        // �}�b�v��
	UINT _vertexNum = 0;											// ���_��

	/// <summary>
	/// ���_�o�b�t�@�[�ƃr���[����
	/// </summary>
	/// <param name="device"></param>
	/// <param name="sizeInBytes"></param>
	/// <param name="stribeInBytes"></param>
	/// <returns></returns>
	HRESULT CreateVertexBufferAndView(ID3D12Device& device, const UINT& sizeInBytes, const UINT& stribeInBytes);

	/// <summary>
	/// �}�b�v����
	/// </summary>
	/// <param name="vertexData">���_�f�[�^</param>
	/// <param name="sizeInBytes">���_�f�[�^�̃T�C�Y</param>
	/// <returns></returns>
	HRESULT MapVertexBuffer(void* vertexData, const UINT& sizeInBytes);

public:
	/// <summary>
	/// 
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	/// <param name="sizeInBytes">�o�b�t�@�[�̑S�T�C�Y</param>
	/// <param name="stribeInBytes">�P�v�f�ӂ�̃T�C�Y</param>
	/// <returns></returns>
	MYRESULT Create(ID3D12Device& device, void* vertexData, const UINT& sizeInBytes, const UINT& stribeInBytes);

	/// <summary>
	/// ���_�o�b�t�@�[�r���[�擾
	/// </summary>
	/// <returns>���_�o�b�t�@�[�r���[</returns>
	const D3D12_VERTEX_BUFFER_VIEW& GetView() const {
		return _vertexBufferView;
	}

	/// <summary>
	/// ���_���擾
	/// </summary>
	/// <returns>���_��</returns>
	UINT GetVertexNum() const {
		return _vertexNum;
	}
};