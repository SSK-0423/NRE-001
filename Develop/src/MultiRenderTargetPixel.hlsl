#include "MultiRenderTargetHeader.hlsli"

float4 main(MultiRenderiFinalVertex input) : SV_TARGET
{
    // ¶‚S•ª‚Ì1‚ÍƒŒƒ“ƒ_[ƒ^[ƒQƒbƒg1‚ğ•`‰æ
    if (input.uv.x >= 0.f && input.uv.x <= 0.25f)
    {
        return render1.Sample(smp, input.uv * 4.f);
    }
    // ‰E‚S•ª‚Ì1‚ÍƒŒƒ“ƒ_[ƒ^[ƒQƒbƒg2‚ğ•`‰æ
    if (input.uv.x >= 0.75f && input.uv.x <= 1.f)
    {
        float2 uv = input.uv - float2(0.75f, 0.f);
        return render2.Sample(smp, uv * 4);
    }

    // ’†‰›‚Í—Î‚ğ•`‰æ
    return float4(0.f, 1.f, 0.f, 1.f);
}