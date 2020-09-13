#pragma once

#include "msg_common_errorcode.h"

#pragma pack(push, 1)

CMD_START(NC_EnterWorld)
	DWORD		dwMapID;
	Vector3		curPos;
	Vector3		faceTo;
CMD_END

CMD_START(NS_EnterWorld)
	DWORD		dwRoleID;
	DWORD		dwMapID;
	Vector3		curPos;
	Vector3		faceTo;
CMD_END

CMD_START(NC_RequestAttackCode)
CMD_END

CMD_START(NS_RequestAttackCode)
	DWORD	dwCode;
CMD_END

CMD_START(NC_ServerAttack)
	DWORD dwCode;
CMD_END

#pragma pack(pop)