#include "MultiRenderTargetHeader.hlsli"

MultiRenderiFinalVertex MrtVertex(float4 pos : POSITION, float2 uv : TEXCOORD)
{
    MultiRenderiFinalVertex output;
    output.pos = pos;
    output.uv = uv;
    
    return output;
}