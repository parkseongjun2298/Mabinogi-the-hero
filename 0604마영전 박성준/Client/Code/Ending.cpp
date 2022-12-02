#include "stdafx.h"
#include "Ending.h"


#include "Export_Function.h"

CEnding::CEnding(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
}

CEnding::~CEnding(void)
{
}

HRESULT CEnding::Ready_Scene(void)
{
	FAILED_CHECK_RETURN(Engine::CScene::Ready_Scene(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Setting(L"GameSetting"), E_FAIL);

	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	

	return S_OK;
}

_int CEnding::Update_Scene(const _float& fTimeDelta)
{
	if (!m_bSound)
	{
		Engine::StopAll();
		Engine::PlayBGM(L"엔딩씬.mp3");
		Engine::SetVolume(Engine::CHANNELID::CH_BGM, 0.1f);
		m_bSound = true;

	}
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"GameLogic"), E_FAIL);
	_int	iExit = Engine::CScene::Update_Scene(fTimeDelta);
		

		
	
	return iExit;
}

void CEnding::Render_Scene(void)
{
	// debug 코드 출력
	//Engine::Render_Font(L"Font_Default", m_pLoading->Get_String(), &_vec2(10.f, 10.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));

}

CEnding * CEnding::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEnding *	pInstance = new CEnding(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CEnding::Free(void)
{
	Engine::CScene::Free();

	
}



HRESULT CEnding::Ready_Layer_Setting(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = nullptr;

	

	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CEnding::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = nullptr;

	
	pGameObject = CEndScene::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"EndScene", pGameObject), E_FAIL);


	

	m_mapLayer.emplace(pLayerTag, pLayer);
	return S_OK;
}

HRESULT CEnding::Ready_LightInfo(void)
{
	return E_NOTIMPL;
}





HRESULT CEnding::Ready_Prototype(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = Engine::COptimization::Create(m_pGraphicDev, true, 129, 129);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	Engine::Ready_Prototype(L"Proto_Optimization", pComponent);

	/*pComponent = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	Engine::Ready_Prototype(L"Proto_Transform", pComponent);*/


	pComponent = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	Engine::Ready_Prototype(L"Proto_Renderer", pComponent);

	return S_OK;
}
