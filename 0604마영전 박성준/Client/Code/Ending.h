#ifndef Ending_h__
#define Ending_h__

#include "Defines.h"
#include "Scene.h"

#include "EndScene.h"

class CEnding : public Engine::CScene
{
private:
	explicit CEnding(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEnding(void);

public:
	virtual HRESULT Ready_Scene(void) override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void Render_Scene(void) override;

private:
	HRESULT			Ready_Layer_Setting(const _tchar * pLayerTag);
	HRESULT			Ready_Layer_GameLogic(const _tchar* pLayerTag);
	HRESULT			Ready_LightInfo(void);
	HRESULT			Ready_Prototype(void);

	
private:
	bool			m_bSound = false;
	

public:
	static CEnding*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free(void) override;

};


#endif // Ending_h__
