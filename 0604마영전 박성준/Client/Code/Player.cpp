#include "stdafx.h"
#include "Player.h"

#include "Export_Function.h"

#include"KeyMgr.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev), m_fDashEffectCool(0.f), 
	m_fRuinBladeCool(0.f), fDotResult(0.f), m_fLordOfManaCool(0.f),
	m_fLOMBotEffectCool(0.f), m_fHitCool(0.f), m_fLOMBlueFireEffectCool(0.f), m_fLOMHandEffectCool(0.f),
	m_fLOMHandEffect2Cool(0.f),
	m_pKeyMgr(Engine::CKeyMgr::GetInstance())

	
{

}

CPlayer::~CPlayer(void)
{

}

HRESULT CPlayer::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


	m_NaviIndex = 2;

	Engine::CManagement::GetInstance()->Get_Navi()->Set_Index(m_NaviIndex);

	_vec3 vPos = (Engine::CManagement::GetInstance()->Get_Navi()->m_vecCell[m_NaviIndex]->m_vPoint[Engine::CCell::POINT_A] + Engine::CManagement::GetInstance()->Get_Navi()->m_vecCell[m_NaviIndex]->m_vPoint[Engine::CCell::POINT_B] + Engine::CManagement::GetInstance()->Get_Navi()->m_vecCell[m_NaviIndex]->m_vPoint[Engine::CCell::POINT_C]) / 3;
	
	m_pTransformCom->m_vInfo[Engine::INFO_POS] = vPos;
	
	m_pTransformCom->m_vScale = _vec3(0.02f, 0.02f, 0.02f);
	
	m_pMeshCom->Set_Animation(10);

	

	return S_OK;
}

