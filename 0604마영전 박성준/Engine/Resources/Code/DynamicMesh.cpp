#include "DynamicMesh.h"

USING(Engine)

Engine::CDynamicMesh::CDynamicMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMesh(pGraphicDev)
	, m_pRootFrame(nullptr)
	, m_pLoader(nullptr)
	, m_pAniCtrl(nullptr)
{

}

Engine::CDynamicMesh::CDynamicMesh(const CDynamicMesh& rhs)
	: CMesh(rhs)
	, m_pRootFrame(rhs.m_pRootFrame)
	, m_pLoader(rhs.m_pLoader)
	, m_MeshContainerList(rhs.m_MeshContainerList)
{
	m_pAniCtrl = CAniCtrl::Create(*rhs.m_pAniCtrl);
}

Engine::CDynamicMesh::~CDynamicMesh(void)
{

}

const D3DXFRAME_DERIVED* Engine::CDynamicMesh::Get_FrameByName(const char* pFrameName)
{
	return (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pFrameName);
}

void CDynamicMesh::Set_Animation(const _uint & iIndex)
{
	m_pAniCtrl->Set_Animation(iIndex);
}

void CDynamicMesh::Play_Animation(const _float & fTimeDelta)
{
	m_pAniCtrl->Play_Animation(fTimeDelta);

	_matrix matTemp;
	Update_FrameMatrix((D3DXFRAME_DERIVED*)m_pRootFrame, D3DXMatrixRotationY(&matTemp, D3DXToRadian(180.f)));
}

void Engine::CDynamicMesh::Render_Meshes(LPD3DXEFFECT& pEffect)
{
	for (auto& iter : m_MeshContainerList)
	{
		D3DXMESHCONTAINER_DERIVED*		pMeshContainer = iter;

		for (_ulong i = 0; i < pMeshContainer->dwNumBones; ++i)
		{
			pMeshContainer->pRenderMatrix[i] = pMeshContainer->pFrameOffsetMatrix[i]
				* (*pMeshContainer->ppFrameCombinedTransformMatrix[i]);
		}

		void*	pSrcVtx = nullptr;
		void*	pDestVtx = nullptr;

		pMeshContainer->pOriMesh->LockVertexBuffer(0, &pSrcVtx);
		pMeshContainer->MeshData.pMesh->LockVertexBuffer(0, &pDestVtx);

		pMeshContainer->pSkinInfo->UpdateSkinnedMesh(pMeshContainer->pRenderMatrix, nullptr, pSrcVtx, pDestVtx);
	
		for (_ulong i = 0; i < pMeshContainer->NumMaterials; ++i)
		{
			//m_pGraphicDev->SetTexture(0, pMeshContainer->ppTexture[i]);

			pEffect->SetTexture("g_BaseTexture", pMeshContainer->ppTexture[i]);
			pEffect->CommitChanges();	// 텍스쳐 출력 시 변경된 사항에 따라 다시 갱신하는 함수

			pMeshContainer->MeshData.pMesh->DrawSubset(i);
		}

		pMeshContainer->pOriMesh->UnlockVertexBuffer();
		pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();

	}
}

_bool Engine::CDynamicMesh::Is_AnimationEnd(void)
{
	return m_pAniCtrl->Is_AnimationEnd();
}

HRESULT Engine::CDynamicMesh::Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName)
{
	_tchar	szFullPath[256] = L"";

	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	m_pLoader = CHierarchyLoader::Create(m_pGraphicDev, pFilePath);
	NULL_CHECK_RETURN(m_pLoader, E_FAIL);

	LPD3DXANIMATIONCONTROLLER		pAniCtrl = nullptr;

	FAILED_CHECK_RETURN(D3DXLoadMeshHierarchyFromX(szFullPath,
													D3DXMESH_MANAGED,
													m_pGraphicDev,
													m_pLoader,			// hierarchy
													NULL,
													&m_pRootFrame,			
													&pAniCtrl),			// aniCtrl
													E_FAIL);

	m_pAniCtrl = CAniCtrl::Create(pAniCtrl);
	NULL_CHECK_RETURN(m_pAniCtrl, E_FAIL);

	Engine::Safe_Release(pAniCtrl);

	_matrix		matTemp;

	Update_FrameMatrix((D3DXFRAME_DERIVED*)m_pRootFrame, D3DXMatrixRotationY(&matTemp, D3DXToRadian(180.f)));

	SetUp_FrameMatrixPointer((D3DXFRAME_DERIVED*)m_pRootFrame);

	return S_OK;
}


