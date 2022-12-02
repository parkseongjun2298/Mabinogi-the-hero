#ifndef MainApp_h__
#define MainApp_h__


#include "Base.h"
#include "Defines.h"
#include "Export_Function.h"
#include "Engine_Defines.h"

BEGIN(Engine)

class CGraphicDev;
class CManagement;

END
class CMainApp : public CBase
{
private:						// 생성자, 소멸자
	explicit CMainApp(void);
	virtual ~CMainApp(void);

public: // 함수, 변수
	HRESULT		Ready_MainApp(void);		// 함수 이름을 지을 경우, 형용사_명사
	_int		Update_MainApp(const _float& fTimeDelta);
	void		Render_MainApp(void);

protected: // 함수, 변수

private:
	HRESULT			Set_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev);
	HRESULT			Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev, 
								Engine::CManagement** ppManagementInstance);

private: // 함수, 변수

	LPDIRECT3DDEVICE9		m_pGraphicDev = nullptr;
	Engine::CGraphicDev*	m_pDeviceClass = nullptr;
	Engine::CManagement*	m_pManagementClass = nullptr;

	

public:  // 생성 함수
	static CMainApp*		Create(void);

private: // 소멸 함수
	virtual void			Free(void);

};

#endif // MainApp_h__

