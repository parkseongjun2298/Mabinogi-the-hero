#include "stdafx.h"
#include "MainApp.h"

#include "Logo.h"
#include"KeyMgr.h"
CMainApp::CMainApp(void)
{

}

CMainApp::~CMainApp(void)
{
	
}

HRESULT CMainApp::Ready_MainApp(void)
{
	FAILED_CHECK_RETURN(Set_DefaultSetting(&m_pGraphicDev), E_FAIL);
	
	FAILED_CHECK_RETURN(Ready_Scene(m_pGraphicDev, &m_pManagementClass), E_FAIL);


	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

_int CMainApp::Update_MainApp(const _float& fTimeDelta)
{
	m_pManagementClass->Update_Scene(fTimeDelta);

	Engine::Set_InputDev();
	Engine::CKeyMgr::GetInstance()->Update();

	return 0;
}

void CMainApp::Render_MainApp(void)
{
	
	Engine::Render_Begin(D3DCOLOR_ARGB(255, 0, 0, 0));//버퍼 색

	m_pManagementClass->Render_Scene(m_pGraphicDev);

	Engine::Render_End();
}

HRESULT CMainApp::Set_DefaultSetting(LPDIRECT3DDEVICE9 * ppGraphicDev)
{
	FAILED_CHECK_RETURN(Engine::Ready_GraphicDev(g_hWnd,
		Engine::MODE_WIN,
		WINCX,
		WINCY,
		&m_pDeviceClass),
		E_FAIL);

	m_pDeviceClass->AddRef();

	(*ppGraphicDev) = m_pDeviceClass->GetDevice();
	(*ppGraphicDev)->AddRef();
	
	// 폰트
	FAILED_CHECK_RETURN(Engine::Ready_Font(m_pGraphicDev, L"Font_Default", L"바탕", 20, 35, FW_HEAVY), E_FAIL);

	// 키입력
	FAILED_CHECK_RETURN(Engine::Ready_InputDev(g_hInst, g_hWnd), E_FAIL);

	return S_OK;
}


HRESULT CMainApp::Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CManagement ** ppManagementInstance)
{
	Engine::CScene*		pScene = nullptr;
	
	FAILED_CHECK_RETURN(Engine::Create_Management(pGraphicDev, ppManagementInstance), E_FAIL);
	(*ppManagementInstance)->AddRef();

	pScene = CLogo::Create(pGraphicDev);
	NULL_CHECK_RETURN(pScene, E_FAIL);
	
	FAILED_CHECK_RETURN((*ppManagementInstance)->SetUp_Scene(pScene), E_FAIL);
	
	return S_OK;
}

CMainApp* CMainApp::Create(void)
{
	CMainApp*	pInstance = new CMainApp;

	if (FAILED(pInstance->Ready_MainApp()))
		Safe_Release(pInstance);
	
	return pInstance;
}

void CMainApp::Free(void)
{
	Safe_Release(m_pGraphicDev);

	Engine::Safe_Release(m_pDeviceClass);
	Engine::Safe_Release(m_pManagementClass);
	
	Engine::Release_Utility();
	Engine::Release_Resources();
	Engine::Release_System();
}

