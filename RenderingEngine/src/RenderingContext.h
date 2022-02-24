#pragma once
#include <d3d12.h>

class DescriptorHeapRTV;
class DescriptorHeapDSV;
class VertexBuffer;
class IndexBuffer;
class RootSignature;
class GraphicsPipelineState;
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
	/// �r���[�|�[�g�Z�b�g
	/// </summary>
	/// <param name="viewport">�r���[�|�[�g</param>
	void SetViewport(const D3D12_VIEWPORT& viewport);

	/// <summary>
	/// �r���[�|�[�g�𕡐��Z�b�g
	/// </summary>
	/// <param name="viewportNum">�r���[�|�[�g��</param>
	/// <param name="viewports">�r���[�|�[�g�z��̐擪</param>
	void SetViewports(UINT viewportNum, const D3D12_VIEWPORT& viewports);

	/// <summary>
	/// �V�U�[��`�Z�b�g
	/// </summary>
	void SetScissorRect(const D3D12_RECT& scissorRect);

	/// <summary>
	/// �V�U�[��`�𕡐��Z�b�g
	/// </summary>
	/// <param name="scissorRectNum">�V�U�[��`��</param>
	/// <param name="scissorRect">�V�U�[��`�z��̐擪</param>
	void SetScissorRects(UINT scissorRectNum, const D3D12_RECT& scissorRect);

	/// <summary>
	/// ���_�o�b�t�@�[���Z�b�g
	/// </summary>
	/// <param name="startSlot">�X���b�g�̊J�n�ʒu</param>
	/// <param name="vertexBuffer">���_�o�b�t�@�[</param>
	void SetVertexBuffer(UINT startSlot, const VertexBuffer& vertexBuffer);

	/// <summary>
	/// �C���f�b�N�X�o�b�t�@�[�Z�b�g
	/// </summary>
	/// <param name="indexBuffer">�C���f�b�N�X�o�b�t�@�[</param>
	void SetIndexBuffer(const IndexBuffer& indexBuffer);

	/// <summary>
	/// �p�C�v���C���X�e�[�g�Z�b�g
	/// </summary>
	/// <param name="pipelineState">�O���t�B�b�N�X�p�C�v���C���X�e�[�g</param>
	void SetPipelineState(GraphicsPipelineState& pipelineState);

	/// <summary>
	/// ���[�g�V�O�l�`���Z�b�g
	/// </summary>
	/// <param name="rootSignature">���[�g�V�O�l�`��</param>
	void SetGraphicsRootSignature(RootSignature& rootSignature);

	/// <summary>
	/// �v���~�e�B�u�g�|���W�[�ݒ�
	/// </summary>
	/// <param name="primitiveTopology"></param>
	void SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY primitiveTopology);

	/// <summary>
	/// �C���X�^���X�`��
	/// </summary>
	/// <param name="vertexNumPerInstance">1�C���X�^���X������̒��_��</param>
	/// <param name="instanceNum">�`�悷��C���X�^���X��</param>
	/// <param name="startVertexLocation">���_�f�[�^�̃I�t�Z�b�g �f�t�H���g:0</param>
	/// <param name="startInstanceLocation">�C���X�^���X�̃I�t�Z�b�g �f�t�H���g:0</param>
	void DrawInstanced(
		UINT vertexNumPerInstance, UINT instanceNum,
		UINT startVertexLocation = 0, UINT startInstanceLocation = 0);

	/// <summary>
	/// �C���X�^���X�`��
	/// </summary>
	/// <param name="indexNumPerInstance">1�C���X�^���X������̃C���f�b�N�X��</param>
	/// <param name="instanceNum">�`�悷��C���X�^���X��</param>
	/// <param name="startIndexLocation">���_�f�[�^�̃I�t�Z�b�g �f�t�H���g:0</param>
	/// <param name="baseVertexLocation">���_�o�b�t�@���璸�_��ǂݍ��ޑO�ɁA�e�C���f�b�N�X�ɒǉ������l
	///									�f�t�H���g:0</param>
	/// <param name="startInstanceLocation">�C���X�^���X�̃I�t�Z�b�g �f�t�H���g:0</param>
	void DrawIndexedInstanced(
		UINT indexNumPerInstance, UINT instanceNum,
		UINT startIndexLocation = 0, INT baseVertexLocation = 0, UINT startInstanceLocation = 0);
};