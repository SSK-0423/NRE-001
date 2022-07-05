// PBR用のシェーダー
Texture2D baseColor : register(t0);
Texture2D roughness : register(t1);
Texture2D metallic : register(t2);
Texture2D normal : register(t3);
Texture2D occlusion : register(t4); // オクルージョンの使い方はわからない⇒多分係数としてかける形だと思う

sampler smp : register(s0);

float PI = 3.141592f;

struct PBRInput
{
    float3 eye;
    float3 pos;
    float3 lightDir;
    float3 w_m;   //マイクロサーフェース上の法線
    float3 w_g;
    float3 w_i;   // 入射方向
    float3 w_o;   // 出射方向
    float2 uv;    // テクスチャ座標
    float cos_in; // 入射角のコサイン
    float cos_mn; // マイクロサーフェース上の法線とのコサイン
};

// Schlickによるフレネル項の近似式
// cos_in 入射角のコサイン
float3 SchlickFresnel(float2 uv, float cos_in)
{
    // Fr = F(0) + (1 - F(0))(1 - cosθ)^5
    float3 F0 = metallic.Sample(smp, uv).rgb;
    
    return F0 + (1 - F0) * pow((1 - cos_in), 5);
}

// 法線分布関数
// Beckman分布
float Beckman(float2 uv, float cos_mn)
{
    float alpha = roughness.Sample(smp, uv).r;
    float alpha2 = pow(alpha, 2);
    float cos_mn4 = pow(cos_mn, 4);
    float tan_mn2 = 1.f / pow(cos_mn, 2) - 1.f;
    
    return 1.f / (alpha2 * cos_mn4) * exp(-tan_mn2 / alpha2);
}

// マイクロファセット
// V-cavityモデル
// w_m マイクロサーフェース上の法線
// w_i 入射方向
// w_o 出射方向
float Vcavity(float3 w_m, float3 w_g, float3 w_i, float3 w_o)
{
    float mg = dot(w_m, w_g);
    float og = dot(w_o, w_g);
    float ig = dot(w_i, w_g);
    float om = saturate(dot(w_o, w_m));
    float im = saturate(dot(w_i, w_m));
    
    return min(1, min(2 * mg * og / om, 2 * mg * ig / im));
}

// 正規化ランバート
float3 normalizeLambert(PBRInput input)
{
    float3 diffuse = baseColor.Sample(smp, input.uv).rgb;
    float3 intensity = float3(1.f, 1.f, 1.f) - metallic.Sample(smp, input.uv).rgb;
    
    return diffuse * intensity * (1.f / PI);
}

// PBRにおける鏡面反射モデル
// Cook-TorranceやKarisモデルの派生元
float3 TorranceSparrow(float D, float G, float3 Fr, float cos_o, float cos_i)
{
    return (D * G * Fr) / (4 * cos_o * cos_i);
}

// Cook-Torranceモデル
float3 CookTorrance(PBRInput input)
{
    float D = Beckman(input.uv, input.cos_mn);
    float G2 = Vcavity(input.w_m, input.w_g, input.w_i, input.w_o);
    float3 Fr = SchlickFresnel(input.uv, input.cos_in);
    float cos_o; // 出射方向のコサイン
    float cos_i; // 入射方向のコサイン
    
    return TorranceSparrow(D, G2, Fr, cos_o, cos_i);
}

// PBR
float4 PBR(PBRInput input)
{
    // 拡散項
    float3 lambertColor = normalizeLambert(input);
    
    // 鏡面反射項
    float3 specularColor = CookTorrance(input);
    
    float3 color = lambertColor + specularColor;
    return float4(color, 1.f);
}