#include "NaviMesh.h"

USING(Engine)

Engine::CNaviMesh::CNaviMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMesh(pGraphicDev)
	, m_dwIndex(0)
{

}

Engine::CNaviMesh::CNaviMesh(const CNaviMesh& rhs)
	: CMesh(rhs)
	, m_dwIndex(rhs.m_dwIndex)
	, m_vecCell(rhs.m_vecCell)
{
	for (auto& iter : m_vecCell)
		iter->AddRef();
}

Engine::CNaviMesh::~CNaviMesh(void)
{

}

HRESULT Engine::CNaviMesh::Ready_Meshes(void)
{

	CCell*		pCell = nullptr;
	
	FAILED_CHECK_RETURN(Link_Cell(), E_FAIL);

	return S_OK;
}

void Engine::CNaviMesh::Render_Meshes(void)
{
	for (auto& iter : m_vecCell)
		iter->Render_Cell();
}
HRESULT CNaviMesh::Plus_Meshes(_vec3 v1, _vec3 v2, _vec3 v3, BYTE& byOption)
{
	CCell*		pCell = nullptr;

	pCell = CCell::Create(m_pGraphicDev,
		m_vecCell.size(),
		byOption,
		&v1,
		&v2,
		&v3);

	m_vecCell.push_back(pCell);

	FAILED_CHECK_RETURN(Link_Cell(), E_FAIL);
}
HRESULT Engine::CNaviMesh::Link_Cell(void)
{
	_ulong	dwVecSize = m_vecCell.size();

	for (_ulong i = 0; i < dwVecSize; ++i)
	{
		for (_ulong j = 0; j < dwVecSize; ++j)
		{
			if(i == j)
				continue;

			// 현재 이웃이 없고, 
			if (nullptr == m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_AB) &&
				true    == m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(CCell::POINT_A),
													   m_vecCell[i]->Get_Point(CCell::POINT_B), 
													   m_vecCell[i]))
			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_AB, m_vecCell[j]);
				continue;
			}

			if (nullptr == m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_BC) &&
					true == m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(CCell::POINT_B),
														m_vecCell[i]->Get_Point(CCell::POINT_C),
														m_vecCell[i]))
			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_BC, m_vecCell[j]);
				continue;
			}

			if (nullptr == m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_CA) &&
					true == m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(CCell::POINT_C),
														m_vecCell[i]->Get_Point(CCell::POINT_A),
														m_vecCell[i]))
			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_CA, m_vecCell[j]);
				continue;
			}
		}
	}

	return S_OK;
}

_vec3 Engine::CNaviMesh::Move_OnNaviMesh(const _vec3* pTargetPos, const _vec3* pTargetDir, _ulong* dwIndex)
{
	_vec3		vEndPos = *pTargetPos + *pTargetDir;
	Engine::CCell::COMPARE Temp= m_vecCell[*dwIndex]->Compare(&vEndPos, dwIndex);
	if (Temp==Engine::CCell::COMPARE_MOVE)
	{
		return vEndPos;
	}

	else if(Temp == Engine::CCell::COMPARE_STOP)
		return *pTargetPos;
}

_vec4 Engine::CNaviMesh::Move_OnNaviMesh(const _vec3* pTargetPos, const _vec3* pTargetDir) // 마지막인자는 스탑일때만 1
{
	_vec3		vEndPos = *pTargetPos + *pTargetDir;


	_vec3 SlidingDir = { 0,0,0 };
	_vec2 Normal;

	Engine::CCell::COMPARE Result = m_vecCell[m_dwIndex]->Compare(&vEndPos, &m_dwIndex, &Normal);
	if (Result == CCell::COMPARE_MOVE)
	{
		_vec4 EndPos = { vEndPos , 0.f };
		return EndPos;

	}
	else if (Result == CCell::COMPARE_STOP)
	{
		SlidingVector(Normal, pTargetDir, &SlidingDir);
		_vec4 EndPos = { (*pTargetPos + SlidingDir),1.f };

		return EndPos;
	}

	_vec4 EndPos = { vEndPos , 0.f };

	return EndPos;

}

CNaviMesh* Engine::CNaviMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNaviMesh*	pInstance = new CNaviMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_Meshes()))
		Safe_Release(pInstance);

	return pInstance;
}

CResources* Engine::CNaviMesh::Clone(void)
{
	return new CNaviMesh(*this);
}

void Engine::CNaviMesh::Free(void)
{
	CMesh::Free();

	for_each(m_vecCell.begin(), m_vecCell.end(), CDeleteObj());
	m_vecCell.clear();
}

void CNaviMesh::SlidingVector(const _vec2 & vNor, const _vec3 * vDir, _vec3 * vPout)
{

	_vec3 vLineNormal = { vNor.x,0,vNor.y };
	_vec3 vPlayerDir = *vDir;

	_vec3 SlidingVector = *vDir + (-vLineNormal * D3DXVec3Dot(&vPlayerDir, &vLineNormal));

	*vPout = SlidingVector;

}
