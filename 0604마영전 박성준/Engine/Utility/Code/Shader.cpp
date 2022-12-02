#include "Shader.h"

USING(Engine)

Engine::CShader::CShader(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_pEffect(nullptr)
{
	m_pGraphicDev->AddRef();
}

Engine::CShader::CShader(const CShader& rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev)
	, m_pEffect(rhs.m_pEffect)
	, m_pErrMsg(rhs.m_pErrMsg)
{
	m_pGraphicDev->AddRef();
	m_pEffect->AddRef();
}

Engine::CShader::~CShader(void)
{

}

HRESULT Engine::CShader::Ready_Shader(const _tchar* pFilePath)
{
	// m_pEffect != NULL && m_pErrMsg == NULL 인 경우 : 정상적인 쉐이더 객체가 생성된 경우
	// m_pEffect  == NULL && m_pErrMsg != NULL 인 경우 : 쉐이더 객체 생성이 실패한 경우
	// m_pEffect != NULL && m_pErrMsg != NULL 인 경우 : 쉐이더 객체가 생성은 됐지만 경고 메시지가 있는 경우


	if (FAILED(D3DXCreateEffectFromFile(m_pGraphicDev, 
										pFilePath,
										NULL, // 쉐이더에서 사용할 메크로 파일의 주소
										NULL, // 쉐이더에서 사용할 포함 파일의 주소
										D3DXSHADER_DEBUG, // 쉐이더 디버깅의 의미(외부 라이브러리 파일을 사용할 경우, 인자값으로 넣어줄 필요가 없다. 하지만 만약을 대비해서 기본 기능을 포함하겠다고 넣은 인자) 
										NULL,	// 각종 리소스 정보를 어디에 저장할 것인가(null인경우 그래픽 장치에 보관하겠다는 의미)
										&m_pEffect, // 쉐이더 컴객체
										&m_pErrMsg))) // 에러 및 경고 메세지를 문자열 상태로 저장하여 보관하는 포인터
	{
		MessageBoxA(NULL, (char*)m_pErrMsg->GetBufferPointer(), "Shader_Error", MB_OK);
		return E_FAIL;
	}

	else if (nullptr != m_pErrMsg)
	{
		MessageBoxA(NULL, (char*)m_pErrMsg->GetBufferPointer(), "Shader_Warning", MB_OK);
	}

	return S_OK;
}

CShader* Engine::CShader::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath)
{
	CShader*	pInstance = new CShader(pGraphicDev);

	if (FAILED(pInstance->Ready_Shader(pFilePath)))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent* Engine::CShader::Clone(void)
{
	return new CShader(*this);
}

void Engine::CShader::Free(void)
{
	Safe_Release(m_pEffect);
	Safe_Release(m_pGraphicDev);
}

