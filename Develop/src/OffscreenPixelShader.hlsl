#include "OffscreenHeader.hlsli"

float4 filetring(OffOut output, float3x3 filter)
{
    float w, h, levels;
    tex.GetDimensions(0, w, h, levels);
    float dx = 1.f / w; //1�s�N�Z��������̈ړ���
    float dy = 1.f / h; //1�s�N�Z��������̈ړ���
    float4 ret = float4(0, 0, 0, 0);

    ret += tex.Sample(smp, output.uv + float2(-2 * dx, -2 * dy)) * filter._m00; //����
    ret += tex.Sample(smp, output.uv + float2(0, -2 * dy)) * filter._m01; //��
    ret += tex.Sample(smp, output.uv + float2(2 * dx, -2 * dy)) * filter._m02; //�E��
    ret += tex.Sample(smp, output.uv + float2(-2 * dx, 0)) * filter._m10; //��
    ret += tex.Sample(smp, output.uv + float2(0, 0)) * filter._m11; //����
    ret += tex.Sample(smp, output.uv + float2(2 * dx, 0)) * filter._m12; //�E
    ret += tex.Sample(smp, output.uv + float2(-2 * dx, 2 * dy)) * filter._m20; //����
    ret += tex.Sample(smp, output.uv + float2(0, 2 * dy)) * filter._m21; //��
    ret += tex.Sample(smp, output.uv + float2(2 * dx, 2 * dy)) * filter._m22; //�E��
    return ret;
}

float4 OffscreenPS(OffOut input) : SV_TARGET
{
    //�������t�B���^�[
    float3x3 smoothing =
    {
        { 1 / 9, 1 / 9, 1 / 9 },
        { 1 / 9, 1 / 9, 1 / 9 },
        { 1 / 9, 1 / 9, 1 / 9 }
    };
    //�G���{�X���H
    float3x3 emboss =
    {
        { 2, 1, 0 },
        { 1, 1, -1 },
        { 0, -1, -2 }
    };
    //�V���[�v�l�X(�G�b�W�̋���)
    float3x3 shapness =
    {
        { 0, -1, 0 },
        { -1, 5, -1 },
        { 0, -1, 0 }
    };
    //�G�b�W
    float3x3 edge =
    {
        { 0, -1, 0 },
        { -1, 4, -1 },
        { 0, -1, 0 }
    };
    return filetring(input, emboss);
    
    //float4 texColor = tex.Sample(smp, input.uv);
    //float4 output = 1.f - texColor;
    
    //return output;
    //return texColor;
}

