#include "StdAfx.h"
#include "player_session.h"
#include "role.h"
#include "creature.h"
#include "map.h"
#include "script_mgr.h"

#include "../WorldDefine/role_att.h"

DWORD PlayerSession::HandleTalkToNPC(tagNetCmd* pCmd)
{
	DWORD dwStart = timeGetTime();

	MTRANS_POINTER(pRecv, pCmd, tagNC_NPCTalk);

	MTRANS_ELSE_RET(pRole,		GetRole(),							Role,				GT_INVALID);
	MTRANS_ELSE_RET(pMap,		pRole->GetMap(),					Map,				GT_INVALID);
	MTRANS_ELSE_RET(pNpc,		pMap->FindCreature(pRecv->dwNPCId), Creature,			GT_INVALID);
	MTRANS_ELSE_RET(pScript,	pNpc->GetScript(),					CreatureScript,		GT_INVALID);

	// ´¥·¢Ì¸»°
	pScript->OnTalk(pNpc, pRole, -1);
	DWORD dwEnd = timeGetTime();
	if (dwEnd - dwStart > 200)
	{
		ILOG->Write(_T("NC_NPCTalk too long:%d dwNPCId:%d\r\n"),
			(dwEnd - dwStart), pRecv->dwNPCId);
	}
	return 0;
}