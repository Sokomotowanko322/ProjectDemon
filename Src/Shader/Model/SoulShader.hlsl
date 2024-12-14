SamplerState TextureSampler : register(s0);
Texture2D ParticleTexture : register(t0);

// ピクセルシェーダーのメイン関数
float4 SoulEffectPS(float4 pos : SV_POSITION, float2 uv : TEXCOORD0) : SV_TARGET
{
    // テクスチャカラーを取得
    float4 color = ParticleTexture.Sample(TextureSampler, uv);

    // UVのU成分にエンコードされた時間情報を取得
    float time = uv.x;

    // 距離によるフェード効果
    float2 center = float2(0.5, 0.5);
    float dist = length(uv - center); // 中心からの距離
    float fade = smoothstep(0.4, 0.5, dist);
    color.a *= (1.0 - fade);

    // グロー効果を表現する色調整
    float glowIntensity = 0.5 + 0.5 * sin(time * 2.0);
    color.rgb *= float3(0.5, 0.7, 1.0) * glowIntensity;

    return color;
}

// エントリーポイント
float4 main(float4 pos : SV_POSITION, float2 uv : TEXCOORD0) : SV_TARGET
{
    return SoulEffectPS(pos, uv);
}