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
	/// ポリゴン表示用のパイプラインステート生成
	/// </summary>
	/// <returns></returns>
	MYRESULT CreateGraphicsPipelineState();

public:
	MYRESULT Create();

	void Draw(RenderingContext& renderContext);
};

/// メモ
/// 1.レンダリングコンテキスト
/// 2.GraphicsPipelineState
/// 3.頂点バッファー
/// 4.インデックスバッファー()