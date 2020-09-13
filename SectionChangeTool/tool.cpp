//-----------------------------------------------------------------------------
//!\file	
//!\brief	
//!
//!\date	
//! last	
//!\author	
//!
//! Copyright (c) 1985-2008 CTCRST Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include "tool.h"
#include "../ServerDefine/login_define.h"
#include "../WorldDefine/login.h"
#include "../ServerDefine/role_data.h"
#include "../ServerDefine/role_data_define.h"

CTool g_Tool;

//-------------------------------------------------------------------
// 工具主函数的初始化
//-------------------------------------------------------------------
BOOL CTool::Init()
{
	
	m_pLog			=	"Log";
	m_pUtil			=	"Util";
	m_pVar			=	"VarContainer";

	// 初始化ID生成器
	m_n64ItemIDGen	=	500000000000;	 

	// 读配置文件
	TCHAR tszFilePath[X_LONG_STRING] = {0};
	GetModuleFileName(NULL, tszFilePath, X_LONG_STRING);
	int nCount = _tcslen(tszFilePath);
	while( tszFilePath[nCount-1] != '\\' )
	{
		nCount--;
	}
	tszFilePath[nCount] = '\0';

	_tcscat(tszFilePath, _T(SERVICE_CONFIG_INI));

	TCHAR tszFile[X_LONG_STRING] = {0};
	if (!m_pUtil->GetIniPath(tszFile, tszFilePath))
	{
		LPCTSTR szLineTemp = m_pUtil->Unicode8ToUnicode("Fail to init tool.\n");
		g_pDlg->Output(szLineTemp);

		return FALSE;
	}

	m_pVar->Load("VirtualFileSys", tszFile);

	// 初始化目标数据库
	LPCTSTR	szLHost		=	m_pVar->GetString(_T("host"),	_T("target_login_db"));
	LPCTSTR	szLUser		=	m_pVar->GetString(_T("user"),	_T("target_login_db"));
	LPCTSTR	szLPsd		=	m_pVar->GetString(_T("psd"),	_T("target_login_db"));
	LPCTSTR	szLDBName	=	m_pVar->GetString(_T("name"),	_T("target_login_db"));
	INT		nLPort		=	m_pVar->GetInt(_T("port"),	_T("target_login_db"));

	m_pdb1 = new ClientDB;
	if( FALSE == m_pdb1->Init(szLHost, szLUser, szLPsd, szLDBName, nLPort) ) 
	{
		// to do 报错
		LPCTSTR szLineTemp = m_pUtil->Unicode8ToUnicode("Fail to connect to New login Database.\n");
		g_pDlg->Output(szLineTemp);

		SAFE_DEL(m_pdb1);
		return FALSE;
	}

	LPCTSTR	szHost		=	m_pVar->GetString(_T("host"),	_T("target_db"));
	LPCTSTR	szUser		=	m_pVar->GetString(_T("user"),	_T("target_db"));
	LPCTSTR	szPsd		=	m_pVar->GetString(_T("psd"),	_T("target_db"));
	LPCTSTR	szDBName	=	m_pVar->GetString(_T("name"),	_T("target_db"));
	INT		nPort		=	m_pVar->GetInt(_T("port"),	_T("target_db"));

	m_pdb2 = new ClientDB;
	if( FALSE == m_pdb2->Init(szHost, szUser, szPsd, szDBName, nPort) ) 
	{
		// to do 报错
		LPCTSTR szLineTemp = m_pUtil->Unicode8ToUnicode("Fail to connect to New Game Database.\n");
		g_pDlg->Output(szLineTemp);
		SAFE_DEL(m_pdb2);
		return FALSE;
	}

	// 读取所有大区和所有世界的信息
	//CreateObj("SectionContainer", "VarContainer");
	
	INT m_nSectionNum = m_pVar->GetInt(_T("num"),	_T("section"));
	for (int i = 1; i<=m_nSectionNum; i++)
	{
		TCHAR szTmp[X_SHORT_STRING] = {0};
		_stprintf(szTmp, _T("section_%d"), i);
		
		DWORD dwSectionID = m_pVar->GetDword(_T("id"), szTmp);
		LPCTSTR szSectionName = m_pVar->GetString(_T("name"), szTmp);

		Section* pSection = new Section(dwSectionID, szSectionName);
		if(!P_VALID(pSection))	
		{
			LPCTSTR szLineTemp = m_pUtil->Unicode8ToUnicode("Fail to do merge.\n");
			g_pDlg->Output(szLineTemp);

			return FALSE;
		}
		
		// 初始化大区中的游戏世界
		if( FALSE == pSection->Init() )
		{
			LPCTSTR szLineTemp = m_pUtil->Unicode8ToUnicode("Fail to init section.\n");
			g_pDlg->Output(szLineTemp);

			SAFE_DEL(pSection);
			return FALSE;
		}

		m_mapSection.Add(pSection->GetID(), pSection);
	}

	// 删除变量管理器
	//KillObj("SectionContainer");
	return TRUE;
}

//-------------------------------------------------------------------
// 从指定的xml文件中读入要参与合服的角色
//-------------------------------------------------------------------
VOID CTool::LoadRoleFromXml(LPCTSTR tszFile)
{
	// 先将所有角色存入一个临时的map
	TMap<DWORD, tagRole*> mapRoleInfo;
	mapRoleInfo.Clear();

	if (FALSE == m_RoleInfoReader.Load( tszFile, mapRoleInfo ))
	{
		LPCTSTR szLineTemp = m_pUtil->Unicode8ToUnicode("Fail to merge!");
		g_pDlg->Output(szLineTemp);
	}

	// 将存入的角色按照大区、世界存入
	DWORD dwIndex = 1;
	
	TMap<DWORD, tagRole*>::TMapIterator it = mapRoleInfo.Begin();
	tagRole* pRole = NULL;
	while (mapRoleInfo.PeekNext(it, pRole))
	{
		Section* pSection = m_mapSection.Peek(pRole->dwSectionID);
		if (!P_VALID(pSection))
		{
			continue;
		}

		TWorldMap mapWorld = pSection->GetWorldMap();
		World* pWorld = mapWorld.Peek(pRole->dwWorldID);
		if (!P_VALID(pWorld))
		{
			continue;
		}

		pWorld->AddRoleInfo(dwIndex, pRole);
		dwIndex++;
	}

	Section* pSection = NULL;
	TSectionMap::TMapIterator it1 = m_mapSection.Begin();
	while (m_mapSection.PeekNext(it1, pSection))
	{
		
		TWorldMap WorldMap = pSection->GetWorldMap();
		TWorldMap::TMapIterator it2 = WorldMap.Begin();
		World* pWorld = NULL;
		while (WorldMap.PeekNext(it2, pWorld))
		{
			pWorld->GetRoleOldID();
		}
	}
}

