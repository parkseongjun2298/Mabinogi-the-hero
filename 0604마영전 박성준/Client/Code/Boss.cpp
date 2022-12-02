#include "stdafx.h"
#include "Boss.h"

#include "Export_Function.h"


#include"DynamicCamera.h"
#include"Player.h"
#include "NaviMesh.h"
#include"MeshNavi.h"

#include"BossHpBack.h"
#include"BossName.h"
CBoss::CBoss(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev),
	m_fSWORDWAVECool(0.f), m_fDownTime(0.f), DyingTime(0.f), m_fColCheck(0.f)
	, m_fTest(0.f)

	
{

}

CBoss::~CBoss(void)
{

}

HRESULT CBoss::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


	m_NaviIndex = 30;

	Engine::CManagement::GetInstance()->Get_Navi()->Set_Index(m_NaviIndex);

	_vec3 vPos = (Engine::CManagement::GetInstance()->Get_Navi()->m_vecCell[m_NaviIndex]->m_vPoint[Engine::CCell::POINT_A] + Engine::CManagement::GetInstance()->Get_Navi()->m_vecCell[m_NaviIndex]->m_vPoint[Engine::CCell::POINT_B] + Engine::CManagement::GetInstance()->Get_Navi()->m_vecCell[m_NaviIndex]->m_vPoint[Engine::CCell::POINT_C]) / 3;
	
	m_pTransformCom->m_vInfo[Engine::INFO_POS] = vPos;
	
	m_pTransformCom->m_vScale = _vec3(0.03f, 0.03f, 0.03f);
	
	m_pTransformCom->Set_Rotate(Engine::ROT_Y, 89.5f);

	m_BossState = BOSS_IDLE;
	
	
	

	return S_OK;
}

