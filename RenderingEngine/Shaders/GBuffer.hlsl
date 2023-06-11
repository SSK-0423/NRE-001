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

cbuffer LightUniform : register(b2)
{
    matrix lightViewProj;
};

struct VertexInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float4 tangent : TANGENT;
    float2 uv : TEXCOORD;
};

struct VertexOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 worldPosition : POSITION;
    float3 lightViewPosition : POSITION1;
    
    float3x3 tangentBasis : TANGENT_BASIS;
    
    // ワールド空間上でライティングする
    float3x3 invTangentBasis : INV_TANGENT_BASIS;
};

struct PixelOutput
{
    float4 color : SV_Target0;
    float4 normal : SV_Target1;
    float4 pos : SV_Target2;
    float4 occMetalRoughShadowFact : SV_Target3;
    float4 emissive : SV_Target4;
};

VertexOutput VSMain(VertexInput input)
{
    VertexOutput output;
    matrix worldViewProj = mul(viewProj, world);
    matrix worldLightViewProj = mul(lightViewProj, world);
    output.position = mul(worldViewProj, float4(input.position, 1.f));
    output.uv = input.uv;
    output.worldPosition = mul(world, float4(input.position, 1.f));
    output.lightViewPosition = mul(lightViewProj, float4(input.position, 1.f));
    
    float3 normal = input.normal;
    float4 tangent = input.tangent;
    
    // 接線空間からワールド空間への基底変換行列を求める
    float3 T = normalize(mul(world, float4(tangent.xyz, 0.f))); // 接線ベクトル
    float3 N = normalize(mul(world, float4(normal, 0.f))); // 法線ベクトル
    float3 B = cross(N, T) * tangent.w; // 従接線ベクトル
    
    output.tangentBasis = float3x3(T, B, N);
    output.invTangentBasis = transpose(float3x3(T, B, N));
    
    return output;
}

PixelOutput PSMain(VertexOutput input)
{
    PixelOutput output;
    output.color = baseColorMap.Sample(smp, input.uv);
    
    float occlusion = occlusionMap.Sample(smp, input.uv).r;
    float roughness = metalRoughMap.Sample(smp, input.uv).g;
    float metallic = metalRoughMap.Sample(smp, input.uv).b;
    float shadowFactor = 0.5f;
    
    output.occMetalRoughShadowFact = float4(occlusion, roughness, metallic, shadowFactor);
    output.pos = float4(input.worldPosition, 1.f);
    output.emissive = emissiveMap.Sample(smp, input.uv);
    
    float3 texSpaceNormal = normalMap.Sample(smp, input.uv).xyz;
    // gltfのノーマルマップはOpenGL形式なのでyを反転させる
    texSpaceNormal.y = 1.f - texSpaceNormal.y;
    
    // -1～1に変換
    texSpaceNormal = texSpaceNormal * 2.f - 1.f;
    
    // テクスチャ空間からワールド空間に変換
    output.normal = float4(mul(input.invTangentBasis, texSpaceNormal), 0.f);
    // 0～1に変換
    output.normal = (output.normal + 1.f) / 2.f;
    output.normal.w = 1.f;
    
    return output;
}