//-------------------------------------------------------------------
//主逻辑 相应各个事件
//-------------------------------------------------------------------
/**
\param nOrderID		命令id
\param nParaNum		参数个数
\param ...			需要传的参数
\return	bool		true为成功， false为失败
\remarks */
BOOL CTool::Control( INT nOrderID, INT nParaNum, ... )
{
	va_list argp;
	va_start(argp, nParaNum);

	switch( nOrderID )
	{
		case EO_XMLSelected:
		{
			TCHAR* cXmlPath;
			cXmlPath = va_arg( argp, TCHAR* );
			//TCHAR tszFile[X_LONG_STRING];
			//m_pUtil->Unicode8ToUnicode( cXmlPath , tszFile );

			LoadRoleFromXml(cXmlPath);
			return true;
		}
		case EO_Merge:
		{
			if( FALSE == DoMerge() )
			{
				// to do 提示操作失败
			}	

			return true;
		}
		default:
		{
			return false;
		}
	}

	va_end(argp);

}

//-------------------------------------------------------------------
// 合服操作
//-------------------------------------------------------------------
BOOL CTool::DoMerge()
{
	// 先合LoginDB的account表
	if( FALSE == SaveAccount())
	{
		LPCTSTR szLineTemp = m_pUtil->Unicode8ToUnicode("Fail to save table account.\n");
		g_pDlg->Output(szLineTemp);

		return FALSE;
	}

	// 再合WorldDB的roledata表
	if ( FALSE == SaveRoleData())
	{
		LPCTSTR szLineTemp = m_pUtil->Unicode8ToUnicode("Fail to save roledata and item.\n");
		g_pDlg->Output(szLineTemp);

		return FALSE;
	}

	return TRUE;
}

//-------------------------------------------------------------------
// 生成loginDB的account表的具体操作
//-------------------------------------------------------------------
BOOL CTool::SaveAccountDetail(const tagRole* pRole, DWORD dwAccountID)
{
	Section* pSection = NULL;
	pSection = m_mapSection.Peek(pRole->dwSectionID);
	if (!P_VALID(pSection))
	{
		LPCTSTR szLineTemp = m_pUtil->Unicode8ToUnicode("Fail to get section.\n");
		g_pDlg->Output(szLineTemp);

		return FALSE;
	}
	
	CHAR szPsd[MAX_MD5_ARRAY] = {0};	// 密码
	CHAR szMibao[MIBAO_LEN] ={0};		// 密保
	BYTE byPrivilege =	0;
	EPlayerLoginStatus eLoginStatus	= (EPlayerLoginStatus)0;
	DWORD dwWorldNameCrc = 0;
	DWORD dwFrobidMask = 0;
	BOOL bGuard = 0;
	
	pSection->GetAccountDetail(pRole->szAccoutName, szPsd, byPrivilege, eLoginStatus, dwWorldNameCrc, dwFrobidMask, bGuard, szMibao);

	// 将详细信息copy至目的数据库的account表中
	
	// 获取一个Stream
	MyStream* pAnotherStream = m_pdb1->m_DB.GetStream();
	if( !P_VALID(pAnotherStream) )
	{
		LPCTSTR szLineTemp = m_pUtil->Unicode8ToUnicode("Fail to get db stream.\n");
		g_pDlg->Output(szLineTemp);

		return FALSE;
	}

	// 获取一条连接
	Connection* pAnotherCon = m_pdb1->m_DB.GetFreeConnection();

	pAnotherStream->SetInsert("account");
	pAnotherStream->FillString("id=") << dwAccountID;
	pAnotherStream->FillString(",name='").FillString(pRole->szAccoutName, pAnotherCon).FillString("'");
	pAnotherStream->FillString(",psd='").FillString(szPsd, pAnotherCon).FillString("'");
	pAnotherStream->FillString(",privilege=") << byPrivilege;
	pAnotherStream->FillString(",login_status=") << 0;
	pAnotherStream->FillString(",worldname_crc=") << dwWorldNameCrc;
	pAnotherStream->FillString(",forbid_mask=") << dwFrobidMask;
	pAnotherStream->FillString(",guard=") << bGuard;
	pAnotherStream->FillString(",mibao='").FillBlob(szMibao, MIBAO_LEN, pAnotherCon).FillString("'");

	m_pdb1->m_DB.Execute(pAnotherStream);

	// 归还连接，释放stream
	m_pdb1->m_DB.ReturnConnection(pAnotherCon);
	m_pdb1->m_DB.ReturnStream(pAnotherStream);

	return TRUE;
}

