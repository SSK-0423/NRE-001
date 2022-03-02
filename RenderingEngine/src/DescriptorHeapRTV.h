#pragma once
#include <d3d12.h>
#include <wrl.h>

#pragma comment(lib,"d3d12.lib")

#include "EngineUtility.h"
#include "RenderTargetBuffer.h"

/// <summary>
/// �����_�[�^�[�Q�b�g�r���[�p�̃f�B�X�N���v�^�q�[�v
/// </summary>
class DescriptorHeapRTV
{
public:
	DescriptorHeapRTV() = default;
	~DescriptorHeapRTV() = default;

	static constexpr UINT MAXDESCRIPTORNUM = 8;							// �o�^�\�ȃf�B�X�N���v�^��

private:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> _rtvHeap = nullptr;	// �f�B�X�N���v�^�q�[�v
	SIZE_T _handleIncrimentSize = 0;										// �n���h���̃C���N�������g�T�C�Y
	UINT _registedRTVNum = 0;									// �q�[�v�ɓo�^���ꂽ�f�B�X�N���v�^��
	UINT _nextHandleLocation = 1;

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
	/// ���̊֐������s�����GetNextCPUDescriptorHandle�Ŏ擾�ł���n���h����2�Ԗڂɏ����������
	/// </summary>
	/// <returns></returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandleForHeapStart() {
		_nextHandleLocation = 1;
		return _rtvHeap->GetCPUDescriptorHandleForHeapStart();
	}

	/// <summary>
	/// ���̃f�B�X�N���v�^�̃n���h���擾
	/// ������2�Ԗڂ̃n���h�����擾�ł���
	/// �o�^���ꂽ���𒴂����n���h�����擾���悤�Ƃ���Ɠo�^���ꂽ���̍Ō�̃n���h����Ԃ�
	/// </summary>
	/// <returns></returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetNextCPUDescriptorHandle();

	/// <summary>
	/// �f�B�X�N���v�^�q�[�v�n���h���̃C���N�������g�T�C�Y�擾
	/// </summary>
	/// <returns></returns>
	SIZE_T GetHandleIncrimentSize() {
		return _handleIncrimentSize;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	UINT GetRegistedDescriptorNum() {
		return _registedRTVNum;
	}

	/// <summary>
	/// �f�B�X�N���v�^�o�^
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	/// <param name="buffer">�����_�[�^�[�Q�b�g�o�b�t�@�[</param>
	void RegistDescriptor(ID3D12Device& device, RenderTargetBuffer& buffer);
};