#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "GraphicsPipelineState.h"
#include "RenderingContext.h"

#include "EngineUtility.h"

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
	MYRESULT CreateGraphicsPipelineState();

public:
	MYRESULT Create();

	void Draw(RenderingContext& renderContext);
};

/// ����
/// 1.�����_�����O�R���e�L�X�g
/// 2.GraphicsPipelineState
/// 3.���_�o�b�t�@�[
/// 4.�C���f�b�N�X�o�b�t�@�[()