//-------------------------------------------------------------------
// 生成loginDB的account表
//-------------------------------------------------------------------
BOOL CTool::SaveAccount()
{
	DWORD dwAccountID = 1;

	TSectionMap::TMapIterator it1= m_mapSection.Begin();
	Section* pSection = NULL;

	while(m_mapSection.PeekNext(it1, pSection))
	{
		TWorldMap mapWorld = pSection->GetWorldMap();
		TWorldMap::TMapIterator it2= mapWorld.Begin();
		World* pWorld = NULL;
		
		while(mapWorld.PeekNext(it2, pWorld))
		{
			// 遍历角色map，进行处理
			TRoleMap mapRole = pWorld->GetRoleMap();
			TRoleMap::TMapIterator it3= mapRole.Begin();
			tagRole* pRole = NULL;

			while( mapRole.PeekNext(it3, pRole) )
			{
				// 将该角色在原login数据库中Account表中对应的记录copy到目的数据库的Account表中来

				// copy之前先检查目的数据库的account表中是否已经有了该账号
			
				// 获取一个Stream
				MyStream* pStream = m_pdb1->m_DB.GetStream();
				if( !P_VALID(pStream) )
				{
					LPCTSTR szLineTemp = m_pUtil->Unicode8ToUnicode("Fail to get db stream.\n");
					g_pDlg->Output(szLineTemp);
					return FALSE;
				}

				// 获取一条连接
				Connection* pCon = m_pdb1->m_DB.GetFreeConnection();

				pStream->SetSelect("account", "*");
				pStream->SetWhere();
				pStream->FillString("name='").FillString(pRole->szAccoutName, pCon).FillString("'");

				// 进行操作
				QueryResult* pResult = m_pdb1->m_DB.Query(pStream);
			
				if( P_VALID(pResult))
				{
					if( pResult->GetRowCount() > 0)
					{
						// 已经有了该账号，该条不处理，继续处理下一条
						m_pdb1->m_DB.FreeQueryResult(pResult);
						m_pdb1->m_DB.ReturnConnection(pCon);
						m_pdb1->m_DB.ReturnStream(pStream);

						continue;
					}
				}
			
				if(SaveAccountDetail(pRole, dwAccountID))
				{
					// 写account_common表

					// 获得AccountID
					pStream->SetSelect("account_common", "*");
					pStream->SetWhere();
					pStream->FillString("AccountName='").FillString(pRole->szAccoutName, pCon).FillString("'");
					
					QueryResult* pResult = m_pdb2->m_DB.Query(pStream);
					if( P_VALID(pResult) && pResult->GetRowCount() > 0)
					{
						continue;
					}

					// 获得account_common信息
					pStream->SetSelect("account_common", "SafeCodeCrc,ResetTime,BagPsdCrc,BaiBaoYuanBao,WareSize,WareSilver,PersonalSet,LastUseRoleID");
					pStream->SetWhere();
					pStream->FillString("AccountName='").FillString(pRole->szAccoutName, pCon).FillString("'");
					
					QueryResult* pRes = pWorld->m_pWorldDB->m_DB.Query(pStream);
					if (P_VALID(pRes) && pRes->GetRowCount() > 0)
					{
						tagAccountCommon* p = new tagAccountCommon;
						p->sSafeCode.dwSafeCodeCrc	= (*pRes)[0].GetDword();
						p->sSafeCode.dwTimeReset	= (*pRes)[1].GetDword();
						p->dwBagPsdCrc				= (*pRes)[2].GetDword();
						p->nBaiBaoYuanBao			= (*pRes)[3].GetInt();
						p->n16WareSize				= (*pRes)[4].GetInt();
						p->n64WareSilver			= (*pRes)[5].GetLong();
						(*pRes)[6].GetBlob(&p->stPersonalSet,min((*pRes)[6].GetLen(), min((*pRes)[6].GetLen(),sizeof(tagDWPlayerPersonalSet))  ));
						p->dwLastUseRoleID			= (*pRes)[7].GetDword();

						// 写入
						pStream->SetInsert("account_common");
						pStream->FillString("AccountID=") << dwAccountID;
						pStream->FillString(",AccountName='").FillString(pRole->szAccoutName).FillString("'");
						pStream->FillString(",SafeCodeCrc=") << p->sSafeCode.dwSafeCodeCrc;
						pStream->FillString(",ResetTime=") << p->sSafeCode.dwTimeReset;
						pStream->FillString(",BagPsdCrc=") << p->dwBagPsdCrc;
						//pStream->FillString(",BaiBaoYuanBao=") << p->nBaiBaoYuanBao;
						pStream->FillString(",WareSize=") << p->n16WareSize;
						//pStream->FillString(",WareSilver=") << p->n64WareSilver;
						pStream->FillString(",PersonalSet='").FillBlob(&p->stPersonalSet, sizeof(tagDWPlayerPersonalSet), pCon).FillString("'");
						pStream->FillString(",LastUseRoleID=") << p->dwLastUseRoleID;
						
						if (!m_pdb2->m_DB.Execute(pStream))
						{
							delete p;
							return FALSE;
						}
						
						delete p;
					}

					dwAccountID++;
				}
			}
		}
	}

	return TRUE;
}

//-------------------------------------------------------------------
// 生成LoongDB的roldata表
//-------------------------------------------------------------------
BOOL CTool::SaveRoleData()
{
	DWORD dwRoleID = 1;

	TSectionMap::TMapIterator it1= m_mapSection.Begin();
	Section* pSection = NULL;

	while(m_mapSection.PeekNext(it1, pSection))
	{
		
		TWorldMap mapWorld = pSection->GetWorldMap();
		TWorldMap::TMapIterator it2= mapWorld.Begin();
		World* pWorld = NULL;

		while(mapWorld.PeekNext(it2, pWorld))
		{
			// 遍历角色map，进行处理
			TRoleMap mapRole = pWorld->GetRoleMap();
			TRoleMap::TMapIterator it3= mapRole.Begin();
			tagRole* pRole = NULL;

			while( mapRole.PeekNext(it3, pRole) )
			{
				// 将该角色在原数据库中roledata表中对应的记录copy到目的数据库的roledata表中来

				// copy之前先检查目的数据库的roledata表中是否已经有了该名字的角色
				DWORD dwNameCrc = 0;
				if ( FALSE == CheckRoleName(pRole, dwRoleID, dwNameCrc) )
				{
					LPCTSTR szLineTemp = m_pUtil->Unicode8ToUnicode("Fail to create role name and crc.\n");
					g_pDlg->Output(szLineTemp);
					return FALSE;
				}
				
				// 检查通过，继续……
				if(SaveRoleDataDetail(pRole, dwRoleID, dwNameCrc))
				{
					pRole->dwNewID = dwRoleID;

					if( SaveEquip(pRole) && SaveVisitingCard(pRole) && SaveSkill(pRole) && SaveTitle(pRole))
					{
						dwRoleID++;
					}
					else
					{
						LPCTSTR szLineTemp = m_pUtil->Unicode8ToUnicode("Fail to add one's equip.\n");
						g_pDlg->Output(szLineTemp);

						return FALSE;
					}
				}
				else
				{
					LPCTSTR szLineTemp = m_pUtil->Unicode8ToUnicode("Fail to save roledata.\n");
					g_pDlg->Output(szLineTemp);
				}
			}
		}
	}

	return TRUE;
}

