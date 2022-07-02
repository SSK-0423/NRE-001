#include "MultiRenderTargetHeader.hlsli"

MultiRenderFirstPixelOut main()
{
    MultiRenderFirstPixelOut output;
    output.render1Color = float4(1.f, 0.f, 0.f, 1.f);
    output.render2Color = float4(0.f, 0.f, 1.f, 1.f);
    
    return output;
}