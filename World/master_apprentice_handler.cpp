//-----------------------------------------------------------------------------
// Copyright (c) 2004 TENGWU Entertainment All rights reserved.
// filename: master_apprentice_handler.h
// author: twan
// actor:
// data: 2010-01-20
// last: 
// brief: 师徒相关
//-----------------------------------------------------------------------------
#include "StdAfx.h"

#include "player_session.h"
#include "role.h"
#include "role_mgr.h"
#include "social_mgr.h"
#include "att_res.h"
#include "currency.h"
#include "../WorldDefine/msg_master_apprentice.h"
#include "../WorldDefine/msg_social.h"
#include "../WorldDefine/SocialDef.h"
#include "../WorldDefine/time.h"
#include "../WorldDefine/chat.h"
#include "../ServerDefine/role_data_define.h"
#include "../ServerDefine/master_apprentice.h"
#include "../ServerDefine/log_cmdid_define.h"
#include "force_breakout_apprentice_mgr.h"

//--------------------------------------------------------------------------
// 徒弟申请拜师
//--------------------------------------------------------------------------
DWORD PlayerSession::HandleApprenticeApplyMaster(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_ApprenticeApplyMaster);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Role* pDestRole = g_roleMgr.GetRolePtrByID(pRecv->dwRoleID);
	if (!P_VALID(pDestRole))
	{
		return GT_INVALID;
	}

	tagNS_ApprenticeApplyMaster send;

	// 已有师傅
	if (pRole->GetMasterID() != GT_INVALID)
	{
		send.dwErrorCode = E_MA_Already_Has_Master;
	}

	// 等级大于等于50级
	else if (pRole->GetLevel() >= MASTER_QUALIFICATION_LEVEL)
	{
		send.dwErrorCode = E_MA_Level_Limit;
	}

	// 对方尚无资格收徒
	else if (!pDestRole->IsHaveDoneQuest(MASTER_QUALIFICATION_QUEST_ID))
	{
		send.dwErrorCode = E_MA_DestRole_Have_No_Qualification;
	}

	// 对方徒弟数量已满
	else if (pDestRole->GetApprenticeData().Size() >= MAX_APPRENTICE_NUM)
	{
		send.dwErrorCode = E_MA_DestRole_Apprentice_Full;
	}

	// 自己的好友栏位已满且对方不为自己好友
	else if ( pRole->GetFriendCount() >= MAX_FRIENDNUM && !P_VALID(pRole->GetFriendPtr(pRecv->dwRoleID)) )
	{
		send.dwErrorCode = E_MA_Friend_Full;
	}

	else
	{
		send.dwErrorCode = E_MA_Success;
	}

	SendMessage(&send, send.dwSize);

	if (send.dwErrorCode == E_MA_Success)
	{
		// 发送通知给师傅方
		tagNS_MasterNotice sendMaster;
		sendMaster.dwRoleID = pRole->GetID();
		sendMaster.nLevel = pRole->GetLevel();
		pDestRole->GetSession()->SendMessage(&sendMaster, sendMaster.dwSize);
	}

	return 0;
}

