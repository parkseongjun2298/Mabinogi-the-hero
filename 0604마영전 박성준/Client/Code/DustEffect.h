#ifndef DustEffect_h__
#define DustEffect_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END

class CDustEffect : public Engine::CGameObject
{
private:
	explicit CDustEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDustEffect(void);

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT		Add_Component(void);
	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT& pDustEffect);
private:
	Engine::CRcTex*			m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;
	_float					m_fFrame = 0.f;
	_float					m_fDeadTime;

public:
	static CDustEffect*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 PlayerPos);

private:
	virtual void Free(void) override;
};


#endif // DustEffect_h__
