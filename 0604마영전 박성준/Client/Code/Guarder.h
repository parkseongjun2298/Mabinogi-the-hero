#ifndef Guarder_h__
#define Guarder_h__

#include "Defines.h"
#include "GameObject.h"
#include"Player.h"
#include"Sword.h"
#include"HitEffect.h"
enum GuarderState
{
	Guarder_DASH_ATT,
	Guarder_DMG_BOT,
	Guarder_DMG_UP,
	Guarder_DOWNSTART,
	Guarder_DOWNING,
	Guarder_DOWNEND,
	Guarder_DYING,
	Guarder_STAND,
	Guarder_TURNLEFT,
	Guarder_TURNRIGHT,
	Guarder_DOWNDMG,
	Guarder_AIRSTART,
	Guarder_AIRING,
	Guarder_AIREND,
	Guarder_KNOCKBACK,
	Guarder_ATT,
	Guarder_FALLING,
	Guarder_RUN,
	Guarder_END
};

BEGIN(Engine)

class CDynamicMesh;
class CCalculator;
class CCollider;
class CShader;
END
class CSword;
class CPlayer;
class CGuarder : public Engine::CGameObject
{
private:
	explicit CGuarder(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGuarder(void);

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
	CHitEffect*	m_pHitEffect = nullptr;
public:
	_float					m_fRadius = 0.f;
	_float					m_fRight = 0.f;

	bool					m_bCheckDead = m_bDead;
	bool					m_bColCheck = false;
	bool					m_bFollowPlayer = false;
	_float					m_fGuarderHp = 100.f;

	bool m_HitCheck = false;
	float m_fHitCool;
public:
	static CGuarder*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _ulong NaviIndex);
	void Move_Guarder(const _float& fTimeDelta);
	void ATT_Guarder(const _float& fTimeDelta);
	void Damage_Guarder(const _float& fTimeDelta);
private:
	virtual void Free(void) override;
private:
	void Guarder_Mode();
public:
	GuarderState m_GuarderState = Guarder_STAND;



	
};


#endif // Guarder_h__
