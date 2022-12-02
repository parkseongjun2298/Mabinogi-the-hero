CBase::CBase(void)
	: m_dwRefCnt(0)
{

}
CBase::~CBase(void)
{

}

unsigned long CBase::AddRef(void)
{
	return m_dwRefCnt++;
}
unsigned long CBase::Release(void)
{
	if (0 == m_dwRefCnt)
	{
		Free();

		delete this;

		return 0;
	}

	return m_dwRefCnt--;
}

// 전위 연산 : 선연산 후대입
// 후위 연산 : 선대입 후연산
//
//int a = 3;
//
//while (--a)
//{
//	cout << "출력" << endl;
//}