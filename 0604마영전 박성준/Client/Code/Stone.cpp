#include "stdafx.h"
#include "Stone.h"

#include "Export_Function.h"

CStone::CStone(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_iFlag(0)
	, m_bColl(false)
	, m_bDraw(false)
{

}

CStone::~CStone(void)
{

}

HRESULT CStone::Ready_GameObject(const _uint& iFlag)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Pos(&_vec3(rand() % 10 + 5.f, 0.f, rand() % 10 + 5.f));

	m_iFlag = iFlag;
	
	return S_OK;
}

_int CStone::Update_GameObject(const _float& fTimeDelta)
{
	SetUp_Terrain();

	//m_bColl = CollisionTo_Object(L"GameLogic", L"Player");

	Engine::CGameObject::Update_GameObject(fTimeDelta);


	_vec3	vPos;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);

	m_bDraw = m_pOptimizationCom->Is_InFrustumForObject(&vPos);

	m_pRendererCom->Add_Render_Group(Engine::RENDER_NONALPHA, this);


	return 0;
}

void CStone::Render_GameObject(void)
{
	if (false == m_bDraw)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_matWorld);
	m_pMeshCom->Render_Meshes();

	_matrix		matWorld;
	
	if (0 == m_iFlag) // aabb
		m_pTransformCom->Get_NRotWorldMatrix(&matWorld);
	
	else// obb
		m_pTransformCom->Get_WorldMatrix(&matWorld);

	m_pColliderCom->Render_Collider(Engine::COLLTYPE(FALSE), &matWorld);

}

void CStone::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CStone::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	// Mesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>
		(Engine::Clone(RESOURCE_STAGE, L"Mesh_Stone"));
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

	// Calculator
	pComponent = m_pCalculatorCom = Engine::CCalculator::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Calculator", pComponent);

	// Collider
	pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev, 
															m_pMeshCom->Get_VtxPos(), 
															m_pMeshCom->Get_VtxCnt(),
															m_pMeshCom->Get_Stride());
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Collider", pComponent);

	// Optimization
	pComponent = m_pOptimizationCom = dynamic_cast<Engine::COptimization*>
		(Engine::Clone_Prototype(L"Proto_Optimization"));
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Optimization", pComponent);

	return S_OK;
}

CStone* CStone::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iFlag)
{
	CStone*	pInstance = new	CStone(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(iFlag)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CStone::SetUp_Terrain(void)
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);

	Engine::CTerrainTex*		pTerrainBufferCom = dynamic_cast<Engine::CTerrainTex*>(Engine::Get_Component(L"GameLogic", L"Terrain", L"Com_Buffer", Engine::ID_STATIC));
	NULL_CHECK(pTerrainBufferCom);

	_float	fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, 
																pTerrainBufferCom->Get_VtxPos(),
																pTerrainBufferCom->Get_VtxCntX(),
																pTerrainBufferCom->Get_VtxCntZ());

	m_pTransformCom->Set_Pos(vPos.x, fHeight, vPos.z);

}

_bool CStone::CollisionTo_Object(const _tchar* pLayerTag, const _tchar* pObjTag)
{
	Engine::CCollider*		pPlayerColliderCom = dynamic_cast<Engine::CCollider*>(Engine::Get_Component(pLayerTag, pObjTag, L"Com_Collider", Engine::ID_STATIC));
	NULL_CHECK_RETURN(pPlayerColliderCom, false);

	if(0 == m_iFlag)
		return m_pCalculatorCom->Collision_AABB(pPlayerColliderCom->Get_Min(), 
											pPlayerColliderCom->Get_Max(), 
											pPlayerColliderCom->Get_CollMatrix(),
											m_pColliderCom->Get_Min(), 
											m_pColliderCom->Get_Max(), 
											m_pColliderCom->Get_CollMatrix());
	else
		return m_pCalculatorCom->Collision_OBB(pPlayerColliderCom->Get_Min(),
			pPlayerColliderCom->Get_Max(),
			pPlayerColliderCom->Get_CollMatrix(),
			m_pColliderCom->Get_Min(),
			m_pColliderCom->Get_Max(),
			m_pColliderCom->Get_CollMatrix());

}

