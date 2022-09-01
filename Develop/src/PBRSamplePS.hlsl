#include "PBRSampleHeader.hlsli"
#include "../../RenderingEngine/src/PBR.hlsli"

struct DemoLight
{
};

float4 main(VSOutput input) : SV_TARGET
{
    float3 lightPos = float3(100, 1000, 0);
    float3 lightColor = float3(3, 3, 3);
    
    float3 N = normalize(input.normal);             // 物体上の法線
    float3 L = normalize(lightPos - input.pos.xyz); // 光の入射方向
    float3 V = normalize(eye - input.pos.xyz);      // 視線方向
    float3 R = normalize(reflect(L, N));            // 光の反射方向
    // マイクロサーフェース上の法線
    // ライトベクトルと視線ベクトルの中間ベクトル(ハーフベクトル)
    float3 H = normalize(V + L);
    
    float NH = saturate(dot(N, H)); //   
    float NV = saturate(dot(N, V)); //  
    float NL = saturate(dot(N, L)); //  入射方向のcos
    float VH = saturate(dot(V, H)); //  出射方向のcos
    
    PBRInput pbrInput;
    pbrInput.NH = NH;
    pbrInput.NV = NV;
    pbrInput.NL = NL;
    pbrInput.VH = VH;
    pbrInput.uv = input.uv;
    pbrInput.lightColor = lightColor;
    
    return PBR(pbrInput);
}

/*
CookTorranceモデル実装メモ
マイクロサーフェースの法線ベクトル : 入射方向ベクトルと出射方向ベクトルの中間にあるハーフベクトル
*/