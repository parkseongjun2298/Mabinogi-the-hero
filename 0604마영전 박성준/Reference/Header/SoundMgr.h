#pragma once
#ifndef SoundMgr_h__
#define SoundMgr_h__

#include "Engine_Defines.h"
#include "Base.h"
#include "fmod.h"

BEGIN(Engine)

class CGameObject;

class ENGINE_DLL CSoundMgr
{
	DECLARE_SINGLETON(CSoundMgr)
private:
	CSoundMgr();
	~CSoundMgr();

public:
	void Initialize(); 

	void Release(); 
public:
	void PlaySoundW(TCHAR* pSoundKey, CHANNELID eID);
	void PlaySoundVolume(TCHAR* pSoundKey, CHANNELID eID, float vol);
	void PlayBGM(TCHAR* pSoundKey);
	void StopSound(CHANNELID eID);
	void StopAll();
	void SetVolume(CHANNELID eID, float vol);
	void Update();
	

private:
	void LoadSoundFile(); 

private:
	// 사운드 리소스 정보를 갖는 객체 
	map<TCHAR*, FMOD_SOUND*> m_mapSound; 
	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL* m_pChannelArr[Engine::CH_MAXCHANNEL];
	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM* m_pSystem; 

};

END

#endif