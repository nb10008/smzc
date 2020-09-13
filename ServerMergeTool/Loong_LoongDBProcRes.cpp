#include "StdAfx.h"

#include "wx/wx.h"
#include "servermergetoolapp.h"
#include "Loong_MergeLogicCtrl.h"
#include "Loong_LoongDBCtrl.h"
#include "../WorldDefine/time.h"


VOID CLoongLoongDBCtrl::ProcResQueryAccountCommon(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pAccountCommon, pData, std::vector <tagTWrap_Loong_AccountCommon*>);

	do{
		tagTWrap_Loong_AccountCommon *p = new tagTWrap_Loong_AccountCommon;

		p->dwAccountID				= (*pRes)[0].GetDword();
		memcpy(p->szAccountName,(*pRes)[1].GetString(),(*pRes)[1].GetLen()+1);
		p->stOthers.sSafeCode.dwSafeCodeCrc	= (*pRes)[2].GetDword();
		p->stOthers.sSafeCode.dwTimeReset	= (*pRes)[3].GetDword();
		p->stOthers.dwBagPsdCrc				= (*pRes)[4].GetDword();
		p->stOthers.nBaiBaoYuanBao			= (*pRes)[5].GetInt();
		p->stOthers.n16WareSize				= (*pRes)[6].GetInt();
		p->stOthers.n64WareSilver			= (*pRes)[7].GetLong();
		(*pRes)[8].GetBlob(&p->stOthers.stPersonalSet,min((*pRes)[8].GetLen(),sizeof(tagDWPlayerPersonalSet)));

		//1.3.2新增
		//"LastLoginTime,	LastLogoutTime,		LastReceiveDailyOfflineRewardTime,	LastReceiveRegressionTime"	);
		p->timeLastLogin						= (*pRes)[9].GetDword();
		p->timeLastLogout						= (*pRes)[10].GetDword();
		p->timeLastReceiveDailyOfflineReward	= (*pRes)[11].GetDword();
		p->timeLastReceiveRegression			= (*pRes)[12].GetDword();

		//1.4.2
		p->stOthers.eSpec						= (EAccountSpec)(*pRes)[13].GetDword();

		pAccountCommon->push_back(p);

	}while(pRes->NextRow());
}
VOID CLoongLoongDBCtrl::ProcResQueryBlackList(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pBlackList, pData, std::vector <tagTWrap_Loong_BlackList*>);
	do{
		tagTWrap_Loong_BlackList *p = new tagTWrap_Loong_BlackList;

		p->dwRoleID		= (*pRes)[0].GetDword();
		p->dwBlackID	= (*pRes)[1].GetDword();

		pBlackList->push_back(p);
	}while(pRes->NextRow());
}
VOID CLoongLoongDBCtrl::ProcResQueryBuff(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pBuff, pData, std::vector <tagTWrap_Loong_Buff*>);
	do{
		tagTWrap_Loong_Buff *p = new tagTWrap_Loong_Buff;

		p->dwRoleID					=	(*pRes)[0].GetDword();
		p->stOthers.dwSrcUnitID		=	(*pRes)[1].GetDword();
		p->stOthers.dwSrcSkillID	=	(*pRes)[2].GetDword();
		p->stOthers.dwItemTypeID	=	(*pRes)[3].GetDword();
		p->stOthers.n64Serial		=	(*pRes)[4].GetLong();

		p->stOthers.dwBuffID		= (*pRes)[5].GetDword();
		p->stOthers.nPersistTick	= (*pRes)[6].GetInt();
		p->stOthers.n8Level			= (*pRes)[7].GetInt();
		p->stOthers.n8CurLapTimes	= (*pRes)[8].GetInt();


		p->stOthers.n8ModifierNum	= (INT8)((*pRes)[9].GetLen()/sizeof(DWORD));

		if(p->stOthers.n8ModifierNum > 0)
		{
			p->pcBlob	= new CHAR[(*pRes)[9].GetLen()];

			(*pRes)[9].GetBlob(p->pcBlob, (*pRes)[9].GetLen());
		}

		//推进去
		pBuff->push_back(p);

	}while(pRes->NextRow());
}
VOID CLoongLoongDBCtrl::ProcResQueryClanData(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pClanData, pData, std::vector <tagTWrap_Loong_ClanData*>);
	do{
		tagTWrap_Loong_ClanData *p = new tagTWrap_Loong_ClanData;

		for(INT nClanType = ECLT_BEGIN; nClanType < ECLT_END; ++nClanType)
		{
			p->stOthers.m_nReputation[nClanType]		= (*pRes)[nClanType * 3].GetInt();
			p->stOthers.m_nContribution[nClanType]	= (*pRes)[nClanType * 3 + 1].GetInt();
			p->stOthers.m_n8ActCount[nClanType]		= (*pRes)[nClanType * 3 + 2].GetByte();

		}

		p->stOthers.m_u16FameMask	= (*pRes)[ECLT_NUM * 3].GetShort();
		p->stOthers.m_dwtLastResetTime= (*pRes)[ECLT_NUM * 3 + 1].GetDword();

		p->dwRoleID = (*pRes)[ECLT_NUM * 3 + 2].GetDword();

		//1.3.1新增		"TotalFame,		FamePm,		RemoveFlag"	);
		p->stOthers.m_dwTotalRep = (*pRes)[ECLT_NUM * 3 + 3].GetDword();
		p->stOthers.m_nRepPm = (*pRes)[ECLT_NUM * 3 + 4].GetInt();
		p->bRemoveFlag = (*pRes)[ECLT_NUM * 3 + 5].GetBool();


		//推进去
		pClanData->push_back(p);

	}while(pRes->NextRow());
}
//enemy
VOID CLoongLoongDBCtrl::ProcResQueryEnemy(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pEnemy, pData, std::vector <tagTWrap_Loong_Enemy*>);
	do{
		tagTWrap_Loong_Enemy *p = new tagTWrap_Loong_Enemy;

		p->dwRoleID		= (*pRes)[0].GetDword();
		p->dwEnemyID	= (*pRes)[1].GetDword();

		pEnemy->push_back(p);

	}while(pRes->NextRow());
}

//equip
VOID CLoongLoongDBCtrl::ProcResQueryEquip(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pEquipList, pData, std::vector <tagTWrap_Loong_Equip*>);

	do{
		tagTWrap_Loong_Equip *pEquip = new tagTWrap_Loong_Equip;

		pEquip->n64SerialNum								= (*pRes)[0].GetLong();
		// 装备属性
		pEquip->stOthers.equipSpec.byQuality 				= (*pRes)[1].GetByte();
		pEquip->stOthers.equipSpec.nPotValUsed 				= (*pRes)[2].GetInt();
		pEquip->stOthers.equipSpec.byMinUseLevel 			= (*pRes)[3].GetByte();
		pEquip->stOthers.equipSpec.byMaxUseLevel 			= (*pRes)[4].GetByte();
		pEquip->stOthers.equipSpec.n16WuHun 				= (*pRes)[5].GetShort();
		pEquip->stOthers.equipSpec.n16MinDmg 				= (*pRes)[6].GetShort();
		pEquip->stOthers.equipSpec.n16MaxDmg 				= (*pRes)[7].GetShort();
		pEquip->stOthers.equipSpec.n16Armor 				= (*pRes)[8].GetShort();
		pEquip->stOthers.equipSpec.nPotVal 					= (*pRes)[9].GetInt();

		pEquip->stOthers.equipSpec.byPosyTimes 				= (*pRes)[11].GetByte();

		pEquip->stOthers.equipSpec.byEngraveTimes 			= (*pRes)[13].GetByte();

		pEquip->stOthers.equipSpec.byHoleNum 				= (*pRes)[15].GetByte();

		pEquip->stOthers.equipSpec.byBrandLevel 			= (*pRes)[17].GetByte();

		pEquip->stOthers.equipSpec.byLongfuLevel			= (*pRes)[19].GetByte();

		pEquip->stOthers.equipSpec.bCanCut 					= (*pRes)[21].GetBool();
		pEquip->stOthers.equipSpec.dwLongHunInnerID			= (*pRes)[22].GetDword();
		pEquip->stOthers.equipSpec.dwLongHunOuterID			= (*pRes)[23].GetDword();
		pEquip->stOthers.equipSpec.bySpecAtt				= (*pRes)[24].GetByte();
		// 时装属性
		pEquip->stOthers.equipSpec.n16Appearance			= (*pRes)[25].GetShort();
		pEquip->stOthers.equipSpec.byRein					= (*pRes)[26].GetByte();
		pEquip->stOthers.equipSpec.bySavvy					= (*pRes)[27].GetByte();
		pEquip->stOthers.equipSpec.byFortune				= (*pRes)[28].GetByte();
		pEquip->stOthers.equipSpec.n8ColorID				= (INT8)(*pRes)[29].GetInt();

		pEquip->stOthers.equipSpec.n16QltyModPct			= (*pRes)[30].GetShort();
		pEquip->stOthers.equipSpec.n16QltyModPctEx			= (*pRes)[31].GetShort();

		pEquip->stOthers.equipSpec.n16AttALimMod			= (*pRes)[32].GetShort();
		pEquip->stOthers.equipSpec.n16AttALimModPct			= (*pRes)[33].GetShort();
		pEquip->stOthers.equipSpec.byFlareVal				= (*pRes)[34].GetByte();
		pEquip->stOthers.equipSpec.n16PotValModPct			= (*pRes)[35].GetShort();

		(*pRes)[10].GetBlob(pEquip->stOthers.equipSpec.nRoleAttEffect, min((*pRes)[10].GetLen(), sizeof(pEquip->stOthers.equipSpec.nRoleAttEffect)));
		(*pRes)[12].GetBlob(pEquip->stOthers.equipSpec.PosyEffect, min((*pRes)[12].GetLen(), sizeof(pEquip->stOthers.equipSpec.PosyEffect)));
		(*pRes)[14].GetBlob(pEquip->stOthers.equipSpec.nEngraveAtt, min((*pRes)[14].GetLen(), sizeof(pEquip->stOthers.equipSpec.nEngraveAtt)));
		(*pRes)[16].GetBlob(pEquip->stOthers.equipSpec.dwHoleGemID, min((*pRes)[16].GetLen(), sizeof(pEquip->stOthers.equipSpec.dwHoleGemID)));
		(*pRes)[18].GetBlob(pEquip->stOthers.equipSpec.nBrandAtt, min((*pRes)[18].GetLen(), sizeof(pEquip->stOthers.equipSpec.nBrandAtt)));
		(*pRes)[20].GetBlob(pEquip->stOthers.equipSpec.byLongfuAtt, min((*pRes)[20].GetLen(), sizeof(pEquip->stOthers.equipSpec.byLongfuAtt)));

		//1.3.0新增
		pEquip->stOthers.equipSpec.nPotIncTimes				= (*pRes)[36].GetInt();
		pEquip->stOthers.equipSpec.dwPurpleQltyIdfPct		= (*pRes)[37].GetDword();

		//1.3.2 新增
		//"Signed,"		"Signature,"		"SignRoleID");
		pEquip->stOthers.equipSpec.bySigned					= (*pRes)[38].GetInt();
		(*pRes)[39].GetTCHAR(pEquip->stOthers.equipSpec.szSignature, 10);
		pEquip->stOthers.equipSpec.dwSignatruePlayerID		= (*pRes)[40].GetDword();

		//2.2.0新增
		pEquip->stOthers.equipSpec.bSoaringFlag				= (*pRes)[41].GetBool();
		pEquip->stOthers.equipSpec.nCastingTimes			= (*pRes)[42].GetInt();

		//FPlan1.0.0
		pEquip->stOthers.equipSpec.nRoleL2AttCount			= (*pRes)[43].GetInt();
		pEquip->stOthers.equipSpec.byConsolidateLevel		= (*pRes)[44].GetInt();
		(*pRes)[45].GetBlob( pEquip->stOthers.equipSpec.ConsolidateEffect,
			min((*pRes)[45].GetLen(), sizeof(pEquip->stOthers.equipSpec.ConsolidateEffect)) );

		pEquip->stOthers.equipSpec.byLastGodStrengthenLevel	= (*pRes)[46].GetInt();
		pEquip->stOthers.equipSpec.byGodStrengthenLevel		= (*pRes)[47].GetInt();
		pEquip->stOthers.equipSpec.dwCurGodStrengthenExp	= (*pRes)[48].GetDword();
		pEquip->stOthers.equipSpec.dwGodStrengthenTime		= (*pRes)[49].GetDword();

		// F-Plan 1.5.0
		pEquip->stOthers.equipSpec.dwGrowTypeID	= (*pRes)[50].GetDword();
		pEquip->stOthers.equipSpec.dwGrowValue	= (*pRes)[51].GetDword();
		pEquip->stOthers.equipSpec.byGrowLevel	= (BYTE)(*pRes)[52].GetInt();
		pEquip->stOthers.equipSpec.nTotalGrowTimes	= (*pRes)[53].GetInt();
		pEquip->stOthers.equipSpec.nTotalIMUseTimes	= (*pRes)[54].GetInt();
		(*pRes)[55].GetBlob( pEquip->stOthers.equipSpec.eGrowStar, min((*pRes)[55].GetLen(), sizeof(pEquip->stOthers.equipSpec.eGrowStar)) );
		(*pRes)[56].GetBlob( pEquip->stOthers.equipSpec.nGrowAttValue, min((*pRes)[56].GetLen(), sizeof(pEquip->stOthers.equipSpec.nGrowAttValue)) );
		pEquip->stOthers.equipSpec.byDiamondLevel 				= (*pRes)[57].GetByte();
		// F-Plan 1.6.0 
		pEquip->stOthers.equipSpec.byStarEnhanceLv 				= (*pRes)[58].GetByte();
		pEquip->stOthers.equipSpec.byHolyHoleNum 				= (*pRes)[59].GetByte();

		pEquipList->push_back(pEquip);

	}while(pRes->NextRow());
}

