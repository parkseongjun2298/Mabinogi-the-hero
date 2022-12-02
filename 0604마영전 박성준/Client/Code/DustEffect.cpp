#include "stdafx.h"
#include "DustEffect.h"

#include "Export_Function.h"

CDustEffect::CDustEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev),
	m_fDeadTime(0.f)
{

}

CDustEffect::~CDustEffect(void)
{

}

HRESULT CDustEffect::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vScale = { 0.4f,0.4f ,0.4f };
	

	return S_OK;
}

_int CDustEffect::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return DEAD_OBJ;

	m_fDeadTime += 1 * fTimeDelta;

	if (m_fDeadTime >= 0.3f)
		Set_bIsDead();

	m_fFrame += 1.f * fTimeDelta;   //이숫자가 텍스쳐 몇개를 돌지 하는 숫자임 ;

	if (1.f < m_fFrame)
		m_fFrame = 0.f;


	Engine::CGameObject::Update_GameObject(fTimeDelta);

	_matrix			matWorld, matView, matBill;
	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	//D3DXMatrixIdentity(&matBill);
	//
	//// y축 회전값을 뺀 빌보드 적용
	//matBill._11 = matView._11;//Right 
	//matBill._13 = matView._13;
	//matBill._31 = matView._31;//Look
	//matBill._33 = matView._33;

	////matBill._22=
	//D3DXMatrixInverse(&matBill, 0, &matBill);
	
	// 기억해둬야 할 것! 지금의 코드는 문제가 없다
	// 그러나 메쉬에 적용할 경우 빌보드의 문제가 생길 수 있다.
	// 월드 = 크기 * 회전 * 위치
	// 빌보드를 적용하겠다는 것 크기 * (회전-1 * 회전) * 위치
	// 현재 이펙트의 스케일링 값이 1인 상태여서 곱셈 순서에 상관없이 회전 값이 빠지게 됨
	// 하지만 일반적으로 메쉬의 크기 값은 1/100로 줄여서 표현 중

	m_pTransformCom->m_matWorld = /*matBill **/ matWorld;
	
	_vec3 vTemp;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vTemp);
	Engine::CGameObject::Compute_ViewZ(&vTemp);

	m_pRendererCom->Add_Render_Group(Engine::RENDER_ALPHA, this);


	return 0;
}

void CDustEffect::Render_GameObject(void)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);

	_uint	iPassMax = 0;

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	pEffect->Begin(&iPassMax, 0);

	pEffect->BeginPass(2);
	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();

	pEffect->End();
}

void CDustEffect::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CDustEffect::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	// buffer
	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	// texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Dust"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	// transform
	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// renderer
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Sample"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);
	return S_OK;
	
}
HRESULT CDustEffect::SetUp_ConstantTable(LPD3DXEFFECT & pDustEffect)
{
	_matrix		matWorld, matView, matProj;
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pDustEffect->SetMatrix("g_matWorld", &matWorld);
	pDustEffect->SetMatrix("g_matView", &matView);
	pDustEffect->SetMatrix("g_matProj", &matProj);

	m_pTextureCom->Set_Texture(pDustEffect, "g_BaseTexture");

	return S_OK;
}
CDustEffect* CDustEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev,_vec3 PlayerPos)
{
	CDustEffect*	pInstance = new	CDustEffect(pGraphicDev);
	
	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);
	_vec3 Temp = PlayerPos + D3DXVECTOR3{ _float(rand() % 3 - 1),0.f,_float(rand() % 3 - 1) };
	pInstance->m_pTransformCom->Set_Pos(&Temp);
	
	return pInstance;
}

