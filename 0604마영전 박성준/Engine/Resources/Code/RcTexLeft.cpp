#include "RcTexLeft.h"

USING(Engine)

Engine::CRcTexLeft::CRcTexLeft(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

Engine::CRcTexLeft::CRcTexLeft(const CRcTexLeft& rhs)
	: CVIBuffer(rhs)
{

}

Engine::CRcTexLeft::~CRcTexLeft(void)
{

}

HRESULT Engine::CRcTexLeft::Ready_Buffer(void)
{
	m_dwVtxCnt = 4;
	m_dwVtxFVF = FVF_TEX;
	m_dwTriCnt = 2;
	m_dwVtxSize = sizeof(VTXTEX);

	m_IdxFmt = D3DFMT_INDEX32;
	m_dwIdxSize = sizeof(INDEX32);

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);


	VTXTEX*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPos = _vec3(0.f, 0.5f, 0.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPos = _vec3(1.f, 0.5f, 0.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPos = _vec3(1.f, -0.5f, 0.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPos = _vec3(0.f, -0.5f, 0.f);
	pVertex[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();

	INDEX32*		pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pIB->Unlock();
	return S_OK;
}

Engine::CRcTexLeft* Engine::CRcTexLeft::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRcTexLeft*	pInstance = new CRcTexLeft(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
		Engine::Safe_Release(pInstance);


	return pInstance;
}

void Engine::CRcTexLeft::Free(void)
{
	CVIBuffer::Free();
}

Engine::CResources* Engine::CRcTexLeft::Clone(void)
{
	return new CRcTexLeft(*this);
}

