Texture2D colorMap : register(t0);
Texture2D normalMap : register(t1);
Texture2D metalRoughMap : register(t2);

sampler smp : register(s0);

static const float PI = 3.141592f;

struct VertexOut
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

VertexOut VSMain(uint id : SV_VertexID)
{
    float x = (id >> 1) * 2.0 - 1.0; // -1, -1, 1, 1
    float y = 1.0 - (id & 1) * 2.0; //  1, -1, 1, -1
    
    VertexOut output;
    output.position = float4(x, y, 0, 1.f);
    
    float u = (id >> 1);
    float v = (id & 1);
    output.uv = float2(u, v);
    
    return output;
}

float4 PSMain(VertexOut input) : SV_Target
{
    float2 uv = input.uv;
    float3 color = colorMap.Sample(smp, uv).rgb;
    float3 normal = normalMap.Sample(smp, uv).rgb;
    float metallic = metalRoughMap.Sample(smp, uv).r;
    float roughness = metalRoughMap.Sample(smp, uv).g;
    
    return float4(metallic, roughness, 0.f, 1.f);
}