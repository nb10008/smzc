//-----------------------------------------------------------------------------
// filename: msg_train.h
// author: lfmeng
// actor:
// data: 2010-04-02
// last:
// brief: 龙之试炼系统--LongDB和LoongWorld通信
//-----------------------------------------------------------------------------
#pragma once

#include "../WorldDefine/train_define.h"

#pragma pack(push, 1)

//-----------------------------------------------------------------------------
// 保存已经开启的试炼状态
//-----------------------------------------------------------------------------
CMD_START(NDBC_SaveOpenTrainState)
	DWORD	dwRoleID;
	DWORD	dwTrainID;
	ETrainState eTrainState;
	DWORD dwCompleteness;
CMD_END

#pragma pack(pop)