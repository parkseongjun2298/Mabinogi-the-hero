#include "stdafx.h"
#include "CollisionMgr.h"

#include "Export_Function.h"

IMPLEMENT_SINGLETON(CCollisionMgr)
CCollisionMgr::CCollisionMgr()
{
	
}

CCollisionMgr::~CCollisionMgr()
{

}

void CCollisionMgr::Collision_SwordEffectToDog(CSwordEffect * pSword, CDog * pDog)
{

	if (pDog->m_bCheckDead)
		return;

	if (Check_Sphere(pSword->Get_ColliderCom(), pDog->Get_ColliderCom()))
	{
		//cout << "================================================칼과개충돌함!==========================================" << endl;
		
		pDog->m_DogState = DOG_KNOCKBACK;
		//개피 깍이고 쫓아오는 bool값 키기
		pDog->m_bFollowPlayer = true;
		pDog->m_fDogHp -= 1.f;
		
		if (!pDog->m_bColCheck)
		
			Engine::PlaySoundVolume(L"sword_hit2.wav", Engine::CH_HITEFFECT, 0.2f);
			pDog->m_bColCheck = true;
		}

		for (auto& Player : Engine::CManagement::GetInstance()->Get_Obj(L"Player"))
		{
			m_pPlayer = dynamic_cast<CPlayer*>(Player);
		};

		if (pDog->m_fDogHp<=0.f)
		{
			pDog->Set_bIsDead();
			
		}



	}
	


void CCollisionMgr::Collision_SwordEffectToSoldier(CSwordEffect * pSword, CSoldier * pSoldier)
{
	if (pSoldier->m_bCheckDead)
		return;

	if (Check_Sphere(pSword->Get_ColliderCom(), pSoldier->Get_ColliderCom()))
	{
		//cout << "================================================칼과기사충돌함!==========================================" << endl;
		
		pSoldier->m_SoldierState = SOLDIER_KNOCKBACK;
		//솔져피 깍이고 쫓아오는 bool값 키기
		pSoldier->m_bFollowPlayer = true;
		pSoldier->m_fSoldierHp -= 1.f;
		
		
		for (auto& Player : Engine::CManagement::GetInstance()->Get_Obj(L"Player"))
		{
			m_pPlayer = dynamic_cast<CPlayer*>(Player);
		};

		if (pSoldier->m_fSoldierHp <= 0.f)
		{
			pSoldier->Set_bIsDead();
		}
	}
}

void CCollisionMgr::Collision_SwordEffectToBoss(CSwordEffect * pSword, CBoss * pBoss)
{
	if (pBoss->m_bCheckDead)
		return;

	if (Check_Sphere(pSword->Get_ColliderCom(), pBoss->Get_ColliderCom()))
	{
		//cout << "================================================칼과보스충돌함!==========================================" << endl;
		if (!pBoss->m_bColCheck)
		{
			Engine::PlaySoundVolume(L"sword_hit2.wav", Engine::CH_HITEFFECT, 0.2f);
			pBoss->m_bColCheck = true;
		}
		
		if (pBoss->m_pBossHpGreen == nullptr)
			return;
		
		if (pBoss->m_fHpCount == 3)
			pBoss->m_pBossHpGreen->m_fSizeX -= 50.f;
		if (pBoss->m_fHpCount == 2)
			pBoss->m_pBossHpYellow->m_fSizeX -= 50.f;
		if (pBoss->m_fHpCount == 1)
			pBoss->m_pBossHpRed->m_fSizeX -= 50.f;
		
		
		
	}
}


void CCollisionMgr::Collision_FountainToBoss(CFountain * pFountain, CBoss * pBoss)
{
	if (pBoss->m_bCheckDead)
		return;

	if (Check_SphereStatic(pFountain->Get_ColliderCom(), pBoss->Get_ColliderCom()))
	{
		//cout << "================================================분수대와보스충돌함!==========================================" << endl;


		pBoss->m_BossState = BOSS_PIERCE;

		if(pBoss->m_pMeshCom->m_pAniCtrl->m_fAccTime >= 1.5f && pBoss->m_pMeshCom->m_pAniCtrl->m_fAccTime < 1.9f)
			pFountain->Set_bIsDead();


	}
}