_int CBoss::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
	{
		Engine::StopSound(Engine::CHANNELID::CH_BGM);
		Engine::PlayBGM(L"보스클리어.mp3");
		Engine::SetVolume(Engine::CHANNELID::CH_BGM, 0.3f);

		m_bCheckDead = true;
		m_BossState = BOSS_DYING;
		 DyingTime +=  1 * fTimeDelta;
		 m_fTest += 1* fTimeDelta;
		 m_pBossSword->Set_bIsDead();

		 m_pMeshCom->m_pAniCtrl->m_fAniSpeed = 0.1f;
		 if (DyingTime >= 8.f)
		 {
			 m_pMeshCom->m_pAniCtrl->m_fAniSpeed = 2.f;
			 m_pCamera->m_pBoss = nullptr;
			 m_pSword->m_pBoss = nullptr;
			 return DEAD_OBJ;
		 }
	}

	for (auto& Player : Engine::CManagement::GetInstance()->Get_Obj(L"Player"))
	{
		m_pPlayer = dynamic_cast<CPlayer*>(Player);
	};
	for (auto& Sword : Engine::CManagement::GetInstance()->Get_Obj(L"Sword"))
	{
		m_pSword = dynamic_cast<CSword*>(Sword);
	};
	for (auto& DynamicCamera : Engine::CManagement::GetInstance()->Get_Obj(L"DynamicCamera"))
	{
		m_pCamera = dynamic_cast<CDynamicCamera*>(DynamicCamera);
	};
	


	_vec3 Temp = *m_pPlayer->Get_Transform()->Get_Info(Engine::INFO_POS) - *this->Get_Transform()->Get_Info(Engine::INFO_POS) ;

	 m_fPlayer_BossDis=D3DXVec3Length(&Temp);
	
	Boss_Mode();

	m_pColliderCom->Update(m_pTransformCom->m_matWorld);

	Engine::CGameObject::Update_GameObject(fTimeDelta);
		

	m_pRendererCom->Add_Render_Group(Engine::RENDER_NONALPHA, this);
	
	if (m_pPlayer->m_PlayerState == SKILL3 && m_pPlayer->m_pMeshCom->m_pAniCtrl->m_fAccTime < 7.1f)
	{
		m_pMeshCom->Play_Animation(fTimeDelta * 0.02);
		
	}
	else 
		m_pMeshCom->Play_Animation(fTimeDelta);
	
		

	
	if (Engine::Get_DIKeyState(DIK_F2) & 0x80)
	{
		Engine::StopSound(Engine::CHANNELID::CH_BGM);
		Engine::PlayBGM(L"CS_inkels_BGM.wav");
		Engine::SetVolume(Engine::CHANNELID::CH_BGM, 0.1f);
		
	}
	if (m_pCamera->m_CameraMode == BOSS)
	{
		m_BossState = BOSS_CUTSCENE1;
		if (!m_bSoundCut1)
		{
			Engine::PlaySoundVolume(L"CS_inkels_1.wav", Engine::CH_BOSS, 0.08f);
			m_bSoundCut1 = true;
		}
	}
	if (m_pCamera->m_CameraMode == BOSS2)
		m_BossState = BOSS_CUTSCENE2;

	if (m_pCamera->m_bBossCtl)
	{
		m_BossState = BOSS_RUN;
		m_pCamera->m_bBossCtl = false;
		m_bStart = true;

		Engine::StopSound(Engine::CHANNELID::CH_BGM);
		Engine::PlayBGM(L"bgm_ep8_ingkells.wav");
		Engine::SetVolume(Engine::CHANNELID::CH_BGM, 0.1f);
	}
	if (m_bStart == true)
	{
		Move_Boss(fTimeDelta);
		m_fSWORDWAVECool += 1 * fTimeDelta;
		
		//UI
		if (!m_bBackCheck)
		{
			m_pBossHpBack = CBossHpBack::Create(m_pGraphicDev);
			Engine::Add_GameObject(L"GameLogic", L"BossHpBack", m_pBossHpBack);


			m_pBossSword = CBossSword::Create(m_pGraphicDev,0);
			Engine::Add_GameObject(L"GameLogic", L"BossSword", m_pBossSword);

			m_bBackCheck = true;
		}
		
				
		if (!m_bRedCheck)
		{
			m_pBossHpRed = CBossHpRed::Create(m_pGraphicDev);
			Engine::Add_GameObject(L"GameLogic", L"CBossHpRed", m_pBossHpRed);
			m_bRedCheck = true;
		}
		if (!m_bYellowCheck)
		{
			m_pBossHpYellow = CBossHpYellow::Create(m_pGraphicDev);
			Engine::Add_GameObject(L"GameLogic", L"CBossHpYellow", m_pBossHpYellow);
			m_bYellowCheck = true;
		}
		if (!m_bGreenCheck)
		{
			m_pBossHpGreen = CBossHpGreen::Create(m_pGraphicDev);
			Engine::Add_GameObject(L"GameLogic", L"CBossHpGreen", m_pBossHpGreen);
			m_bGreenCheck = true;
		}
		if (!m_bXCheck)
		{
			m_pBossHpX = CBossHpX::Create(m_pGraphicDev);
			Engine::Add_GameObject(L"GameLogic", L"BossHpX", m_pBossHpX);
			m_bXCheck = true;
		}
		if (!m_bBossNameCheck)
		{
			m_pBossName = CBossName::Create(m_pGraphicDev);
			Engine::Add_GameObject(L"GameLogic", L"BossName", m_pBossName);
			m_bBossNameCheck = true;
		}
	
		if (m_pBossHpRed->m_fSizeX <= 0.f)
			Set_bIsDead();

	}
	if (m_fPlayer_BossDis >= 5.f && m_fPlayer_BossDis < 25.f && m_fSWORDWAVECool>=10.f && !m_bAniCheck)
		m_BossState = BOSS_SWORDWAVE;

	if (m_fPlayer_BossDis < 5.f   && m_fSWORDWAVECool < 10.f  && !m_bAniCheck)
		m_BossState = BOSS_SWING_TRIPLE;

	if (m_fPlayer_BossDis <5.f  && m_fSWORDWAVECool >= 10.f && !m_bAniCheck)
		m_BossState = BOSS_PIERCE;


	if (m_bPIERCECheck && m_bSWORDWAVECheck && m_bSWING_TRIPLECheck && m_bWHIRLWINDCheck)
		m_bGroogy = true;
		//m_BossState = BOSS_DOWNSTART;



	if (m_HitCheck)
	{
		m_fColCheck += 1 * fTimeDelta;
		//5초지나면
		if (m_fColCheck >= 0.5f)
		{
			m_HitCheck = false;
			m_bColCheck = false;
			m_fColCheck = 0.f;
		}

	}


	if (!m_HitCheck && m_bColCheck)
	{
		m_pHitEffect = CHitEffect::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[Engine::INFO_POS]);
		Engine::Add_GameObject(L"GameLogic", L"HitEffect", m_pHitEffect);

		m_HitCheck = true;
	}

	Engine::SoundUpdate();


	return 0;
}

