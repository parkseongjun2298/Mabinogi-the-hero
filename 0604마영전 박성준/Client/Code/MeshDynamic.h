#pragma once
#ifndef MeshDynamic_h__
#define MeshDynamic_h__

#include "GameObject.h"
#include "Defines.h"

BEGIN(Engine)
class CDynamicMesh;
END

class CMeshDynamic : public Engine::CGameObject
{
private:
	explicit CMeshDynamic(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMeshDynamic(void);

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT		Add_Component(void);
	void		Key_Input(const _float& fTimeDelta);

private:
	Engine::CDynamicMesh*	m_pMeshCom = nullptr;

public:
	static CMeshDynamic*		Create(LPDIRECT3DDEVICE9 pGraphicDev, CString MeshName);

private:
	virtual void Free(void) override;

private:
	CString m_MeshName = L"";
};

#endif // !MeshDynamic_h__