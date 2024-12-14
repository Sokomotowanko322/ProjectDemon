Texture2D<float4> rt : register(t0);
Texture2D<float4> blurRT : register(t1);
Texture2D<float> depthRT : register(t2);
Texture2D<float4> normalRT : register(t3);
SamplerState smp : register(s0);


cbuffer UserData : register(b4)
{
    float dissolveHeight; //
    float bbMinY; //バウンディングボックスの最小Y
    float bbMaxY; //バウンディングボックスの最大Y
    float dummy;
    float2 clickedUV;
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 dif : COLOR0;
    //float4 spec : COLOR1;
    float2 uv : TEXCOORD0;
};

struct PSOutput
{
    float4 col : SV_Target0;
    float4 outline : SV_Target1;
};


PSOutput main(PS_INPUT input)
{
    uint2 wh;
    uint num;
    normalRT.GetDimensions(0, wh.x, wh.y, num); //テクスチャの解像度を取得する
    
    float2 diffUV = 1.0 / float2(wh.x, wh.y); //となりのピクセルのUV差を計算
    float3 norm = normalRT.Sample(smp, input.uv).rgb; //真ん中
    
    //周囲(四近傍)の法線情報を取得する
    float3 normL = normalRT.Sample(smp, input.uv + float2(-diffUV.x, 0)).rgb; //左
    float3 normR = normalRT.Sample(smp, input.uv + float2(+diffUV.x, 0)).rgb; //右
    float3 normU = normalRT.Sample(smp, input.uv + float2(0, -diffUV.y)).rgb; //上
    float3 normD = normalRT.Sample(smp, input.uv + float2(0, +diffUV.y)).rgb; //下
    
    float3 edgeN = saturate(abs(norm * 4 - normL - normR - normU - normD));
    
    
    float outlineN = 1 - saturate(edgeN.r + edgeN.g + edgeN.b);
    
    
    float oln = outlineN;

    //return float4(outline,outline,outline, 1.0);
    
    float targetDep = depthRT.Sample(smp, clickedUV).r;
    
    float dep = depthRT.Sample(smp, input.uv).r;
    float depL = depthRT.Sample(smp, input.uv + float2(-diffUV.x, 0)).r; //左
    float depR = depthRT.Sample(smp, input.uv + float2(+diffUV.x, 0)).r; //右
    float depU = depthRT.Sample(smp, input.uv + float2(0, -diffUV.y)).r; //上
    float depD = depthRT.Sample(smp, input.uv + float2(0, +diffUV.y)).r; //下
    float outlineD = saturate(abs(dep * 4 - depL - depR - depU - depD));
    outlineD = 1.0 - outlineD;
    PSOutput o;
    float old = step(0.99, pow(outlineD, 10)) * oln;
    //if (distance(dep, 0.3) < 0.2)
    //{
    //    old = 1 - old;
    //}
    //if (outlineD >= 0.5 && oln <= 0.01)
    //{
    //    o.col = float4(1, 0, 0, 1);
    //}
    //else
    //{
    //    o.col = float4(old, old, old, 1);
    //}
    o.col = float4(old, old, old, 1);
    o.outline = float4(old, old, old, 1);
    //o.col = float4(edgeN, 1);

    //o.outline = float4(edgeN, 1);
    //return o;
    
    float diffDep = distance(dep, targetDep);
    diffDep = pow(diffDep, 0.3);

    //被写界深度
    float4 col = lerp(rt.Sample(smp, input.uv), blurRT.Sample(smp, input.uv), 0);
    float outline = outlineD * outlineN;
    PSOutput output;
    output.col = rt.Sample(smp, input.uv); //float4(col.rgb * 0.75, col.a);
    output.outline = float4(edgeN, 1);
    return output;
    //return float4(col.rgb * float3(1, outline, outline), col.a);
    
    
    //空気遠近法
    //return lerp(col, float4(0.5, 0.5, 0.5, 1), pow(dep, 500));

}