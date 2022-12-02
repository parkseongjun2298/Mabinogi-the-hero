#include "stdafx.h"
#include "RuinBladeEffect.h"

#include "Export_Function.h"

CRuinBladeEffect::CRuinBladeEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_fAngleX(0.f),
	m_fDeadTime(0.f), m_fSaveAngleY(0.f)
{

}

CRuinBladeEffect::~CRuinBladeEffect(void)
{

}

HRESULT CRuinBladeEffect::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->m_vScale = { 15.f,15.f ,15.f };

	return S_OK;
}

_int CRuinBladeEffect::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return DEAD_OBJ;

	m_fDeadTime += 1 * fTimeDelta;

	if (m_fDeadTime >= 0.1f)
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

	m_pTransformCom->Set_Rotate(Engine::ROT_Y, m_fSaveAngleY);
	/*m_pTransformCom->Set_Rotate(Engine::ROT_X, 1.57129);
	m_fAngleX +=1 * fTimeDelta;
	m_fAngleX *= 0.2;
	m_pTransformCom->Rotation(Engine::ROT_Y, m_fAngleX);*/
	
	return 0;
}

void CRuinBladeEffect::Render_GameObject(void)
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

void CRuinBladeEffect::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CRuinBladeEffect::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	// buffer
	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	// texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_LongBladeEffect"));
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

HRESULT CRuinBladeEffect::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
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

CRuinBladeEffect* CRuinBladeEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev,_vec3 PlayerPos, float AngleY)
{
	CRuinBladeEffect*	pInstance = new	CRuinBladeEffect(pGraphicDev);
	
	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);
	_vec3 Temp = PlayerPos + D3DXVECTOR3{ 0.f,0.1f,0.f };
	pInstance->m_pTransformCom->Set_Pos(&Temp);
	pInstance->m_fSaveAngleY = AngleY;
	return pInstance;
}

