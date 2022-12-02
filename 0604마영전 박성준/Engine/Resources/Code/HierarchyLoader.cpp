#include "HierarchyLoader.h"

USING(Engine)

Engine::CHierarchyLoader::CHierarchyLoader(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath)
	: m_pGraphicDev(pGraphicDev)
	, m_pPath(pPath)
{
	m_pGraphicDev->AddRef();
}

Engine::CHierarchyLoader::~CHierarchyLoader(void)
{
	
}

STDMETHODIMP Engine::CHierarchyLoader::CreateFrame(THIS_ LPCSTR Name,
													LPD3DXFRAME *ppNewFrame)
{
	D3DXFRAME_DERIVED*			pNewFrame = new D3DXFRAME_DERIVED;
	ZeroMemory(pNewFrame, sizeof(D3DXFRAME_DERIVED));
	
	Allocate_Name(&pNewFrame->Name, Name);

	pNewFrame->CombinedTransformMatrix = *D3DXMatrixIdentity(&pNewFrame->TransformationMatrix);

	*ppNewFrame = pNewFrame;

	return S_OK;
}

STDMETHODIMP Engine::CHierarchyLoader::CreateMeshContainer(THIS_ LPCSTR Name, 
															CONST D3DXMESHDATA *pMeshData, 
															CONST D3DXMATERIAL *pMaterials, 
															CONST D3DXEFFECTINSTANCE *pEffectInstances,
															DWORD NumMaterials,
															CONST DWORD *pAdjacency, 
															LPD3DXSKININFO pSkinInfo,
															LPD3DXMESHCONTAINER *ppNewMeshContainer)
{
	D3DXMESHCONTAINER_DERIVED*	pDerivedContainer = new D3DXMESHCONTAINER_DERIVED;
	ZeroMemory(pDerivedContainer, sizeof(D3DXMESHCONTAINER_DERIVED));

	Allocate_Name(&pDerivedContainer->Name, Name);

	pDerivedContainer->MeshData.Type = D3DXMESHTYPE_MESH;

	LPD3DXMESH pMesh = pMeshData->pMesh;
	
	_ulong dwNumFaces = pMesh->GetNumFaces();	// �޽��� ���� �������� ���� ��ȯ

	pDerivedContainer->pAdjacency = new _ulong[dwNumFaces * 3];
	memcpy(pDerivedContainer->pAdjacency, pAdjacency, sizeof(_ulong) * dwNumFaces * 3);

	_ulong	dwFVF = pMesh->GetFVF(); // �޽��� �����ϴ� ���ؽ��� fvf������ ��ȯ�ϴ� �Լ�

										  // �޽� FVF �Ӽ� �� NORMAL�� ���� ���
	if (!(dwFVF & D3DFVF_NORMAL))
	{
		pMesh->CloneMeshFVF(pMesh->GetOptions(),
							dwFVF | D3DFVF_NORMAL, 
							m_pGraphicDev, 
							&pDerivedContainer->MeshData.pMesh);

		D3DXComputeNormals(pDerivedContainer->MeshData.pMesh, 
							pDerivedContainer->pAdjacency);
	}
	else
	{
		pMesh->CloneMeshFVF(pMesh->GetOptions(),
							dwFVF, 
							m_pGraphicDev, 
							&pDerivedContainer->MeshData.pMesh);
	}


	pDerivedContainer->NumMaterials = (NumMaterials == 0 ? 1 : NumMaterials);

	pDerivedContainer->pMaterials = new D3DXMATERIAL[pDerivedContainer->NumMaterials];
	ZeroMemory(pDerivedContainer->pMaterials, sizeof(D3DXMATERIAL) * pDerivedContainer->NumMaterials);

	pDerivedContainer->ppTexture = new LPDIRECT3DTEXTURE9[pDerivedContainer->NumMaterials];
	ZeroMemory(pDerivedContainer->ppTexture, sizeof(LPDIRECT3DTEXTURE9) * pDerivedContainer->NumMaterials);

	if (0 != NumMaterials)
	{
		memcpy(pDerivedContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * pDerivedContainer->NumMaterials);

		for (_ulong i = 0; i < pDerivedContainer->NumMaterials; ++i)
		{
			_tchar		szFullPath[256] = L"";
			_tchar		szFileName[256] = L"";

			MultiByteToWideChar(CP_ACP, 
								0,
								pDerivedContainer->pMaterials[i].pTextureFilename, 
								strlen(pDerivedContainer->pMaterials[i].pTextureFilename),
								szFileName, 
								256);

			lstrcpy(szFullPath, m_pPath);
			lstrcat(szFullPath, szFileName);

			FAILED_CHECK_RETURN(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &pDerivedContainer->ppTexture[i]), E_FAIL);
		}
	}

	// ���� ������ ���� ���
	else
	{
		pDerivedContainer->pMaterials[0].MatD3D.Diffuse = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pDerivedContainer->pMaterials[0].MatD3D.Specular = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pDerivedContainer->pMaterials[0].MatD3D.Ambient = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pDerivedContainer->pMaterials[0].MatD3D.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pDerivedContainer->pMaterials[0].MatD3D.Power = 0.f;

		pDerivedContainer->ppTexture = nullptr;
	}	
	
	if (nullptr == pSkinInfo)
		return S_OK;

	pDerivedContainer->pSkinInfo = pSkinInfo;
	pDerivedContainer->pSkinInfo->AddRef();

	pDerivedContainer->MeshData.pMesh->CloneMeshFVF(pDerivedContainer->MeshData.pMesh->GetOptions(),
													pDerivedContainer->MeshData.pMesh->GetFVF(), 
													m_pGraphicDev, 
													&pDerivedContainer->pOriMesh);

	pDerivedContainer->dwNumBones = pDerivedContainer->pSkinInfo->GetNumBones(); // ���� ������ ��ȯ�ϴ� �Լ�(�޽� �����̳ʿ� ������ �ִ� ���� ����)

	pDerivedContainer->pFrameOffsetMatrix = new _matrix[pDerivedContainer->dwNumBones];
	ZeroMemory(pDerivedContainer->pFrameOffsetMatrix, sizeof(_matrix) * pDerivedContainer->dwNumBones);

	pDerivedContainer->ppFrameCombinedTransformMatrix = new _matrix*[pDerivedContainer->dwNumBones];
	ZeroMemory(pDerivedContainer->ppFrameCombinedTransformMatrix, sizeof(_matrix*) * pDerivedContainer->dwNumBones);

	pDerivedContainer->pRenderMatrix = new _matrix[pDerivedContainer->dwNumBones];
	ZeroMemory(pDerivedContainer->pRenderMatrix, sizeof(_matrix) * pDerivedContainer->dwNumBones);

	for (_ulong i = 0; i < pDerivedContainer->dwNumBones; ++i)
		pDerivedContainer->pFrameOffsetMatrix[i] = *pDerivedContainer->pSkinInfo->GetBoneOffsetMatrix(i); // �޽��� �׸��� ���� ����鿡�� �ε��� ���� �ο��Ǿ� �ִ�. �̷� �ε��� ������� ������� ��ȸ�ϸ� ��� ������ ������ �Լ�
			
	*ppNewMeshContainer = pDerivedContainer;

	return S_OK;
}