_int CPlayer::Update_GameObject(const _float& fTimeDelta)
{
	if (!m_bPlayerHpBackUi)
	{
		m_pPlayerHpBack = CPlayerHpBack::Create(m_pGraphicDev);
		Engine::Add_GameObject(L"GameLogic", L"PlayerHpBack", m_pPlayerHpBack);

		m_pPlayerSkillBar = CPlayerSkillBar::Create(m_pGraphicDev);
		Engine::Add_GameObject(L"GameLogic", L"CPlayerSkillBar", m_pPlayerSkillBar);

		m_pPlayerSkill1 = CPlayerSkill1::Create(m_pGraphicDev);
		Engine::Add_GameObject(L"GameLogic", L"CPlayerSkill1", m_pPlayerSkill1);

		m_pPlayerSkill2 = CPlayerSkill2::Create(m_pGraphicDev);
		Engine::Add_GameObject(L"GameLogic", L"CPlayerSkill2", m_pPlayerSkill2);
		m_bPlayerHpBackUi = true;
	}
	if (!m_bPlayerSpBackUi)
	{
		m_pPlayerSpBack = CPlayerSpBack::Create(m_pGraphicDev);
		Engine::Add_GameObject(L"GameLogic", L"PlayerSpBack", m_pPlayerSpBack);
		m_bPlayerSpBackUi = true;
	}
	if (!m_bPlayerHpUi)
	{
		m_pPlayerHp = CPlayerHp::Create(m_pGraphicDev);
		Engine::Add_GameObject(L"GameLogic", L"PlayerHp", m_pPlayerHp);
		m_bPlayerHpUi = true;
	}
	if (!m_bPlayerSpUi)
	{
		m_pPlayerSp = CPlayerSp::Create(m_pGraphicDev);
		Engine::Add_GameObject(L"GameLogic", L"PlayerSp", m_pPlayerSp);
		m_bPlayerSpUi = true;
	}
	
	
	


	Save_Navi_Index = m_NaviIndex;

	Key_Input(fTimeDelta);

	Player_Mode();
	
	
	m_pColliderCom->Update(m_pTransformCom->m_matWorld);
	Engine::CGameObject::Update_GameObject(fTimeDelta);
	for (auto& Boss : Engine::CManagement::GetInstance()->Get_Obj(L"Boss"))
	{
		m_pBoss = dynamic_cast<CBoss*>(Boss);
	};
	if (m_pBoss != nullptr && m_pBoss->m_BossState == BOSS_DYING)
		m_pMeshCom->Play_Animation(fTimeDelta * 0.1);
	else
		m_pMeshCom->Play_Animation(fTimeDelta); //나중에 m_pMeshCom->Play_Animation(fTimeDelta);로 바꿀수있음바꾸자
	/*if (m_pBoss != nullptr && m_pBoss->m_bCheckDead)
	{
		Engine::StopSound(Engine::CHANNELID::CH_BGM);
		Engine::PlayBGM(L"보스클리어.mp3");
		Engine::SetVolume(Engine::CHANNELID::CH_BGM, 0.1f);

	}*/

	m_pRendererCom->Add_Render_Group(Engine::RENDER_NONALPHA, this);

	if (m_bDashEffectCheck)
	{
		m_fDashEffectCool += 1 * fTimeDelta;
		//1초지나면
		if (m_fDashEffectCool >= 1.f)
		{
			m_bDashEffectCheck = false;
			m_fDashEffectCool = 0.f;
		}

	}
	if (m_bLOMBotEffectCheck)
	{
		m_fLOMBotEffectCool += 1 * fTimeDelta;
		//1초지나면
		if (m_fLOMBotEffectCool >= 10.f)
		{
			m_bLOMBotEffectCheck = false;
			m_fLOMBotEffectCool = 0.f;
		}

	}
	if (m_bLOMBotEffect2Check)
	{
		m_fLOMBotEffect2Cool += 1 * fTimeDelta;
		//1초지나면
		if (m_fLOMBotEffect2Cool >= 10.f)
		{
			m_bLOMBotEffect2Check = false;
			m_fLOMBotEffect2Cool = 0.f;
		}

	}
	if (m_bLOMBlueFireEffectCheck)
	{
		m_fLOMBlueFireEffectCool += 1 * fTimeDelta;
		//1초지나면
		if (m_fLOMBlueFireEffectCool >= 10.f)
		{
			m_bLOMBlueFireEffectCheck = false;
			m_fLOMBlueFireEffectCool = 0.f;
		}

	}
	if (m_bLOMHandEffectCheck)
	{
		m_fLOMHandEffectCool += 1 * fTimeDelta;
		//1초지나면
		if (m_fLOMHandEffectCool >= 10.f)
		{
			m_bLOMHandEffectCheck = false;
			m_fLOMHandEffectCool = 0.f;
		}

	}
	if (m_bLOMHandEffect2Check)
	{
		m_fLOMHandEffect2Cool += 1 * fTimeDelta;
		//1초지나면
		if (m_fLOMHandEffect2Cool >= 10.f)
		{
			m_bLOMHandEffect2Check = false;
			m_fLOMHandEffect2Cool = 0.f;
		}

	}

	if (m_bRuinBladeCheck)
	{
		m_fRuinBladeCool += 1 * fTimeDelta;
		//5초지나면
		if (m_fRuinBladeCool >= 5.f)
		{
			m_bRuinBladeCheck = false;
			m_bRuinSound = false;
			m_fRuinBladeCool = 0.f;
		}

	}
	if (m_bLordOfManaCheck)
	{
		m_fLordOfManaCool += 1 * fTimeDelta;
		//5초지나면
		if (m_fLordOfManaCool >= 5.f)
		{
			m_bLordOfManaCheck = false;
			m_fLordOfManaCool = 0.f;
		}

	}
	
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
	
	//cout << "플레이어가 밝고있는 인데스:" << Save_Navi_Index << endl;
	//cout << "애니시간:" << m_pMeshCom->m_pAniCtrl->m_fAccTime << endl; 

	/*cout << "플레이어 충돌구 센터X:" << (m_pColliderCom->m_vCenter.x) << endl;
	cout << "플레이어 충돌구 센터Y:" << (m_pColliderCom->m_vCenter.y) << endl;
	cout << "플레이어 충돌구 센터Z:" << (m_pColliderCom->m_vCenter.z) << endl;*/
	//cout << m_iKillMonCnt << endl;

	/*cout << "PosX:" << m_pTransformCom->m_vInfo[Engine::INFO_POS].x << endl;
	cout << "PosY:" << m_pTransformCom->m_vInfo[Engine::INFO_POS].y << endl;
	cout << "PosZ:" << m_pTransformCom->m_vInfo[Engine::INFO_POS].z << endl;*/

	
	

	
	
	

	return 0;
}

void CPlayer::Render_GameObject(void)
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

void CPlayer::Free(void)
{
	Engine::CGameObject::Free();
}

