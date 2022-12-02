#ifndef Cell_h__
#define Cell_h__

#include "Engine_Defines.h"
#include "Base.h"
#include "Line.h"

BEGIN(Engine)

class ENGINE_DLL CCell : public CBase 
{
public:
	enum POINT		{	POINT_A, POINT_B, POINT_C, POINT_END };
	enum NEIGHBOR	{	NEIGHBOR_AB, NEIGHBOR_BC,NEIGHBOR_CA, NEIGHBOR_END };
	enum LINE		{	LINE_AB, LINE_BC, LINE_CA, LINE_END };
	enum COMPARE	{	COMPARE_MOVE, COMPARE_STOP, COMPARE_END };


private:
	explicit CCell(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCell(void);

public:
	const	_vec3*		Get_Point(POINT eType) const { return &m_vPoint[eType]; }
	_vec3		Get_PointNoConst(POINT eType) { return m_vPoint[eType]; }
	CCell*				Get_Neighbor(NEIGHBOR eType) const { return m_pNeighbor[eType]; }
	const _ulong*		Get_Index(void) { return &m_dwIndex; }
	void				Set_Neighbor(NEIGHBOR eType, CCell* pNeighbor) { m_pNeighbor[eType] = pNeighbor; }

public:
	HRESULT		Ready_Cell(const _ulong& dwIndex,
							BYTE& byOption,
							const _vec3* pPointA,
							const _vec3* pPointB,
							const _vec3* pPointC);

	_bool		Compare_Point(const _vec3* pPointFirst, const _vec3* pPointSecond, CCell* pCell);
	void		Render_Cell(void);

	COMPARE		Compare(const _vec3* pEndPos, _ulong* pIndex);
	COMPARE		Compare(const _vec3* pEndPos, _ulong* pIndex, _vec2* NormalInLine);
private:
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	CCell*		m_pNeighbor[NEIGHBOR_END];
	CLine*		m_pLine[LINE_END];
	LPD3DXLINE	m_pD3DXLine;
	_ulong		m_dwIndex;
public:
	_vec3		m_vPoint[POINT_END];
	_bool		m_bIsSelect = false;
public:
	static CCell*		Create(LPDIRECT3DDEVICE9 pGraphicDev, 
								const _ulong& dwIndex,
								BYTE& byOption,
								const _vec3* pPointA,
								const _vec3* pPointB, 
								const _vec3* pPointC);
	virtual void		Free(void);
	BYTE		m_byOption;
};

END
#endif // Cell_h__
