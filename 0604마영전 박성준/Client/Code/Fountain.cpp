#include "stdafx.h"
#include "Fountain.h"

#include "Export_Function.h"

#include"Boss.h"
#include"FountainPiece0.h"
#include"FountainPiece1.h"
#include"FountainPiece2.h"
#include"FountainPiece3.h"
#include"FountainPiece4.h"
#include"FountainPiece5.h"
#include"FountainPiece6.h"
#include"FountainPiece7.h"
#include"FountainPiece8.h"
#include"FountainPiece9.h"
#include"Fountain2.h"
#include"CollisionMgr.h"

CFountain::CFountain(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_iFlag(0)
	, m_bColl(false)
	
{

}

CFountain::~CFountain(void)
{

}

HRESULT CFountain::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->m_vInfo[Engine::INFO_POS] = { 0.f,0.f ,-1.f };
	m_pTransformCom->m_vScale = { 0.02f, 0.02f, 0.02f };
	
	
	return S_OK;
}

_int CFountain::Update_GameObject(const _float& fTimeDelta)
{
	
	if (m_bDead)
	{
		if (!m_bSound)
		{
			Engine::PlaySoundVolume(L"explosion_lv2.wav", Engine::CH_EXAMPLE, 0.17f);
			m_bSound = true;
		}
		m_pPiece0 = CFountainPiece0::Create(m_pGraphicDev);
		Engine::Add_GameObject(L"GameLogic", L"Piece0", m_pPiece0);
		m_pPiece1 = CFountainPiece1::Create(m_pGraphicDev);
		Engine::Add_GameObject(L"GameLogic", L"Piece1", m_pPiece1);
		m_pPiece2 = CFountainPiece2::Create(m_pGraphicDev);
		Engine::Add_GameObject(L"GameLogic", L"Piece2", m_pPiece2);
		m_pPiece3 = CFountainPiece3::Create(m_pGraphicDev);
		Engine::Add_GameObject(L"GameLogic", L"Piece3", m_pPiece3);
		m_pPiece4 = CFountainPiece4::Create(m_pGraphicDev);
		Engine::Add_GameObject(L"GameLogic", L"Piece4", m_pPiece4);
		m_pPiece5 = CFountainPiece5::Create(m_pGraphicDev);
		Engine::Add_GameObject(L"GameLogic", L"Piece5", m_pPiece5);
		m_pPiece6 = CFountainPiece6::Create(m_pGraphicDev);
		Engine::Add_GameObject(L"GameLogic", L"Piece6", m_pPiece6);
		m_pPiece7 = CFountainPiece7::Create(m_pGraphicDev);
		Engine::Add_GameObject(L"GameLogic", L"Piece7", m_pPiece7);
		m_pPiece8 = CFountainPiece8::Create(m_pGraphicDev);
		Engine::Add_GameObject(L"GameLogic", L"Piece8", m_pPiece8);
		m_pPiece9 = CFountainPiece9::Create(m_pGraphicDev);
		Engine::Add_GameObject(L"GameLogic", L"Piece9", m_pPiece9);
		m_pFountain2 = CFountain2::Create(m_pGraphicDev);
		Engine::Add_GameObject(L"GameLogic", L"Fountain2", m_pFountain2);

		return DEAD_OBJ;
	}

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	
	m_pRendererCom->Add_Render_Group(Engine::RENDER_NONALPHA, this);
	
	for (auto& Boss : Engine::CManagement::GetInstance()->Get_Obj(L"Boss"))
	{
		m_pBoss = dynamic_cast<CBoss*>(Boss);
	};
	


	if (m_pBoss == nullptr)
		return 0;

	if (m_pBoss->m_bStart)
	{
		if (m_pBoss->m_bCheckDead == false)
		{
			CCollisionMgr::GetInstance()->Collision_FountainToBoss(this, m_pBoss);
		}
	}



	return 0;
}

void CFountain::Render_GameObject(void)
{
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);

	_uint	iPassMax = 0;

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	pEffect->Begin(&iPassMax, 0);

	pEffect->BeginPass(0);

	m_pMeshCom->Render_Meshes(pEffect);

	pEffect->EndPass();

	pEffect->End();


	if (GetAsyncKeyState(VK_TAB) & 0x8000)
		m_pColliderCom->Render_Collider(Engine::COL_FALSE, &m_pTransformCom->m_matWorld);

}

void CFountain::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CFountain::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	// Mesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(RESOURCE_STAGE, L"Mesh_Fountain"));
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


	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Mesh"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;
}

HRESULT CFountain::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
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
	tMtrlInfo.Specular = D3DXCOLOR(0.1, 0.1, 0.1, 1.f);
	tMtrlInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	tMtrlInfo.Power = 1000.f;

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

CFountain* CFountain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFountain*	pInstance = new	CFountain(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

