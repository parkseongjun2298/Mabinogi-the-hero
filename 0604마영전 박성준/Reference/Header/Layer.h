#ifndef Layer_h__
#define Layer_h__

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CLayer : public CBase
{
protected:
	explicit CLayer(void);
	virtual ~CLayer(void);

public:
	CComponent*		Get_Component(const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID);
	list<CGameObject*>	Get_ObjectList(const _tchar* pObjTag);
public:
	HRESULT				Add_GameObject(const _tchar* pObjTag, CGameObject* pGameObject);

	virtual HRESULT		Ready_Layer(void);
	virtual _int		Update_Layer(const _float& fTimeDelta);
	virtual void		Render_Layer(void);

protected:
	map<const _tchar*, list<CGameObject*>>		m_mapObject;

public:
	static CLayer*		Create(void);
	virtual			void Free(void);

};


END
#endif // Layer_h__
