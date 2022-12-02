#ifndef NaviMesh_h__
#define NaviMesh_h__

#include "Mesh.h"
#include "Cell.h"

BEGIN(Engine)

class ENGINE_DLL CNaviMesh : public CMesh
{
private:
	explicit CNaviMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNaviMesh(const CNaviMesh& rhs);
	virtual ~CNaviMesh(void);

public:
	void	Set_Index(const _ulong& dwIndex) { m_dwIndex = dwIndex; }
	
	_vec4	Move_OnNaviMesh(const _vec3* pTargetPos,
		const _vec3* pTargetDir);

	_vec3	Move_OnNaviMesh(const _vec3* pTargetPos, 
							const _vec3* pTargetDir,
							_ulong* dwIndex);
public:
	HRESULT		Ready_Meshes(void);
	void		Render_Meshes(void);
	HRESULT		Plus_Meshes(_vec3 v1, _vec3 v2, _vec3 v3, BYTE& byOption);


private:
	HRESULT		Link_Cell(void);

public:
	vector<CCell*>			m_vecCell;
	_ulong					m_dwIndex;

public:
	static CNaviMesh*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CResources* Clone(void) override;
	virtual void Free(void) override;
	void SlidingVector(const _vec2 & vNor, const _vec3 * vDir, _vec3 * vPout);
};


END
#endif // NaviMesh_h__
