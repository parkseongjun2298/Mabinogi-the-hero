#include "stdafx.h"
#include "LordOfManaBlueFireEffect.h"
#include "Export_Function.h"

CLordOfManaBlueFireEffect::CLordOfManaBlueFireEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_fAngleX(0.f),
	m_fDeadTime(0.f)
{

}

CLordOfManaBlueFireEffect::~CLordOfManaBlueFireEffect(void)
{

}

HRESULT CLordOfManaBlueFireEffect::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->m_vScale = { 1.f,1.f ,1.f };

	return S_OK;
}

_int CLordOfManaBlueFireEffect::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return DEAD_OBJ;

	m_fDeadTime += 1 * fTimeDelta;

	if (m_fDeadTime >= 2.f)
		Set_bIsDead();

	m_fFrame += 1.f * fTimeDelta;   //이숫자가 텍스쳐 몇개를 돌지 하는 숫자임 ;

	if (1.f < m_fFrame)
		m_fFrame = 0.f;


	Engine::CGameObject::Update_GameObject(fTimeDelta);

	_matrix			matWorld, matView, matBill;
	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	

	m_pTransformCom->m_matWorld =  matWorld;
	
	_vec3 vTemp;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vTemp);
	Engine::CGameObject::Compute_ViewZ(&vTemp);

	m_pRendererCom->Add_Render_Group(Engine::RENDER_ALPHA, this);


	

	//m_pTransformCom->Set_Rotate(Engine::ROT_X, 1.57129);
	//m_fAngleX +=1 * fTimeDelta;
	//m_fAngleX *= 0.2;
	//m_pTransformCom->Rotation(Engine::ROT_Y, m_fAngleX);
	
	return 0;
}

void CLordOfManaBlueFireEffect::Render_GameObject(void)
{
	
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_matWorld);

	//m_pTextureCom->Render_Texture(_ulong(m_fFrame));
	//m_pBufferCom->Render_Buffer();
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

void CLordOfManaBlueFireEffect::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CLordOfManaBlueFireEffect::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	// buffer
	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	// texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_BlueFire"));
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

HRESULT CLordOfManaBlueFireEffect::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
{
	_matrix		matWorld, matView, matProj;
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	


	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture");

	return S_OK;
}

CLordOfManaBlueFireEffect* CLordOfManaBlueFireEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev,_vec3 PlayerPos)
{
	CLordOfManaBlueFireEffect*	pInstance = new	CLordOfManaBlueFireEffect(pGraphicDev);
	
	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);
	_vec3 Temp = PlayerPos + D3DXVECTOR3{ _float(rand() % 6 -3),_float(rand() % 2 + 1),_float(rand() % 6 - 3) };
	pInstance->m_pTransformCom->Set_Pos(&Temp);

	return pInstance;
}

