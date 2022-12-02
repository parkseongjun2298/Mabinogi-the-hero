#ifndef LoadingBar_h__
#define LoadingBar_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CRcTexLeft;
class CShader;
class CTransform;
END

class CLoadingBar : public Engine::CGameObject
{
private:
	explicit CLoadingBar(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoadingBar(void);

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT		Add_Component(void);
	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT& pEffect);

private:
	Engine::CRcTexLeft*		m_pBufferCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;


public:
	static CLoadingBar*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;
};


#endif // LoadingBar_h__
