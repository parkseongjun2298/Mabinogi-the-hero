#ifndef Soldier_h__
#define Soldier_h__

#include "Defines.h"
#include "GameObject.h"

#include"Sword.h"
#include"HitEffect.h"
enum SoldierState
{
	
	SORDIER_ATT,
	SORDIER_DMG_BOT,
	SORDIER_DMG_UP,
	SORDIER_DOWNSTART,
	SORDIER_DOWNING,
	SORDIER_DOWNEND,
	SORDIER_DYING,
	SORDIER_STAND,
	SORDIER_STAPBACK,
	SORDIER_STAPLEFT,
	SORDIER_STAPRIGHT,
	SORDIER_STAPFRONT,
	SOLDIER_KNOCKBACK,
	SOLDIER_RUN,
	SORDIER_DEFENCESTART,
	SORDIER_DEFENCEING,
	SORDIER_DEFENCEEND,
	SORDIER_DEFENCEHIT,
	SOLDIER_END
};

BEGIN(Engine)

class CDynamicMesh;
class CCalculator;
class CCollider;
class CShader;
END
class CSword;
class CPlayer;
class CSoldier : public Engine::CGameObject
{
private:
	explicit CSoldier(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSoldier(void);

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT		Add_Component(void);
	
	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT& pEffect);
public:
	Engine::CCollider* Get_ColliderCom() { return m_pColliderCom; }
private:
	Engine::CDynamicMesh*	m_pMeshCom = nullptr;
	
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CCollider*		m_pColliderCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;
	
	_vec3					m_vPos;
	_vec3					m_vDirection;
	_float					m_fSpeed = 2.f;

	CPlayer*				m_pPlayer = nullptr;
	CSword*					m_pSword = nullptr;
	_float					m_fPlayer_BossDis;

	CHitEffect*	m_pHitEffect = nullptr;
public:
	_float					m_fRadius = 0.f;
	_float					m_fRight = 0.f;
	bool					m_bCheckDead = m_bDead;
	bool					m_bColCheck = false;
	bool					m_bFollowPlayer = false;
	_float					m_fSoldierHp = 30.f;

	bool					m_bDisolve = false;
	bool m_HitCheck = false;
	float m_fHitCool;

public:
	static CSoldier*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _ulong NaviIndex);
	void Move_Soldier(const _float& fTimeDelta);
	void Att_Soldier(const _float& fTimeDelta);
	void Damage_Soldier(const _float& fTimeDelta);
private:
	virtual void Free(void) override;
private:
	void Soldier_Mode();
public:
	SoldierState m_SoldierState = SORDIER_STAND;

	
	
};


#endif // Soldier_h__
