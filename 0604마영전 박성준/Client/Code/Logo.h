#ifndef Logo_h__
#define Logo_h__

#include "Defines.h"
#include "Scene.h"

#include "BackGround.h"
#include "BackGround2.h"
#include"LoadingBar.h"
#include "LoadingBarBase.h"
#include "Loading.h"

//#include "TestPlayer.h"
//#include "TestMonster.h"

class CLogo : public Engine::CScene
{
private:
	explicit CLogo(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLogo(void);

public:
	virtual HRESULT Ready_Scene(void) override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void Render_Scene(void) override;

private:
	HRESULT			Ready_Resource(RESOURCEID eMax);
	HRESULT			Ready_Layer_GameLogic(const _tchar* pLayerTag);

	//HRESULT Ready_Prototype(void);
private:
	CLoading*			m_pLoading = nullptr;
	CLoadingBar*		m_pLoadingBar = nullptr;
	CLoadingBarBase*		m_pLoadingBarBase = nullptr;
	CBackGround2*		m_pBackGround2 = nullptr;
	int					m_iSaveLoadingBar=0;

public:
	static CLogo*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void) override;

};


#endif // Logo_h__
