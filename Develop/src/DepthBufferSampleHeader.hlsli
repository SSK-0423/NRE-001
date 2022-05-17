struct DepthSampleVSOut
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

// �R���X�^���g�o�b�t�@�[
cbuffer constBuff : register(b0)
{
    matrix worldViewProj;
    matrix rotationY;
}