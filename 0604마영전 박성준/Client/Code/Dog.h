#ifndef Dog_h__
#define Dog_h__

#include "Defines.h"
#include "GameObject.h"
#include"Player.h"
#include"Sword.h"
#include"HitEffect.h"
enum DogState
{
	DOG_DASH_ATT,
	DOG_DMG_BOT,
	DOG_DMG_UP,
	DOG_DOWNSTART,
	DOG_DOWNING,
	DOG_DOWNEND,
	DOG_DYING,
	DOG_STAND,
	DOG_TURNLEFT,
	DOG_TURNRIGHT,
	DOG_DOWNDMG,
	DOG_AIRSTART,
	DOG_AIRING,
	DOG_AIREND,
	DOG_KNOCKBACK,
	DOG_ATT,
	DOG_FALLING,
	DOG_RUN,
	DOG_END
};

BEGIN(Engine)

class CDynamicMesh;
class CCalculator;
class CCollider;
class CShader;

END
class CSword;
class CPlayer;
class CDog : public Engine::CGameObject
{
private:
	explicit CDog(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDog(void);

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

	bool m_HitCheck = false;
	float m_fHitCool;

public:
	_float					m_fRadius = 0.f;
	_float					m_fRight = 0.f;

	bool					m_bCheckDead = m_bDead;
	bool					m_bColCheck = false;
	bool					m_bFollowPlayer = false;
	_float					m_fDogHp = 40.f;
	bool					m_bDisolve = false;

	_float					m_fTest;
public:
	static CDog*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _ulong NaviIndex);
	void Move_Dog(const _float& fTimeDelta);
	void ATT_Dog(const _float& fTimeDelta);
	void Damage_Dog(const _float& fTimeDelta);
private:
	virtual void Free(void) override;
private:
	void Dog_Mode();
public:
	DogState m_DogState = DOG_STAND;
private:
		
	CHitEffect*	m_pHitEffect = nullptr;
};


#endif // Dog_h__
