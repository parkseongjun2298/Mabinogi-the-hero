#pragma once
#ifndef BossMap_h__
#define BossMap_h__

#include "GameObject.h"
#include "Defines.h"
BEGIN(Engine)
class CShader;
END
class CBossMap : public Engine::CGameObject
{
private:
	explicit CBossMap(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBossMap(void);

public:
	HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT		Add_Component(void);
	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT& pEffect);
private:
	Engine::CStaticMesh* m_pMeshCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;
public:
	static CBossMap*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;
};

#endif // !BossMap_h__