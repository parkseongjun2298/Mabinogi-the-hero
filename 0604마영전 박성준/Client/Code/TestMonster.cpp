#include "stdafx.h"
#include "TestMonster.h"

#include "Export_Function.h"

CTestMonster::CTestMonster(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CTestMonster::~CTestMonster(void)
{

}

HRESULT CTestMonster::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	
	return S_OK;
}

_int CTestMonster::Update_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	Engine::CTransform*		pPlayerTransCom = dynamic_cast<Engine::CTransform*>
											(Engine::Get_Component(L"GameLogic", 
																	L"Player", 
																	L"Com_Transform", 
																	Engine::ID_DYNAMIC));
	NULL_CHECK_RETURN(pPlayerTransCom, -1);

	_vec3	vPlayerPos;
	pPlayerTransCom->Get_Info(Engine::INFO_POS, &vPlayerPos);

	m_pTransformCom->Chase_Target(&vPlayerPos, m_fSpeed * fTimeDelta);

	return 0;
}

void CTestMonster::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_matWorld);

	m_pBufferCom->Render_Buffer();
}

void CTestMonster::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CTestMonster::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	// buffer
	pComponent = m_pBufferCom = dynamic_cast<Engine::CTriCol*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_TriCol"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	// transform
	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Transform", pComponent);


	return S_OK;
}

CTestMonster* CTestMonster::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTestMonster*	pInstance = new	CTestMonster(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