void CCollisionMgr::Collision_SwordEffectToKnight(CSwordEffect * pSword, CKnight * pKnight)
{
	if (pKnight->m_bCheckDead)
		return;

	if (Check_Sphere(pSword->Get_ColliderCom(), pKnight->Get_ColliderCom()))
	{
		//cout << "================================================칼과기사충돌함!==========================================" << endl;
		
		pKnight->m_KnightState = Knight_KNOCKBACK;
		//솔져피 깍이고 쫓아오는 bool값 키기
		
		pKnight->m_bFollowPlayer = true;
		pKnight->m_fKnightHp -= 1.f;


		for (auto& Player : Engine::CManagement::GetInstance()->Get_Obj(L"Player"))
		{
			m_pPlayer = dynamic_cast<CPlayer*>(Player);
		};

		if (pKnight->m_fKnightHp <= 0.f)
		{
			pKnight->Set_bIsDead();
		}
	}
}

void CCollisionMgr::Collision_SwordEffectToGuarder(CSwordEffect * pSword, CGuarder * pGuarder)
{
	if (pGuarder->m_bCheckDead)
		return;

	if (Check_Sphere(pSword->Get_ColliderCom(), pGuarder->Get_ColliderCom()))
	{
		//cout << "================================================칼과기사충돌함!==========================================" << endl;
		
		pGuarder->m_GuarderState = Guarder_KNOCKBACK;
		//솔져피 깍이고 쫓아오는 bool값 키기
		pGuarder->m_bFollowPlayer = true;
		pGuarder->m_fGuarderHp -= 1.f;


		for (auto& Player : Engine::CManagement::GetInstance()->Get_Obj(L"Player"))
		{
			m_pPlayer = dynamic_cast<CPlayer*>(Player);
		};

		if (pGuarder->m_fGuarderHp <= 0.f)
		{
			pGuarder->Set_bIsDead();
		}
	}
}

void CCollisionMgr::CollisionRuinBladeToDog(CRuinBlade * pRuin, CDog * pDog)
{
	if (pDog->m_bCheckDead)
		return;

	if (Check_Sphere(pRuin->Get_ColliderCom(), pDog->Get_ColliderCom()))
	{
	//	cout << "================================================루인블레이드와개충돌함!==========================================" << endl;
		pDog->m_DogState = DOG_KNOCKBACK;
		//개피 깍이고 쫓아오는 bool값 키기
		pDog->m_bFollowPlayer = true;
		pDog->m_fDogHp -= 20.f;

		for (auto& Player : Engine::CManagement::GetInstance()->Get_Obj(L"Player"))
		{
			m_pPlayer = dynamic_cast<CPlayer*>(Player);
		};

		if (pDog->m_fDogHp <= 0.f)
		{
			pDog->Set_bIsDead();
		}
	}
}

void CCollisionMgr::CollisionRuinBladeToSoldier(CRuinBlade * pRuin, CSoldier * pSoldier)
{
	if (pSoldier->m_bCheckDead)
		return;

	if (Check_Sphere(pRuin->Get_ColliderCom(), pSoldier->Get_ColliderCom()))
	{
		//cout << "================================================루인블레이드와기사충돌함!==========================================" << endl;
		pSoldier->m_SoldierState = SOLDIER_KNOCKBACK;
		//솔져피 깍이고 쫓아오는 bool값 키기
		pSoldier->m_bFollowPlayer = true;
		pSoldier->m_fSoldierHp -= 20.f;


		for (auto& Player : Engine::CManagement::GetInstance()->Get_Obj(L"Player"))
		{
			m_pPlayer = dynamic_cast<CPlayer*>(Player);
		};

		if (pSoldier->m_fSoldierHp <= 0.f)
		{
			pSoldier->Set_bIsDead();
		}
	}
}

