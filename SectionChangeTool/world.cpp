#include "stdafx.h"
#include "world.h"
#include "../WorldDefine/time.h"
#include "../WorldDefine/container_define.h"

BOOL World::Init(DWORD dwSectionID, int nIndex)
{
	m_dwSectionID = dwSectionID;

	// 得到world主键的名字
	TCHAR szWorld[X_SHORT_STRING] = {0};
	_stprintf(szWorld, _T("world_%d"), nIndex);

	TObjRef<VarContainer> pVar = "SectionContainer";
	pVar->Load("VirtualFileSys", szWorld);

	// 得到游戏世界的名称和ID
	LPCTSTR szWorldName = pVar->GetString(_T("name"), szWorld);
	LPCTSTR szDBHost = pVar->GetString(_T("dbhost"), szWorld);
	LPCTSTR szDBName = pVar->GetString(_T("dbname"), szWorld);
	LPCTSTR szDBPsd = pVar->GetString(_T("psd"), szWorld);
	LPCTSTR szDBUser = pVar->GetString(_T("user"), szWorld);
	m_nDBPort = pVar->GetInt(_T("dbport"), szWorld);
	m_dwWorldID = pVar->GetDword(_T("id"), szWorld);

	ZeroMemory(m_szWorldName,	X_SHORT_STRING);
	ZeroMemory(m_szDBHost,		X_LONG_STRING);
	ZeroMemory(m_szDBName,		X_LONG_STRING);
	ZeroMemory(m_szDBPsd,		X_LONG_STRING);
	ZeroMemory(m_szDBUser,		X_LONG_STRING);

	_tcscpy_s(m_szWorldName,	X_SHORT_STRING, szWorldName);
	_tcscpy_s(m_szDBHost,	X_LONG_STRING, szDBHost);
	_tcscpy_s(m_szDBName,	X_LONG_STRING, szDBName);
	_tcscpy_s(m_szDBUser,	X_LONG_STRING, szDBUser);
	_tcscpy_s(m_szDBPsd,	X_LONG_STRING, szDBPsd);

	m_pWorldDB = new ClientDB;
	BOOL bRtv = m_pWorldDB->Init(m_szDBHost, m_szDBUser, m_szDBPsd, m_szDBName, m_nDBPort);
	if (!bRtv)
	{
		//to do 报错
		SAFE_DEL(m_pWorldDB);
		return FALSE;
	}
	
	//KillObj("SectionContainer");
	return TRUE;
}

