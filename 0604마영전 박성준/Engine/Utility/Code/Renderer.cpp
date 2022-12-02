#include "Renderer.h"

USING(Engine)
IMPLEMENT_SINGLETON(CRenderer)

Engine::CRenderer::CRenderer(void)
{

}

Engine::CRenderer::~CRenderer(void)
{
	Free();
}

void Engine::CRenderer::Add_Render_Group(RENDERID eID, CGameObject* pGameObject)
{
	if (RENDER_END <= eID)
		return;

	m_RenderGroup[eID].push_back(pGameObject);

	pGameObject->AddRef();
}

void Engine::CRenderer::Render_GameObject(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	//for (_uint i = 0; i < RENDER_END; ++i)
	//{
	//	for (auto& iter : m_RenderGroup[i])
	//	{
	//		iter->Render_GameObject();
	//		Safe_Release(iter);			// 지우는 게 아님, 레퍼런스 카운트를 감소
	//	}
	//	m_RenderGroup[i].clear();
	//}

	Render_Priority(pGraphicDev);
	Render_NonAlpha(pGraphicDev);
	Render_NonAlpha_Tool(pGraphicDev);
	Render_Alpha(pGraphicDev);
	Render_UI(pGraphicDev);

	Clear_RenderGroup();

}

void Engine::CRenderer::Clear_RenderGroup(void)
{
	for (_uint i = 0; i < RENDER_END; ++i)
	{
		for_each(m_RenderGroup[i].begin(), m_RenderGroup[i].end(), CDeleteObj());
		m_RenderGroup[i].clear();
	}
}

void CRenderer::Render_Priority(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);

	for (auto& iter : m_RenderGroup[RENDER_PRIORITY])
		iter->Render_GameObject();

	pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void CRenderer::Render_NonAlpha(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (auto& iter : m_RenderGroup[RENDER_NONALPHA])
		iter->Render_GameObject();

	pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void CRenderer::Render_NonAlpha_Tool(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (auto& iter : m_RenderGroup[RENDER_NONALPHA_TOOL])
		iter->Render_GameObject();

	pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

_bool Compare_Z(CGameObject* pDest, CGameObject* pSour)
{
	return pDest->Get_ViewZ() > pSour->Get_ViewZ();
}


void CRenderer::Render_Alpha(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	/*pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xc0);*/

	m_RenderGroup[RENDER_ALPHA].sort(Compare_Z);

	for (auto& iter : m_RenderGroup[RENDER_ALPHA])
		iter->Render_GameObject();

	//pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void CRenderer::Render_UI(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xc0);

	for (auto& iter : m_RenderGroup[RENDER_UI])
		iter->Render_GameObject();

	pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void Engine::CRenderer::Free(void)
{
	Clear_RenderGroup();
}

CComponent * CRenderer::Clone(void)
{
	return this->GetInstance();
}

