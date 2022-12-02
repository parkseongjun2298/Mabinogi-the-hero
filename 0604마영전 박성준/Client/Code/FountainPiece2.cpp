#include "stdafx.h"
#include "FountainPiece2.h"

#include "Export_Function.h"



CFountainPiece2::CFountainPiece2(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_iFlag(0)
	, m_bColl(false),
	m_fSaveTime(0.f)
	
{

}

CFountainPiece2::~CFountainPiece2(void)
{

}

HRESULT CFountainPiece2::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vInfo[Engine::INFO_POS] = { 0.8f,0.2f ,1.f }; //생성되는 위치. 이거 잘조절
	m_pTransformCom->m_vScale = { 0.02f, 0.02f, 0.02f };
	
	
	return S_OK;
}

_int CFountainPiece2::Update_GameObject(const _float& fTimeDelta)
{
	
	if (m_bDead)
		return DEAD_OBJ;

	
	Engine::CGameObject::Update_GameObject(fTimeDelta);
		
	m_pRendererCom->Add_Render_Group(Engine::RENDER_NONALPHA, this);
	
	m_fSaveTime += 1 * fTimeDelta;
	if (m_fSaveTime >= 3.f)
		Set_bIsDead();

	/*cout << "PosX:" << m_pTransformCom->m_vInfo[Engine::INFO_POS].x << endl;
	cout << "PosY:" << m_pTransformCom->m_vInfo[Engine::INFO_POS].y << endl;
	cout << "PosZ:" << m_pTransformCom->m_vInfo[Engine::INFO_POS].z << endl;*/
	//cout << "생성된지시간:" << m_fSaveTime << endl;

	_vec3 vPos, vLook, vRight;
	vPos = *m_pTransformCom->Get_Info(Engine::INFO_POS);
	vLook = *m_pTransformCom->Get_Info(Engine::INFO_LOOK);
	vRight = *m_pTransformCom->Get_Info(Engine::INFO_RIGHT);
	
	_vec3 Dest{ 15.f,0.f,-25.f }; //목적지인데 이거 클수록 더 멀리,빨리나감 .이거 x랑 z만 조절 하자

	m_fEndHeight = Dest.y - vPos.y;
	m_fHeight = m_fMaxHeight - vPos.y;
	m_fg = 2 * m_fHeight / (m_fMaxTime*m_fMaxTime);
	m_fY = sqrtf(2 * m_fg*m_fHeight);
	float a = m_fg;
	float b = -2 * m_fY;
	float c = 2 * m_fEndHeight;
	m_fEndTime = (-b + sqrtf(b*b - 2 * a*c)) / (2 * a);

	m_fX = -(vPos.x- Dest.x) / m_fEndTime;
	m_fZ = -(vPos.z - Dest.z) / m_fEndTime;

	vPos.x = vPos.x + m_fX*m_fSaveTime;
	vPos.y = vPos.y + (m_fY*m_fSaveTime) - (0.5f*m_fg*m_fSaveTime*m_fSaveTime);
	vPos.z = vPos.z + m_fZ*m_fSaveTime;

	m_pTransformCom->Set_Pos(&vPos);

	return 0;
}

void CFountainPiece2::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_matWorld);
	m_pMeshCom->Render_Meshes();


	if (GetAsyncKeyState(VK_TAB) & 0x8000)
		m_pColliderCom->Render_Collider(Engine::COL_FALSE, &m_pTransformCom->m_matWorld);

}

void CFountainPiece2::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CFountainPiece2::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	// Mesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(RESOURCE_STAGE, L"Mesh_FountainPiece1"));
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

CFountainPiece2* CFountainPiece2::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFountainPiece2*	pInstance = new	CFountainPiece2(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

