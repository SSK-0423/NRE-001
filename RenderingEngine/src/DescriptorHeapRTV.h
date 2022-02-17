#pragma once
#include <d3d12.h>
#include <wrl.h>

#pragma comment(lib,"d3d12.lib")

#include "EngineUtility.h"

/// <summary>
/// �����_�[�^�[�Q�b�g�r���[�p�̃f�B�X�N���v�^�q�[�v
/// </summary>
class DescriptorHeapRTV
{
public:
	DescriptorHeapRTV() = default;
	~DescriptorHeapRTV() = default;

private:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> _rtvHeap = nullptr;	// �f�B�X�N���v�^�q�[�v
	UINT _handleIncrimentSize = 0;	                                    // �n���h���̃C���N�������g�T�C�Y

	/// <summary>
	/// �f�B�X�N���v�^�q�[�v����
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	/// <returns></returns>
	HRESULT CreateDescriptorHeap(ID3D12Device& device);

public:
	/// <summary>
	/// RTV�p�f�B�X�N���v�^�q�[�v���� ��������s���邱�ƂŃf�B�X�N���v�^�q�[�v��������悤�ɂȂ�
	/// </summary>
	/// <param name="device"></param>
	/// <returns>MYRESULT::SUCCESS: ���� MYRESULT::FAILED: ���s</returns>
	MYRESULT Create(ID3D12Device& device);

	/// <summary>
	/// CPU�̃f�B�X�N���v�^�q�[�v�̐擪�n���h�����擾
	/// </summary>
	/// <returns></returns>
	D3D12_CPU_DESCRIPTOR_HANDLE* GetCPUDescriptorHandleForHeapStart() {
		return &_rtvHeap->GetCPUDescriptorHandleForHeapStart();
	}

	/// <summary>
	/// �f�B�X�N���v�^�q�[�v�n���h���̃C���N�������g�T�C�Y�擾
	/// </summary>
	/// <returns></returns>
	UINT GetHandleIncrimentSize() {
		return _handleIncrimentSize;
	}
};