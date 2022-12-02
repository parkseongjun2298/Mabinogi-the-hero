#include "AniCtrl.h"

USING(Engine)

Engine::CAniCtrl::CAniCtrl(LPD3DXANIMATIONCONTROLLER pAniCtrl)
	: m_pAniCtrl(pAniCtrl)
	, m_fAccTime(0.f)
	, m_iCurrentTrack(0)
	, m_iNewTrack(1)
	, m_iOldAniIndex(999)
	, m_dPeriod(0.0)
	, m_fAniSpeed(2.f)
{
	m_pAniCtrl->AddRef();
}

Engine::CAniCtrl::CAniCtrl(const CAniCtrl& rhs)
	: m_iCurrentTrack(rhs.m_iCurrentTrack)
	, m_iNewTrack(rhs.m_iNewTrack)
	, m_fAccTime(rhs.m_fAccTime)
	, m_iOldAniIndex(rhs.m_iOldAniIndex)
	, m_dPeriod(0.0)
	, m_fAniSpeed(2.f)
{
	rhs.m_pAniCtrl->CloneAnimationController(rhs.m_pAniCtrl->GetMaxNumAnimationOutputs(),
											rhs.m_pAniCtrl->GetMaxNumAnimationSets(), 
											rhs.m_pAniCtrl->GetMaxNumTracks(),
											rhs.m_pAniCtrl->GetMaxNumEvents(), 
											&m_pAniCtrl);

	// 애니메이션 정보는 공유되어선 안되기 때문에 복제 함수가 제공
	// 1인자 : 복제 시 원본 객체애서 제공되는 애니메이션 개수
	// 2인자 : 구동 가능한 애니메이션의 최대 개수(대개 1인자의 값과 일치)
	// 3인자 : 애니메이션을 구동하기 위한 트랙의 최대 개수
	// 4인자 : 현재 메쉬에 적용되는 독특한 이벡트 효과의 최대 개수(우리는 사용하지 못함)

}

Engine::CAniCtrl::~CAniCtrl(void)
{

}

HRESULT Engine::CAniCtrl::Ready_AnimationCtrl(void)
{
	


	return S_OK;
}

void Engine::CAniCtrl::Set_Animation(const _uint& iIndex)
{
	if (m_iOldAniIndex == iIndex)
		return;

	m_iNewTrack = (m_iCurrentTrack == 0 ? 1 : 0);

	LPD3DXANIMATIONSET		pAS = NULL;

	// 인덱스 값에 해당하는 애니메이션 셋 정보를 가져오는 함수
	m_pAniCtrl->GetAnimationSet(iIndex, &pAS);

	m_dPeriod = pAS->GetPeriod(); // 현재 애니메이션 셋이 가진 재생 시간을 반환하는 함수(한 애니메이션의 총 길이)

	
	// 애니메이션 셋을 현재의 설정한 트랙에 세팅하는 함수
	m_pAniCtrl->SetTrackAnimationSet(m_iNewTrack, pAS);

	// 트랙 내에서 벌어지는 이벤트들을 사용하지 않겠다(보간을 하는 경우에 이벤트 때문에 안되는 경우가 간혹 발생)
	m_pAniCtrl->UnkeyAllTrackEvents(m_iCurrentTrack);
	m_pAniCtrl->UnkeyAllTrackEvents(m_iNewTrack);

	// 2인자 : 현재 트랙을 재생할 지 멈출 지 결정하는 인자값, 3인자 : 언제 종료시킬 것인가(애니메이션 재생 시 사용되는 실수값은 모두 double형이어야 함)
	m_pAniCtrl->KeyTrackEnable(m_iCurrentTrack, FALSE, m_fAccTime + 0.25);

	m_pAniCtrl->KeyTrackSpeed(m_iCurrentTrack, m_fAniSpeed, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);

	// 재생되는 트랙의 가중치를 설정하는 함수
	m_pAniCtrl->KeyTrackWeight(m_iCurrentTrack, 0.1f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);

	// 트랙을 활성화하는 함수
	m_pAniCtrl->SetTrackEnable(m_iNewTrack, TRUE);
	m_pAniCtrl->KeyTrackSpeed(m_iNewTrack, m_fAniSpeed, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_iNewTrack, 0.9f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);

	m_pAniCtrl->ResetTime();	// advanced함수 호출 시 증가하던 시간 값을 재설정하는 함수
	m_fAccTime = 0.f;

	// 기존의 재생 중이던 트랙에서 새로운 트랙으로 변환할 때 재생하는 시작점을 지시하는 함수
	m_pAniCtrl->SetTrackPosition(m_iNewTrack, 0.0);
	m_iOldAniIndex = iIndex;

	m_iCurrentTrack = m_iNewTrack;
}

void Engine::CAniCtrl::Play_Animation(const _float& fTimeDelta)
{
	m_pAniCtrl->AdvanceTime(fTimeDelta, nullptr);   // 실제 애니메이션을 재생하는 함수(2인자 : 애니메이션 동작에 따라 사운드나 이펙트 처리를 담당하는 객체 주소가 들어감, 사용할 수 없고 만약 사용한다 하더라도 코드의 가중만 커짐)	
													// AdvanceTime이 호출 되는 시점에 내부적으로	 증가하는 시간 값이 있다.
	m_fAccTime += fTimeDelta;
}

CAniCtrl* Engine::CAniCtrl::Create(LPD3DXANIMATIONCONTROLLER pAniCtrl)
{
	CAniCtrl*	pInstance = new CAniCtrl(pAniCtrl);

	if (FAILED(pInstance->Ready_AnimationCtrl()))
		Safe_Release(pInstance);

	return pInstance;
}

CAniCtrl* Engine::CAniCtrl::Create(const CAniCtrl& rhs)
{
	CAniCtrl*		pInstance = new CAniCtrl(rhs);

	if (FAILED(pInstance->Ready_AnimationCtrl()))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CAniCtrl::Free(void)
{
	Safe_Release(m_pAniCtrl);
}

_bool Engine::CAniCtrl::Is_AnimationEnd(void)
{
	D3DXTRACK_DESC		tTrackInfo;
	ZeroMemory(&tTrackInfo, sizeof(D3DXTRACK_DESC));

	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &tTrackInfo);

	
	if (tTrackInfo.Position >= m_dPeriod - 0.1f)
		return true;



	return false;
}