//氏族珍宝激活
VOID CLoongLoongDBCtrl::ProcResQueryActClanTreasure(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pActClanTreasure, pData, std::vector <tagTWrap_Loong_ActClanTreasure*>);
	do{
		tagTWrap_Loong_ActClanTreasure *p = new tagTWrap_Loong_ActClanTreasure;

		p->u16TreasureID	= (*pRes)[0].GetDword();
		p->dwActNameID		= (*pRes)[1].GetDword();
		p->dwRoleID			= (*pRes)[2].GetDword();
		p->byClanType		= (*pRes)[3].GetDword();
		p->dwActTime		= (*pRes)[4].GetDword();

		pActClanTreasure->push_back(p);

	}while(pRes->NextRow());
}
//氏族进入快照
VOID CLoongLoongDBCtrl::ProcResQueryFameHallEnterSnap(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pFameHallEnterSnap, pData, std::vector <tagTWrap_Loong_FameHallEnterSnap*>);
	do{
		tagTWrap_Loong_FameHallEnterSnap *p = new tagTWrap_Loong_FameHallEnterSnap;

		p->dwRoleID				= (*pRes)[0].GetDword();
		p->dwEnterNameID		= (*pRes)[1].GetDword();
		p->byClanType			= (*pRes)[2].GetByte();
		p->dwEnterTime			= (*pRes)[3].GetDword();


		pFameHallEnterSnap->push_back(p);

	}while(pRes->NextRow());
}
//氏族进入快照
VOID CLoongLoongDBCtrl::ProcResQueryRepRstTime(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pRepRstTime, pData, std::vector <tagTWrap_Loong_RepRstTime*>);
	do{
		tagTWrap_Loong_RepRstTime *p = new tagTWrap_Loong_RepRstTime;

		p->byClanType				= (ECLanType)(*pRes)[0].GetDword();
		p->dwRepRstTimeStamp		= (*pRes)[1].GetDword();

		pRepRstTime->push_back(p);

	}while(pRes->NextRow());
}
//好友列表(单向)
VOID CLoongLoongDBCtrl::ProcResQueryFriendSave(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pFriendSave, pData, std::vector <tagTWrap_Loong_Friend*>);
	do{
		tagTWrap_Loong_Friend *p = new tagTWrap_Loong_Friend;

		p->dwRoleID					= (*pRes)[0].GetDword();
		p->stOthers.dwFriendID		= (*pRes)[1].GetDword();
		p->stOthers.nGroupID		= (*pRes)[2].GetDword();


		pFriendSave->push_back(p);

	}while(pRes->NextRow());
}
//友好度
VOID CLoongLoongDBCtrl::ProcResQueryFriendshipSave(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pFriendshipSave, pData, std::vector <tagTWrap_Loong_FriendshipSave*>);
	do{
		tagTWrap_Loong_FriendshipSave *p = new tagTWrap_Loong_FriendshipSave;

		p->dwRoleID					= (*pRes)[0].GetDword();
		p->stOthers.dwFriendID		= (*pRes)[1].GetDword();
		p->stOthers.nFriVal			= (*pRes)[2].GetDword();

		pFriendshipSave->push_back(p);

	}while(pRes->NextRow());
}
//团购
VOID CLoongLoongDBCtrl::ProcResQueryGroupPurchase(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pGroupPurchase, pData, std::vector <tagTWrap_Loong_GroupPurchase*>);
	do{
		tagTWrap_Loong_GroupPurchase *p = new tagTWrap_Loong_GroupPurchase;

		p->stOthers.dwGuildID			= (*pRes)[0].GetDword();
		p->stOthers.dwRoleID			= (*pRes)[1].GetDword();
		p->stOthers.dwMallID			= (*pRes)[2].GetDword();

		p->stOthers.nPrice				= (*pRes)[3].GetDword();
		p->stOthers.nParticipatorNum	= (*pRes)[4].GetDword();
		p->stOthers.nRequireNum			= (*pRes)[5].GetDword();
		p->stOthers.nRemainTime			= (*pRes)[6].GetDword();


		if((*pRes)[7].GetLen() != 0)
		{
			p->pcBlob = new CHAR[(*pRes)[7].GetLen()];
			p->dwBlobSize = (*pRes)[7].GetLen();

			(*pRes)[7].GetBlob( p->pcBlob, (*pRes)[7].GetLen() );
		}
		pGroupPurchase->push_back(p);

	}while(pRes->NextRow());
}
//帮派	
VOID CLoongLoongDBCtrl::ProcResQueryGuild(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pGuild, pData, std::vector <tagTWrap_Loong_Guild*>);
	do{
		tagTWrap_Loong_Guild *p = new tagTWrap_Loong_Guild;

		p->stOthers.dwID				= (*pRes)[0].GetDword();
		p->stOthers.dwFounderNameID	= (*pRes)[2].GetDword();
		p->stOthers.dwSpecState		= (EGuildSpecState)(*pRes)[3].GetInt();
		p->stOthers.byLevel			= (*pRes)[4].GetByte();
		p->stOthers.byHoldCity[0]	= (*pRes)[5].GetByte();
		p->stOthers.byHoldCity[1]	= (*pRes)[6].GetByte();
		p->stOthers.byHoldCity[2]	= (*pRes)[7].GetByte();
		p->stOthers.nFund			= (*pRes)[8].GetInt();
		p->stOthers.nMaterial		= (*pRes)[9].GetInt();
		p->stOthers.nReputation		= (*pRes)[10].GetInt();
		p->stOthers.nDailyCost		= (*pRes)[11].GetInt();
		p->stOthers.n16Peace			= (INT16)(*pRes)[12].GetInt();
		p->stOthers.n16Rank			= (INT16)(*pRes)[13].GetInt();
		
		memcpy(p->szDate,(*pRes)[16].GetString(),(*pRes)[16].GetLen()+1);

		p->stOthers.nGroupPurchase	= (*pRes)[17].GetInt();

		p->stOthers.dwLeaderRoleID	= (*pRes)[18].GetInt();
		p->stOthers.byAffairRemainTimes	= (*pRes)[19].GetByte();
		p->stOthers.bCommendation	= (*pRes)[20].GetBool();

		memset(p->stOthers.szName,0,sizeof(p->stOthers.szName));
		memset(p->stOthers.szTenet,0,sizeof(p->stOthers.szTenet));
		memset(p->stOthers.szSymbolURL,0,sizeof(p->stOthers.szSymbolURL));
		memset(p->stOthers.wGuildPosTakeRewardTimes,0,sizeof(p->stOthers.wGuildPosTakeRewardTimes));

		(*pRes)[1].GetBlob(p->stOthers.szName, (*pRes)[1].GetLen());
		(*pRes)[14].GetBlob(p->stOthers.szTenet, (*pRes)[14].GetLen());
		(*pRes)[15].GetBlob(p->stOthers.szSymbolURL, (*pRes)[15].GetLen());
		(*pRes)[21].GetBlob(p->stOthers.wGuildPosTakeRewardTimes, (*pRes)[21].GetLen());

		p->stOthers.dwGuildValue1	= (*pRes)[22].GetDword();
		p->stOthers.dwGuildValue2	= (*pRes)[23].GetDword();

		pGuild->push_back(p);

	}while(pRes->NextRow());
}
//帮派成员	
VOID CLoongLoongDBCtrl::ProcResQueryGuildMem(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pGuildMem, pData, std::vector <tagTWrap_Loong_GuildMem*>);
	do{
		tagTWrap_Loong_GuildMem *p = new tagTWrap_Loong_GuildMem;

		p->stOthers.sGuildMember.dwRoleID			= (*pRes)[0].GetDword();
		p->stOthers.dwGuildID						= (*pRes)[1].GetDword();
		p->stOthers.sGuildMember.eGuildPos			= (EGuildMemberPos)(*pRes)[2].GetInt();
		p->stOthers.sGuildMember.nTotalContribution	= (*pRes)[3].GetInt();
		p->stOthers.sGuildMember.nContribution		= (*pRes)[4].GetInt();
		p->stOthers.sGuildMember.nExploit			= (*pRes)[5].GetInt();
		p->stOthers.sGuildMember.nSalary				= (*pRes)[6].GetInt();
		memcpy(p->szDate, (*pRes)[7].GetString(), min(X_DATATIME_LEN, (*pRes)[7].GetLen()+1));

		p->stOthers.sGuildMember.bUseGuildWare				= (*pRes)[8].GetBool();

		pGuildMem->push_back(p);

	}while(pRes->NextRow());
}
//Item
VOID CLoongLoongDBCtrl::ProcResQueryItem(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pItem, pData, std::vector <tagTWrap_Loong_Item*>);
	do{
		tagTWrap_Loong_Item *p = new tagTWrap_Loong_Item;

		p->stOthers.n64Serial 					= (*pRes)[0].GetLong();
		p->stOthers.n16Num 						= (*pRes)[1].GetShort();
		p->stOthers.dwTypeID 					= (*pRes)[2].GetDword();
		p->stOthers.byBind 						= (*pRes)[3].GetInt();
		p->stOthers.bLock 						= (*pRes)[4].GetBool();
		p->stOthers.nUseTimes 					= (*pRes)[5].GetInt();

		p->stOthers.eCreateMode					= (EItemCreateMode)(*pRes)[7].GetInt(); 
		p->stOthers.dwCreateID					= (*pRes)[8].GetDword();
		p->stOthers.dwCreatorID 				= (*pRes)[9].GetDword();

		p->stOthers.dwOwnerID 					= (*pRes)[11].GetDword();
		p->stOthers.dwAccountID 				= (*pRes)[12].GetDword();
		p->stOthers.eConType 					= (EItemConType)(*pRes)[13].GetInt();
		p->stOthers.n16Index 					= (*pRes)[14].GetShort();
		p->stOthers.dwNameID					= (*pRes)[15].GetDword();
		//1.3.2 新增
		p->stOthers.dwUnlockTime				= (*pRes)[16].GetDword();

		//FPlan1.2.1
		p->stOthers.dwSpecVal1					= (*pRes)[17].GetDword();
		p->stOthers.dwSpecVal2					= (*pRes)[18].GetDword();

		DataTime2DwordTime(p->stOthers.dw1stGainTime, (*pRes)[6].GetString(), (*pRes)[6].GetLen());
		DataTime2DwordTime(p->stOthers.dwCreateTime, (*pRes)[10].GetString(), (*pRes)[10].GetLen());


		pItem->push_back(p);

	}while(pRes->NextRow());
}
//item cd time
VOID CLoongLoongDBCtrl::ProcResQueryItemCDTime(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pItemCDTime, pData, std::vector <tagTWrap_Loong_ItemCDTime*>);
	do{
		tagTWrap_Loong_ItemCDTime *p = new tagTWrap_Loong_ItemCDTime;

		p->dwRoleID					= (*pRes)[0].GetDword();

		if((*pRes)[1].GetLen() >= sizeof(tagCDTime))
		{
			p->vecCDTime.resize((*pRes)[1].GetLen()/sizeof(tagCDTime));
			(*pRes)[1].GetBlob(&p->vecCDTime[0], (*pRes)[1].GetLen());		
		}

		pItemCDTime->push_back(p);

	}while(pRes->NextRow());
}
//item need log
VOID CLoongLoongDBCtrl::ProcResQueryItemNeedLog(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pItemNeedLog, pData, std::vector <tagTWrap_Loong_ItemNeedLog*>);
	do{
		tagTWrap_Loong_ItemNeedLog *p = new tagTWrap_Loong_ItemNeedLog;

		p->dwTypeID					= (*pRes)[0].GetDword();
		p->bNeedLog					= (*pRes)[1].GetBool();
		p->sMinQlty					= (*pRes)[2].GetShort();

		pItemNeedLog->push_back(p);

	}while(pRes->NextRow());
}
//item log baibao
VOID CLoongLoongDBCtrl::ProcResQueryLogBaiBao(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pLogBaiBao, pData, std::vector <tagTWrap_Loong_LogBaiBao*>);
	do{
		tagTWrap_Loong_LogBaiBao *p = new tagTWrap_Loong_LogBaiBao;

		p->dwAccountID = (*pRes)[0].GetDword();
		p->stOthers.dwRoleID = (*pRes)[1].GetDword();
		p->stOthers.dwTypeID = (*pRes)[4].GetDword();
		p->stOthers.n16Type = (*pRes)[3].GetShort();
		DataTime2DwordTime(*(tagDWORDTime*)&(p->stOthers.dwTime), (*pRes)[2].GetString(), (*pRes)[2].GetLen());
		
		if((*pRes)[5].GetLen() != 0)
		{
			p->ptcWords = new TCHAR[(*pRes)[5].GetLen()/2];
			//
			(*pRes)[5].GetBlob(p->ptcWords, (*pRes)[5].GetLen());
		}
		p->stOthers.n16Size = (INT16)(sizeof(tagBaiBaoRecord) + (*pRes)[5].GetLen() - sizeof(p->stOthers.szWords));

		pLogBaiBao->push_back(p);

	}while(pRes->NextRow());
}
//item role del
VOID CLoongLoongDBCtrl::ProcResQueryRoleDel(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pRoleDel, pData, std::vector <tagTWrap_Loong_RoleDel*>);
	do{
		tagTWrap_Loong_RoleDel *p = new tagTWrap_Loong_RoleDel;

		p->dwAccountID = (*pRes)[0].GetDword();
		p->dwRoleID = (*pRes)[1].GetDword();
		
		(*pRes)[2].GetTCHAR(p->szRoleName, X_SHORT_NAME);
		(*pRes)[3].GetTCHAR(p->szIP, X_IP_LEN);
		(*pRes)[4].GetTCHAR(p->szDeleteTime, X_DATATIME_LEN);

		pRoleDel->push_back(p);

	}while(pRes->NextRow());

}
//roledata
VOID CLoongLoongDBCtrl::ProcResQueryRoleData(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pRoleData, pData, std::vector <tagTWrap_Loong_RoleData*>);
	do{
		tagTWrap_Loong_RoleData *p = new tagTWrap_Loong_RoleData;
		tagRoleDataConst	*pRoleDataConst = &p->stOthers.RoleDataConst;
		tagRoleDataSave		*pRoleDataSave	= &p->stOthers.RoleDataSave;


		(*pRes)[0].GetTCHAR(pRoleDataConst->szRoleName, X_SHORT_NAME);
		pRoleDataConst->dwRoleNameCrc				= (*pRes)[1].GetDword();
		pRoleDataConst->Avatar.bySex				= (*pRes)[3].GetByte();
		pRoleDataConst->Avatar.wHairMdlID			= (*pRes)[4].GetShort();
		pRoleDataConst->Avatar.wHairTexID			= (*pRes)[5].GetShort();
		pRoleDataConst->Avatar.wFaceMdlID			= (*pRes)[6].GetShort();
		pRoleDataConst->Avatar.wFaceDetailTexID		= (*pRes)[7].GetShort();
		pRoleDataConst->Avatar.wDressMdlID			= (*pRes)[8].GetShort();

		pRoleDataSave->DisplaySet					= (*pRes)[2].GetByte();
		pRoleDataSave->dwMapID						= (*pRes)[9].GetDword();
		pRoleDataSave->fCoordinate[0]				= (*pRes)[10].GetFloat();
		pRoleDataSave->fCoordinate[1]				= (*pRes)[11].GetFloat();
		pRoleDataSave->fCoordinate[2]				= (*pRes)[12].GetFloat();
		pRoleDataSave->fFace[0]						= (*pRes)[13].GetFloat();
		pRoleDataSave->fFace[1]						= (*pRes)[14].GetFloat();
		pRoleDataSave->fFace[2]						= (*pRes)[15].GetFloat();

		pRoleDataSave->eClass						= (EClassType)(*pRes)[16].GetInt();
		pRoleDataSave->eClassEx						= (EClassTypeEx)(*pRes)[17].GetInt();
		pRoleDataSave->nLevel						= (*pRes)[18].GetShort();
		pRoleDataSave->nCurExp						= (*pRes)[19].GetLong();
		pRoleDataSave->nHP							= (*pRes)[20].GetInt();
		pRoleDataSave->nMP							= (*pRes)[21].GetInt();

		pRoleDataSave->nAttPoint					= (*pRes)[22].GetInt();
		pRoleDataSave->nTalentPoint					= (*pRes)[23].GetInt();

		pRoleDataSave->nAttPointAdd[ERA_Physique]	= (*pRes)[24].GetInt();
		pRoleDataSave->nAttPointAdd[ERA_Strength]	= (*pRes)[25].GetInt();
		pRoleDataSave->nAttPointAdd[ERA_Pneuma]		= (*pRes)[26].GetInt();
		pRoleDataSave->nAttPointAdd[ERA_InnerForce]	= (*pRes)[27].GetInt();
		pRoleDataSave->nAttPointAdd[ERA_Technique]	= (*pRes)[28].GetInt();
		pRoleDataSave->nAttPointAdd[ERA_Agility]	= (*pRes)[29].GetInt();

		pRoleDataSave->nInjury						= (*pRes)[30].GetInt();
		pRoleDataSave->nVitality					= (*pRes)[31].GetInt();
		pRoleDataSave->nKnowledge					= (*pRes)[32].GetInt();
		pRoleDataSave->nMorality					= (*pRes)[33].GetInt();
		pRoleDataSave->nCulture						= (*pRes)[34].GetInt();
		pRoleDataSave->nCredit						= (*pRes)[35].GetInt();
		pRoleDataSave->nIdentity					= (*pRes)[36].GetInt();
		pRoleDataSave->nVIPPoint					= (*pRes)[37].GetInt();

		pRoleDataSave->n16BagSize					= (*pRes)[38].GetShort();
		pRoleDataSave->nBagGold						= (*pRes)[39].GetInt();
		pRoleDataSave->nBagSilver					= (*pRes)[40].GetInt();
		pRoleDataSave->nBagYuanBao					= (*pRes)[41].GetInt();

		pRoleDataSave->dwGuildID					= (*pRes)[42].GetDword();
		//pRoleDataSave->nWareGold					= (*pRes)[43].GetInt();
		//pRoleDataSave->nWareSilver					= (*pRes)[44].GetInt();
		//pRoleDataSave->nWareYuanBao					= (*pRes)[45].GetInt();

		if(!DataTime2DwordTime(pRoleDataConst->CreateTime,	(*pRes)[46].GetString(), (*pRes)[46].GetLen()))
		{
			//songg
			//IMSG(_T("Error: Func DataTime2DwordTime() run error in CLoongDB::FormatSaveRoleData()!!!!!\r\n"));
			//IMSG(_T("Error: Role Name CRC32 is %u!!!!!\r\n"), pRoleDataConst->dwRoleNameCrc);
			//ASSERT(0);
		}

		DataTime2DwordTime(pRoleDataSave->LoginTime,	(*pRes)[47].GetString(), (*pRes)[47].GetLen());
		DataTime2DwordTime(pRoleDataSave->LogoutTime,	(*pRes)[48].GetString(), (*pRes)[48].GetLen());

		pRoleDataSave->nOnlineTime					= (*pRes)[49].GetDword();
		pRoleDataSave->nCurOnlineTime				= (*pRes)[50].GetDword();

		pRoleDataSave->nRage						= (*pRes)[51].GetInt();
		pRoleDataSave->nEndurance					= (*pRes)[52].GetInt();
		pRoleDataSave->bSafeGuard					= (*pRes)[53].GetInt();

		DataTime2DwordTime(pRoleDataSave->CloseSafeGuardTime,	(*pRes)[54].GetString(), (*pRes)[54].GetLen());

		pRoleDataSave->talent[0].eType				= (ETalentType)(*pRes)[55].GetInt();
		pRoleDataSave->talent[1].eType				= (ETalentType)(*pRes)[56].GetInt();
//  	pRoleDataSave->talent[2].eType				= (ETalentType)(*pRes)[57].GetInt();
//  	pRoleDataSave->talent[3].eType				= (ETalentType)(*pRes)[58].GetInt();

		pRoleDataSave->talent[0].nPoint				= (*pRes)[59].GetInt();
		pRoleDataSave->talent[1].nPoint				= (*pRes)[60].GetInt();
// 		pRoleDataSave->talent[2].nPoint				= (*pRes)[61].GetInt();
// 		pRoleDataSave->talent[3].nPoint				= (*pRes)[62].GetInt();

		pRoleDataSave->dwRebornMapID				= (*pRes)[63].GetDword();
		pRoleDataSave->nTotalTax					= (*pRes)[64].GetInt();
		pRoleDataSave->nMorale						= (*pRes)[65].GetInt();

		DataTime2DwordTime(*(tagDWORDTime*)&pRoleDataSave->dwTimeGetMallFree, (*pRes)[66].GetString(), (*pRes)[66].GetLen());
		pRoleDataSave->sRemoteOpenSet				= (*pRes)[67].GetDword();
		pRoleDataSave->u16ActiveTitleID				= (*pRes)[68].GetInt();
		pRoleDataSave->nExVolume					= (*pRes)[69].GetInt();


		p->dwRoleID									= (*pRes)[70].GetDword();

		ZeroMemory(&pRoleDataSave->AvatarEquip, SIZE_AVATAR_EQUIP);
		if(!(*pRes)[71].IsNull())
		{
			(*pRes)[71].GetBlob(&pRoleDataSave->AvatarEquip, min((*pRes)[71].GetLen(), SIZE_AVATAR_EQUIP));
		}
		p->bRemoveFlag								= (*pRes)[72].GetBool();
		memcpy(p->szRemoveTime,(*pRes)[73].GetString(),(*pRes)[73].GetLen()+1);
		p->dwAccountID								= (*pRes)[74].GetDword();
		//脚本数据
		if(!(*pRes)[75].IsNull())
		{
			(*pRes)[75].GetBlob(pRoleDataSave->Data.dwData, (*pRes)[75].GetLen());
		}
		pRoleDataSave->nTreasureSum					= (*pRes)[76].GetDword();

		pRoleDataSave->byStallLevel					= (*pRes)[77].GetBool();
		pRoleDataSave->nStallDailyExp				= (*pRes)[78].GetInt();
		pRoleDataSave->nStallCurExp					= (*pRes)[79].GetInt();
		pRoleDataSave->dwLastStallTime				= (*pRes)[80].GetDword();

		//1.2.5.0新增
		pRoleDataSave->nHostility					= (*pRes)[81].GetInt();
		pRoleDataSave->nDeadUnSetSafeGuardCountDown = (*pRes)[82].GetInt();
		pRoleDataSave->dwItemTransportMapID			= (*pRes)[83].GetDword();
		pRoleDataSave->fItemTransportX			= (*pRes)[84].GetFloat();
		pRoleDataSave->fItemTransportZ			= (*pRes)[85].GetFloat();
		pRoleDataSave->fItemTransportY			= (*pRes)[86].GetFloat();

		//1.3.0 新增
		pRoleDataSave->nStrength					= (*pRes)[87].GetInt();
		pRoleDataSave->nStrengthNum					= (*pRes)[88].GetInt();
		pRoleDataSave->dwLoverID					= (*pRes)[89].GetDword();
		pRoleDataSave->bHaveWedding					= (*pRes)[90].GetBool();
		pRoleDataSave->timeLastLessing				= (*pRes)[91].GetDword();
		pRoleDataSave->nLessingLevel				= (*pRes)[92].GetInt();
		pRoleDataSave->bNeedPrisonRevive			= (*pRes)[93].GetBool();

		//1.3.1 新增
		pRoleDataSave->nLevelPm						= (*pRes)[94].GetInt();
		pRoleDataSave->nEquipValue					= (*pRes)[95].GetInt();
		pRoleDataSave->nEquipPm						= (*pRes)[96].GetInt();
		pRoleDataSave->dateLastLessingLoong			= (*pRes)[97].GetDword();
		pRoleDataSave->dwCurrentDayOnlineTime		= (*pRes)[98].GetDword();
		pRoleDataSave->timeOneDayFirstLogined		= (*pRes)[99].GetDword();
		pRoleDataSave->timeOnlineRewardPerDayRec	= (*pRes)[100].GetDword();
		pRoleDataSave->dwLastUpgrade				= (*pRes)[101].GetDword();
		p->n64WorkedLevel							= (*pRes)[102].GetLong();


		//1.3.2 新增
		pRoleDataSave->bIsOpenOfflineReward			= (*pRes)[103].GetInt();
		pRoleDataSave->nSpirit						= (*pRes)[104].GetInt();
		pRoleDataSave->dwFixSpiritCD				= (*pRes)[105].GetDword();
		pRoleDataSave->nConsumptiveStrength			= (*pRes)[106].GetInt();
		pRoleDataSave->nKillScore					= (*pRes)[107].GetInt();
		pRoleDataSave->nAdvanceStrength				= (*pRes)[108].GetInt();
		pRoleDataSave->dwMasterID					= (*pRes)[109].GetDword();
		pRoleDataSave->nJingWuPoint					= (*pRes)[110].GetDword();
		pRoleDataSave->nQinWuPoint					= (*pRes)[111].GetDword();

		//1.4.0新增
		pRoleDataSave->nMARelation					= (*pRes)[113].GetInt();

		//1.5.0新增
		if(!(*pRes)[114].IsNull())
		{
			p->dwTStateTblSize = min((*pRes)[114].GetLen(), sizeof(tagTrainStateTable));
			(*pRes)[114].GetBlob(&p->stTrainStateTable, min((*pRes)[114].GetLen(), sizeof(tagTrainStateTable)));
		}


		pRoleDataSave->nWunXun						= (*pRes)[115].GetInt();	
		pRoleDataSave->nWunJi						= (*pRes)[116].GetInt();	
		pRoleDataSave->nLessingLevelEx				= (*pRes)[117].GetInt();	

		//1.5.1新增
		pRoleDataSave->nGodMiraclePoints			= (*pRes)[118].GetInt();

		//2.0新增
		pRoleDataSave->byTrainStatesTransfered		= (*pRes)[119].GetByte();
		pRoleDataSave->nWeary						= (*pRes)[120].GetInt();

		//2.2新增
		pRoleDataSave->nSoaringValue				= (*pRes)[121].GetInt();
		pRoleDataSave->nSoaringSkillLearnTimes		= (*pRes)[122].GetInt();
		pRoleDataSave->nToughness					= (*pRes)[123].GetInt();

		//FPlan1.0.0
		pRoleDataSave->nConsolidateTimes			= (*pRes)[124].GetInt();
		pRoleDataSave->nCompleteRefreshTime			= (*pRes)[125].GetInt();
		pRoleDataSave->nIMRefreshTime				= (*pRes)[126].GetInt();
		pRoleDataSave->dwRoleState					= (*pRes)[127].GetDword();
		pRoleDataSave->u16FollowPetPocketValve		= (*pRes)[128].GetShort();
		pRoleDataSave->u16RidingPetPocketValve		= (*pRes)[129].GetShort();
		pRoleDataSave->nVipLevel					= (*pRes)[130].GetInt();
		pRoleDataSave->dwVipStartTime				= (*pRes)[131].GetDword();
		pRoleDataSave->n16FreeWorldTalkTimes		= (*pRes)[132].GetShort();
		pRoleDataSave->byCanRankFlag				= (*pRes)[133].GetInt();
		pRoleDataSave->nYaoJingValue				= (*pRes)[134].GetDword();

		//FPlan1.2.0
		pRoleDataSave->byBuy50LvlItemFlag			= (*pRes)[135].GetInt();
		pRoleDataSave->byBuy60LvlItemFlag			= (*pRes)[136].GetInt();
		pRoleDataSave->byBuy70LvlItemFlag			= (*pRes)[137].GetInt();
		pRoleDataSave->byBuy80LvlItemFlag			= (*pRes)[138].GetInt();

		//FPlan1.2.1
		pRoleDataSave->nYellowListCompleteRefreshTime	= (*pRes)[139].GetInt();
		pRoleDataSave->nYellowListIMRefreshTime			= (*pRes)[140].GetInt();

		//FPlan1.2.2
		pRoleDataSave->timeTakeGuildReward			= (*pRes)[141].GetDword();
		pRoleDataSave->dwExpPilularUseTimes			= (*pRes)[142].GetDword();

		//FPlan1.3.0
		pRoleDataSave->dwPickupModeSetting			= (*pRes)[143].GetDword();

		//FPlan1.3.3
		pRoleDataSave->byPlayerBack					= (*pRes)[144].GetByte();
		pRoleDataSave->byPlayerBackDays				= (*pRes)[145].GetDword();
		pRoleDataSave->dwVipMaxDays					= (*pRes)[146].GetDword();

		//FPlan1.3.6
		pRoleDataSave->byUsingConstraintsPKSafeGuardLevel = (*pRes)[147].GetByte();
		pRoleDataSave->dwBePlayActLayerID			 = (*pRes)[148].GetDword();
		pRoleDataSave->byBePlayActLevel				 = (*pRes)[149].GetByte();

		// FPlan 1.4.0 神职相关
		pRoleDataSave->nGodhead						= (*pRes)[150].GetInt();
		pRoleDataSave->nGodFaith					= (*pRes)[151].GetInt();
		pRoleDataSave->nGodCondense					= (*pRes)[152].GetInt();
		pRoleDataSave->dwClergy						= (*pRes)[153].GetDword();
		pRoleDataSave->dwPreClergy					= (*pRes)[154].GetDword();
		pRoleDataSave->dwClergyMarsPoint			= (*pRes)[155].GetDword();
		pRoleDataSave->dwClergyApolloPoint			= (*pRes)[156].GetDword();
		pRoleDataSave->dwClergyRabbiPoint			= (*pRes)[157].GetDword();
		pRoleDataSave->dwClergyPeacePoint			= (*pRes)[158].GetDword();
		pRoleDataSave->eClergyCandidate				= (ECampaignGodType)(*pRes)[159].GetInt();
		pRoleDataSave->eSeniorOrJunior				= (ECandidateFlag)(*pRes)[160].GetByte();

		// FPlan 1.4.1
		pRoleDataSave->dwKeyCodeRewarded			= (*pRes)[161].GetDword();

		// FPlan 1.4.3
		pRoleDataSave->dwFamilyID			= (*pRes)[162].GetDword();
		
		// FPlan 1.4.4
		pRoleDataSave->dwTrainDate			= (*pRes)[163].GetDword();
		pRoleDataSave->nTrainNum			= (*pRes)[164].GetDword();

		// FPlan 1.4.6
		pRoleDataSave->nAwardPoint			= (*pRes)[165].GetDword();
		pRoleDataSave->nAwardFlag			= (*pRes)[166].GetDword();

		// FPlan 1.4.8
		//DataTime2DwordTime(pRoleDataSave->timeBloodBrother,	(*pRes)[167].GetString(), (*pRes)[167].GetLen());
		pRoleDataSave->timeBloodBrother		= (*pRes)[167].GetDword();
		pRoleDataSave->dwBrotherTeatherID	= (*pRes)[168].GetDword();

		pRoleDataSave->nPulseLearnRemainTime = (*pRes)[169].GetInt();

		// F-Plan 1.5.0
		pRoleDataSave->nTigerTime					= (*pRes)[170].GetInt();

		pRoleDataSave->nGod							= (*pRes)[171].GetInt();		//神魂
		pRoleDataSave->nMonster						= (*pRes)[172].GetInt();		//魔魂
		pRoleDataSave->nGodPoint					= (*pRes)[173].GetInt();		//神魂声望
		pRoleDataSave->nMonsterPoint				= (*pRes)[174].GetInt();		//魔魂声望

		//添加鲜花鸡蛋属性
		pRoleDataSave->nFlower						= (*pRes)[175].GetInt();		//鲜花
		pRoleDataSave->nEgg							= (*pRes)[176].GetInt();		//鸡蛋
		pRoleDataSave->nHolyValue					= (*pRes)[177].GetInt();		//圣灵值
		pRoleDataSave->nHidAdd						= (*pRes)[178].GetInt();		//命中
		pRoleDataSave->nEEI_All						= (*pRes)[179].GetInt();		//元素全伤

		
		pRoleData->push_back(p);

	}while(pRes->NextRow());
}