STDMETHODIMP Engine::CHierarchyLoader::DestroyFrame(THIS_ LPD3DXFRAME pFrameToFree)
{

	Safe_Delete_Array(pFrameToFree->Name);

	if (nullptr != pFrameToFree->pMeshContainer)
		DestroyMeshContainer(pFrameToFree->pMeshContainer);	

	if (nullptr != pFrameToFree->pFrameSibling)
		DestroyFrame(pFrameToFree->pFrameSibling);

	if (nullptr != pFrameToFree->pFrameFirstChild)
		DestroyFrame(pFrameToFree->pFrameFirstChild);

	Safe_Delete(pFrameToFree);

	return S_OK;
}

STDMETHODIMP Engine::CHierarchyLoader::DestroyMeshContainer(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	D3DXMESHCONTAINER_DERIVED*	pDerivedMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerToFree;

	for (_ulong i = 0; i < pDerivedMeshContainer->NumMaterials; ++i)
		Safe_Release(pDerivedMeshContainer->ppTexture[i]);

	Safe_Delete_Array(pDerivedMeshContainer->ppTexture);

	Safe_Delete_Array(pDerivedMeshContainer->Name);
	Safe_Delete_Array(pDerivedMeshContainer->pAdjacency);
	Safe_Delete_Array(pDerivedMeshContainer->pMaterials);

	Safe_Delete_Array(pDerivedMeshContainer->ppFrameCombinedTransformMatrix);
	Safe_Delete_Array(pDerivedMeshContainer->pFrameOffsetMatrix);
	Safe_Delete_Array(pDerivedMeshContainer->pRenderMatrix);

	Safe_Release(pDerivedMeshContainer->MeshData.pMesh);
	Safe_Release(pDerivedMeshContainer->pSkinInfo);
	Safe_Release(pDerivedMeshContainer->pOriMesh);

	Safe_Delete(pDerivedMeshContainer);

	return S_OK;
}

void Engine::CHierarchyLoader::Allocate_Name(char** ppName, const char* pFrameName)
{
	if (nullptr == pFrameName)
		return;

	_uint	iLength = strlen(pFrameName);

	*ppName = new char[iLength + 1];

	strcpy_s(*ppName, iLength + 1, pFrameName);


}

CHierarchyLoader* Engine::CHierarchyLoader::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath)
{
	CHierarchyLoader*	pInstance = new CHierarchyLoader(pGraphicDev, pPath);
		
	return pInstance;
}

_ulong Engine::CHierarchyLoader::Release(void)
{
	Safe_Release(m_pGraphicDev);

	delete this;

	return 0;
}