//-------------------------------------------------------------------
// 生成LoongDB的roldata表的具体操作（将指定角色的RoleData信息copy至目的数据库的roledata表中）
//-------------------------------------------------------------------
BOOL CTool::SaveRoleDataDetail(const tagRole* pRole, DWORD dwRoleID, DWORD dwNameCrc)
{
	// 获取一个Stream
	MyStream* pStream = m_pdb2->m_DB.GetStream();
	if( !P_VALID(pStream) )
		return FALSE;

	// 获取一条连接
	Connection* pCon = m_pdb2->m_DB.GetFreeConnection();


	Section* pSection = NULL;
	pSection = m_mapSection.Peek(pRole->dwSectionID);
	if (!P_VALID(pSection))
	{
		return FALSE;
	}
	
	World* pWorld = NULL;
	TWorldMap mapWorld = pSection->GetWorldMap();

	pWorld = mapWorld.Peek(pRole->dwWorldID);
	if (!P_VALID(pWorld))
	{
		return FALSE;
	}

	tagTWrap_Loong_RoleData* pRoleData = new tagTWrap_Loong_RoleData;  // 要记得释放
	ZeroMemory(pRoleData, sizeof(tagTWrap_Loong_RoleData));

	pWorld->GetRoleData(pRole->dwOldID, pRoleData);

	// 获得该角色的AccountID
	DWORD dwAccountID = GetRoleAccountID(pRole);
	if ( GT_INVALID == dwAccountID)
	{
		// 获得账号ID失败
		return FALSE;
	}

	// 组装sql语句
	pStream->SetInsert("roledata");
	pStream->FillString("AccountID=") << dwAccountID;
	pStream->FillString(",RoleID=") << dwRoleID;

	// 需保存属性相关
	tagRoleDataSave *p = &pRoleData->stOthers.RoleDataSave;

	pStream->FillString(",AvatarEquip='").FillBlob(&p->AvatarEquip, SIZE_AVATAR_EQUIP, pCon);
	pStream->FillString("',DisplaySet=")<< (BYTE)p->DisplaySet;
	pStream->FillString(",MapID=") << p->dwMapID;
	pStream->FillString(",X=") << p->fCoordinate[0];// 坐标
	pStream->FillString(",Y=") << p->fCoordinate[1];
	pStream->FillString(",Z=") << p->fCoordinate[2];
	pStream->FillString(",FaceX=") << p->fFace[0];// 朝向
	pStream->FillString(",FaceY=") << p->fFace[1];
	pStream->FillString(",FaceZ=") << p->fFace[2];
	pStream->FillString(",RebornMapID=") << p->dwRebornMapID;
	pStream->FillString(",SpeakOff=") << p->bSpeakOff;
	pStream->FillString(",Class=") << (INT32)p->eClass;
	pStream->FillString(",ClassEx=") << p->eClassEx;
	pStream->FillString(",Level=") << p->nLevel;
	pStream->FillString(",LastUpgrade=") << p->dwLastUpgrade;
	pStream->FillString(",ExpCurLevel=") << p->nCurExp;
	pStream->FillString(",HP=") << p->nHP;
	pStream->FillString(",MP=") << p->nMP;
	pStream->FillString(",Rage=") << p->nRage;
	pStream->FillString(",Endurance=") << p->nEndurance;
	pStream->FillString(",Vitality=") << p->nVitality;
	pStream->FillString(",Injury=") << p->nInjury;
	pStream->FillString(",Knowledge=") << p->nKnowledge;
	pStream->FillString(",Morale=") << p->nMorale;
	pStream->FillString(",Morality=") << p->nMorality;
	pStream->FillString(",Culture=") << p->nCulture;
	pStream->FillString(",Hostility=") << p->nHostility;
	pStream->FillString(",Credit=") << p->nCredit;
	pStream->FillString(",Identity=") << p->nIdentity;
	pStream->FillString(",Spirit=") << p->nSpirit;
	pStream->FillString(",FixSpirit=") << p->dwFixSpiritCD;
	pStream->FillString(",VIPPoint=") << p->nVIPPoint;
	pStream->FillString(",AttPtAvail=") << p->nAttPoint;
	pStream->FillString(",TalentPtAvail=") << p->nTalentPoint;
	pStream->FillString(",Strength=") << p->nStrength;
	pStream->FillString(",EquipValue=") << p->nEquipValue;
	pStream->FillString(",AdvanceStrength=") << p->nAdvanceStrength;
	pStream->FillString(",ConsumptiveStrength=") << p->nConsumptiveStrength;
	pStream->FillString(",KillCount=") << p->nKillScore;
	pStream->FillString(",PhysiqueAdded=") << p->nAttPointAdd[ERA_Physique];
	pStream->FillString(",StrengthAdded=") << p->nAttPointAdd[ERA_Strength];
	pStream->FillString(",PneumaAdded=") << p->nAttPointAdd[ERA_Pneuma];
	pStream->FillString(",InnerforceAdded=") << p->nAttPointAdd[ERA_InnerForce];
	pStream->FillString(",TechniqueAdded=") << p->nAttPointAdd[ERA_Technique];
	pStream->FillString(",AgilityAdded=") << p->nAttPointAdd[ERA_Agility];
	pStream->FillString(",TalentType1=") << p->talent[0].eType;
	pStream->FillString(",TalentType2=") << p->talent[1].eType;
	pStream->FillString(",TalentType3=") << p->talent[2].eType;
	pStream->FillString(",TalentType4=") << p->talent[3].eType;
	pStream->FillString(",TalentVal1=") << p->talent[0].nPoint;
	pStream->FillString(",TalentVal2=") << p->talent[1].nPoint;
	pStream->FillString(",TalentVal3=") << p->talent[2].nPoint;
	pStream->FillString(",TalentVal4=") << p->talent[3].nPoint;
	pStream->FillString(",SGFlag=") << p->bSafeGuard;
	pStream->FillString(",BagSize=") << p->n16BagSize;		// 背包
	pStream->FillString(",BagGold=") << p->nBagGold;
	pStream->FillString(",BagSilver=") << p->nBagSilver;
	pStream->FillString(",BagYuanBao=") << 0; // 背包中的元宝清空
	pStream->FillString(",ExchangeVolume=") << p->nExVolume;
	pStream->FillString(",TotalTax=") << p->nTotalTax;	// 缴税总额
	pStream->FillString(",RemoteOpenSet=") << (DWORD)p->sRemoteOpenSet;
	pStream->FillString(",CurTitleID=") << p->u16ActiveTitleID;

	CHAR szTmpTime[X_DATATIME_LEN+1] = {0};
	if(DwordTime2DataTime(szTmpTime, sizeof(szTmpTime), p->LoginTime))
	{
		pStream->FillString(",LoginTime='").FillString(szTmpTime);
		pStream->FillString("'");

		DwordTime2DataTime(szTmpTime, sizeof(szTmpTime), p->LogoutTime);
		pStream->FillString(",LogoutTime='").FillString(szTmpTime);
		pStream->FillString("'");

		DwordTime2DataTime(szTmpTime, sizeof(szTmpTime), p->CloseSafeGuardTime);
		pStream->FillString(",CloseSGTime='").FillString(szTmpTime);
		pStream->FillString("'");

		DwordTime2DataTime(szTmpTime, sizeof(szTmpTime), p->dwTimeGetMallFree);
		pStream->FillString(",GetMallFreeTime='").FillString(szTmpTime);
		pStream->FillString("'");
	}
	else
	{
		IMSG(_T("Error: Func DwordTime2DataTime() run error in CLoongDB::FormatSaveRoleData()!!!!!\r\n"));
		ASSERT(0);
	}

	pStream->FillString(",OnlineTime=") << p->nOnlineTime;
	pStream->FillString(",CurOnlineTime=") << p->nCurOnlineTime;
	pStream->FillString(",ScriptData='");
	pStream->FillBlob(p->Data.dwData, sizeof(DWORD)*ESD_Role, pCon);
	pStream->FillString("'");
	pStream->FillString(",StallLevel=") << p->byStallLevel;
	pStream->FillString(",StallDailyExp=") << p->nStallDailyExp;
	pStream->FillString(",StallCurExp=") << p->nStallCurExp;
	pStream->FillString(",StallLastTime=") << p->dwLastStallTime;
	pStream->FillString(",DeadUnSetSafeGuardCountDown=") << p->nDeadUnSetSafeGuardCountDown;
	pStream->FillString(",NeedPrisonRevive=") << p->bNeedPrisonRevive;
	//pStream->FillString(",LoverID=") << 4294967295;
	//pStream->FillString(",HaveWedding=") << 0;
	pStream->FillString(",LastLessingTime=") << (DWORD)p->timeLastLessing;
	pStream->FillString(",LastLessingLevel=") << p->nLessingLevel;
	pStream->FillString(",LastLessingLoongDate=") << (DWORD)p->dateLastLessingLoong;
	pStream->FillString(",CurrentDayOnlineTime=") << p->dwCurrentDayOnlineTime;
	pStream->FillString(",OneDayFirstLoginedTime=") << (DWORD)p->timeOneDayFirstLogined;
	pStream->FillString(",OnlineRewardPerDayRecTime=") << (DWORD)p->timeOnlineRewardPerDayRec;
	pStream->FillString(",PetPocketValve=") << (DWORD)p->u16PetPocketValve;
	//pStream->FillString(",MasterID=") << 4294967295;
	pStream->FillString(",JingWuPoint=") << p->nJingWuPoint;
	pStream->FillString(",QinWuPoint=") << p->nQinWuPoint;
	pStream->FillString(",MiraclePrcNum=") << (INT16)p->n16MiraclePracticeNum;
	pStream->FillString(",WorkedLevel=") << 1;

	// 固定属性相关
	tagRoleDataConst *pRoleDataConst = &pRoleData->stOthers.RoleDataConst;

	CHAR szCreateTime[X_DATATIME_LEN+1] = {0};
	DwordTime2DataTime(szCreateTime, sizeof(szCreateTime), pRoleDataConst->CreateTime);

	if (0==dwNameCrc)
	{
		pStream->FillString(",RoleNameCrc=") << (DWORD)pRoleDataConst->dwRoleNameCrc;
	}
	else
	{
		pStream->FillString(",RoleNameCrc=") << dwNameCrc;
	}
	pStream->FillString(",CreateTime='").FillString(szCreateTime).FillString("'");
	pStream->FillString(",RoleName='").FillString(pRole->szRoleName).FillString("'");
	pStream->FillString(",Sex=") << pRoleDataConst->Avatar.bySex;
	pStream->FillString(",HairModelID=") << pRoleDataConst->Avatar.wHairMdlID;
	pStream->FillString(",HairColorID=") << pRoleDataConst->Avatar.wHairTexID;
	pStream->FillString(",FaceModelID=") << pRoleDataConst->Avatar.wFaceMdlID;
	pStream->FillString(",FaceDetailID=") << pRoleDataConst->Avatar.wFaceDetailTexID;
	pStream->FillString(",DressModelID=") << pRoleDataConst->Avatar.wDressMdlID;

	// 执行语句
	BOOL bRet = m_pdb2->m_DB.Execute(pStream);

	// 释放内存
	delete pRoleData;

	// 归还连接，释放stream
	m_pdb1->m_DB.ReturnConnection(pCon);
	m_pdb1->m_DB.ReturnStream(pStream);

	return bRet;
}

