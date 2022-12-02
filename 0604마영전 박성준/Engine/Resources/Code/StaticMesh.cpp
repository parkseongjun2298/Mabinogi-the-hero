#include "StaticMesh.h"

USING(Engine)

Engine::CStaticMesh::CStaticMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMesh(pGraphicDev)
{

}

Engine::CStaticMesh::CStaticMesh(const CStaticMesh& rhs)
	: CMesh(rhs)
	, m_pMesh(rhs.m_pMesh)
	, m_pAdjacency(rhs.m_pAdjacency)
	, m_pSubsetBuffer(rhs.m_pSubsetBuffer)
	, m_pSubsets(rhs.m_pSubsets)
	, m_dwSubsetCnt(rhs.m_dwSubsetCnt)
	, m_pOriMesh(rhs.m_pOriMesh)
	, m_dwVtxCnt(rhs.m_dwVtxCnt)
	, m_dwStride(rhs.m_dwStride)
	, m_pVtxPos(rhs.m_pVtxPos)
{
	m_ppTexture = new LPDIRECT3DTEXTURE9[rhs.m_dwSubsetCnt];

	for (_ulong i = 0; i < rhs.m_dwSubsetCnt; ++i)
	{
		m_ppTexture[i] = rhs.m_ppTexture[i];
		m_ppTexture[i]->AddRef();
	}

	m_pMesh->AddRef();
	m_pOriMesh->AddRef();
	m_pAdjacency->AddRef();
	m_pSubsetBuffer->AddRef();
}

Engine::CStaticMesh::~CStaticMesh(void)
{

}

HRESULT Engine::CStaticMesh::Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName)
{
	_tchar	szFullPath[256] = L"";

	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	FAILED_CHECK_RETURN(D3DXLoadMeshFromX(szFullPath, 
											D3DXMESH_MANAGED, 
											m_pGraphicDev, 
											&m_pAdjacency,
											&m_pSubsetBuffer, 
											NULL,			// 메쉬가 지닌 특별한 이펙트 리소스의 주소 , 그러나 우리는 사용 할 수 없다
											&m_dwSubsetCnt, 
											&m_pOriMesh), 
											E_FAIL);

	_ulong	dwFVF = m_pOriMesh->GetFVF(); // 메쉬를 구성하는 버텍스의 fvf정보를 반환하는 함수

	// 메쉬 FVF 속성 중 NORMAL이 없는 경우
	if (!(dwFVF & D3DFVF_NORMAL))
	{
		m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(), dwFVF | D3DFVF_NORMAL, m_pGraphicDev, &m_pMesh);
		D3DXComputeNormals(m_pMesh, (_ulong*)m_pAdjacency->GetBufferPointer());
	}
	else
	{
		m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(), dwFVF, m_pGraphicDev, &m_pMesh);
	}

	//////////////////////////////////////////////////////////////////////////
	void*		pVertex = nullptr;

	m_dwVtxCnt = m_pMesh->GetNumVertices(); // 메쉬가 지닌 정점의 개수를 반환하는 함수
	m_pVtxPos = new _vec3[m_dwVtxCnt];

	m_pMesh->LockVertexBuffer(0, &pVertex);

	D3DVERTEXELEMENT9		Decl[MAX_FVF_DECL_SIZE]; // 메쉬 정점 안에 들어있는 FVF 정보를 저장하는 구조체
	ZeroMemory(Decl, sizeof(D3DVERTEXELEMENT9) * MAX_FVF_DECL_SIZE);

	m_pMesh->GetDeclaration(Decl); // 메쉬 정점 안에 들어있는 FVF 정보를 저장하는 구조체에 값을 얻어오는 함수

	_ubyte byOffset = 0;

	for (_ulong i = 0; i < MAX_FVF_DECL_SIZE; ++i)
	{
		if (Decl[i].Usage == D3DDECLUSAGE_POSITION)
		{
			byOffset = (_ubyte)Decl[i].Offset;
			break;
		}
	}

	// fvf옵션 값을 이용하여 메쉬 정점의 크기를 구함
	m_dwStride = D3DXGetFVFVertexSize(dwFVF);

	for (_ulong i = 0; i < m_dwVtxCnt; ++i)
	{
		m_pVtxPos[i] = *((_vec3*)(((_ubyte*)pVertex) + (i * m_dwStride + byOffset)));
	}
	
	// d3dxbuffer가 담고 있는 정보 중 첫 번째 주소를 반환하는 함수
	m_pSubsets = (D3DXMATERIAL*)m_pSubsetBuffer->GetBufferPointer();

	m_ppTexture = new LPDIRECT3DTEXTURE9[m_dwSubsetCnt];

	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		_tchar	szFileName[256] = L"";

		lstrcpy(szFullPath, pFilePath);
		
		MultiByteToWideChar(CP_ACP, 
							0, 
							m_pSubsets[i].pTextureFilename, 
							strlen(m_pSubsets[i].pTextureFilename), 
							szFileName, 
							256);

		lstrcat(szFullPath, szFileName);

		FAILED_CHECK_RETURN(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &m_ppTexture[i]), E_FAIL);
	}

	m_pMesh->UnlockVertexBuffer();
	
	return S_OK;
}

void Engine::CStaticMesh::Render_Meshes(void)
{
	for (_uint i = 0; i < m_dwSubsetCnt; ++i)
	{
		m_pGraphicDev->SetTexture(0, m_ppTexture[i]);
		m_pMesh->DrawSubset(i);
	}
}

void Engine::CStaticMesh::Render_Meshes(LPD3DXEFFECT& pEffect)
{
	for (_uint i = 0; i < m_dwSubsetCnt; ++i)
	{

		_bool	bAlpha = false;
		_uint	iPass = 0;

		if (bAlpha = Find_Alpha(m_pSubsets[i].pTextureFilename))
			iPass = 1;

		pEffect->SetTexture("g_BaseTexture", m_ppTexture[i]);
		pEffect->CommitChanges();
		
		pEffect->BeginPass(iPass);

		m_pMesh->DrawSubset(i);

		pEffect->EndPass();
	}
}

CStaticMesh* Engine::CStaticMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName)
{
	CStaticMesh*	pInstance = new CStaticMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_Meshes(pFilePath, pFileName)))
		Safe_Release(pInstance);

	return pInstance;
}

CResources* Engine::CStaticMesh::Clone(void)
{
	return new CStaticMesh(*this);
}

void Engine::CStaticMesh::Free(void)
{
	CMesh::Free();

	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
		Safe_Release(m_ppTexture[i]);

	Safe_Delete_Array(m_ppTexture);

	if (false == m_bClone)
		Safe_Delete_Array(m_pVtxPos);

	Safe_Release(m_pAdjacency);
	Safe_Release(m_pSubsetBuffer);
	Safe_Release(m_pMesh);
	Safe_Release(m_pOriMesh);
}


_bool CStaticMesh::Find_Alpha(const char* pFileName)
{
	_uint	iLength = strlen(pFileName);

	for (_uint i = 0; i < iLength; ++i)
	{
		if (pFileName[i] == '.')
		{
			if (pFileName[i - 1] == 'A')
				return true;
		}
	}

	return false;
}

