#ifndef Stage_h__
#define Stage_h__

#include "Defines.h"
#include "Scene.h"

#include "Terrain.h"
#include "DynamicCamera.h"
#include "SkyBox.h"
#include "Player.h"
#include "Dog.h"
#include "Soldier.h"
#include "Npc.h"
#include "NpcUI.h"
#include "Sword.h"
#include "MeshStatic.h"
#include "MeshDynamic.h"
#include "BossMap.h"
#include "MeshNavi.h"
#include"UI.h"
#include"Boss.h"
#include"Fountain.h"
#include"FountainPiece0.h"
#include"GhostWall.h"
#include"TreeMesh.h"


#include"CollisionMgr.h"

class CStage : public Engine::CScene
{
private:
	explicit CStage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStage(void);

public:
	virtual HRESULT Ready_Scene(void) override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void Render_Scene(void) override;
	
private:
	HRESULT			Ready_Layer_GameLogic(const _tchar* pLayerTag);
	HRESULT			Ready_LightInfo(void);
	HRESULT			Ready_Prototype(void);

	HRESULT			Ready_Layer_Setting(const _tchar* pLayerTag);
	void LoadData(const TCHAR * pFilePath);
private:
	_ulong			m_dwRenderCnt = 0;
	_tchar			m_szFPS[256];
	_float			m_fTime = 0.f;
	bool			m_bSound = false;
public:
	static CStage*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void) override;
	
};


#endif // Stage_h__
