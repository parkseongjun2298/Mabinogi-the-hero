#include "Timer.h"

USING(Engine)

Engine::CTimer::CTimer(void)
{
	ZeroMemory(&m_FrameTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_LastTime, sizeof(LARGE_INTEGER));

	ZeroMemory(&m_FixTime, sizeof(LARGE_INTEGER));
	ZeroMemory(&m_CpuTick, sizeof(LARGE_INTEGER));
}

Engine::CTimer::~CTimer(void)
{

}

HRESULT Engine::CTimer::Ready_Timer(void)
{
	QueryPerformanceCounter(&m_FrameTime);			//	1020
	QueryPerformanceCounter(&m_LastTime);			//	1048
	QueryPerformanceCounter(&m_FixTime);			//	1100

	QueryPerformanceFrequency(&m_CpuTick);	

	return S_OK;
}

void Engine::CTimer::Set_TimeDelta(void)
{
	QueryPerformanceCounter(&m_FrameTime);	// 1400 - 1048

	if (m_FrameTime.QuadPart - m_FixTime.QuadPart >= m_CpuTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_CpuTick);
		m_FixTime = m_FrameTime;
	}
	
	m_fTimeDelta = ((_float)m_FrameTime.QuadPart - (_float)m_LastTime.QuadPart) / (_float)m_CpuTick.QuadPart;

	m_LastTime = m_FrameTime;
}


CTimer* Engine::CTimer::Create(void)
{
	CTimer*	pInstance = new CTimer;

	if (FAILED(pInstance->Ready_Timer()))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CTimer::Free(void)
{

}