//--------------------------------------------------------------------------
// 确保所有section下的多个world下的所有role不重名，有重名的话就改名
//--------------------------------------------------------------------------
BOOL CTool::CheckRoleName(tagRole* &pRole, DWORD dwRoleID, DWORD &dwNameCrc)
{
	//// 获取一个Stream
	//MyStream* pStream = m_pdb2->m_DB.GetStream();
	//if( !P_VALID(pStream) )
	//	return FALSE;

	//// 获取一条连接
	//Connection* pCon = m_pdb2->m_DB.GetFreeConnection();

	//pStream->SetSelect("roledata", "*");
	//pStream->SetWhere();
	//pStream->FillString("RoleName='").FillString(pRole->szRoleName, pCon).FillString("'");

	//// 进行操作
	//QueryResult* pResult = m_pdb2->m_DB.Query(pStream);
	//if( P_VALID(pResult) && pResult->GetRowCount() > 0)
	//{
	//	// 获得该角色所在的服名
	//	Section* pSection = NULL;
	//	pSection = m_mapSection.Peek(pRole->dwSectionID);
	//	if (!P_VALID(pSection))
	//	{
	//		ASSERT(0);
	//		return FALSE;
	//	}

	//	World* pWorld = NULL;
	//	TWorldMap mapWorld = pSection->GetWorldMap();

	//	pWorld = mapWorld.Peek(pRole->dwWorldID);
	//	if (!P_VALID(pWorld))
	//	{
	//		ASSERT(0);
	//		return FALSE;
	//	}

	//	// 将该角色改名
	//	TCHAR szNewName[X_SHORT_NAME] = {0};
	//	_stprintf(szNewName, _T("%s_%s"), pRole->szRoleName, pWorld->m_szWorldName);
	//	szNewName[X_SHORT_NAME-1] = '\0';
	//	_tcscpy_s(pRole->szRoleName, X_SHORT_NAME, szNewName);
	//
	//	// 重新计算Crc
	//	_tcslwr(szNewName);
	//	dwNameCrc = m_pUtil->Crc32(szNewName);
	//	
	//	bRet = TRUE;
	//}
	//else
	//{
	//	bRet = TRUE;
	//}
	//
	//// 释放Result
	//m_pdb2->m_DB.FreeQueryResult(pResult);
	//m_pdb2->m_DB.ReturnConnection(pCon);
	//m_pdb2->m_DB.ReturnStream(pStream);

	// 获得该角色所在的服名
	Section* pSection = NULL;
	pSection = m_mapSection.Peek(pRole->dwSectionID);
	if (!P_VALID(pSection))
	{
		ASSERT(0);
		return FALSE;
	}

	World* pWorld = NULL;
	TWorldMap mapWorld = pSection->GetWorldMap();

	pWorld = mapWorld.Peek(pRole->dwWorldID);
	if (!P_VALID(pWorld))
	{
		ASSERT(0);
		return FALSE;
	}

	// 将该角色改名
	TCHAR szNewName[X_SHORT_NAME] = {0};
	_stprintf(szNewName, _T("%s_%s"), pRole->szRoleName, pWorld->m_szWorldName);
	szNewName[X_SHORT_NAME-1] = '\0';
	_tcscpy_s(pRole->szRoleName, X_SHORT_NAME, szNewName);

	// 重新计算Crc
	_tcslwr(szNewName);
	dwNameCrc = m_pUtil->Crc32(szNewName);

	return TRUE;
}

