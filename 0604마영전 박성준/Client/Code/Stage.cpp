#include "stdafx.h"
#include "Stage.h"
#include"Ending.h"

#include "Export_Function.h"

CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CScene(pGraphicDev)
{
}

CStage::~CStage(void)
{
}

HRESULT CStage::Ready_Scene(void)
{
	//Engine::CLine3DMgr::GetInstance()->Ready_Line(m_pGraphicDev);
	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	FAILED_CHECK_RETURN(Engine::CScene::Ready_Scene(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Prototype(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Setting(L"GameSetting"), E_FAIL);

	Engine::CSoundMgr::GetInstance()->Initialize();
		
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	return S_OK;
}

_int CStage::Update_Scene(const _float& fTimeDelta)
{
	
	if (!m_bSound)
	{
		Engine::PlayBGM(L"잡몹브금.mp3");
		Engine::SetVolume(Engine::CHANNELID::CH_BGM, 0.1f);
		m_bSound = true;
		
	}
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"GameLogic"), E_FAIL);
	m_fTime += fTimeDelta;
	_int iExit = Engine::CScene::Update_Scene(fTimeDelta);
	if (GetAsyncKeyState('Y') & 0x0001)
	{
		Engine::CScene*		pScene = nullptr;

		pScene = CEnding::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pScene, E_FAIL);
		FAILED_CHECK_RETURN(Engine::SetUp_Scene(pScene), E_FAIL);
		return iExit;
	}
	Engine::SoundUpdate();

	return iExit;
}

void CStage::Render_Scene(void)
{
	// debug 코드 출력

	++m_dwRenderCnt;

	if (m_fTime >= 1.f)
	{
		//wsprintf(m_szFPS, L"FPS : %d", m_dwRenderCnt);
		m_fTime = 0.f;
		m_dwRenderCnt = 0;
	}

	//Engine::Render_Font(L"Font_Default", m_szFPS, &_vec2(10.f, 10.f), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));

}

CStage * CStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage *	pInstance = new CStage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CStage::Free(void)
{
	Engine::CScene::Free();
}
HRESULT CStage::Ready_Layer_Setting(const _tchar* pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = nullptr;

	// Navi
	pGameObject = CMeshNavi::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NaviMesh", pGameObject), E_FAIL);

	m_mapLayer.emplace(pLayerTag, pLayer);
}