VOID World::GetRoleData(DWORD dwRoleID, tagTWrap_Loong_RoleData* p)
{
	//InitDB();

	// 获取一个Stream
	MyStream* pStream = m_pWorldDB->m_DB.GetStream();
	if( !P_VALID(pStream) ) return;

	// 获取一条连接
	Connection* pCon = m_pWorldDB->m_DB.GetFreeConnection();

	pStream->SetSelect("roledata", 
		"RoleName,"				"RoleNameCrc,"					"DisplaySet,"					"Sex,"					"HairModelID,"
		"HairColorID,"			"FaceModelID,"					"FaceDetailID,"					"DressModelID,"			"MapID,"	
		"X,"					"Y,"							"Z,"							"FaceX,"				"FaceY,"	
		"FaceZ,"				"Class,"						"ClassEx,"						"Level,"				"ExpCurLevel,"	
		"HP,"					"MP,"							"AttPtAvail,"					"TalentPtAvail,"		"PhysiqueAdded,"	
		"StrengthAdded,"		"PneumaAdded,"					"InnerforceAdded,"				"TechniqueAdded,"		"AgilityAdded,"	
		"Injury,"				"Vitality,"						"Knowledge,"					"Morality,"				"Culture,"	
		"Credit,"				"Identity,"						"VIPPoint,"						"BagSize,"				"BagGold,"	
		"BagSilver,"			"BagYuanBao,"					"GuildID,"						"0,"					"0,"	
		"0,"					"CreateTime,"					"LoginTime,"					"LogoutTime,"			"OnlineTime,"	
		"CurOnlineTime,"		"Rage,"							"Endurance,"					"SGFlag,"				"CloseSGTime,"

		"TalentType1,"			"TalentType2,"					"TalentType3,"					"TalentType4,"			"TalentVal1,"		
		"TalentVal2,"			"TalentVal3,"					"TalentVal4,"					"RebornMapID,"			"TotalTax,"			
		"Morale,"				"GetMallFreeTime,"				"RemoteOpenSet,"				"CurTitleID,"			"ExchangeVolume,"	
		"RoleID,"				"AvatarEquip,"					"RemoveFlag,"					"RemoveTime,"			"AccountID,"		
		"ScriptData,"			"TreasureSum,"					"StallLevel,"					"StallDailyExp,"		"StallCurExp,"	
		"StallLastTime,"

		"Hostility,"			"DeadUnSetSafeGuardCountDown,"	"ItemTransportMapID,"			"ItemTransportX,"		"ItemTransportZ,"
		"ItemTransportY,"		"Strength," 					"StrengthNum,"					"LoverID,"				"HaveWedding,"	
		"LastLessingTime,"		"LastLessingLevel,"				"NeedPrisonRevive,"				"LevelPm,"				"EquipValue,"
		"EquipPm,"				"LastLessingLoongDate,"			"CurrentDayOnlineTime,"			"OneDayFirstLoginedTime,""OnlineRewardPerDayRecTime,"
		"LastUpgrade,"			"WorkedLevel,"					"OfflineExperienceRewardFlag,"	"Spirit,"				"FixSpirit,"
		"ConsumptiveStrength,"	"KillCount,"					"AdvanceStrength,"				"MasterID,"				"JingWuPoint,"
		"QinWuPoint,"			"PetPocketValve");

	pStream->SetWhere();
	pStream->FillString("RoleID=") << dwRoleID;

	// 进行查询
	QueryResult* pRes = m_pWorldDB->m_DB.Query(pStream);

	// 查询结果为空
	if( !P_VALID(pRes) || pRes->GetRowCount() <= 0) return;

	// 获得查询结果
	tagRoleDataConst	*pRoleDataConst = &p->stOthers.RoleDataConst;
	tagRoleDataSave		*pRoleDataSave	= &p->stOthers.RoleDataSave;


	(*pRes)[0].GetTCHAR(pRoleDataConst->szRoleName, X_SHORT_STRING);
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
	pRoleDataSave->nCurExp						= (*pRes)[19].GetInt();
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
	//pRoleDataSave->nWareSilver				= (*pRes)[44].GetInt();
	//pRoleDataSave->nWareYuanBao				= (*pRes)[45].GetInt();

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
	pRoleDataSave->talent[2].eType				= (ETalentType)(*pRes)[57].GetInt();
	pRoleDataSave->talent[3].eType				= (ETalentType)(*pRes)[58].GetInt();

	pRoleDataSave->talent[0].nPoint				= (*pRes)[59].GetInt();
	pRoleDataSave->talent[1].nPoint				= (*pRes)[60].GetInt();
	pRoleDataSave->talent[2].nPoint				= (*pRes)[61].GetInt();
	pRoleDataSave->talent[3].nPoint				= (*pRes)[62].GetInt();

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
	pRoleDataSave->fItemTransportX				= (*pRes)[84].GetFloat();
	pRoleDataSave->fItemTransportZ				= (*pRes)[85].GetFloat();
	pRoleDataSave->fItemTransportY				= (*pRes)[86].GetFloat();

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
	pRoleDataSave->u16PetPocketValve			= (*pRes)[112].GetShort();

	// 释放结果集
	m_pWorldDB->m_DB.FreeQueryResult(pRes);
	m_pWorldDB->m_DB.ReturnConnection(pCon);
	m_pWorldDB->m_DB.ReturnStream(pStream);

	// 关闭数据库
	//CloseDB();
}