DWORD CTool::GetRoleAccountID(const tagRole* pRole)
{
	DWORD dwAccountID = GT_INVALID;

	if (!P_VALID(pRole))
	{
		return GT_INVALID;
	}

	// 获取一个Stream
	MyStream* pStream = m_pdb1->m_DB.GetStream();
	if( !P_VALID(pStream) )
		return FALSE;

	// 获取一条连接
	Connection* pCon = m_pdb1->m_DB.GetFreeConnection();

	pStream->SetSelect("account", "id");
	pStream->SetWhere();
	pStream->FillString("name='").FillString(pRole->szAccoutName, pCon).FillString("'");

	// 进行操作
	QueryResult* pResult = m_pdb1->m_DB.Query(pStream);
	if( P_VALID(pResult) && pResult->GetRowCount() > 0)
	{
		dwAccountID = (*pResult)[0].GetDword();
	}
	else
	{
		dwAccountID = GT_INVALID;
	}

	// 释放Result
	m_pdb1->m_DB.FreeQueryResult(pResult);
	m_pdb1->m_DB.ReturnConnection(pCon);
	m_pdb1->m_DB.ReturnStream(pStream);

	return dwAccountID;
}

BOOL CTool::SaveEquip(const tagRole* pRole)
{
	Section* pSection = NULL;
	pSection = m_mapSection.Peek(pRole->dwSectionID);
	if (!P_VALID(pSection))
	{
		ASSERT(0);
		return FALSE;
	}

	World* pWorld = NULL;
	TWorldMap mapWorld = pSection->GetWorldMap();

	pWorld = mapWorld.Peek(pRole->dwWorldID);
	if (!P_VALID(pWorld))
	{
		ASSERT(0);
		return FALSE;
	}

	// 连接该游戏世界的数据库
	//pWorld->InitDB();
	
	// 读取该觉得装备栏里的装备的item表中数据
	tagTWrap_Loong_Item* pItem = NULL; // 函数结束前释放
	BYTE byItemNum = 0; 

	pWorld->GetRoleEquipBarItemDetail(pRole, byItemNum, pItem);
	//if ( !P_VALID(pItem) && byItemNum <= 0 )
	//{
	//	// 关闭数据库连接
	//	//pWorld->CloseDB();
	//	ASSERT(0);
	//	return FALSE;
	//}
	
	// 将详细数据写到目的数据库的item和equip表中
	for (int i = 0; i<byItemNum; i++)
	{
		INT64 n64Serial = ++m_n64ItemIDGen; 

		MyStream* pStream = m_pdb2->m_DB.GetStream();
		if( !P_VALID(pStream) ) return FALSE;
		Connection* pCon = m_pdb2->m_DB.GetFreeConnection();
	
		tagTWrap_Loong_Item* p = &(pItem[i]);

		//　写入item表
		pStream->SetInsert("item");
		pStream->FillString("SerialNum=") << n64Serial;
		pStream->FillString(",Num=") << p->stOthers.n16Num;
		pStream->FillString(",TypeID=") << p->stOthers.dwTypeID;
		pStream->FillString(",Bind=") << p->stOthers.byBind;
		pStream->FillString(",UnlockTime=") << p->stOthers.dwUnlockTime;
		pStream->FillString(",LockStat=") << p->stOthers.bLock;
		pStream->FillString(",UseTimes=") << p->stOthers.nUseTimes;

		pStream->FillString(",CreateMode=") << p->stOthers.eCreateMode;
		pStream->FillString(",CreateID=") << pRole->dwNewID;
		pStream->FillString(",CreatorID=") << pRole->dwNewID;

		CHAR szTmpTime[X_DATATIME_LEN + 1] = {0};
		DwordTime2DataTime(szTmpTime, sizeof(szTmpTime), p->stOthers.dwCreateTime);
		pStream->FillString(",CreateTime='").FillString(szTmpTime);
		pStream->FillString("'");
		
		CHAR szTime[X_DATATIME_LEN + 1] = {0};
		DwordTime2DataTime(szTime, sizeof(szTime), p->stOthers.dw1stGainTime);
		pStream->FillString(",FirstGainTime='").FillString(szTime);
		pStream->FillString("'");
		pStream->FillString(",OwnerID=") << pRole->dwNewID;

		DWORD dwAccountID = GetRoleAccountID(pRole);
		pStream->FillString(",AccountID=") << dwAccountID;
		pStream->FillString(",ContainerTypeID=") << p->stOthers.eConType;
		pStream->FillString(",Suffix=") << p->stOthers.n16Index;

		if ( FALSE == m_pdb2->m_DB.Execute(pStream))
		{
			// to do 给出报错
			ASSERT(0);
			return FALSE;
		}

		// 写入equip表
		// 先读出Equip表中的详细信息
		tagTWrap_Loong_Equip* it = new tagTWrap_Loong_Equip; // 函数结尾处释放
		pWorld->GetRoleEquipBarEquipDetail(p->stOthers.n64Serial, it);
		if (!P_VALID(it))
		{
			// to do 这个地方需要提示整个操作失败了
			ASSERT(0);
			return FALSE;
		}
		
		// 向equip表中写
		pStream->SetInsert("equip");
		pStream->FillString("SerialNum=") << n64Serial;
		pStream->FillString(",Quality=") << it->stOthers.equipSpec.byQuality;
		pStream->FillString(",MinUseLevel=") << it->stOthers.equipSpec.byMinUseLevel;
		pStream->FillString(",MaxUseLevel=") << it->stOthers.equipSpec.byMaxUseLevel;
		pStream->FillString(",AttALimMod=") << it->stOthers.equipSpec.n16AttALimMod;
		pStream->FillString(",AttALimModPct=") << it->stOthers.equipSpec.n16AttALimModPct;

		pStream->FillString(",WuHun=") << it->stOthers.equipSpec.n16WuHun;
		pStream->FillString(",MinDmg=") << it->stOthers.equipSpec.n16MinDmg;
		pStream->FillString(",MaxDmg=") << it->stOthers.equipSpec.n16MaxDmg;
		pStream->FillString(",Armor=") << it->stOthers.equipSpec.n16Armor;
		pStream->FillString(",PotVal=") << it->stOthers.equipSpec.nPotVal;
		pStream->FillString(",PotValUsed=") << it->stOthers.equipSpec.nPotValUsed;

		pStream->FillString(",PosyTimes=") << it->stOthers.equipSpec.byPosyTimes;
		pStream->FillString(",EngraveTimes=") << it->stOthers.equipSpec.byEngraveTimes;
		pStream->FillString(",HoleNum=") << it->stOthers.equipSpec.byHoleNum;
		pStream->FillString(",BrandLevel=") << it->stOthers.equipSpec.byBrandLevel;
		pStream->FillString(",LongFuLevel=") << it->stOthers.equipSpec.byLongfuLevel;

		pStream->FillString(",CanCut=") << it->stOthers.equipSpec.bCanCut;
		pStream->FillString(",LongInnerID=") << it->stOthers.equipSpec.dwLongHunInnerID;
		pStream->FillString(",LongOuterID=") << it->stOthers.equipSpec.dwLongHunOuterID;
		pStream->FillString(",SpecAtt=") << it->stOthers.equipSpec.bySpecAtt;
		pStream->FillString(",FlareVal=") << it->stOthers.equipSpec.byFlareVal;
		pStream->FillString(",QltyModPct=") << it->stOthers.equipSpec.n16QltyModPct;
		pStream->FillString(",QltyModPctEx=") << it->stOthers.equipSpec.n16QltyModPctEx;
		pStream->FillString(",PotValModPct=") << it->stOthers.equipSpec.n16PotValModPct;

		// 时装属性
		pStream->FillString(",Appearance=") << it->stOthers.equipSpec.n16Appearance;
		pStream->FillString(",Rein=") << it->stOthers.equipSpec.byRein;
		pStream->FillString(",Savvy=") << it->stOthers.equipSpec.bySavvy;
		pStream->FillString(",Fortune=") << it->stOthers.equipSpec.byFortune;
		pStream->FillString(",ColorID=") << it->stOthers.equipSpec.n8ColorID;

		pStream->FillString(",RoleAttEffect='").FillBlob(it->stOthers.equipSpec.nRoleAttEffect, sizeof(it->stOthers.equipSpec.nRoleAttEffect), pCon);
		pStream->FillString("',PosyEffect='").FillBlob(it->stOthers.equipSpec.PosyEffect, sizeof(it->stOthers.equipSpec.PosyEffect), pCon);
		pStream->FillString("',EngraveAtt='").FillBlob(it->stOthers.equipSpec.nEngraveAtt, sizeof(it->stOthers.equipSpec.nEngraveAtt), pCon);
		pStream->FillString("',HoleGemID='").FillBlob(it->stOthers.equipSpec.dwHoleGemID, sizeof(it->stOthers.equipSpec.dwHoleGemID), pCon);
		pStream->FillString("',DerateVal='").FillBlob(it->stOthers.equipSpec.nBrandAtt, sizeof(it->stOthers.equipSpec.nBrandAtt), pCon);
		pStream->FillString("',WuXing='").FillBlob(it->stOthers.equipSpec.byLongfuAtt, sizeof(it->stOthers.equipSpec.byLongfuAtt), pCon);
		pStream->FillString("'");

		//1.3.0新增
		pStream->FillString(",PotIncTimes=") << it->stOthers.equipSpec.nPotIncTimes;
		pStream->FillString(",PurpleQltyIdfPct=") << it->stOthers.equipSpec.dwPurpleQltyIdfPct;

		//1.3.2 新增
		//"Signed,"		"Signature,"		"SignRoleID");
		pStream->FillString(",Signed=") << it->stOthers.equipSpec.bySigned;
		pStream->FillString(",SignRoleID=") << it->stOthers.equipSpec.dwSignatruePlayerID;
		pStream->FillString(",Signature='").FillString(it->stOthers.equipSpec.szSignature).FillString("'");
		
		if ( FALSE == m_pdb2->m_DB.Execute(pStream))
		{
			// to do 给出报错
			ASSERT(0);
			return FALSE;
		}

		delete it;
		
		// 释放连接
		m_pdb2->m_DB.ReturnConnection(pCon);
		m_pdb2->m_DB.ReturnStream(pStream);
	}

	// 释放内存
	
	delete[] pItem; 

	// 关闭数据库连接
	//pWorld->CloseDB();

	return TRUE;
}