HRESULT CStage::Ready_Layer_GameLogic(const _tchar* pLayerTag)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject*		pGameObject = nullptr;

	// DynamicCamera
	pGameObject = CDynamicCamera::Create(m_pGraphicDev, &_vec3(0.f, 4.f, -5.f), //eye at up
		&_vec3(0.f, 0.f, 0.f),
		&_vec3(0.f, 1.f, 0.f),
		D3DXToRadian(60.f), _float(WINCX) / WINCY, 0.1f, 10000.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DynamicCamera", pGameObject), E_FAIL);

	
	// SkyBox
	pGameObject = CSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkyBox", pGameObject), E_FAIL);

	//// Terrain
	//pGameObject = CTerrain::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Terrain", pGameObject), E_FAIL);

	// Player
	pGameObject = CPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", pGameObject), E_FAIL);

	// SWord
	pGameObject = CSword::Create(m_pGraphicDev, 0);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Sword", pGameObject), E_FAIL);

	

	// Map
	pGameObject = CBossMap::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Map", pGameObject), E_FAIL);

	// Fountain
	pGameObject = CFountain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Fountain", pGameObject), E_FAIL);


	// Guarder
	pGameObject = CGuarder::Create(m_pGraphicDev,34);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Guarder", pGameObject), E_FAIL);

	// Dog
	pGameObject = CDog::Create(m_pGraphicDev, 10);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Dog", pGameObject), E_FAIL);

	// Knight
	pGameObject = CKnight::Create(m_pGraphicDev,35);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Knight", pGameObject), E_FAIL);

	// Soldier
	pGameObject = CSoldier::Create(m_pGraphicDev,15);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Soldier", pGameObject), E_FAIL);



	// Npc
	pGameObject = CNpc::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Npc", pGameObject), E_FAIL);
	
	//GhostWall
	pGameObject = CGhostWall::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GhostWall", pGameObject), E_FAIL);

	_vec3 Temp = {-2.8f,0.f,-10.4f};
	//Tree
	pGameObject = CTreeMesh::Create(m_pGraphicDev, Temp);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TreeMesh", pGameObject), E_FAIL);
	
	 Temp = { -4.8f,0.f,-10.4f };
	//Tree
	pGameObject = CTreeMesh::Create(m_pGraphicDev, Temp);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TreeMesh", pGameObject), E_FAIL);

	Temp = { -6.8f,0.f,-10.4f };
	//Tree
	pGameObject = CTreeMesh::Create(m_pGraphicDev, Temp);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TreeMesh", pGameObject), E_FAIL);

	Temp = { -8.8f,0.f,-10.4f };
	//Tree
	pGameObject = CTreeMesh::Create(m_pGraphicDev, Temp);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TreeMesh", pGameObject), E_FAIL);

	Temp = { -8.8f,0.f,-8.4f };
	//Tree
	pGameObject = CTreeMesh::Create(m_pGraphicDev, Temp);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TreeMesh", pGameObject), E_FAIL);

	Temp = { -8.8f,0.f,-6.4f };
	//Tree
	pGameObject = CTreeMesh::Create(m_pGraphicDev, Temp);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TreeMesh", pGameObject), E_FAIL);

	Temp = { 3.f,0.f,-10.2f };
	//Tree
	pGameObject = CTreeMesh::Create(m_pGraphicDev, Temp);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TreeMesh", pGameObject), E_FAIL);

	Temp = { 5.f,0.f,-10.2f };
	//Tree
	pGameObject = CTreeMesh::Create(m_pGraphicDev, Temp);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TreeMesh", pGameObject), E_FAIL);

	Temp = { 7.f,0.f,-10.2f };
	//Tree
	pGameObject = CTreeMesh::Create(m_pGraphicDev, Temp);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TreeMesh", pGameObject), E_FAIL);

	Temp = { 9.f,0.f,-10.2f };
	//Tree
	pGameObject = CTreeMesh::Create(m_pGraphicDev, Temp);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TreeMesh", pGameObject), E_FAIL);

	Temp = { 9.f,0.f,-8.2f };
	//Tree
	pGameObject = CTreeMesh::Create(m_pGraphicDev, Temp);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TreeMesh", pGameObject), E_FAIL);

	Temp = { 9.f,0.f,-6.2f };
	//Tree
	pGameObject = CTreeMesh::Create(m_pGraphicDev, Temp);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TreeMesh", pGameObject), E_FAIL);

//==============================================
//Tree
	pGameObject = CTreeMesh::Create(m_pGraphicDev, Temp);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TreeMesh", pGameObject), E_FAIL);

	Temp = { -4.8f,0.f,8.2f };
	//Tree
	pGameObject = CTreeMesh::Create(m_pGraphicDev, Temp);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TreeMesh", pGameObject), E_FAIL);

	Temp = { -6.8f,0.f,8.2f };
	//Tree
	pGameObject = CTreeMesh::Create(m_pGraphicDev, Temp);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TreeMesh", pGameObject), E_FAIL);

	Temp = { -8.8f,0.f,8.2f };
	//Tree
	pGameObject = CTreeMesh::Create(m_pGraphicDev, Temp);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TreeMesh", pGameObject), E_FAIL);

	Temp = { -8.8f,0.f,6.2f };
	//Tree
	pGameObject = CTreeMesh::Create(m_pGraphicDev, Temp);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TreeMesh", pGameObject), E_FAIL);

	Temp = { -8.8f,0.f,3.3f };


	//Tree
	pGameObject = CTreeMesh::Create(m_pGraphicDev, Temp);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TreeMesh", pGameObject), E_FAIL);

	Temp = { 3.f,0.f,8.2f };
	//Tree
	pGameObject = CTreeMesh::Create(m_pGraphicDev, Temp);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TreeMesh", pGameObject), E_FAIL);

	Temp = { 5.f,0.f,8.2f };
	//Tree
	pGameObject = CTreeMesh::Create(m_pGraphicDev, Temp);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TreeMesh", pGameObject), E_FAIL);

	Temp = { 7.f,0.f,8.2f };
	//Tree
	pGameObject = CTreeMesh::Create(m_pGraphicDev, Temp);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TreeMesh", pGameObject), E_FAIL);

	Temp = { 9.f,0.f,6.2f };
	//Tree
	pGameObject = CTreeMesh::Create(m_pGraphicDev, Temp);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TreeMesh", pGameObject), E_FAIL);

	Temp = { 9.f,0.f,4.2f };
	//Tree
	pGameObject = CTreeMesh::Create(m_pGraphicDev, Temp);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TreeMesh", pGameObject), E_FAIL);

	Temp = { 9.f,0.f,2.7f };
	//Tree
	pGameObject = CTreeMesh::Create(m_pGraphicDev, Temp);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TreeMesh", pGameObject), E_FAIL);


	m_mapLayer.emplace(pLayerTag, pLayer);

	// StaticMesh
	LoadData(L"../../Data/StaticObjectData.dat");
	return S_OK;	
}

