#ifndef Bomb_h__
#define Bomb_h__

#include "Defines.h"
#include "GameObject.h"


BEGIN(Engine)


class CTransform;
class CRenderer;
class CCalculator;
class CStaticMesh;
class CCollider;

END
class CBoss;
class CBomb : public Engine::CGameObject
{
private:
	explicit CBomb(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBomb(void);

public:
	HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;
public:
	Engine::CCollider* Get_ColliderCom() { return m_pColliderCom; }

private:
	HRESULT		Add_Component(void);
	

private:
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CStaticMesh*	m_pMeshCom = nullptr;
	Engine::CCollider*		m_pColliderCom = nullptr;

	_bool	m_bColl;
	_uint	m_iFlag;
	float	m_fSpeed = 2.f;
	
	_vec3 vStartPos;
	_vec3 vEndPos;
	_vec3 VPos;
	float m_fX;
	float m_fY;
	float m_fZ;
	float m_fg;
	float m_fEndTime;
	float m_fMaxHeight = 5;                         //m_fMaxTime=10.2일때 3000은 분수대 까지만 2000 보스 앞까지 1000 분수대 까지
	float m_fHeight;
	float m_fEndHeight;
	float m_fTime = 0.f;
	float m_fMaxTime = 1.2;
	CBoss* m_pBoss = nullptr;
	POINT m_pMousePoint;

public:
	static CBomb*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* CanonPos, POINT MousePos);

private:
	virtual void Free(void) override;


private:
	
	float m_fAngleX;
	float m_fAngleY;
	float m_fSaveTime;
	bool  m_bRotate = false;

	_vec3						m_pVtxPos;
};


#endif // Bomb_h__
