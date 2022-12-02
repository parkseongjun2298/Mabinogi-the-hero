#ifndef EndScene_h__
#define EndScene_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CShader;
class CRcTex;
class CTexture;
class CTransform;
class CRenderer;
END

class CEndScene : public Engine::CGameObject
{
private:
	explicit CEndScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEndScene(void);

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT		Add_Component(void);
	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT& pEffect);

private:
	Engine::CRcTex*			m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;

	Engine::CShader*		m_pShaderCom = nullptr;

	_matrix					m_matProj;
	_float					m_fX, m_fY;
	_float					m_fSizeX, m_fSizeY;


public:
	static CEndScene*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;
};


#endif // EndScene_h__