//--------------------------------------------------------------------------
// 师傅的回复
//--------------------------------------------------------------------------
DWORD PlayerSession::HandleMasterReply(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_MasterReply);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Role* pDestRole = g_roleMgr.GetRolePtrByID(pRecv->dwRoleID);
	if (!P_VALID(pDestRole))
	{
		return GT_INVALID;
	}

	// 如果已经是师徒关系
	if (pDestRole->GetMasterID() == pRole->GetID())
	{
		return GT_INVALID;
	}

	tagNS_MasterReply				sendMaster;			// 发给师傅的消息
	tagNS_MasterReplyApprentice		sendApprentice;		// 发给双方的消息

	sendApprentice.bAccept = pRecv->bAccept;
	sendApprentice.dwRoleID = pRecv->dwRoleID;			// 徒弟ID

	// 如果拒绝
	if (!pRecv->bAccept)
	{
		sendMaster.dwErrorCode = E_MA_Success;
		sendApprentice.dwErrorCode = E_MA_Dest_Refuse_BaiShi;
		
	}

	// 如果接受
	else
	{
		// 判断双方好友数量是否已满
		if ( pRole->GetFriendCount() >= MAX_FRIENDNUM && !P_VALID(pRole->GetFriendPtr(pRecv->dwRoleID)) )
		{
			sendMaster.dwErrorCode = E_MA_Friend_Full;
			sendApprentice.dwErrorCode = E_MA_Dest_Friend_Full;
			// 发送给师傅
			SendMessage(&sendMaster, sendMaster.dwSize);

			// 发送给徒弟
			sendApprentice.bAccept = false;
			sendApprentice.dwRoleID = pRole->GetID();
			pDestRole->GetSession()->SendMessage(&sendApprentice, sendApprentice.dwSize);
			return 0;
		}

		else if ( pDestRole->GetFriendCount() >= MAX_FRIENDNUM && !P_VALID(pDestRole->GetFriendPtr(pRole->GetID())) )
		{
			sendMaster.dwErrorCode = E_MA_Dest_Friend_Full;
			sendApprentice.dwErrorCode = E_MA_Friend_Full;
			// 发送给师傅
			SendMessage(&sendMaster, sendMaster.dwSize);

			// 发送给徒弟
			sendApprentice.bAccept = false;
			sendApprentice.dwRoleID = pRole->GetID();
			pDestRole->GetSession()->SendMessage(&sendApprentice, sendApprentice.dwSize);
			return 0;
		}

		// 成功
		else
		{
			sendMaster.dwErrorCode = E_MA_Success;
			sendApprentice.dwErrorCode = E_MA_Success;

			// 互加为好友
			tagNC_RoleMakeFriend sendMasterMakeFriend;
			tagNC_RoleMakeFriend sendApprenticeMakeFriend;
			sendMasterMakeFriend.dwDestRoleID = pDestRole->GetID();
			sendApprenticeMakeFriend.dwDestRoleID = pRole->GetID();
			g_socialMgr.AddEvent(pRole->GetID(), EVT_MakeFriend, sendMasterMakeFriend.dwSize, &sendMasterMakeFriend);
			g_socialMgr.AddEvent(pDestRole->GetID(), EVT_MakeFriend, sendApprenticeMakeFriend.dwSize, &sendApprenticeMakeFriend);

			// 互加为师徒
			pRole->AddApprentice(pDestRole->GetID());
			pDestRole->SetMasterID(pRole->GetID());

			// 将同门信息同步给徒弟
			pRole->GetApprenticeData().ResetIterator();
			DWORD dwMateID = 0;
			while (pRole->GetApprenticeData().PeekNext(dwMateID))
			{
				// 互为同门
				pDestRole->AddClassMate(dwMateID);
				Role* pMate = g_roleMgr.GetRolePtrByID(dwMateID);
				if (P_VALID(pMate))
				{
					pMate->AddClassMate(pRecv->dwRoleID);
				}
			}
			pDestRole->GetClassMate().nJingWuPoint = pRole->GetJingWuPoint();
			pDestRole->GetClassMate().nQinWuPoint = pRole->GetQinWuPoint();

			// 保存到数据库
			//DWORD dwSize = sizeof(DWORD) + sizeof(tagOneMAData2DB) + sizeof(tagApprenticeData);
			//MCREATE_MSG(pSend, dwSize, NDBC_SaveOneMasterApprenticeData);
			tagNDBC_SaveOneMasterApprenticeData send;

			send.dwMasterID = pRole->GetID();
			send.sOneMAData.eType = EOMAD_Apprentice;
			send.sOneMAData.eMode = EMAMM_Mod;
			*(DWORD*)(send.sOneMAData.byData) = pDestRole->GetID();

			g_dbSession.Send(&send, send.dwSize);

			// 跑马灯公告
			//tstringstream tszMsg;
			//TCHAR tszRoleName[X_SHORT_NAME] = {0};
			//g_roleMgr.GetRoleNameByID(pRole->GetID(), tszRoleName);
			//tszMsg << tszRoleName << _T("已将");
			//g_roleMgr.GetRoleNameByID(pDestRole->GetID(), tszRoleName);
			//tszMsg << tszRoleName << _T("收为徒弟！");

			//DWORD dwLen = sizeof(tagNS_AutoNotice) - sizeof(TCHAR) + 2*(tszMsg.str().length() + 1);
			//MCREATE_MSG(pSendMsg, dwLen, NS_AutoNotice);

			//pSendMsg->nCirInterval = 60;
			//pSendMsg->nType = 2;		// 绿色字体
			//_tcscpy(pSendMsg->szContent, tszMsg.str().c_str());
			//g_roleMgr.SendWorldMsg(pSendMsg, dwLen);
			//MDEL_MSG(pSendMsg);
			const RoleScript* pScript = g_ScriptMgr.GetRoleScript();
			if (P_VALID(pScript))
			{
				pScript->OnHandleMaster(pRole, pDestRole);
			}

			// 设置双方的师徒关系
			pRole->ModifyMasterApprenticeCount(1);
			pDestRole->ModifyMasterApprenticeCount(1);

			// 同步师傅和他的徒弟的
			pRole->SyncOnlineNum2Role(ESOR_Online_ClassMate_Num, pRole->GetOnlineClassmatesCnt());
			pRole->SyncOnlineNum2Apprentices();
		}
	}

	// 发送给师傅
	SendMessage(&sendMaster, sendMaster.dwSize);

	// 发送给双方
	sendApprentice.dwRoleID = pRole->GetID();
	pDestRole->GetSession()->SendMessage(&sendApprentice, sendApprentice.dwSize);

	if (sendApprentice.dwErrorCode != E_MA_Success)
	{
		sendApprentice.dwErrorCode = E_MA_You_Refuse_BaiShi;
	}
	
	sendApprentice.dwRoleID = pDestRole->GetID();
	SendMessage(&sendApprentice, sendApprentice.dwSize);

	return 0;
}

