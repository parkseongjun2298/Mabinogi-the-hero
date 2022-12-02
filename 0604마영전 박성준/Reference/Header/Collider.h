#ifndef Collider_h__
#define Collider_h__

#include "Component.h"
#include "Mesh.h"
#include "StaticMesh.h"
#include "DynamicMesh.h"
#include "Transform.h"
BEGIN(Engine)

class ENGINE_DLL CCollider : public CComponent
{
private:
	explicit CCollider(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCollider(void);

public:
	const _matrix*		Get_CollMatrix(void) { return &m_matWorld; }
	const _vec3*		Get_Min(void) { return &m_vMin; }
	const _vec3*		Get_Max(void) { return &m_vMax; }

public:
	HRESULT		Ready_Collider(const _vec3* pPos, 
								const _ulong& dwVtxCnt, 
								const _ulong& dwStride,
								CMesh* pMesh,
								Engine::MESHTYPE Type);

	void		Render_Collider(COLLTYPE eType, 
								const _matrix* pColliderMatrix);

	virtual void		Update_Component(const _float& fTimeDelta);

	void				Update(_matrix matWorld);
private:
	_vec3						m_vMin, m_vMax;
	
public:
	float						m_fRadius;//매쉬의 반지름
	_matrix						m_matWorld;
	_vec3						m_vCenter; // 구체의 중점
	_float						m_fScale = 0.f; // 구체의 반지름
//#ifdef _DEBUG

	LPDIRECT3DDEVICE9			m_pGraphicDev;
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;
	LPDIRECT3DTEXTURE9			m_pTexture[COL_END];

//#endif
public:
	LPD3DXMESH					m_pMesh;
	void Set_Pos(const _float & fX, const _float & fY, const _float & fZ);
	

public:
	static CCollider*	Create(LPDIRECT3DDEVICE9 pGraphicDev, 
								const _vec3* pPos, 
								const _ulong& dwVtxCnt,
								const _ulong& dwStride,
								CMesh* pMesh,
								 Engine::MESHTYPE Type);
private:
	virtual void		Free(void);
};

END
#endif // Collider_h__