//---------------------------------------------------------------------
// 获得所有角色的dwOldID
//---------------------------------------------------------------------
VOID World::GetRoleOldID()
{
	tagRole* pRole = NULL;
	TRoleMap::TMapIterator it =m_mapRole.Begin();

	while(m_mapRole.PeekNext(it, pRole))
	{
		pRole->dwOldID = m_pWorldDB->GetRoleOldID(pRole);
	}
}

VOID World::GetRoleEquipBarItemDetail(const tagRole* pRole, BYTE& byNum, tagTWrap_Loong_Item* &p)
{
	//InitDB();

	// 获取一个Stream
	MyStream* pStream = m_pWorldDB->m_DB.GetStream();
	if( !P_VALID(pStream) ) return;

	// 获取一条连接
	Connection* pCon = m_pWorldDB->m_DB.GetFreeConnection();

	// 处理item表
	pStream->SetSelect("item", "*");
	pStream->SetWhere();
	pStream->FillString("OwnerID=") << pRole->dwOldID;
	pStream->FillString(" and ContainerTypeID=") << EICT_Equip;

	QueryResult* pRes = m_pWorldDB->m_DB.Query(pStream);
	BYTE byResultNum = pRes->GetRowCount(); 
	if (P_VALID(pRes) && byResultNum > 0)
	{
		if ( byResultNum > X_EQUIP_BAR_SIZE ) 
		{
			// to do 给出报错
			return;
		}
		
		byNum = byResultNum;
		p = new tagTWrap_Loong_Item[byNum];	// 该处的内存空间在外部释放
	}
	
	for (INT i=0; i<byResultNum; ++i)
	{
		tagTWrap_Loong_Item* pTmp = &(p[i]);

		// 保存详细数据
		pTmp->stOthers.n64Serial 					= (*pRes)[0].GetLong();
		pTmp->stOthers.n16Num 						= (*pRes)[1].GetShort();
		pTmp->stOthers.dwTypeID 					= (*pRes)[2].GetDword();
		pTmp->stOthers.byBind 						= (*pRes)[3].GetInt();
		pTmp->stOthers.bLock 						= (*pRes)[4].GetBool();
		pTmp->stOthers.dwUnlockTime					= (*pRes)[5].GetDword();
		pTmp->stOthers.nUseTimes 					= (*pRes)[6].GetInt();

		pTmp->stOthers.eCreateMode					= (EItemCreateMode)(*pRes)[8].GetInt(); 
		pTmp->stOthers.dwCreateID					= (*pRes)[9].GetDword();
		pTmp->stOthers.dwCreatorID 					= (*pRes)[10].GetDword();

		pTmp->stOthers.dwOwnerID 					= (*pRes)[12].GetDword();
		pTmp->stOthers.dwAccountID 					= (*pRes)[13].GetDword();
		pTmp->stOthers.eConType 					= (EItemConType)(*pRes)[14].GetInt();
		pTmp->stOthers.n16Index 					= (*pRes)[15].GetShort();
		pTmp->stOthers.dwNameID						= (*pRes)[16].GetDword();
		//1.3.2 新增
		

		DataTime2DwordTime(pTmp->stOthers.dw1stGainTime,	(*pRes)[7].GetString(),		(*pRes)[7].GetLen());
		DataTime2DwordTime(pTmp->stOthers.dwCreateTime,	(*pRes)[11].GetString(),	(*pRes)[11].GetLen());

		pRes->NextRow();
	}

	// 释放资源
	m_pWorldDB->m_DB.FreeQueryResult(pRes);
	m_pWorldDB->m_DB.ReturnConnection(pCon);
	m_pWorldDB->m_DB.ReturnStream(pStream);

	//CloseDB();
}

