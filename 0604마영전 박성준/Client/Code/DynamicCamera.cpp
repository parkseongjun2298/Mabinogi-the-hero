#include "stdafx.h"
#include "DynamicCamera.h"

#include "Export_Function.h"
#include "Transform.h"
#include"KeyMgr.h"
CDynamicCamera::CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CCamera(pGraphicDev),
	m_pKeyMgr(Engine::CKeyMgr::GetInstance()), m_fBossDieTime(0.f)
{

}

CDynamicCamera::~CDynamicCamera(void)
{

}

HRESULT CDynamicCamera::Ready_GameObject(const _vec3* pEye,
	const _vec3* pAt,
	const _vec3* pUp,
	const _float& fFovY,
	const _float& fAspect,
	const _float& fNear,
	const _float& fFar)
{
	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;

	m_fFovY = fFovY;
	m_fAspect = fAspect;
	m_fNear = fNear;
	m_fFar = fFar;

	FAILED_CHECK_RETURN(Engine::CCamera::Ready_GameObject(), E_FAIL);

	return S_OK;
}

_int CDynamicCamera::Update_GameObject(const _float& fTimeDelta)
{
	Key_Input(fTimeDelta);
	CameraMode(fTimeDelta);
	



	if (true == m_bMouseFix)
	{
		Fix_Mouse();
		Mouse_Move(fTimeDelta);
	}



	_int iExit = Engine::CCamera::Update_GameObject(fTimeDelta);

	return iExit;
}

void CDynamicCamera::Key_Input(const _float& fTimeDelta)
{

	//cout << "카메라상태:" << m_CameraMode << endl;

	for (auto& Player : Engine::CManagement::GetInstance()->Get_Obj(L"Player"))
	{
		m_pPlayer = dynamic_cast<CPlayer*>(Player);
	};

	for (auto& Boss : Engine::CManagement::GetInstance()->Get_Obj(L"Boss"))
	{
		m_pBoss = dynamic_cast<CBoss*>(Boss);
	};


	_matrix      matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);


	if (Engine::Get_DIKeyState(DIK_F1) & 0x80)
	{
		m_CameraMode = PLAYER;
	}
	if (Engine::Get_DIKeyState(DIK_F2) & 0x80)
	{
		m_CameraMode = VIEW;
	}
	if (Engine::Get_DIKeyState(DIK_F3) & 0x80)
	{
		m_CameraMode = BOSS2;
	}
	if (Engine::Get_DIKeyState(DIK_F4) & 0x80)
	{
		m_CameraMode = FREE;
	}

	/*if ((Engine::Get_DIKeyState(DIK_F5) & 0x80))
	{
		m_CameraMode = BOSS_DIE;
	}*/
	if (m_pBoss != nullptr && m_pBoss->m_bCheckDead)//조건문 재댈하기
	{
	
			m_CameraMode = BOSS_DIE;
	}
	


	if (Engine::Get_DIKeyState(DIK_3) & 0x80)
	{
		m_CameraMode = LORDOFMANA;
	}





	if (Engine::Get_DIKeyState(DIK_Q) & 0x80)
	{
		if (true == m_bClick)
			return;

		m_bClick = true;

		/*if (true == m_bMouseFix)
		m_bMouseFix = false;
		else
		m_bMouseFix = true;*/
	}
	else
		m_bClick = false;

	if (false == m_bMouseFix)
		return;

}

CDynamicCamera* CDynamicCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pEye, const _vec3* pAt, const _vec3* pUp, const _float& fFovY, const _float& fAspect, const _float& fNear, const _float& fFar)
{
	CDynamicCamera*   pInstance = new CDynamicCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(pEye, pAt, pUp, fFovY, fAspect, fNear, fFar)))

		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CDynamicCamera::Free(void)
{
	Engine::CCamera::Free();
}