void CBoss::Render_GameObject(void)
{

	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);

	_uint	iPassMax = 0;

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	pEffect->Begin(&iPassMax, 0);

	if(m_bCheckDead)
		pEffect->BeginPass(2);
	else
		pEffect->BeginPass(0);


	m_pMeshCom->Render_Meshes(pEffect);

	pEffect->EndPass();

	pEffect->End();


	if (GetAsyncKeyState(VK_TAB) & 0x8000)
		m_pColliderCom->Render_Collider(Engine::COL_FALSE, &m_pTransformCom->m_matWorld);
	

}

void CBoss::Free(void)
{
	Engine::CGameObject::Free();
}

void CBoss::Boss_Mode()
{
	switch (m_BossState)
	{
	case BOSS_IDLE:
		m_pMeshCom->Set_Animation(25);
		break;
	case BOSS_CUTSCENE1:
		m_pMeshCom->Set_Animation(27);
		break;
	case BOSS_CUTSCENE2:
		m_pMeshCom->Set_Animation(26);
		break;
	case BOSS_RUN:
		m_pMeshCom->Set_Animation(7);
		break;
	case BOSS_DAMAGE_WEAK_BOT:
		m_pMeshCom->Set_Animation(12);
		break;
	case BOSS_WHIRLWIND:
		m_pMeshCom->Set_Animation(16);
		break;
	case BOSS_SWORDWAVE:
		m_pMeshCom->Set_Animation(21);
		break;
	case BOSS_SWING_TRIPLE:
		m_pMeshCom->Set_Animation(22);
		break;
	case BOSS_THRUST_TO_WHIRLWIND:
		m_pMeshCom->Set_Animation(23);
		break;
	case BOSS_DOUBLESMASH:
		m_pMeshCom->Set_Animation(24);
		break;
	case BOSS_PIERCE:
		m_pMeshCom->Set_Animation(28);
		break;
	case BOSS_TRIPLE_COMBO:
		m_pMeshCom->Set_Animation(29);
		break;
	case BOSS_DOWNSTART:
		m_pMeshCom->Set_Animation(3);
		break;
	case BOSS_DOWNING:
		m_pMeshCom->Set_Animation(4);
		break;
	case BOSS_DOWNEND:
		m_pMeshCom->Set_Animation(2);
		break;
	case BOSS_DYING:
		m_pMeshCom->Set_Animation(10);
		break;
	default:
		break;
	}
}

void CBoss::Damage_Boss(const _float& fTimeDelta)
{
			m_fSpeed = 0.f;
		if (true == m_pMeshCom->Is_AnimationEnd())
			m_BossState = BOSS_RUN;
	
}