void CPlayer::Player_Mode()
{
	switch (m_PlayerState)
	{
	case IDLE:
		m_pMeshCom->Set_Animation(0);
		break;
	case WALK:
		m_pMeshCom->Set_Animation(6);
		break;
	case ATT_1:
		m_pMeshCom->Set_Animation(1);
		break;
	case ATT_2:
		m_pMeshCom->Set_Animation(2);
		break;
	case ATT_3:
		m_pMeshCom->Set_Animation(3);
		break;
	case ATT_4:
		m_pMeshCom->Set_Animation(4);
		break;
	case ATT_5:
		m_pMeshCom->Set_Animation(5);
		break;
	case RUN:
		m_pMeshCom->Set_Animation(7);
		break;
	case SKILL1:
		m_pMeshCom->Set_Animation(24);
		break;
	case SKILL2:
		m_pMeshCom->Set_Animation(25);
		break;
	case SKILL3:
		m_pMeshCom->Set_Animation(26);
		break;
	case DASH:
		m_pMeshCom->Set_Animation(31);
		break;
	case HIT_FRONT:
		m_pMeshCom->Set_Animation(10);
		break;
	default:
		break;
	}
}

void CPlayer::RuinBlade(_vec3 vLook, _vec3 vPos)
{
	if (!m_bRuinSound)
	{
		Engine::PlaySoundVolume(L"arisha_skill_ruin_blade_encahnt.wav", Engine::CH_PLAYEREFFECT, 0.16f);
		m_bRuinSound = true;
	}
	if (!m_bRuinBladeCheck && m_pMeshCom->m_pAniCtrl->m_fAccTime >= 2.f)
	{
		Engine::PlaySoundVolume(L"arisha_skill_ruin_blade_wave.wav", Engine::CH_PLAYEREFFECT, 0.16f);
		
			m_pRuinBlade = CRuinBlade::Create(m_pGraphicDev, vLook,vPos, m_pTransformCom->m_vAngle.y);
			Engine::Add_GameObject(L"GameLogic", L"RuinBlade", m_pRuinBlade);

		m_bRuinBladeCheck = true;
		
	}
}

void CPlayer::LordOfMana(_vec3 vPos)
{
	if (!m_bLOMHandEffect2Check && m_pMeshCom->m_pAniCtrl->m_fAccTime < 3.f && !m_bLOMHandEffectCheck)
	{

		m_pLOMHandEffect2 = CLordOfManaHandEffect2::Create(m_pGraphicDev, 0);
		Engine::Add_GameObject(L"GameLogic", L"LoMHandEffect2", m_pLOMHandEffect2);
		m_bLOMHandEffect2Check = true;

		m_pLOMHandEffect= CLordOfManaHandEffect::Create(m_pGraphicDev, 0);
		Engine::Add_GameObject(L"GameLogic", L"LoMHandEffect", m_pLOMHandEffect);
		m_bLOMHandEffectCheck = true;

		Engine::PlaySoundVolume(L"arisha_skill_lord_of_mana_begin.wav", Engine::CH_PLAYEREFFECT, 0.15f);

		Engine::PlaySoundVolume(L"arisha_skill_lord_of_mana_during.wav", Engine::CH_PLAYEREFFECT2, 0.15f);
	}
	if (!m_bLOMBotEffectCheck && m_pMeshCom->m_pAniCtrl->m_fAccTime >= 3.f)
	{
		

		m_pLoMBEffect = CLordOfManaBotEffect::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[Engine::INFO_POS]);
		Engine::Add_GameObject(L"GameLogic", L"LoMBotEffect", m_pLoMBEffect);

		m_pLoMBEffect2 = CLordOfManaBotEffect2::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[Engine::INFO_POS]);
		Engine::Add_GameObject(L"GameLogic", L"LoMBotEffect2", m_pLoMBEffect2);

		m_pWhite = CWhite::Create(m_pGraphicDev);
		Engine::Add_GameObject(L"GameLogic", L"White", m_pWhite);
		
		

		m_bLOMBotEffectCheck = true;

		
	}
	


	if (!m_bLordOfManaCheck && m_pMeshCom->m_pAniCtrl->m_fAccTime >= 7.2f)
	{
		Engine::PlaySoundVolume(L"arisha_skill_lord_of_mana_end.wav", Engine::CH_PLAYEREFFECT, 0.15f); 
		m_pLordOfMana = CLordOfMana::Create(m_pGraphicDev, vPos);
		Engine::Add_GameObject(L"GameLogic", L"LordOfMana", m_pLordOfMana);

		m_pWhite = CWhite::Create(m_pGraphicDev);
		Engine::Add_GameObject(L"GameLogic", L"White", m_pWhite);

		m_bLordOfManaCheck = true;

		
		if (!m_bLOMBlueFireEffectCheck)
		{
			for (_uint i = 0; i < 50; ++i)
			{
				m_pLoMBlueFireEffect = CLordOfManaBlueFireEffect::Create(m_pGraphicDev, vPos);
				Engine::Add_GameObject(L"GameLogic", L"LordOfManaBlueFireEffect", m_pLoMBlueFireEffect);
			}
			m_bLOMBlueFireEffectCheck = true;
		}
	}
}

