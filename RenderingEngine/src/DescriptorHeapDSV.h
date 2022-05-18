#pragma once

#include <d3d12.h>
#include <wrl.h>

#pragma comment(lib,"d3d12.lib")

#include "EngineUtility.h"
#include "DepthStencilBuffer.h"

class DescriptorHeapDSV
{
public:
	DescriptorHeapDSV() = default;
	~DescriptorHeapDSV() = default;

private:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> _dsvHeap = nullptr;		// �f�B�X�N���v�^�q�[�v
	SIZE_T _handleIncrimentSize = 0;										// �n���h���̃C���N�������g�T�C�Y
	UINT _registedDSVNum = 0;									            // �q�[�v�ɓo�^���ꂽ�f�B�X�N���v�^��
	
	/// <summary>
	/// �f�B�X�N���v�^�q�[�v����
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	/// <returns></returns>
	HRESULT CreateDescriptorHeap(ID3D12Device& device);

public:
	/// <summary>
	/// DSV�p�f�B�X�N���v�^�q�[�v���� ��������s���邱�ƂŃf�B�X�N���v�^�q�[�v��������悤�ɂȂ�
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	/// <returns>MYRESULT::SUCCESS: ���� MYRESULT::FAILED: ���s</returns>
	MYRESULT Create(ID3D12Device& device);

	/// <summary>
	/// �f�B�X�N���v�^�q�[�v�n���h���̃C���N�������g�T�C�Y�擾
	/// </summary>
	/// <returns></returns>
	SIZE_T GetHandleIncrimentSize() {
		return _handleIncrimentSize;
	}

	/// <summary>
	/// �o�^�ς݂̃f�B�X�N���v�^���擾
	/// </summary>
	/// <returns></returns>
	UINT GetRegistedDescriptorNum() {
		return _registedDSVNum;
	}

	/// <summary>
	/// �f�B�X�N���v�^�o�^
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	/// <param name="buffer">�f�v�X�X�e���V���o�b�t�@�[</param>
	void RegistDescriptor(ID3D12Device& device, DepthStencilBuffer& buffer);

	/// <summary>
	/// CPU�̃f�B�X�N���v�^�q�[�v�̐擪�n���h�����擾
	/// </summary>
	/// <returns></returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandleForHeapStart() {
		return _dsvHeap->GetCPUDescriptorHandleForHeapStart();
	}
};