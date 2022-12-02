#ifndef Collider2_h__
#define Collider2_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCollider2 : public CComponent
{
private:
	explicit CCollider2(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCollider2(void);

public:
	const _matrix*		Get_CollMatrix(void) { return &m_matWorld; }
	const _vec3*		Get_Min(void) { return &m_vMin; }
	const _vec3*		Get_Max(void) { return &m_vMax; }

public:
	HRESULT		Ready_Collider2(const _vec3* pPos, 
								const _ulong& dwVtxCnt, 
								const _ulong& dwStride);
	HRESULT		Ready_Collider2(const _vec3& vMax, const _vec3& vMin);

	void		Render_Collider2(COLLTYPE eType, 
								const _matrix* pCollider2Matrix);


private:
	_vec3						m_vMin, m_vMax;
	_matrix						m_matWorld;

#ifdef _DEBUG

	LPDIRECT3DDEVICE9			m_pGraphicDev;
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;
	LPDIRECT3DTEXTURE9			m_pTexture[COL_END];

#endif

public:
	static CCollider2*	Create(LPDIRECT3DDEVICE9 pGraphicDev, 
								const _vec3* pPos, 
								const _ulong& dwVtxCnt,
								const _ulong& dwStride);
	static CCollider2*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& vMax, const _vec3& vMin);
private:
	virtual void		Free(void);
};

END
#endif // Collider2_h__
