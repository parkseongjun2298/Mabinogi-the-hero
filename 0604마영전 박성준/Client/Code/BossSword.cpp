#include "stdafx.h"
#include "BossSword.h"

#include "Export_Function.h"



#include"Collider2.h"
CBossSword::CBossSword(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev)
	, m_iFlag(0)
	, m_bColl(false)
	, m_pParentBoneMatrix(nullptr)
	, m_pParentWorldMatrix(nullptr)
	, m_iTrailIdxCount(30)
	, m_fAngleY(0.f)
{

}

CBossSword::~CBossSword(void)
{

}

HRESULT CBossSword::Ready_GameObject(const _uint& iFlag)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	
	

	m_fAngleY = 270;
	m_pTransformCom->Rotation(Engine::ROT_Y, D3DXToRadian(m_fAngleY));
	temp = { m_pTransformCom->Get_Info(Engine::INFO_POS)->x ,m_pTransformCom->Get_Info(Engine::INFO_POS)->y+1 ,m_pTransformCom->Get_Info(Engine::INFO_POS)->z };
	//z y x
	// X=y Y=z Z=x 임 이칼;
	m_pTransformCom->Set_Pos(&temp);
	m_iFlag = iFlag;

	_vec3 vMid = (*m_pColliderCom->Get_Max() + *m_pColliderCom->Get_Min()) / 2.f;
	m_vTopPos = vMid;
	m_vTopPos.z += (m_pColliderCom->Get_Max()->z + m_pColliderCom->Get_Min()->z) / 2.f - 1;

	m_vBottomPos = vMid;
	m_vBottomPos.z -= 15;
	return S_OK;
}

_int CBossSword::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return DEAD_OBJ;

	Engine::CGameObject::Update_GameObject(fTimeDelta);
	/*m_fAngleY+= 0.5f * fTimeDelta  ;
	cout << m_fAngleY << endl;
	m_pTransformCom->Rotation(Engine::ROT_Y, D3DXToRadian(m_fAngleY));*/
	if (nullptr == m_pParentBoneMatrix)
	{
		Engine::CDynamicMesh*	pPlayerMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(L"GameLogic", L"Boss", L"Com_Mesh", Engine::ID_STATIC));
		NULL_CHECK_RETURN(pPlayerMeshCom, 0);

		const Engine::D3DXFRAME_DERIVED*	pFrame = pPlayerMeshCom->Get_FrameByName("ValveBiped_Anim_Attachment_RH"); //ValveBiped_Anim_Attachment_GRAPPLING ValveBiped_Anim_Attachment_RH ValveBiped_Bip01_R_ForeTwist

		m_pParentBoneMatrix = &pFrame->CombinedTransformMatrix;

		Engine::CTransform*	pPlayerTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"GameLogic", L"Boss", L"Com_Transform", Engine::ID_DYNAMIC));
		NULL_CHECK_RETURN(pPlayerTransform, 0);

		m_pParentWorldMatrix = pPlayerTransform->Get_WorldMatrixPointer();
	}
		
	m_pRendererCom->Add_Render_Group(Engine::RENDER_NONALPHA, this);
	

		

	/*cout << "PosX:" << temp.x << endl;
	cout << "PosY:" << temp.y << endl;
	cout << "PosZ:" << temp.z << endl;*/
	

	return 0;
}

void CBossSword::Render_GameObject(void)
{
	m_pTransformCom->Set_ParentMatrix(&(*m_pParentBoneMatrix * *m_pParentWorldMatrix));

	Engine::TRIALVTX tTrailTop, tTrailBottom;
	tTrailTop.dwColor = D3DCOLOR_ARGB(255, 255, 0, 0);
	D3DXVec3TransformCoord(&tTrailTop.vPosition, &m_vTopPos, &m_pTransformCom->m_matWorld);
	tTrailBottom.dwColor = D3DCOLOR_ARGB(255, 255, 0, 0);
	D3DXVec3TransformCoord(&tTrailBottom.vPosition, &m_vBottomPos, &m_pTransformCom->m_matWorld);

	m_lstTrailVtx.push_back(tTrailTop);
	m_lstTrailVtx.push_back(tTrailBottom);

	while (true)
	{
		if (m_lstTrailVtx.size() <= 100)
			break;
		m_lstTrailVtx.pop_front();
	}

	/*LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);

	_uint	iPassMax = 0;

	FAILED_CHECK_RETURN(SetUp_ConstantTable(pEffect), );

	pEffect->Begin(&iPassMax, 0);

	pEffect->BeginPass(0);

	m_pMeshCom->Render_Meshes(pEffect);

	pEffect->EndPass();
	pEffect->End();*/


	Engine::TRIALVTX vertex[100];
	for (_uint i = 0; i < m_iTrailIdxCount; ++i)
		vertex[i].dwColor = D3DCOLOR_ARGB(255, 255, 0, 255);
	for (_uint i = 0; i < m_iTrailIdxCount / 2; ++i)
	{
		vertex[2 * i].vTexUV = { ((m_iTrailIdxCount / 2.f - i) / (m_iTrailIdxCount / 2 - 1.f)), 0.f };
		vertex[(2 * i) + 1].vTexUV = { ((m_iTrailIdxCount / 2.f - i) / (m_iTrailIdxCount / 2 - 1.f)), 1.f };
	}

	int iIdx = 0;
	for (auto& iter = m_lstTrailVtx.rbegin(); iter != m_lstTrailVtx.rend(); ++iter)
	{
		vertex[iIdx].vPosition = iter->vPosition;
		++iIdx;
		if (iIdx >= m_iTrailIdxCount)
			break;
	}

	_matrix matWorld;
	D3DXMatrixIdentity(&matWorld);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);


	
		LPD3DXEFFECT	pEffect = m_pBossSwordTrailShaderCom->Get_EffectHandle();
		NULL_CHECK(pEffect);
		_uint	iPassMax = 0;
		iPassMax = 0;
		FAILED_CHECK_RETURN(SetUp_BossSwordTrailConstantTable(pEffect), );

		pEffect->Begin(&iPassMax, 0);

		pEffect->BeginPass(2);

		m_pGraphicDev->SetFVF(Engine::FVF_TRAIL);
		m_pGraphicDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, m_iTrailIdxCount - 2, vertex, sizeof(Engine::TRIALVTX));

		pEffect->EndPass();
		pEffect->End();

		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	

	


}