void Engine::CDynamicMesh::Update_FrameMatrix(D3DXFRAME_DERIVED* pFrame,
												const _matrix* pParentMatrix)
{
	if (nullptr == pFrame)
		return;

	pFrame->CombinedTransformMatrix = pFrame->TransformationMatrix * *(pParentMatrix);

	if (nullptr != pFrame->pFrameSibling)
		Update_FrameMatrix((D3DXFRAME_DERIVED*)pFrame->pFrameSibling, pParentMatrix);
	
	if (nullptr != pFrame->pFrameFirstChild)
		Update_FrameMatrix((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild, &pFrame->CombinedTransformMatrix);
}

CDynamicMesh* Engine::CDynamicMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName)
{
	CDynamicMesh*	pInstance = new CDynamicMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_Meshes(pFilePath, pFileName)))
		Safe_Release(pInstance);

	return pInstance;
}

CResources* Engine::CDynamicMesh::Clone(void)
{
	return new CDynamicMesh(*this);
}

void Engine::CDynamicMesh::Free(void)
{
	CMesh::Free();

	Safe_Release(m_pAniCtrl);

	if (false == m_bClone)
	{
		m_pLoader->DestroyFrame(m_pRootFrame);
		Safe_Release(m_pLoader);
	}

	m_MeshContainerList.clear();
}

void Engine::CDynamicMesh::SetUp_FrameMatrixPointer(D3DXFRAME_DERIVED* pFrame)
{
	if (nullptr != pFrame->pMeshContainer)
	{
		D3DXMESHCONTAINER_DERIVED*		pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pFrame->pMeshContainer;

		for (_ulong i = 0; i < pMeshContainer->dwNumBones; ++i)
		{
			const char* pBoneName = pMeshContainer->pSkinInfo->GetBoneName(i);
			D3DXFRAME_DERIVED*		pBoneFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pBoneName);
			pMeshContainer->ppFrameCombinedTransformMatrix[i] = &pBoneFrame->CombinedTransformMatrix;
		}

		m_MeshContainerList.push_back(pMeshContainer);
	}

	if (nullptr != pFrame->pFrameSibling)
		SetUp_FrameMatrixPointer((D3DXFRAME_DERIVED*)pFrame->pFrameSibling);

	if (nullptr != pFrame->pFrameFirstChild)
		SetUp_FrameMatrixPointer((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild);

}

void Engine::CDynamicMesh::Render_Meshes(void)
{
	for (auto& iter : m_MeshContainerList)
	{
		D3DXMESHCONTAINER_DERIVED*		pMeshContainer = iter;

		for (_ulong i = 0; i < pMeshContainer->dwNumBones; ++i)
		{
			pMeshContainer->pRenderMatrix[i] = pMeshContainer->pFrameOffsetMatrix[i] 
											 * (*pMeshContainer->ppFrameCombinedTransformMatrix[i]);
		}

		void*	pSrcVtx = nullptr;
		void*	pDestVtx = nullptr;

		pMeshContainer->pOriMesh->LockVertexBuffer(0, &pSrcVtx);
		pMeshContainer->MeshData.pMesh->LockVertexBuffer(0, &pDestVtx);

		pMeshContainer->pSkinInfo->UpdateSkinnedMesh(pMeshContainer->pRenderMatrix, nullptr, pSrcVtx, pDestVtx);
		// 소프트웨어 스키닝을 수행하는 함수(스키닝 뿐 아니라 애니메이션 변경 시, 뼈대들과 정점 정보들의 변경을 동시에 수행)
		// 1인자 : 뼈의 변환 상태 행렬 주소
		// 2인자 : 원본 상태로 돌리기 위한 상태 행렬(원래는 역행렬을 싹 다 구해서 넣어줘야 하지만 굳이 안넣어줘도 상관 없음)
		// 3인자 : 변하지 않는 원본 메쉬의 정점 정보
		// 4인자 : 변한된 메쉬의 정점 정보

		for (_ulong i = 0; i < pMeshContainer->NumMaterials; ++i)
		{
			m_pGraphicDev->SetTexture(0, pMeshContainer->ppTexture[i]);
			pMeshContainer->MeshData.pMesh->DrawSubset(i);
		}

		pMeshContainer->pOriMesh->UnlockVertexBuffer();
		pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();

	}
}
