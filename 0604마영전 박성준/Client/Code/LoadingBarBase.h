#ifndef LoadingBarBase_h__
#define LoadingBarBase_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CRcTexLeft;
class CShader;
class CTransform;
END

class CLoadingBarBase : public Engine::CGameObject
{
private:
	explicit CLoadingBarBase(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoadingBarBase(void);

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT		Add_Component(void);
	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT& pEffect);

private:
	Engine::CRcTexLeft*			m_pBufferCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;


public:
	static CLoadingBarBase*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;
};


#endif // LoadingBarBase_h__
