#ifndef NpcUI_h__
#define NpcUI_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcCube;
class CTexture;
class CTransform;
class CRenderer;

END
class CTextUI;
class CNpcUI : public Engine::CGameObject
{
private:
	explicit CNpcUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CNpcUI(void);

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT		Add_Component(void);

private:
	Engine::CRcTex*			m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;

	_matrix					m_matProj;
	_float					m_fX, m_fY;
	_float					m_fSizeX, m_fSizeY;
	CTextUI*				m_pTextUi;
public:
	static CNpcUI*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;


};


#endif // NpcUI_h__
