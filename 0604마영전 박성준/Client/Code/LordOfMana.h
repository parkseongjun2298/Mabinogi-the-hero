#ifndef LordOfMana_h__
#define LordOfMana_h__

#include "Defines.h"
#include "GameObject.h"

enum GhostState
{
	Ghost_Idle,
	Ghost_END
};

BEGIN(Engine)


class CDynamicMesh;
class CCalculator;
class CCollider;
class CShader;

END
class CBoss;
class CDog;
class CSoldier;
class CKnight;
class CGuarder;
class CLordOfMana : public Engine::CGameObject
{
private:
	explicit CLordOfMana(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLordOfMana(void);

public:
	HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;
public:
	Engine::CCollider* Get_ColliderCom() { return m_pColliderCom; }

private:
	HRESULT		Add_Component(void);
	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT& pEffect);

private:
	Engine::CDynamicMesh*	m_pMeshCom = nullptr;

	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CCollider*		m_pColliderCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;

	_bool	m_bColl;
	_uint	m_iFlag;

	


public:
	static CLordOfMana*		Create(LPDIRECT3DDEVICE9 pGraphicDev,_vec3 Pos);
	_float m_fRight = 0;
private:
	virtual void Free(void) override;
private:
	void Ghost_Mode();
public:
	GhostState m_GhostState = Ghost_Idle;

private:
	float m_fBladeSpeed;
	
	_vec3 m_vPos;
	float m_fAngleX;
	float m_fAngleY;
	float m_fSaveTime;
	bool  m_bRotate = false;

	_vec3						m_pVtxPos;


	CBoss* m_pBoss = nullptr;
	CDog*  m_pDog = nullptr;
	CSoldier* m_pSoldier = nullptr;
	CGuarder* m_pGuarder = nullptr;
	CKnight* m_pKnight = nullptr;
};


#endif // LordOfMana_h__
