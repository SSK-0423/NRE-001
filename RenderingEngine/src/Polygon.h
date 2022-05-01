#pragma once

#include <array>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "GraphicsPipelineState.h"
#include "RootSignature.h"
#include "RenderingContext.h"
#include "Shader.h"
#include "DescriptorHeapCBV_SRV_UAV.h"

#include "EngineUtility.h"

/// <summary>
/// �|���S�������p�f�[�^
/// </summary>
struct PolygonData
{
	Shader vertexShader;	                            // ���_�V�F�[�_�[
	Shader pixelShader;									// �s�N�Z���V�F�[�_�[
	VertexBuffer vertexBuffer;	                        // ���_�o�b�t�@�[
	IndexBuffer indexBuffer;	                        // �C���f�b�N�X�o�b�t�@�[
	RootSignatureData rootSignatureData;				// ���[�g�V�O�l�`��
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;	// ���_���C�A�E�g
	std::array<DXGI_FORMAT, D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT> colorFormats = {
	DXGI_FORMAT_R8G8B8A8_UNORM,
	DXGI_FORMAT_UNKNOWN,
	DXGI_FORMAT_UNKNOWN,
	DXGI_FORMAT_UNKNOWN,
	DXGI_FORMAT_UNKNOWN,
	DXGI_FORMAT_UNKNOWN,
	DXGI_FORMAT_UNKNOWN,
	DXGI_FORMAT_UNKNOWN,
	};	// �����_�[�^�[�Q�b�g�̃J���[�t�H�[�}�b�g

	/// <summary>
	/// �J���[�t�H�[�}�b�g�z��𒲂ׂă����_�[�^�[�Q�b�g����Ԃ�
	/// </summary>
	/// <returns>�����_�[�^�[�Q�b�g��</returns>
	size_t GetRenderTargetNum() const;
};

/// <summary>
/// �|���S���̒��_�f�[�^
/// </summary>
struct PolygonVertex {
	DirectX::XMFLOAT3 vertex;
	DirectX::XMFLOAT2 uv;
};

namespace MyFrameWork
{
	class Polygon
	{
	public:
		Polygon() = default;
		~Polygon() = default;

	private:
		VertexBuffer _vertexBuffer;
		IndexBuffer _indexBuffer;
		GraphicsPipelineState _graphicsPipelineState;
		RootSignature _rootSignature;
		DescriptorHeapCBV_SRV_UAV* _descriptorHeap = nullptr;

		/// <summary>
		/// �|���S���\���p�̃p�C�v���C���X�e�[�g����
		/// </summary>
		/// <returns></returns>
		MYRESULT CreateGraphicsPipelineState(ID3D12Device& device, const PolygonData& data);

	public:
		/// <summary>
		/// �|���S������
		/// </summary>
		/// <param name="device">�f�o�C�X</param>
		/// <param name="data">�|���S���f�[�^</param>
		/// <returns></returns>
		MYRESULT Create(ID3D12Device& device, const PolygonData& data);

		/// <summary>
		/// �`��
		/// </summary>
		/// <param name="renderContext">�����_�����O�R���e�L�X�g</param>
		void Draw(RenderingContext& renderContext);

		void SetDescriptorHeap(DescriptorHeapCBV_SRV_UAV& descriptorHeap);
	};
}

/// ����
/// 1.�����_�����O�R���e�L�X�g
/// 2.GraphicsPipelineState
/// 3.���_�o�b�t�@�[
/// 4.�C���f�b�N�X�o�b�t�@�[()