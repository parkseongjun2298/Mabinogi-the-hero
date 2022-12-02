#include "Scene.h"

USING(Engine)

Engine::CScene::CScene(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Engine::CScene::~CScene(void)
{

}

HRESULT Engine::CScene::Ready_Scene(void)
{
	return S_OK;
}

_int Engine::CScene::Update_Scene(const _float& fTimeDelta)
{
	_int iExit = 0;

	for (auto& iter : m_mapLayer)
	{
		iExit = iter.second->Update_Layer(fTimeDelta);

		if (iExit & 0x80000000)
			return iExit;
	}

	return iExit;
}

void Engine::CScene::Render_Scene(void)
{
	
}


void Engine::CScene::Free(void)
{
	for_each(m_mapLayer.begin(), m_mapLayer.end(), CDeleteMap());
	m_mapLayer.clear();

	Safe_Release(m_pGraphicDev);
}
HRESULT Engine::CScene::Add_GameObject(const _tchar* pLayerTag, const _tchar* pObjTag, CGameObject* pGameObject)
{
	auto	iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(pLayerTag));

	if (iter == m_mapLayer.end())
		return E_FAIL;

	return iter->second->Add_GameObject(pObjTag, pGameObject);
}
CComponent* Engine::CScene::Get_Component(const _tchar* pLayerTag, 
										const _tchar* pObjTag, 
										const _tchar* pComponentTag, 
										COMPONENTID eID)
{
	auto	iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(pLayerTag));

	if (iter == m_mapLayer.end())
		return nullptr;


	return iter->second->Get_Component(pObjTag, pComponentTag, eID);
}

list<CGameObject*> CScene::Get_ObjectList(const _tchar * pLayerTag, const _tchar * pObjTag)
{
	auto	iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(pLayerTag));


	return iter->second->Get_ObjectList(pObjTag);
}

