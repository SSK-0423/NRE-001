#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <vector>

#pragma comment(lib,"d3d12.lib")

#include "EngineUtility.h"

namespace NamelessEngine::DX12API
{
	class IndexBuffer {
	public:
		IndexBuffer() = default;
		~IndexBuffer() = default;
	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> _indexBuffer = nullptr;	// �C���f�b�N�X�o�b�t�@�[
		D3D12_INDEX_BUFFER_VIEW _indexBufferView;	                    // �C���f�b�N�X�o�b�t�@�[�r���[
		UINT* _indexMap = nullptr;	                                    // �}�b�v��
		UINT _indexNum = 0;	                                            // �C���f�b�N�X��

		/// <summary>
		/// �C���f�b�N�X�o�b�t�@�[�ƃr���[����
		/// </summary>
		/// <param name="device">�f�o�C�X</param>
		/// <param name="index">�C���f�b�N�X�f�[�^</param>
		/// <returns></returns>
		HRESULT CreateIndexBufferAndView(ID3D12Device& device, const std::vector<UINT>& index);

		/// <summary>
		/// �}�b�v����
		/// </summary>
		/// <param name="index">�C���f�b�N�X�f�[�^</param>
		/// <returns></returns>
		HRESULT MapIndexBuffer(const std::vector<UINT>& index);

	public:
		/// <summary>
		/// �C���f�b�N�X�o�b�t�@�[�𐶐�����
		/// </summary>
		/// <param name="device">�f�o�C�X</param>
		/// <param name="index">�C���f�b�N�X�f�[�^</param>
		/// <returns></returns>
		Utility::RESULT Create(ID3D12Device& device, const std::vector<UINT>& index);

		/// <summary>
		/// �C���f�b�N�X�o�b�t�@�[�r���[�擾
		/// </summary>
		/// <returns>�C���f�b�N�X�o�b�t�@�[�r���[</returns>
		const D3D12_INDEX_BUFFER_VIEW& GetView() const {
			return _indexBufferView;
		}

		/// <summary>
		/// �C���f�b�N�X���擾
		/// </summary>
		/// <returns>�C���f�b�N�X��</returns>
		UINT GetIndexNum() const {
			return _indexNum;
		}
	};
}