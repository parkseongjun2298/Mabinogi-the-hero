#ifndef Transform_h__
#define Transform_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform : public CComponent
{
private:
	explicit CTransform(void);
	virtual ~CTransform(void);

public:
	void				Get_Info(INFO eType, _vec3* pInfo);
	const _vec3*		Get_Info(INFO eType)const;
	void				Move_Pos(const _vec3* const pDir);
	void				Rotation(ROTATION eType, const _float& fAngle);
	void				Set_Rotate(ROTATION eType, _float fAngle);
	void				Set_Pos(const _float& fX, const _float& fY, const _float& fZ);
	void				Set_Pos(_vec3* pPos);

	void				Chase_Target(const _vec3* pTargetPos, const _float& fSpeed);
	const _matrix*		Compute_LookAtTarget(const _vec3* pTargetPos);
	void				Get_WorldMatrix(_matrix* pWorld) const;
	const _matrix*		Get_NRotWorldMatrix(_matrix* pWorld) const;
	void				Move_TargetPos(const _vec3* pTargetPos, const _float& fSpeed, const _float& fTimeDelta);

	void				Set_ParentMatrix(const _matrix* pParent);
	const _matrix*		Get_WorldMatrixPointer(void) const;

public:
	HRESULT				Ready_Transform(void);
	virtual void		Update_Component(const _float& fTimeDelta);

public:
	_vec3		m_vInfo[INFO_END];
	_vec3		m_vScale;
	_vec3		m_vAngle;
	_matrix		m_matWorld;
	_matrix		m_matNRotWorld;
	_matrix		matRot[ROT_END];
public:
	static CTransform*		Create(void);
private:
	virtual void Free(void);

};


END
#endif // Transform_h__
