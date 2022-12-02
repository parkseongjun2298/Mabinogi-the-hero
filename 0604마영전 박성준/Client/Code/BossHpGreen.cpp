#include "stdafx.h"
#include "BossHpGreen.h"

#include "Export_Function.h"
#include "BossHp4.h"
#include"Boss.h"
CBossHpGreen::CBossHpGreen(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CBossHpGreen::~CBossHpGreen(void)
{

}

HRESULT CBossHpGreen::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fX = WINCX / 4;
	m_fY = WINCY/14;

	m_fSizeX = 1300;
	m_fSizeY = 100;

	

	return S_OK;
}

_int CBossHpGreen::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
	{
		m_pBossHp4->Set_bIsDead();
		m_pBoss->m_fHpCount = 2;
		return DEAD_OBJ;
	}
	for (auto& Boss : Engine::CManagement::GetInstance()->Get_Obj(L"Boss"))
	{
		m_pBoss = dynamic_cast<CBoss*>(Boss);
	}
	

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_Render_Group(Engine::RENDER_UI, this);

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);


	if (m_fSizeX <= 0.f)
	{
		Set_bIsDead();
	}

	
	if (!m_bCheckHp4)
	{
		m_pBossHp4 = CBossHp4::Create(m_pGraphicDev);
		Engine::Add_GameObject(L"GameLogic", L"BossHp4", m_pBossHp4);

		m_bCheckHp4 = true;
	}

	//cout << "그린 사이즈:" << m_fSizeX << endl;

	return 0;
}

void CBossHpGreen::Render_GameObject(void)
{
	_matrix		matWorld, matView, matOldView, matOldProj;

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matOldProj);

	matWorld._11 = m_fSizeX;
	matWorld._22 = m_fSizeY;
	matWorld._33 = 1.f;
	matWorld._41 = m_fX - WINCX * 0.5f;
	matWorld._42 = -m_fY + WINCY * 0.5f;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);
}

void CBossHpGreen::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CBossHpGreen::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	// buffer
	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTexLeft*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTexLeft"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	// texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_BossHpBar_Green"));
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


	return S_OK;
}

CBossHpGreen* CBossHpGreen::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBossHpGreen*	pInstance = new	CBossHpGreen(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

