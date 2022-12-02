#ifndef Engine_Struct_h__
#define Engine_Struct_h__

#include "Engine_Enum.h"
namespace Engine
{
	typedef	struct tagVertexColor
	{		
		_vec3			vPos;
		_ulong			dwColor;

	}VTXCOL;

	const _ulong		FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE;

	typedef	struct tagVertexTexture
	{
		_vec3			vPos;
		_vec3			vNormal;
		_vec2		 	vTexUV;

	}VTXTEX;

	const _ulong		FVF_TEX = D3DFVF_XYZ  | D3DFVF_NORMAL | D3DFVF_TEX1;

	typedef	struct tagVertexCubeTexture
	{
		_vec3			vPos;
		_vec3		 	vTex;

	}VTXCUBE;

	const _ulong		FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

	typedef struct tagScreenVertex
	{
		_vec4				vPos;
		_vec2				vTexUV;

	}VTXSCREEN;

	const _ulong		FVF_SCREEN = D3DFVF_XYZRHW | D3DFVF_TEX1;

	typedef	struct tagIndex16
	{
		_ushort	_0, _1, _2;

	}INDEX16;

	typedef	struct tagIndex32
	{
		_ulong	_0, _1, _2;

	}INDEX32;


	typedef	struct D3DXFRAME_DERIVED : public D3DXFRAME 
	{
		_matrix			CombinedTransformMatrix;
		
	}D3DXFRAME_DERIVED;

	typedef struct D3DXMESHCONTAINER_DERIVED : public D3DXMESHCONTAINER
	{
		LPDIRECT3DTEXTURE9*		ppTexture;

		LPD3DXMESH				pOriMesh;	// 최초의 상태를 가지고 있는 메쉬 객체(불변하는 데이터)

		_ulong					dwNumBones; // 뼈의 개수

		_matrix*				pFrameOffsetMatrix; // (애니메이션 정보를 포함하고 있는)뼈대들이 갖고 있는 최초의 행렬 상태

		_matrix**				ppFrameCombinedTransformMatrix; // CombinedTransformMatrix 행렬을 실시간으로 계속 갱신되기 때문에 아예 주소값들을 모아서 따로 보관하려 한다.
																//	ppFrameCombinedTransformMatrix을 통해 순회하며 모든 행렬에 접근 할 수 있게 된다.

		_matrix*				pRenderMatrix; // == pFrameOffsetMatrix * (*ppFrameCombinedMatrix) 최종적으로 출력을 하기 위한 행렬의 주소
		
	}D3DXMESHCONTAINER_DERIVED;

	struct MeshInfo
	{
		D3DXVECTOR3 vPos;
		D3DXVECTOR3 vScale;
		D3DXVECTOR3 vRot;
		Engine::MESHTYPE eType;
	};

	typedef struct tagTrailVertex
	{
		_vec3 vPosition;
		DWORD dwColor;
		_vec2 vTexUV;
	}TRIALVTX;
	const _ulong		FVF_TRAIL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;


}
#endif // Engine_Struct_h__
