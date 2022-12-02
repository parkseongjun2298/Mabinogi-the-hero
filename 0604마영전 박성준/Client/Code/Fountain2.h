#ifndef Fountain2_h__
#define Fountain2_h__

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


class CFountain2 : public Engine::CGameObject
{
private:
	explicit CFountain2(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFountain2(void);

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
	
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CStaticMesh*	m_pMeshCom = nullptr;
	Engine::CCollider*		m_pColliderCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;
	_bool	m_bColl;
	_uint	m_iFlag;

	


public:
	static CFountain2*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	_float m_fRight = 0;
private:
	virtual void Free(void) override;

	
private:
	
	
	float m_fAngleX;
	float m_fAngleY;
	float m_fSaveTime;
	bool  m_bRotate = false;

	_vec3						m_pVtxPos;
};


#endif // Fountain2_h__
