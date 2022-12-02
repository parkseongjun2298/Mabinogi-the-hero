#ifndef Knight_h__
#define Knight_h__

#include "Defines.h"
#include "GameObject.h"
#include"Player.h"
#include"Sword.h"
#include"HitEffect.h"
enum KnightState
{
	Knight_DASH_ATT,//2타
	Knight_DMG_BOT,//2타
	Knight_DMG_UP,
	Knight_DOWNSTART,//2타
	Knight_DOWNING,
	Knight_DOWNEND,  //2타
	Knight_DYING, //다이
	Knight_STAND,//아이들상태
	Knight_TURNLEFT,//2타
	Knight_TURNRIGHT,
	Knight_DOWNDMG, //2타
	Knight_AIRSTART,//2타
	Knight_AIRING,//2타
	Knight_AIREND,//2타
	Knight_KNOCKBACK, //이후다없는애니
	Knight_ATT,
	Knight_FALLING,
	Knight_RUN,
	Knight_END
};

BEGIN(Engine)

class CDynamicMesh;
class CCalculator;
class CCollider;
class CShader;
END
class CSword;
class CPlayer;
class CKnight : public Engine::CGameObject
{
private:
	explicit CKnight(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CKnight(void);

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT		Add_Component(void);
	
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
	_float					m_fSpeed = 2.f;

	CPlayer*				m_pPlayer=nullptr;
	CSword*					m_pSword = nullptr;
	_float					m_fPlayer_BossDis;
public:
	_float					m_fRadius = 0.f;
	_float					m_fRight = 0.f;
	_float					m_fDieCiunt = 0.f;
	bool					m_bCheckDead = m_bDead;
	bool					m_bColCheck = false;
	bool					m_bFollowPlayer = false;
	_float					m_fKnightHp = 100.f;

	CHitEffect*	m_pHitEffect = nullptr;
	bool m_HitCheck = false;
	float m_fHitCool;
public:
	static CKnight*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _ulong NaviIndex);
	void Move_Knight(const _float& fTimeDelta);
	void ATT_Knight(const _float& fTimeDelta);
	void Damage_Knight(const _float& fTimeDelta);
private:
	virtual void Free(void) override;
private:
	void Knight_Mode();
public:
	KnightState m_KnightState = Knight_STAND;



	
};


#endif // Knight_h__