HRESULT CPlayer::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	// Mesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(RESOURCE_STAGE, L"Mesh_Arisha"));
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

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer*	pInstance = new	CPlayer(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CPlayer::Key_Input(const _float& fTimeDelta)
{
	//m_PlayerState = IDLE;
	


	m_fSpeed = 4.f;
	_vec3 vPos, vLook, vRight, vRightLook, vLeftLook,vBackLook;
	vPos = *m_pTransformCom->Get_Info(Engine::INFO_POS);
	vLook = *m_pTransformCom->Get_Info(Engine::INFO_LOOK);
	vRight = *m_pTransformCom->Get_Info(Engine::INFO_RIGHT);
	vBackLook = -vLook;
	vRightLook = vLook + vRight;
	vLeftLook = vLook - vRight;

	D3DXVec3Normalize(&vLook, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);
	D3DXVec3Normalize(&vRightLook, &vRightLook);
	D3DXVec3Normalize(&vLeftLook, &vLeftLook);
		
	//cout <<"플레이어 애니번호:"<<m_PlayerState << endl;
	if (m_PlayerState == DASH)
		m_fSpeed = 7.f;
	if (m_PlayerState == RUN)
		m_fSpeed = 5.f;

	_vec3  vCrossResult;

	

	if (GetAsyncKeyState('W') & 0x8000)
	{
		/*m_pDustEffect = CDustEffect::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[Engine::INFO_POS]);
		Engine::Add_GameObject(L"GameLogic", L"DustEffect", m_pDustEffect);*/

		if (GetAsyncKeyState('A') & 0x8000)
		{
			m_pTransformCom->Set_Pos(&Engine::CManagement::GetInstance()->Get_Navi()->Move_OnNaviMesh(&vPos, &(vLeftLook * m_fSpeed * m_fDynamicTime), &m_NaviIndex));
			
			
			// 캐릭터 회전
			D3DXVec3Cross(&vCrossResult, &vLook, &vLeftLook);
			fDotResult = D3DXVec3Dot(&vLook, &vLeftLook);
			fDotResult = acosf(fDotResult);

			fDotResult = min(fDotResult, 1.f);
			fDotResult = max(fDotResult, -1.f);
			if (vCrossResult.y > 0.f)
				m_pTransformCom->Rotation(Engine::ROT_Y, fDotResult * fTimeDelta * 8.f);
			else
				m_pTransformCom->Rotation(Engine::ROT_Y, fDotResult * fTimeDelta * -8.f);


			if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
			{
				m_PlayerState = RUN;
			}

			else  
			 m_PlayerState = WALK;
		}
		else if (GetAsyncKeyState('D') & 0x8000)
		{
			m_pTransformCom->Set_Pos(&Engine::CManagement::GetInstance()->Get_Navi()->Move_OnNaviMesh(&vPos, &(vRightLook * m_fSpeed * m_fDynamicTime), &m_NaviIndex));
			// 캐릭터 회전
			D3DXVec3Cross(&vCrossResult, &vLook, &vRightLook);
			fDotResult = D3DXVec3Dot(&vLook, &vRightLook);
			fDotResult = acosf(fDotResult);

			fDotResult = min(fDotResult, 1.f);
			fDotResult = max(fDotResult, -1.f);
			if (vCrossResult.y > 0.f)
				m_pTransformCom->Rotation(Engine::ROT_Y, fDotResult * fTimeDelta * 8.f);
			else if(vCrossResult.y <0.f)
				m_pTransformCom->Rotation(Engine::ROT_Y, fDotResult * fTimeDelta * -8.f);

			if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
			{
				m_PlayerState = RUN;
			}
			else
			 m_PlayerState = WALK;

		}
		else
		{
			m_pTransformCom->Set_Pos(&Engine::CManagement::GetInstance()->Get_Navi()->Move_OnNaviMesh(&vPos, &(vLook * m_fSpeed * m_fDynamicTime), &m_NaviIndex));
			// 캐릭터 회전
			/*D3DXVec3Cross(&vCrossResult, &vLook, &vLook);
			fDotResult = D3DXVec3Dot(&vLook, &vLook);
			fDotResult = acosf(fDotResult);

			fDotResult = min(fDotResult, 3.14f);
			fDotResult = max(fDotResult, 0.f);
			if (vCrossResult.y > 0.f)
				m_pTransformCom->Rotation(Engine::ROT_Y, fDotResult * fTimeDelta * 5.f);
			else
				m_pTransformCom->Rotation(Engine::ROT_Y, fDotResult * fTimeDelta * -5.f);*/

			if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
			{
				m_fSpeed = 5.f;
				m_PlayerState = RUN;
			}
			else
			 m_PlayerState = WALK;
		}
	}


	else if (GetAsyncKeyState('S') & 0x8000)
	{
		/*m_pDustEffect = CDustEffect::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[Engine::INFO_POS]);
		Engine::Add_GameObject(L"GameLogic", L"DustEffect", m_pDustEffect);*/

		if (GetAsyncKeyState('A') & 0x8000)
		{
			m_pTransformCom->Set_Pos(&Engine::CManagement::GetInstance()->Get_Navi()->Move_OnNaviMesh(&vPos, &(vRightLook * -m_fSpeed * m_fDynamicTime), &m_NaviIndex));

			// 캐릭터 회전
			D3DXVec3Cross(&vCrossResult, &vLook, &vLeftLook);
			fDotResult = D3DXVec3Dot(&vLook, &vLeftLook);
			fDotResult = acosf(fDotResult);

			fDotResult = min(fDotResult, 1.f);
			fDotResult = max(fDotResult, -1.f);
			if (vCrossResult.y > 0.f)
				m_pTransformCom->Rotation(Engine::ROT_Y, fDotResult * fTimeDelta * 8.f);
			else
				m_pTransformCom->Rotation(Engine::ROT_Y, fDotResult * fTimeDelta * -8.f);

			if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
			{
				m_fSpeed = 5.f;
				m_PlayerState = RUN;
			}
			else
			m_PlayerState = WALK;
		}
		else if (GetAsyncKeyState('D') & 0x8000)
		{
			m_pTransformCom->Set_Pos(&Engine::CManagement::GetInstance()->Get_Navi()->Move_OnNaviMesh(&vPos, &(vRightLook * -m_fSpeed * m_fDynamicTime), &m_NaviIndex));
			// 캐릭터 회전
			D3DXVec3Cross(&vCrossResult, &vLook, &vRightLook);
			fDotResult = D3DXVec3Dot(&vLook, &vRightLook);
			fDotResult = acosf(fDotResult);

			fDotResult = min(fDotResult, 1.f);
			fDotResult = max(fDotResult, -1.f);
			if (vCrossResult.y > 0.f)
				m_pTransformCom->Rotation(Engine::ROT_Y, fDotResult * fTimeDelta * 8.f);
			else
				m_pTransformCom->Rotation(Engine::ROT_Y, fDotResult * fTimeDelta * -8.f);

			if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
			{
				m_fSpeed = 5.f;
				m_PlayerState = RUN;
			}
			else
			m_PlayerState = WALK;
		}
		else
		{
			m_pTransformCom->Set_Pos(&Engine::CManagement::GetInstance()->Get_Navi()->Move_OnNaviMesh(&vPos, &(vLook * -m_fSpeed * m_fDynamicTime), &m_NaviIndex));

			// 캐릭터 회전
			D3DXVec3Cross(&vCrossResult, &vLook, &vBackLook);
			fDotResult = D3DXVec3Dot(&vLook, &vBackLook);
			fDotResult = acosf(fDotResult);

			fDotResult = min(fDotResult, 1.f);
			fDotResult = max(fDotResult, -1.f);
			if (vCrossResult.y > 0.f)
				m_pTransformCom->Rotation(Engine::ROT_Y, fDotResult * fTimeDelta * 5.f);
			else
				m_pTransformCom->Rotation(Engine::ROT_Y, fDotResult * fTimeDelta * -5.f);

			if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
			{
				m_fSpeed = 5.f;
				m_PlayerState = RUN;
			}
			else
			m_PlayerState = WALK;
		}
	}

	else if (GetAsyncKeyState('A') & 0x8000)
	{
		/*m_pDustEffect = CDustEffect::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[Engine::INFO_POS]);
		Engine::Add_GameObject(L"GameLogic", L"DustEffect", m_pDustEffect);*/

		m_pTransformCom->Set_Pos(&Engine::CManagement::GetInstance()->Get_Navi()->Move_OnNaviMesh(&vPos, &(vRight * -m_fSpeed * m_fDynamicTime), &m_NaviIndex));
		// 캐릭터 회전
		D3DXVec3Cross(&vCrossResult, &vLook, &vLeftLook);
		fDotResult = D3DXVec3Dot(&vLook, &vLeftLook);
		fDotResult = acosf(fDotResult);

		fDotResult = min(fDotResult, 3.14f);
		fDotResult = max(fDotResult, 0.f);
		if (vCrossResult.y > 0.f)
			m_pTransformCom->Rotation(Engine::ROT_Y, fDotResult * fTimeDelta * 8.f);
		else
			m_pTransformCom->Rotation(Engine::ROT_Y, fDotResult * fTimeDelta * -8.f);
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
		{
			m_fSpeed = 5.f;
			m_PlayerState = RUN;
		}
		else
	
		m_PlayerState = WALK;
	}

	else if (GetAsyncKeyState('D') & 0x8000)
	{
		/*m_pDustEffect = CDustEffect::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[Engine::INFO_POS]);
		Engine::Add_GameObject(L"GameLogic", L"DustEffect", m_pDustEffect);*/

		m_pTransformCom->Set_Pos(&Engine::CManagement::GetInstance()->Get_Navi()->Move_OnNaviMesh(&vPos, &(vRight * m_fSpeed * m_fDynamicTime), &m_NaviIndex));
		// 캐릭터 회전
		D3DXVec3Cross(&vCrossResult, &vLook, &vRightLook);
		fDotResult = D3DXVec3Dot(&vLook, &vRightLook);
		fDotResult = acosf(fDotResult);

		fDotResult = min(fDotResult, 3.14f);
		fDotResult = max(fDotResult, 0.f);
		if (vCrossResult.y > 0.f)
			m_pTransformCom->Rotation(Engine::ROT_Y, fDotResult * fTimeDelta * 8.f);
		else
			m_pTransformCom->Rotation(Engine::ROT_Y, fDotResult * fTimeDelta * -8.f);

		if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
		{
			m_fSpeed = 5.f;
			m_PlayerState = RUN;
		}
		else
		
		m_PlayerState = WALK;
	}

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		/*m_pDustEffect = CDustEffect::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[Engine::INFO_POS]);
		Engine::Add_GameObject(L"GameLogic", L"DustEffect", m_pDustEffect);*/

		m_pTransformCom->Set_Pos(&Engine::CManagement::GetInstance()->Get_Navi()->Move_OnNaviMesh(&vPos, &(vLook * m_fSpeed * m_fDynamicTime), &m_NaviIndex));
		m_PlayerState = DASH;
		if (!m_bDashEffectCheck)
		{
			m_pEffect = CEffect::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[Engine::INFO_POS], m_pTransformCom->m_vAngle.y);
			Engine::Add_GameObject(L"GameLogic", L"Effect", m_pEffect);
			
			m_bDashEffectCheck=true;
			Engine::PlaySoundVolume(L"arisha_skill_speedy_move.wav", Engine::CH_PLAYEREFFECT, 0.08f);
		}
		
	}



