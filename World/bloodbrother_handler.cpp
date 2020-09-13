#include "StdAfx.h"
#include "player_session.h"
#include "role.h"
#include "..\WorldDefine\msg_bloodbrother.h"
#include "..\WorldDefine\brother_define.h"
#include "../ServerDefine/log_cmdid_define.h"

DWORD PlayerSession::HandleTryMakeBrother(tagNetCmd * pCmd)
{
	// 接收网络消息
	MGET_MSG(pRecv, pCmd, NC_TryMakeBrother);
	if (!P_VALID(pRecv))
		return GT_INVALID;

	// 获取角色
	Role* pRole = GetRole();
	if(!P_VALID(pRole))
		return GT_INVALID;

	DWORD dwErrorCode = 0;
	tagNS_TryMakeBrother send;


	Team* pTemp = (Team*)g_groupMgr.GetTeamPtr(pRole->GetTeamID());
	if ( !P_VALID(pTemp))
	{
		dwErrorCode = E_bloodbrother_TeamNotHave2;
	}
	else
	{
		if(pTemp->GetMemNum() == 2)
		{
			Role* pRole1 = pTemp->GetMemPtr(0);
			Role* pRole2 = pTemp->GetMemPtr(1);
			if(!P_VALID(pRole1) || !P_VALID(pRole2))
				return GT_INVALID;

			// 该好友已存在于好友列表
			tagFriend *pFriend1 = pRole1->GetFriendPtr(pRole2->GetID());
			tagFriend *pFriend2 = pRole2->GetFriendPtr(pRole1->GetID());
			// 相互是朋友
			if(P_VALID(pFriend1) && P_VALID(pFriend2))
			{
				// 等级要求
				if ( (pRole1->GetLevel() >= BloodBrother_DistinguishLevel && pRole2->GetLevel() < BloodBrother_DistinguishLevel )
					|| (pRole1->GetLevel() < BloodBrother_DistinguishLevel && pRole2->GetLevel() >= BloodBrother_DistinguishLevel) )
				{
					Role* pBrother = pRole1->GetLevel() >=BloodBrother_DistinguishLevel ? pRole2 :pRole1;
					Role* pTeather = pRole1->GetLevel() >=BloodBrother_DistinguishLevel ? pRole1 :pRole2;
					// 是否可以结拜
					if(pBrother->GetBrotherMgr().CanMakeBrother())
					{
						TList<tagItem*> ItemList;
						tagItem* pItem;
						// 道具查看
						if (SearchPack(pBrother->GetItemMgr().GetBag(), ItemList, EISF_BloodBrother, tagFindItemSpecFunc()) == 0)
						{
							dwErrorCode = E_bloodbrother_NotHaveItem;
						}
						else
						{
							bool bFind = false;
							ItemList.ResetIterator();
							while( ItemList.PeekNext(pItem) )
							{
								if (pItem->pProtoType->nSpecFuncVal1 == pRecv->dwDays )
								{
									bFind = true;
								}
							}
							if(bFind)
							{
								tagDWORDTime time = GetCurrentDWORDTime();
								tagDWORDTime timeEnd = IncreaseTime(time,pItem->pProtoType->nSpecFuncVal1*24*60*60);
								// 扣物品要扣小号身上的
								if (E_Success == pBrother->GetItemMgr().DelFromBag(pItem->n64Serial, ELCID_BloodBrother,1))
								{
									// 设置ROLE相关字段
									pBrother->MakeBorther(pTeather->GetID());
									pTeather->MakeBorther(0,true);

									pBrother->GetBrotherMgr().SetEndTime(timeEnd);
									pBrother->SyncBrotherEndTime();
									// 添加buff
									pBrother->TryAddBuffToMyself(BloodBrotherBuffBrother);
									pTeather->TryAddBuffToMyself(BloodBrotherBuffTeather);

									if( pBrother->GetID() == pRole->GetID())
									{
										pTeather->MakeBrotherSuccess(pBrother->GetID() );
										send.dwRoleID = pTeather->GetID();
									}
									else
									{
										pBrother->MakeBrotherSuccess(pTeather->GetID() );
										send.dwRoleID = pBrother->GetID();
									}
								}
								else
								{
									IMSG(_T("LevelUpWardrobeLayer::DelFromBagFailed!!!\r\n"));
									return GT_INVALID;
								}
							}
							else
								dwErrorCode = E_bloodbrother_NotHaveItem;
						}
					}
					else
						dwErrorCode = E_bloodbrother_AlreadyHaveTeather;
				}
				else
					dwErrorCode = E_bloodbrother_TeamLevelNotFeet;
			}
			else
				dwErrorCode = E_bloodbrother_NotFriend;
		}
		else
			dwErrorCode = E_bloodbrother_TeamNotHave2;
		
	}

	send.dwErrorCode = dwErrorCode;
	pRole->SendMessage(&send, send.dwSize);
	return 0;
}
