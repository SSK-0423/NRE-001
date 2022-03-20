#pragma once
#include "DescriptorHeapCBV_SRV_UAV.h"
#include "Texture.h"
#include "RootSignature.h"
#include "GraphicsPipelineState.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Polygon.h"

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

public:
	void BeginDraw();
	void EndDraw();
	void NextPass();
};