void CDynamicCamera::Mouse_Move(const _float& fTimeDelta)
{
	_long   dwMouseMove = 0;

	_matrix      matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);

	if (dwMouseMove = Engine::Get_DIMouseMove(Engine::DIMS_Y))
	{
		_vec3   vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3   vLook = m_vAt - m_vEye;

		_matrix   matRotAxis;

		D3DXMatrixRotationAxis(&matRotAxis, &vRight, D3DXToRadian(dwMouseMove / 10.f));

		D3DXVec3TransformNormal(&vLook, &vLook, &matRotAxis);

		m_vAt = m_vEye + vLook;
	}

	if (dwMouseMove = Engine::Get_DIMouseMove(Engine::DIMS_X))
	{
		_vec3   vUp = _vec3(0.f, 1.f, 0.f);

		_vec3   vLook = m_vAt - m_vEye;

		_matrix   matRotAxis;

		D3DXMatrixRotationAxis(&matRotAxis, &vUp, D3DXToRadian(dwMouseMove / 10.f));

		D3DXVec3TransformNormal(&vLook, &vLook, &matRotAxis);

		m_vAt = m_vEye + vLook;
	}

}

void CDynamicCamera::Fix_Mouse(void)
{
	POINT   ptMouse{ WINCX >> 1, WINCY >> 1 };

	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);

}

void CDynamicCamera::CameraMode(const _float& fTimeDelta)
{
	switch (m_CameraMode)
	{
	case PLAYER_IDLE:
		Idle_Mode();
		break;
	case PLAYER:
		Player_Mode(fTimeDelta);
		Fix_Mouse();
		break;
	case BOSS:
		Boss_Mode(fTimeDelta);
		Fix_Mouse();
		break;
	case BOSS2:
		Boss_Mode2(fTimeDelta);
		Fix_Mouse();
		break;
	case VIEW:
		View_Mode(fTimeDelta);
		Fix_Mouse();
		break;
	case FREE:
		Free_Mode(fTimeDelta);
		Fix_Mouse();
		break;
	case LORDOFMANA:
		LordOfMana_Mode(fTimeDelta);
		break;
	case BOSS_DIE:
		BossDie_Mode(fTimeDelta);
		break;
	default:
		break;
	}
}

void CDynamicCamera::Idle_Mode()
{
	if (nullptr == m_pPlayer)
		return;

	_long   dwMouseMove = 0;

	_matrix      matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);

	m_vEye = { 0.f, 0.f, -4.f };
	m_vEye *= m_fDist;

	_matrix matPlayer = m_pPlayer->Get_Transform()->m_matWorld;
	matPlayer.m[3][1] += m_pPlayer->m_fRadius;
	_matrix matWorld, matRotX, matRotY, matTrans;

	if (dwMouseMove = Engine::Get_DIMouseMove(Engine::DIMS_Y)) // 마우스 상하 움직임
	{
		m_MouseAngle += dwMouseMove / 20.f;

		if (m_MouseAngle > 60) // 카메라가 뒤집히자않게 락걸기.
			m_MouseAngle = 60.f;
		if (m_MouseAngle <5.f)
			m_MouseAngle = 5.f;
	}
	if (dwMouseMove = Engine::Get_DIMouseMove(Engine::DIMS_X)) // 마우스 좌우 움직임
		m_pPlayer->Get_Transform()->Rotation(Engine::ROT_Y, D3DXToRadian(dwMouseMove / 20.f)); // 조종플레이어 Y축(좌우)회전




	D3DXMatrixRotationX(&matRotX, D3DXToRadian(m_MouseAngle));
	D3DXMatrixRotationY(&matRotY, D3DXToRadian(m_MouseAngleX));
	D3DXMatrixTranslation(&matTrans, m_vEye.x, m_vEye.y, m_vEye.z);

	matWorld = matRotX * matTrans * matRotY  * matPlayer;

	D3DXVec3TransformCoord(&m_vEye, &m_vEye, &matWorld);

	_vec3 vPos = matPlayer.m[3];
	_vec3 vRight = m_pPlayer->Get_Transform()->m_vInfo[Engine::INFO_RIGHT];

	m_vEye += vRight * m_pPlayer->m_fRight;
	m_vAt = vPos + vRight * m_pPlayer->m_fRight;

	//eye 카메라 위치 at 초점 
}

