#ifndef Fountain_h__
#define Fountain_h__

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
class CBoss;
class CFountainPiece0;
class CFountainPiece1;
class CFountainPiece2;
class CFountainPiece3;
class CFountainPiece4;
class CFountainPiece5;
class CFountainPiece6;
class CFountainPiece7;
class CFountainPiece8;
class CFountainPiece9;
class CFountain2;
class CFountain : public Engine::CGameObject
{
private:
	explicit CFountain(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFountain(void);

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
	static CFountain*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	_float m_fRight = 0;
private:
	virtual void Free(void) override;

public:
	bool					m_bCheckDead = m_bDead;
private:
	
	CBoss*			m_pBoss = nullptr;
	CFountainPiece0* m_pPiece0 = nullptr;
	CFountainPiece1* m_pPiece1 = nullptr;
	CFountainPiece2* m_pPiece2 = nullptr;
	CFountainPiece3* m_pPiece3 = nullptr;
	CFountainPiece4* m_pPiece4 = nullptr;
	CFountainPiece5* m_pPiece5 = nullptr;
	CFountainPiece6* m_pPiece6 = nullptr;
	CFountainPiece7* m_pPiece7 = nullptr;
	CFountainPiece8* m_pPiece8 = nullptr;
	CFountainPiece9* m_pPiece9 = nullptr;
	CFountain2*		m_pFountain2=nullptr;
	float m_fAngleX;
	float m_fAngleY;
	float m_fSaveTime;
	bool  m_bRotate = false;

	bool m_bSound = false;

	_vec3						m_pVtxPos;
};


#endif // Fountain_h__