//skill
VOID CLoongLoongDBCtrl::ProcResQuerySkill(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pSkill, pData, std::vector <tagTWrap_Loong_Skill*>);
	do{

		tagTWrap_Loong_Skill *p = new tagTWrap_Loong_Skill;

		p->dwRoleID					= (*pRes)[0].GetDword();
		p->stOthers.dwID			= (*pRes)[1].GetDword();
		p->stOthers.nLearnLevel		= (*pRes)[2].GetInt();
		p->stOthers.nSelfLevel		= (*pRes)[3].GetInt();
		p->stOthers.nProficiency	= (*pRes)[4].GetInt();
		p->stOthers.nCoolDown		= (*pRes)[5].GetInt();
		p->stOthers.dwActiveTime	= (*pRes)[6].GetDword();

		pSkill->push_back(p);


	}while(pRes->NextRow());
}
//task
VOID CLoongLoongDBCtrl::ProcResQueryTask(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pTask, pData, std::vector <tagTWrap_Loong_Task*>);
	do{
		tagTWrap_Loong_Task *p = new tagTWrap_Loong_Task;

		p->stOthers.u16QuestID		= (UINT16)(*pRes)[0].GetDword();
		p->stOthers.dwAcceptTime	= (*pRes)[1].GetDword();
		p->stOthers.bQuestFlag		= (*pRes)[2].GetShort();

		for(INT32 j=0; j<QUEST_CREATURES_COUNT; ++j)
		{
			p->stOthers.n16MonsterNum[j] = (INT16)(*pRes)[j+3].GetInt();
		}

		(*pRes)[QUEST_CREATURES_COUNT+3].GetBlob(p->stOthers.Data.dwData, (*pRes)[QUEST_CREATURES_COUNT+3].GetLen());

		if((*pRes)[QUEST_CREATURES_COUNT+4].GetLen() > 0)
			(*pRes)[QUEST_CREATURES_COUNT+4].GetBlob(&p->stOthers.DynamicTarget, (*pRes)[QUEST_CREATURES_COUNT+4].GetLen());


		p->dwRoleID = (*pRes)[QUEST_CREATURES_COUNT+5].GetDword();
		p->stOthers.nChuandaoCount = (*pRes)[QUEST_CREATURES_COUNT+6].GetInt();


		pTask->push_back(p);


	}while(pRes->NextRow());
}
//task
VOID CLoongLoongDBCtrl::ProcResQueryTaskDone(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pTaskDone, pData, std::vector <tagTWrap_Loong_TaskDone*>);
	do{
		tagTWrap_Loong_TaskDone *p = new tagTWrap_Loong_TaskDone;


		p->stOthers.u16QuestID = (UINT16)(*pRes)[0].GetDword();
		p->stOthers.dwStartTime = (*pRes)[2].GetDword();
		p->stOthers.nTimes = (*pRes)[1].GetInt();
		p->dwRoleID			= (*pRes)[3].GetDword();

		pTaskDone->push_back(p);

	}while(pRes->NextRow());
}
//title
VOID CLoongLoongDBCtrl::ProcResQueryTitle(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pTitle, pData, std::vector <tagTWrap_Loong_Title*>);
	do{
		tagTWrap_Loong_Title *p = new tagTWrap_Loong_Title;

		p->stOthers.u16TitleID		= (UINT16)(*pRes)[0].GetDword();
		p->stOthers.dwStateMark		= (*pRes)[1].GetDword();
		p->dwRoleID					= (*pRes)[2].GetDword();
		p->stOthers.dwTimeRemain	= (*pRes)[3].GetDword();

		pTitle->push_back(p);

	}while(pRes->NextRow());
}
//名帖
VOID CLoongLoongDBCtrl::ProcResQueryVisitingCard(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pVisitingCard, pData, std::vector <tagTWrap_Loong_VisitingCard*>);
	do{
		tagTWrap_Loong_VisitingCard *p = new tagTWrap_Loong_VisitingCard;

		p->stOthers.dwLevel			= (*pRes)[0].GetInt();
		p->stOthers.dwJob			= (*pRes)[1].GetInt();
		p->stOthers.dwMateRoleID		= (*pRes)[2].GetInt();
		p->stOthers.dwFactionID		= (*pRes)[3].GetInt();
		p->stOthers.dwPosition		= (*pRes)[4].GetInt();

		p->stOthers.customVCardData.bVisibility		= (*pRes)[5].GetBool();
		p->stOthers.customVCardData.bySex			= (*pRes)[6].GetByte();
		p->stOthers.customVCardData.byConstellation	= (*pRes)[7].GetByte();
		p->stOthers.customVCardData.byChineseZodiac	= (*pRes)[8].GetByte();
		p->stOthers.customVCardData.byArea			= (*pRes)[9].GetByte();
		p->stOthers.customVCardData.byProvince		= (*pRes)[10].GetByte();
		(*pRes)[11].GetTCHAR(p->stOthers.customVCardData.chCity, LEN_CITY_NAME);


		(*pRes)[12].GetTCHAR(p->tcHeadUrl, LEN_MAX_URL);
		(*pRes)[13].GetTCHAR(p->tcSignature, LEN_MAX_SIGNATURE);
		(*pRes)[14].GetTCHAR(p->tcNameHistory, 500);

		p->stOthers.dwRoleID		= (*pRes)[15].GetInt();

		pVisitingCard->push_back(p);

	}while(pRes->NextRow());
}
//元宝
VOID CLoongLoongDBCtrl::ProcResQueryYBAccount(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pYBAccount, pData, std::vector <tagTWrap_Loong_YBAccount*>);
	do{
		tagTWrap_Loong_YBAccount *p = new tagTWrap_Loong_YBAccount;

		p->dwRoleID = (*pRes)[0].GetDword();
		p->nYuanBao = (*pRes)[1].GetDword();
		p->nGold = (*pRes)[2].GetInt();
		p->nSilver = (*pRes)[3].GetInt();
		p->bSellOrder = (*pRes)[4].GetBool();
		p->bBuyOrder = (*pRes)[5].GetBool();

		pYBAccount->push_back(p);

	}while(pRes->NextRow());
}
//元宝交易订单
VOID CLoongLoongDBCtrl::ProcResQueryYuanBaoOrder(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pYuanBaoOrder, pData, std::vector <tagTWrap_Loong_YuanBaoOrder*>);
	do{
		tagTWrap_Loong_YuanBaoOrder *p = new tagTWrap_Loong_YuanBaoOrder;

		p->dwID = (*pRes)[0].GetDword();
		p->dwRoleID = (*pRes)[1].GetDword();
		p->eYBOType = (EYBOTYPE)(*pRes)[2].GetInt();
		p->nPrice = (*pRes)[3].GetInt();
		p->nNum = (*pRes)[4].GetInt();
		p->eYBOMode = (EYBOMODE)(*pRes)[5].GetInt();
		p->dwStartTime = (*pRes)[6].GetDword();
		p->dwEndTime =  (*pRes)[7].GetDword();
		p->nAvgPrice = (*pRes)[8].GetInt();
		p->nDealNum = (*pRes)[9].GetInt();

		pYuanBaoOrder->push_back(p);

	}while(pRes->NextRow());
}
//bill item
VOID CLoongLoongDBCtrl::ProcResQueryBillItem(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pBillItem, pData, std::vector <tagTWrap_Loong_BillItem*>);
	do{
		tagTWrap_Loong_BillItem *p = new tagTWrap_Loong_BillItem;

		strncpy(p->szTokenID,(*pRes)[0].GetString(),(*pRes)[0].GetLen()+1);
		p->dwAccountID = (*pRes)[1].GetDword();
		p->dwItemTypeID = (*pRes)[2].GetDword();
		p->nNum = (*pRes)[3].GetInt();

		pBillItem->push_back(p);

	}while(pRes->NextRow());
}
//bill yuanbao
VOID CLoongLoongDBCtrl::ProcResQueryBillYuanbao(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pBillYuanBao, pData, std::vector <tagTWrap_Loong_BillYuanBao*>);
	do{
		tagTWrap_Loong_BillYuanBao *p = new tagTWrap_Loong_BillYuanBao;

		strncpy(p->szTokenID,(*pRes)[0].GetString(),(*pRes)[0].GetLen()+1);
		p->dwAccountID = (*pRes)[1].GetDword();
		p->nNum = (*pRes)[2].GetInt();

		pBillYuanBao->push_back(p);

	}while(pRes->NextRow());
}
//bill yuanbao log
VOID CLoongLoongDBCtrl::ProcResQueryBillYuanbaoLog(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pBillYuanBaoLog, pData, std::vector <tagTWrap_Loong_BillYuanBaoLog*>);
	do{
		tagTWrap_Loong_BillYuanBaoLog *p = new tagTWrap_Loong_BillYuanBaoLog;

		strncpy(p->szTokenID,(*pRes)[0].GetString(),(*pRes)[0].GetLen()+1);
		p->dwAccountID = (*pRes)[1].GetDword();
		p->nNum = (*pRes)[2].GetInt();
		memcpy(p->szTime,(*pRes)[3].GetString(),(*pRes)[3].GetLen()+1);

		pBillYuanBaoLog->push_back(p);

	}while(pRes->NextRow());
}
//pet skill
VOID CLoongLoongDBCtrl::ProcResQueryPetSkill(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pPetSkill, pData, std::vector <tagTWrap_Loong_PetSkill*>);
	do{
		tagTWrap_Loong_PetSkill *p = new tagTWrap_Loong_PetSkill;

		p->stOthers.dwTypeID	= (*pRes)[0].GetDword();			//'宠物技能id', 
		p->dwPetID = (*pRes)[1].GetDword();							//宠物id', 
		p->stOthers.nPara1	= (*pRes)[2].GetDword();				// '参数1', 
		p->stOthers.nPara2	= (*pRes)[3].GetDword();				// '参数2', 

		pPetSkill->push_back(p);
	}while(pRes->NextRow());
}		
//pet data
VOID CLoongLoongDBCtrl::ProcResQueryPetData(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pPetData, pData, std::vector <tagTWrap_Loong_PetData*>);
	do{
		tagTWrap_Loong_PetData *p = new tagTWrap_Loong_PetData;

		p->stOthers.dwPetID	= (*pRes)[0].GetDword();
		(*pRes)[1].GetTCHAR(p->stOthers.tszName, X_SHORT_NAME);

		p->stOthers.dwMasterID	= (*pRes)[2].GetDword();				// 主人ID
		p->stOthers.dwProtoID	= (*pRes)[3].GetDword();				// 原型ID
		p->stOthers.nQuality		= (*pRes)[4].GetInt();
		p->stOthers.nAptitude	= (*pRes)[5].GetInt();
		p->stOthers.nPotential	= (*pRes)[6].GetInt();
		p->stOthers.nSpirit		= (*pRes)[7].GetInt();
		p->stOthers.nExpCur		= (*pRes)[8].GetInt();
		p->stOthers.nStep		= (*pRes)[9].GetInt();
		p->stOthers.nGrade		= (*pRes)[10].GetInt();
		p->stOthers.nTalentCount	= (*pRes)[11].GetInt();
		p->stOthers.nWuXingEnergy	= (*pRes)[12].GetInt();
		p->stOthers.byPetState		= (*pRes)[13].GetInt();
		p->stOthers.bLocked			= (*pRes)[14].GetBool();

		p->bRemoveFlag				= (*pRes)[15].GetBool();

		p->stOthers.dwPetValue		= (*pRes)[16].GetDword();
		p->stOthers.nPetPm			= (*pRes)[17].GetInt();

		//1.5.0新增
		p->stOthers.dwBornDate	= (*pRes)[18].GetDword();
		p->stOthers.bLive		= (*pRes)[19].GetBool();
		p->stOthers.nAddedLife	= (*pRes)[20].GetInt();

		pPetData->push_back(p);

	}while(pRes->NextRow());
}

