#include "DepthBufferSampleHeader.hlsli"

// �R���X�^���g�o�b�t�@�[
cbuffer constBuff : register(b0)
{
    float4x4 rotation;
}

DepthSampleVSOut NearPolygonVS( float4 pos : POSITION, float2 uv : TEXCOORD )
{
    DepthSampleVSOut output;
    
    // ��]������
    output.position = pos;
    output.uv = uv;
    
    return output;
}