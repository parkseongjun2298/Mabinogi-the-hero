#include "Frustum.h"
#include "QuadTree.h"

USING(Engine)

CFrustum::CFrustum(LPDIRECT3DDEVICE9 pGraphicDev)
	:m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

CFrustum::~CFrustum(void)
{

}

_bool CFrustum::Is_InFrustumForObject(const _vec3* pPos, const _float fRadius /*= 0.f*/)
{
	Ready_Frustum();

	_matrix		matProj, matView;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXMatrixInverse(&matView, NULL, &matView);

	for (_uint i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &matProj);
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &matView);
	}

	// ������ ��ǥ�� ��ȯ�� �Ϸ�
	// x+
	D3DXPlaneFromPoints(&m_Plane[0], &m_vPoint[1], &m_vPoint[5], &m_vPoint[6]);

	// x-
	D3DXPlaneFromPoints(&m_Plane[1], &m_vPoint[4], &m_vPoint[0], &m_vPoint[3]);

	// y+
	D3DXPlaneFromPoints(&m_Plane[2], &m_vPoint[4], &m_vPoint[5], &m_vPoint[1]);

	// y-
	D3DXPlaneFromPoints(&m_Plane[3], &m_vPoint[3], &m_vPoint[2], &m_vPoint[6]);

	// z+
	D3DXPlaneFromPoints(&m_Plane[4], &m_vPoint[7], &m_vPoint[6], &m_vPoint[5]);

	// z-
	D3DXPlaneFromPoints(&m_Plane[5], &m_vPoint[0], &m_vPoint[1], &m_vPoint[2]);

	return Is_InFrustum(pPos);
}

void CFrustum::Is_InFrustumForTerrain(const _vec3* pVtxPos, 
										const _ulong& dwCntX, 
										const _ulong& dwCntZ,
										Engine::INDEX32* pIndex,
										_ulong* pTriCnt,
										CQuadTree* pQuadTree)
{
	Ready_Frustum();

	_matrix		matProj, matView;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixInverse(&matProj, NULL, &matProj);
	D3DXMatrixInverse(&matView, NULL, &matView);

	for (_uint i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &matProj);
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &matView);
	}

	// ������ ��ǥ�� ��ȯ�� �Ϸ�
	// x+
	D3DXPlaneFromPoints(&m_Plane[0], &m_vPoint[1], &m_vPoint[5], &m_vPoint[6]);

	// x-
	D3DXPlaneFromPoints(&m_Plane[1], &m_vPoint[4], &m_vPoint[0], &m_vPoint[3]);

	// y+
	D3DXPlaneFromPoints(&m_Plane[2], &m_vPoint[4], &m_vPoint[5], &m_vPoint[1]);

	// y-
	D3DXPlaneFromPoints(&m_Plane[3], &m_vPoint[3], &m_vPoint[2], &m_vPoint[6]);

	// z+
	D3DXPlaneFromPoints(&m_Plane[4], &m_vPoint[7], &m_vPoint[6], &m_vPoint[5]);

	// z-
	D3DXPlaneFromPoints(&m_Plane[5], &m_vPoint[0], &m_vPoint[1], &m_vPoint[2]);

	*pTriCnt = 0;

	if (nullptr != pQuadTree)
		pQuadTree->Culling_ForTerrain(this, pVtxPos, pIndex, pTriCnt);

	else
	{
		_bool	bIn[3] = { false };
		_ulong	dwTriCnt = 0;

		for (_ulong i = 0; i < dwCntZ - 1; ++i)
		{
			for (_ulong j = 0; j < dwCntX - 1; ++j)
			{
				_ulong dwIndex = i * dwCntX + j;

				// ������ ��

				bIn[0] = Is_InFrustum(&pVtxPos[dwIndex + dwCntX]);
				bIn[1] = Is_InFrustum(&pVtxPos[dwIndex + dwCntX + 1]);
				bIn[2] = Is_InFrustum(&pVtxPos[dwIndex + 1]);

				if (true == bIn[0] || true == bIn[1] || true == bIn[2])
				{
					pIndex[dwTriCnt]._0 = dwIndex + dwCntX;
					pIndex[dwTriCnt]._1 = dwIndex + dwCntX + 1;
					pIndex[dwTriCnt]._2 = dwIndex + 1;
					++dwTriCnt;
				}

				// ���� �Ʒ�

				bIn[0] = Is_InFrustum(&pVtxPos[dwIndex + dwCntX]);
				bIn[1] = Is_InFrustum(&pVtxPos[dwIndex + 1]);
				bIn[2] = Is_InFrustum(&pVtxPos[dwIndex]);

				if (true == bIn[0] || true == bIn[1] || true == bIn[2])
				{
					pIndex[dwTriCnt]._0 = dwIndex + dwCntX;
					pIndex[dwTriCnt]._1 = dwIndex + 1;
					pIndex[dwTriCnt]._2 = dwIndex;
					++dwTriCnt;
				}

			}
		}

		*pTriCnt = dwTriCnt;
	}
}

HRESULT CFrustum::Ready_Frustum(void)
{
	m_vPoint[0] = _vec3(-1.f, 1.f, 0.f);
	m_vPoint[1] = _vec3(1.f, 1.f, 0.f);
	m_vPoint[2] = _vec3(1.f, -1.f, 0.f);
	m_vPoint[3] = _vec3(-1.f, -1.f, 0.f);

	m_vPoint[4] = _vec3(-1.f, 1.f, 1.f);
	m_vPoint[5] = _vec3(1.f, 1.f, 1.f);
	m_vPoint[6] = _vec3(1.f, -1.f, 1.f);
	m_vPoint[7] = _vec3(-1.f, -1.f, 1.f);

	return S_OK;
}

_bool CFrustum::Is_InFrustum(const _vec3* pPos)
{
	_float fDistance = 0.f;

	for (_uint i = 0; i < 6; ++i)
	{
		fDistance = D3DXPlaneDotCoord(&m_Plane[i], pPos);

		if (fDistance > 0.f)
			return false;
	}

	return true;

}

_bool CFrustum::Is_InFrustum(const _vec3* pPos, const _float& fRadius)
{
	_float fDistance = 0.f;

	for (_uint i = 0; i < 6; ++i)
	{
		fDistance = D3DXPlaneDotCoord(&m_Plane[i], pPos);

		if (fDistance > fRadius)
			return false;
	}

	return true;
}

CFrustum* CFrustum::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFrustum*	pInstance = new CFrustum(pGraphicDev);

	if (FAILED(pInstance->Ready_Frustum()))
		Safe_Release(pInstance);

	return pInstance;
}

void CFrustum::Free(void)
{
	Safe_Release(m_pGraphicDev);
}

