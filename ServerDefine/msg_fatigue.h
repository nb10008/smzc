#pragma once
#include "fatigue_define.h"
#pragma pack(push,1)

//-------------------------------------------------------------------------
// login 与world的防沉迷消息
//-------------------------------------------------------------------------
CMD_START(NLW_FatigueNotify)
	INT				nNum;
	tagFatigueNotify	notify[1];
CMD_END

#pragma pack(pop)