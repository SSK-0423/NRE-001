#include "OffscreenHeader.hlsli"

float4 BackgroundPS(OffOut input) : SV_TARGET
{
    return float4(input.uv.x * input.uv.y, input.uv.x, 1 - input.uv.x, 1.f);
    return float4(0.f, normalize(input.svpos.x), normalize(input.svpos.y), 1.f);
}