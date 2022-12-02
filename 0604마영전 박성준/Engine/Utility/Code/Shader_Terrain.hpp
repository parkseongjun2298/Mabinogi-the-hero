#include "Shader_Base.hpp"


texture			g_BaseTexture1;

sampler BaseSampler1 = sampler_state
{
	texture = g_BaseTexture1;
	minfilter = linear;
	magfilter = linear;
};

texture			g_FilterTexture;

sampler FilterSampler = sampler_state
{
	texture = g_FilterTexture;
	minfilter = linear;
	magfilter = linear;
};

texture			g_AuraTexture;

sampler AuraSampler = sampler_state
{
	texture = g_AuraTexture;
	minfilter = linear;
	magfilter = linear;
};

vector	g_vAuraPos;
float	g_fRange;


vector	g_vLightDir = (vector)0.f;

vector	g_vLightDiffuse;
vector	g_vLightSpecular;
vector	g_vLightAmbient;

vector	g_vMtrlDiffuse;
vector	g_vMtrlSpecular;
vector	g_vMtrlAmbient;

vector	g_vCamPos;

float	g_fDetail;
float	g_fPower;

struct	VS_IN 
{
	vector			vPosition	: POSITION;	
	vector			vNormal		: NORMAL;
	float2			vTexUV		: TEXCOORD0;
};

struct	VS_OUT
{
	vector			vPosition	: POSITION;	
	vector			vShade		: COLOR0;
	vector			vSpecular	: COLOR1;
	float2			vTexUV		: TEXCOORD0;
	vector			vWorldPos	: TEXCOORD1;
};


VS_OUT	VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV  = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);

	vector	vWorldNormal = mul(vector(In.vNormal.xyz, 0.f), g_matWorld);
	vector	vWorldLightDir = g_vLightDir * -1.f;

	// ���ݻ籤
	float	fIntensity = saturate(dot(normalize(vWorldNormal), normalize(vWorldLightDir)));
	//float	fIntensity = max(dot(normalize(vWorldNormal), normalize(vWorldLightDir)), 0.f);

	Out.vShade = fIntensity;
	Out.vShade.a = 1.f;

	// ���ݻ籤
	vector	vReflect = reflect(normalize(g_vLightDir), normalize(vWorldNormal));

	vector	vWorldPos = mul(vector(In.vPosition.xyz, 1.f), g_matWorld);

	vector	vLook = g_vCamPos - vWorldPos;

	Out.vSpecular = pow((saturate(dot(normalize(vReflect), normalize(vLook)))), g_fPower);
	Out.vSpecular.a = 1.f;
	
	Out.vTexUV = In.vTexUV;
	Out.vWorldPos = vWorldPos;

	return Out;
}

struct PS_IN	
{
	vector			vShade		: COLOR0;
	vector			vSpecular	: COLOR1;
	float2			vTexUV		: TEXCOORD0;
	vector			vWorldPos	: TEXCOORD1;
};

struct PS_OUT	
{
	vector			vColor : COLOR0;
};


PS_OUT	PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vColor0 = tex2D(BaseSampler, In.vTexUV * g_fDetail);	// 2���� �ؽ��ķκ��� uv��ǥ�� �ش��ϴ� ���� ������ �Լ�, ��ȯŸ���� vectorŸ��
	vector vColor1 = tex2D(BaseSampler1, In.vTexUV);	// 2���� �ؽ��ķκ��� uv��ǥ�� �ش��ϴ� ���� ������ �Լ�, ��ȯŸ���� vectorŸ��

	vector	vFilter = tex2D(FilterSampler, In.vTexUV);

	Out.vColor = vColor0 * vFilter + vColor1 * (1.f - vFilter);


	//Out.vColor = (In.vShade * vColor) * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient);
	
	//Out.vColor = (vColor) * (g_vLightDiffuse * g_vMtrlDiffuse) + (In.vShade * (g_vLightAmbient * g_vMtrlAmbient));

	//Out.vColor = (vColor) * (g_vLightDiffuse * g_vMtrlDiffuse) * (In.vShade + (g_vLightAmbient * g_vMtrlAmbient));

	// ����ŧ��

	Out.vColor = (In.vShade * Out.vColor) * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient)
		+ In.vSpecular * (g_vLightSpecular * g_vMtrlSpecular);


	vector		vAuraColor = (vector)0.f;
						
	// worldpos�� ���� �ؽ�ó�� �ȼ� ��
	// auraPos�� �÷��̾��� ��ġ ��

	if(g_vAuraPos.x - g_fRange < In.vWorldPos.x &&
		g_vAuraPos.x + g_fRange > In.vWorldPos.x &&
		g_vAuraPos.z + g_fRange > In.vWorldPos.z &&
		g_vAuraPos.z - g_fRange < In.vWorldPos.z)
	{
		float2 vTexUV;

		vTexUV.x = (In.vWorldPos.x - (g_vAuraPos.x - g_fRange)) / (g_fRange * 2.f);
		vTexUV.y = ((g_vAuraPos.z + g_fRange) - In.vWorldPos.z) / (g_fRange * 2.f);

		vAuraColor = tex2D(AuraSampler, vTexUV);
	}

	Out.vColor += vAuraColor;
	
	return Out;
}


technique Default_Device
{
	// ����� ĸ��ȭ
	pass 
	{	
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}

};

