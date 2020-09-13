//-----------------------------------------------------------------------------
//!\file talent_handler.cpp
//!\author Aslan
//!
//!\date 2008-07-28
//! last 2008-07-28
//!
//!\brief 天资消息处理类
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"

#include "../WorldDefine/RoleDefine.h"
#include "../WorldDefine/talent_define.h"
#include "../WorldDefine/msg_talent.h"
#include "../ServerDefine/log_cmdid_define.h"

#include "player_session.h"
#include "unit.h"
#include "role.h"


//-----------------------------------------------------------------------------
// 学习技能
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleLearnSkill(tagNetCmd* pCmd)
{
	tagNC_LearnSkill* pRecv = (tagNC_LearnSkill*)pCmd;
	if( !P_VALID(m_pRole) ) return GT_INVALID;

	DWORD dwErrorCode = m_pRole->LearnSkill(pRecv->dwSkillID, pRecv->dwNPCID, pRecv->n64ItemID);

	// 发送返回消息
	tagNS_LearnSkill send;
	send.dwSkillID = pRecv->dwSkillID;
	send.dwErrorCode = dwErrorCode;
	SendMessage(&send, send.dwSize);

	return 0;
}

//-----------------------------------------------------------------------------
// 技能升级
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleLevelUpSkill(tagNetCmd* pCmd)
{
	tagNC_LevelUpSkill* pRecv = (tagNC_LevelUpSkill*)pCmd;
	if( !P_VALID(m_pRole) ) return GT_INVALID;

	DWORD dwErrorCode = m_pRole->LevelUpSkill(pRecv->dwSkillID, pRecv->dwNPCID, pRecv->n64ItemID);

	// 发送返回消息
	tagNS_LevelUpSkill send;
	send.dwSkillID = pRecv->dwSkillID;
	send.dwErrorCode = dwErrorCode;
	SendMessage(&send, send.dwSize);

	return 0;
}

//-----------------------------------------------------------------------------
// 遗忘技能
//-----------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleForgetSkill(tagNetCmd* pCmd)
{
	tagNC_ForgetSkill* pRecv = (tagNC_ForgetSkill*)pCmd;
	if( !P_VALID(m_pRole) ) return GT_INVALID;

	DWORD dwErrorCode = m_pRole->ForgetSkill(pRecv->dwSkillID, pRecv->dwNPCID);

	// 发送返回消息
	tagNS_ForgetSkill send;
	send.dwSkillID = pRecv->dwSkillID;
	send.dwErrorCode = dwErrorCode;
	SendMessage(&send, send.dwSize);

	return 0;
}

//------------------------------------------------------------------------------
// 洗点
//------------------------------------------------------------------------------
DWORD PlayerSession::HandleRoleClearTalent(tagNetCmd* pCmd)
{
	tagNC_ClearTalent* pRecv = (tagNC_ClearTalent*)pCmd;
	if( !P_VALID(m_pRole) ) return GT_INVALID;

	DWORD dwErrorCode = m_pRole->ClearTalent(pRecv->n64ItemID);

	// 发送返回消息
	tagNS_ClearTalent send;
	send.eType = pRecv->eType;
	send.dwErrorCode = dwErrorCode;
	SendMessage(&send, send.dwSize);

	return 0;
}

//------------------------------------------------------------------------------
// 修炼天资
//------------------------------------------------------------------------------

DWORD  PlayerSession::HandlePracticeTalent(tagNetCmd* pCmd)
{
     
    tagNC_RolePracticeTalentPoint* pRecv = (tagNC_RolePracticeTalentPoint*)pCmd;
    Role* pRole = GetRole();
	if ( !P_VALID(pRole) )
	{
		return GT_INVALID;
	}
    ERolePracticeTalentErrorCode dwErrorCode = ERPT_Success;
	// 背包中是否有足够的金钱
	CurrencyMgr &CurMgr = pRole->GetCurMgr();
	
	INT nCurrentTotalTalentPoint = 0;
	//获取当前可投天资点天资 
	INT nCurrentTalentPoint = pRole->GetAttValue(ERA_TalentPoint);
    nCurrentTotalTalentPoint = nCurrentTalentPoint + pRole->GetTotalTalentPoint(); //当前总天资数 = 当前可投天资数 + 当前已投天资数 
	
    INT nLevel = pRole->GetLevel();
	INT nTalentFromLevelNum = 0;
	//计算通过升级得到的天资点数
	for(INT nL = 0 ;nL < nLevel ; nL++)
	{
		const tagLevelUpEffect*  pLevelEff = g_attRes.GetLevelUpEffect(nL+1); //索引和级别相同
		if ( !P_VALID(pLevelEff) )
		{
			return GT_INVALID;
		}
		nTalentFromLevelNum += pLevelEff->n16TalentAvail;

	}
    //计算通过购买的天资数 
	INT nTalentFromBuying = nCurrentTotalTalentPoint - nTalentFromLevelNum ;
    
    // 保存客户端要购买的天资点数
	INT nWantTalentNum = pRecv->nTalentPointAdd;
    if ( 0 >= nWantTalentNum || nTalentFromBuying < 0)
	{
		return 0 ; 
	}

	if ( nCurrentTotalTalentPoint + nWantTalentNum - pRole->GetTalentAvail_limit(nLevel) > 0 )
		return ERTEC_OverMaxTalentAvail_Limit;
   
	INT64 n64Silver = 0;

	for (INT n = 0 ; n < nWantTalentNum ; n++)
	{

		const tagTalentAvailCost* pTal = g_attRes.GetTalentAvailCost(n+1+nTalentFromBuying);//有效访问索引从1开始
		if ( !P_VALID(pTal) )
		{
			return GT_INVALID;
		}
		n64Silver +=static_cast<INT64>(pTal->dwCostSilver) ;
	}
    INT nTalentTotalBuying  = pRole->GetTalentAvail_limit(nLevel);
    if ( nWantTalentNum > nTalentTotalBuying - nTalentFromLevelNum)
	{
		dwErrorCode  = ERPT_OverStepMaxTalent ;
	}
	else
	{
		if( n64Silver > CurMgr.GetBagSilver())
		{
			dwErrorCode = ERPT_NoEnoughMonney ;
		}
		else
		{
             CurMgr.DecBagSilver(n64Silver,ELCID_Buy_Talent);
             pRole->ModAttValue(ERA_TalentPoint,nWantTalentNum);
			 dwErrorCode = ERPT_Success ; 
		}
	}
    tagNS_RolePracticeTalentPoint send;
	send.dwErrorCode = dwErrorCode;

	SendMessage(&send,send.dwSize);


	return 0;
}

DWORD PlayerSession::HandlePracticeSoaringSkill(tagNetCmd*pCmd)
{
	tagNC_PracticeSoaringSkill* pRecv = (tagNC_PracticeSoaringSkill*)pCmd;
	Role* pRole = GetRole();
	if ( !P_VALID(pRole) )
	{
		return GT_INVALID;
	}
	tagNS_PracticeSoaringSkill msg;
	msg.dwErrCode = pRole->LearnSoaringSkill(pRecv->dwSkillID);
	msg.n16CurPrcNum = pRole->GetPracticeSoaringSkillTimes();
	SendMessage(&msg,msg.dwSize);
	return E_Success;
}

