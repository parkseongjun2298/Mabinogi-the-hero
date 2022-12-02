#include "stdafx.h"
#include "MeshNavi.h"

#include "Export_Function.h"

CMeshNavi::CMeshNavi(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{
}

CMeshNavi::~CMeshNavi(void)
{
}

HRESULT CMeshNavi::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	LoadData(L"../../Data/StageBoss1.Navi");

	return S_OK;
}

_int CMeshNavi::Update_GameObject(const _float & fTimeDelta)
{
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_Render_Group(Engine::RENDER_NONALPHA_TOOL, this);

	return NO_EVENT;
}

void CMeshNavi::Render_GameObject(void)
{
	if (GetAsyncKeyState(VK_TAB) & 0x8000)
		m_pNaviCom->Render_Meshes();
}

HRESULT CMeshNavi::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	// NaviMesh
	pComponent = m_pNaviCom = dynamic_cast<Engine::CNaviMesh*>(Engine::Clone_Prototype(L"Mesh_Navi"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Navi", pComponent);

	// renderer
	pComponent = m_pRendererCom = dynamic_cast<Engine::CRenderer*>(Engine::Clone_Prototype(L"Proto_Renderer"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);

	return S_OK;
}

CMeshNavi* CMeshNavi::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMeshNavi*	pInstance = new	CMeshNavi(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CMeshNavi::Free(void)
{
	Engine::CGameObject::Free();
}

void CMeshNavi::LoadData(const TCHAR * pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, 0, nullptr,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD dwBytes = 0;

	struct CellPoint3
	{
		_vec3 A;
		_vec3 B;
		_vec3 C;
		BYTE Option;
	};

	CellPoint3 CellPos;
	while (true)
	{
		ReadFile(hFile, &CellPos, sizeof(CellPoint3), &dwBytes, nullptr);

		if (0 == dwBytes)
			break;

		m_pNaviCom->Plus_Meshes(CellPos.A, CellPos.B, CellPos.C, CellPos.Option);
	}

	CloseHandle(hFile);
}
