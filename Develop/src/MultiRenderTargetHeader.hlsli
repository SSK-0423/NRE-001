// �T���v���[
sampler smp : register(s0);
// �e�N�X�`��
Texture2D render1 : register(t0); // �����_�[�^�[�Q�b�g1
Texture2D render2 : register(t1); // �����_�[�^�[�Q�b�g2

struct MultiRenderOut
{
    float4 svpos : SV_POSITION;
    float2 uv : TEXCOORD;
};

// �P�p�X�ڂ̃s�N�Z���V�F�[�_�[�p
struct MultiRenderFirstPixelOut
{
    float4 render1Color : SV_TARGET0;
    float4 render2Color : SV_TARGET1;
};

// �ŏI�p�X�̃V�F�[�_�[
struct MultiRenderiFinalVertex
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};