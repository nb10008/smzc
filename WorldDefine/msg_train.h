//-----------------------------------------------------------------------------
// filename: msg_train.h
// author: lfmeng
// actor:
// data: 2010-3-10
// last:
// brief: 龙之试炼系统消息定义
//-----------------------------------------------------------------------------

#pragma once

#include "train_define.h"

#pragma pack(push, 1)

//----------------------------------------------------------------------------
// 错误码枚举
//----------------------------------------------------------------------------
enum ETrainErrCode
{
	E_Train_Success	= 0,
	E_Train_List_Null,			// 试炼列表为空
	E_Train_Bag_Full,			// 行囊空间不足，请为行囊清理出足够空间再重新点击完成试炼！
	E_Train_Not_Pass,			// 不能通过试炼
	E_Train_Pass,				// 通过试炼
	E_Train_Unknown,			// 未知错误
};

//-----------------------------------------------------------------------------
// 取试炼列表
//-----------------------------------------------------------------------------
CMD_START(NC_GetTrainList)
CMD_END

CMD_START(NS_GetTrainList)
	ETrainErrCode		eTrainErrCode;
	INT					nTrainCnt;
	tagTrainState		pTrainAttTable[1];
CMD_END

//-----------------------------------------------------------------------------
// 完成试炼
//-----------------------------------------------------------------------------
CMD_START(NC_FinishTrain)
	DWORD 			dwTrainID;
CMD_END

CMD_START(NS_FinishTrain)
	ETrainErrCode	eTrainErrCode;
	DWORD			dwTrainID;				// 试炼ID
	ETrainState		eTrainState;			// 试炼状态
CMD_END

//-----------------------------------------------------------------------------
// 开启试炼
//-----------------------------------------------------------------------------
CMD_START(NS_OpenTrain)
	DWORD dwTrainID;				// 试炼ID
CMD_END

//-----------------------------------------------------------------------------
// 同步试炼完成度
//-----------------------------------------------------------------------------
CMD_START(NS_SyncTrainCompleteness)
	DWORD dwTrainID;				// 试炼ID
	DWORD dwCompleteness;
CMD_END

#pragma pack(pop)