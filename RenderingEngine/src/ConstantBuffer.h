#pragma once
#include <d3d12.h>
#include <wrl.h>

#pragma comment(lib,"d3d12.lib")

#include "EngineUtility.h"

/// <summary>
/// �R���X�^���g�o�b�t�@�\�N���X
/// </summary>
class ConstantBuffer {
public:
	ConstantBuffer() = default;
	~ConstantBuffer() = default;

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> _constantBuffer = nullptr;

	/// <summary>
	/// �R���X�^���g�o�b�t�@�\����
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	/// <param name="bufferSize">�o�b�t�@�[�T�C�Y
	/// <returns></returns>
	HRESULT CreateConstantBuffer(ID3D12Device& device, const UINT& bufferSize);

	/// <summary>
	/// �}�b�v����
	/// </summary>
	/// <param name="data">�f�o�C�X</param>
	/// <param name="bufferSize">�o�b�t�@�[�T�C�Y</param>
	/// <returns></returns>
	HRESULT MapConstantBuffer(void* data, const UINT& bufferSize);

public:
	/// <summary>
	/// �R���X�^���g�o�b�t�@�\����
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	/// <param name="data">�o�b�t�@�[�ɓ����f�[�^</param>
	/// <param name="bufferSize">�o�b�t�@�[�T�C�Y</param>
	/// <returns>MYRESULT::SUCCESS: ���� MYRESULT::FAILED: ���s</returns>
	MYRESULT Create(ID3D12Device& device, void* data, const UINT& bufferSize);

	/// <summary>
	/// �o�b�t�@�[�擾
	/// </summary>
	/// <returns>�o�b�t�@�\</returns>
	ID3D12Resource& GetBuffer() {
		return *_constantBuffer.Get();
	}
};