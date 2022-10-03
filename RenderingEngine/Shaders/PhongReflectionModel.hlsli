struct PhongReflectionModelData
{
    float3 ray;
    float3 intensity;
    float3 direction;
    float3 normal;
    float3 diffuse;
    float diffuseFactor;
    float3 specular;
    float specularFactor;
    float shiness;
    float3 ambient;
    float ambientFactor;
};

float3 PhongReflectionLambert(PhongReflectionModelData data)
{
    float3 intensity = data.intensity;
    float3 direction = data.direction;
    float3 normal = data.normal;
    float3 diffuse = data.diffuse;
    float diffuseFactor = data.diffuseFactor;
    
    float3 lambertColor = intensity * diffuse * dot(direction, normal) * diffuseFactor;
    return lambertColor;
}

float3 PhongReflectionSpecular(PhongReflectionModelData data)
{
    float3 ray = data.ray;
    float3 intensity = data.intensity;
    float3 direction = data.direction;
    float3 normal = data.normal;
    float3 refLight = normalize(reflect(direction, normal));
    
    float3 specular = data.specular;
    float specularFactor = data.specularFactor;
    float shiness = data.shiness;
    
    float3 specularColor = intensity * specular * pow(saturate(dot(refLight, -ray)),shiness) * specularFactor;
    return specularColor;
}

float3 PhongReflectionAmbient(PhongReflectionModelData data)
{
    float3 Ia = float3(1.f, 1.f, 1.f);
    float3 ambient = data.ambient;
    float ambientFactor = data.ambientFactor;
    
    float3 ambientColor = Ia * ambient * ambientFactor;
    
    return saturate(ambientColor);
}

/// ƒtƒHƒ“‚Ì”½ŽËƒ‚ƒfƒ‹
float4 PhongReflectionModel(PhongReflectionModelData data)
{
    float3 color = (0.f, 0.f, 0.f);
    color = PhongReflectionLambert(data) + PhongReflectionSpecular(data) + PhongReflectionAmbient(data);
    
    return float4(color, 1.f);
}