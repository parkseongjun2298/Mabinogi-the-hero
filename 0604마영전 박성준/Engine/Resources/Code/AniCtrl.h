#ifndef AniCtrl_h__
#define AniCtrl_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CAniCtrl : public CBase
{
private:
	explicit	 CAniCtrl(LPD3DXANIMATIONCONTROLLER  pAniCtrl);
	explicit	 CAniCtrl(const CAniCtrl& rhs);
	virtual		~CAniCtrl(void);

public:
	LPD3DXANIMATIONCONTROLLER		Get_AnimationCtrl(void) { return m_pAniCtrl; }

public:
	HRESULT		Ready_AnimationCtrl(void);
	void		Set_Animation(const _uint& iIndex);
	void		Play_Animation(const _float& fTimeDelta);
	_bool		Is_AnimationEnd(void);

public:
	LPD3DXANIMATIONCONTROLLER		m_pAniCtrl;
	_uint							m_iCurrentTrack;
	_uint							m_iNewTrack;
	_float							m_fAccTime;
	_uint							m_iOldAniIndex;
	_double							m_dPeriod; //애니메이션 총 재생시간
	_float							m_fAniSpeed;
public:
	static CAniCtrl*		Create(LPD3DXANIMATIONCONTROLLER pAniCtrl);
	static CAniCtrl*		Create(const CAniCtrl& rhs);
	virtual void			Free(void);

};

END

#endif // AniCtrl_h__
