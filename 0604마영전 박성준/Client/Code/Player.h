#ifndef Player_h__
#define Player_h__

#include "Defines.h"
#include "GameObject.h"

#include"NpcUI.h"
#include"TextUI.h"
#include"TextUI2.h"
#include"PlayerHp.h"
#include"PlayerSp.h"
#include"PlayerHpBack.h"
#include"PlayerSpBack.h"

#include"Dog.h"
#include"Soldier.h"
//#include"Canon.h"
#include"Boss.h"
#include"CollisionMgr.h"

#include"Effect.h"
#include"LordOfManaBotEffect.h"
#include"LordOfManaBotEffect2.h"
#include"LordOfManaBlueFireEffect.h"
#include"White.h"
#include"RuinBlade.h"
#include"LordOfMana.h"
#include"LordOfManaHandEffect.h"
#include"LordOfManaHandEffect2.h"
#include"HitEffect.h"
#include"LowHp.h"
#include"DustEffect.h"
#include"PlayerSkillBar.h"
#include"PlayerSkill1.h"
#include"PlayerSkill2.h"

#include"SwordEffect.h"

enum PlayerState
{
	IDLE,
	ATT_1,
	ATT_2,
	ATT_3,
	ATT_4,
	ATT_5,
	WALK,
	RUN,
	STOPRUN,
	HIT_BACK,
	HIT_FRONT,
	HIT_LEFT,
	HIT_RIGHT,
	AIRBON_1,
	AIRBON_2,
	AIRBON_3,//123하나로동작
	DIE,
	DOWN_RESIST, //17
	MB1,
	MB2,
	MB3,
	MB4, 
	MB5,
	MB6,
	SKILL1,//공던지기
	SKILL2,
	SKILL3,
	HITBYATT, //공격시 맞을때
	DARKNIGHT1,
	DARKNIGHT2,
	DARKNIGHT3,
	DASH,
	MANABLADE,
	NORMALBLADE,//33

	PLAYER_END
};

BEGIN(Engine)

class CDynamicMesh;
class CCalculator;
class CCollider;
class CShader;
class CKeyMgr;
END
class CPlayerSp;
class CPlayerHp;
class CPlayerSpBack;
class CPlayerHpBack;
class CNpcUI;

class CCTextUI;
class CCTextUI2;
//class CCanon;
class CBoss;
class CEffect;
class CLordOfManaBotEffect;
class CLordOfManaBotEffect2;

class CSwordEffect;
class CPlayer : public Engine::CGameObject
{
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayer(void);

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT		Add_Component(void);
	void		Key_Input(const _float& fTimeDelta);
	Engine::CKeyMgr*	 m_pKeyMgr;
	
	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT& pEffect);
public:
	Engine::CCollider* Get_ColliderCom() { return m_pColliderCom; }
	
	Engine::CDynamicMesh*	m_pMeshCom = nullptr;
private:
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CCollider*		m_pColliderCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;
	
	_vec3					m_vPos;
	_vec3					m_vDirection;
	_float					m_fSpeed = 4.f;
public:
	_float					m_fRadius = 0.f;
	_float					m_fRight = 0.f;
	bool					m_bMission = false;
public:
	static CPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;
private:
	void Player_Mode();
	void RuinBlade(_vec3 vLook, _vec3 vPos);
	void LordOfMana(_vec3 vPos);
public:
	PlayerState m_PlayerState = IDLE;
	int m_iKillMonCnt = 0;
	bool m_bQuestClear = false;
	_ulong Save_Navi_Index;
	int m_iAttCnt = 0;
	/*bool m_bColCheck = false;
	float m_fColCheck;*/
private:
	bool m_bRightSpinCheck = false;
	bool m_bLeftSpinCheck = false;
	bool m_bFrontSpinCheck = false;
	bool m_bBackSpinCheck = false;
	
	bool m_bRuinSound = false;
	

	
	bool m_bDash = false;

	bool m_bCreateCanon = false;
	bool m_bPlayerHpUi = false;
	bool m_bPlayerSpUi = false;
	bool m_bPlayerHpBackUi = false;
	bool m_bPlayerSpBackUi = false;

	bool m_bDashEffectCheck = false;
	float m_fDashEffectCool;

	bool m_bLOMBotEffectCheck = false;
	float m_fLOMBotEffectCool;

	bool m_bLOMBotEffect2Check = false;
	float m_fLOMBotEffect2Cool;

	bool m_bLOMBlueFireEffectCheck = false;
	float m_fLOMBlueFireEffectCool;

	bool m_bLOMHandEffectCheck = false;
	float m_fLOMHandEffectCool;

	bool m_bLOMHandEffect2Check = false;
	float m_fLOMHandEffect2Cool;

	bool m_bRuinBladeCheck = false;
	float m_fRuinBladeCool;
	
	bool m_bLordOfManaCheck = false;
	float m_fLordOfManaCool;

	bool m_HitCheck = false;
	float m_fHitCool;


public:
	CPlayerHp*	m_pPlayerHp = nullptr;
	CPlayerSp*	m_pPlayerSp = nullptr;
	float fDotResult;
private:
	
	CPlayerHpBack*	m_pPlayerHpBack = nullptr;
	CPlayerSpBack*	m_pPlayerSpBack = nullptr;
	CNpcUI*		m_pNpcUi = nullptr;
	CTextUI*	m_pTextUi = nullptr;
	CTextUI2*	m_pTextUi2 = nullptr;
	
	//CCanon*		m_pCanon = nullptr;
	CBoss*		m_pBoss = nullptr;

	CEffect*	m_pEffect = nullptr;
	CLordOfManaBotEffect* m_pLoMBEffect = nullptr;
	CLordOfManaBotEffect2* m_pLoMBEffect2 = nullptr;
	CLordOfManaBlueFireEffect* m_pLoMBlueFireEffect = nullptr;
	CRuinBlade*	m_pRuinBlade = nullptr;
	CLordOfMana* m_pLordOfMana = nullptr;
	CWhite*		m_pWhite = nullptr;
	CLowHp*		m_pLowHp = nullptr;
	CHitEffect*	m_pHitEffect = nullptr;
	CLordOfManaHandEffect* m_pLOMHandEffect = nullptr;
	CLordOfManaHandEffect2* m_pLOMHandEffect2 = nullptr;
	CPlayerSkillBar* m_pPlayerSkillBar = nullptr;
	CPlayerSkill1*   m_pPlayerSkill1 = nullptr;
	CPlayerSkill2*   m_pPlayerSkill2 = nullptr;
	CSwordEffect*	m_pSwordEffect = nullptr;
	CDustEffect*	m_pDustEffect = nullptr;

};


#endif // Player_h__