//--------------------------------------------------------------------------
// 师傅主动收徒
//--------------------------------------------------------------------------
DWORD PlayerSession::HandleMasterAcceptApprentice(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_MasterAcceptApprentice);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Role* pDestRole = g_roleMgr.GetRolePtrByID(pRecv->dwRoleID);
	if (!P_VALID(pDestRole))
	{
		return GT_INVALID;
	}

	tagNS_MasterAcceptApprentice send;

	// 对方已有师傅
	if (pDestRole->GetMasterID() != GT_INVALID)
	{
		send.dwErrorCode = E_MA_Dest_Already_Has_Master;
	}

	// 对方等级大于等于50级
	else if (pDestRole->GetLevel() >= MASTER_QUALIFICATION_LEVEL)
	{
		send.dwErrorCode = E_MA_Dest_Level_Limit;
	}

	// 您尚无资格收徒
	else if (!pRole->IsHaveDoneQuest(MASTER_QUALIFICATION_QUEST_ID))
	{
		send.dwErrorCode = E_MA_Have_No_Qualification;
	}

	// 您徒弟数量已满
	else if (pRole->GetApprenticeData().Size() >= MAX_APPRENTICE_NUM)
	{
		send.dwErrorCode = E_MA_Apprentice_Full;
	}

	// 自己的好友栏位已满且对方不为自己好友
	else if ( pRole->GetFriendCount() >= MAX_FRIENDNUM && !P_VALID(pRole->GetFriendPtr(pRecv->dwRoleID)) )
	{
		send.dwErrorCode = E_MA_Friend_Full;
	}

	else
	{
		send.dwErrorCode = E_MA_Success;
	}

	SendMessage(&send, send.dwSize);

	if (send.dwErrorCode == E_MA_Success)
	{
		// 发送通知给徒弟方
		tagNS_ApprenticeNotice sendApprentice;
		sendApprentice.dwRoleID = pRole->GetID();
		sendApprentice.nLevel = pRole->GetLevel();
		pDestRole->GetSession()->SendMessage(&sendApprentice, sendApprentice.dwSize);
	}

	return 0;
}

