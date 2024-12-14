SamplerState sample : register(s0);
Texture2D<float4> tex : register(t0);
Texture2D<float4> dissolveTex : register(t3);

// 頂点シェーダーの出力
struct PS_INPUT
{
    float4 Diffuse : COLOR0;            // ディフューズカラー
    float4 Specular : COLOR1;           // スペキュラカラー
    float4 TexCoords0_1 : TEXCOORD0;    // xy:テクスチャ座標 zw:サブテクスチャ座標
    float3 WPosition : POSITION0;       // 変換前のワールド座標
    float3 VPosition : TEXCOORD1;       // 座標( ビュー空間 )
    float3 VNormal : TEXCOORD2;         // 法線( ビュー空間 )
#ifdef BUMPMAP
	float3 VTan            : TEXCOORD3 ;    // 接線( ビュー空間 )
	float3 VBin            : TEXCOORD4 ;    // 従法線( ビュー空間 )
#endif // BUMPMAP
    float1 Fog : TEXCOORD5;             // フォグパラメータ( x )

#if SHADOWMAP
	float3 ShadowMap0Pos   : TEXCOORD6 ;	// シャドウマップ０のライト座標( x, y, z )
	float3 ShadowMap1Pos   : TEXCOORD7 ;	// シャドウマップ１のライト座標( x, y, z )
	float3 ShadowMap2Pos   : TEXCOORD8 ;	// シャドウマップ２のライト座標( x, y, z )
#endif // SHADOWMAP

    float4 Position : SV_POSITION; // 座標( プロジェクション空間 )
};

struct PS_OUTPUT
{
    float4 color : SV_Target0;      //色
    float4 depth : SV_Target1;      //深度値
    float4 normal: SV_Target2;      //法線情報
};

cbuffer UseData : register(b4)
{
    float dissolveHeight;           //ディゾルブの高さ
    float bbMinY;                   //バウンディングボックス(物体を囲んだ部分領域)の最小Y軸
    float bbMaxY;                   //バウンディングボックス(物体を囲んだ部分領域)の最大Y軸
    float dummy;                    //ダミー
};

cbuffer NearFar : register(b5)
{
    float near;
    float far;
}

PS_OUTPUT main(PS_INPUT input) 
{
    float dissolveNoise = dissolveTex.Sample(sample, input.TexCoords0_1.xy).r;
    float range = bbMaxY - bbMinY;
    
    float dissolveRate = saturate(dissolveHeight / range);
    dissolveNoise *= 0.05;

    float Yrate = (input.WPosition.y - bbMinY) / range; 
    if (dissolveHeight < Yrate + dissolveNoise)
    {
        discard;
    }
  
    float edge = pow(1.0 - saturate(dissolveHeight - (Yrate + dissolveNoise)), 50.0);
    
    //範囲を0～1に限定、反転
    float shineRate = 1.0 - saturate(dissolveHeight + dissolveNoise - Yrate);
    
    float3 shineColor = float3(0.5, 0.5, 1.0) * pow(shineRate, 50.0);
    
    float4 texCol = tex.Sample(sample, input.TexCoords0_1.xy);
    float3 eye = float3(0, 50, -150);
    float3 V = normalize(input.Position.xyz - eye);//視線ベクトル算出
    
    float3 light = normalize(float3(1, -1, 1));
    float3 R = reflect(light, input.VNormal);
    
    float spec = pow(saturate(dot(R, -V)), 20.0);
    
    float ambient = 0.25;
    float brightness = max(saturate(dot(input.VNormal, -light)), ambient);
    
    float rim = 1.0 - saturate(dot(-V, input.VNormal));
    rim = pow(rim, 50);
    float3 rimColor = float3(1.0, 0.8, 0.8) * rim;

    PS_OUTPUT outData;
    outData.color = float4(input.Diffuse.rgb * texCol.rgb * brightness + spec + float3(0.5, 0.5, 1.0) * edge, texCol.a);

    float dep = (input.VPosition.z - 1.0) / (500.0 - 1.0); //もとのz値を線形に0.0～1.0に変換
    
    outData.depth = float4(dep, dep, dep, 1);
    outData.normal = float4((input.VNormal + 1.0) * 0.5, 1.0);
    return outData;
}