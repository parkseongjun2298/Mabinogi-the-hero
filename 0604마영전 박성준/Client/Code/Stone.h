#ifndef Stone_h__
#define Stone_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)


class CTransform;
class CRenderer;
class CCalculator;
class CStaticMesh;
class CCollider;
class COptimization;

END

class CStone : public Engine::CGameObject
{
private:
	explicit CStone(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStone(void);

public:
	HRESULT Ready_GameObject(const _uint& iFlag);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT		Add_Component(void);
	void		SetUp_Terrain(void);
	_bool		CollisionTo_Object(const _tchar* pLayerTag,
									const _tchar* pObjTag);

private:
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CStaticMesh*	m_pMeshCom = nullptr;
	Engine::CCollider*		m_pColliderCom = nullptr;
	Engine::COptimization*	m_pOptimizationCom = nullptr;

	_bool	m_bColl;
	_uint	m_iFlag;
	_bool	m_bDraw;

public:
	static CStone*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iFlag);

private:
	virtual void Free(void) override;
};


#endif // Stone_h__
