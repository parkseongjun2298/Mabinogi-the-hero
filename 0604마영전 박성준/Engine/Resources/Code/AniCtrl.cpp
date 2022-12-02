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

	// �ִϸ��̼� ������ �����Ǿ �ȵǱ� ������ ���� �Լ��� ����
	// 1���� : ���� �� ���� ��ü�ּ� �����Ǵ� �ִϸ��̼� ����
	// 2���� : ���� ������ �ִϸ��̼��� �ִ� ����(�밳 1������ ���� ��ġ)
	// 3���� : �ִϸ��̼��� �����ϱ� ���� Ʈ���� �ִ� ����
	// 4���� : ���� �޽��� ����Ǵ� ��Ư�� �̺�Ʈ ȿ���� �ִ� ����(�츮�� ������� ����)

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

	// �ε��� ���� �ش��ϴ� �ִϸ��̼� �� ������ �������� �Լ�
	m_pAniCtrl->GetAnimationSet(iIndex, &pAS);

	m_dPeriod = pAS->GetPeriod(); // ���� �ִϸ��̼� ���� ���� ��� �ð��� ��ȯ�ϴ� �Լ�(�� �ִϸ��̼��� �� ����)

	
	// �ִϸ��̼� ���� ������ ������ Ʈ���� �����ϴ� �Լ�
	m_pAniCtrl->SetTrackAnimationSet(m_iNewTrack, pAS);

	// Ʈ�� ������ �������� �̺�Ʈ���� ������� �ʰڴ�(������ �ϴ� ��쿡 �̺�Ʈ ������ �ȵǴ� ��찡 ��Ȥ �߻�)
	m_pAniCtrl->UnkeyAllTrackEvents(m_iCurrentTrack);
	m_pAniCtrl->UnkeyAllTrackEvents(m_iNewTrack);

	// 2���� : ���� Ʈ���� ����� �� ���� �� �����ϴ� ���ڰ�, 3���� : ���� �����ų ���ΰ�(�ִϸ��̼� ��� �� ���Ǵ� �Ǽ����� ��� double���̾�� ��)
	m_pAniCtrl->KeyTrackEnable(m_iCurrentTrack, FALSE, m_fAccTime + 0.25);

	m_pAniCtrl->KeyTrackSpeed(m_iCurrentTrack, m_fAniSpeed, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);

	// ����Ǵ� Ʈ���� ����ġ�� �����ϴ� �Լ�
	m_pAniCtrl->KeyTrackWeight(m_iCurrentTrack, 0.1f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);

	// Ʈ���� Ȱ��ȭ�ϴ� �Լ�
	m_pAniCtrl->SetTrackEnable(m_iNewTrack, TRUE);
	m_pAniCtrl->KeyTrackSpeed(m_iNewTrack, m_fAniSpeed, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_iNewTrack, 0.9f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);

	m_pAniCtrl->ResetTime();	// advanced�Լ� ȣ�� �� �����ϴ� �ð� ���� �缳���ϴ� �Լ�
	m_fAccTime = 0.f;

	// ������ ��� ���̴� Ʈ������ ���ο� Ʈ������ ��ȯ�� �� ����ϴ� �������� �����ϴ� �Լ�
	m_pAniCtrl->SetTrackPosition(m_iNewTrack, 0.0);
	m_iOldAniIndex = iIndex;

	m_iCurrentTrack = m_iNewTrack;
}

void Engine::CAniCtrl::Play_Animation(const _float& fTimeDelta)
{
	m_pAniCtrl->AdvanceTime(fTimeDelta, nullptr);   // ���� �ִϸ��̼��� ����ϴ� �Լ�(2���� : �ִϸ��̼� ���ۿ� ���� ���峪 ����Ʈ ó���� ����ϴ� ��ü �ּҰ� ��, ����� �� ���� ���� ����Ѵ� �ϴ��� �ڵ��� ���߸� Ŀ��)	
													// AdvanceTime�� ȣ�� �Ǵ� ������ ����������	 �����ϴ� �ð� ���� �ִ�.
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


