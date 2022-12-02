#ifndef Terrain_h__
#define Terrain_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CTerrainTex;
class CTexture;
class CTransform;
class CRenderer;
class COptimization;
class CShader;

END

class CTerrain : public Engine::CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTerrain(void);

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT		Add_Component(void);
	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT& pEffect);

private:
	Engine::CTerrainTex*	m_pBufferCom	= nullptr;
	Engine::CTexture*		m_pTextureCom	= nullptr;
	Engine::CTexture*		m_pFilterCom	= nullptr;
	Engine::CTexture*		m_pAuraCom		= nullptr;
	
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom	= nullptr;
	Engine::COptimization*	m_pOptimizationCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;
	
	_vec3					m_vDirection;
	_float					m_fSpeed = 4.f;

	Engine::INDEX32*		m_pIndex = nullptr;
	_ulong					m_dwTriCnt = 0;

public:
	static CTerrain*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;
};


#endif // Terrain_h__
