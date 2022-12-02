#ifndef Tree_h__
#define Tree_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CTransform;
class CRenderer;
class CCalculator;
class CStaticMesh;
class CCollider;
class CShader;

END

class CTree : public Engine::CGameObject
{
private:
	explicit CTree(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTree(void);

public:
	virtual HRESULT Ready_GameObject(void);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT		Add_Component(void);
	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT& pEffect);

private:
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CStaticMesh*	m_pMeshCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;

public:
	static CTree*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;
};


#endif // Tree_h__
