#include "Layer.h"

USING(Engine)

Engine::CLayer::CLayer(void)
{

}

Engine::CLayer::~CLayer(void)
{

}

HRESULT Engine::CLayer::Ready_Layer(void)
{
	return S_OK;
}

_int Engine::CLayer::Update_Layer(const _float& fTimeDelta)
{
	for (auto& MyPair : m_mapObject)
	{
		auto& iter = MyPair.second.begin();

		for (; iter != MyPair.second.end();)
		{
			int Dead = (*iter)->Update_GameObject(fTimeDelta);
			if (Dead == 1)
			{
				Safe_Release(*iter);
				iter = MyPair.second.erase(iter);
			
			}
			else
				++iter;
		}
	}

	return 0;
}

void Engine::CLayer::Render_Layer(void)
{
	
}

CLayer* Engine::CLayer::Create(void)
{
	CLayer*	pInstance = new CLayer;

	if (FAILED(pInstance->Ready_Layer()))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CLayer::Free(void)
{
	for (auto& MyPair : m_mapObject)
	{
		for (auto& pObject : MyPair.second)
			Safe_Release(pObject);

		MyPair.second.clear();
	}
	m_mapObject.clear();
}

HRESULT Engine::CLayer::Add_GameObject(const _tchar* pObjTag, CGameObject* pGameObject)
{
	if (nullptr == pGameObject)
		return E_FAIL;

	m_mapObject[pObjTag].push_back(pGameObject);

	return S_OK;
}

CComponent* Engine::CLayer::Get_Component(const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID)
{
	auto	iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_Finder(pObjTag));

	if (iter == m_mapObject.end())
		return nullptr;

	return iter->second.front()->Get_Component(pComponentTag, eID);
}
list<CGameObject*> CLayer::Get_ObjectList(const _tchar* pObjTag)
{
	return m_mapObject[pObjTag];
}