void CCollisionMgr::CollisionRuinBladeToBoss(CRuinBlade * pRuin, CBoss * pBoss)
{
	if (pBoss->m_bCheckDead)
		return;

	if (Check_Sphere(pRuin->Get_ColliderCom(), pBoss->Get_ColliderCom()))
	{
		//cout << "================================================루인블레이드와보스충돌함!==========================================" << endl;
		
		
		if (!pBoss->m_bColCheck)
		{
		//	Engine::PlaySoundVolume(L"sword_hit2.wav", Engine::CH_HITEFFECT, 0.2f);
			pBoss->m_bColCheck = true;
		}
		//pBoss->m_BossState = BOSS_DAMAGE_WEAK_BOT;  //일단 주석
		if (pBoss ->m_pBossHpGreen==nullptr)
			return;

		if (pBoss->m_fHpCount == 3)
			pBoss->m_pBossHpGreen->m_fSizeX -= 20.f;
		if (pBoss->m_fHpCount == 2)
			pBoss->m_pBossHpYellow->m_fSizeX -= 20.f;
		if (pBoss->m_fHpCount == 1)
			pBoss->m_pBossHpRed->m_fSizeX -= 20.f;

		//pBoss->Set_bIsDead();


	}
}

void CCollisionMgr::Collision_RuinBladeToKnight(CRuinBlade * pSword, CKnight * pKnight)
{
	if (pKnight->m_bCheckDead)
		return;

	if (Check_Sphere(pSword->Get_ColliderCom(), pKnight->Get_ColliderCom()))
	{
		//cout << "================================================루인블레이드와기사충돌함!==========================================" << endl;
		pKnight->m_KnightState = Knight_KNOCKBACK;
		//솔져피 깍이고 쫓아오는 bool값 키기
		pKnight->m_bFollowPlayer = true;
		pKnight->m_fKnightHp -= 2.f;


		for (auto& Player : Engine::CManagement::GetInstance()->Get_Obj(L"Player"))
		{
			m_pPlayer = dynamic_cast<CPlayer*>(Player);
		};

		if (pKnight->m_fKnightHp <= 0.f)
		{
			pKnight->Set_bIsDead();
		}
	}
}

void CCollisionMgr::Collision_RuinBladeToGuarder(CRuinBlade * pSword, CGuarder * pGuarder)
{
	if (pGuarder->m_bCheckDead)
		return;

	if (Check_Sphere(pSword->Get_ColliderCom(), pGuarder->Get_ColliderCom()))
	{
		//cout << "================================================루인블레이드와기사충돌함!==========================================" << endl;
		pGuarder->m_GuarderState = Guarder_KNOCKBACK;
		//솔져피 깍이고 쫓아오는 bool값 키기
		pGuarder->m_bFollowPlayer = true;
		pGuarder->m_fGuarderHp -= 2.f;


		for (auto& Player : Engine::CManagement::GetInstance()->Get_Obj(L"Player"))
		{
			m_pPlayer = dynamic_cast<CPlayer*>(Player);
		};

		if (pGuarder->m_fGuarderHp <= 0.f)
		{
			pGuarder->Set_bIsDead();
		}
	}
}

void CCollisionMgr::CollisionLordOfManaToDog(CLordOfMana * PLord, CDog * pDog)
{
	if (pDog->m_bCheckDead)
		return;

	if (Check_Sphere(PLord->Get_ColliderCom(), pDog->Get_ColliderCom()))
	{
		//cout << "================================================로드오브마나와개충돌함!==========================================" << endl;
		pDog->m_DogState = DOG_KNOCKBACK;
		//개피 깍이고 쫓아오는 bool값 키기
		pDog->m_bFollowPlayer = true;
		pDog->m_fDogHp -= 10.f;

		for (auto& Player : Engine::CManagement::GetInstance()->Get_Obj(L"Player"))
		{
			m_pPlayer = dynamic_cast<CPlayer*>(Player);
		};

		if (pDog->m_fDogHp <= 0.f)
		{
			pDog->Set_bIsDead();
			
		}
	}
}

