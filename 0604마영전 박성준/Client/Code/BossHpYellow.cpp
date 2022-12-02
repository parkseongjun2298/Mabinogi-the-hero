#include "stdafx.h"
#include "BossHpYellow.h"

#include "Export_Function.h"
#include "BossHp3.h"
#include "BossHp2.h"
#include "BossHp1.h"
#include"Boss.h"
CBossHpYellow::CBossHpYellow(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_fBossHpNum(0.f)
{

}

CBossHpYellow::~CBossHpYellow(void)
{

}

HRESULT CBossHpYellow::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fX = WINCX / 4;
	m_fY = WINCY / 14;

	m_fSizeX = 1300;
	m_fSizeY = 100;

	m_fBossHpNum = 3.f;
	return S_OK;
}

_int CBossHpYellow::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
	{
		m_pBossHp1->Set_bIsDead();
		m_pBoss->m_fHpCount=1;
		return DEAD_OBJ;
	}

	for (auto& Boss : Engine::CManagement::GetInstance()->Get_Obj(L"Boss"))
	{
		m_pBoss = dynamic_cast<CBoss*>(Boss);
	}
	//m_fSizeX -= 1;

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_Render_Group(Engine::RENDER_UI, this);

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);




	//피통 개수 여러개 만들고 m_fSizeX<=0.f되면 그 개수 줄어들고 그 피통개수가 0이고  m_fSizeX<=0.f 이면 빨간색으로

	if (m_fBossHpNum == 0 && m_fBossHpNum <= 0.f)
	{
		Set_bIsDead();
	}
	if (m_fSizeX <= 0.f)
	{
		m_fBossHpNum -= 1.f;
		m_fSizeX = 1299;
	}
	if (m_fBossHpNum == 3 && !m_bCheckHp3 && m_fSizeX<=1299)
	{
		m_pBossHp3 = CBossHp3::Create(m_pGraphicDev);
		Engine::Add_GameObject(L"GameLogic", L"BossHp3", m_pBossHp3);

		m_bCheckHp3 = true;
	}
	if (m_fBossHpNum == 2 && !m_bCheckHp2)
	{
		m_pBossHp3->Set_bIsDead();


		m_pBossHp2 = CBossHp2::Create(m_pGraphicDev);
		Engine::Add_GameObject(L"GameLogic", L"BossHp2", m_pBossHp2);

		m_bCheckHp2 = true;
	}

	if (m_fBossHpNum == 1 && !m_bCheckHp1)
	{
		m_pBossHp2->Set_bIsDead();

		m_pBossHp1 = CBossHp1::Create(m_pGraphicDev);
		Engine::Add_GameObject(L"GameLogic", L"BossHp1", m_pBossHp1);

		m_bCheckHp1 = true;
	}



	//cout << "옐로 넘버:" << m_fBossHpNum << endl;
	return 0;
}

void CBossHpYellow::Render_GameObject(void)
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

void CBossHpYellow::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CBossHpYellow::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	// buffer
	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTexLeft*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTexLeft"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	// texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_BossHpBar_Yellow"));
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

CBossHpYellow* CBossHpYellow::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBossHpYellow*	pInstance = new	CBossHpYellow(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

