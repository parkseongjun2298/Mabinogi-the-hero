#ifndef MeshStatic_h__
#define MeshStatic_h__

#include "GameObject.h"
#include "Defines.h"

BEGIN(Engine)

class CCalculator;
class CMeshStatic;
class CCollider;

END

class CMeshStatic : public Engine::CGameObject
{
private:
	explicit CMeshStatic(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMeshStatic(void);

public:
	HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT		Add_Component(void);

private:
	Engine::CStaticMesh*	m_pMeshCom = nullptr;
	Engine::CCollider*		m_pColliderCom = nullptr;

	_bool	m_bColl;

private:
	CString m_MeshName = L"";

public:
	static CMeshStatic*		Create(LPDIRECT3DDEVICE9 pGraphicDev, CString MeshName, Engine::MESHTYPE MeshType);

private:
	virtual void Free(void) override;

	LPD3DXMESH* pMesh;
};


#endif // Stone_h__
