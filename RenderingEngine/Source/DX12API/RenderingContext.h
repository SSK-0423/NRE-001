#pragma once
#include <d3d12.h>
#include "EngineUtility.h"

class DescriptorHeapRTV;
class DescriptorHeapDSV;
class DescriptorHeapCBV_SRV_UAV;
class VertexBuffer;
class IndexBuffer;
class RootSignature;
class GraphicsPipelineState;

namespace NamelessEngine::DX12API
{
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
			const UINT& rtvNum, const D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[],
			const D3D12_CPU_DESCRIPTOR_HANDLE* dsvHandle);

		/// <summary>
		/// �w��F�Ń����_�[�^�[�Q�b�g������
		/// </summary>
		/// <param name="rtvHandle">�����_�[�^�[�Q�b�g�r���[�̃n���h��</param>
		/// <param name="color">�������F</param>
		/// <param name="numRects">rects�̗v�f��</param>
		/// <param name="rects">D3D12_RECT�̔z�� nullptr�Ń����_�[�^�[�Q�b�g�̑S�͈͏�����</param>
		void ClearRenderTarget(
			D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, const Utility::ColorRGBA& color,
			const UINT& numRects, const D3D12_RECT* rects);

		/// <summary>
		/// �w��F�Ń����_�[�^�[�Q�b�g������
		/// </summary>
		/// <param name="rtvHandle">�����_�[�^�[�Q�b�g�r���[�̃n���h��</param>
		/// <param name="color">�������F</param>
		/// <param name="numRects">rects�̗v�f��</param>
		/// <param name="rects">D3D12_RECT�̔z�� nullptr�Ń����_�[�^�[�Q�b�g�̑S�͈͏�����</param>
		void ClearRenderTarget(
			D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, const float* color,
			const UINT& numRects, const D3D12_RECT* rects);

		/// <summary>
		/// �f�v�X�X�e���V���̒l������
		/// </summary>
		/// <param name="dsvHandle">�f�v�X�X�e���V���r���[�̃n���h��</param>
		/// <param name="clearFlags">��������̎w��</param>
		/// <param name="clearDepth">�������[�x�l</param>
		/// <param name="clearStencil">�������X�e���V���l</param>
		/// <param name="numRects">rects�̗v�f��</param>
		/// <param name="rects">D3D12_RECT�̔z�� nullptr�Ń����_�[�^�[�Q�b�g�̑S�͈͏�����</param>
		void ClearDepthStencilView(
			D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle, D3D12_CLEAR_FLAGS clearFlags,
			const FLOAT& clearDepth, const UINT8& clearStencil, const UINT& numRects, const D3D12_RECT* rects);

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
		void SetViewports(const UINT& viewportNum, const D3D12_VIEWPORT& viewports);

		/// <summary>
		/// �V�U�[��`�Z�b�g
		/// </summary>
		void SetScissorRect(const D3D12_RECT& scissorRect);

		/// <summary>
		/// �V�U�[��`�𕡐��Z�b�g
		/// </summary>
		/// <param name="scissorRectNum">�V�U�[��`��</param>
		/// <param name="scissorRect">�V�U�[��`�z��̐擪</param>
		void SetScissorRects(const UINT& scissorRectNum, const D3D12_RECT& scissorRect);

		/// <summary>
		/// ���_�o�b�t�@�[���Z�b�g
		/// </summary>
		/// <param name="startSlot">�X���b�g�̊J�n�ʒu</param>
		/// <param name="vertexBuffer">���_�o�b�t�@�[</param>
		void SetVertexBuffer(const UINT& startSlot, const VertexBuffer& vertexBuffer);

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
		void SetPrimitiveTopology(const D3D12_PRIMITIVE_TOPOLOGY& primitiveTopology);

		/// <summary>
		/// �f�B�X�N���v�^�q�[�v���Z�b�g����
		/// </summary>
		/// <param name="descriptorHeap">�f�B�X�N���v�^�q�[�v�̃_�u���|�C���^</param>
		void SetDescriptorHeap(ID3D12DescriptorHeap* const* descriptorHeap);

		/// <summary>
		/// �f�B�X�N���v�^�q�[�v���Z�b�g����
		/// </summary>
		/// <param name="descriptorHeap">�f�B�X�N���v�^�q�[�v�N���X</param>
		void SetDescriptorHeap(DescriptorHeapCBV_SRV_UAV& descriptorHeap);

		/// <summary>
		/// �����̃f�B�X�N���v�^�q�[�v���Z�b�g����
		/// </summary>
		/// <param name="descriptorHeapNum">�Z�b�g����f�B�X�N���v�^�q�[�v��</param>
		/// <param name="descriptorHeaps">�f�B�X�N���v�^�q�[�v�̃_�u���|�C���^</param>
		void SetDescriptorHeaps(const UINT& descriptorHeapNum, ID3D12DescriptorHeap* const* descriptorHeaps);

		/// <summary>
		/// �f�B�X�N���v�^�e�[�u���Z�b�g
		/// </summary>
		/// <param name="rootPrameterIndex">�Z�b�g���郋�[�g�p�����[�^�̃C���f�b�N�X</param>
		/// <param name="handle">�Z�b�g�ς݂̃f�B�X�N���v�^�q�[�v��GPU�n���h��</param>
		void SetGraphicsRootDescriptorTable(const UINT& rootPrameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE handle);

		/// <summary>
		/// �C���X�^���X�`��
		/// </summary>
		/// <param name="vertexNumPerInstance">1�C���X�^���X������̒��_��</param>
		/// <param name="instanceNum">�`�悷��C���X�^���X��</param>
		/// <param name="startVertexLocation">���_�f�[�^�̃I�t�Z�b�g �f�t�H���g:0</param>
		/// <param name="startInstanceLocation">�C���X�^���X�̃I�t�Z�b�g �f�t�H���g:0</param>
		void DrawInstanced(
			const UINT& vertexNumPerInstance, const UINT& instanceNum,
			const UINT& startVertexLocation = 0, const UINT& startInstanceLocation = 0);

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
			const UINT& indexNumPerInstance, const UINT& instanceNum,
			const UINT startIndexLocation = 0, const INT& baseVertexLocation = 0,
			const UINT& startInstanceLocation = 0);

		/// <summary>
		/// �e�N�X�`���̈�̃R�s�[
		/// </summary>
		/// <param name="src">GPU��̃R�s�[���A�h���X</param>
		/// <param name="dst">GPU��̃R�s�[��A�h���X</param>
		/// <param name="dstX">�R�s�[��̈�̃I�t�Z�b�g(X) �f�t�H���g�l:0</param>
		/// <param name="dstY">�R�s�[��̈�̃I�t�Z�b�g(Y) �f�t�H���g�l:0</param>
		/// <param name="dstZ">�R�s�[��̈�̃I�t�Z�b�g(Z) �f�t�H���g�l:0</param>
		/// <param name="srcBox">�R�s�[���̈�{�b�N�X �f�t�H���g�l:nullptr</param>
		void CopyTextureRegion(
			const D3D12_TEXTURE_COPY_LOCATION& src, const D3D12_TEXTURE_COPY_LOCATION& dst,
			const UINT& dstX = 0, const UINT& dstY = 0, const UINT& dstZ = 0, D3D12_BOX* srcBox = nullptr);
	};
}