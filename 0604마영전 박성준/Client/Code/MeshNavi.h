#pragma once
#ifndef MeshNavi_h__
#define MeshNavi_h__

#include "GameObject.h"
#include "Defines.h"

BEGIN(Engine)
class CNaviMesh;
END

class CMeshNavi : public Engine::CGameObject
{
private:
	explicit CMeshNavi(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMeshNavi(void);

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT		Add_Component(void);

	Engine::CNaviMesh*		m_pNaviCom = nullptr;

public:
	static CMeshNavi*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

private:
	void LoadData(const TCHAR * pFilePath);
};

#endif // !MeshNavi_h__
