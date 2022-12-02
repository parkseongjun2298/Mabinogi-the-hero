#include "stdafx.h"
#include "Bomb.h"

#include "Export_Function.h"

#include"Player.h"
#include"CollisionMgr.h"
#include"Boss.h"
CBomb::CBomb(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_iFlag(0)
	, m_bColl(false)
	
{

}

CBomb::~CBomb(void)
{

}

HRESULT CBomb::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->m_vScale = { 0.025f, 0.025f, 0.025f };
	
	
	return S_OK;
}

_int CBomb::Update_GameObject(const _float& fTimeDelta)
{
	
	if (m_bDead)
		return DEAD_OBJ;

	m_pColliderCom->Update(m_pTransformCom->m_matWorld);
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	
	m_pRendererCom->Add_Render_Group(Engine::RENDER_NONALPHA, this);
	
	_vec3 vPos, vLook, vRight;
	vPos = *m_pTransformCom->Get_Info(Engine::INFO_POS);
	vLook = *m_pTransformCom->Get_Info(Engine::INFO_LOOK);
	vRight = *m_pTransformCom->Get_Info(Engine::INFO_RIGHT);

	

	/*D3DXVec3Normalize(&vLook, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);

	float TempSpeed = m_fSpeed*fTimeDelta;
	vLook.z += TempSpeed;
	m_pTransformCom->Move_Pos(&(vLook));*/

	for (auto& Boss : Engine::CManagement::GetInstance()->Get_Obj(L"Boss"))
	{
		m_pBoss = dynamic_cast<CBoss*>(Boss);
	};

	m_fEndHeight = m_pBoss->Get_Transform()->Get_Info(Engine::INFO_POS)->y - vPos.y;
	m_fHeight = m_fMaxHeight - vPos.y;
	m_fg = 2*m_fHeight/(m_fMaxTime*m_fMaxTime);
	m_fY = sqrtf(2 * m_fg*m_fHeight);
	float a = m_fg;
	float b = -2 * m_fY;
	float c = 2 * m_fEndHeight;
	m_fEndTime = (-b + sqrtf(b*b - 2 * a*c)) / (2 * a);

	m_fX = -(vPos.x - m_pBoss->Get_Transform()->Get_Info(Engine::INFO_POS)->x )/ m_fEndTime;
	m_fZ = -(vPos.z - m_pBoss->Get_Transform()->Get_Info(Engine::INFO_POS)->z )/ m_fEndTime;

	m_fTime += 1*fTimeDelta;
	

	vPos.x = vPos.x + m_fX*m_fTime;
	vPos.y = vPos.y + (m_fY*m_fTime)-(0.5f*m_fg*m_fTime*m_fTime);
	vPos.z = vPos.z + m_fZ*m_fTime;

	m_pTransformCom->Set_Pos(&vPos);

	cout << "캐논볼posX:" << m_pTransformCom->Get_Info(Engine::INFO_POS)->x << endl;
	cout << "캐논볼posY:" << m_pTransformCom->Get_Info(Engine::INFO_POS)->y << endl;
	cout << "캐논볼posZ:" << m_pTransformCom->Get_Info(Engine::INFO_POS)->z << endl;



	if (m_fTime>=2.5f/*m_pTransformCom->Get_Info(Engine::INFO_POS)->y <=0.f*/)
		Set_bIsDead();
	return 0;
}

void CBomb::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_matWorld);
	m_pMeshCom->Render_Meshes();
	
	if (GetAsyncKeyState(VK_TAB) & 0x8000)
		m_pColliderCom->Render_Collider(Engine::COL_FALSE, &m_pTransformCom->m_matWorld);   
}

void CBomb::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CBomb::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	// Mesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(RESOURCE_STAGE, L"Mesh_Bomb"));
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

CBomb* CBomb::Create(LPDIRECT3DDEVICE9 pGraphicDev,const _vec3* CanonPos,POINT MousePos)
{
	CBomb*	pInstance = new	CBomb(pGraphicDev);
	
	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);
	pInstance->m_pTransformCom->m_vInfo[Engine::INFO_POS] = { CanonPos->x,CanonPos->y+2.f ,CanonPos->z };
	pInstance->m_pMousePoint = MousePos;

	return pInstance;
}

