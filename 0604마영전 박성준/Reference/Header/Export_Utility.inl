#include "Export_Utility.h"
// Management
// Get
CComponent*		Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID)
{
	return CManagement::GetInstance()->Get_Component(pLayerTag, pObjTag, pComponentTag, eID);
}
HRESULT Add_GameObject(const _tchar * pLayerTag, const _tchar * pObjTag, CGameObject * pGameObject)
{
	return CManagement::GetInstance()->Add_GameObject(pLayerTag, pObjTag, pGameObject);
}
// Set
HRESULT		SetUp_Scene(CScene* pScene)
{
	return CManagement::GetInstance()->SetUp_Scene(pScene);
}
// General
HRESULT	Create_Management(LPDIRECT3DDEVICE9 pGraphicDev, CManagement** ppManagement)
{
	CManagement*		pManagement = CManagement::GetInstance();

	if (nullptr == pManagement)
		return E_FAIL;

	FAILED_CHECK_RETURN(pManagement->Ready_Management(pGraphicDev), E_FAIL);

	*ppManagement = pManagement;

	return S_OK;
}

// Renderer
// Get
// Set
// General
CRenderer*		Get_Renderer(void)
{
	return CRenderer::GetInstance();
}

// LightMgr
// Get
const D3DLIGHT9*		Get_Light(const _uint& iIndex)
{
	return CLightMgr::GetInstance()->Get_Light(iIndex);
}
// Set
// General
HRESULT		Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex)
{
	return CLightMgr::GetInstance()->Ready_Light(pGraphicDev, pLightInfo, iIndex);
}

// PrototypeMgr
// Get
// Set
// General
HRESULT		Ready_Prototype(const _tchar* pProtoTag, CComponent* pInstance)
{
	return CPrototypeMgr::GetInstance()->Ready_Prototype(pProtoTag, pInstance);
}
CComponent*	Clone_Prototype(const _tchar* pProtoTag)
{
	return CPrototypeMgr::GetInstance()->Clone_Prototype(pProtoTag);
}

void PlaySoundW(TCHAR* pSoundKey, CHANNELID eID) {
	CSoundMgr::GetInstance()->PlaySoundW(pSoundKey, eID);
}
void PlaySoundVolume(TCHAR* pSoundKey, CHANNELID eID, float vol) {
	CSoundMgr::GetInstance()->PlaySoundVolume(pSoundKey, eID, vol);
}
void PlayBGM(TCHAR* pSoundKey) {
	CSoundMgr::GetInstance()->PlayBGM(pSoundKey);
}
void StopSound(CHANNELID eID) {
	CSoundMgr::GetInstance()->StopSound(eID);
}
void StopAll() {
	CSoundMgr::GetInstance()->StopAll();
}
void SetVolume(CHANNELID eID, float vol) {
	CSoundMgr::GetInstance()->SetVolume(eID, vol);
}
void SoundUpdate() {
	CSoundMgr::GetInstance()->Update();
}


// Release Utility
void		Release_Utility(void)
{
	CPrototypeMgr::GetInstance()->DestroyInstance();
	CLightMgr::GetInstance()->DestroyInstance();
	CRenderer::GetInstance()->DestroyInstance();
	CManagement::GetInstance()->DestroyInstance();
}