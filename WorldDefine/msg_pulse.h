#pragma once

#pragma pack(push, 1)

//-----------------------------------------------------------------------
// 错误码
//-----------------------------------------------------------------------
enum
{
	E_Pulse_Success = 0,						// 成功
	E_Pulse_NoItem,							// 道具不足	
	E_Pulse_NoMoney,						// 金钱不足
	E_Pulse_LessLevel,						// 等级不足	
	E_Pulse_NoPerSkill,						// 前置技能尚未学会
	E_Pulse_LessGodLevel,					// 神格不足
	E_Pulse_MaxLevel,						// 等级已经达到最高等级
	E_Pulse_NoProto,							// 找不到经脉圆形
	E_Pulse_MaxTimePerDay,				// 今天次数已经满了
	E_Pulse_Fail,									// 修炼失败
};

//-----------------------------------------------------------------------------
//	激活血脉
//-----------------------------------------------------------------------------
CMD_START(NC_ActivePulse)	
	DWORD dwPulseID;				// 要激活的经脉的ID
CMD_END

CMD_START(NS_ActivePulse)	
	DWORD dwid;				// 激活的经脉的ID
	INT		nLearnTime;
	DWORD dwErrorCode;
CMD_END

//-----------------------------------------------------------------------------
//	剩余激活血脉次数
//-----------------------------------------------------------------------------
CMD_START(NS_PulseCanLearnNum)	
	DWORD	dwNum;
CMD_END