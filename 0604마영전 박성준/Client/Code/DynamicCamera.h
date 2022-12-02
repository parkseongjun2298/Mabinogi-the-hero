#ifndef DynamicCamera_h__
#define DynamicCamera_h__

#include "Defines.h"
#include "Base.h"
#include "Camera.h"
#include "Player.h"
#include"Boss.h"

BEGIN(Engine)
class CKeyMgr;
END

enum CameraState
{
	PLAYER_IDLE,
	PLAYER,
	VIEW,
	BOSS,
	BOSS2,
	FREE,
	LORDOFMANA,
	BOSS_DIE,
	CAMERASTATE_END
};


class CDynamicCamera : public Engine::CCamera
{
private:
	explicit CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDynamicCamera(void);

public:
	HRESULT Ready_GameObject(const _vec3* pEye,
		const _vec3* pAt,
		const _vec3* pUp,
		const _float& fFovY,
		const _float& fAspect,
		const _float& fNear,
		const _float& fFar);

	virtual _int Update_GameObject(const _float& fTimeDelta) override;

private:
	void		Key_Input(const _float& fTimeDelta);
	void		Mouse_Move(const _float& fTimeDelta);
	void		Fix_Mouse(void);
	void		CameraMode(const _float& fTimeDelta);
	void		Idle_Mode();
	void		Player_Mode(const _float& fTimeDelta);
	void		Boss_Mode(const _float& fTimeDelta);
	void		Boss_Mode2(const _float& fTimeDelta);
	void		Free_Mode(const _float& fTimeDelta);
	void		View_Mode(const _float& fTimeDelta);


	void		LordOfMana_Mode(const _float& fTimeDelta);

	void		BossDie_Mode(const _float& fTimeDelta);

private:
	_bool		m_bClick = false;
	_bool		m_bMouseFix = false;
	Engine::CKeyMgr*	 m_pKeyMgr;

public:
	static CDynamicCamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const _vec3* pEye,
		const _vec3* pAt,
		const _vec3* pUp,
		const _float& fFovY,
		const _float& fAspect,
		const _float& fNear,
		const _float& fFar);
	CPlayer*		m_pPlayer = nullptr;
	CBoss*			m_pBoss = nullptr;

private:
	virtual void Free(void);


public:
	_float			m_MouseAngle = 0.f;
	_float			m_MouseAngleX = 0.f;

	bool			m_bBossCtl = false;
private:
	_float			m_fDist = 30.f;
	_float			m_fRotX = 0.f;
	_float          m_fRotY = 0.f;
	_float			m_fRotX2 = 160.f;

	_float          m_fBossRotY = 0.f;

	_float			m_fBossRotX = 320.f;
	_float			m_fBossRotX2 = 140.f;

	float			m_fBossDieTime = 0.f;
	_float		m_fZdist = 3.f;


	bool			m_bViewCam2 = false;
	bool			m_bViewFin = false;


	bool			m_bBossCam = false;
	bool			m_bBossFin = false;

	bool			m_bBossCam2 = false;
	bool			m_bBossFin2 = false;



	bool			m_bAttEff = false;
	bool			m_bAttEffCheck = false;


public:
	CameraState m_CameraMode = PLAYER;




};



#endif // DynamicCamera_h__