//--------------------------------------------------------------------------
// 徒弟的回复
//--------------------------------------------------------------------------
DWORD PlayerSession::HandleApprenticeReply(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_ApprenticeReply);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	Role* pDestRole = g_roleMgr.GetRolePtrByID(pRecv->dwRoleID);
	if (!P_VALID(pDestRole))
	{
		return GT_INVALID;
	}

	// 已经是师徒关系
	if (pRole->GetMasterID() == pDestRole->GetID())
	{
		return GT_INVALID;
	}

	tagNS_ApprenticeReply				sendApprentice;			// 发给徒弟的消息
	tagNS_ApprenticeReplyMaster			sendMaster;				// 发给双方的消息

	sendMaster.bAccept = pRecv->bAccept;
	sendMaster.dwRoleID = pRecv->dwRoleID;

	// 如果拒绝
	if (!pRecv->bAccept)
	{
		sendMaster.dwErrorCode = E_MA_Dest_Refuse_ShouTu;
		sendApprentice.dwErrorCode = E_MA_Success;

	}

	// 如果接受
	else
	{
		// 判断双方好友数量是否已满
		if ( pRole->GetFriendCount() >= MAX_FRIENDNUM && !P_VALID(pRole->GetFriendPtr(pRecv->dwRoleID)) )
		{
			sendMaster.dwErrorCode = E_MA_Dest_Friend_Full;
			sendApprentice.dwErrorCode = E_MA_Friend_Full;

			// 发送给徒弟
			SendMessage(&sendApprentice, sendApprentice.dwSize);

			// 发送给师傅
			sendMaster.bAccept = false;
			sendMaster.dwRoleID = pRole->GetID();
			pDestRole->GetSession()->SendMessage(&sendMaster, sendMaster.dwSize);
			return 0;
		}

		else if ( pDestRole->GetFriendCount() >= MAX_FRIENDNUM && !P_VALID(pDestRole->GetFriendPtr(pRole->GetID())) )
		{
			sendMaster.dwErrorCode = E_MA_Friend_Full;
			sendApprentice.dwErrorCode = E_MA_Dest_Friend_Full;

			// 发送给徒弟
			SendMessage(&sendApprentice, sendApprentice.dwSize);

			// 发送给师傅
			sendMaster.bAccept = false;
			sendMaster.dwRoleID = pRole->GetID();
			pDestRole->GetSession()->SendMessage(&sendMaster, sendMaster.dwSize);
			return 0;
		}

		// 成功
		else
		{
			sendMaster.dwErrorCode = E_MA_Success;
			sendApprentice.dwErrorCode = E_MA_Success;

			// 互加为好友
			tagNC_RoleMakeFriend sendMasterMakeFriend;
			tagNC_RoleMakeFriend sendApprenticeMakeFriend;
			sendMasterMakeFriend.dwDestRoleID = pRole->GetID();
			sendApprenticeMakeFriend.dwDestRoleID = pDestRole->GetID();
			g_socialMgr.AddEvent(pDestRole->GetID(), EVT_MakeFriend, sendMasterMakeFriend.dwSize, &sendMasterMakeFriend);
			g_socialMgr.AddEvent(pRole->GetID(), EVT_MakeFriend, sendApprenticeMakeFriend.dwSize, &sendApprenticeMakeFriend);

			// 互加为师徒
			pRole->SetMasterID(pDestRole->GetID());
			pDestRole->AddApprentice(pRole->GetID());

			// 将同门信息同步给徒弟
			pDestRole->GetApprenticeData().ResetIterator();
			DWORD dwMateID = 0;
			while (pDestRole->GetApprenticeData().PeekNext(dwMateID))
			{
				pRole->AddClassMate(dwMateID);
				Role* pMate = g_roleMgr.GetRolePtrByID(dwMateID);
				if (P_VALID(pMate))
				{
					pMate->AddClassMate(pRole->GetID());
				}
			}
			pRole->GetClassMate().nJingWuPoint = pDestRole->GetJingWuPoint();
			pRole->GetClassMate().nQinWuPoint = pDestRole->GetQinWuPoint();

			// 保存到数据库
			//DWORD dwSize = sizeof(DWORD) + sizeof(tagOneMAData2DB) + sizeof(tagApprenticeData);
			//MCREATE_MSG(pSend, dwSize, NDBC_SaveOneMasterApprenticeData);
			tagNDBC_SaveOneMasterApprenticeData send;
			
			send.dwMasterID = pDestRole->GetID();
			send.sOneMAData.eType = EOMAD_Apprentice;
			send.sOneMAData.eMode = EMAMM_Mod;
			*(DWORD*)(send.sOneMAData.byData) = pRole->GetID();
			
			g_dbSession.Send(&send, send.dwSize);

			// 跑马灯公告
			//tstringstream tszMsg;
			//TCHAR tszRoleName[X_SHORT_NAME] = {0};
			//g_roleMgr.GetRoleNameByID(pDestRole->GetID(), tszRoleName);
			//tszMsg << tszRoleName << _T("已将");
			//g_roleMgr.GetRoleNameByID(pRole->GetID(), tszRoleName);
			//tszMsg << tszRoleName << _T("收为徒弟！");

			//DWORD dwLen = sizeof(tagNS_AutoNotice) - sizeof(TCHAR) + 2*(tszMsg.str().length() + 1);
			//MCREATE_MSG(pSendMsg, dwLen, NS_AutoNotice);

			//pSendMsg->nCirInterval = 60;
			//pSendMsg->nType = 2;		// 绿色字体
			//_tcscpy(pSendMsg->szContent, tszMsg.str().c_str());
			//g_roleMgr.SendWorldMsg(pSendMsg, dwLen);
			//MDEL_MSG(pSendMsg);

			const RoleScript* pScript = g_ScriptMgr.GetRoleScript();
			if (P_VALID(pScript))
			{
				pScript->OnHandleMaster(pDestRole, pRole);
			}

			// 设置双方的师徒关系
			pRole->ModifyMasterApprenticeCount(1);
			pDestRole->ModifyMasterApprenticeCount(1);

			// 同步师傅和他的徒弟的
			pDestRole->SyncOnlineNum2Role(ESOR_Online_ClassMate_Num, pDestRole->GetOnlineClassmatesCnt());
			pDestRole->SyncOnlineNum2Apprentices();
		}
	}

	// 发送给徒弟
	SendMessage(&sendApprentice, sendApprentice.dwSize);

	// 发送给双方
	sendMaster.dwRoleID = pRole->GetID();
	pDestRole->GetSession()->SendMessage(&sendMaster, sendMaster.dwSize);

	if (sendMaster.dwErrorCode != E_MA_Success)
	{
		sendMaster.dwErrorCode = E_MA_You_Refuse_ShouTu;
	}
	
	sendMaster.dwRoleID = pDestRole->GetID();
	SendMessage(&sendMaster, sendMaster.dwSize);
	return 0;
}

