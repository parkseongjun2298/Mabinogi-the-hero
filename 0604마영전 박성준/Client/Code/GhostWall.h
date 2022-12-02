#ifndef GhostWall_h__
#define GhostWall_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END

class CGhostWall : public Engine::CGameObject
{
private:
	explicit CGhostWall(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGhostWall(void);

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT		Add_Component(void);
	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT& pGhostWall);
private:
	Engine::CRcTex*			m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;
	_float					m_fFrame = 0.f;
	_float					m_fDeadTime;

public:
	static CGhostWall*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;
};


#endif // GhostWall_h__
