#include "stdafx.h"
#include "MeshStatic.h"

#include "Export_Function.h"

CMeshStatic::CMeshStatic(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_bColl(false)
{

}

CMeshStatic::~CMeshStatic(void)
{

}

HRESULT CMeshStatic::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	
	m_pTransformCom->m_vScale = { 0.01f, 0.01f, 0.01f };

	//_vec3 vPos;
	//_float fRadius;
	//BYTE*	pV;
	//m_pMeshCom->Get_Mesh()->LockVertexBuffer(0, (void**)&pV);

	//D3DXComputeBoundingSphere((_vec3*)pV, m_pMeshCom->Get_Mesh()->GetNumVertices(),
	//	m_pMeshCom->Get_Mesh()->GetNumBytesPerVertex(), &vPos, &fRadius);

	//m_pMeshCom->Get_Mesh()->UnlockVertexBuffer();

	//D3DXCreateSphere(m_pGraphicDev, fRadius, 10, 10, pMesh, NULL);
		
	return S_OK;
}

_int CMeshStatic::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return DEAD_OBJ;

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_Render_Group(Engine::RENDER_NONALPHA, this);

	return NO_EVENT;
}

void CMeshStatic::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_matWorld);
	m_pMeshCom->Render_Meshes();
	
	//m_pColliderCom->Render_Collider(Engine::COLLTYPE(m_bColl), &m_pTransformCom->m_matWorld);


	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

}

void CMeshStatic::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CMeshStatic::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	m_pMeshName = m_MeshName;

	// Mesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(RESOURCE_STAGE, m_pMeshName));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);
		
	//// transform
	//pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Clone_Prototype(L"Proto_Transform")); //¿©±â¼­ ÅÍÁü
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// transform
	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// renderer
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);

	//// Calculator
	//pComponent = m_pCalculatorCom = Engine::CCalculator::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Calculator", pComponent);

	//// Collider
	//pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev, 
	//														m_pMeshCom->Get_VtxPos(), 
	//														m_pMeshCom->Get_VtxCnt(),
	//														m_pMeshCom->Get_Stride());
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Collider", pComponent);

	return S_OK;
}

CMeshStatic* CMeshStatic::Create(LPDIRECT3DDEVICE9 pGraphicDev, CString MeshName, Engine::MESHTYPE MeshType)
{
	CMeshStatic*	pInstance = new	CMeshStatic(pGraphicDev);

	pInstance->m_MeshName = MeshName;
	pInstance->m_eMesyType = MeshType;

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}
