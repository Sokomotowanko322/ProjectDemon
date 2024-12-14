SamplerState sample : register(s0);
Texture2D<float4> tex : register(t0);
Texture2D<float4> dissolveTex : register(t3);

// ���_�V�F�[�_�[�̏o��
struct PS_INPUT
{
    float4 Diffuse : COLOR0;            // �f�B�t���[�Y�J���[
    float4 Specular : COLOR1;           // �X�y�L�����J���[
    float4 TexCoords0_1 : TEXCOORD0;    // xy:�e�N�X�`�����W zw:�T�u�e�N�X�`�����W
    float3 WPosition : POSITION0;       // �ϊ��O�̃��[���h���W
    float3 VPosition : TEXCOORD1;       // ���W( �r���[��� )
    float3 VNormal : TEXCOORD2;         // �@��( �r���[��� )
#ifdef BUMPMAP
	float3 VTan            : TEXCOORD3 ;    // �ڐ�( �r���[��� )
	float3 VBin            : TEXCOORD4 ;    // �]�@��( �r���[��� )
#endif // BUMPMAP
    float1 Fog : TEXCOORD5;             // �t�H�O�p�����[�^( x )

#if SHADOWMAP
	float3 ShadowMap0Pos   : TEXCOORD6 ;	// �V���h�E�}�b�v�O�̃��C�g���W( x, y, z )
	float3 ShadowMap1Pos   : TEXCOORD7 ;	// �V���h�E�}�b�v�P�̃��C�g���W( x, y, z )
	float3 ShadowMap2Pos   : TEXCOORD8 ;	// �V���h�E�}�b�v�Q�̃��C�g���W( x, y, z )
#endif // SHADOWMAP

    float4 Position : SV_POSITION; // ���W( �v���W�F�N�V������� )
};

struct PS_OUTPUT
{
    float4 color : SV_Target0;      //�F
    float4 depth : SV_Target1;      //�[�x�l
    float4 normal: SV_Target2;      //�@�����
};

cbuffer UseData : register(b4)
{
    float dissolveHeight;           //�f�B�]���u�̍���
    float bbMinY;                   //�o�E���f�B���O�{�b�N�X(���̂��͂񂾕����̈�)�̍ŏ�Y��
    float bbMaxY;                   //�o�E���f�B���O�{�b�N�X(���̂��͂񂾕����̈�)�̍ő�Y��
    float dummy;                    //�_�~�[
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
    
    //�͈͂�0�`1�Ɍ���A���]
    float shineRate = 1.0 - saturate(dissolveHeight + dissolveNoise - Yrate);
    
    float3 shineColor = float3(0.5, 0.5, 1.0) * pow(shineRate, 50.0);
    
    float4 texCol = tex.Sample(sample, input.TexCoords0_1.xy);
    float3 eye = float3(0, 50, -150);
    float3 V = normalize(input.Position.xyz - eye);//�����x�N�g���Z�o
    
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

    float dep = (input.VPosition.z - 1.0) / (500.0 - 1.0); //���Ƃ�z�l����`��0.0�`1.0�ɕϊ�
    
    outData.depth = float4(dep, dep, dep, 1);
    outData.normal = float4((input.VNormal + 1.0) * 0.5, 1.0);
    return outData;
}