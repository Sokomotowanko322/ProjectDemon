
struct VertexToPixel
{
    float4 svPos		: SV_POSITION;
    float4 diffuse		: COLOR0;
    float4 specular		: COLOR1;
    float2 uv			: TEXCOORD;
};

struct VertexToPixelLit
{
	float4 svPos		: SV_POSITION;	// ���W( �v���W�F�N�V������� )
	float2 uv			: TEXCOORD0;	// �e�N�X�`�����W
	float3 vwPos		: TEXCOORD1;	// ���W( �r���[���W )
	float3 normal       : TEXCOORD2;	// �@��( ���[���h���W )
	float4 diffuse      : COLOR0;		// �f�B�t���[�Y�J���[
	float3 worldPos     : TEXCOORD3;	// ���_���W( ���[���h���W )
};

struct VertexToPixelLitFog
{
	float4 svPos		: SV_POSITION;	// ���W( �v���W�F�N�V������� )
	float2 uv			: TEXCOORD0;	// �e�N�X�`�����W
	float3 vwPos		: TEXCOORD1;	// ���W( �r���[���W )
	float3 normal       : TEXCOORD2;	// �@��( ���[���h���W )
	float4 diffuse      : COLOR0;		// �f�B�t���[�Y�J���[
	float3 worldPos     : TEXCOORD3;	// ���_���W( ���[���h���W )
	float  fogFactor    : TEXCOORD4;	// �t�H�O�̋���(0.0:�t�H�O���Z���A1.0:�t�H�O������)
};

struct VertexToPixelShadow
{
	float4 svPos		: SV_POSITION;	// ���W( �v���W�F�N�V������� )
	float2 uv			: TEXCOORD0;	// �e�N�X�`�����W
	float4 vwPos		: TEXCOORD1;	// ���W( �r���[���W )
};