BOOL CTool::SaveVisitingCard(const tagRole* pRole)
{
	// 获取一个Stream
	MyStream* pStream = m_pdb2->m_DB.GetStream();
	if( !P_VALID(pStream) )
		return FALSE;

	// 获取一条连接
	Connection* pCon = m_pdb2->m_DB.GetFreeConnection();

	Section* pSection = NULL;
	pSection = m_mapSection.Peek(pRole->dwSectionID);
	if (!P_VALID(pSection))
	{
		ASSERT(0);
		return FALSE;
	}

	World* pWorld = NULL;
	TWorldMap mapWorld = pSection->GetWorldMap();

	pWorld = mapWorld.Peek(pRole->dwWorldID);
	if (!P_VALID(pWorld))
	{
		ASSERT(0);
		return FALSE;
	}
	
	tagTWrap_Loong_VisitingCard* it = new tagTWrap_Loong_VisitingCard;
	pWorld->GetVisitingCard(pRole, it);

	pStream->SetInsert("visiting_card");
	pStream->FillString("Level=") << (INT32)it->stOthers.dwLevel;
	pStream->FillString(",Job=") << (INT32)it->stOthers.dwJob;
	pStream->FillString(",MateRoleID=") << (INT32)it->stOthers.dwMateRoleID;
	pStream->FillString(",FactionID=") << (INT32)it->stOthers.dwFactionID;
	pStream->FillString(",Position=") << (INT32)it->stOthers.dwPosition;

	pStream->FillString(",Visibility=") << it->stOthers.customVCardData.bVisibility;
	pStream->FillString(",Sex=") << it->stOthers.customVCardData.bySex;
	pStream->FillString(",Constellation=") << it->stOthers.customVCardData.byConstellation;
	pStream->FillString(",ChineseZodiac=") << it->stOthers.customVCardData.byChineseZodiac;
	pStream->FillString(",Area=") << it->stOthers.customVCardData.byArea;
	pStream->FillString(",Province=") << it->stOthers.customVCardData.byProvince;
	pStream->FillString(",City='").FillString(it->stOthers.customVCardData.chCity, pCon).FillString("'");
	pStream->FillString(",HeadUrl='").FillString(it->tcHeadUrl, pCon).FillString("'");
	pStream->FillString(",Signature='").FillString(it->tcSignature, pCon).FillString("'");
	pStream->FillString(",NameHistory='").FillString(it->tcNameHistory, pCon).FillString("'");
	pStream->FillString(",RoleID=") << pRole->dwNewID;

	BOOL bRet = m_pdb2->m_DB.Execute(pStream);
	
	// 释放Result
	m_pdb2->m_DB.ReturnConnection(pCon);
	m_pdb2->m_DB.ReturnStream(pStream);

	delete it;

	if (!bRet)
	{
		LPCTSTR szLineTemp = m_pUtil->Unicode8ToUnicode("Fail to save visiting_card.\n");
		g_pDlg->Output(szLineTemp);
	}

	return bRet;
}

