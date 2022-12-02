#include "stdafx.h"
#include "Loading.h"

#include "Export_Function.h"

CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_bFinish(false)
{
	m_pGraphicDev->AddRef();
}

CLoading::~CLoading(void)
{

}

HRESULT CLoading::Ready_Loading(LOADINGID eLoadingID)
{
	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, Thread_Main, this, 0, NULL);

	m_eLoadingID = eLoadingID;

	return S_OK;
}

_uint CLoading::Loading_ForStage(void)
{
	//lstrcpy(m_szLoading, L"Buffer Loading................");


	// TerrainTex
	FAILED_CHECK_RETURN(Engine::Ready_Buffers(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Buffer_TerrainTex",
		Engine::BUFFER_TERRAINTEX, 129, 129, 1),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Buffers(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Buffer_CubeTex",
		Engine::BUFFER_CUBETEX),
		E_FAIL);

	m_iLoading = 10;
	//lstrcpy(m_szLoading, L"Texture Loading................");
	

	// Texture


	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Texture_Terrain",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/Terrain/Grass_%d.tga", 2),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Texture_Cube",
		Engine::TEX_CUBE,
		L"../Bin/Resource/Texture/SkyBox/SkyBox%d.dds", 2),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Texture_Effect",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/Effect/Dash_Effect/aming_point_big0%d.tga",1),
		E_FAIL);
	
	

	/*FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Texture_Effect",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/Explosion/Explosion%d.png", 90),
		E_FAIL);*/

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Texture_NpcUI",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/UI/dwyn_0.png"),
		E_FAIL);

	
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Texture_UI",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/UI/QuestEp0.png"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Texture_TextUI",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/UI/TextDialog2.png"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Texture_QuestComplete",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/UI/QuestComplete.png"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Texture_PlayerHpBar_Back",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/PlayerHp/PlayerHpBar_Back.png"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Texture_PlayerHpBar_Value",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/PlayerHp/PlayerHpBar_Value.png"),
		E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Texture_PlayerSpBar_Back",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/PlayerHp/PlayerStaminaBar_Back.png"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Texture_PlayerSpBar_Value",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/PlayerHp/PlayerStaminaBar_ValueBar.png"),
		E_FAIL);

	//FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
	//	RESOURCE_STATIC,
	//	L"Texture_Filter",
	//	Engine::TEX_NORMAL,
	//	L"../Bin/Resource/Texture/Terrain/Filter.bmp"),
	//	E_FAIL);

	//FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
	//	RESOURCE_STATIC,
	//	L"Texture_Aura",
	//	Engine::TEX_NORMAL,
	//	L"../Bin/Resource/Texture/Terrain/Aura0.tga"),
	//	E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Texture_BossHpBar_Green",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/BossHp/BossHpBar_0.png"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Texture_BossHpBar_Yellow",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/BossHp/BossHpBar_1.png"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Texture_BossHpBar_Red",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/BossHp/BossHpBar_2.png"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Texture_BossHpBar_Black",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/BossHp/BossHpBar_Base.png"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Texture_BossName",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/BossHp/BossName_ingkells.png"),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Texture_X",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/BossHp/output/Font_Boss_Life_Number_011.png"),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Texture_0",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/BossHp/output/Font_Boss_Life_Number_001.png"),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Texture_1",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/BossHp/output/Font_Boss_Life_Number_002.png"),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Texture_2",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/BossHp/output/Font_Boss_Life_Number_003.png"),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Texture_3",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/BossHp/output/Font_Boss_Life_Number_004.png"),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Texture_4",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/BossHp/output/Font_Boss_Life_Number_005.png"),
		E_FAIL);


	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Texture_LordOfManaBot",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/Effect/aming_point_big01.tga"),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Texture_FakeSdk",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/Effect/aming_point_big00.png"),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Texture_White",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/Effect/white.tga"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Texture_Hit",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/Effect/efx_sword_slashpoint.tga"),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Texture_BlueFire",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/Effect/shape_fire_06.tga"),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Texture_BlueFire2",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/Effect/shape_fire_10.tga"),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Texture_GhostWall",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/Effect/ghostwall_b.tga"),
		E_FAIL);
	


	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Texture_Dissolve",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/dissolve.png"),
		E_FAIL);

	

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Texture_LowHpScreen",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/Screen/lowHpScreen.png"),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Texture_Ending",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/Ending.jpg"),
		E_FAIL);


	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Texture_Dust",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/Effect/dust.tga"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Texture_Skill_Ready",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/SkillIcon/skill_slot.png"),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Texture_Skill2",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/SkillIcon/Skill2.tga"),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Texture_Skill4",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/SkillIcon/Skill4.tga"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Texture_SwordTrail",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/Effect/heroes_longblade_trail03.tga"),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Texture_LongBlade",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/Effect/heroes_longblade_trail05.tga"),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Texture_SwordTrail2",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/Effect/trail_ingkells01.tga"),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Texture_SwordTrail3",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/SwordTrail_0.png"),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Texture_SwordTrail4",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/SwordTrail_1.png"),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Texture(m_pGraphicDev,
		RESOURCE_STATIC,
		L"Texture_LongBladeEffect",
		Engine::TEX_NORMAL,
		L"../Bin/Resource/Texture/Effect/blunt_yellow.tga"),
		E_FAIL);
	


	m_iLoading = 30;
	//lstrcpy(m_szLoading, L"Static Mesh Loading................");
	//static
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Fountain",
		Engine::TYPE_STATIC,
		L"../../Client/Bin/Resource/Mesh/StaticMesh/Mesh_Fountain/", L"Fountain.x"),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Fountain2",
		Engine::TYPE_STATIC,
		L"../../Client/Bin/Resource/Mesh/StaticMesh/Mesh_Fountain/", L"Fountain_0.x"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_FountainPiece0",
		Engine::TYPE_STATIC,
		L"../../Client/Bin/Resource/Mesh/StaticMesh/Mesh_FountainPiece/", L"Fountain_Dbr_0.x"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_FountainPiece1",
		Engine::TYPE_STATIC,
		L"../../Client/Bin/Resource/Mesh/StaticMesh/Mesh_FountainPiece/", L"Fountain_Dbr_1.x"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_FountainPiece2",
		Engine::TYPE_STATIC,
		L"../../Client/Bin/Resource/Mesh/StaticMesh/Mesh_FountainPiece/", L"Fountain_Dbr_2.x"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_FountainPiece3",
		Engine::TYPE_STATIC,
		L"../../Client/Bin/Resource/Mesh/StaticMesh/Mesh_FountainPiece/", L"Fountain_Dbr_3.x"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_FountainPiece4",
		Engine::TYPE_STATIC,
		L"../../Client/Bin/Resource/Mesh/StaticMesh/Mesh_FountainPiece/", L"Fountain_Dbr_4.x"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_FountainPiece5",
		Engine::TYPE_STATIC,
		L"../../Client/Bin/Resource/Mesh/StaticMesh/Mesh_FountainPiece/", L"Fountain_Dbr_5.x"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_FountainPiece6",
		Engine::TYPE_STATIC,
		L"../../Client/Bin/Resource/Mesh/StaticMesh/Mesh_FountainPiece/", L"Fountain_Dbr_6.x"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_FountainPiece7",
		Engine::TYPE_STATIC,
		L"../../Client/Bin/Resource/Mesh/StaticMesh/Mesh_FountainPiece/", L"Fountain_Dbr_7.x"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_FountainPiece8",
		Engine::TYPE_STATIC,
		L"../../Client/Bin/Resource/Mesh/StaticMesh/Mesh_FountainPiece/", L"Fountain_Dbr_8.x"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_FountainPiece9",
		Engine::TYPE_STATIC,
		L"../../Client/Bin/Resource/Mesh/StaticMesh/Mesh_FountainPiece/", L"Fountain_Dbr_9.x"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Gate",
		Engine::TYPE_STATIC,
		L"../../Client/Bin/Resource/Mesh/StaticMesh/Mesh_Gate/", L"OrtelGate.x"),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_TombStone",
		Engine::TYPE_STATIC,
		L"../../Client/Bin/Resource/Mesh/StaticMesh/Mesh_TombStone/", L"TombStone.x"),
		E_FAIL);


	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Weapon",
		Engine::TYPE_STATIC,
		L"../../Client/Bin/Resource/Mesh/StaticMesh/Mesh_Weapon/", L"WeaponArisha2.x"),
		E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Tree",
		Engine::TYPE_STATIC,
		L"../../Client/Bin/Resource/Mesh/StaticMesh/Mesh_Tree/", L"Tree.x"),
		E_FAIL);
	m_iLoading = 70;


	//lstrcpy(m_szLoading, L"Dynamic Mesh Loading................");
	//Dynamic
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Arisha",
		Engine::TYPE_DYNAMIC,
		L"../Bin/Resource/Mesh/DynamicMesh/Mesh_Arisha/", L"Arisha.x"),
		E_FAIL);
	m_iLoading = 90;
	
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Dog",
		Engine::TYPE_DYNAMIC,
		L"../Bin/Resource/Mesh/DynamicMesh/Mesh_Dog/", L"Dog_DashAtt.x"),
		E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Soldier",
		Engine::TYPE_DYNAMIC,
		L"../Bin/Resource/Mesh/DynamicMesh/Mesh_Soldier/", L"Soldier.x"),
		E_FAIL);
	m_iLoading = 100;
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Knight",
		Engine::TYPE_DYNAMIC,
		L"../Bin/Resource/Mesh/DynamicMesh/Mesh_Knight/", L"Knight_Att.x"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Guarder",
		Engine::TYPE_DYNAMIC,
		L"../Bin/Resource/Mesh/DynamicMesh/Mesh_Guarder/", L"Guarder.x"),
		E_FAIL);
	m_iLoading = 110;
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Npc",
		Engine::TYPE_DYNAMIC,
		L"../Bin/Resource/Mesh/DynamicMesh/Mesh_Npc/", L"NPC_Dwyn.x"),
		E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Ghost",
		Engine::TYPE_DYNAMIC,
		L"../Bin/Resource/Mesh/DynamicMesh/Mesh_Ghost/", L"ghost_idle.x"),
		E_FAIL);
	
	m_iLoading = 120;
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Ingkells",
		Engine::TYPE_DYNAMIC,
		L"../Bin/Resource/Mesh/DynamicMesh/Mesh_Ingkells/", L"Ingkells.x"),
		E_FAIL);
	m_iLoading = 140;
		// Map
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_BossMap",
		Engine::TYPE_STATIC,
		L"../Bin/Resource/Mesh/StaticMesh/CentralGarden/", L"CentralGarden.x"),
		E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_Meshes(m_pGraphicDev,
		RESOURCE_STAGE,
		L"Mesh_Navi",
		Engine::TYPE_NAVI,
		NULL, NULL),
		E_FAIL);
	m_bFinish = true;

	m_iLoading = 150;

	lstrcpy(m_szLoading, L"Loading Complete!!!!!");

	return 0;
}

unsigned int APIENTRY CLoading::Thread_Main(void* pArg)
{
	CLoading*		pLoading = (CLoading*)pArg;

	_uint iFlag = 0;

	EnterCriticalSection(pLoading->Get_Crt());

	switch (pLoading->Get_LoadingID())
	{
	case LOADING_STAGE:
		iFlag = pLoading->Loading_ForStage();
		break;

	case LOADING_BOSS:
		break;
	}

	LeaveCriticalSection(pLoading->Get_Crt());
	_endthreadex(0);

	return iFlag;
}

CLoading* CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eLoadingID)
{
	CLoading*	pInstance = new CLoading(pGraphicDev);

	if (FAILED(pInstance->Ready_Loading(eLoadingID)))
		Safe_Release(pInstance);

	return pInstance;
}

void CLoading::Free(void)
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);

	Safe_Release(m_pGraphicDev);
}

