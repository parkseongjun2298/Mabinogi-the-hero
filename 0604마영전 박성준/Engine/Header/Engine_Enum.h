#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	// ID_DYNAMIC : 실시간 업데이트가 필요한 컴포넌트
	enum COMPONENTID { ID_DYNAMIC, ID_STATIC, ID_END };

	enum WINMODE { MODE_FULL, MODE_WIN };

	enum INFO { INFO_RIGHT, INFO_UP, INFO_LOOK, INFO_POS, INFO_END };
	
	enum ROTATION { ROT_X, ROT_Y, ROT_Z, ROT_END };

	enum RESOURCETYPE {	RESOURCE_STATIC, RESOURCE_STAGE, RESOURCE_END	};

	enum BUFFERID { BUFFER_TRICOL, BUFFER_RCTEX, BUFFER_RCTEXLEFT, BUFFER_TERRAINTEX, BUFFER_CUBETEX };
	
	enum TEXTURETYPE { TEX_NORMAL, TEX_CUBE, TEX_END };
	
	enum RENDERID { RENDER_PRIORITY, RENDER_NONALPHA, RENDER_NONALPHA_TOOL, RENDER_ALPHA, RENDER_UI, RENDER_END };
	
	enum COLLTYPE { COL_FALSE, COL_TRUE, COL_END };
	
	enum MESHTYPE { TYPE_STATIC, TYPE_DYNAMIC, TYPE_NAVI, TYPE_NONE};

	enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_MB, DIM_END };
	enum MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };

	enum CHANNELID {
		CH_BGM,
		CH_PLAYER,
		CH_PLAYEREFFECT,
		CH_PLAYEREFFECT2,
		CH_HITEFFECT,
		CH_PLAYERHIT,
		CH_EFFECT,
		CH_MONSTER,
		CH_BOSS,
		CH_BOSSEFFECT,
		CH_UI,
		CH_EXAMPLE,
		CH_MAXCHANNEL
	};

}

#endif // Engine_Enum_h__
