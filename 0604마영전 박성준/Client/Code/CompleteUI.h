#ifndef CompleteUI_h__
#define CompleteUI_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END
class CPlayer;
class CCompleteUI : public Engine::CGameObject
{
private:
	explicit CCompleteUI(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCompleteUI(void);

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
	_float					m_fTest;
public:
	static CCompleteUI*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

	CPlayer* m_pPlayer;
};


#endif // CompleteUI_h__
