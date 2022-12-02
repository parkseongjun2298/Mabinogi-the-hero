#ifndef TextUI2_h__
#define TextUI2_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END
class CUI;
class CTextUI2 : public Engine::CGameObject
{
private:
	explicit CTextUI2(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTextUI2(void);

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
	_tchar					m_szText[256];


	void		Key_Input(const _float& fTimeDelta);
	bool		m_bScene1 = false;
	

	CUI* m_pUi = nullptr;

public:
	bool		m_bSaveDead=m_bDead;
public:
	static CTextUI2*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;


};


#endif // TextUI2_h__
