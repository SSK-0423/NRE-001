#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "GraphicsPipelineState.h"
#include "RootSignature.h"
#include "RenderingContext.h"

#include "EngineUtility.h"

struct PolygonData
{
	RootSignature _rootSignature;
	D3D12_INPUT_LAYOUT_DESC _inputLayout;
};

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

	/// <summary>
	/// ポリゴン表示用のパイプラインステート生成
	/// </summary>
	/// <returns></returns>
	MYRESULT CreateGraphicsPipelineState();

public:
	MYRESULT Create(ID3D12Device& device);

	void Draw(RenderingContext& renderContext);
};

/// メモ
/// 1.レンダリングコンテキスト
/// 2.GraphicsPipelineState
/// 3.頂点バッファー
/// 4.インデックスバッファー()