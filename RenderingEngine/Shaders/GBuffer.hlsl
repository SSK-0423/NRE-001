Texture2D baseColorMap : register(t0);
Texture2D metalRoughMap : register(t1);
Texture2D normalMap : register(t2);
Texture2D occlusionMap : register(t3);
Texture2D emissiveMap : register(t4);

sampler smp : register(s0);

cbuffer Uniforms : register(b0)
{
    matrix view;
    matrix proj;
    matrix viewProj;
    float3 eyePos;
};

cbuffer Transform : register(b1)
{
    matrix world;
};

struct VertexInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 uv : TEXCOORD;
};

struct VertexOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 worldPosition : POSITION;
    
    // ���[���h��ԏ�Ń��C�e�B���O����
    float3x3 invTangentBasis : INV_TANGENT_BASIS;
};

struct PixelOutput
{
    float4 color : SV_Target0;
    float4 normal : SV_Target1;
    float4 pos : SV_Target2;
    float4 metalRough : SV_Target3;
    float4 occlusion : SV_Target4;
    float4 emissive : SV_Target5;
};

VertexOutput VSMain(VertexInput input)
{
    VertexOutput output;
    matrix worldViewProj = mul(viewProj, world);
    output.position = mul(worldViewProj, float4(input.position, 1.f));
    output.uv = input.uv;
    output.worldPosition = mul(world, float4(input.position, 1.f));
    
    // �ڐ���Ԃ��烏�[���h��Ԃւ̊��ϊ��s������߂�
    float3 T = normalize(mul(world, float4(input.tangent, 0.f))); // �ڐ��x�N�g��
    float3 N = normalize(mul(world, float4(input.normal, 0.f)));; // �@���x�N�g��
    float3 B = normalize(cross(N, T)); // �]�ڐ��x�N�g��
    
    output.invTangentBasis = transpose(float3x3(T, B, N));
    
    return output;
}

PixelOutput PSMain(VertexOutput input)
{
    PixelOutput output;
    output.color = baseColorMap.Sample(smp, input.uv);
    output.metalRough = float4(0.f, metalRoughMap.Sample(smp, input.uv).b, metalRoughMap.Sample(smp, input.uv).g, 0.f);
    output.pos = float4(input.worldPosition, 1.f);
    output.occlusion = occlusionMap.Sample(smp, input.uv);
    output.emissive = emissiveMap.Sample(smp, input.uv);
    
    // -1�`1�ɕϊ�
    float3 texSpaceNormal = normalMap.Sample(smp, input.uv).xyz * 2.f - 1.f;
    
    // �e�N�X�`����Ԃ��烏�[���h��Ԃɕϊ�
    output.normal = float4(mul(input.invTangentBasis, texSpaceNormal), 1.f);
    
    return output;
}