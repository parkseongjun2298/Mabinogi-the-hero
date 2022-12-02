#include "stdafx.h"
#include "Logo.h"
#include "Stage.h"

#include "Export_Function.h"

CLogo::CLogo(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
}

CLogo::~CLogo(void)
{
}

HRESULT CLogo::Ready_Scene(void)
{
	FAILED_CHECK_RETURN(Engine::CScene::Ready_Scene(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Resource(RESOURCE_END), E_FAIL);
	//FAILED_CHECK_RETURN(Ready_Prototype(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"GameLogic"), E_FAIL);

	m_pLoading = CLoading::Create(m_pGraphicDev, LOADING_STAGE);
	NULL_CHECK_RETURN(m_pLoading, E_FAIL);

	

	return S_OK;
}

_int CLogo::Update_Scene(const _float& fTimeDelta)
{
	_int	iExit = Engine::CScene::Update_Scene(fTimeDelta);

	m_iSaveLoadingBar=m_pLoading->Get_Loding();
	m_pLoadingBar->Get_Transform()->m_vScale.x = m_iSaveLoadingBar * 0.01f;

	if (true == m_pLoading->Get_Finish())
	{
		

		


		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		{
			Engine::CScene*		pScene = nullptr;

			pScene = CStage::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pScene, E_FAIL);
			FAILED_CHECK_RETURN(Engine::SetUp_Scene(pScene), E_FAIL);
			return iExit;
		}

	}
	return iExit;
}

void CLogo::Render_Scene(void)
{
	// debug 코드 출력
	//Engine::Render_Font(L"Font_Default", m_pLoading->Get_String(), &_vec2(10.f, 10.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));

}

CLogo * CLogo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLogo *	pInstance = new CLogo(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CLogo::Free(void)
{
	Engine::CScene::Free();

	Safe_Release(m_pLoading);
}

HRESULT CLogo::Ready_Resource(RESOURCEID eMax)
{
	FAILED_CHECK_RETURN(Engine::Reserve_ContainerSize(eMax), E_FAIL);

	// 버퍼 추가
	FAILED_CHECK_RETURN(Engine::Ready_Buffers(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Buffer_TriCol",
		Engine::BUFFER_TRICOL),
		E_FAIL);

	// RcTex
	FAILED_CHECK_RETURN(Engine::Ready_Buffers(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Buffer_RcTex",
		Engine::BUFFER_RCTEX),
		E_FAIL);
	// RcTex
	FAILED_CHECK_RETURN(Engine::Ready_Buffers(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Buffer_RcTexLeft",
		Engine::BUFFER_RCTEXLEFT),
		E_FAIL);


	// Texture
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Texture_Logo",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/Logo/Logo0.png"),
		E_FAIL);

	
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Texture_LoadingBar",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/Loading/LoadingBar.png"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Texture_LoadingBarBase",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/Loading/LoadingBack.png"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Texture_Logo2",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/Logo/EventDialog_1.png"),
		E_FAIL);
	
	return S_OK;
}

HRESULT CLogo::Ready_Layer_GameLogic(const _tchar* pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = nullptr;

	// backGround
	pGameObject = CBackGround::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BackGround", pGameObject), E_FAIL);

	//LoadingBarBase
	pGameObject = m_pLoadingBarBase = CLoadingBarBase::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LoadingBarBase", pGameObject), E_FAIL);

	//LoadingBar
	pGameObject = m_pLoadingBar= CLoadingBar::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LoadingBar", pGameObject), E_FAIL);
	
	// backGround
	pGameObject = CBackGround2::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BackGround2", pGameObject), E_FAIL);

	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;


}

//HRESULT CLogo::Ready_Prototype(void)
//{
//	Engine::CComponent*		pComponent = nullptr;
//
//	pComponent = Engine::CTransform::Create();
//	NULL_CHECK_RETURN(pComponent, E_FAIL);
//	Engine::Ready_Prototype(L"Proto_Transform", pComponent);
//
//	return S_OK;
//}