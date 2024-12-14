// VS/PS����
#include "../Common/VertexToPixelHeader.hlsli"

// IN
#define PS_INPUT VertexToPixelLitFog

// PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

// �萔�o�b�t�@�F�X���b�g4�Ԗ�
cbuffer cbParam : register(b4)
{
	float4 g_diff_color;
	float4 g_ambient_color;
	float3 g_light_dir;
	float  dummy;
}

float4 main(PS_INPUT PSInput) : SV_TARGET0
{

	float4 color;

	// �e�N�X�`���[�̐F���擾
	color = diffuseMapTexture.Sample(diffuseMapSampler, PSInput.uv);
	if (color.a < 0.01f)
	{
		discard;
	}
	
	float lightDot = dot(PSInput.normal, -g_light_dir);
	float3 rgb = (color.rgb * (lightDot  * g_diff_color.rgb)) + g_ambient_color.rgb;
	
	// �t�H�O
    rgb *= PSInput.fogFactor;
	
	return float4(rgb, color.a);

}
