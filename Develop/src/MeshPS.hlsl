#include "MeshSampleHeader.hlsli"

float4 main(VSOutput input) : SV_TARGET
{
	
    // ライト
    float3 intensity = float3(1.f, 1.f, 1.f);
    float3 lightPos = float3(0.f, 5.f, -2.f);
    
    // オブジェクト表面から見たライト方向
    float3 direction = normalize(lightPos - input.pos.xyz);
    
    // 法線
    float3 normal = normalize(input.normal);
    
	// フォンの反射モデル
    // 拡散反射
    float3 diffuse = float3(0.5f, 0.5f, 0.5f);
    float3 lambertColor = intensity * diffuse * dot(direction, normal);
    
	// 鏡面反射
    float3 specular = float3(0.5f, 0.5f, 0.5f);
    float shiness = 8.f;
    float3 refLight = normalize(reflect(direction, normal));
    // 内積なので向きを合わせる
    float3 specularColor = intensity * specular * pow(saturate(dot(refLight, -input.ray)), shiness);
    
    
    // 環境光
    float3 ambient = float3(0.3f, 0.3f, 0.3f);
    float3 Ia = float3(1.f, 1.f, 1.f);
    float3 ambientColor = Ia * ambient;
    
    float4 color = float4(lambertColor + specularColor + ambientColor, 1.f);
    return color;
}