//vip stall
VOID CLoongLoongDBCtrl::ProcResQueryVipStall(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pVipStall, pData, std::vector <tagTWrap_Loong_VipStall*>);
	do{
		tagTWrap_Loong_VipStall *p = new tagTWrap_Loong_VipStall;

		BYTE index = (*pRes)[0].GetByte();
		p->byStallIndex	= index;
		p->dwOwnerID	= (*pRes)[1].GetDword();
		p->nRemainTime	= (*pRes)[2].GetInt();

		pVipStall->push_back(p);

	}while(pRes->NextRow());
}
//guild_commodity
VOID CLoongLoongDBCtrl::ProcResQueryGuildCommodity(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pGuildCommodity, pData, std::vector <tagTWrap_Loong_GuildCommodity*>);
	do{
		tagTWrap_Loong_GuildCommodity *p = new tagTWrap_Loong_GuildCommodity;

		p->stOthers.dwRoleID = (*pRes)[0].GetDword();
		p->dwGuildID = (*pRes)[1].GetDword();
		p->stOthers.nLevel = (*pRes)[2].GetInt();
		p->stOthers.nTael = (*pRes)[3].GetInt();

		if(!(*pRes)[4].IsNull())
		{
			(*pRes)[4].GetBlob(p->stOthers.sGoodInfo, (*pRes)[4].GetLen());
			p->dwGoodInfoSize = (*pRes)[4].GetLen();
		}

		pGuildCommodity->push_back(p);

	}while(pRes->NextRow());
}
//guild_skill
VOID CLoongLoongDBCtrl::ProcResQueryGuildSkill(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pGuildSkill, pData, std::vector <tagTWrap_Loong_GuildSkill*>);
	do{
		tagTWrap_Loong_GuildSkill *p = new tagTWrap_Loong_GuildSkill;

		p->dwGuildID				= (*pRes)[0].GetDword();
		p->stOthers.dwSkillID		= (*pRes)[1].GetDword();
		p->stOthers.n16Progress		= (*pRes)[2].GetInt();
		p->stOthers.nLevel			= (*pRes)[3].GetInt();
		p->stOthers.bResearching	= (*pRes)[4].GetBool();

		// FPlan1.0.0
		p->stOthers.bActive			= (*pRes)[5].GetBool();

		pGuildSkill->push_back(p);

	}while(pRes->NextRow());
}
//vip_netbar
VOID CLoongLoongDBCtrl::ProcResQueryVipBar(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pVipNetBar, pData, std::vector <tagTWrap_Loong_VipNetBar*>);
	do{
		tagTWrap_Loong_VipNetBar *p = new tagTWrap_Loong_VipNetBar;

		p->dwAccountID = (*pRes)[0].GetDword();
		p->dwLoginTime = (*pRes)[1].GetDword();

		pVipNetBar->push_back(p);

	}while(pRes->NextRow());
}
//guild_upgrade
VOID CLoongLoongDBCtrl::ProcResQueryGuildUpgrade(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pGuildFacilities, pData, std::vector <tagTWrap_Loong_GuildFacilities*>);
	do{
		tagTWrap_Loong_GuildFacilities *p = new tagTWrap_Loong_GuildFacilities;

		p->dwGuildID = (*pRes)[0].GetDword();
		p->eType = (EFacilitiesType)(*pRes)[1].GetInt();
		p->byLevel = (*pRes)[2].GetByte();
		p->n16Progress = (*pRes)[3].GetInt();

		for (int n=0; n<MAX_UPGRADE_NEED_ITEM_TYPE; n++)
		{
			p->dwItemTypeID[n]	= (*pRes)[n*2+4].GetDword();
			p->nItemNeed[n]		= (*pRes)[n*2+5].GetInt();
		}

		pGuildFacilities->push_back(p);

	}while(pRes->NextRow());
}
//commerce_rank
VOID CLoongLoongDBCtrl::ProcResQueryCommerceRank(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pCommerceRank, pData, std::vector <tagTWrap_Loong_CommerceRank*>);
	do{
		tagTWrap_Loong_CommerceRank *p = new tagTWrap_Loong_CommerceRank;

		p->stOthers.dwRoleID = (*pRes)[0].GetDword();
		p->dwGuildID = (*pRes)[1].GetDword();
		p->stOthers.nTimes = (*pRes)[2].GetInt();
		p->stOthers.nTael = (*pRes)[3].GetInt();

		pCommerceRank->push_back(p);

	}while(pRes->NextRow());
}
//activity
VOID CLoongLoongDBCtrl::ProcResQueryActivity(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pActivityData, pData, std::vector <tagTWrap_Loong_ActivityData*>);
	do{
		tagTWrap_Loong_ActivityData *p = new tagTWrap_Loong_ActivityData;

		p->dwID = (*pRes)[0].GetDword();

		// ActivityData 5个全读 其中后4个是为了合服特殊处理
		for(INT nALoop = 0; nALoop<ACTIVITY_DATA_NUM; ++nALoop)
		{
			if(!(*pRes)[nALoop + 1].IsNull())
			{
				p->bHaveData[nALoop] = TRUE;
				(*pRes)[nALoop + 1].GetBlob(p->stAData[nALoop].Data.dwData, (*pRes)[nALoop + 1].GetLen());
			}
		}

		pActivityData->push_back(p);

	}while(pRes->NextRow());
}
//left msg
VOID CLoongLoongDBCtrl::ProcResQueryLeftMsg(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pLeftMsg, pData, std::vector <tagTWrap_Loong_LeftMsg*>);
	do{
		tagTWrap_Loong_LeftMsg *p = new tagTWrap_Loong_LeftMsg;

		p->dwMsgID = (*pRes)[0].GetDword();
		p->dwRoleID = (*pRes)[1].GetDword();

		if(!(*pRes)[2].IsNull())
		{
			p->pcBlob = new CHAR[(*pRes)[2].GetLen()];
			p->dwBlobSize = (*pRes)[2].GetLen();

			(*pRes)[2].GetBlob(p->pcBlob, (*pRes)[2].GetLen());
		}

		pLeftMsg->push_back(p);

	}while(pRes->NextRow());
}

