#ifndef Management_h__
#define Management_h__

#include "Engine_Defines.h"
#include "Base.h"
#include "Scene.h"
#include "NaviMesh.h"


BEGIN(Engine)

class ENGINE_DLL CManagement : public CBase 
{
	DECLARE_SINGLETON(CManagement)

private:
	explicit	CManagement(void);
	virtual		~CManagement(void);

public:
	CComponent*		Get_Component(const _tchar* pLayerTag,
									const _tchar* pObjTag,
									const _tchar* pComponentTag,
									COMPONENTID eID);
	list<CGameObject*>	Get_ObjectList(const _tchar* pLayerTag,
		const _tchar* pObjTag);

	list<CGameObject*> Get_Obj(const _tchar * pObjTag);

	list<CGameObject*> Get_Playeres();
	CGameObject*		Get_Camera();
	CNaviMesh*			Get_Navi();
public:
	HRESULT		Ready_Management(LPDIRECT3DDEVICE9 pGraphicDev);
	HRESULT		SetUp_Scene(CScene* pScene);
	_int		Update_Scene(const _float& fTimeDelta);
	void		Render_Scene(LPDIRECT3DDEVICE9& pGraphicDev);
public:
	CScene* Get_Scene() { return m_pScene; }
private:
	CScene*		m_pScene = nullptr;
	
public:
	virtual void	Free(void);
public:
	HRESULT Add_GameObject(const _tchar * pLayerTag, const _tchar * pObjTag, CGameObject * pGameObject);
};

END
#endif // Management_h__
