#include "stdafx.h"
#include "Knight.h"

#include "Export_Function.h"

CKnight::CKnight(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_fDieCiunt(0.f), m_fHitCool(0.f)

	
{

}

CKnight::~CKnight(void)
{

}

HRESULT CKnight::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);



	Engine::CManagement::GetInstance()->Get_Navi()->Set_Index(m_NaviIndex);

	_vec3 vPos = (Engine::CManagement::GetInstance()->Get_Navi()->m_vecCell[m_NaviIndex]->m_vPoint[Engine::CCell::POINT_A] + Engine::CManagement::GetInstance()->Get_Navi()->m_vecCell[m_NaviIndex]->m_vPoint[Engine::CCell::POINT_B] + Engine::CManagement::GetInstance()->Get_Navi()->m_vecCell[m_NaviIndex]->m_vPoint[Engine::CCell::POINT_C]) / 3;
	
	m_pTransformCom->m_vInfo[Engine::INFO_POS] = vPos;
	
	m_pTransformCom->m_vScale = _vec3(0.025f, 0.025f, 0.025f);
	
	m_pTransformCom->Rotation(Engine::ROT_Y, 135);
	m_KnightState = Knight_STAND;

	

	return S_OK;
}

_int CKnight::Update_GameObject(const _float& fTimeDelta)
{
	for (auto& Player : Engine::CManagement::GetInstance()->Get_Obj(L"Player"))
	{
		m_pPlayer = dynamic_cast<CPlayer*>(Player);
	};
	for (auto& Sword : Engine::CManagement::GetInstance()->Get_Obj(L"Sword"))
	{
		m_pSword = dynamic_cast<CSword*>(Sword);
	};

	if (m_bDead)
	{
		m_bCheckDead = true;
		m_KnightState = Knight_DYING;
		 m_fDieCiunt += 1 * fTimeDelta;
		if (m_fDieCiunt >=0.9f)
		{
			m_pPlayer->m_iKillMonCnt += 1;
			m_pSword->m_pKnight = nullptr;
			return DEAD_OBJ;
		}
	}
	

	

	_vec3 Temp = *m_pPlayer->Get_Transform()->Get_Info(Engine::INFO_POS) - *this->Get_Transform()->Get_Info(Engine::INFO_POS);

	m_fPlayer_BossDis = D3DXVec3Length(&Temp);
	Knight_Mode();

	
	m_pColliderCom->Update(m_pTransformCom->m_matWorld);
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	

	m_pRendererCom->Add_Render_Group(Engine::RENDER_NONALPHA, this);

	if (m_pPlayer->m_PlayerState == SKILL3 && m_pPlayer->m_pMeshCom->m_pAniCtrl->m_fAccTime < 7.1f)
	{
		//m_pMeshCom->m_pAniCtrl->m_fAniSpeed = 0.2f;
		m_pMeshCom->Play_Animation(fTimeDelta * 0.02);
	}
	else
		m_pMeshCom->Play_Animation(fTimeDelta);
		//m_pMeshCom->m_pAniCtrl->m_fAniSpeed = 2.f;


	


	


	if(m_bFollowPlayer)
		Move_Knight(fTimeDelta);

	 if (m_bFollowPlayer && m_fPlayer_BossDis >= 0.1f && m_fPlayer_BossDis < 3.f)
		m_KnightState = Knight_DASH_ATT;

	
	 if (m_HitCheck)
	 {
		 m_fHitCool += 1 * fTimeDelta;
		 //5초지나면
		 if (m_fHitCool >= 1.f)
		 {
			 m_HitCheck = false;
			 m_fHitCool = 0.f;
		 }

	 }
	

	return 0;
}

void CKnight::Render_GameObject(void)
{

	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);

	_uint	iPassMax = 0;

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	pEffect->Begin(&iPassMax, 0);
	if (m_bCheckDead)
		pEffect->BeginPass(2);
	else
		pEffect->BeginPass(0);

	m_pMeshCom->Render_Meshes(pEffect);

	pEffect->EndPass();

	pEffect->End();


	if (GetAsyncKeyState(VK_TAB) & 0x8000)
		m_pColliderCom->Render_Collider(Engine::COL_FALSE, &m_pTransformCom->m_matWorld);

}

void CKnight::Free(void)
{
	Engine::CGameObject::Free();
}

void CKnight::Knight_Mode()
{
	switch (m_KnightState)
	{
	case Knight_STAND:
		m_pMeshCom->Set_Animation(7);
		break;
	case Knight_DASH_ATT:
		m_pMeshCom->Set_Animation(0);
		break;
	case Knight_DYING:
		m_pMeshCom->Set_Animation(6);
		break;
	default:
		break;
	}
}

HRESULT CKnight::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	// Mesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(RESOURCE_STAGE, L"Mesh_Knight"));
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

CKnight* CKnight::Create(LPDIRECT3DDEVICE9 pGraphicDev, _ulong NaviIndex)
{
	CKnight*	pInstance = new	CKnight(pGraphicDev);
	pInstance->m_NaviIndex = NaviIndex;
	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);
	

	return pInstance;
}

void CKnight::Move_Knight(const _float& fTimeDelta)
{
	//m_KnightState = Knight_RUN;
	 if (m_KnightState == Knight_DASH_ATT )
	{
		ATT_Knight(fTimeDelta);
	}
	
	
}

void CKnight::ATT_Knight(const _float & fTimeDelta)
{
	
	_vec3 vRight = m_pTransformCom->m_vInfo[Engine::INFO_LOOK];

	if (m_pMeshCom->m_pAniCtrl->m_fAccTime >= 1.5f && m_pMeshCom->m_pAniCtrl->m_fAccTime < 3.f)
	{
		m_fSpeed = 50.f;
		float TempSpeed = m_fSpeed*fTimeDelta;
		//공격모션할때만 충돌 체크
		CCollisionMgr::GetInstance()->Collision_PlayerToKnight(m_pPlayer, this);
		m_pTransformCom->m_vInfo[Engine::INFO_POS] = Engine::CManagement::GetInstance()->Get_Navi()->Move_OnNaviMesh(
			&m_pTransformCom->m_vInfo[Engine::INFO_POS],
			&(vRight * TempSpeed),
			&m_NaviIndex);
	}
	if (true == m_pMeshCom->Is_AnimationEnd())
		m_KnightState = Knight_STAND;

	
	//cout << "애니시간:" << m_pMeshCom->m_pAniCtrl->m_fAccTime << endl;
}

void CKnight::Damage_Knight(const _float & fTimeDelta)
{
	if (!m_HitCheck)
	{
		m_pHitEffect = CHitEffect::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[Engine::INFO_POS]);
		Engine::Add_GameObject(L"GameLogic", L"HitEffect", m_pHitEffect);


		m_HitCheck = true;
	}

	m_fSpeed = 0.f;
	if (true == m_pMeshCom->Is_AnimationEnd())
		m_KnightState = Knight_RUN;
}


HRESULT CKnight::SetUp_ConstantTable(LPD3DXEFFECT& pEffect)
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

	tMtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f); //전체적으로 어두워짐
	tMtrlInfo.Specular = D3DXCOLOR(0.1, 0.1, 0.1, 0.1f); //반짝반짝거리는거
	tMtrlInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);//뿌얘짐
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

