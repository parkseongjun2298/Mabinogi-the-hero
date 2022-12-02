#ifndef Boss_h__
#define Boss_h__

#include "Defines.h"
#include "GameObject.h"

#include"CollisionMgr.h"
#include"BossHpGreen.h"
#include"BossHpYellow.h"
#include"BossHpRed.h"
#include"BossHpX.h"

#include"Sword.h"
#include"HitEffect.h"
#include"DustEffect.h"

#include"BossSword.h"
enum BossState
{
	BOSS_TAUNT,
	BOSS_DMG_TAUNT,
	BOSS_DOWNEND,
	BOSS_DOWNSTART,
	BOSS_DOWNING,
	BOSS_TURNRIGHT,
	BOSS_TURNLEFT,
	BOSS_RUN,
	BOSS_WEAKPOINT_BROKEN,
	BOSS_LOW_HP,
	BOSS_DYING,
	BOSS_DAMAGE_WEAK_TOP,
	BOSS_DAMAGE_WEAK_BOT,
	BOSS_BLIND_DURING,
	BOSS_BLIND_BEGIN,
	BOSS_THREAT,
	BOSS_WHIRLWIND,
	BOSS_TURNRIGHT_ATK,
	BOSS_TURNLEFT_ATK,
	BOSS_RAGE_SWORDWAVE_END,
	BOSS_RAGE_SWORDWAVE_BEGIN,
	BOSS_SWORDWAVE,
    BOSS_SWING_TRIPLE,
	BOSS_THRUST_TO_WHIRLWIND,
	BOSS_DOUBLESMASH,
	BOSS_IDLE,
	BOSS_CUTSCENE2,
	BOSS_CUTSCENE1,
	BOSS_PIERCE,
	BOSS_TRIPLE_COMBO,
	BOSS_END

	/*0	taunt			����
	1	damage_to_taunt		������ ������ ����
	2	down_end		�ٿ(�Ͼ�¸��)
	3	down_begin		�ٿ����
	4	down_during		�ٿ���
	5	turn_right		���������� ���� ���
	6	turn_left		�������� ���¸��
	7	run			�޸���
	8	weakpoint_broken	�κ��ı����� �ı��Ǵ� ����
	9	low_hp			�� ������ �����°�
	10	dying_front		�״¸��
	11	damage_weak_top ? ?
	12	damage_weak_bottom ? ?
	13	blind_during		���� ������
	14	blind_begin		�÷��̾ ��ź(����ź)�� ������ ������ ����
	15	threat			���� : ���� 2������ �г���� �������� �ϸ�������
	16	whirlwind		������
	17	turnright_atk(�������� �������� ������ ���������� ���鼭 1Ÿ �����ϴ� ���)
	18	turnleft_atk(�������� ������ �����ٺ��� �������� ���鼭 1Ÿ �����ϴ� ���)
	19	rage_swordwave_end(�г���� �ҵ���̺� 2Ÿ end)
	20	rage_swordwave_begin(�г���¿����� �ҵ���̺� 2���� ���� begin)
	21	swordwave		������� �� �����
	22	swing_triple		3��Ÿ
	23	thrust_to_whirlwind	��������� ������
	24	doublesmash		2��Ÿ ���Ž�
	25	Idle			���̵� ����
	26	cutscene_2		�������� �ι�° �ƾ�
	27	cutscene_1		�������� ù��° �ƾ�
	28	Pierce			���
	29	triple_combo		3Ÿ ������*/
};

BEGIN(Engine)

class CDynamicMesh;
class CCalculator;
class CCollider;
class CShader;
class CNaviMesh;
END
class CDynamicCamera;
class CPlayer;
class CSword;
class CBossHpBack;
class CBossHpYellow;
class CBossHpGreen;
class CBossHpRed;
class CBossHpX;
class CBossName;

class CBoss : public Engine::CGameObject
{
private:
	explicit CBoss(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoss(void);

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

public:
	CBossHpBack*				m_pBossHpBack = nullptr;
	CBossHpYellow*				m_pBossHpYellow = nullptr;
	CBossHpGreen*				m_pBossHpGreen = nullptr;
	CBossHpRed*					m_pBossHpRed = nullptr;
private:
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CCollider*		m_pColliderCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;
	CDynamicCamera*			m_pCamera = nullptr;
	CPlayer*				m_pPlayer = nullptr;
	CSword*					m_pSword = nullptr;
	CBossSword*				m_pBossSword = nullptr;
	CBossHpX*					m_pBossHpX = nullptr;
	CBossName*					m_pBossName = nullptr;
	bool						m_bBackCheck = false;
	bool						m_bXCheck = false;
	bool						m_bRedCheck = false;
	bool						m_bGreenCheck = false;
	bool						m_bYellowCheck = false;
	bool						m_bBossNameCheck = false;
	_vec3					m_vPos;
	_vec3					m_vDirection;
	_float					m_fSpeed = 2.f;
	_float					m_fPlayer_BossDis;

	_float					m_fTest;

public:
	_float					m_fRadius = 0.f;
	_float					m_fRight = 0.f;

	bool					m_bCheckDead = m_bDead;

	bool					m_bStart = false;
	bool					m_HitCheck = false;
	bool					m_bColCheck = false;
	float					m_fColCheck;
	bool					m_bGroogy = false;
	bool					m_bSoundCut1 = false;
	bool					m_bGroogySound = false;
	bool					m_bPierceSound = false;
	bool					m_bSWORDWAVESound = false;
	bool					m_bTripleSound = false;
	bool					m_bTripleSound2 = false;
	bool					m_bTripleSound3 = false;
	_float					m_fHpCount = 3.f;
public:
	static CBoss*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;
private:
	void Boss_Mode();
	void Damage_Boss(const _float& fTimeDelta);
	void Move_Boss(const _float& fTimeDelta);
	void WHIRLWIND(const _float& fTimeDelta);
	void SWORDWAVE(const _float& fTimeDelta);
	void SWING_TRIPLE(const _float& fTimeDelta);
	void PIERCE(const _float& fTimeDelta);
	void Boss_Down(const _float& fTimeDelta);

public:
	bool	m_bPIERCECheck = false;
	bool	m_bWHIRLWINDCheck = false;
	bool	m_bSWORDWAVECheck = false;
	bool	m_bSWING_TRIPLECheck = false;
	bool	m_bAniCheck = false;
private:
	float m_fSWORDWAVECool;
	float m_fDownTime;
	_float DyingTime;
public:
	BossState m_BossState = BOSS_IDLE;

	
private:
	Engine::CNaviMesh*	m_pNaviMesh = nullptr;
	CHitEffect*	m_pHitEffect = nullptr;
	CDustEffect*	m_pDustEffect = nullptr;
};


#endif // Boss_h__
