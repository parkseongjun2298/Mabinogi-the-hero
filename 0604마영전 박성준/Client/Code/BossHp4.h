#ifndef BossHp4_h__
#define BossHp4_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTexLeft;
class CTexture;
class CTransform;
class CRenderer;

END
class CBoss;
class CBossHp4 : public Engine::CGameObject
{
private:
	explicit CBossHp4(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBossHp4(void);

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
	_float					m_fSizeX, m_fSizeY;

public:
	static CBossHp4*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

	CBoss* m_pBoss;
};


#endif // BossHp4_h__
