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

// �e�N�X�`��
Texture2D<float4> renderTex : register(t0);
Texture2D<float> depthTex : register(t1);

// �T���v���[
sampler smp : register(s0);