void CBoss::Move_Boss(const _float& fTimeDelta)
{
	//m_BossState = BOSS_RUN;
	
	
	if (m_BossState == BOSS_DAMAGE_WEAK_BOT)
	{
		Damage_Boss(fTimeDelta);
	}

	if (m_BossState == BOSS_WHIRLWIND)
	{
		WHIRLWIND(fTimeDelta);
	}
	if (m_BossState == BOSS_SWORDWAVE)
	{
		SWORDWAVE(fTimeDelta);
	}
	if (m_BossState == BOSS_SWING_TRIPLE)
	{
		SWING_TRIPLE(fTimeDelta);
	}
	if (m_BossState == BOSS_PIERCE)
	{
		PIERCE(fTimeDelta);
	}
	if (m_bGroogy)
	{
		Boss_Down(fTimeDelta);
	}

    if (m_BossState == BOSS_RUN && m_fPlayer_BossDis >= 2.f)
	{

		/*m_pDustEffect = CDustEffect::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[Engine::INFO_POS]);
		Engine::Add_GameObject(L"GameLogic", L"DustEffect", m_pDustEffect);*/


		m_fSpeed = 2.f;
		float TempSpeed = m_fSpeed*fTimeDelta;
		
		_vec3 vNormal;
		_vec3 vRight = m_pTransformCom->m_vInfo[Engine::INFO_RIGHT];
		_vec3 vLookDir = m_pPlayer->Get_Transform()->m_vInfo[Engine::INFO_POS]
			- m_pTransformCom->m_vInfo[Engine::INFO_POS];

		D3DXVec3Normalize(&vRight, &vRight);
		D3DXVec3Normalize(&vLookDir, &vLookDir);
		D3DXVec3Cross(&vNormal, &vRight, &vLookDir);

		float radian = D3DXVec3Dot(&vRight, &vLookDir);
		radian = min(radian, 1.f);
		radian = max(radian, -1.f);
		if (vNormal.y > 0.f) {
			m_pTransformCom->m_vAngle.y += acosf(radian);
		}
		else if (vNormal.y < 0.f) {
			m_pTransformCom->m_vAngle.y += -acosf(radian);
		}


		m_pTransformCom->m_vInfo[Engine::INFO_POS] = Engine::CManagement::GetInstance()->Get_Navi()->Move_OnNaviMesh(
			&m_pTransformCom->m_vInfo[Engine::INFO_POS],
			&(vLookDir * TempSpeed),
			&m_NaviIndex);
	

	}
	

	//cout << "보스 인덱스:" << m_NaviIndex << endl;
	
	//cout<<"Distance:" << m_fPlayer_BossDis << endl;
	//cout << "쿨타임:" << m_fSWORDWAVECool << endl;
	//cout << "애니시간:" << m_pMeshCom->m_pAniCtrl->m_fAccTime << endl; 
}

void CBoss::WHIRLWIND(const _float & fTimeDelta)
{
	
	m_bAniCheck = true;

	_vec3 vRight = m_pTransformCom->m_vInfo[Engine::INFO_RIGHT];

	if (m_pMeshCom->m_pAniCtrl->m_fAccTime >= 0.5f && m_pMeshCom->m_pAniCtrl->m_fAccTime < 1.1f)
	{
		if (!m_bSWORDWAVESound)
		{
			Engine::PlaySoundVolume(L"swing_lv5.wav", Engine::CH_BOSSEFFECT, 0.16f);
			m_bSWORDWAVESound = true;
		}

		m_fSpeed = 60.f;
		float TempSpeed = m_fSpeed*fTimeDelta;
		//공격모션할때만 충돌 체크
		CCollisionMgr::GetInstance()->Collision_PlayerToBoss(m_pPlayer, this);
		m_pTransformCom->m_vInfo[Engine::INFO_POS] = Engine::CManagement::GetInstance()->Get_Navi()->Move_OnNaviMesh(
			&m_pTransformCom->m_vInfo[Engine::INFO_POS],
			&(vRight * TempSpeed),
			&m_NaviIndex);
	}
	if (true == m_pMeshCom->Is_AnimationEnd())
	{
		m_bSWORDWAVESound = false;
		m_bWHIRLWINDCheck = true;
		m_BossState = BOSS_RUN;
		m_fSWORDWAVECool = 0.f;

		m_bAniCheck = false;
	}
}

