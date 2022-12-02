matrix			g_matWorld;			// 상수 테이블 : 클라이언트에 있는 실제 데이터를 대표하는 이름
matrix			g_matView;
matrix			g_matProj;

texture			g_BaseTexture;		// 텍스쳐를 대표하는 상수 테이블

sampler BaseSampler = sampler_state // 구조체의 이름은 자유롭게 지을 수 있다 (= sampler_state) 반드시 기입해줘야 함
{
	texture = g_BaseTexture;
minfilter = linear;
magfilter = linear;
};

// HLSL의 장점은 함수의 인자값 자료형과 반환 타입 자료형이 달라질 수 있다

struct	VS_IN
{
	vector			vPosition : POSITION;	// Semantic(fvf 속성 옵션), 로컬영역의 포지션
	float2			vTexUV : TEXCOORD0;
};

struct	VS_OUT
{
	vector			vPosition : POSITION;	// Semantic(fvf 속성 옵션), 투영 변환까지 끝난 포지션
	float2			vTexUV : TEXCOORD0;
};

// 버텍스 쉐이딩

VS_OUT	VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN	// 픽셀쉐이더를 사용하기 위한 구조체 자료형에서는 절대 POSITION이라는 SEMANTIC을 사용할 수 없다.
{
	float2			vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector			vColor : COLOR0;
};

// 픽셀 쉐이딩

PS_OUT	PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);	// 2차원 텍스쳐로부터 uv좌표에 해당하는 색을 얻어오는 함수, 반환타입은 vector타입

	float fTemp = (Out.vColor.r + Out.vColor.g + Out.vColor.b) / 3.f;
	Out.vColor.a = fTemp;

	Out.vColor.a *= 0.8f;

	return Out;
}


PS_OUT	PS_TEMP(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);	// 2차원 텍스쳐로부터 uv좌표에 해당하는 색을 얻어오는 함수, 반환타입은 vector타입



	return Out;
}

technique Default_Device
{
	// 기능의 캡슐화
	pass
{
	alphablendenable = true;
SRCBLEND = SRCALPHA;
destBlend = invsrcalpha;

vertexshader = compile vs_3_0 VS_MAIN();
pixelshader = compile ps_3_0 PS_MAIN();
}

};

