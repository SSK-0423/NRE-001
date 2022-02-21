#pragma once
#include <d3d12.h>

class DescriptorHeapRTV;
class DescriptorHeapDSV;
class VertexBuffer;
struct ColorRGBA;

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
	/// �����_�[�^�[�Q�b�g���P�Z�b�g����
	/// </summary>
	/// <param name="_rtvHandle">�����_�[�^�[�Q�b�g�r���[�̃n���h��</param>
	/// <param name="_dsvHandle">�f�v�X�X�e���V���r���[�̃n���h��</param>
	void SetRenderTarget(
		const D3D12_CPU_DESCRIPTOR_HANDLE* rtvHandle, const D3D12_CPU_DESCRIPTOR_HANDLE* dsvHandle);

	/// <summary>
	/// �����̃����_�[�^�[�Q�b�g���Z�b�g����
	/// </summary>
	/// <param name="rtvNum">�Z�b�g���郌���_�[�^�[�Q�b�g��</param>
	/// <param name="rtvHandles">�����_�[�^�[�Q�b�g�r���[�̃n���h���̔z��</param>
	/// <param name="dsvHandle">�f�v�X�X�e���V���r���[�̃n���h��</param>
	void SetRenderTargets(
		UINT rtvNum, const D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[],
		const D3D12_CPU_DESCRIPTOR_HANDLE* dsvHandle);

	/// <summary>
	/// �w��F�Ń����_�[�^�[�Q�b�g������
	/// </summary>
	/// <param name="rtvHandle">�����_�[�^�[�Q�b�g�r���[�̃n���h��</param>
	/// <param name="color">�������F</param>
	/// <param name="numRects">rects�̗v�f��</param>
	/// <param name="rects">D3D12_RECT�̔z�� nullptr�Ń����_�[�^�[�Q�b�g�̑S�͈͏�����</param>
	void ClearRenderTarget(
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, const ColorRGBA& color, UINT numRects, const D3D12_RECT* rects);

	/// <summary>
	/// �w��F�Ń����_�[�^�[�Q�b�g������
	/// </summary>
	/// <param name="rtvHandle">�����_�[�^�[�Q�b�g�r���[�̃n���h��</param>
	/// <param name="color">�������F</param>
	/// <param name="numRects">rects�̗v�f��</param>
	/// <param name="rects">D3D12_RECT�̔z�� nullptr�Ń����_�[�^�[�Q�b�g�̑S�͈͏�����</param>
	void ClearRenderTarget(
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, const float* color, UINT numRects, const D3D12_RECT* rects);

	/// <summary>
	/// ���_�o�b�t�@�[���Z�b�g
	/// </summary>
	/// <param name="startSlot">�X���b�g�̊J�n�ʒu</param>
	/// <param name="vertexBuffer">���_�o�b�t�@�[</param>
	void SetVertexBuffer(UINT startSlot, const VertexBuffer& vertexBuffer);

};