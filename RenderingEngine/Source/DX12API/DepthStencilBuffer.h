#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

#include "EngineUtility.h"

namespace NamelessEngine::DX12API
{
	struct DepthStencilBufferData
	{
		DXGI_FORMAT depthFormat;
		UINT width;
		UINT height;
		float clearDepth;
		UINT8 clearStencil;

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="colorFormat">�[�x�t�H�[�}�b�g �f�t�H���g�FDXGI_FORMAT_D32_FLOAT</param>
		/// <param name="width">�����_�[�^�[�Q�b�g�� �f�t�H���g�F1024</param>
		/// <param name="height">�����_�[�^�[�Q�b�g�� �f�t�H���g�F768</param>
		/// <param name="clearColor">�������[�x �f�t�H���g�F1.f</param>
		DepthStencilBufferData(
			const DXGI_FORMAT& depthFormat = DXGI_FORMAT_D32_FLOAT,
			const UINT& width = 1024, const UINT& height = 768,
			const float& clearDepth = 1.f, const UINT8& clearStencil = 0);
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

		DepthStencilBufferData _depthStencilBufferData;

	public:
		/// <summary>
		/// �f�v�X�X�e���V���o�b�t�@�[����
		/// </summary>
		/// <param name="device">�f�o�C�X</param>
		/// <param name="data">�f�v�X�X�e���V���o�b�t�@�[�̐ݒ�</param>
		/// <returns>MYRESULT::SUCCESS: ���� MYRESULT::FAILED: ���s</returns>
		Utility::MYRESULT Create(ID3D12Device& device, const DepthStencilBufferData& data);

		/// <summary>
		/// �o�b�t�@�[�擾
		/// </summary>
		/// <returns></returns>
		ID3D12Resource& GetBuffer() {
			return *_dsvBuffer.Get();
		}

		/// <summary>
		/// �f�v�X�X�e���V���o�b�t�@�[�̐ݒ�\���̂��擾
		/// </summary>
		/// <returns></returns>
		DepthStencilBufferData GetDepthStencilBufferData()
		{
			return _depthStencilBufferData;
		}
	};
}