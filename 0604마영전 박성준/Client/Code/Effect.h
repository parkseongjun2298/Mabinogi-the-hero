#ifndef Effect_h__
#define Effect_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END

class CEffect : public Engine::CGameObject
{
private:
	explicit CEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEffect(void);

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
	_float					m_fFrame = 0.f;
	_float					m_fDeadTime;
	float					m_fSaveAngleY;
public:
	static CEffect*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 PlayerPos , float AngleY);

private:
	virtual void Free(void) override;
};


#endif // Effect_h__
