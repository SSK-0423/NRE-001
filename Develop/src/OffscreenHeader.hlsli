// �R���X�^���g�o�b�t�@�[
cbuffer cbuff : register(b0)
{
    matrix worldViewProj;
    matrix mat;
};
// �T���v���[
sampler smp : register(s0);
// �e�N�X�`��
Texture2D tex : register(t0);

struct OffOut
{
    float4 svpos : SV_POSITION;
    float2 uv : TEXCOORD;
};