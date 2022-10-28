// Schlick�ɂ��t���l�����̋ߎ���
// cos_in ���ˊp�̃R�T�C��
float3 SchlickFresnel(float2 uv, float VH)
{
    // Fr = F(0) + (1 - F(0))(1 - cos��)^5
    float3 F0 = baseColor.Sample(smp, uv).rgb * metallic.Sample(smp, uv).r;
    
    return F0 + (1.f - F0) * pow((1 - VH), 5);
}

// �@�����z�֐�
// Beckman���z
float Beckman(float2 uv, float NH)
{
    float alpha = roughness.Sample(smp, uv).r;
    alpha = 0.001f;
    float alpha2 = pow(alpha, 2);
    float cos4 = pow(NH, 4);
    float tan_mn2 = (1.f - pow(NH, 2)) / pow(NH, 2);
    
    return (1.f / (alpha2 * cos4)) * exp(-tan_mn2 / alpha2);
}

// �}�C�N���t�@�Z�b�g
// V-cavity���f��
// w_m �}�C�N���T�[�t�F�[�X��̖@��
// w_i ���˕���
// w_o �o�˕���
float Vcavity(float NH, float NV, float NL, float VH)
{
    return min(1.f, min(2.f * NH * NV / VH, 2.f * NH * NL / VH));
}

// ���K�������o�[�g
float3 normalizeLambert(PBRInput input)
{
    float3 diffuse = baseColor.Sample(smp, input.uv).rgb;
    return diffuse * (1.f / PI);
}

// PBR�ɂ����鋾�ʔ��˃��f��
// Cook-Torrance��Karis���f���̔h����
float3 TorranceSparrow(float D, float G, float3 Fr, float cos_o, float cos_i)
{
    return (D * G * Fr) / (4.f * cos_o * cos_i);
}

// Cook-Torrance���f��
float3 CookTorrance(PBRInput input)
{
    float D = Beckman(input.uv, input.NH);
    float G2 = Vcavity(input.NH, input.NV, input.NL, input.VH);
    float3 Fr = SchlickFresnel(input.uv, input.VH);
    float cos_o = input.NL; // �o�˕����̃R�T�C��
    float cos_i = input.NV; // ���˕����̃R�T�C��
    
    return TorranceSparrow(D, G2, Fr, cos_o, cos_i);
}