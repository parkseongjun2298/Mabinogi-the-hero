#ifndef BackGround2_h__
#define BackGround2_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CShader;
class CTransform;
END

class CBackGround2 : public Engine::CGameObject
{
private:
	explicit CBackGround2(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBackGround2(void);

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT		Add_Component(void);
	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT& pEffect);

private:
	Engine::CRcTex*			m_pBufferCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;
	float Temp;

public:
	static CBackGround2*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;
};


#endif // BackGround2_h__
