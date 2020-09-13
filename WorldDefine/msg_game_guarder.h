//-----------------------------------------------------------------------------
// filename: msg_fatigue_guarder.h
// author: client/server xtian/xlguo
// actor:
// data: 2009-03-02
// last: 
// brief: 反外挂
//-----------------------------------------------------------------------------
#pragma once
#pragma pack(push, 1)

//----------------------------------------------------------------------------
// 反外挂消息
//----------------------------------------------------------------------------

#define XTRAP_CMD_STEP_ONE	0x0001
#define XTRAP_CMD_STEP_TWO	0x0010
#define XTRAP_BUFFER_SIZE			128

CMD_START(NC_GameGuarder)
	INT16	nLen;
	char	chCmd;
	char	chData[1];
CMD_END

CMD_START(NS_GameGuarder)
	INT16	nLen;
	char	chCmd;
	char	chData[1];
CMD_END

CMD_START(NS_XTrap)
	int	cmd;
	BYTE	buffer[XTRAP_BUFFER_SIZE];
CMD_END

CMD_START(NC_XTrap)
	int	cmd;
	BYTE	buffer[XTRAP_BUFFER_SIZE];
CMD_END

#pragma pack(pop)