//external_links
VOID CLoongLoongDBCtrl::ProcResQueryExternalLink(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pExternalLink, pData, std::vector <tagTWrap_Loong_ExternalLink*>);
	do{
		tagTWrap_Loong_ExternalLink *p = new tagTWrap_Loong_ExternalLink;

		(*pRes)[0].GetTCHAR(p->linkName, sizeof(p->linkName));
		(*pRes)[1].GetTCHAR(p->linkValue, sizeof(p->linkValue));

		pExternalLink->push_back(p);

	}while(pRes->NextRow());
}

//name_history
VOID CLoongLoongDBCtrl::ProcResQueryNameHistory(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pNameHistory, pData, std::vector <tagTWrap_Loong_NameHistory*>);
	do{
		tagTWrap_Loong_NameHistory *p = new tagTWrap_Loong_NameHistory;

		p->dwAccountID = (*pRes)[0].GetDword();
		p->dwRoleID	= (*pRes)[1].GetDword();

		for(int iLoop = 0; iLoop<10; iLoop++)
		{
			(*pRes)[iLoop+2].GetTCHAR(&p->szRoleName[X_SHORT_NAME*iLoop], X_SHORT_NAME);
		}

		pNameHistory->push_back(p);

	}while(pRes->NextRow());
}
//city
VOID CLoongLoongDBCtrl::ProcResQueryCity(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pCity, pData, std::vector <tagTWrap_Loong_City*>);
	do{
		tagTWrap_Loong_City *p = new tagTWrap_Loong_City;

		p->stOthers.sCityAtt.dwCityID		= (*pRes)[0].GetDword();
		p->stOthers.sCityAtt.dwGuildID		= (*pRes)[1].GetDword();
		p->stOthers.sCityAtt.nDefence		= (*pRes)[2].GetInt();
		p->stOthers.sCityAtt.nEudemonTally	= (*pRes)[3].GetInt();
		p->stOthers.sCityAtt.nTaxRate		= (*pRes)[4].GetInt();
		p->stOthers.sCityAtt.dwTaxRateTime	= (*pRes)[5].GetDword();
		p->stOthers.sCityAtt.nTaxation		= (*pRes)[6].GetInt();
		p->stOthers.sCityAtt.nProlificacy	= (*pRes)[7].GetInt();

		p->stOthers.nSignUpNum				= (*pRes)[8].GetLen() / sizeof(tagCitySignUp);

		if(p->stOthers.nSignUpNum != 0)
		{
			p->pcSigupList	= new CHAR[(*pRes)[8].GetLen()];
			(*pRes)[8].GetBlob(p->pcSigupList, (*pRes)[8].GetLen());
		}

		pCity->push_back(p);

	}while(pRes->NextRow());
}

