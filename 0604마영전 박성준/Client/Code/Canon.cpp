#include "stdafx.h"
#include "Canon.h"

#include "Export_Function.h"

#include"Player.h"
#include"CollisionMgr.h"

CCanon::CCanon(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_iFlag(0)
	, m_bColl(false)
	
{

}

CCanon::~CCanon(void)
{

}

HRESULT CCanon::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vInfo[Engine::INFO_POS] = { 0.f,0.f ,-10.f };
	m_pTransformCom->m_vScale = { 0.025f, 0.025f, 0.025f };
	m_fAngleY = 270.f;
	m_pTransformCom->Set_Rotate(Engine::ROT_Y, D3DXToRadian(m_fAngleY));
	
	return S_OK;
}

_int CCanon::Update_GameObject(const _float& fTimeDelta)
{
	
	if (m_bDead)
		return DEAD_OBJ;


	Engine::CGameObject::Update_GameObject(fTimeDelta);

	
	m_pRendererCom->Add_Render_Group(Engine::RENDER_NONALPHA, this);
	



	return 0;
}

void CCanon::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_matWorld);
	m_pMeshCom->Render_Meshes();


	if (GetAsyncKeyState(VK_TAB) & 0x8000)
		m_pColliderCom->Render_Collider(Engine::COL_FALSE, &m_pTransformCom->m_matWorld);

}

void CCanon::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CCanon::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	// Mesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(RESOURCE_STAGE, L"Mesh_Canon"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);
		
	// transform
	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// renderer
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);



	// Collider
	pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev, 
															m_pMeshCom->Get_VtxPos(),
															m_pMeshCom->Get_VtxCnt(),
															m_pMeshCom->Get_Stride(),
															m_pMeshCom,
															Engine::MESHTYPE::TYPE_STATIC);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Collider", pComponent);


	return S_OK;
}

CCanon* CCanon::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCanon*	pInstance = new	CCanon(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

