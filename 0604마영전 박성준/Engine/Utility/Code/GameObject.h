#ifndef GameObject_h__
#define GameObject_h__

#include "Component.h"

BEGIN(Engine)
class CTransform;
class CRenderer;
class CTexture;

class ENGINE_DLL CGameObject : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGameObject(void);

public:
	CComponent*		Find_Component(const _tchar* pComponentTag, COMPONENTID eID);
	CComponent*		Get_Component(const _tchar* pComponentTag, COMPONENTID eID);
	_float			Get_ViewZ(void) const { return m_fViewZ; }

public:
	virtual HRESULT		Ready_GameObject(void);
	virtual _int		Update_GameObject(const _float& fTimeDelta);
	virtual void		Render_GameObject(void);
public:
	CTransform* Get_Transform();
	void Set_bIsDead() { m_bDead = true; }

	MESHTYPE		Get_MeshType() { return m_eMesyType; }
	const _tchar*	Get_MeshName() { return m_pMeshName; }
	void		Compute_ViewZ(const _vec3* pPos);

protected:
	LPDIRECT3DDEVICE9					m_pGraphicDev;
	_float								m_fViewZ;
	map<const _tchar*, CComponent*>		m_mapComponent[ID_END];

	_bool m_bDead = false;

	//추가
	CTransform*		m_pTransformCom = nullptr;
	CRenderer*		m_pRendererCom = nullptr;
	CTexture*		m_pTextureCom = nullptr;

	MESHTYPE		m_eMesyType = MESHTYPE::TYPE_NONE;
	const _tchar*	m_pMeshName;

	_float			m_fDynamicTime = 0.f;
	_float			m_fTime = 0.f;
	_float			m_fSpeed = 0.f;


	_ulong m_NaviIndex; // 네비 인덱스
public:
	virtual void Free(void);

};


END
#endif // GameObject_h__