void CBossSword::Free(void)
{
	Engine::CGameObject::Free();
}

HRESULT CBossSword::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	// Mesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(RESOURCE_STAGE, L"Mesh_Weapon"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);
		
	// transform
	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// renderer
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);

	

	// Collider
	pComponent = m_pColliderCom = Engine::CCollider2::Create(m_pGraphicDev, 
															m_pMeshCom->Get_VtxPos(),
															m_pMeshCom->Get_VtxCnt(),
															m_pMeshCom->Get_Stride()
															);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Collider", pComponent);

	//texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STATIC, L"Texture_SwordTrail4"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	// Shader
	pComponent = m_pBossSwordTrailShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_SwordTrail"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_ShaderBossSwordTrail", pComponent);


	// Shader
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Mesh"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);
	
	return S_OK;
}

HRESULT CBossSword::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
{
	_matrix		matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	// 조명

	const D3DLIGHT9*		pLightInfo = Engine::Get_Light(0);
	NULL_CHECK_RETURN(pLightInfo, E_FAIL);

	pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLightInfo->Diffuse);
	pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLightInfo->Specular);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLightInfo->Ambient);

	// 재질

	D3DMATERIAL9					tMtrlInfo;
	ZeroMemory(&tMtrlInfo, sizeof(D3DMATERIAL9));

	tMtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	tMtrlInfo.Power = 100.f;

	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&tMtrlInfo.Diffuse);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&tMtrlInfo.Ambient);
	pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&tMtrlInfo.Specular);

	pEffect->SetFloat("g_fPower", tMtrlInfo.Power);

	D3DXMatrixInverse(&matView, NULL, &matView);
	_vec4	vCamPos;
	memcpy(&vCamPos, &matView.m[3][0], sizeof(_vec4));

	pEffect->SetVector("g_vCamPos", &vCamPos);

	return S_OK;
}

HRESULT CBossSword::SetUp_BossSwordTrailConstantTable(LPD3DXEFFECT & pEffect)
{
	_matrix		matWorld, matView, matProj;

	D3DXMatrixIdentity(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	m_pTextureCom->Set_Texture(pEffect, "g_TrailTexture");
	// 조명

	const D3DLIGHT9*		pLightInfo = Engine::Get_Light(0);
	NULL_CHECK_RETURN(pLightInfo, E_FAIL);

	pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLightInfo->Diffuse);
	pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLightInfo->Specular);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLightInfo->Ambient);

	// 재질

	D3DMATERIAL9					tMtrlInfo;
	ZeroMemory(&tMtrlInfo, sizeof(D3DMATERIAL9));

	tMtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	tMtrlInfo.Power = 10.f;

	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&tMtrlInfo.Diffuse);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&tMtrlInfo.Ambient);
	pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&tMtrlInfo.Specular);

	pEffect->SetFloat("g_fPower", tMtrlInfo.Power);

	D3DXMatrixInverse(&matView, NULL, &matView);
	_vec4	vCamPos;
	memcpy(&vCamPos, &matView.m[3][0], sizeof(_vec4));

	pEffect->SetVector("g_vCamPos", &vCamPos);

	return S_OK;
}

HRESULT CBossSword::SetUp_BossSwordTrailConstantTable2(LPD3DXEFFECT & pEffect)
{
	return E_NOTIMPL;
}

CBossSword* CBossSword::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iFlag)
{
	CBossSword*	pInstance = new	CBossSword(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(iFlag)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}



