#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

#include "EngineUtility.h"

struct DepthStencilBufferData
{
	UINT width;
	UINT height;
};

/// <summary>
/// �f�v�X�X�e���V���o�b�t�@�[�N���X
/// </summary>
class DepthStencilBuffer
{
public:
	DepthStencilBuffer() = default;
	~DepthStencilBuffer() = default;

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> _dsvBuffer = nullptr;

public:
	/// <summary>
	/// �f�v�X�X�e���V���o�b�t�@�[����
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	/// <param name="data">�f�v�X�X�e���V���o�b�t�@�[�̐ݒ�</param>
	/// <returns>MYRESULT::SUCCESS: ���� MYRESULT::FAILED: ���s</returns>
	MYRESULT Create(ID3D12Device& device, const DepthStencilBufferData& data);
	
	/// <summary>
	/// �o�b�t�@�[�擾
	/// </summary>
	/// <returns></returns>
	ID3D12Resource& GetBuffer() {
		return *_dsvBuffer.Get();
	}
};