//--------------------------------------------------------------------------
// 查看我的师门
//--------------------------------------------------------------------------
DWORD PlayerSession::HandleMatesInfo(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_MatesInfo);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// 得到同门数量
	INT nNum = 0;
	for (INT i=0; i != MAX_APPRENTICE_NUM; ++i)
	{
		if (pRole->GetClassMate().dwData[i] != 0)
		{
			++nNum;
		}
	}

	// 获取师傅离线数据
	tagRoleInfo* pMasterInfo = g_roleMgr.GetRoleInfo(pRole->GetMasterID());
	if (!P_VALID(pMasterInfo))
	{
		return GT_INVALID;
	}
	
	DWORD dwSize = sizeof(tagNS_MatesInfo) + (nNum - 1) * sizeof(tagMatesAndApprenticeInfo);
	MCREATE_MSG(pSend, dwSize, NS_MatesInfo);
	
	pSend->nNum = nNum;
	pSend->nJingWuPoint = pRole->GetJingWuPoint();
	pSend->sMasterInfo.dwRoleID = pMasterInfo->dwRoleID;
	pSend->sMasterInfo.nLevel = (INT32)pMasterInfo->byLevel;
	pSend->sMasterInfo.dwJingWuPoint = pRole->GetClassMate().nJingWuPoint;
	pSend->sMasterInfo.dwQinWuPoint = pRole->GetClassMate().nQinWuPoint;

	for (INT i=0; i != nNum; ++i)
	{
		// 获取离线数据
		tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(pRole->GetClassMate().dwData[i]);
		if (!P_VALID(pRoleInfo))
		{
			continue;
		}

		pSend->sMatesInfo[i].dwRoleID = pRoleInfo->dwRoleID;
		pSend->sMatesInfo[i].nLevel = (INT)pRoleInfo->byLevel;
		pSend->sMatesInfo[i].bIsOnLine = pRoleInfo->bOnline;
		pSend->sMatesInfo[i].lastOnLineTime = pRoleInfo->bOnline?GetCurrentDWORDTime(): pRoleInfo->dwTimeLastLogout;
		pSend->sMatesInfo[i].lastQuestTime = tagDWORDTime();			// 暂不发送师徒任务时间
	}

	SendMessage(pSend, pSend->dwSize);

	return 0;
}

//--------------------------------------------------------------------------
// 查看我的徒弟
//--------------------------------------------------------------------------
DWORD PlayerSession::HandleApprenticeInfo(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_ApprenticeInfo);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	INT nNum = pRole->GetApprenticeData().Size();
	if (nNum <= 0)
	{
		return GT_INVALID;
	}

	DWORD dwSize = sizeof(tagNS_ApprenticeInfo) + (nNum - 1) * sizeof(tagMatesAndApprenticeInfo);
	MCREATE_MSG(pSend, dwSize, NS_ApprenticeInfo);

	pSend->nNum = nNum;
	pSend->sMasterInfo.dwRoleID = pRole->GetID();
	pSend->sMasterInfo.nLevel = pRole->GetLevel();
	pSend->sMasterInfo.dwJingWuPoint = pRole->GetJingWuPoint();
	pSend->sMasterInfo.dwQinWuPoint = pRole->GetQinWuPoint();

	(pRole->GetApprenticeData()).ResetIterator();
	DWORD dwApprenticeID = GT_INVALID;
	nNum = 0;
	while (pRole->GetApprenticeData().PeekNext(dwApprenticeID))
	{
		// 获取离线数据
		tagRoleInfo* pRoleInfo = g_roleMgr.GetRoleInfo(dwApprenticeID);
		if (!P_VALID(pRoleInfo))
		{
			continue;
		}

		pSend->sApprenticeInfo[nNum].dwRoleID = pRoleInfo->dwRoleID;
		pSend->sApprenticeInfo[nNum].nLevel = (INT)pRoleInfo->byLevel;
		pSend->sApprenticeInfo[nNum].bIsOnLine = pRoleInfo->bOnline;
		pSend->sApprenticeInfo[nNum].lastOnLineTime = pRoleInfo->bOnline?GetCurrentDWORDTime():pRoleInfo->dwTimeLastLogout;
		pSend->sApprenticeInfo[nNum].lastQuestTime = tagDWORDTime();			// 暂不发送师徒任务时间

		++nNum;
	}

	SendMessage(pSend, pSend->dwSize);

	return 0;
}

//--------------------------------------------------------------------------
// 师傅分配精武点数
//--------------------------------------------------------------------------
DWORD PlayerSession::HandleGiveJingWuPoint(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_GiveJingWuPoint);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	if (pRecv->nPoint <= 0)
	{
		return GT_INVALID;
	}

	tagNS_GiveJingWuPoint send;

	// 获取徒弟
	Role* pApprentice = g_roleMgr.GetRolePtrByID(pRecv->dwRoleID);
	if (!P_VALID(pApprentice))
	{
		// 徒弟不在线
		send.dwErrorCode = E_MA_Not_OnLine;
		SendMessage(&send, send.dwSize);
		return GT_INVALID;
	}

	if (pRole->GetJingWuPoint() < pRecv->nPoint)
	{
		send.dwErrorCode = E_MA_Not_Enough_JingWuPoint;
		SendMessage(&send, send.dwSize);
		return GT_INVALID;
	}

	pRole->SetJingWuPoint(pRole->GetJingWuPoint() - pRecv->nPoint);
	pApprentice->SetJingWuPoint(pApprentice->GetJingWuPoint() + pRecv->nPoint);
	send.dwErrorCode = E_MA_Success;
	SendMessage(&send, send.dwSize);

	// 发送通知给徒弟
	tagNS_GetJingWuPoint sendApprentice;
	sendApprentice.nPoint = pRecv->nPoint;
	pApprentice->GetSession()->SendMessage(&sendApprentice, sendApprentice.dwSize);

	// 更新数据库
	tagNDBC_SaveOneMasterApprenticeData sendDB;
	sendDB.dwMasterID = pRole->GetID();
	sendDB.sOneMAData.eType = EOMAD_JingWu;
	sendDB.sOneMAData.eMode = EMAMM_Mod;		// 增量更新
	*(INT*)(sendDB.sOneMAData.byData) = -pRecv->nPoint;

	g_dbSession.Send(&sendDB, sendDB.dwSize);

	return 0;
}