VOID World::GetRoleEquipBarEquipDetail(INT64 n64Serial, tagTWrap_Loong_Equip* &pEquip)
{
	//InitDB();

	// 获取一个Stream
	MyStream* pStream = m_pWorldDB->m_DB.GetStream();
	if( !P_VALID(pStream) ) return;

	// 获取一条连接
	Connection* pCon = m_pWorldDB->m_DB.GetFreeConnection();

	// 处理item表
	pStream->SetSelect("equip", "*");
	pStream->SetWhere();
	pStream->FillString("SerialNum=") << n64Serial;

	QueryResult* pRes = m_pWorldDB->m_DB.Query(pStream);
	
	if (!P_VALID(pRes) || pRes->GetRowCount() <= 0)
	{
		// to do 报错
		return;
	}

	// 装备属性
	pEquip->stOthers.equipSpec.byQuality 				= (*pRes)[1].GetByte();
	pEquip->stOthers.equipSpec.byMinUseLevel 			= (*pRes)[2].GetByte();
	pEquip->stOthers.equipSpec.byMaxUseLevel 			= (*pRes)[3].GetByte();
	pEquip->stOthers.equipSpec.n16WuHun 				= (*pRes)[4].GetShort();
	pEquip->stOthers.equipSpec.n16MinDmg 				= (*pRes)[5].GetShort();
	pEquip->stOthers.equipSpec.n16MaxDmg 				= (*pRes)[6].GetShort();
	pEquip->stOthers.equipSpec.n16Armor 				= (*pRes)[7].GetShort();
	pEquip->stOthers.equipSpec.nPotVal 					= (*pRes)[8].GetInt();
	pEquip->stOthers.equipSpec.nPotValUsed 				= (*pRes)[9].GetInt();
	pEquip->stOthers.equipSpec.nPotIncTimes				= (*pRes)[10].GetInt();
	pEquip->stOthers.equipSpec.dwPurpleQltyIdfPct		= (*pRes)[11].GetDword();
	
	
	(*pRes)[12].GetBlob(pEquip->stOthers.equipSpec.nRoleAttEffect, min((*pRes)[12].GetLen(), sizeof(pEquip->stOthers.equipSpec.nRoleAttEffect)));
	
	
	pEquip->stOthers.equipSpec.dwLongHunInnerID			= (*pRes)[13].GetDword();
	pEquip->stOthers.equipSpec.dwLongHunOuterID			= (*pRes)[14].GetDword();
	pEquip->stOthers.equipSpec.bySpecAtt				= (*pRes)[15].GetByte();
	pEquip->stOthers.equipSpec.n16Appearance			= (*pRes)[16].GetShort();
	pEquip->stOthers.equipSpec.byRein					= (*pRes)[17].GetByte();
	pEquip->stOthers.equipSpec.bySavvy					= (*pRes)[18].GetByte();
	pEquip->stOthers.equipSpec.byFortune				= (*pRes)[19].GetByte();
	pEquip->stOthers.equipSpec.n8ColorID				= (INT8)(*pRes)[20].GetInt();
	pEquip->stOthers.equipSpec.n16AttALimMod			= (*pRes)[21].GetShort();
	pEquip->stOthers.equipSpec.n16AttALimModPct			= (*pRes)[22].GetShort();
	pEquip->stOthers.equipSpec.byPosyTimes 				= (*pRes)[23].GetByte();
	
	
	(*pRes)[24].GetBlob(pEquip->stOthers.equipSpec.PosyEffect, min((*pRes)[24].GetLen(), sizeof(pEquip->stOthers.equipSpec.PosyEffect)));
	
	
	
	pEquip->stOthers.equipSpec.byEngraveTimes 			= (*pRes)[25].GetByte();
	
	
	(*pRes)[26].GetBlob(pEquip->stOthers.equipSpec.nEngraveAtt, min((*pRes)[26].GetLen(), sizeof(pEquip->stOthers.equipSpec.nEngraveAtt)));
	
	
	pEquip->stOthers.equipSpec.byHoleNum 				= (*pRes)[27].GetByte();
	
	
	(*pRes)[28].GetBlob(pEquip->stOthers.equipSpec.dwHoleGemID, min((*pRes)[28].GetLen(), sizeof(pEquip->stOthers.equipSpec.dwHoleGemID)));
	
	
	pEquip->stOthers.equipSpec.byBrandLevel 			= (*pRes)[29].GetByte();
	
	
	(*pRes)[30].GetBlob(pEquip->stOthers.equipSpec.nBrandAtt, min((*pRes)[30].GetLen(), sizeof(pEquip->stOthers.equipSpec.nBrandAtt)));
	
	
	pEquip->stOthers.equipSpec.byLongfuLevel			= (*pRes)[31].GetByte();
	
	
	(*pRes)[32].GetBlob(pEquip->stOthers.equipSpec.byLongfuAtt, min((*pRes)[32].GetLen(), sizeof(pEquip->stOthers.equipSpec.byLongfuAtt)));
	
	
	pEquip->stOthers.equipSpec.byFlareVal				= (*pRes)[33].GetByte();
	pEquip->stOthers.equipSpec.bCanCut 					= (*pRes)[34].GetBool();
	pEquip->stOthers.equipSpec.n16QltyModPct			= (*pRes)[35].GetShort();
	pEquip->stOthers.equipSpec.n16QltyModPctEx			= (*pRes)[36].GetShort();
	pEquip->stOthers.equipSpec.n16PotValModPct			= (*pRes)[37].GetShort();
	pEquip->stOthers.equipSpec.bySigned					= (*pRes)[38].GetInt();
	(*pRes)[39].GetTCHAR(pEquip->stOthers.equipSpec.szSignature, 10);
	pEquip->stOthers.equipSpec.dwSignatruePlayerID		= (*pRes)[40].GetDword();

	// 释放资源
	m_pWorldDB->m_DB.FreeQueryResult(pRes);
	m_pWorldDB->m_DB.ReturnConnection(pCon);
	m_pWorldDB->m_DB.ReturnStream(pStream);

	//CloseDB();
}

