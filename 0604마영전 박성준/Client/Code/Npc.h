#ifndef Npc_h__
#define Npc_h__

#include "Defines.h"
#include "GameObject.h"

enum NpcState
{
	Npc_Idle,
	Npc_END
};

BEGIN(Engine)

class CDynamicMesh;
class CCalculator;
class CCollider;
class CShader;
END

class CNpc : public Engine::CGameObject
{
private:
	explicit CNpc(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CNpc(void);

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT		Add_Component(void);
	
	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT& pEffect);

private:
	Engine::CDynamicMesh*	m_pMeshCom = nullptr;
	
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CCollider*		m_pColliderCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;
	
	_vec3					m_vPos;
	_vec3					m_vDirection;
	_float					m_fSpeed = 4.f;
public:
	_float					m_fRadius = 0.f;
	_float					m_fRight = 0.f;
public:
	static CNpc*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;
private:
	void Npc_Mode();
public:
	NpcState m_NpcState = Npc_Idle;
private:
	bool m_bRightSpinCheck = false;
	bool m_bLeftSpinCheck = false;
	bool m_bFrontSpinCheck = false;
	bool m_bBackSpinCheck = false;
	


	
};


#endif // Npc_h__
