#include "PrototypeMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CPrototypeMgr)

Engine::CPrototypeMgr::CPrototypeMgr(void)
{

}

Engine::CPrototypeMgr::~CPrototypeMgr(void)
{
	Free();
}

HRESULT Engine::CPrototypeMgr::Ready_Prototype(const _tchar* pProtoTag, CComponent* pInstance)
{
	CComponent*		pComponent = Find_Prototype(pProtoTag);

	if (nullptr != pComponent)
		return E_FAIL;

	m_mapProto.emplace(pProtoTag, pInstance);

	return S_OK;
}

CComponent* Engine::CPrototypeMgr::Clone_Prototype(const _tchar* pProtoTag)
{
	CComponent*		pInstance = Find_Prototype(pProtoTag);
	NULL_CHECK_RETURN(pInstance, NULL);

	return pInstance->Clone();
}

CComponent* Engine::CPrototypeMgr::Find_Prototype(const _tchar* pProtoTag)
{
	auto	iter = find_if(m_mapProto.begin(), m_mapProto.end(), CTag_Finder(pProtoTag));

	if (iter == m_mapProto.end())
		return nullptr;

	return iter->second;
}

void Engine::CPrototypeMgr::Free(void)
{
	for_each(m_mapProto.begin(), m_mapProto.end(), CDeleteMap());
	m_mapProto.clear();
}

