#ifndef PlayerSp_h__
#define PlayerSp_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTexLeft;
class CTexture;
class CTransform;
class CRenderer;

END
class CPlayer;
class CPlayerSp : public Engine::CGameObject
{
private:
	explicit CPlayerSp(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPlayerSp(void);

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT		Add_Component(void);

private:
	Engine::CRcTexLeft*			m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;

	_matrix					m_matProj;
	_float					m_fX, m_fY;
	_float					m_fSizeX, m_fSizeY;

public:
	static CPlayerSp*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

	CPlayer* m_pPlayer;
};


#endif // PlayerSp_h__
