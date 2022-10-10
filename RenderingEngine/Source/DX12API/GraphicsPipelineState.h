#pragma once

#include <d3d12.h>
#include <wrl.h>

#pragma comment(lib,"d3d12.lib")

#include "EngineUtility.h"

namespace NamelessEngine::DX12API
{
	/// <summary>
	/// �O���t�B�b�N�X�p�C�v���C���X�e�[�g�N���X
	/// </summary>
	class GraphicsPipelineState {
	public:
		GraphicsPipelineState() = default;
		~GraphicsPipelineState() = default;

	private:
		Microsoft::WRL::ComPtr<ID3D12PipelineState> _pipelineState = nullptr;	// �p�C�v���C���X�e�[�g

		D3D12_GRAPHICS_PIPELINE_STATE_DESC _pipelineStateDesc;

		/// <summary>
		/// �p�C�v���C���X�e�[�g����
		/// </summary>
		/// <param name="device">�f�o�C�X</param>
		/// <param name="piplineStateDesc">�O���t�B�b�N�X�p�C�v���C���X�e�[�g�\����</param>
		/// <returns></returns>
		HRESULT CreateGraphicsPipelineState(
			ID3D12Device& device, const D3D12_GRAPHICS_PIPELINE_STATE_DESC& piplineStateDesc);

	public:
		/// <summary>
		/// �p�C�v���C���X�e�[�g����
		/// </summary>
		/// <param name="device">�f�o�C�X</param>
		/// <param name="piplineStateDesc">�O���t�B�b�N�X�p�C�v���C���X�e�[�g�\����</param>
		/// <returns></returns>
		Utility::RESULT Create(ID3D12Device& device, const D3D12_GRAPHICS_PIPELINE_STATE_DESC& piplineStateDesc);

		/// <summary>
		/// �p�C�v���C���X�e�[�g�擾
		/// </summary>
		/// <returns>�p�C�v���C���X�e�[�g</returns>
		ID3D12PipelineState& GetPipelineState() {
			return *_pipelineState.Get();
		}
	};
}