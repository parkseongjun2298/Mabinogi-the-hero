#ifndef Sword_h__
#define Sword_h__

#include "Defines.h"
#include "GameObject.h"


BEGIN(Engine)


class CTransform;
class CRenderer;
class CCalculator;
class CStaticMesh;
class CCollider2;
class CShader;
class CTexture;
END
class CPlayer;
class CDog;
class CSoldier;
class CBoss;
class CGuarder;
class CKnight;
class CSwordEffect;
class CSword : public Engine::CGameObject
{
private:
	explicit CSword(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSword(void);

public:
	HRESULT Ready_GameObject(const _uint& iFlag);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;
public:
	Engine::CCollider2* Get_ColliderCom() { return m_pColliderCom; }

private:
	HRESULT		Add_Component(void);
	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT& pEffect);
	HRESULT		SetUp_SwordTrailConstantTable(LPD3DXEFFECT& pEffect);
	

private:
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CStaticMesh*	m_pMeshCom = nullptr;
	Engine::CCollider2*		m_pColliderCom = nullptr;

	Engine::CShader*		m_pShaderCom = nullptr;

	Engine::CShader*		m_pSwordTrailShaderCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	_bool	m_bColl;
	_uint	m_iFlag;

	const _matrix*			m_pParentBoneMatrix;
	const _matrix*			m_pParentWorldMatrix;

	list<Engine::TRIALVTX>	m_lstTrailVtx;
	_vec3					m_vTopPos;
	_vec3					m_vBottomPos;
	_uint					m_iTrailIdxCount;
public:
	static CSword*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iFlag);

private:
	virtual void Free(void) override;

public:
	CSoldier*		m_pSoldier = nullptr;
	CBoss*			m_pBoss = nullptr;
	CDog*			m_pDog = nullptr;
	CKnight*		m_pKnight = nullptr;
	CGuarder*		m_pGuarder = nullptr;

	CSwordEffect*	m_pSwordEffect = nullptr;
private:
	CPlayer*		m_pPlayer = nullptr;

	_vec3 temp;
	float m_fAngleX;
	float m_fAngleY;
	float m_fSaveTime;
	bool  m_bRotate = false;

	_vec3						m_pVtxPos;
};


#endif // Sword_h__