void CBoss::SWORDWAVE(const _float & fTimeDelta)
{
	m_bAniCheck = true;
	if (m_pMeshCom->m_pAniCtrl->m_fAccTime >= 2.f && m_pMeshCom->m_pAniCtrl->m_fAccTime < 2.1f)
	{
		if (!m_bSWORDWAVESound)
		{
			Engine::PlaySoundVolume(L"swing_lv5.wav", Engine::CH_BOSSEFFECT, 0.16f);
			m_bSWORDWAVESound = true;
		}
		//공격모션할때만 충돌 체크
		CCollisionMgr::GetInstance()->Collision_PlayerToBoss(m_pPlayer, this);
		m_pTransformCom->m_vInfo[Engine::INFO_POS] = m_pPlayer->Get_Transform()->m_vInfo[Engine::INFO_POS];
		
	}
	if (true == m_pMeshCom->Is_AnimationEnd())
	{
		m_bSWORDWAVESound = false;
		m_bSWORDWAVECheck = true;
		m_BossState =  BOSS_WHIRLWIND;

		//m_bAniCheck = false;
	}

}

void CBoss::SWING_TRIPLE(const _float & fTimeDelta)  
{
	m_bAniCheck = true;
	_vec3 vRight = m_pTransformCom->m_vInfo[Engine::INFO_RIGHT];

	if (m_pMeshCom->m_pAniCtrl->m_fAccTime >= 1.f && m_pMeshCom->m_pAniCtrl->m_fAccTime < 1.8f)
	{
		
			if (!m_bTripleSound)
			{
				Engine::PlaySoundVolume(L"swing_lv5.wav", Engine::CH_BOSSEFFECT, 0.16f);
				m_bTripleSound = true;
			}
		m_fSpeed = 60.f;
		float TempSpeed = m_fSpeed*fTimeDelta;
		//공격모션할때만 충돌 체크
		CCollisionMgr::GetInstance()->Collision_PlayerToBoss(m_pPlayer, this);

		m_pTransformCom->m_vInfo[Engine::INFO_POS] = Engine::CManagement::GetInstance()->Get_Navi()->Move_OnNaviMesh(
			&m_pTransformCom->m_vInfo[Engine::INFO_POS],
			&(vRight * TempSpeed),
			&m_NaviIndex);

	}

	if (m_pMeshCom->m_pAniCtrl->m_fAccTime >= 1.8f && m_pMeshCom->m_pAniCtrl->m_fAccTime < 2.6f)
	{
	
		if (!m_bTripleSound2)
		{
			Engine::PlaySoundVolume(L"swing_lv5.wav", Engine::CH_BOSSEFFECT, 0.16f);
			m_bTripleSound2 = true;
		}
		m_fSpeed = 60.f;
		float TempSpeed = m_fSpeed*fTimeDelta;
		//공격모션할때만 충돌 체크
		CCollisionMgr::GetInstance()->Collision_PlayerToBoss(m_pPlayer, this);

		m_pTransformCom->m_vInfo[Engine::INFO_POS] = Engine::CManagement::GetInstance()->Get_Navi()->Move_OnNaviMesh(
			&m_pTransformCom->m_vInfo[Engine::INFO_POS],
			&(vRight * TempSpeed),
			&m_NaviIndex);

	}


	if (m_pMeshCom->m_pAniCtrl->m_fAccTime >= 2.6f && m_pMeshCom->m_pAniCtrl->m_fAccTime < 3.4f)
	{
		
		if (!m_bTripleSound3)
		{
			Engine::PlaySoundVolume(L"swing_lv5.wav", Engine::CH_BOSSEFFECT, 0.16f);
			m_bTripleSound3 = true;
		}
		m_fSpeed = 60.f;
		float TempSpeed = m_fSpeed*fTimeDelta;
		//공격모션할때만 충돌 체크
		CCollisionMgr::GetInstance()->Collision_PlayerToBoss(m_pPlayer, this);

		m_pTransformCom->m_vInfo[Engine::INFO_POS] = Engine::CManagement::GetInstance()->Get_Navi()->Move_OnNaviMesh(
			&m_pTransformCom->m_vInfo[Engine::INFO_POS],
			&(vRight * TempSpeed),
			&m_NaviIndex);

	}


				

	if (true == m_pMeshCom->Is_AnimationEnd())
	{
		m_bTripleSound = false;
		m_bTripleSound2 = false;
		m_bTripleSound3 = false;
		m_bSWING_TRIPLECheck = true;
		m_BossState = BOSS_RUN;

		m_bAniCheck = false;
	}
}

