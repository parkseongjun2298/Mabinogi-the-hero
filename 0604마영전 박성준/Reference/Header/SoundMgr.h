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
	// ���� ���ҽ� ������ ���� ��ü 
	map<TCHAR*, FMOD_SOUND*> m_mapSound; 
	// FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD_CHANNEL* m_pChannelArr[Engine::CH_MAXCHANNEL];
	// ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
	FMOD_SYSTEM* m_pSystem; 

};

END

#endif