 // ノイズテクスチャ
Texture2D noiseTexture : register(t0);

// サンプラーのSTATE
SamplerState state : register(s0); 

cbuffer DissolveBuffer : register(b0)
{
    // ディゾルブの進行度
    float dissolveAmount; 
    // 光る縁の色
    float4 edgeColor;
    // 縁の幅
    float edgeWidth;
    // フェードアウト用の透明度
    float transparency;
}

struct PS_INPUT
{
    // テクスチャの座標
    float2 TexCoord : TEXCOORD0;
    // 法線
    float3 Normal : NORMAL;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    // ノイズテクスチャをサンプリング、ディゾルブ進行度と比較
    float noiseValue = noiseTexture.Sample(state, input.TexCoord).r;

   // フレネル効果でエッジを光らせる
    float fresnel = pow(1.0 - saturate(dot(input.Normal, float3(0, 0, 1))), 2.0);

    // ディゾルブ進行度と比較し、ノイズ値が進行度より小さい場合にピクセルを破棄
    if (noiseValue < dissolveAmount - edgeWidth)
    {
        discard;
    }
    
    // 縁部分を滑らかに処理
    float edgeFactor = smoothstep(dissolveAmount - edgeWidth, dissolveAmount, noiseValue);
    
    // 光る縁の計算
    float4 edgeGlow = edgeColor * edgeFactor * fresnel;

    // ディゾルブ部分を白色にし、エッジを光らせる
    float4 baseColor = lerp(float4(1.0, 1.0, 1.0, 1.0), edgeGlow, edgeFactor);

    // フェードアウトのために透明度を適用
    baseColor.a *= transparency;

    return baseColor;
}