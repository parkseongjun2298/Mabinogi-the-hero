#ifndef White_h__
#define White_h__

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
class CCompleteWhite;
class CWhite : public Engine::CGameObject
{
private:
	explicit CWhite(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CWhite(void);

public:
	virtual HRESULT Ready_GameObject(void) override;
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

	_matrix					m_matProj;
	_float					m_fX, m_fY;
	_float					m_fSizeX, m_fSizeY;

	_float					m_fDeadTime;


public:
	static CWhite*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;
	CCompleteWhite* m_pCompleteWhite = nullptr;
	CPlayer* m_pPlayer=nullptr;
};


#endif // White_h__
