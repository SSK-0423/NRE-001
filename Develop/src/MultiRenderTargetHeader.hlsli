// �T���v���[
sampler smp : register(s0);
// �e�N�X�`��
Texture2D render1 : register(t0);   // �����_�[�^�[�Q�b�g1
Texture2D render2 : register(t1);   // �����_�[�^�[�Q�b�g2


struct MultiRenderOut
{
    float4 svpos : SV_POSITION;
    float2 uv : TEXCOORD;
};