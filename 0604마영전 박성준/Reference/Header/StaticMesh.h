#ifndef StaticMesh_h__
#define StaticMesh_h__

#include "Mesh.h"

BEGIN(Engine)

class ENGINE_DLL CStaticMesh : public CMesh
{
private:
	explicit CStaticMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CStaticMesh(const CStaticMesh& rhs);
	virtual ~CStaticMesh(void);

public:
	_vec3*		Get_VtxPos(void) {	return m_pVtxPos;	}
	_ulong				Get_VtxCnt(void) const { return m_dwVtxCnt; }
	_ulong				Get_Stride(void) const { return m_dwStride; }

	LPD3DXMESH			Get_Mesh(void) { return m_pMesh; }
public:
	HRESULT		Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName);
	void		Render_Meshes(void);
	void		Render_Meshes(LPD3DXEFFECT& pEffect);

private:
	_bool		Find_Alpha(const char* pFileName);

private:
	LPD3DXMESH					m_pMesh;		// 메쉬를 대표하는 컴 객체

	LPD3DXBUFFER				m_pAdjacency;	// 이웃하는 정점(폴리곤) 정보를 보관하기 위한 주소값(포인터)
	LPD3DXBUFFER				m_pSubsetBuffer; // 서브셋 정보를 저장하기 위한 포인터

	D3DXMATERIAL*				m_pSubsets;		// 재질 정보를 보관하기 위한 구조체 포인터
	_ulong						m_dwSubsetCnt;	// 서브셋 개수 == 텍스쳐 개수 == 재질의 개수

	LPDIRECT3DTEXTURE9*			m_ppTexture;	// 여러 개의 텍스쳐 정보를 보관하기 위해 이중포인터

	LPD3DXMESH					m_pOriMesh;		// collider와는 상관없음, 노말 정보를 삽입하기 전에 원본 메쉬

	_ulong						m_dwVtxCnt;
	_ulong						m_dwStride;	   // 정점 중 pos값에 해당하는 메모리 공간에 접근하기 위해 사용할 변수
	_vec3*						m_pVtxPos;


public:
	static	CStaticMesh*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
										const _tchar* pFilePath,
										const _tchar* pFileName);
public:
	virtual CResources*			Clone(void);
private:
	virtual void				Free(void);
};

END
#endif // StaticMesh_h__