//Master
VOID CLoongLoongDBCtrl::ProcResQueryMaster(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pMaster, pData, std::vector <tagTWrap_Loong_Master*>);
	do{
		tagTWrap_Loong_Master *p = new tagTWrap_Loong_Master;

		p->dwID				= (*pRes)[0].GetDword();
		p->nJingWuPoint		= (*pRes)[1].GetInt();
		p->nQinWuPoint		= (*pRes)[2].GetInt();
		p->nLevelUpBonus	= (*pRes)[3].GetInt();
		p->nRewardNum		= (*pRes)[4].GetInt();


		(*pRes)[5].GetBlob(&p->sApprentice, (*pRes)[5].GetLen());

		pMaster->push_back(p);

	}while(pRes->NextRow());
}

//short cut bar
VOID CLoongLoongDBCtrl::ProcResQueryShortCutBar(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pShortCutBar, pData, std::vector <tagTWrap_Loong_ShortCutBar*>);
	do{
		tagTWrap_Loong_ShortCutBar *p = new tagTWrap_Loong_ShortCutBar;

		p->dwRoleID	= (*pRes)[0].GetDword();

		(*pRes)[1].GetBlob(&p->QuickBarData, (*pRes)[1].GetLen());

		pShortCutBar->push_back(p);

	}while(pRes->NextRow());
}

//CentralbillingLog
VOID CLoongLoongDBCtrl::ProcResQueryCentralbillingLog(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	std::set <string> *pCentralbillingLog = (std::set <string>*)pData;

	do{
		string strAccountName = Loong_CheckAccountName((CHAR *)(*pRes)[0].GetString());

		pCentralbillingLog->insert(strAccountName);

	}while(pRes->NextRow());
}

//roledataname
VOID CLoongLoongDBCtrl::ProcResQueryRoleDataName(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	std::set <tstring> *pRoleDataName = (std::set <tstring>*)pData;

	do{	
		TCHAR	szRoleName[X_SHORT_NAME] = {0};
		(*pRes)[0].GetTCHAR(szRoleName, X_SHORT_NAME);
		_tcslwr(szRoleName);

		pRoleDataName->insert(szRoleName);

	}while(pRes->NextRow());
}

//roledatanamecrc
VOID CLoongLoongDBCtrl::ProcResQueryRoleDataNameCrc(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	std::set <DWORD> *pRoleDataNameCrc= (std::set <DWORD>*)pData;

	do{
		pRoleDataNameCrc->insert((*pRes)[0].GetDword());

	}while(pRes->NextRow());

}

//TrainState
VOID CLoongLoongDBCtrl::ProcResQueryTrainState(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pTrainState, pData, std::vector <tagTWrap_Loong_TrainState*>);
	do{
		tagTWrap_Loong_TrainState *p = new tagTWrap_Loong_TrainState;

		p->dwRoleID				= (*pRes)[0].GetDword();
		p->stOthers.dwTrainID	= (*pRes)[1].GetDword();
		p->stOthers.eTrainState = (ETrainState)(*pRes)[2].GetInt(); 

		//FPlan1.0.0
		p->stOthers.nCompleteness = (*pRes)[3].GetInt(); 

		pTrainState->push_back(p);

	}while(pRes->NextRow());
}

//ActiveRankData
VOID CLoongLoongDBCtrl::ProcResQueryActiveRankData(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pActiveRankData, pData, std::vector <tagTWrap_Loong_ActiveRankData*>);
	do{
		tagTWrap_Loong_ActiveRankData *p = new tagTWrap_Loong_ActiveRankData;

		p->dwRoleId    =  (*pRes)[0].GetDword();
		(*pRes)[1].GetTCHAR(p->ucRoleName, X_SHORT_NAME);
		p->n16RoleLevel = (*pRes)[2].GetDword();
		p->nDataType[0] = (*pRes)[3].GetDword();
		p->nDataType[1] = (*pRes)[4].GetDword();
		p->nDataType[2] = (*pRes)[5].GetDword();
		p->nDataType[3] = (*pRes)[6].GetDword();
		p->nDataType[4] = (*pRes)[7].GetDword();
		p->nDataType[5] = (*pRes)[8].GetDword();
		p->nDataType[6] = (*pRes)[9].GetDword();
		p->nDataType[7] = (*pRes)[10].GetDword();

		pActiveRankData->push_back(p);

	}while(pRes->NextRow());
}

//fabao
VOID CLoongLoongDBCtrl::ProcResQueryFabao(OUT LPVOID &pData, Beton::QueryResult *pRes)
{

	MTRANS_POINTER(pFabao, pData, std::vector <tagTWrap_Loong_Fabao*>);
	do{
		tagTWrap_Loong_Fabao *p = new tagTWrap_Loong_Fabao;


		p->n64Serial				= (*pRes)[0].GetLong();

		p->stOthers.n16NativeIntelligence	= (*pRes)[1].GetInt();
		p->stOthers.n16Stage				= (*pRes)[2].GetInt();
		p->stOthers.nPracticeValue			= (*pRes)[3].GetInt();
		(*pRes)[4].GetBlob(p->stOthers.eAttType,min((*pRes)[4].GetLen(), sizeof(p->stOthers.eAttType)));

		(*pRes)[5].GetBlob(p->stOthers.nAttGrowing0,min((*pRes)[5].GetLen(), sizeof(p->stOthers.nAttGrowing0)));
		(*pRes)[6].GetBlob(p->stOthers.nAttGrowing,min((*pRes)[6].GetLen(), sizeof(p->stOthers.nAttGrowing)));

		p->stOthers.eWuxing					= (EWuXing)(*pRes)[7].GetInt();

		(*pRes)[8].GetBlob(p->stOthers.eWuxingHole,min((*pRes)[8].GetLen(), sizeof(p->stOthers.eWuxingHole)));
		(*pRes)[9].GetBlob(p->stOthers.dwWuxingBeadID,min((*pRes)[9].GetLen(), sizeof(p->stOthers.dwWuxingBeadID)));
		(*pRes)[10].GetBlob(p->stOthers.eSlotAttType,min((*pRes)[10].GetLen(), sizeof(p->stOthers.eSlotAttType)));
		(*pRes)[11].GetBlob(p->stOthers.nSlotAttValue,min((*pRes)[11].GetLen(), sizeof(p->stOthers.nSlotAttValue)));

		p->stOthers.nUseLevel			= (*pRes)[13].GetInt();
		p->stOthers.nSlotNum			= (*pRes)[14].GetInt();
		p->stOthers.bySlotLingzhuMatch	= (*pRes)[15].GetInt();

		(*pRes)[16].GetBlob(p->stOthers.bySlotMacthNdx,min((*pRes)[16].GetLen(),sizeof(p->stOthers.bySlotMacthNdx)));

		p->stOthers.nMood			= (*pRes)[17].GetInt();
		p->stOthers.nMoodTicks		= (*pRes)[18].GetInt();
		p->stOthers.dwMoodBuffID	= (*pRes)[19].GetDword();

		p->stOthers.eEleInjury	= (EElementInjury)(*pRes)[20].GetInt();
		p->stOthers.nEleInjury	= (*pRes)[21].GetInt();		
		p->stOthers.nEleResistance[EER_Fire]	= (*pRes)[22].GetInt();
		p->stOthers.nEleResistance[EER_Water]	= (*pRes)[23].GetInt();
		p->stOthers.nEleResistance[EER_Earth]	= (*pRes)[24].GetInt();
		p->stOthers.nEleResistance[EER_Wind]	= (*pRes)[25].GetInt();
		p->stOthers.nEleResistance[EER_Bright]	= (*pRes)[26].GetInt();
		p->stOthers.nEleResistance[EER_Dark]	= (*pRes)[27].GetInt();
		// 
		p->stOthers.wDamageUpgradeTimes		= (*pRes)[28].GetInt();
		p->stOthers.wResistanceUpgradeTimes	= (*pRes)[29].GetInt();
		p->stOthers.dwFairySkill[0]				= (*pRes)[30].GetDword();
		p->stOthers.dwFairySkill[1]				= (*pRes)[31].GetDword();
		p->stOthers.dwFairySkill[2]				= (*pRes)[32].GetDword();
		p->stOthers.dwFairySkill[3]				= (*pRes)[33].GetDword();
		p->stOthers.dwFairySkill[4]				= (*pRes)[34].GetDword();
		p->stOthers.dwUnsureSkill				= (*pRes)[35].GetDword();

		//FPlan 1.2.2
		//"StoredExpOpen,"	"CurStoredExp");
		p->stOthers.bStoredExpOpen				= (*pRes)[36].GetBool();
		p->stOthers.nCurStoredExp				= (*pRes)[37].GetInt();

		//FPlan 1.3.0
		//"FabaoStatus,"		"StatusChangeTime,"		"BirthValue,"				"BirthTimes,"		"LevelBirth"
		p->stOthers.eFabaoStatus			= (EFabaoStatus)(*pRes)[38].GetInt();
		p->stOthers.dwStatusChangeTime	= (*pRes)[39].GetDword();
		p->stOthers.nBirthValue			= (*pRes)[40].GetInt();
		p->stOthers.nBirthTimes			= (*pRes)[41].GetInt();
		(*pRes)[42].GetBlob(p->stOthers.byBirthLevel,min((*pRes)[42].GetLen(),sizeof(p->stOthers.byBirthLevel)));
		p->stOthers.byIsGivenBirthTimes	= (*pRes)[43].GetByte();

		//FPlan 1.4.8
		p->stOthers.byMaxIntelligence	= (*pRes)[44].GetByte();

		//F-Plan 1.5.8
		p->stOthers.nElePierce	= (*pRes)[45].GetInt();
		p->stOthers.wEleInjPromoteTimes	= (*pRes)[46].GetInt();
		p->stOthers.wEleResPromoteTimes	= (*pRes)[47].GetInt();

		pFabao->push_back(p);

	}while(pRes->NextRow());


}

