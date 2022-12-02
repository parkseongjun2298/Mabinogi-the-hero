matrix			g_matWorld;			// ��� ���̺� : Ŭ���̾�Ʈ�� �ִ� ���� �����͸� ��ǥ�ϴ� �̸�
matrix			g_matView;
matrix			g_matProj;

texture			g_BaseTexture;		// �ؽ��ĸ� ��ǥ�ϴ� ��� ���̺�

sampler BaseSampler = sampler_state // ����ü�� �̸��� �����Ӱ� ���� �� �ִ� (= sampler_state) �ݵ�� ��������� ��
{
	texture = g_BaseTexture;
minfilter = linear;
magfilter = linear;
};

// HLSL�� ������ �Լ��� ���ڰ� �ڷ����� ��ȯ Ÿ�� �ڷ����� �޶��� �� �ִ�

struct	VS_IN
{
	vector			vPosition : POSITION;	// Semantic(fvf �Ӽ� �ɼ�), ���ÿ����� ������
	float2			vTexUV : TEXCOORD0;
};

struct	VS_OUT
{
	vector			vPosition : POSITION;	// Semantic(fvf �Ӽ� �ɼ�), ���� ��ȯ���� ���� ������
	float2			vTexUV : TEXCOORD0;
};

// ���ؽ� ���̵�

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

struct PS_IN	// �ȼ����̴��� ����ϱ� ���� ����ü �ڷ��������� ���� POSITION�̶�� SEMANTIC�� ����� �� ����.
{
	float2			vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector			vColor : COLOR0;
};

// �ȼ� ���̵�

PS_OUT	PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);	// 2���� �ؽ��ķκ��� uv��ǥ�� �ش��ϴ� ���� ������ �Լ�, ��ȯŸ���� vectorŸ��

	float fTemp = (Out.vColor.r + Out.vColor.g + Out.vColor.b) / 3.f;
	Out.vColor.a = fTemp;

	Out.vColor.a *= 0.8f;

	return Out;
}


PS_OUT	PS_TEMP(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);	// 2���� �ؽ��ķκ��� uv��ǥ�� �ش��ϴ� ���� ������ �Լ�, ��ȯŸ���� vectorŸ��



	return Out;
}

technique Default_Device
{
	// ����� ĸ��ȭ
	pass
{
	alphablendenable = true;
SRCBLEND = SRCALPHA;
destBlend = invsrcalpha;

vertexshader = compile vs_3_0 VS_MAIN();
pixelshader = compile ps_3_0 PS_MAIN();
}

};

