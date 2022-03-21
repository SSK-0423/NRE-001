#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "GraphicsPipelineState.h"
#include "RootSignature.h"
#include "RenderingContext.h"
#include "Shader.h"

#include "EngineUtility.h"

/// <summary>
/// �|���S�������p�f�[�^
/// </summary>
struct PolygonData
{
	Shader _vertexShader;	                            // ���_�V�F�[�_�[
	Shader _pixelShader;	                            // �s�N�Z���V�F�[�_�[
	VertexBuffer _vertexBuffer;	                        // ���_�o�b�t�@�[
	IndexBuffer _indexBuffer;	                        // �C���f�b�N�X�o�b�t�@�[
	RootSignature _rootSignature;	                    // ���[�g�V�O�l�`��
	std::vector<D3D12_INPUT_ELEMENT_DESC> _inputLayout;	// ���_���C�A�E�g
};

/// <summary>
/// �|���S���̒��_�f�[�^
/// </summary>
struct PolygonVertex {
	DirectX::XMFLOAT3 _vertex;
	DirectX::XMFLOAT2 _uv;
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
	};
}

/// ����
/// 1.�����_�����O�R���e�L�X�g
/// 2.GraphicsPipelineState
/// 3.���_�o�b�t�@�[
/// 4.�C���f�b�N�X�o�b�t�@�[()