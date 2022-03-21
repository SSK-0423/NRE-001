#pragma once
#include "DescriptorHeapCBV_SRV_UAV.h"
#include "Texture.h"
#include "RootSignature.h"
#include "GraphicsPipelineState.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Polygon.h"

#include "EngineUtility.h"

#include <d3dx12.h>

class RenderingContext;

/// <summary>
/// �I�t�X�N���[�������_�[�����p�f�[�^
/// </summary>
struct OffScreenRenderData
{
	RenderTargetBufferData renderTargetData;	// �����_�[�^�[�Q�b�g�f�[�^
	RootSignature rootSignature;                // ���[�g�V�O�l�`��
	ShaderData vertexShaderData;                // ���_�V�F�[�_�[�f�[�^
	ShaderData pixelShaderData;                 // �s�N�Z���V�F�[�_�[�f�[�^
	CD3DX12_VIEWPORT viewport;                  // �r���[�|�[�g
	CD3DX12_RECT scissorRect;                   // �V�U�[��`
};

/// <summary>
/// �I�t�X�N���[�������_�[�N���X
/// </summary>
class OffScreenRender
{
private:
	DescriptorHeapCBV_SRV_UAV _offscreenHeap;	        // �I�t�X�N���[���e�N�X�`���p�q�[�v
	Texture _offscreenTexture;	                        // �I�t�X�N���[���e�N�X�`��

	DescriptorHeapRTV _offscreenRTVHeap;	            // �I�t�X�N���[�������_�[�p�q�[�v
	RenderTargetBuffer _offscreenRender;	            // �I�t�X�N���[�������_�[

	RootSignature _offscreenRootSignature;	            // �I�t�X�N���[���p���[�g�V�O�l�`��(���͖��g�p)
	GraphicsPipelineState _offscreenGraphicsPipeline;	// �I�t�X�N���[���p�p�C�v���C��

	VertexBuffer _offscreenPolygonVB;					// �I�t�X�N���[���p���_�o�b�t�@�[
	IndexBuffer _offscreenPolygonIB;					// �I�t�X�N���[���p�C���f�b�N�X�o�b�t�@�[
	MyFrameWork::Polygon _offscreenPolygon;				// �I�t�X�N���[���|���S��
														// ����ɑO�p�X�̃����_�����O���ʂ��e�N�X�`���}�b�v����
	Shader _offscreenVS;	                            // �I�t�X�N���[���|���S���p���_�V�F�[�_�[
	Shader _offscreenPS;	                            // �I�t�X�N���[���|���S���p�s�N�Z���V�F�[�_�[

	OffScreenRenderData _offScreenRenderData;

	/// <summary>
	/// �I�t�X�N���[���|���S������
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	/// <param name="offScreenData">�I�t�X�N���[���f�[�^</param>
	/// <returns>MYRESULT::SUCCESS: ���� MYRESULT::FAILED: ���s</returns>
	MYRESULT CreateOffScreenPolygon(ID3D12Device& device, const OffScreenRenderData& offScreenData);

public:
	/// <summary>
	/// �I�t�X�N���[�������_�[����
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	/// <param name="offScreenRenderData">�I�t�X�N���[�������_�[�f�[�^</param>
	/// <returns>MYRESULT::SUCCESS: ���� MYRESULT::FAILED: ���s</returns>
	MYRESULT Create(ID3D12Device& device, const OffScreenRenderData& offScreenRenderData);

	/// <summary>
	/// �����_�����O�J�n �I�t�X�N���[�������_�����O���J�n����ۂɌĂ�
	/// </summary>
	void BeginRendering(RenderingContext& renderContext);
	/// <summary>
	/// �����_�����O�I��
	/// </summary>
	void EndRendering(RenderingContext& renderContext);
	/// <summary>
	/// ���t���[���Ń����_�[�^�[�Q�b�g�Ƃ��Ďg�p�ł���悤�ɂ���
	/// </summary>
	void NextPass(RenderingContext& renderContext);
	/// <summary>
	/// �����_�����O���ʂ��e�N�X�`���}�b�s���O�����|���S���`��
	/// </summary>
	/// <param name="renderContext"></param>
	void Draw(RenderingContext& renderContext);

	DescriptorHeapCBV_SRV_UAV& GetDescriptorHeap() { return _offscreenHeap; }
};