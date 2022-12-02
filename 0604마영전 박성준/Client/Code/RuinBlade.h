#ifndef RuinBlade_h__
#define RuinBlade_h__

#include "Defines.h"
#include "GameObject.h"
#include"RuinBladeEffect.h"

enum GhostState2
{
	Ghost2_Idle,
	Ghost2_END
};
BEGIN(Engine)



class CDynamicMesh;
class CCalculator;
class CCollider;
class CShader;
class CTexture;
class CRcTex;
END
class CBoss;
class CDog;
class CSoldier;
class CKnight;
class CGuarder;
class CRuinBlade : public Engine::CGameObject
{
private:
	explicit CRuinBlade(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CRuinBlade(void);

public:
	HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;
public:
	Engine::CCollider* Get_ColliderCom() { return m_pColliderCom; }

private:
	HRESULT		Add_Component(void);
	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT& pEffect);
	HRESULT		SetUp_ConstantTextTable(LPD3DXEFFECT& pEffect);
private:
	
	Engine::CDynamicMesh*	m_pMeshCom = nullptr;
	Engine::CRcTex*			m_pBufferCom = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CCollider*		m_pColliderCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;
	Engine::CShader*		m_pShaderTextCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;


	_bool	m_bColl;
	_uint	m_iFlag;

	bool					m_bBladeCheck = false;


public:
	static CRuinBlade*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 Look,_vec3 Pos, float AngleY);
	_float m_fRight = 0;
private:
	virtual void Free(void) override;
private:
	void Ghost_Mode();
public:
	GhostState2 m_GhostState = Ghost2_Idle;

private:
	float m_fBladeSpeed;
	_vec3 m_vLook;
	_vec3 m_vPos;
	float m_fAngleX;
	float m_fAngleY;
	float m_fSaveTime;
	float m_fSaveAngleY;
	bool  m_bRotate = false;

	_vec3						m_pVtxPos;


	CBoss* m_pBoss = nullptr;
	CDog*  m_pDog = nullptr;
	CSoldier* m_pSoldier = nullptr;
	CGuarder* m_pGuarder = nullptr;
	CKnight* m_pKnight = nullptr;

	CRuinBladeEffect* m_pRuinBladeEffect = nullptr;
};


#endif // RuinBlade_h__