HRESULT CStage::Ready_LightInfo(void)
{
	D3DLIGHT9		tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;
	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	tLightInfo.Direction = _vec3(1.f, -1.f, 1.f);

	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0), E_FAIL);

	return S_OK;
}

HRESULT CStage::Ready_Prototype(void)
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

void CStage::LoadData(const TCHAR * pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, 0, nullptr,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	if (!Engine::CManagement::GetInstance()->Get_ObjectList(L"GameLogic", L"Mesh").empty())
	{
		{
			for (auto& Object : Engine::CManagement::GetInstance()->Get_ObjectList(L"GameLogic", L"Mesh"))
			{
				Object->Set_bIsDead();
			}
		}
	}

	int i = Engine::CManagement::GetInstance()->Get_ObjectList(L"GameLogic", L"Mesh").size();

	DWORD dwBytes = 0;
	Engine::MeshInfo tInfo = {};

	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(Engine::MeshInfo), &dwBytes, nullptr);

		if (0 == dwBytes)
			break;

		wstring wst = L"";
		_tchar tcr;

		while (true)
		{
			ReadFile(hFile, &tcr, sizeof(_tchar), &dwBytes, nullptr);
			if (0 == tcr)
				break;
			wst += tcr;
		}

		const _tchar* MeshName = wst.c_str();


		Engine::CGameObject* pObj = nullptr;   

		if (tInfo.eType == Engine::TYPE_STATIC)
		{
			pObj = CMeshStatic::Create(Engine::CGraphicDev::GetInstance()->GetDevice(), MeshName, tInfo.eType);
			pObj->Get_Transform()->m_vInfo[Engine::INFO_POS] = tInfo.vPos;
			pObj->Get_Transform()->m_vInfo[Engine::INFO_POS].y = tInfo.vPos.y-1.2f;

			pObj->Get_Transform()->m_vScale = tInfo.vScale;
			pObj->Get_Transform()->m_vAngle = tInfo.vRot;

			m_mapLayer[L"GameLogic"]->Add_GameObject(L"Mesh", pObj);
		}

		if (tInfo.eType == Engine::TYPE_DYNAMIC)
		{
			CString TempName = MeshName;

		


			pObj = CMeshDynamic::Create(Engine::CGraphicDev::GetInstance()->GetDevice(), MeshName);
			pObj->Get_Transform()->m_vInfo[Engine::INFO_POS] = tInfo.vPos;
			pObj->Get_Transform()->m_vScale = tInfo.vScale;
			pObj->Get_Transform()->m_vAngle = tInfo.vRot;
			m_mapLayer[L"GameLogic"]->Add_GameObject(L"Mesh", pObj);
		}
	}

	CloseHandle(hFile);
}