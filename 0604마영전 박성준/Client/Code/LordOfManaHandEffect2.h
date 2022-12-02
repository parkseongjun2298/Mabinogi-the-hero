#ifndef LordOfManaHandEffect2_h__
#define LordOfManaHandEffect2_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END
class CPlayer;
class CLordOfManaHandEffect2 : public Engine::CGameObject
{
private:
	explicit CLordOfManaHandEffect2(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLordOfManaHandEffect2(void);

public:
	virtual HRESULT Ready_GameObject(const _uint& iFlag) ;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT		Add_Component(void);
	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT& pEffect);
private:
	Engine::CRcTex*			m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;
	_float					m_fFrame = 0.f;
	_float					m_fDeadTime;
	

	_uint	m_iFlag;
	const _matrix*			m_pParentBoneMatrix;
	const _matrix*			m_pParentWorldMatrix;

private:
	CPlayer*		m_pPlayer = nullptr;
public:
	static CLordOfManaHandEffect2*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iFlag);

private:
	virtual void Free(void) override;
};


#endif // LordOfManaHandEffect2_h__