BOOL CTool::SaveSkill(const tagRole* pRole)
{
	// 获取一个Stream
	MyStream* pStream = m_pdb2->m_DB.GetStream();
	if( !P_VALID(pStream) )
		return FALSE;

	// 获取一条连接
	Connection* pCon = m_pdb2->m_DB.GetFreeConnection();

	Section* pSection = NULL;
	pSection = m_mapSection.Peek(pRole->dwSectionID);
	if (!P_VALID(pSection))
	{
		ASSERT(0);
		return FALSE;
	}

	World* pWorld = NULL;
	TWorldMap mapWorld = pSection->GetWorldMap();

	pWorld = mapWorld.Peek(pRole->dwWorldID);
	if (!P_VALID(pWorld))
	{
		ASSERT(0);
		return FALSE;
	}

	pStream->SetSelect("skill", "*");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << pRole->dwOldID;

	// 进行操作
	QueryResult* pResult = pWorld->m_pWorldDB->m_DB.Query(pStream);
	
	if( P_VALID(pResult) )
	{	
		INT nCount = pResult->GetRowCount(); 
		if (nCount<=0)
		{
			return TRUE;
		}

		for (int i=0; i<nCount; i++)
		{
			pStream->SetInsert("skill");
			pStream->FillString("ID=")					<< (*pResult)[1].GetDword();
			pStream->FillString(",BiddenLevel=")		<< (*pResult)[2].GetInt();
			pStream->FillString(",SelfLevel=")			<< (*pResult)[3].GetInt();
			pStream->FillString(",Proficiency=")		<< (*pResult)[4].GetInt();
			pStream->FillString(",CoolDown =")			<< (*pResult)[5].GetInt();
			pStream->FillString(",RoleID=")				<< pRole->dwNewID;
			
			m_pdb2->m_DB.Execute(pStream);

			pResult->NextRow();

		}
	}

	// 释放Result
	m_pdb2->m_DB.FreeQueryResult(pResult);
	m_pdb2->m_DB.ReturnConnection(pCon);
	m_pdb2->m_DB.ReturnStream(pStream);

	return TRUE;
}

BOOL CTool::SaveTitle(const tagRole* pRole)
{
	// 获取一个Stream
	MyStream* pStream = m_pdb2->m_DB.GetStream();
	if( !P_VALID(pStream) )
		return FALSE;

	// 获取一条连接
	Connection* pCon = m_pdb2->m_DB.GetFreeConnection();

	Section* pSection = NULL;
	pSection = m_mapSection.Peek(pRole->dwSectionID);
	if (!P_VALID(pSection))
	{
		ASSERT(0);
		return FALSE;
	}

	World* pWorld = NULL;
	TWorldMap mapWorld = pSection->GetWorldMap();

	pWorld = mapWorld.Peek(pRole->dwWorldID);
	if (!P_VALID(pWorld))
	{
		ASSERT(0);
		return FALSE;
	}

	pStream->SetSelect("title", "*");
	pStream->SetWhere();
	pStream->FillString("RoleID=") << pRole->dwOldID;

	// 进行操作
	QueryResult* pResult = pWorld->m_pWorldDB->m_DB.Query(pStream);
	
	if( P_VALID(pResult))
	{
		INT nCount = pResult->GetRowCount(); 
		if (nCount<=0)
		{
			return TRUE;
		}

		for (int i=0; i<nCount; i++)
		{
			pStream->SetInsert("title");
			pStream->FillString("ID=")			<< (*pResult)[1].GetDword();
			pStream->FillString(",Count=")		<< (*pResult)[2].GetDword();
			pStream->FillString(",RoleID=")		<< pRole->dwNewID;

			m_pdb2->m_DB.Execute(pStream);

			pResult->NextRow();

		}
	}

	// 释放Result
	m_pdb2->m_DB.FreeQueryResult(pResult);
	m_pdb2->m_DB.ReturnConnection(pCon);
	m_pdb2->m_DB.ReturnStream(pStream);

	return TRUE;
}