//	//=============================================================================================공격======================================================================//
	else if (m_pKeyMgr->KeyDown(Engine::KEY_LBUTTON))
	{
		m_pSwordEffect = CSwordEffect::Create(m_pGraphicDev, *m_pTransformCom->Get_Info(Engine::INFO_POS));
		Engine::Add_GameObject(L"GameLogic", L"SwordEffect", m_pSwordEffect);

		//cout << "어택카운터:" << m_iAttCnt << endl;

		m_pTransformCom->Set_Pos(&Engine::CManagement::GetInstance()->Get_Navi()->Move_OnNaviMesh(&vPos, &(vLook * 0.4 * m_fDynamicTime), &m_NaviIndex));
		if (m_iAttCnt == 0)
		{
			m_PlayerState = ATT_1;
			m_iAttCnt += 1;
			Engine::PlaySoundVolume(L"arisha_attack_01.wav", Engine::CH_PLAYEREFFECT, 0.2f);
			

			
		}
		else if (m_iAttCnt == 1)
		{
			m_PlayerState = ATT_2;
			m_iAttCnt += 1;
			Engine::PlaySoundVolume(L"arisha_attack_02.wav", Engine::CH_PLAYEREFFECT, 0.2f);
			
		}
		else if (m_iAttCnt == 2)
		{
			m_PlayerState = ATT_3;
			m_iAttCnt += 1;
			Engine::PlaySoundVolume(L"arisha_attack_03.wav", Engine::CH_PLAYEREFFECT, 0.08f);
			
		}
		else if (m_iAttCnt == 3)
		{
			m_PlayerState = ATT_4;
			m_iAttCnt += 1;
			Engine::PlaySoundVolume(L"arisha_attack_04.wav", Engine::CH_PLAYEREFFECT, 0.2f);
			
		}
		else if (m_iAttCnt == 4)
		{
			m_PlayerState = ATT_5;
			m_iAttCnt = 0;
			Engine::PlaySoundVolume(L"arisha_attack_05.wav", Engine::CH_PLAYEREFFECT, 0.2f);
			
		}
			 
			
	}


