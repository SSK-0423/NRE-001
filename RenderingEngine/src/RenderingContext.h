#pragma once
#include <d3d12.h>

class RenderTarget;

/// <summary>
/// �����_�����O�R���e�L�X�g�N���X ���̃N���X����ĕ`�施�߂𔭍s����
/// </summary>
class RenderingContext {
public:
	RenderingContext();
	~RenderingContext();
private:
	ID3D12GraphicsCommandList* _cmdList = nullptr;
public:
	/// <summary>
	/// �����_�����O�R���e�L�X�g������������
	/// </summary>
	/// <param name="cmdList">�O���t�B�N�X�G���W���̃R�}���h���X�g</param>
	void Init(ID3D12GraphicsCommandList& cmdList);

	/// <summary>
	/// �R�}���h���X�g�̖��ߎ�t�I��
	/// </summary>
	void Close();

	/// <summary>
	/// �R�}���h���X�g�ƃR�}���h�A���P�[�^��������Ԃɂ���
	/// </summary>
	/// <param name="cmdAlloc">�Z�b�g�����R�}���h���X�g�𐶐�����ۂɎw�肵���R�}���h�A���P�[�^</param>
	void Reset(ID3D12CommandAllocator& cmdAlloc);

	/// <summary>
	/// ���\�[�X�̏�ԑJ��
	/// </summary>
	/// <param name="resource">�J�ڑΏۂ̃��\�[�X</param>
	/// <param name="beforeState">���݂̏��</param>
	/// <param name="afterState">���̏��</param>
	void TransitionResourceState(
		ID3D12Resource& resource, D3D12_RESOURCE_STATES presentState, D3D12_RESOURCE_STATES afterState);

	/// <summary>
	/// �����_�[�^�[�Q�b�g�Z�b�g
	/// </summary>
	/// <param name="renderTarget"></param>
	/// <param name="descriptorContinuous"></param>
	void SetRenderTargets(RenderTarget& renderTarget,BOOL descriptorContinuous);

};