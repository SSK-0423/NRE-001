#include "PolygonShaderHeader.hlsli"

// �T���v���[
sampler smp : register(s0);
// �e�N�X�`��
Texture2D tex : register(t0);

float4 PolygonPS(VSOut input) : SV_TARGET
{
    float4 texColor = tex.Sample(smp, input.uv);
    
    return texColor;
}