void CCollisionMgr::CollisionLordOfManaToSoldier(CLordOfMana * PLord, CSoldier * pSoldier)
{
	if (pSoldier->m_bCheckDead)
		return;

	if (Check_Sphere(PLord->Get_ColliderCom(), pSoldier->Get_ColliderCom()))
	{
		//cout << "================================================로드오브마나와기사충돌함!==========================================" << endl;
		pSoldier->m_SoldierState = SOLDIER_KNOCKBACK;
		//솔져피 깍이고 쫓아오는 bool값 키기
		pSoldier->m_bFollowPlayer = true;
		pSoldier->m_fSoldierHp -= 10.f;


		for (auto& Player : Engine::CManagement::GetInstance()->Get_Obj(L"Player"))
		{
			m_pPlayer = dynamic_cast<CPlayer*>(Player);
		};

		if (pSoldier->m_fSoldierHp <= 0.f)
		{
			pSoldier->Set_bIsDead();
			
		}
	}
}

void CCollisionMgr::CollisionLordOfManaToBoss(CLordOfMana * PLord, CBoss * pBoss)
{
	if (pBoss->m_bCheckDead)
		return;

	if (Check_Sphere(PLord->Get_ColliderCom(), pBoss->Get_ColliderCom()))
	{
		//cout << "================================================로드오브마나와보스충돌함!==========================================" << endl;

		if (!pBoss->m_bColCheck)
		{
			//Engine::PlaySoundVolume(L"sword_hit2.wav", Engine::CH_HITEFFECT, 0.2f);
			pBoss->m_bColCheck = true;
		}

		//pBoss->m_BossState = BOSS_DAMAGE_WEAK_BOT;  //일단 주석
		if (pBoss->m_pBossHpGreen == nullptr)
			return;

		if (pBoss->m_fHpCount == 3)
			pBoss->m_pBossHpGreen->m_fSizeX -= 20.f;
		if (pBoss->m_fHpCount == 2)
			pBoss->m_pBossHpYellow->m_fSizeX -= 20.f;
		if (pBoss->m_fHpCount == 1)
			pBoss->m_pBossHpRed->m_fSizeX -= 20.f;

		//pBoss->Set_bIsDead();


	}
}

void CCollisionMgr::Collision_LordOfManaToKnight(CLordOfMana * pSword, CKnight * pKnight)
{
	if (pKnight->m_bCheckDead)
		return;

	if (Check_Sphere(pSword->Get_ColliderCom(), pKnight->Get_ColliderCom()))
	{
		//cout << "================================================루인블레이드와기사충돌함!==========================================" << endl;
		pKnight->m_KnightState = Knight_KNOCKBACK;
		//솔져피 깍이고 쫓아오는 bool값 키기
		pKnight->m_bFollowPlayer = true;
		pKnight->m_fKnightHp -= 10.f;


		for (auto& Player : Engine::CManagement::GetInstance()->Get_Obj(L"Player"))
		{
			m_pPlayer = dynamic_cast<CPlayer*>(Player);
		};

		if (pKnight->m_fKnightHp <= 0.f)
		{
			pKnight->Set_bIsDead();
		}
	}
}

void CCollisionMgr::Collision_LordOfManaToGuarder(CLordOfMana * pSword, CGuarder * pGuarder)
{
	if (pGuarder->m_bCheckDead)
		return;

	if (Check_Sphere(pSword->Get_ColliderCom(), pGuarder->Get_ColliderCom()))
	{
		//cout << "================================================루인블레이드와기사충돌함!==========================================" << endl;
		pGuarder->m_GuarderState = Guarder_KNOCKBACK;
		//솔져피 깍이고 쫓아오는 bool값 키기
		pGuarder->m_bFollowPlayer = true;
		pGuarder->m_fGuarderHp -= 10.f;


		for (auto& Player : Engine::CManagement::GetInstance()->Get_Obj(L"Player"))
		{
			m_pPlayer = dynamic_cast<CPlayer*>(Player);
		};

		if (pGuarder->m_fGuarderHp <= 0.f)
		{
			pGuarder->Set_bIsDead();
		}
	}
}

void CCollisionMgr::Collision_PlayerToDog(CPlayer * pPlayer, CDog * pDog)
{
	if (pDog->m_bCheckDead)
		return;

	if (Check_Sphere(pPlayer->Get_ColliderCom(), pDog->Get_ColliderCom()))
	{
		//cout << "================================================플레이어와개충돌함!==========================================" << endl;
		//pDog->m_bColCheck = true;
		pPlayer->m_PlayerState = HIT_FRONT;
		pPlayer->m_pPlayerHp->m_fSizeX -= 1.f;
	}
	/*else
		pDog->m_bColCheck = false;*/
}

