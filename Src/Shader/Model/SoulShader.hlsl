SamplerState TextureSampler : register(s0);
Texture2D ParticleTexture : register(t0);

// �s�N�Z���V�F�[�_�[�̃��C���֐�
float4 SoulEffectPS(float4 pos : SV_POSITION, float2 uv : TEXCOORD0) : SV_TARGET
{
    // �e�N�X�`���J���[���擾
    float4 color = ParticleTexture.Sample(TextureSampler, uv);

    // UV��U�����ɃG���R�[�h���ꂽ���ԏ����擾
    float time = uv.x;

    // �����ɂ��t�F�[�h����
    float2 center = float2(0.5, 0.5);
    float dist = length(uv - center); // ���S����̋���
    float fade = smoothstep(0.4, 0.5, dist);
    color.a *= (1.0 - fade);

    // �O���[���ʂ�\������F����
    float glowIntensity = 0.5 + 0.5 * sin(time * 2.0);
    color.rgb *= float3(0.5, 0.7, 1.0) * glowIntensity;

    return color;
}

// �G���g���[�|�C���g
float4 main(float4 pos : SV_POSITION, float2 uv : TEXCOORD0) : SV_TARGET
{
    return SoulEffectPS(pos, uv);
}