//--------------------------------------------------------------------------
// 使用精武点数界面客户端请求
//--------------------------------------------------------------------------
DWORD PlayerSession::HandleJingWu2Client(tagNetCmd* pCmd)
{
	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	if (pRole->GetLevel() >= 50)
	{
		return GT_INVALID;
	}

	tagNS_JingWu2Client send;
	const tagMasterApprenticeProto* pProto = g_attRes.GetMasterApprenticeProto(pRole->GetLevel());
	if (!P_VALID(pProto))
	{
		return GT_INVALID;
	}

	send.nNeedJingWuPoint = pProto->nLevelUpNeedPoint;
	send.nNeedYuanBao = pProto->nLevelUpNeedYuanBao;
	send.nJingWuPoint = pRole->GetJingWuPoint();
	send.nYuanBao = pRole->GetCurMgr().GetBagYuanBao();
	SendMessage(&send, send.dwSize);

	return 0;
}

//--------------------------------------------------------------------------
// 徒弟使用精武点数
//--------------------------------------------------------------------------
DWORD PlayerSession::HandleUseJingWuPoint(tagNetCmd* pCmd)
{
	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	const tagMasterApprenticeProto* pProto = g_attRes.GetMasterApprenticeProto(pRole->GetLevel());
	if (!P_VALID(pProto))
	{
		return GT_INVALID;
	}

	tagNS_UseJingWuPoint send;

	// 精武点数不够
	if (pRole->GetJingWuPoint() < pProto->nLevelUpNeedPoint)
	{
		send.dwErrorCode = E_MA_Not_Enough_JingWuPoint;
	}

	// 宝鉴不够
	//else if (pRole->GetCurMgr().GetBagYuanBao() < pProto->nLevelUpNeedYuanBao)
	else if (pRole->GetItemMgr().GetBagSameItemCount(APPRENTICE_LEVEL_UP_ITEM) < pProto->nLevelUpNeedYuanBao)
	{
		send.dwErrorCode = E_MA_Not_Enough_BaoJian;
	}

	else
	{
		pRole->SetJingWuPoint(pRole->GetJingWuPoint() - pProto->nLevelUpNeedPoint);
		pRole->GetItemMgr().RemoveFromRole(APPRENTICE_LEVEL_UP_ITEM, pProto->nLevelUpNeedYuanBao, ELCLD_Apprentice_UseBaoJian);
		
		const tagLevelUpEffect* pEffect = g_attRes.GetLevelUpEffect(pRole->GetLevel());
		if (!P_VALID(pEffect))
		{
			return GT_INVALID;
		}

		// 调用脚本
		g_ScriptMgr.GetRoleScript()->OnUseJingWuPoint(pRole, pRole->GetJingWuPoint());

		pRole->ExpChange(pEffect->nExpLevelUp - pRole->GetCurLevelExp());

		// 使用的精武点数等量转换为师父的勤武点数
		Role* pMaster = g_roleMgr.GetRolePtrByID(pRole->GetMasterID());
		if (P_VALID(pMaster))		// 如果师傅在线
		{
			pMaster->SetQinWuPoint(pMaster->GetQinWuPoint() + pProto->nLevelUpNeedPoint);
		}

		// 更新数据库
		tagNDBC_SaveOneMasterApprenticeData sendDB;
		sendDB.dwMasterID = pRole->GetMasterID();
		sendDB.sOneMAData.eType = EOMAD_QinWu;
		sendDB.sOneMAData.eMode = EMAMM_Mod;		// 增量更新
		*(INT*)(sendDB.sOneMAData.byData) = pProto->nLevelUpNeedPoint;

		g_dbSession.Send(&sendDB, sendDB.dwSize);

		send.dwErrorCode = E_MA_Success;
		send.nCurJingWuPoint = pRole->GetJingWuPoint();

		// 更新所有同门其师尊的勤武点数
		for (INT i=0; i!=MAX_APPRENTICE_NUM; ++i)
		{
			DWORD dwMateID = pRole->GetClassMate().dwData[i];
			Role* pMate = g_roleMgr.GetRolePtrByID(dwMateID);
			if (P_VALID(pMate))
			{
				pMate->GetClassMate().nQinWuPoint += pProto->nLevelUpNeedPoint;
			}
		}
	}

	SendMessage(&send, send.dwSize);

	return 0;
}

