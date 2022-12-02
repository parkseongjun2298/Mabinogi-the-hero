#include "LightMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CLightMgr)


Engine::CLightMgr::CLightMgr(void)
{

}

Engine::CLightMgr::~CLightMgr(void)
{
	Free();
}

HRESULT Engine::CLightMgr::Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex)
{
	CLight*		pInstance = CLight::Create(pGraphicDev, pLightInfo, iIndex);
	NULL_CHECK_RETURN(pInstance, E_FAIL);

	m_vecLight.push_back(pInstance);

	return S_OK;
}

void Engine::CLightMgr::Free(void)
{
	for_each(m_vecLight.begin(), m_vecLight.end(), CDeleteObj());
	m_vecLight.clear();
}

const D3DLIGHT9* Engine::CLightMgr::Get_Light(const _uint& iIndex)
{
	return m_vecLight[iIndex]->Get_Light();
}

