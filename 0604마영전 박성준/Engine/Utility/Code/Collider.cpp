#include "Collider.h"

USING(Engine)

Engine::CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev), m_fRadius(1.f)
{
	m_pGraphicDev->AddRef();
}

Engine::CCollider::~CCollider(void)
{

}

HRESULT Engine::CCollider::Ready_Collider(const _vec3* pPos,
	const _ulong& dwVtxCnt,
	const _ulong& dwStride,
	CMesh* pMesh,
	Engine::MESHTYPE Type)
{

	BYTE* pV;
	if (Type == Engine::MESHTYPE::TYPE_STATIC)
	{
		m_pMesh = dynamic_cast<CStaticMesh*>(pMesh)->Get_Mesh();
		

	}
	if (Type == Engine::MESHTYPE::TYPE_DYNAMIC)
	{
		m_pMesh = dynamic_cast<CDynamicMesh*>(pMesh)->Get_MeshContainerList().front()->pOriMesh;
		
	}

	m_pMesh->LockVertexBuffer(0, (void**)&pV);
	D3DXComputeBoundingSphere((_vec3*)pV, m_pMesh->GetNumVertices(), m_pMesh->GetNumBytesPerVertex(), &(m_vCenter), &m_fRadius);
	m_pMesh->UnlockVertexBuffer();
	//m_fRadius = 20.f;
	
	D3DXCreateSphere(m_pGraphicDev, m_fRadius , 10, 10, &m_pMesh, NULL);
	



	return S_OK;
}

void Engine::CCollider::Render_Collider(COLLTYPE eType, const _matrix* pColliderMatrix)
{
	//m_matWorld = *pColliderMatrix;

#ifdef _DEBUG

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, pColliderMatrix);


	m_pMesh->DrawSubset(0);
	
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
#endif
}

void CCollider::Update_Component(const _float & fTimeDelta)
{
	
	memcpy(&m_vCenter, &m_matWorld.m[3], sizeof(_vec3));// 중점
	m_fScale = m_fRadius * m_matWorld.m[1][1];	// 반지름

	
	
}

void CCollider::Update(_matrix matWorld)
{
	m_matWorld = matWorld; 
	
}

void CCollider::Set_Pos(const _float & fX, const _float & fY, const _float & fZ)
{
	
		m_matWorld._41 = fX;
		m_matWorld._42 = fY;
		m_matWorld._43 = fZ;
	
}

CCollider* Engine::CCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPos, const _ulong& dwVtxCnt, const _ulong& dwStride, CMesh* pMesh, Engine::MESHTYPE Type)
{
	CCollider*	pInstance = new CCollider(pGraphicDev);

	if (FAILED(pInstance->Ready_Collider(pPos, dwVtxCnt, dwStride, pMesh, Type)))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CCollider::Free(void)
{
#ifdef _DEBUG
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
	Safe_Release(m_pMesh);
	Safe_Release(m_pGraphicDev);

	for (_uint i = 0; i < COL_END; ++i)
		Safe_Release(m_pTexture[i]);
#endif

}