// F-Plan 1.6.0 
VOID CLoongLoongDBCtrl::ProcResQueryHoly(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pHoly, pData, std::vector <tagTWrap_Loong_Holy*>);
	do{
		tagTWrap_Loong_Holy *p = new tagTWrap_Loong_Holy;

		p->n64Serial				= (*pRes)[0].GetLong();
		p->stOthers.nDevourNum	= (*pRes)[1].GetInt();
		p->stOthers.n16EquipmentNumber	= (*pRes)[2].GetInt();
		p->stOthers.nCoValue	= (*pRes)[3].GetInt();
		p->stOthers.nToDayEatNum = (*pRes)[4].GetInt();
		(*pRes)[5].GetBlob(p->stOthers.n64EquipSerialIDs,min((*pRes)[5].GetLen(), sizeof(p->stOthers.n64EquipSerialIDs)));
		p->stOthers.nCostHoly = (*pRes)[6].GetInt();
		p->stOthers.dwLevelUpAtt[ESAT_Demage]	= (*pRes)[7].GetDword();
		p->stOthers.dwLevelUpAtt[ESAT_HolyDef]	= (*pRes)[8].GetDword();
		p->stOthers.dwLevelUpAtt[ESAT_Crit]	= (*pRes)[9].GetDword();
		p->stOthers.dwLevelUpAtt[ESAT_CritRate]	= (*pRes)[10].GetDword();
		p->stOthers.dwLevelUpAtt[ESAT_ExDmg]	= (*pRes)[11].GetDword();
		p->stOthers.dwLevelUpAtt[ESAT_AttackTec]	= (*pRes)[12].GetDword();
		p->stOthers.dwLevelUpAtt[ESAT_NeglectToughness]	= (*pRes)[13].GetDword();
		p->stOthers.dwLevelUpAtt[ESAT_LingNeng]	= (*pRes)[14].GetDword();
		p->stOthers.nMaxDevourNum = (*pRes)[15].GetInt();

		pHoly->push_back(p);

	}while(pRes->NextRow());
}

VOID CLoongLoongDBCtrl::ProcResQueryHolyEquip(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pHolyEquip, pData, std::vector <tagTWrap_Loong_HolyEquip*>);
	do{
		tagTWrap_Loong_HolyEquip *p = new tagTWrap_Loong_HolyEquip;

		p->n64Serial				= (*pRes)[0].GetLong();
		p->stOthers.nCostHoly	= (*pRes)[1].GetInt();			
		p->stOthers.nHolyAttEffect[ESAT_Demage]	= (*pRes)[2].GetInt();
		p->stOthers.nHolyAttEffect[ESAT_HolyDef]	= (*pRes)[3].GetInt();
		p->stOthers.nHolyAttEffect[ESAT_Crit]	= (*pRes)[4].GetInt();
		p->stOthers.nHolyAttEffect[ESAT_CritRate]	= (*pRes)[5].GetInt();
		p->stOthers.nHolyAttEffect[ESAT_ExDmg]	= (*pRes)[6].GetInt();
		p->stOthers.nHolyAttEffect[ESAT_AttackTec]	= (*pRes)[7].GetInt();
		p->stOthers.nHolyAttEffect[ESAT_NeglectToughness]	= (*pRes)[8].GetInt();
		p->stOthers.n16EnhanceCount = (*pRes)[9].GetInt();

		pHolyEquip->push_back(p);

	}while(pRes->NextRow());
}

//ForceBreakOutApprentice
VOID CLoongLoongDBCtrl::ProcResQueryForceBreakOutApprentice(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pForceBreakOutApprentice, pData, std::vector <tagTWrap_Loong_ForceBreakOutApprentice*>);
	do{
		tagTWrap_Loong_ForceBreakOutApprentice *p = new tagTWrap_Loong_ForceBreakOutApprentice;

		p->dwApprenticeID    =  (*pRes)[0].GetDword();
		p->dwMasterID		 = (*pRes)[1].GetDword();
		p->BreakoutInsertTime = (*pRes)[2].GetDword();

		pForceBreakOutApprentice->push_back(p);

	}while(pRes->NextRow());
}


//-------------------------------------------------------------------------------------------------------
// 读取角色任务板任务信息结果
//-------------------------------------------------------------------------------------------------------
VOID CLoongLoongDBCtrl::ProcResQueryBoardQuest(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pBoardQuestSaveData, pData, std::vector <tagTWrap_Sheng_BoardQuestSaveData*>);

	do{
		tagTWrap_Sheng_BoardQuestSaveData *p = new tagTWrap_Sheng_BoardQuestSaveData;

		p->dwRoleID				= (*pRes)[0].GetDword();
		p->u16QuestID			= (UINT16)(*pRes)[1].GetInt();
		p->u16BoardID			= (UINT16)(*pRes)[2].GetInt();
		p->eState				= (EBoardQuestState)(*pRes)[3].GetInt();
		p->byType				= (BYTE)(*pRes)[4].GetInt();

		pBoardQuestSaveData->push_back(p);

	}while(pRes->NextRow());
}

//-------------------------------------------------------------------------------------------------------
// 帮派宣战表
//-------------------------------------------------------------------------------------------------------
VOID CLoongLoongDBCtrl::ProcResQueryGuildWarDeclare(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pGuildWarDeclare, pData, std::vector <tagTWrap_Sheng_GuildWarDeclare*>);

	do{
		tagTWrap_Sheng_GuildWarDeclare *p = new tagTWrap_Sheng_GuildWarDeclare;

		p->dwAttackGuildID = (*pRes)[0].GetDword();
		p->dwDefenceGuildID = (*pRes)[1].GetDword();
		p->dwWarDeclareTime = (*pRes)[2].GetDword();

		DWORD dwWarEnded = (*pRes)[3].GetDword();
		p->bWarEnded = (dwWarEnded == 0)?FALSE:TRUE;

		pGuildWarDeclare->push_back(p);

	}while(pRes->NextRow());
}

//-------------------------------------------------------------------------------------------------------
// 角色在线时间统计
//-------------------------------------------------------------------------------------------------------
VOID CLoongLoongDBCtrl::ProcResQueryCurMonthOnlineTime(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pCurMonthOnlineTime, pData, std::vector <tagTWrap_Sheng_CurMonthOnlineTime*>);

	do{
		tagTWrap_Sheng_CurMonthOnlineTime *p = new tagTWrap_Sheng_CurMonthOnlineTime;

		p->dwAccountID = (*pRes)[0].GetDword();
		p->dwRoleID = (*pRes)[1].GetDword();
		p->nJanuary = (*pRes)[2].GetShort();
		p->nFebruary = (*pRes)[3].GetShort();
		p->nMarch = (*pRes)[4].GetShort();
		p->nApril = (*pRes)[5].GetShort();
		p->nMay = (*pRes)[6].GetShort();
		p->nJune = (*pRes)[7].GetShort();
		p->nJuly = (*pRes)[8].GetShort();
		p->nAugust = (*pRes)[9].GetShort();
		p->nSeptember = (*pRes)[10].GetShort();
		p->nOctober = (*pRes)[11].GetShort();
		p->nNovember = (*pRes)[12].GetShort();
		p->nDecember = (*pRes)[13].GetShort();

		p->byYear = (*pRes)[14].GetByte();

		//FPlan 1.2.2
		DataTime2DwordTime(p->dwLastUpdateTm, (*pRes)[15].GetString(), (*pRes)[15].GetLen());

		pCurMonthOnlineTime->push_back(p);

	}while(pRes->NextRow());
}


//-------------------------------------------------------------------------------------------------------
// 妖精重生
//-------------------------------------------------------------------------------------------------------
VOID CLoongLoongDBCtrl::ProcResQueryFabaoNirVanaData(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pFabaoNirVanaData, pData, std::vector <tagTWrap_Sheng_FabaoNirVanaData*>);

	do{
		tagTWrap_Sheng_FabaoNirVanaData *p = new tagTWrap_Sheng_FabaoNirVanaData;

		p->n64ID							= (*pRes)[0].GetLong();
		p->dwAccountID						= (*pRes)[1].GetDword();
		p->dwRoleID							= (*pRes)[2].GetDword();
		p->data.nStage						= (*pRes)[3].GetInt();
		p->data.nEleInjuryType				= (*pRes)[4].GetInt();
		p->data.nEleInjury					= (*pRes)[5].GetInt();
		p->data.nEleResistance[EER_Fire]	= (*pRes)[6].GetInt();
		p->data.nEleResistance[EER_Water]	= (*pRes)[7].GetInt();
		p->data.nEleResistance[EER_Earth]	= (*pRes)[8].GetInt();
		p->data.nEleResistance[EER_Wind]	= (*pRes)[9].GetInt();
		p->data.nEleResistance[EER_Bright]	= (*pRes)[10].GetInt();
		p->data.nEleResistance[EER_Dark]	= (*pRes)[11].GetInt();
		p->data.wDamageUpgradeTimes		= (*pRes)[12].GetInt();
		p->data.wResistanceUpgradeTimes	= (*pRes)[13].GetInt();

		//F-Plan 1.5.8
		p->data.nElePierce	= (*pRes)[14].GetInt();
		p->data.wEleInjPromoteTimes	= (*pRes)[15].GetInt();
		p->data.wEleResPromoteTimes	= (*pRes)[16].GetInt();

		pFabaoNirVanaData->push_back(p);

	}while(pRes->NextRow());
}

//-------------------------------------------------------------------------------------------------------
// 神之竞标
//-------------------------------------------------------------------------------------------------------
VOID CLoongLoongDBCtrl::ProcResQueryGodBid(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pGodBid, pData, std::vector <tagTWrap_Sheng_GodBid*>);

	do{
		tagTWrap_Sheng_GodBid *p = new tagTWrap_Sheng_GodBid;

		p->dwRoleID		= (*pRes)[0].GetDword();
		p->n64Bidden	= (*pRes)[1].GetLong();
		p->byBidType	= (*pRes)[2].GetByte();
		p->dwBidID		= (*pRes)[3].GetDword();

		pGodBid->push_back(p);

	}while(pRes->NextRow());
}

//-------------------------------------------------------------------------------------------------------
// 砸蛋信息
//-------------------------------------------------------------------------------------------------------
VOID CLoongLoongDBCtrl::ProcResQueryEggInfo(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pEggInfo, pData, std::vector <tagTWrap_Sheng_EggInfo*>);

	do{
		tagTWrap_Sheng_EggInfo *p = new tagTWrap_Sheng_EggInfo;

		p->dwRoleID		= (*pRes)[0].GetDword();

		p->stOthers.nCurLayer = (*pRes)[1].GetInt();
		p->stOthers.n16ColourEggPos1 = (*pRes)[2].GetInt();
		p->stOthers.n16ColourEggPos2 = (*pRes)[3].GetInt();

		for (INT nCnt = 0; nCnt < MAX_EGG_LAYER; nCnt++)
		{
			p->stOthers.sBrokenEggInfo[nCnt].nBrokePos = (*pRes)[(4+2*nCnt)].GetDword();
			p->stOthers.sBrokenEggInfo[nCnt].dwItemID = (*pRes)[(5+2*nCnt)].GetDword();
		}

		pEggInfo->push_back(p);

	}while(pRes->NextRow());
}

//-------------------------------------------------------------------------------------------------------
// 衣橱
//-------------------------------------------------------------------------------------------------------
VOID CLoongLoongDBCtrl::ProcResQueryWardrobe(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pWardrobe, pData, std::vector <tagTWrap_Sheng_Wardrobe*>);

	do{
		tagTWrap_Sheng_Wardrobe *p = new tagTWrap_Sheng_Wardrobe;

		p->dwRoleID		= (*pRes)[0].GetDword();
		p->dwLayerID	= (*pRes)[1].GetDword();
		p->byLevel		= (BYTE)((*pRes)[2].GetDword());
		p->byState		= (BYTE)((*pRes)[3].GetDword());
		p->dwPlayActStartTime	= (*pRes)[4].GetDword();
		p->dwFriendID	= (*pRes)[5].GetDword();

		pWardrobe->push_back(p);

	}while(pRes->NextRow());
}
//-------------------------------------------------------------------------------------------------------
// 领域
//-------------------------------------------------------------------------------------------------------
VOID CLoongLoongDBCtrl::ProcResQueryArea(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pArea, pData, std::vector <tagTWrap_Sheng_Area*>);

	do{
		tagTWrap_Sheng_Area *p = new tagTWrap_Sheng_Area;

		p->dwAreaID		= (*pRes)[0].GetDword();
		p->dwRoleID		= (*pRes)[1].GetInt();
		p->bActivation  = (*pRes)[2].GetInt();
		p->dwCD			= (*pRes)[3].GetInt();

		pArea->push_back(p);

	}while(pRes->NextRow());
}

//-------------------------------------------------------------------------------------------------------
// 稀有领域
//-------------------------------------------------------------------------------------------------------
VOID CLoongLoongDBCtrl::ProcResQueryRareArea(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pRareArea, pData, std::vector <tagTWrap_Sheng_RareArea*>);

	do{
		tagTWrap_Sheng_RareArea *p = new tagTWrap_Sheng_RareArea;

		p->dwAreaID				= (*pRes)[0].GetDword();
		p->dwID					= (*pRes)[1].GetDword();
		p->n64ID				= (*pRes)[2].GetLong();
		p->eType				= (EGlobalLimitedItemExistType)(*pRes)[3].GetInt();
		p->nTime				= (*pRes)[4].GetInt();
		p->dwRecordTime			= (*pRes)[5].GetDword();

		pRareArea->push_back(p);

	}while(pRes->NextRow());
}

