#include "MeshSampleHeader.hlsli"
#include "../../RenderingEngine/src/PhongReflectionModel.hlsli"

// コンスタントバッファー
cbuffer material : register(b0)
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float shiness;
}

float4 main(VSOutput input) : SV_TARGET
{
    PhongReflectionModelData data;
    // 視線
    data.ray = input.ray;
    // ライトの強さ
    data.intensity = float3(1.f, 1.f, 1.f);
    // オブジェクト表面から見たライト方向
    float3 lightDir = float3(1.f, -1.f, -1.f);
    data.direction = normalize(-lightDir);
    // 法線
    data.normal = normalize(input.normal);
    
    // マテリアル属性セット
    data.diffuse = diffuse.rgb;
    data.diffuseFactor = diffuse.a;
    data.specular = specular.rgb;
    data.specularFactor = specular.a;
    data.shiness = shiness;
    data.ambient = ambient.rgb;
    data.ambientFactor = ambient.a;
    
    return PhongReflectionModel(data);
}