void CDynamicCamera::Player_Mode(const _float& fTimeDelta)
{

	if (nullptr == m_pPlayer)
		return;


	_long   dwMouseMove = 0;

	_matrix      matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);
	
	if (dwMouseMove = Engine::Get_DIMouseMove(Engine::DIMS_Y)) // 마우스 상하 움직임
	{
		m_MouseAngle += dwMouseMove / 20.f;

		if (m_MouseAngle > 70) // 카메라가 뒤집히자않게 락걸기.
			m_MouseAngle = 70.f;
		if (m_MouseAngle <-20.f)
			m_MouseAngle = -20.f;
	}
	if (dwMouseMove = Engine::Get_DIMouseMove(Engine::DIMS_X)) // 마우스 좌우 움직임
		m_pPlayer->Get_Transform()->Rotation(Engine::ROT_Y, D3DXToRadian(dwMouseMove / 20.f)); // 조종플레이어 Y축(좌우)회전

	_vec3 playerLook, playerPos;
	memcpy(&playerLook, &m_pPlayer->Get_Transform()->m_matWorld.m[2][0], sizeof(_vec3));
	memcpy(&playerPos, &m_pPlayer->Get_Transform()->m_matWorld.m[3][0], sizeof(_vec3));

	_vec3 vCamPos;
	_vec3 vDir = { 0.f, 1.f, -4.f };


	_matrix matRotY, matRotX, matSum;
	D3DXMatrixRotationX(&matRotX, D3DXToRadian(m_MouseAngle));
	D3DXMatrixRotationY(&matRotY, m_fRotY);
	matSum = matRotX * matRotY;
	D3DXVec3TransformNormal(&vCamPos, &vDir, &matSum);

	playerPos.y += 1.f;
	m_vEye = vCamPos + playerPos;
	m_vAt = playerPos;








	if (m_pPlayer->m_PlayerState == PlayerState::ATT_5/*Engine::Get_DIMouseState(Engine::DIM_LB) & 0x80*/)
	{
		_vec3	vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));
		float Temp = 50.f * fTimeDelta;
		_vec3	vLength = *D3DXVec3Normalize(&vLook, &vLook) *Temp;
		if (!m_bAttEff)
		{
			m_vEye += vLength;
			m_vAt += vLength;
			m_bAttEff = true;
		}
	}

	if (m_pPlayer->m_PlayerState == PlayerState::IDLE)
	{
		m_bAttEff = false;
	}


}

void CDynamicCamera::Boss_Mode(const _float& fTimeDelta)
{




	if (!m_bBossCam)
	{


		_matrix      matCamWorld;
		D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);

		m_vEye = D3DXVECTOR3{ 0.f,6.f,30.f };
		m_vAt = D3DXVECTOR3{ 0.f,0.f,0.f }; //보스를 억지로 돌려서 눈은 보고있는곳이랑 다른듯?


		_matrix matWorld;
		if (m_fBossRotX < 330)
			m_fBossRotX += 10 * fTimeDelta;
		if (m_fBossRotX >= 330)
		{
			m_fBossRotX = 330;

			m_bBossCam = true;

		}

		D3DXMatrixRotationY(&matWorld, D3DXToRadian(m_fBossRotX));
		_vec3 Temp;
		D3DXVec3TransformCoord(&Temp, &Temp, &matWorld);
		m_vAt += m_vEye + Temp;
	}
	if (m_bBossCam)
	{
		m_vEye = D3DXVECTOR3{ 0.f,6.f,30.f };
		m_vAt = D3DXVECTOR3{ -50.f,0.f,10.f };


		_matrix matWorld;
		if (m_fBossRotX > 310)
			m_fBossRotX -= 10 * fTimeDelta;
		if (m_fBossRotX <= 310)
		{
			m_fBossRotX = 310;
			m_bBossFin = true;

		}
		D3DXMatrixRotationY(&matWorld, D3DXToRadian(m_fBossRotX));
		_vec3 Temp;
		D3DXVec3TransformCoord(&Temp, &Temp, &matWorld);
		m_vAt += m_vEye + Temp;
	}

	if (m_bBossFin)
		m_CameraMode = BOSS2;
}

