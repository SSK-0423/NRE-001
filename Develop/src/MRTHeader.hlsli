// ���_�V�F�[�_�[�̏o��
struct VertexOutput
{
    float4 svpos : SV_POSITION;
    float2 uv : TEXCOORD;
};

// �s�N�Z���V�F�[�_�[�̏o��
struct PixelOutput
{
    float4 outColor1 : SV_TARGET0;
    float4 outColor2 : SV_TARGET1;
};