#ifndef Optimization_h__
#define Optimization_h__


#include "Component.h"
#include "QuadTree.h"

BEGIN(Engine)

class ENGINE_DLL COptimization : public CComponent
{

private:
	explicit COptimization(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit COptimization(const COptimization& rhs);
	virtual ~COptimization(void);

public:
	HRESULT		Ready_Optimization(_bool bChoice, const _ulong& dwCntX, const _ulong& dwCntZ);
	_bool		Is_InFrustumForObject(const _vec3* pPos, const _float fRadius = 0.f);

	void		Is_InFrustumForTerrain(const _vec3* pVtxPos,
										const _ulong& dwCntX,
										const _ulong& dwCntZ,
										Engine::INDEX32* pIndex,
										_ulong* pTriCnt);
	
private:
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	_bool					m_bClone;
	CFrustum*				m_pFrustum;
	CQuadTree*				m_pQuadTree;

public:
	static COptimization*		Create(LPDIRECT3DDEVICE9 pGraphicDev, 
										_bool bChoice, 
										const _ulong& dwCntX, 
										const _ulong& dwCntZ);
	virtual CComponent*			Clone(void);
	virtual void				Free(void);

};

END

#endif // Optimization_h__
