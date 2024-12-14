 // �m�C�Y�e�N�X�`��
Texture2D noiseTexture : register(t0);

// �T���v���[��STATE
SamplerState state : register(s0); 

cbuffer DissolveBuffer : register(b0)
{
    // �f�B�]���u�̐i�s�x
    float dissolveAmount; 
    // ���鉏�̐F
    float4 edgeColor;
    // ���̕�
    float edgeWidth;
    // �t�F�[�h�A�E�g�p�̓����x
    float transparency;
}

struct PS_INPUT
{
    // �e�N�X�`���̍��W
    float2 TexCoord : TEXCOORD0;
    // �@��
    float3 Normal : NORMAL;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    // �m�C�Y�e�N�X�`�����T���v�����O�A�f�B�]���u�i�s�x�Ɣ�r
    float noiseValue = noiseTexture.Sample(state, input.TexCoord).r;

   // �t���l�����ʂŃG�b�W�����点��
    float fresnel = pow(1.0 - saturate(dot(input.Normal, float3(0, 0, 1))), 2.0);

    // �f�B�]���u�i�s�x�Ɣ�r���A�m�C�Y�l���i�s�x��菬�����ꍇ�Ƀs�N�Z����j��
    if (noiseValue < dissolveAmount - edgeWidth)
    {
        discard;
    }
    
    // �����������炩�ɏ���
    float edgeFactor = smoothstep(dissolveAmount - edgeWidth, dissolveAmount, noiseValue);
    
    // ���鉏�̌v�Z
    float4 edgeGlow = edgeColor * edgeFactor * fresnel;

    // �f�B�]���u�����𔒐F�ɂ��A�G�b�W�����点��
    float4 baseColor = lerp(float4(1.0, 1.0, 1.0, 1.0), edgeGlow, edgeFactor);

    // �t�F�[�h�A�E�g�̂��߂ɓ����x��K�p
    baseColor.a *= transparency;

    return baseColor;
}