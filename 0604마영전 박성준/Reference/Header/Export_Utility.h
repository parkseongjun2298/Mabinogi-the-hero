#ifndef Export_Utility_h__
#define Export_Utility_h__

#include "Engine_Defines.h"
#include "Management.h"
#include "Transform.h"
#include "Renderer.h"
#include "Camera.h"
#include "Calculator.h"
#include "LightMgr.h"
#include "Collider.h"
#include "PrototypeMgr.h"
#include "Shader.h"
#include "SoundMgr.h"
BEGIN(Engine)

// Management
// Get
inline 	CComponent*		Get_Component(const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag, COMPONENTID eID);
inline	HRESULT Add_GameObject(const _tchar * pLayerTag, const _tchar * pObjTag, CGameObject * pGameObject);
// Set
inline HRESULT		SetUp_Scene(CScene* pScene);
// General
inline HRESULT	Create_Management(LPDIRECT3DDEVICE9 pGraphicDev, CManagement** ppManagement);

// Renderer
// Get
// Set
// General
inline CRenderer*		Get_Renderer(void);

// LightMgr
// Get
inline const D3DLIGHT9*		Get_Light(const _uint& iIndex);
// Set
// General
inline HRESULT		Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLightInfo, const _uint& iIndex);

// PrototypeMgr
// Get
// Set
// General
inline HRESULT		Ready_Prototype(const _tchar* pProtoTag, CComponent* pInstance);
inline CComponent*	Clone_Prototype(const _tchar* pProtoTag);

// Release Utility
inline void		Release_Utility(void);

// SoundMgr
// Get
// Set
// General
inline void PlaySoundW(TCHAR* pSoundKey, CHANNELID eID);
inline void PlaySoundVolume(TCHAR* pSoundKey, CHANNELID eID, float vol);
inline void PlayBGM(TCHAR* pSoundKey);
inline void StopSound(CHANNELID eID);
inline void StopAll();
inline void SetVolume(CHANNELID eID, float vol);
inline void SoundUpdate();

#include "Export_Utility.inl"

END
#endif // Export_Utility_h__