else if (GetAsyncKeyState('1') & 0x8000)
{
	m_PlayerState = SKILL1;
}
else if (GetAsyncKeyState('2') & 0x8000)
{
	m_PlayerState = SKILL2;
		RuinBlade(vLook,vPos);
	
}
else if (GetAsyncKeyState('3') & 0x8000)
{
	m_PlayerState = SKILL3;
	LordOfMana(vPos);
}
	
	

else if ((GetAsyncKeyState('F') & 0x8000))
{
	if (m_iKillMonCnt>=2)
	{
		
		//NpcUi
		m_pNpcUi = CNpcUI::Create(m_pGraphicDev);
		Engine::Add_GameObject(L"GameLogic", L"NpcUI", m_pNpcUi);

		//TextUI2
		m_pTextUi2 = CTextUI2::Create(m_pGraphicDev);
		Engine::Add_GameObject(L"GameLogic", L"TextUI2", m_pTextUi2);

		m_bQuestClear = true;
	}
	
	else
	{
		//NpcUi
		m_pNpcUi = CNpcUI::Create(m_pGraphicDev);
		Engine::Add_GameObject(L"GameLogic", L"NpcUI", m_pNpcUi);

		//TextUI
		m_pTextUi = CTextUI::Create(m_pGraphicDev);
		Engine::Add_GameObject(L"GameLogic", L"TextUI", m_pTextUi);

	}
	
}