//--------------------------------------------------------------------------
// 徒弟脱离师门
//--------------------------------------------------------------------------
DWORD PlayerSession::HandleBreakOutMaster(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_BreakOutMaster);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_BreakOutMaster send;

	// 获取师傅离线数据结构
	tagRoleInfo *pInfo = g_roleMgr.GetRoleInfo(pRecv->dwRoleID);
	if (!P_VALID(pInfo))
	{
		return GT_INVALID;
	}

	// 自己已出师
	if (pRole->GetLevel() >= GOOD_STU_CAN_LEAVE_LEVEL)
	{
		send.dwErrorCode = E_MA_Success;
	}

	// 不满足师傅3日未上线
	else if (pInfo->bOnline || CalcTimeDiff(GetCurrentDWORDTime(), pInfo->dwTimeLastLogout) < MASTER_BREAKOUT_DAY_LIMIT * 24 * 3600)
	{
		send.dwErrorCode = E_MA_OnLine_Day_Limit;
	}

	else
	{
		send.dwErrorCode = E_MA_Success;
		
	}

	SendMessage(&send, send.dwSize);
	
	return 0;
}

//--------------------------------------------------------------------------
// 强行脱离师门
//--------------------------------------------------------------------------
DWORD PlayerSession::HandleForceBreakOutMaster(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_ForceBreakOutMaster);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_ForceBreakOutMaster send;

	// 强行脱离师门申请
	if (g_forceBreakOutApprenticeMgr.FindApprentice(pRole->GetID(), send.dwRemainTime) == FALSE)
	{
		g_forceBreakOutApprenticeMgr.InsertApprentice2List(pRole->GetID(), pRecv->dwRoleID);
		send.dwErrorCode = E_Success;
	}
	else
	{
		send.dwErrorCode = E_MA_Already_Force_BreakOut;
	}
	pRole->SendMessage(&send, send.dwSize);

	return 0;
}

//--------------------------------------------------------------------------
// 再次确认
//--------------------------------------------------------------------------
DWORD PlayerSession::HandleBreakOutMasterConfirm(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_BreakOutMasterConfirm);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// 获取师傅离线数据结构
	tagRoleInfo *pInfo = g_roleMgr.GetRoleInfo(pRecv->dwRoleID);
	if (!P_VALID(pInfo))
	{
		return GT_INVALID;
	}

	// 不满足删除条件
	if (pRole->GetLevel() < GOOD_STU_CAN_LEAVE_LEVEL && (pInfo->bOnline ||  CalcTimeDiff(GetCurrentDWORDTime(), pInfo->dwTimeLastLogout) < MASTER_BREAKOUT_DAY_LIMIT * 24 * 3600))
	{
		return GT_INVALID;
	}

	pRole->SetJingWuPoint(0);			// 清空精武点数
	pRole->SetMasterID(GT_INVALID);		// 置空师傅ID

	Role* pMaster = g_roleMgr.GetRolePtrByID(pRecv->dwRoleID);

	// 如果师傅在线
	if (P_VALID(pMaster))
	{
		pMaster->DeleteApprentice(pRole->GetID());

		pMaster->ModifyMasterApprenticeCount(-1);
		// 给师傅发送脱离师门关系消息
		tagNS_BreakOutMasterConfirm send;
		send.dwRoleID = pRole->GetID();
		pMaster->GetSession()->SendMessage(&send, send.dwSize);

		// 通知师傅在线师门人数可能发生变化
		pMaster->SyncOnlineNum2Role(ESOR_Online_ClassMate_Num, pMaster->GetOnlineClassmatesCnt());
	}
	else
	{
		g_roleMgr.ModifyMasterApprenticeCount(pRecv->dwRoleID, -1);
		g_roleMgr.SaveRoleInfoToDB(pRecv->dwRoleID);
	}

	tagNS_BreakOutMasterConfirm send;
	send.dwRoleID = pRecv->dwRoleID;
	SendMessage(&send, send.dwSize);

	// 更新数据库
	tagNDBC_DeleteAnApprentice sendDB;
	sendDB.dwMasterID = pRecv->dwRoleID;
	sendDB.dwApprenticeID = pRole->GetID();
	g_dbSession.Send(&sendDB, sendDB.dwSize);

	// 徒弟更新同门信息
	DWORD dwMateID = GT_INVALID;
	for (INT i=0; i!=MAX_APPRENTICE_NUM; ++i)
	{
		dwMateID = pRole->GetClassMate().dwData[i];
		Role* pMate = g_roleMgr.GetRolePtrByID(dwMateID);
		if (P_VALID(pMate))
		{
			pMate->DeleteClassMate(pRole->GetID());
// 			pMate->SyncOnlineNum2Role(ESOR_Online_ClassMate_Num, pMate->GetOnlineClassmatesCnt());
		}
	}

 	// 跟被删除的徒弟同步同门人数变化
 	pRole->SyncOnlineNum2Classmates();

	// 删除所有同门
	pRole->DeleteAllClassMate();

	// 先通知自己
	pRole->SyncOnlineNum2Role(ESOR_Online_ClassMate_Num, pRole->GetOnlineClassmatesCnt());

	// 减少一个师徒关系
	pRole->ModifyMasterApprenticeCount(-1);
	
	return 0;
}

