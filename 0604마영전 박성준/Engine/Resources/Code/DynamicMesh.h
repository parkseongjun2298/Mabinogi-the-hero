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
	_ulong						m_dwStride;	   // ���� �� pos���� �ش��ϴ� �޸� ������ �����ϱ� ���� ����� ����
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
	D3DXFRAME*			m_pRootFrame;			// ù ��° ���� �����ϱ� ���� ������
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
//	LPSTR                   Name;					// ���� �̸��� �����ϱ� ���� ���ڿ� ������(�ƽ�Ű �ڵ�)
//	D3DXMATRIX              TransformationMatrix;	// x���� �ε� �� ���밡 ���� ������ ���� ��� ������ ����	
//
//	LPD3DXMESHCONTAINER     pMeshContainer;			// ���� �������� �����ϴ� �޽��� ���� ������ �����ϴ� ����ü
//
//	struct _D3DXFRAME       *pFrameSibling;			// ���� ���� �ּҸ� �����ϴ� ������
//	struct _D3DXFRAME       *pFrameFirstChild;		// �ڽ� ���� �ּҸ� �����ϴ� ������
//} D3DXFRAME, *LPD3DXFRAME;
//
//
//typedef struct _D3DXMESHCONTAINER
//{
//	LPSTR                   Name;				// �޽� �����̳��� �̸��� �����ϱ� ���� ���ڿ� ������(�׷��� ���� ������� ����)
//
//	D3DXMESHDATA            MeshData;			// �޽��� ������ �����ϱ� ���� ����ü
//
//	LPD3DXMATERIAL          pMaterials;			// ���� ������ �����ϱ� ���� ������
//	LPD3DXEFFECTINSTANCE    pEffects;			// �޽� ���� �ִ� ��Ư�� ����Ʈ ������ ��� ���� ������(�׷��� �츮�� ����� �� ����)
//	DWORD                   NumMaterials;		// ������ ����
//	DWORD                  *pAdjacency;			// �����ϴ� �̿��� ������ �����ϱ� ���� �ּҸ� ��Ƶα� ���� ������
//
//	LPD3DXSKININFO          pSkinInfo;			// ��Ű���� �ϱ����� �İ�ü(������ ���� �� �˾Ƶ־� �� ���� �߿��� ��� ����)
//
//	struct _D3DXMESHCONTAINER *pNextMeshContainer;
//} D3DXMESHCONTAINER, *LPD3DXMESHCONTAINER;