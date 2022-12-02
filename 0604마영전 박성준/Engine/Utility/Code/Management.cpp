#include "Export_Utility.h"

USING(Engine)
IMPLEMENT_SINGLETON(CManagement)

Engine::CManagement::CManagement(void)
{

}

Engine::CManagement::~CManagement(void)
{
	Free();
}

HRESULT Engine::CManagement::SetUp_Scene(CScene* pScene)
{
	if(nullptr != m_pScene)
		Safe_Release(m_pScene);

	Get_Renderer()->Clear_RenderGroup();

	m_pScene = pScene;
	m_pScene->Update_Scene(0.1);

	return S_OK;
}

_int Engine::CManagement::Update_Scene(const _float& fTimeDelta)
{
	if (nullptr == m_pScene)
		return -1;

	m_pScene->Update_Scene(fTimeDelta);

	return 0;
}

void Engine::CManagement::Render_Scene(LPDIRECT3DDEVICE9& pGraphicDev)
{
	Get_Renderer()->Render_GameObject(pGraphicDev);


	if (nullptr == m_pScene)
		return;

	m_pScene->Render_Scene();
}

void Engine::CManagement::Free(void)
{
	Safe_Release(m_pScene);
}
HRESULT Engine::CManagement::Add_GameObject(const _tchar* pLayerTag, const _tchar* pObjTag, CGameObject* pGameObject)
{
	return m_pScene->Add_GameObject(pLayerTag, pObjTag, pGameObject);
}
CComponent* Engine::CManagement::Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID)
{
	if (nullptr == m_pScene)
		return nullptr;

	return m_pScene->Get_Component(pLayerTag, pObjTag, pComponentTag, eID);
}
list<CGameObject*> CManagement::Get_ObjectList(const _tchar * pLayerTag, const _tchar * pObjTag)
{
	return m_pScene->Get_ObjectList(pLayerTag, pObjTag);
}

list<CGameObject*> CManagement::Get_Obj(const _tchar * pObjTag)
{
	return m_pScene->Get_ObjectList(L"GameLogic", pObjTag);
}

list<CGameObject*> CManagement::Get_Playeres()
{
	return m_pScene->Get_ObjectList(L"GameLogic", L"NaviMesh");
}

CGameObject * CManagement::Get_Camera()
{
	list<CGameObject*> pObjList = m_pScene->Get_ObjectList(L"GameLogic", L"DynamicCamera");

	return pObjList.front();
}

CNaviMesh* CManagement::Get_Navi()
{
	return dynamic_cast<CNaviMesh*>(m_pScene->Get_Component(L"GameSetting", L"NaviMesh", L"Com_Navi", ID_STATIC));
}

HRESULT Engine::CManagement::Ready_Management(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CShader*		pInstance = nullptr;

	pInstance = CShader::Create(pGraphicDev, L"../../Reference/Header/Shader_Sample.hpp");
	NULL_CHECK_RETURN(pInstance, E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Shader_Sample", pInstance), E_FAIL);

	pInstance = CShader::Create(pGraphicDev, L"../../Reference/Header/Shader_Terrain.hpp");
	NULL_CHECK_RETURN(pInstance, E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Shader_Terrain", pInstance), E_FAIL);

	pInstance = CShader::Create(pGraphicDev, L"../../Reference/Header/Shader_Mesh.hpp");
	NULL_CHECK_RETURN(pInstance, E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Shader_Mesh", pInstance), E_FAIL);

	pInstance = CShader::Create(pGraphicDev, L"../../Reference/Header/Shader_Ui.hpp");
	NULL_CHECK_RETURN(pInstance, E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Shader_Ui", pInstance), E_FAIL);

	pInstance = CShader::Create(pGraphicDev, L"../../Reference/Header/Shader_Trail.hpp");
	NULL_CHECK_RETURN(pInstance, E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(L"Shader_SwordTrail", pInstance), E_FAIL);

	


	return S_OK;
}

