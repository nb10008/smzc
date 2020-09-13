#pragma once

#include "../WorldDefine/clergy_define.h"
//#include "../ServerDefine/clergy_define.h"
#include	"role_data_define.h"

#pragma pack(push, 1)

CMD_START( NDBC_DeleteRoleArea )
	DWORD dwRoleID;
	DWORD dwAreaID;
CMD_END

CMD_START( NDBC_CreateRoleArea )
	tagAreaSave	area;
CMD_END

#pragma pack(pop)