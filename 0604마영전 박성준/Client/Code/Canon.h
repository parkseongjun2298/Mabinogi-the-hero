#ifndef Canon_h__
#define Canon_h__

#include "Defines.h"
#include "GameObject.h"


BEGIN(Engine)


class CTransform;
class CRenderer;
class CCalculator;
class CStaticMesh;
class CCollider;

END
class CPlayer;
class CCanon : public Engine::CGameObject
{
private:
	explicit CCanon(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCanon(void);

public:
	HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;
public:
	Engine::CCollider* Get_ColliderCom() { return m_pColliderCom; }

private:
	HRESULT		Add_Component(void);
	

private:
	
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CStaticMesh*	m_pMeshCom = nullptr;
	Engine::CCollider*		m_pColliderCom = nullptr;

	_bool	m_bColl;
	_uint	m_iFlag;

	


public:
	static CCanon*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	_float m_fRight = 0;
private:
	virtual void Free(void) override;


private:
	CPlayer*		m_pPlayer = nullptr;
	
	float m_fAngleX;
	float m_fAngleY;
	float m_fSaveTime;
	bool  m_bRotate = false;

	_vec3						m_pVtxPos;
};


#endif // Canon_h__
