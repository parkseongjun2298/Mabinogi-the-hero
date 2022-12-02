#include "stdafx.h"
#include "TextUI2.h"

#include "Export_Function.h"
#include"UI.h"
CTextUI2::CTextUI2(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
{

}

CTextUI2::~CTextUI2(void)
{

}

HRESULT CTextUI2::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fX = WINCX / 2;
	m_fY = 800;

	m_fSizeX = 1600;
	m_fSizeY = 250;

	return S_OK;
}

_int CTextUI2::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return DEAD_OBJ;
		

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	m_pRendererCom->Add_Render_Group(Engine::RENDER_UI, this);

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	Key_Input(fTimeDelta);
	

	return 0;
}

void CTextUI2::Render_GameObject(void)
{
	// m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_matWorld);

	_matrix		matWorld, matView, matOldView, matOldProj;

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matOldProj);

	matWorld._11 = m_fSizeX;
	matWorld._22 = m_fSizeY;
	matWorld._33 = 1.f;
	matWorld._41 = m_fX - WINCX * 0.5f;
	matWorld._42 = -m_fY + WINCY * 0.5f;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);

	if (!m_bScene1)
	{
		wsprintf(m_szText, L"진짜 쎄구나. 반대편에 문지기 한명있는데 걔도 잡아줘. ");
		Engine::Render_Font(L"Font_Default", m_szText, &_vec2(WINCX / 4, 775), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		if (Engine::Get_DIKeyState(DIK_RETURN) & 0x80)
		{
			m_bScene1 = true;

		}
	}
	if (m_bScene1)
	{
		wsprintf(m_szText, L"문지기 뒤의 문밖에서 보자.");
		Engine::Render_Font(L"Font_Default", m_szText, &_vec2(WINCX / 4, 775), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
		
	}
			
	
	

}

void CTextUI2::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CTextUI2::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	// buffer
	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	// texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_TextUI"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	// transform
	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// renderer
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);


	return S_OK;
}

void CTextUI2::Key_Input(const _float & fTimeDelta)
{
	if (m_bScene1 && (Engine::Get_DIKeyState(DIK_RSHIFT) & 0x80))
		Set_bIsDead();

	
}

CTextUI2* CTextUI2::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTextUI2*	pInstance = new	CTextUI2(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