VOID World::GetVisitingCard(const tagRole* pRole, tagTWrap_Loong_VisitingCard*& p)
{
	// 获取一个Stream
	MyStream* pStream = m_pWorldDB->m_DB.GetStream();
	if( !P_VALID(pStream) ) return;

	// 获取一条连接
	Connection* pCon = m_pWorldDB->m_DB.GetFreeConnection();

	// 处理item表
	pStream->SetSelect("visiting_card", "*");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << pRole->dwOldID;

	QueryResult* pRes = m_pWorldDB->m_DB.Query(pStream);

	if (!P_VALID(pRes) || pRes->GetRowCount() <= 0)
	{
		// to do 报错
		return;
	}

	p->stOthers.dwLevel			= (*pRes)[1].GetInt();
	p->stOthers.dwJob			= (*pRes)[2].GetInt();
	p->stOthers.dwMateRoleID	= (*pRes)[3].GetInt();
	p->stOthers.dwFactionID		= (*pRes)[4].GetInt();
	p->stOthers.dwPosition		= (*pRes)[5].GetInt();

	p->stOthers.customVCardData.bVisibility		= (*pRes)[6].GetBool();
	p->stOthers.customVCardData.bySex			= (*pRes)[7].GetByte();
	p->stOthers.customVCardData.byConstellation	= (*pRes)[8].GetByte();
	p->stOthers.customVCardData.byChineseZodiac	= (*pRes)[9].GetByte();
	p->stOthers.customVCardData.byArea			= (*pRes)[10].GetByte();
	p->stOthers.customVCardData.byProvince		= (*pRes)[11].GetByte();
	(*pRes)[12].GetTCHAR(p->stOthers.customVCardData.chCity, LEN_CITY_NAME);

	(*pRes)[13].GetTCHAR(p->tcHeadUrl,		LEN_MAX_URL);
	(*pRes)[14].GetTCHAR(p->tcSignature,	LEN_MAX_SIGNATURE);
	(*pRes)[15].GetTCHAR(p->tcNameHistory,	500);

	m_pWorldDB->m_DB.FreeQueryResult(pRes);
	m_pWorldDB->m_DB.ReturnConnection(pCon);
	m_pWorldDB->m_DB.ReturnStream(pStream);
}