#ifndef BossHpGreen_h__
#define BossHpGreen_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTexLeft;
class CTexture;
class CTransform;
class CRenderer;

END
class CBossHp4;
class CBoss;
class CBossHpGreen : public Engine::CGameObject
{
private:
	explicit CBossHpGreen(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBossHpGreen(void);

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
public:
	_float					m_fX, m_fY;
	_float					m_fSizeX, m_fSizeY;
	bool					m_bCheckDead = m_bDead;

	bool					m_bCheckHp4=false;
public:
	static CBossHpGreen*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

	CBossHp4* m_pBossHp4=nullptr;
	CBoss*	 m_pBoss = nullptr;
};


#endif // BossHpGreen_h__