void CDynamicCamera::Boss_Mode2(const _float & fTimeDelta)
{
	if (!m_bBossCam2)
	{


		_matrix      matCamWorld;
		D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);

		m_vEye = D3DXVECTOR3{ 0.f,6.f,10.f };
		m_vAt = D3DXVECTOR3{ 0.f,0.f,0.f }; //보스를 억지로 돌려서 눈은 보고있는곳이랑 다른듯?


		_matrix matWorld;
		if (m_fBossRotX2 < 150)
			m_fBossRotX2 += 10 * fTimeDelta;
		if (m_fBossRotX2 >= 150)
		{
			m_fBossRotX2 = 150;

			m_bBossCam2 = true;

		}

		D3DXMatrixRotationY(&matWorld, D3DXToRadian(m_fBossRotX2));
		_vec3 Temp;
		D3DXVec3TransformCoord(&Temp, &Temp, &matWorld);
		m_vAt += m_vEye + Temp;
	}
	if (m_bBossCam2)
	{
		m_vEye = D3DXVECTOR3{ 0.f,6.f,10.f };
		m_vAt = D3DXVECTOR3{ -50.f,0.f,10.f };


		_matrix matWorld;
		if (m_fBossRotX2 > 130)
			m_fBossRotX2 -= 10 * fTimeDelta;
		if (m_fBossRotX2 <= 130)
		{
			m_fBossRotX2 = 130;
			m_bBossFin2 = true;
			m_bBossCtl = true;
		}
		D3DXMatrixRotationY(&matWorld, D3DXToRadian(m_fBossRotX2));
		_vec3 Temp;
		D3DXVec3TransformCoord(&Temp, &Temp, &matWorld);
		m_vAt += m_vEye + Temp;
	}

	if (m_bBossFin2)
		m_CameraMode = PLAYER;
}

void CDynamicCamera::Free_Mode(const _float& fTimeDelta)
{

	_matrix      matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);

	_long   dwMouseMove = 0;


	if (dwMouseMove = Engine::Get_DIMouseMove(Engine::DIMS_Y))
	{
		_vec3   vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3   vLook = m_vAt - m_vEye;

		_matrix   matRotAxis;

		D3DXMatrixRotationAxis(&matRotAxis, &vRight, D3DXToRadian(dwMouseMove / 10.f));

		D3DXVec3TransformNormal(&vLook, &vLook, &matRotAxis);

		m_vAt = m_vEye + vLook;
	}

	if (dwMouseMove = Engine::Get_DIMouseMove(Engine::DIMS_X))
	{
		_vec3   vUp = _vec3(0.f, 1.f, 0.f);

		_vec3   vLook = m_vAt - m_vEye;

		_matrix   matRotAxis;

		D3DXMatrixRotationAxis(&matRotAxis, &vUp, D3DXToRadian(dwMouseMove / 10.f));

		D3DXVec3TransformNormal(&vLook, &vLook, &matRotAxis);

		m_vAt = m_vEye + vLook;
	}

	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		_vec3   vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3   vLength = *D3DXVec3Normalize(&vLook, &vLook) * 10.f * fTimeDelta;

		m_vEye += vLength;
		m_vAt += vLength;

	}

	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		_vec3   vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3   vLength = *D3DXVec3Normalize(&vLook, &vLook) * 10.f * fTimeDelta;

		m_vEye -= vLength;
		m_vAt -= vLength;
	}

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		_vec3   vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3   vLength = *D3DXVec3Normalize(&vRight, &vRight) * 10.f * fTimeDelta;

		m_vEye -= vLength;
		m_vAt -= vLength;
	}

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		_vec3   vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3   vLength = *D3DXVec3Normalize(&vRight, &vRight) * 10.f * fTimeDelta;

		m_vEye += vLength;
		m_vAt += vLength;
	}
	if (Engine::Get_DIKeyState(DIK_ADD) & 0x80)
	{
		_vec3	vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vLook, &vLook) * 10.f * fTimeDelta;

		m_vEye += vLength;
		m_vAt += vLength;
	}
	if (Engine::Get_DIKeyState(DIK_SUBTRACT) & 0x80)
	{
		_vec3	vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vLook, &vLook) * 10.f * fTimeDelta;

		m_vEye -= vLength;
		m_vAt -= vLength;
	}



}