void CBoss::PIERCE(const _float & fTimeDelta)
{
	
	m_bAniCheck = true;
	_vec3 vRight = m_pTransformCom->m_vInfo[Engine::INFO_RIGHT];

	if (m_pMeshCom->m_pAniCtrl->m_fAccTime >= 1.5f && m_pMeshCom->m_pAniCtrl->m_fAccTime < 1.9f)
	{
		m_fSpeed = 60.f;
		float TempSpeed = m_fSpeed*fTimeDelta;
		//공격모션할때만 충돌 체크
		CCollisionMgr::GetInstance()->Collision_PlayerToBoss(m_pPlayer, this);

		m_pTransformCom->m_vInfo[Engine::INFO_POS] = Engine::CManagement::GetInstance()->Get_Navi()->Move_OnNaviMesh(
			&m_pTransformCom->m_vInfo[Engine::INFO_POS],
			&(vRight * TempSpeed),
			&m_NaviIndex);
		if (!m_bPierceSound)
		{
			Engine::PlaySoundVolume(L"swing_lv5.wav", Engine::CH_BOSSEFFECT, 0.16f);
			m_bPierceSound = true;
		}
	}
	

	if (true == m_pMeshCom->Is_AnimationEnd())
	{
		m_bPIERCECheck = true;
		m_BossState = BOSS_RUN;

		m_bAniCheck = false;
		m_bPierceSound = false;
	}

}

void CBoss::Boss_Down(const _float & fTimeDelta)
{
	m_fDownTime += 1 * fTimeDelta;
	m_BossState = BOSS_DOWNSTART;
	if (!m_bGroogySound)
	{
		Engine::PlaySoundVolume(L"CS_inkels_4.wav", Engine::CH_BOSS, 0.16f);
		m_bGroogySound = true;
	}
	if (m_fDownTime >= 1.f && m_fDownTime < 2.f)
	{
		
		m_BossState = BOSS_DOWNING;
		
	}
	if (m_fDownTime >= 2.f && m_fDownTime < 3.f)
	{
		m_BossState = BOSS_DOWNEND;
	}
	if ( m_fDownTime >= 3.f)
	{
		m_bPIERCECheck = false;  
		m_bSWORDWAVECheck = false;
		m_bSWING_TRIPLECheck = false;
		m_bWHIRLWINDCheck = false;
		m_fDownTime = 0.f;
		m_bGroogy = false;
		m_BossState = BOSS_RUN;
		m_bGroogySound = false;

		m_bAniCheck = false;
	}


}

HRESULT CBoss::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	// Mesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(RESOURCE_STAGE, L"Mesh_Ingkells"));
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

	// texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_Dissolve"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	return S_OK;
}

CBoss* CBoss::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBoss*	pInstance = new	CBoss(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}




HRESULT CBoss::SetUp_ConstantTable(LPD3DXEFFECT& pEffect)
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
	tMtrlInfo.Specular = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.f);
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
	pEffect->SetFloat("g_fUv", m_fTest / 10);


	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture2");
	return S_OK;
}