else if (m_bQuestClear && !m_bCreateCanon)

{
	/*m_pCanon=CCanon::Create(m_pGraphicDev);
	Engine::Add_GameObject(L"GameLogic", L"Canon", m_pCanon);*/

	m_pBoss = CBoss::Create(m_pGraphicDev);
	Engine::Add_GameObject(L"GameLogic", L"Boss", m_pBoss);

	m_bCreateCanon = true;
}


else if (m_PlayerState == HIT_FRONT)
{

	

	if (!m_HitCheck)
	{
		Engine::PlaySoundVolume(L"아리샤hit.mp3", Engine::CH_PLAYERHIT, 0.3f);
		m_pLowHp = CLowHp::Create(m_pGraphicDev);
		Engine::Add_GameObject(L"GameLogic", L"LowHp", m_pLowHp);

		m_pHitEffect = CHitEffect::Create(m_pGraphicDev, m_pTransformCom->m_vInfo[Engine::INFO_POS]);
		Engine::Add_GameObject(L"GameLogic", L"HitEffect", m_pHitEffect);

		m_pTransformCom->Set_Pos(&Engine::CManagement::GetInstance()->Get_Navi()->Move_OnNaviMesh(&vPos, &(vLook * -2 * m_fDynamicTime), &m_NaviIndex));

		m_HitCheck = true;
	}


}



	else if (true == m_pMeshCom->Is_AnimationEnd())
	{
		m_PlayerState = IDLE;
		m_iAttCnt = 0;
	}
	

	

}





HRESULT CPlayer::SetUp_ConstantTable(LPD3DXEFFECT& pEffect)
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
	tMtrlInfo.Emissive = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
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