//--------------------------------------------------------------------------
// 师傅逐出徒弟
//--------------------------------------------------------------------------
DWORD PlayerSession::HandleBreakOutApprentice(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_BreakOutApprentice);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagNS_BreakOutApprentice send;

	// 获取徒弟离线数据结构
	tagRoleInfo *pInfo = g_roleMgr.GetRoleInfo(pRecv->dwRoleID);
	if (!P_VALID(pInfo))
	{
		return GT_INVALID;
	}

	// 徒弟已出师
	if (pInfo->byLevel >= GOOD_STU_CAN_LEAVE_LEVEL)
	{
		send.dwErrorCode = E_MA_Success;
	}

	// 不满足徒弟3日未上线
	else if (pInfo->bOnline || CalcTimeDiff(GetCurrentDWORDTime(), pInfo->dwTimeLastLogout) < MASTER_BREAKOUT_DAY_LIMIT * 24 * 3600)
	{
		send.dwErrorCode = E_MA_OnLine_Day_Limit;
	}

	else
	{
		send.dwErrorCode = E_MA_Success;
		
	}

	SendMessage(&send, send.dwSize);

	return 0;
}

//--------------------------------------------------------------------------
// 再次确认
//--------------------------------------------------------------------------
DWORD PlayerSession::HandleBreakOutApprenticeConfirm(tagNetCmd* pCmd)
{
	MGET_MSG(pRecv, pCmd, NC_BreakOutApprenticeConfirm);

	Role* pRole = GetRole();
	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	tagRoleInfo *pInfo = g_roleMgr.GetRoleInfo(pRecv->dwRoleID);
	if (!P_VALID(pInfo))
	{
		return GT_INVALID;
	}

	// 不满足删除条件
	if (pInfo->byLevel < GOOD_STU_CAN_LEAVE_LEVEL && (pInfo->bOnline ||  CalcTimeDiff(GetCurrentDWORDTime(), pInfo->dwTimeLastLogout) < MASTER_BREAKOUT_DAY_LIMIT * 24 * 3600))
	{
		return GT_INVALID;
	}
	

	Role* pApprentice = g_roleMgr.GetRolePtrByID(pRecv->dwRoleID);

	// 如果徒弟在线
	if (P_VALID(pApprentice))
	{
		pApprentice->SetMasterID(GT_INVALID);
		pApprentice->SetJingWuPoint(0);			// 清空精武点数
		pApprentice->ModifyMasterApprenticeCount(-1);		

		// 删除所有同门
		pApprentice->DeleteAllClassMate();

		// 给徒弟发送脱离师门关系消息
		tagNS_BreakOutApprenticeConfirm send;
		send.dwRoleID = pRole->GetID();
		pApprentice->GetSession()->SendMessage(&send, send.dwSize);

		// 通知被逐出师门的徒弟师门在线人数发生变化
		pApprentice->SyncOnlineNum2Role(ESOR_Online_ClassMate_Num, pApprentice->GetOnlineClassmatesCnt());
	}
	else
	{
		g_roleMgr.ModifyMasterApprenticeCount(pRecv->dwRoleID, -1);
		g_roleMgr.SaveRoleInfoToDB(pRecv->dwRoleID);
	}

	tagNS_BreakOutApprenticeConfirm send;
	send.dwRoleID = pRecv->dwRoleID;
	SendMessage(&send, send.dwSize);

	// 更新数据库
	tagNDBC_DeleteAnApprentice sendDB;
	sendDB.dwMasterID = pRole->GetID();
	sendDB.dwApprenticeID = pRecv->dwRoleID;
	g_dbSession.Send(&sendDB, sendDB.dwSize);

	// 更新徒弟信息
	pRole->DeleteApprentice(pRecv->dwRoleID);

	// 徒弟更新同门信息
	pRole->GetApprenticeData().ResetIterator();
	DWORD dwApprenticeID = GT_INVALID;
	while (pRole->GetApprenticeData().PeekNext(dwApprenticeID))
	{
		Role* pApprentice = g_roleMgr.GetRolePtrByID(dwApprenticeID);
		if (P_VALID(pApprentice))
		{
			pApprentice->DeleteClassMate(pRecv->dwRoleID);

			// 通知徒弟在线师门人数发生变化
			pApprentice->SyncOnlineNum2Role(ESOR_Online_ClassMate_Num, pApprentice->GetOnlineClassmatesCnt());
		}
	}

	// 通知师傅在线师门人数发生变化
	pRole->SyncOnlineNum2Role(ESOR_Online_ClassMate_Num, pRole->GetOnlineClassmatesCnt());

// 	// 通知徒弟在线师门人数发生变化
// 	pRole->SyncOnlineNum2Apprentices();


	// 减少一个师徒关系
	pRole->ModifyMasterApprenticeCount(-1);

	return 0;
}