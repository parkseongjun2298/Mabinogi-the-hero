#include "Shader_Base.hpp"

vector	g_vLightDir = (vector)0.f;

vector	g_vLightDiffuse;
vector	g_vLightSpecular;
vector	g_vLightAmbient;

vector	g_vMtrlDiffuse;
vector	g_vMtrlSpecular;
vector	g_vMtrlAmbient;

vector	g_vCamPos;
float	g_fPower;
float	g_fUv;
texture			g_BaseTexture2;
sampler BaseSampler2 = sampler_state
{
	texture = g_BaseTexture2;
	minfilter = linear;
	magfilter = linear;
};



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

	// 난반사광
	float	fIntensity = saturate(dot(normalize(vWorldNormal), normalize(vWorldLightDir)));

	Out.vShade = fIntensity;
	Out.vShade.a = 1.f;

	// 정반사광
	vector	vReflect = reflect(normalize(g_vLightDir), normalize(vWorldNormal));

	vector	vWorldPos = mul(vector(In.vPosition.xyz, 1.f), g_matWorld);

	vector	vLook = g_vCamPos - vWorldPos;

	Out.vSpecular = pow((saturate(dot(normalize(vReflect), normalize(vLook)))), g_fPower);
	Out.vSpecular.a = 1.f;
	
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN	
{
	vector			vShade : COLOR0;
	vector			vSpecular : COLOR1;
	float2			vTexUV : TEXCOORD0;
};

struct PS_OUT	
{
	vector			vColor : COLOR0;
};


PS_OUT	PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vColor = tex2D(BaseSampler, In.vTexUV);	// 2차원 텍스쳐로부터 uv좌표에 해당하는 색을 얻어오는 함수, 반환타입은 vector타입

	//Out.vColor = (In.vShade * vColor) * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient);
	
	//// 스펙큘러

	Out.vColor = (In.vShade * vColor) * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient)
		+ In.vSpecular * (g_vLightSpecular * g_vMtrlSpecular);

	return Out;
}

PS_OUT		PS_ALPHA(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	return Out;

}
PS_OUT	PS_DISSOLVE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	
	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	vector vColor = tex2D(BaseSampler2, In.vTexUV);	// 2차원 텍스쳐로부터 uv좌표에 해당하는 색을 얻어오는 함수, 반환타입은 vector타입
	 
	Out.vColor = Out.vColor * vColor;
		

	float ClipAmount = vColor - g_fUv;

	clip(ClipAmount);

	
	return Out;
}



technique Default_Device
{
	// 기능의 캡슐화
	pass 
	{	
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}

	pass AlphtTest
	{
		alphatestenable = true;
		alphaFunc = Greater;
		alphaRef = 0xc0;
		cullmode = none;

		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_ALPHA();
	}
	pass DISSOLVE
	{
		alphablendenable = true;
	SRCBLEND = SRCALPHA;
	destBlend = invsrcalpha;

	vertexshader = compile vs_3_0 VS_MAIN();
	pixelshader = compile ps_3_0 PS_DISSOLVE();
	}
};