void CCollisionMgr::Collision_PlayerToSoldier(CPlayer * pPlayer, CSoldier * pSoldier)
{
	if (pSoldier->m_bCheckDead)
		return;

	if (Check_Sphere(pPlayer->Get_ColliderCom(), pSoldier->Get_ColliderCom()))
	{
		//cout << "================================================플레이어와기사충돌함!==========================================" << endl;
		//pSoldier->m_bColCheck = true;
		pPlayer->m_PlayerState = HIT_FRONT;
		pPlayer->m_pPlayerHp->m_fSizeX -= 1.f;
	}
	/*else
		pSoldier->m_bColCheck = false;*/
}

void CCollisionMgr::Collision_PlayerToBoss(CPlayer * pPlayer, CBoss * pBoss)
{
	if (pBoss->m_bCheckDead)
		return;

	if (Check_Sphere(pPlayer->Get_ColliderCom(), pBoss->Get_ColliderCom()))
	{
		//cout << "================================================플레이어와보스충돌함!==========================================" << endl;
		

		pPlayer->m_PlayerState = HIT_FRONT;
		pPlayer->m_pPlayerHp->m_fSizeX -= 1.f;
		
	}
	else
		pBoss->m_bColCheck = false;
}

void CCollisionMgr::Collision_PlayerToKnight(CPlayer * pSword, CKnight * pKnight)
{
	if (pKnight->m_bCheckDead)
		return;

	if (Check_Sphere(pSword->Get_ColliderCom(), pKnight->Get_ColliderCom()))
	{
		//cout << "================================================플레이어와기사충돌함!==========================================" << endl;
		//pSoldier->m_bColCheck = true;
		pSword->m_PlayerState = HIT_FRONT;
		pSword->m_pPlayerHp->m_fSizeX -= 1.f;
	}
}

void CCollisionMgr::Collision_PlayerToGuarder(CPlayer * pSword, CGuarder * pGuarder)
{
	if (pGuarder->m_bCheckDead)
		return;

	if (Check_Sphere(pSword->Get_ColliderCom(), pGuarder->Get_ColliderCom()))
	{
		//cout << "================================================플레이어와기사충돌함!==========================================" << endl;
		//pSoldier->m_bColCheck = true;
		pSword->m_PlayerState = HIT_FRONT;
		pSword->m_pPlayerHp->m_fSizeX -= 1.f;
	}
}

bool CCollisionMgr::Check_Sphere(Engine::CCollider * Col1, Engine::CCollider * Col2)
{

	if (Col2 == nullptr) 
		return false;

	float fDist;
	
	fDist = D3DXVec3Length(&(Col1->m_vCenter - Col2->m_vCenter));

	if (fDist <= Col1->m_fScale + Col2->m_fScale )
		return true;

	return false;
}

bool CCollisionMgr::Check_SphereStatic(Engine::CCollider * Col1, Engine::CCollider * Col2)
{
	if (Col2 == nullptr)
		return false;

	float fDist;
	_vec3 Temp = { Col1->Get_CollMatrix()->_41 ,Col1->Get_CollMatrix()->_42 ,Col1->Get_CollMatrix()->_43 };
	fDist = D3DXVec3Length(&(Temp - Col2->m_vCenter));

	if (fDist <= Col1->m_fScale + Col2->m_fScale)
		return true;

	return false;
}

bool CCollisionMgr::Check_SphereStatic(Engine::CCollider2 * Col1, Engine::CCollider * Col2)
{
	if (Col2  == nullptr) 
		return false;

	/*float fDist;
	_vec3 Temp = { Col1->Get_CollMatrix()->_41 ,Col1->Get_CollMatrix()->_42 ,Col1->Get_CollMatrix()->_43 };
	fDist = D3DXVec3Length(&(Temp - Col2->m_vCenter));

	if (fDist <= Col1->m_fScale + Col2->m_fScale)
		return true;*/

	return false;
}


