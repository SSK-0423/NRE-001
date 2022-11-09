float3 Reinhard(float3 color)
{
    return color / (1.f + color);
}