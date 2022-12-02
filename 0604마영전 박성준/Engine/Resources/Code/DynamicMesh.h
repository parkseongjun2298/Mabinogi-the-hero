#ifndef DynamicMesh_h__
#define DynamicMesh_h__

#include "Mesh.h"
#include "HierarchyLoader.h"
#include "AniCtrl.h"

BEGIN(Engine)

class ENGINE_DLL CDynamicMesh : public CMesh
{
private:
	explicit CDynamicMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDynamicMesh(const CDynamicMesh& rhs);
	virtual ~CDynamicMesh(void);

public:
	const D3DXFRAME_DERIVED*		Get_FrameByName(const char* pFrameName);

public:
	void		Set_Animation(const _uint& iIndex);
	void		Play_Animation(const _float& fTimeDelta);
	_bool		Is_AnimationEnd(void);

public:
	list<D3DXMESHCONTAINER_DERIVED*>	Get_MeshContainerList() { return m_MeshContainerList; }
	const _vec3*		Get_VtxPos(void) { return m_pVtxPos; }
	_ulong				Get_VtxCnt(void) const { return m_dwVtxCnt; }
	_ulong				Get_Stride(void) const { return m_dwStride; }
private:
	_ulong						m_dwVtxCnt;
	_ulong						m_dwStride;	   // 정점 중 pos값에 해당하는 메모리 공간에 접근하기 위해 사용할 변수
	_vec3*						m_pVtxPos;
	

public:
	HRESULT		Ready_Meshes(const _tchar* pFilePath, 
							const _tchar* pFileName);

	void		Render_Meshes(void);
	void		Render_Meshes(LPD3DXEFFECT& pEffect);

private:
	void		Update_FrameMatrix(D3DXFRAME_DERIVED* pFrame, 
									const _matrix* pParentMatrix);
	
	void		SetUp_FrameMatrixPointer(D3DXFRAME_DERIVED* pFrame);

private:
	D3DXFRAME*			m_pRootFrame;			// 첫 번째 뼈를 저장하기 위한 포인터
	CHierarchyLoader*	m_pLoader;
public:
	CAniCtrl*			m_pAniCtrl;

	list<D3DXMESHCONTAINER_DERIVED*>		m_MeshContainerList;

public:
	static CDynamicMesh*		Create(LPDIRECT3DDEVICE9 pGraphicDev, 
										const _tchar* pFilePath, 
										const _tchar* pFileName);
	virtual CResources*			Clone(void);

private:
	virtual void				Free(void);
};

END
#endif // DynamicMesh_h__
//
//typedef struct _D3DXFRAME
//{
//	LPSTR                   Name;					// 뼈의 이름을 보관하기 위한 문자열 포인터(아스키 코드)
//	D3DXMATRIX              TransformationMatrix;	// x파일 로드 시 뼈대가 지닌 최초의 상태 행렬 정보를 보관	
//
//	LPD3DXMESHCONTAINER     pMeshContainer;			// 뼈를 기준으로 생성하는 메쉬의 각종 정보를 보관하는 구조체
//
//	struct _D3DXFRAME       *pFrameSibling;			// 형제 뼈의 주소를 보관하는 포인터
//	struct _D3DXFRAME       *pFrameFirstChild;		// 자식 뼈의 주소를 보관하는 포인터
//} D3DXFRAME, *LPD3DXFRAME;
//
//
//typedef struct _D3DXMESHCONTAINER
//{
//	LPSTR                   Name;				// 메쉬 컨테이너의 이름을 저장하기 위한 문자열 포인터(그러나 거의 사용하지 않음)
//
//	D3DXMESHDATA            MeshData;			// 메쉬의 종류를 결정하기 위한 구조체
//
//	LPD3DXMATERIAL          pMaterials;			// 재질 정보를 저장하기 위한 포인터
//	LPD3DXEFFECTINSTANCE    pEffects;			// 메쉬 내에 있는 독특한 이펙트 정보를 담기 위한 포인터(그러나 우리는 사용할 수 없다)
//	DWORD                   NumMaterials;		// 재질의 개수
//	DWORD                  *pAdjacency;			// 인접하는 이웃의 정보를 접근하기 위해 주소를 담아두기 위한 포인터
//
//	LPD3DXSKININFO          pSkinInfo;			// 스키닝을 하기위한 컴객체(포폴을 만들 때 알아둬야 할 가장 중요한 멤버 변수)
//
//	struct _D3DXMESHCONTAINER *pNextMeshContainer;
//} D3DXMESHCONTAINER, *LPD3DXMESHCONTAINER;