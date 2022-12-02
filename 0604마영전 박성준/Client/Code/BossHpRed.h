#ifndef BossHpRed_h__
#define BossHpRed_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTexLeft;
class CTexture;
class CTransform;
class CRenderer;

END
class CBossHp0;
class CBoss;
class CBossHpRed : public Engine::CGameObject
{
private:
	explicit CBossHpRed(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBossHpRed(void);

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

	bool					m_bCheckHp0 = false;
public:
	static CBossHpRed*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

	CBossHp0* m_pBossHp0 = nullptr;
	CBoss*	 m_pBoss = nullptr;
	
};


#endif // BossHpRed_h__
