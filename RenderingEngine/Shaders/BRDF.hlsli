static const float PI = 3.141592f;
static const float EPSILON = 0.0001f;
static const int COOK_TORRANCE = 0;
static const int GGX_MODEL = 1;

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
float Beckman(float roughness, float VH)
{
    float alpha = pow(roughness, 2);
    alpha = max(EPSILON, roughness);
    float alpha2 = pow(alpha, 2);
    float cos4 = pow(VH, 4);
    float tan_mn2 = (1.f - pow(VH, 2)) / pow(VH, 2);
    
    return (1.f / (alpha2 * cos4)) * exp(-tan_mn2 / alpha2);
}
// GGX
float GGX(float roughness, float NH)
{
    float alpha = pow(roughness, 2);
    float alpha2 = pow(alpha, 2);
    float cos2 = pow(NH, 2);
    float temp = pow(cos2 * (alpha2 - 1.f) + 1, 2);
    
    return alpha2 / max(EPSILON, PI * temp);
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

float Smith(float roughness, float NL, float NV)
{
    float cos2Out = NV * NV;
    float cos2In = NL * NL;
    
    float tan2In = (1.f + cos2In) / cos2In;
    float tan2Out = (1.f + cos2Out) / cos2Out;
    
    float alpha2In = 1.f / (pow(roughness, 2) * tan2In);
    float alpha2Out = 1.f / (pow(roughness, 2) * tan2Out);
    
    float lambdaIn = (-1.f + sqrt(1.f + 1.f / alpha2In)) * 0.5f;
    float lambdaOut = (-1.f + sqrt(1.f + 1.f / alpha2Out)) * 0.5f;
    
    return 1.f / (1.f + lambdaIn, lambdaOut);
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
    return (D * G * Fr) / (4.f * cos_o * cos_i + EPSILON);
}

// Cook-Torranceモデル
float3 CookTorrance(
    float3 baseColor, float metallic, float roughness, float2 uv, float3 N, float3 H, float3 V, float3 L)
{
    float NH = saturate(dot(N, H));
    float NV = saturate(dot(N, V));
    float NL = saturate(dot(N, L));
    float VH = saturate(dot(V, H));
    
    float D = Beckman(roughness, NH);
    float G2 = Vcavity(NH, NV, NL, VH);
    float3 Fr = SchlickFresnel(baseColor, metallic, uv, VH);
    float cos_o = NL; // 出射方向のコサイン
    float cos_i = NV; // 入射方向のコサイン
    
    return TorranceSparrow(D, G2, Fr, cos_o, cos_i);
}

// GGXモデル
float3 GGXModel(
    float3 baseColor, float metallic, float roughness, float2 uv, float3 N, float3 H, float3 V, float3 L)
{
    float NH = saturate(dot(N, H));
    float NV = saturate(dot(N, V));
    float NL = saturate(dot(N, L));
    float VH = saturate(dot(V, H));
    
    float D = GGX(roughness, NH);
    float G2 = Smith(roughness, NL, NV);
    float3 Fr = SchlickFresnel(baseColor, metallic, uv, VH);
    float cos_o = NL; // 出射方向のコサイン
    float cos_i = NV; // 入射方向のコサイン
    
    return TorranceSparrow(D, G2, Fr, cos_o, cos_i);
}