void CDynamicCamera::View_Mode(const _float & fTimeDelta)
{
	
	
	if (!m_bViewCam2)
	{
		_matrix      matCamWorld;
		D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);

		m_vEye = D3DXVECTOR3{ 10.f,12.f,5.f };
		m_vAt = D3DXVECTOR3{ -30.f,0.f,-30.f }; //at을 회전시켜야함


		_matrix matWorld;
		if (m_fRotX < 100)
			m_fRotX += 20 * fTimeDelta;
		if (m_fRotX >= 100)
		{
			m_fRotX = 100;
			//m_fRotX = 0;
			m_bViewCam2 = true;

		}
		D3DXMatrixRotationY(&matWorld, D3DXToRadian(m_fRotX));
		_vec3 Temp;
		D3DXVec3TransformCoord(&Temp, &Temp, &matWorld);
		m_vAt += m_vEye + Temp;
	}
	if (m_bViewCam2)
	{
		m_vEye = D3DXVECTOR3{ 0.f,6.f,-15.f };
		m_vAt = D3DXVECTOR3{ 200.f,6.f,400.f };


		_matrix matWorld;
		if (m_fRotX2 > 100)
			m_fRotX2 -= 20 * fTimeDelta;
		if (m_fRotX2 <= 100)
		{
			m_fRotX2 = 100;
			m_bViewFin = true;

		}
		D3DXMatrixRotationY(&matWorld, D3DXToRadian(m_fRotX2));
		_vec3 Temp;
		D3DXVec3TransformCoord(&Temp, &Temp, &matWorld);
		m_vAt += m_vEye + Temp;
	}

	if (m_bViewFin)
		m_CameraMode = BOSS;

	/*_vec3   Up;
	memcpy(&Up, &matCamWorld.m[1][0], sizeof(_vec3));

	_vec3   vLook = m_vAt - m_vEye;

	_matrix   matRotAxis;

	if(m_fRotX <= 360)
	m_fRotX+=5;
	if (m_fRotX > 360)
	{
	m_fRotX = 360;

	}

	D3DXMatrixRotationAxis(&matRotAxis, &Up, D3DXToRadian(m_fRotX*fTimeDelta));

	D3DXVec3TransformNormal(&vLook, &vLook, &matRotAxis);

	m_vAt = m_vEye + vLook;
	*/

}



void CDynamicCamera::LordOfMana_Mode(const _float & fTimeDelta)
{

	if (nullptr == m_pPlayer)
		return;



	_matrix      matCamWorld;
	D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);

	_vec3 playerLook, playerPos;
	memcpy(&playerLook, &m_pPlayer->Get_Transform()->m_matWorld.m[2][0], sizeof(_vec3));
	memcpy(&playerPos, &m_pPlayer->Get_Transform()->m_matWorld.m[3][0], sizeof(_vec3));

	_vec3 vCamPos;
	_vec3 vDir = { 0.f, 1.f, -3.f };


	_matrix  matRotY;

	if (m_pPlayer->m_pMeshCom->m_pAniCtrl->m_fAccTime >= 2.7f && m_pPlayer->m_pMeshCom->m_pAniCtrl->m_fAccTime < 4.f)
	{
		m_fRotY = 3.f;
		//m_pTransformCom->Set_Rotate(Engine::ROT_Y, m_fRotY);

	}
	else
		m_fRotY = 0.f;


	//m_fRotY += 1 * fTimeDelta;
	D3DXMatrixRotationY(&matRotY, m_fRotY);

	D3DXVec3TransformNormal(&vCamPos, &vDir, &matRotY);


	m_vEye = vCamPos + playerPos;
	m_vAt = playerPos;


	//cout << m_fRotY << endl;

	if (m_pPlayer->m_pMeshCom->m_pAniCtrl->m_fAccTime >= 10.8f)
		m_CameraMode = PLAYER;



}

void CDynamicCamera::BossDie_Mode(const _float & fTimeDelta)
{
	

	if (nullptr == m_pBoss)
		return;


	

		m_fBossDieTime += 1 * fTimeDelta;
	
		_matrix      matCamWorld;
		D3DXMatrixInverse(&matCamWorld, NULL, &m_matView);

		_vec3 BossLook, BossPos;
		memcpy(&BossLook, &m_pBoss->Get_Transform()->m_matWorld.m[1][0], sizeof(_vec3));
		memcpy(&BossPos, &m_pBoss->Get_Transform()->m_matWorld.m[3][0], sizeof(_vec3));

		_vec3 vCamPos;
		_vec3 vDir = { 0.f, 3.5f, -6.f };


		_matrix  matRotY;

		m_fBossRotY += 1 * fTimeDelta;
		D3DXMatrixRotationY(&matRotY, m_fBossRotY);



		D3DXVec3TransformNormal(&vCamPos, &vDir, &matRotY);


		m_vEye = vCamPos + BossPos;
		m_vAt = BossPos;
	
		if (m_fBossDieTime >= 7.7f)
		{
			m_CameraMode = PLAYER;
			return;
		}

}
