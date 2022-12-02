#ifndef CollisionMgr_h__
#define CollisionMgr_h__

#include "Defines.h"
#include "GameObject.h"
#include"Player.h"
#include"Sword.h"
#include"SwordEffect.h"
#include"Dog.h"
#include"Soldier.h"
#include"Knight.h"
#include"Guarder.h"
#include"Boss.h"
#include"Bomb.h"
#include"Fountain.h"
#include"RuinBlade.h"
#include"LordOfMana.h"



BEGIN(Engine)

class CCollider;

END
class CPlayer;
class CSwordEffect;
class CDog;
class CSoldier;
class CGuarder;
class CKnight;
class CBoss;
class CBomb;
class CFountain;
class CRuinBlade;
class CLordOfMana;


class CCollisionMgr
{
	DECLARE_SINGLETON(CCollisionMgr)
private:
	CCollisionMgr();
public:
	virtual ~CCollisionMgr();

public:
	//static vs Dynamic
	void Collision_SwordEffectToDog(CSwordEffect* pSword, CDog* pDog);
	void Collision_SwordEffectToSoldier(CSwordEffect* pSword, CSoldier* pSoldier);
	void Collision_SwordEffectToBoss(CSwordEffect* pSword, CBoss* pBoss);
	//void Collision_BombToBoss(CBomb* pBomb, CBoss* pBoss);
	void Collision_FountainToBoss(CFountain* pFountain, CBoss* pBoss);
	void Collision_SwordEffectToKnight(CSwordEffect* pSword, CKnight* pKnight);
	void Collision_SwordEffectToGuarder(CSwordEffect* pSword, CGuarder* pGuarder);

	void CollisionRuinBladeToDog(CRuinBlade* pRuin, CDog* pDog);
	void CollisionRuinBladeToSoldier(CRuinBlade* pRuin, CSoldier* pSoldier);
	void CollisionRuinBladeToBoss(CRuinBlade* pRuin, CBoss* pBoss);
	void Collision_RuinBladeToKnight(CRuinBlade* pSword, CKnight* pKnight);
	void Collision_RuinBladeToGuarder(CRuinBlade* pSword, CGuarder* pGuarder);


	void CollisionLordOfManaToDog(CLordOfMana* PLord, CDog* pDog);
	void CollisionLordOfManaToSoldier(CLordOfMana* PLord, CSoldier* pSoldier);
	void CollisionLordOfManaToBoss(CLordOfMana* PLord, CBoss* pBoss);
	void Collision_LordOfManaToKnight(CLordOfMana* pSword, CKnight* pKnight);
	void Collision_LordOfManaToGuarder(CLordOfMana* pSword, CGuarder* pGuarder);

	//Dynamic vs Dynamic
	void Collision_PlayerToDog(CPlayer* pPlayer, CDog* pDog);
	void Collision_PlayerToSoldier(CPlayer* pPlayer, CSoldier* pSoldier);
	void Collision_PlayerToBoss(CPlayer* pPlayer, CBoss* pBoss);
	void Collision_PlayerToKnight(CPlayer* pSword, CKnight* pKnight);
	void Collision_PlayerToGuarder(CPlayer* pSword, CGuarder* pGuarder);

private:
	bool Check_Sphere(Engine::CCollider* Col1, Engine::CCollider* Col2);
	bool Check_SphereStatic(Engine::CCollider* Col1, Engine::CCollider* Col2);
	bool Check_SphereStatic(Engine::CCollider2* Col1, Engine::CCollider* Col2);


private:
	CPlayer* m_pPlayer=nullptr;
	


	
};


#endif // CollisionMgr_h__
