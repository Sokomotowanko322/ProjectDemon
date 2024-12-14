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
	float  dummy1;

	float  g_time;
	float  g_offset;
	float  dummy2;
	float  dummy3;

}

// HSV����RGB�ւ̕ϊ��֐�
float3 hsv2rgb(float h, float s, float v)
{
	return ((
		clamp(
			abs(
				frac(
					h + float3(0.0f, 2.0f, 1.0f) / 3.0f) * 6.0f - 3.0f) - 1.0f, 0.0f, 1.0f) - 1.0f) * s + 1.0f) * v;
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
	float3 rgb = (color.rgb * (lightDot * g_diff_color.rgb)) + g_ambient_color.rgb;
	
	float3 pos = PSInput.worldPos;
	float time = g_time + g_offset;

	// �c�̑傫���Ǝ��Ԃ�-1.0�`1.0�̒l�����
	float area = sin(pos.y * 0.008f - time * 1.0f);

	// �c�ɓ���͈͂����(1 or 0)
	float isArea = step(0.996f, area * area);

	// ���悹
	//-------------------------------------------------
	// �F�����v�Z�i���� + �c�����̍��W�Ɋ�Â��ĕω��j
	float hue = frac(pos.y * 0.05f);

	// HSV����RGB�ւ̕ϊ�
	// �ʓx1.0�A�P�x1.0�ŐF���擾
	rgb += hsv2rgb(hue, 1.0f, 1.0f) * isArea;
	//-------------------------------------------------

	// �t�H�O
    rgb *= PSInput.fogFactor;
	
	return float4(rgb, color.a);

}
