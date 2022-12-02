#include "stdafx.h"
#include "UI.h"

#include "Export_Function.h"
#include "Player.h"

CUI::CUI(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CUI::~CUI(void)
{

}

HRESULT CUI::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fX = 1700;
	m_fY = 200.f;

	m_fSizeX = WINCX/4;
	m_fSizeY = WINCY/4;

	return S_OK;
}

_int CUI::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return DEAD_OBJ;

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_Render_Group(Engine::RENDER_ALPHA, this);

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);


	for (auto& Player : Engine::CManagement::GetInstance()->Get_Obj(L"Player"))
	{
		m_pPlayer = dynamic_cast<CPlayer*>(Player);
	}
	if (m_pPlayer->m_iKillMonCnt >= 4 && !m_bCompleteUiCheck) 
	{

		m_pCompleteUi = CCompleteUI::Create(m_pGraphicDev);
		Engine::Add_GameObject(L"GameLogic", L"CompleteUi", m_pCompleteUi);
		m_bCompleteUiCheck = true;
	}


	if (m_pPlayer->m_bQuestClear)
		Set_bIsDead();
	

	return 0;
}

void CUI::Render_GameObject(void)
{
	
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);

	_uint	iPassMax = 0;

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	pEffect->Begin(&iPassMax, 0);

	pEffect->BeginPass(0);
	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();

	pEffect->End();

	
	//===========================================================
	//_matrix		matWorld, matView, matOldView, matOldProj;

	//D3DXMatrixIdentity(&matWorld);
	//D3DXMatrixIdentity(&matView);

	//m_pGraphicDev->GetTransform(D3DTS_VIEW, &matOldView);
	//m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matOldProj);

	//matWorld._11 = m_fSizeX;
	//matWorld._22 = m_fSizeY;
	//matWorld._33 = 1.f;
	//matWorld._41 = m_fX - WINCX * 0.5f;
	//matWorld._42 = -m_fY + WINCY * 0.5f;

	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	//m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	//m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	//

	//m_pTextureCom->Render_Texture();
	//m_pBufferCom->Render_Buffer();

	//

	////m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
	////m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);
}

void CUI::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CUI::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	// buffer
	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	// texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_UI"));
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
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Ui"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;
}

HRESULT CUI::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
{
	_matrix		matWorld, matView,matOldView, matOldProj;

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	//m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matOldProj);

	matWorld._11 = m_fSizeX;
	matWorld._22 = m_fSizeY;
	matWorld._33 = 1.f;
	matWorld._41 = m_fX - WINCX * 0.5f;
	matWorld._42 = -m_fY + WINCY * 0.5f;

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &m_matProj);

	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture");
	
	return S_OK;
}

CUI* CUI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI*	pInstance = new	CUI(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

