#ifndef BossHpYellow_h__
#define BossHpYellow_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTexLeft;
class CTexture;
class CTransform;
class CRenderer;

END
class CBossHp3;
class CBossHp2;
class CBossHp1;
class CBoss;
class CBossHpYellow : public Engine::CGameObject
{
private:
	explicit CBossHpYellow(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBossHpYellow(void);

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT		Add_Component(void);

private:
	Engine::CRcTexLeft*		m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;

	_matrix					m_matProj;
	_float					m_fX, m_fY;

public:
	_float					m_fSizeX, m_fSizeY;
	bool					m_bCheckDead = m_bDead;
	_float					m_fBossHpNum;

	bool					m_bCheckHp3 = false;
	bool					m_bCheckHp2 = false;
	bool					m_bCheckHp1 = false;
public:
	static CBossHpYellow*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;
	CBossHp3* m_pBossHp3 = nullptr;
	CBossHp2* m_pBossHp2 = nullptr;
	CBossHp1* m_pBossHp1 = nullptr;
	CBoss*	 m_pBoss = nullptr;
};


#endif // BossHpYellow_h__
