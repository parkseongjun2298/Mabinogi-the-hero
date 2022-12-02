#include "stdafx.h"
#include "LordOfMana.h"

#include "Export_Function.h"



#include"Dog.h"
#include"Soldier.h"
#include"Knight.h"
#include"Guarder.h"
#include"Boss.h"
#include"CollisionMgr.h"
CLordOfMana::CLordOfMana(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_iFlag(0)
	, m_bColl(false)
	, m_fBladeSpeed(20.f)
	, m_fSaveTime(0.f)
	
{

}

CLordOfMana::~CLordOfMana(void)
{

}

HRESULT CLordOfMana::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vScale = _vec3(1.f, 1.f, 1.f);
	m_vPos = { m_pTransformCom->Get_Info(Engine::INFO_POS)->x ,m_pTransformCom->Get_Info(Engine::INFO_POS)->y - 2 ,m_pTransformCom->Get_Info(Engine::INFO_POS)->z };

	m_pTransformCom->Set_Pos(&m_vPos);
	
	m_pMeshCom->Set_Animation(0);
	return S_OK;
}

_int CLordOfMana::Update_GameObject(const _float& fTimeDelta)
{
	
	if (m_bDead)
		return DEAD_OBJ;

	Ghost_Mode();
	m_pColliderCom->Update(m_pTransformCom->m_matWorld);
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	m_pMeshCom->Play_Animation(fTimeDelta);
	
	m_pRendererCom->Add_Render_Group(Engine::RENDER_NONALPHA, this);
	

	m_fSaveTime += 1.f*fTimeDelta;

	if (m_fSaveTime >= 1.1f)
	{
		Set_bIsDead();
	}
	
	for (auto& Dog : Engine::CManagement::GetInstance()->Get_Obj(L"Dog"))
	{
		m_pDog = dynamic_cast<CDog*>(Dog);
	};
	for (auto& Soldier : Engine::CManagement::GetInstance()->Get_Obj(L"Soldier"))
	{
		m_pSoldier = dynamic_cast<CSoldier*>(Soldier);
	};
	for (auto& Guarder : Engine::CManagement::GetInstance()->Get_Obj(L"Guarder"))
	{
		m_pGuarder = dynamic_cast<CGuarder*>(Guarder);
	};
	for (auto& Knight : Engine::CManagement::GetInstance()->Get_Obj(L"Knight"))
	{
		m_pKnight = dynamic_cast<CKnight*>(Knight);
	};
	for (auto& Boss : Engine::CManagement::GetInstance()->Get_Obj(L"Boss"))
	{
		m_pBoss = dynamic_cast<CBoss*>(Boss);
	};
		
	
		// 여기서 가끔 터짐
		 if (m_pSoldier != nullptr)
		{
			CCollisionMgr::GetInstance()->CollisionLordOfManaToSoldier(this, m_pSoldier);
		}
		 if (m_pDog != nullptr)
		 {
			 CCollisionMgr::GetInstance()->CollisionLordOfManaToDog(this, m_pDog);//충돌어디서 불러야하지
		 }
		 if (m_pKnight != nullptr )
		 {
			 CCollisionMgr::GetInstance()->Collision_LordOfManaToKnight(this, m_pKnight);//충돌어디서 불러야하지
		 }
		 if (m_pGuarder != nullptr )
		 {
			 CCollisionMgr::GetInstance()->Collision_LordOfManaToGuarder(this, m_pGuarder);//충돌어디서 불러야하지
		 }
		 if (m_pBoss != nullptr )
		{

			CCollisionMgr::GetInstance()->CollisionLordOfManaToBoss(this, m_pBoss);
		}


		/*cout << "PosX:" << m_pTransformCom->m_vInfo[Engine::INFO_POS].x << endl;
		cout << "PosY:" << m_pTransformCom->m_vInfo[Engine::INFO_POS].y << endl;
		cout << "PosZ:" << m_pTransformCom->m_vInfo[Engine::INFO_POS].z << endl;*/

		
	return 0;
}

void CLordOfMana::Render_GameObject(void)
{
	/*m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_matWorld);
	m_pMeshCom->Render_Meshes();*/
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_matWorld);
	//m_pMeshCom->Render_Meshes();

	if (GetAsyncKeyState(VK_TAB) & 0x8000)
		m_pColliderCom->Render_Collider(Engine::COL_FALSE, &m_pTransformCom->m_matWorld);

}

void CLordOfMana::Free(void)
{
	Engine::CGameObject::Free();
}

void CLordOfMana::Ghost_Mode()
{
	switch (m_GhostState)
	{
	case Ghost_Idle:
		m_pMeshCom->Set_Animation(0);
		break;

	default:
		break;
	}
}

HRESULT CLordOfMana::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	// Mesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(RESOURCE_STAGE, L"Mesh_Ghost"));
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
															m_pMeshCom->Get_Stride(),
															m_pMeshCom,
															Engine::MESHTYPE::TYPE_DYNAMIC);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Collider", pComponent);

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Mesh"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);


	return S_OK;
}

HRESULT CLordOfMana::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
{
	_matrix		matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	// 조명

	const D3DLIGHT9*		pLightInfo = Engine::Get_Light(0);
	NULL_CHECK_RETURN(pLightInfo, E_FAIL);

	pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLightInfo->Diffuse);
	pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLightInfo->Specular);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLightInfo->Ambient);

	// 재질

	D3DMATERIAL9					tMtrlInfo;
	ZeroMemory(&tMtrlInfo, sizeof(D3DMATERIAL9));

	tMtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	tMtrlInfo.Power = 100.f;

	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&tMtrlInfo.Diffuse);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&tMtrlInfo.Ambient);
	pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&tMtrlInfo.Specular);

	pEffect->SetFloat("g_fPower", tMtrlInfo.Power);

	D3DXMatrixInverse(&matView, NULL, &matView);
	_vec4	vCamPos;
	memcpy(&vCamPos, &matView.m[3][0], sizeof(_vec4));

	pEffect->SetVector("g_vCamPos", &vCamPos);

	return S_OK;
}

CLordOfMana* CLordOfMana::Create(LPDIRECT3DDEVICE9 pGraphicDev,_vec3 Pos)
{
	CLordOfMana*	pInstance = new	CLordOfMana(pGraphicDev);
	
	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);
	pInstance->m_pTransformCom->m_vInfo[Engine::INFO_POS] = Pos;

	

	return pInstance;
}

