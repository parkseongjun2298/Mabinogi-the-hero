#include "stdafx.h"
#include "LordOfManaHandEffect2.h"

#include "Export_Function.h"

CLordOfManaHandEffect2::CLordOfManaHandEffect2(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev) , m_iFlag(0),
	m_fDeadTime(0.f), m_pParentBoneMatrix(nullptr)
	, m_pParentWorldMatrix(nullptr)
{

}

CLordOfManaHandEffect2::~CLordOfManaHandEffect2(void)
{

}

HRESULT CLordOfManaHandEffect2::Ready_GameObject(const _uint& iFlag)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->m_vScale = { 15.f,15.f ,15.f };

	m_iFlag = iFlag;


	return S_OK;
}

_int CLordOfManaHandEffect2::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return DEAD_OBJ;

	if (nullptr == m_pParentBoneMatrix)
	{
		Engine::CDynamicMesh*	pPlayerMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(L"GameLogic", L"Player", L"Com_Mesh", Engine::ID_STATIC));
		NULL_CHECK_RETURN(pPlayerMeshCom, 0);

		const Engine::D3DXFRAME_DERIVED*	pFrame = pPlayerMeshCom->Get_FrameByName("ValveBiped_Bip01_L_Hand"); //ValveBiped_Anim_Attachment_RH

		m_pParentBoneMatrix = &pFrame->CombinedTransformMatrix;

		Engine::CTransform*	pPlayerTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"GameLogic", L"Player", L"Com_Transform", Engine::ID_DYNAMIC));
		NULL_CHECK_RETURN(pPlayerTransform, 0);

		m_pParentWorldMatrix = pPlayerTransform->Get_WorldMatrixPointer();
	}
	Engine::CGameObject::Update_GameObject(fTimeDelta);
	m_pTransformCom->Set_ParentMatrix(&(*m_pParentBoneMatrix * *m_pParentWorldMatrix));
	m_pRendererCom->Add_Render_Group(Engine::RENDER_ALPHA, this);

	

	m_fDeadTime += 1 * fTimeDelta;

	if (m_fDeadTime >= 3.f)
		Set_bIsDead();

	m_fFrame += 1.f * fTimeDelta;   //이숫자가 텍스쳐 몇개를 돌지 하는 숫자임 ;

	if (1.f < m_fFrame)
		m_fFrame = 0.f;


	
	
	_matrix			matWorld, matView, matBill;
	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	

	m_pTransformCom->m_matWorld =  matWorld;
	
	_vec3 vTemp;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vTemp);
	Engine::CGameObject::Compute_ViewZ(&vTemp);

	


	m_pTransformCom->Set_Rotate(Engine::ROT_Y, 1.57129);
	_vec3 Temp = { m_fDeadTime ,m_fDeadTime ,m_fDeadTime };
	m_pTransformCom->m_vScale + Temp;
	
	
	return 0;
}

void CLordOfManaHandEffect2::Render_GameObject(void)
{
	
	
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);

	_uint	iPassMax = 0;

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	pEffect->Begin(&iPassMax, 0);

	pEffect->BeginPass(0);
	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();

	pEffect->End();

}

void CLordOfManaHandEffect2::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CLordOfManaHandEffect2::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	// buffer
	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	// texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_LordOfManaBot"));
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

	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Ui"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);
	return S_OK;
}

HRESULT CLordOfManaHandEffect2::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
{
	_matrix		matWorld, matView, matProj;
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture");

	return S_OK;
}

CLordOfManaHandEffect2* CLordOfManaHandEffect2::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iFlag)
{
	CLordOfManaHandEffect2*	pInstance = new	CLordOfManaHandEffect2(pGraphicDev);
	
	if (FAILED(pInstance->Ready_GameObject(iFlag)))
		Engine::Safe_Release(pInstance);
	

	return pInstance;
}

