#include "stdafx.h"
#include "BossHpRed.h"

#include "Export_Function.h"
#include "Boss.h"
#include "BossHp0.h"
CBossHpRed::CBossHpRed(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CBossHpRed::~CBossHpRed(void)
{

}

HRESULT CBossHpRed::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fX = WINCX / 4;
	m_fY = WINCY / 14;

	m_fSizeX = 1300;
	m_fSizeY = 100;

	return S_OK;
}

_int CBossHpRed::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
	{
		m_pBossHp0->Set_bIsDead();
		m_pBoss->m_fHpCount =0;
		return DEAD_OBJ;
	}

	//m_fSizeX -= 1;

	if (m_fSizeX <= 0.f)
		m_fSizeX = 0.f;

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_Render_Group(Engine::RENDER_UI, this);

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);


	for (auto& Boss : Engine::CManagement::GetInstance()->Get_Obj(L"Boss"))
	{
		m_pBoss = dynamic_cast<CBoss*>(Boss);
	}

	if (m_pBoss->m_bCheckDead)
		Set_bIsDead();


	if (!m_bCheckHp0 && m_fSizeX <= 1299)
	{
		m_pBossHp0 = CBossHp0::Create(m_pGraphicDev);
		Engine::Add_GameObject(L"GameLogic", L"BossHp0", m_pBossHp0);

		m_bCheckHp0 = true;
	}

	//cout << "남은체력:"<<m_fSizeX << endl;

	return 0;
}

void CBossHpRed::Render_GameObject(void)
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

void CBossHpRed::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CBossHpRed::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	// buffer
	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTexLeft*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTexLeft"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	// texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_BossHpBar_Red"));
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

CBossHpRed* CBossHpRed::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBossHpRed*	pInstance = new	CBossHpRed(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

