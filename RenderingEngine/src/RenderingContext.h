#pragma once
#include <d3d12.h>

/// <summary>
/// �����_�����O�R���e�L�X�g�N���X ���̃N���X����ĕ`�施�߂𔭍s����
/// </summary>
class RenderingContext {
public:
	RenderingContext();
	~RenderingContext();
private:
	ID3D12GraphicsCommandList* _cmdList;
public:
	/// <summary>
	/// �����_�����O�R���e�L�X�g������������
	/// </summary>
	/// <param name="cmdList">�O���t�B�N�X�G���W���̃R�}���h���X�g</param>
	void Init(ID3D12GraphicsCommandList& cmdList);

	/// <summary>
	/// �R�}���h���Ăю󂯕t�������Ԃɂ���
	/// </summary>
	/// <param name="cmdAlloc"></param>
	/// <param name="pipeline"></param>
	void Reset(ID3D12CommandAllocator* cmdAlloc, ID3D12PipelineState* pipeline);

};