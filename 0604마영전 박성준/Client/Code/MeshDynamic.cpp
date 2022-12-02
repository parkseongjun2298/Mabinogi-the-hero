#include "stdafx.h"
#include "MeshDynamic.h"

#include "Export_Function.h"

CMeshDynamic::CMeshDynamic(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CMeshDynamic::~CMeshDynamic(void)
{

}

HRESULT CMeshDynamic::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pMeshCom->Set_Animation(0);

	return S_OK;
}

_int CMeshDynamic::Update_GameObject(const _float & fTimeDelta)
{
	Key_Input(fTimeDelta);

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	m_pMeshCom->Play_Animation(fTimeDelta);
	m_pRendererCom->Add_Render_Group(Engine::RENDER_NONALPHA, this);

	return NO_EVENT;
}

void CMeshDynamic::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_matWorld);

	m_pMeshCom->Render_Meshes();
}

HRESULT CMeshDynamic::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	m_pMeshName = m_MeshName;

	// Mesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(RESOURCE_STAGE, m_pMeshName));
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

	return S_OK;
}

void CMeshDynamic::Key_Input(const _float & fTimeDelta)
{
}

CMeshDynamic * CMeshDynamic::Create(LPDIRECT3DDEVICE9 pGraphicDev, CString MeshName)
{
	CMeshDynamic*	pInstance = new	CMeshDynamic(pGraphicDev);

	pInstance->m_MeshName = MeshName;

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CMeshDynamic::Free(void)
{
	Engine::CGameObject::Free();
}
