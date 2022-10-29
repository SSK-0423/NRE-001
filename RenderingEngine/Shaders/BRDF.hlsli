static const float PI = 3.141592f;

// Schlickによるフレネル項の近似式
// cos_in 入射角のコサイン
float3 SchlickFresnel(float3 baseColor, float metallic, float2 uv, float VH)
{
    // Fr = F(0) + (1 - F(0))(1 - cosθ)^5
    float3 F0 = baseColor * metallic;
    
    return F0 + (1.f - F0) * pow((1 - VH), 5);
}

// 法線分布関数
// Beckman分布
float Beckman(float roughness, float2 uv, float NH)
{
    float alpha = roughness;
    alpha = 0.001f;
    float alpha2 = pow(alpha, 2);
    float cos4 = pow(NH, 4);
    float tan_mn2 = (1.f - pow(NH, 2)) / pow(NH, 2);
    
    return (1.f / (alpha2 * cos4)) * exp(-tan_mn2 / alpha2);
}

// マイクロファセット
// V-cavityモデル
// w_m マイクロサーフェース上の法線
// w_i 入射方向
// w_o 出射方向
float Vcavity(float NH, float NV, float NL, float VH)
{
    return min(1.f, min(2.f * NH * NV / VH, 2.f * NH * NL / VH));
}

// 正規化ランバート
float3 normalizeLambert(float3 baseColor)
{
    return baseColor * (1.f / PI);
}

// PBRにおける鏡面反射モデル
// Cook-TorranceやKarisモデルの派生元
float3 TorranceSparrow(float D, float G, float3 Fr, float cos_o, float cos_i)
{
    return (D * G * Fr) / (4.f * cos_o * cos_i);
}

// Cook-Torranceモデル
float3 CookTorrance(float roughness, float2 uv, float3 normal, float3 halfVec,float3 view, float light)
{
    float NH = dot(normal, halfVec);
    float NV = dot(normal, view);
    float NL = dot(normal, light);
    float VH = dot(view, halfVec);
    
    return float3(0, 0, 0);
    //float D = Beckman(input.uv, input.NH);
    //float G2 = Vcavity(input.NH, input.NV, input.NL, input.VH);
    //float3 Fr = SchlickFresnel(input.uv, input.VH);
    //float cos_o = input.NL; // 出射方向のコサイン
    //float cos_i = input.NV; // 入射方向のコサイン
    
    //return TorranceSparrow(D, G2, Fr, cos_o, cos_i);
}