struct TexMapOut
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

// �T���v���[
sampler smp : register(s0);

// �e�N�X�`��
Texture2D tex : register(t0);