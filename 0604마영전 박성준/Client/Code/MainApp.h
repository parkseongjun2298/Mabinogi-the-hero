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
private:						// ������, �Ҹ���
	explicit CMainApp(void);
	virtual ~CMainApp(void);

public: // �Լ�, ����
	HRESULT		Ready_MainApp(void);		// �Լ� �̸��� ���� ���, �����_���
	_int		Update_MainApp(const _float& fTimeDelta);
	void		Render_MainApp(void);

protected: // �Լ�, ����

private:
	HRESULT			Set_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev);
	HRESULT			Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev, 
								Engine::CManagement** ppManagementInstance);

private: // �Լ�, ����

	LPDIRECT3DDEVICE9		m_pGraphicDev = nullptr;
	Engine::CGraphicDev*	m_pDeviceClass = nullptr;
	Engine::CManagement*	m_pManagementClass = nullptr;

	

public:  // ���� �Լ�
	static CMainApp*		Create(void);

private: // �Ҹ� �Լ�
	virtual void			Free(void);

};

#endif // MainApp_h__

