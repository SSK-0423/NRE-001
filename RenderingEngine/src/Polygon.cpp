#include "Polygon.h"

MYRESULT Polygon::CreateGraphicsPipelineState()
{
	return MYRESULT::SUCCESS;
}

MYRESULT Polygon::Create()
{
	return MYRESULT::SUCCESS;
}

void Polygon::Draw(RenderingContext& renderContext)
{
	renderContext.SetVertexBuffer(0, _vertexBuffer);
	renderContext.SetPipelineState(_graphicsPipelineState);
}
