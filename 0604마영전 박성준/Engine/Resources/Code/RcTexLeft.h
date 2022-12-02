#ifndef RcTexLeft_h__
#define RcTexLeft_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcTexLeft : public CVIBuffer
{
private:
	explicit CRcTexLeft(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRcTexLeft(const CRcTexLeft& rhs);
	virtual ~CRcTexLeft(void);

public:
	virtual HRESULT		Ready_Buffer(void);

public:
	static CRcTexLeft*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CResources* Clone(void);
	virtual void	Free(void);
};


END
#endif //RcTexLeft_h__
