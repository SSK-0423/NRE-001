Texture2D worldPosMap : register(t0);
Texture2D lightDepthMap : register(t1);

sampler smp : register(s0);

cbuffer Light : register(b0)
{
    matrix lightViewProj;
};

struct VertexOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

VertexOutput VSMain(uint id : SV_VertexID)
{
    float x = (id >> 1) * 2.0 - 1.0; // -1, -1, 1, 1
    float y = 1.0 - (id & 1) * 2.0; //  1, -1, 1, -1
    
    VertexOutput output;
    output.position = float4(x, y, 0, 1.f);
    
    float u = (id >> 1);
    float v = (id & 1);
    output.uv = float2(u, v);
    
    return output;
}

float4 PSMain(VertexOutput input) : SV_Target
{
    // シャドウイング
    float4 worldPos = worldPosMap.Sample(smp, input.uv);
    float4 posFromLight = mul(lightViewProj, worldPos);
    
    // -1～1
    posFromLight.xyz /= posFromLight.w;
    
    float2 shadowUV = (posFromLight.xy + float2(1.f, -1.f)) * float2(0.5f, -0.5f);
    
    float posDepth = posFromLight.z;
    
    float4 depth = lightDepthMap.Sample(smp, shadowUV);
    float bias = 0.0001f;
    
    if (posDepth > depth.r + bias)
    {
        return float4(0.5f, 0.5f, 0.5f, 1.f);
    }
    
    return float4(1.f, 1.f, 1.f, 1.f);
}