//-------------------------------------------------------------------------------------------------------
// F-Plan 1.5.0 SoulCrystal魂晶
//-------------------------------------------------------------------------------------------------------
VOID CLoongLoongDBCtrl::ProcResQuerySoulCrystal(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pSoulCrystal, pData, std::vector <tagTWrap_Loong_SoulCrystal*>);

	do{
		tagTWrap_Loong_SoulCrystal *p = new tagTWrap_Loong_SoulCrystal;

		p->n64SerialNum				= (*pRes)[0].GetLong();
		p->stOthers.SoulSpec.bySoulCrystalLvl					= (*pRes)[1].GetByte();
		p->stOthers.SoulSpec.bySoulCrystalQlty				= (*pRes)[2].GetByte();
		p->stOthers.SoulSpec.dwStaticSkillID				= (*pRes)[3].GetDword();
		p->stOthers.SoulSpec.nSoulAttID[0]				= (*pRes)[4].GetInt();
		p->stOthers.SoulSpec.nSoulAttID[1]			= (*pRes)[5].GetInt();
		p->stOthers.SoulSpec.nSoulAttID[2]			= (*pRes)[6].GetInt();
		p->stOthers.SoulSpec.nSoulAttID[3]			= (*pRes)[7].GetInt();
		p->stOthers.SoulSpec.nSoulAttID[4]			= (*pRes)[8].GetInt();
		p->stOthers.SoulSpec.nSoulAttID[5]			= (*pRes)[9].GetInt();
		p->stOthers.SoulSpec.nSoulAttID[6]			= (*pRes)[10].GetInt();
		p->stOthers.SoulSpec.nSoulAttID[7]			= (*pRes)[11].GetInt();

		pSoulCrystal->push_back(p);

	}while(pRes->NextRow());
}


//-------------------------------------------------------------------------------------------------------
// F-Plan 1.5.0 account_reactive, account_reactiver
//-------------------------------------------------------------------------------------------------------
VOID CLoongLoongDBCtrl::ProcResQueryAccountReactive(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pAccountReactive, pData, std::vector <tagTWrap_Loong_AccountReactive*>);

	do{
		tagTWrap_Loong_AccountReactive *p = new tagTWrap_Loong_AccountReactive;

		p->dwAccountID				= (*pRes)[0].GetInt();
		p->stOthers.dwKey					= (*pRes)[1].GetDword();
		p->stOthers.nScore				= (*pRes)[2].GetInt();
		p->stOthers.byRewardLevel				= (*pRes)[3].GetByte();

		pAccountReactive->push_back(p);

	}while(pRes->NextRow());
}

VOID CLoongLoongDBCtrl::ProcResQueryAccountReactiver(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pAccountReactiver, pData, std::vector <tagTWrap_Loong_AccountReactiver*>);

	do{
		tagTWrap_Loong_AccountReactiver *p = new tagTWrap_Loong_AccountReactiver;

		p->dwAccountID				= (*pRes)[0].GetInt();
		p->stOthers.dwKeyCode					= (*pRes)[1].GetDword();
		p->stOthers.byLevel				= (*pRes)[2].GetByte();
		p->stOthers.dwExp				= (*pRes)[3].GetDword();

		pAccountReactiver->push_back(p);

	}while(pRes->NextRow());
}

// F-Plan 1.5.12
VOID CLoongLoongDBCtrl::ProcResQueryUseKeyCode(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pUseKeyCode, pData, std::vector <tagTWrap_Loong_UseKeyCode*>);

	do{
		tagTWrap_Loong_UseKeyCode *p = new tagTWrap_Loong_UseKeyCode;

		p->dwRoleID				= (*pRes)[0].GetDword();

		pUseKeyCode->push_back(p);

	}while(pRes->NextRow());
}

//-----------------------------------------------------------------------------------------------
//Family	
VOID CLoongLoongDBCtrl::ProcResQueryFamily(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pFamily, pData, std::vector <tagTWrap_Loong_Family*>);
	do{
		tagTWrap_Loong_Family *p = new tagTWrap_Loong_Family;

		p->stFamilyAtt.sFamilyAtt.dwFamilyID = (*pRes)[0].GetDword();
		//if( p->sFamilyAtt.dwFamilyID == GT_INVALID )
		//{
		//	pRes->NextRow();
		//	continue;
		//}
		p->stFamilyAtt.sFamilyAtt.dwLeaderID	= (*pRes)[2].GetDword();
		p->stFamilyAtt.sFamilyAtt.dwFounderID = (*pRes)[3].GetDword();
		p->stFamilyAtt.sFamilyAtt.nActive			= (*pRes)[4].GetInt();
		DataTime2DwordTime(*(tagDWORDTime*)&(p->stFamilyAtt.sFamilyAtt.dwCreateTime), (*pRes)[5].GetString(), (*pRes)[5].GetLen());
		(*pRes)[1].GetTCHAR(p->stFamilyAtt.strFamilyName, MAX_FAMILY_NAME);
		pFamily->push_back(p);

	}while(pRes->NextRow());
}
//-----------------------------------------------------------------------------------------------
//FamilyMem	
VOID CLoongLoongDBCtrl::ProcResQueryFamilyMem(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pFamilyMem, pData, std::vector <tagTWrap_Loong_FamilyMem*>);
	do{
		tagTWrap_Loong_FamilyMem *p = new tagTWrap_Loong_FamilyMem;

		p->stFamilyMem.dwRoleID			= (*pRes)[0].GetDword();
		p->stFamilyMem.dwFamilyID		= (*pRes)[1].GetDword();
		//if( p->stFamilyMem.dwFamilyID == GT_INVALID )
		//{
		//	pRes->NextRow();
		//	continue;
		//}
		DataTime2DwordTime(*(tagDWORDTime*)&(p->stFamilyMem.dwJoinTime), (*pRes)[2].GetString(), (*pRes)[2].GetLen());
		p->stFamilyMem.bLeader	=(*pRes)[3].GetBool();
		p->stFamilyMem.bRewarded = (*pRes)[4].GetBool();

		pFamilyMem->push_back(p);

	}while(pRes->NextRow());
}
//-----------------------------------------------------------------------------------------------
//FamilyQuset	
VOID CLoongLoongDBCtrl::ProcResQueryFamilyQuset(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pFamilyQuset, pData, std::vector <tagTWrap_Loong_FamilyQuset*>);
	do{
		tagTWrap_Loong_FamilyQuset *p = new tagTWrap_Loong_FamilyQuset;

		p->stFamilyQuest.dwFamilyID = (*pRes)[0].GetDword();
		p->stFamilyQuest.nProcess = (*pRes)[1].GetInt();
		p->stFamilyQuest.bIssueState = (1==(*pRes)[2].GetByte()) ? true : false;
	
		for (int i=0, j=3; i<MAX_FAMILY_QUESTS_NUM; ++i, ++j)
		{
			p->stFamilyQuest.QuestData[i].dwQuestID = (*pRes)[j+i].GetDword();
			p->stFamilyQuest.QuestData[i].nTargetCount = (*pRes)[j+i+1].GetInt();
		}

		pFamilyQuset->push_back(p);

	}while(pRes->NextRow());
}
//-----------------------------------------------------------------------------------------------
//FamilyRoleSprite	
VOID CLoongLoongDBCtrl::ProcResQueryFamilyRoleSprite(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pFamilyRoleSprite, pData, std::vector <tagTWrap_Loong_FamilyRoleSprite*>);
	do{
		tagTWrap_Loong_FamilyRoleSprite *p = new tagTWrap_Loong_FamilyRoleSprite;

		p->dwFamilyID								= (*pRes)[0].GetDword();
		p->stFamilyRoleSprite.dwSpriteType			= (*pRes)[1].GetDword();
		(*pRes)[2].GetTCHAR(p->stFamilyRoleSprite.tszRoleName, X_SHORT_NAME);
		p->stFamilyRoleSprite.fShili				= (*pRes)[3].GetInt();
		p->stFamilyRoleSprite.fGongming				= (*pRes)[4].GetFloat();
		p->stFamilyRoleSprite.dwRoleID				= (*pRes)[5].GetFloat();
		p->stFamilyRoleSprite.un2Level				= (*pRes)[6].GetShort();
		p->stFamilyRoleSprite.bQuality				= (*pRes)[7].GetByte();

		pFamilyRoleSprite->push_back(p);

	}while(pRes->NextRow());
}
//-----------------------------------------------------------------------------------------------
//FamilySprite	
VOID CLoongLoongDBCtrl::ProcResQueryFamilySprite(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pFamilySprite, pData, std::vector <tagTWrap_Loong_FamilySprite*>);
	do{
		tagTWrap_Loong_FamilySprite *p = new tagTWrap_Loong_FamilySprite;

		p->stFamilySprite.dwFamilyID								= (*pRes)[0].GetDword();
		p->stFamilySprite.un2Level									= (*pRes)[1].GetShort();
		p->stFamilySprite.n8Exp										= (*pRes)[2].GetLong();
		(*pRes)[3].GetTCHAR(p->stFamilySprite.tszName, X_SHORT_NAME);
		p->stFamilySprite.nAbility[EFSA_HP]							= (*pRes)[4].GetInt();
		p->stFamilySprite.nAbility[EFSA_EXAttack]					= (*pRes)[5].GetInt();
		p->stFamilySprite.nAbility[EFSA_InAttack]					= (*pRes)[6].GetInt();
		p->stFamilySprite.nAbility[EFSA_EXDefense]					= (*pRes)[7].GetInt();
		p->stFamilySprite.nAbility[EFSA_InDefense]					= (*pRes)[8].GetInt();
		p->stFamilySprite.nAbility[EFSA_EXAttackDeeper]				= (*pRes)[9].GetInt();
		p->stFamilySprite.nAbility[EFSA_InAttackDeeper]				= (*pRes)[10].GetInt();
		p->stFamilySprite.nAbility[EFSA_EXAttackResistance]			= (*pRes)[11].GetInt();
		p->stFamilySprite.nAbility[EFSA_InAttackResistance]			= (*pRes)[12].GetInt();
		p->stFamilySprite.nAbility[EFSA_Toughness]					= (*pRes)[13].GetInt();
		p->stFamilySprite.nAbility[EFSA_CritDes]					= (*pRes)[14].GetInt();
		p->stFamilySprite.nAbility[EFSA_ControleffectDeepen]		= (*pRes)[15].GetInt();
		p->stFamilySprite.nAbility[EFSA_ControleffectResistance]	= (*pRes)[16].GetInt();
		p->stFamilySprite.nAbility[EFSA_SlowingeffectDeepen]		= (*pRes)[17].GetInt();
		p->stFamilySprite.nAbility[EFSA_SlowingeffectResistance]	= (*pRes)[18].GetInt();
		p->stFamilySprite.nAbility[EFSA_FixedeffectDeepen]			= (*pRes)[19].GetInt();
		p->stFamilySprite.nAbility[EFSA_FixedeffectResistance]		= (*pRes)[20].GetInt();
		p->stFamilySprite.nAbility[EFSA_AgingeffectDeepen]			= (*pRes)[21].GetInt();
		p->stFamilySprite.nAbility[EFSA_AgingeffectResistance]		= (*pRes)[22].GetInt();

		pFamilySprite->push_back(p);

	}while(pRes->NextRow());
}
//-----------------------------------------------------------------------------------------------
//SpriteRequirement	
VOID CLoongLoongDBCtrl::ProcResQuerySpriteRequirement(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pSpriteRequirement, pData, std::vector <tagTWrap_Loong_SpriteRequirement*>);
	do{
		tagTWrap_Loong_SpriteRequirement *p = new tagTWrap_Loong_SpriteRequirement;

		p->dwFamilyID									= (*pRes)[0].GetDword();
		p->stSpriteRequirement.dwRequirementID			= (*pRes)[1].GetDword();
		p->stSpriteRequirement.bComplete				= (*pRes)[2].GetBool();
		(*pRes)[3].GetTCHAR(p->stSpriteRequirement.tszRoleName, X_SHORT_NAME);
		p->stSpriteRequirement.nRet						= (*pRes)[4].GetInt();

		pSpriteRequirement->push_back(p);

	}while(pRes->NextRow());
}
//-----------------------------------------------------------------------------------------------
VOID CLoongLoongDBCtrl::ProcResQueryGmServerData(OUT LPVOID &pData, Beton::QueryResult *pRes)
{
	MTRANS_POINTER(pGmServerData, pData, std::vector <tagTWrap_Loong_GmServerData*>);
	do{
		tagTWrap_Loong_GmServerData *p = new tagTWrap_Loong_GmServerData;

		p->n64Value = (*pRes)[0].GetLong();

		pGmServerData->push_back(p);

	}while(pRes->NextRow());
}
//-----------------------------------------------------------------------------------------------


