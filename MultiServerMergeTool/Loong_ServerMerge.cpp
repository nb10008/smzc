#include "StdAfx.h"
#include "wx/wx.h"
#include "Resource.h"
#include "Loong_ServerMerge.h"
#include "Loong_LoginDBCtrl.h"
#include "Loong_LoongDBCtrl.h"
#include "Loong_OutputCtrl.h"
#include "Loong_TMethod.h"


CLoongServerMerge::CLoongServerMerge(void)
{
	m_bDataClean = FALSE;
	m_eMergeType = EM_MERGE_NULL;
	m_strSrc2WorldName = _T("");

	//保留多少天物品删除备份和装备删除备份
	m_nItemDelEquipDelSurviveDay = 30;

}

CLoongServerMerge::~CLoongServerMerge(void)
{	
	//关闭句柄
	std::list<HANDLE>::iterator itH = m_listSimpleThreadPool.begin();
	for(;itH != m_listSimpleThreadPool.end(); ++itH)
	{
		if(*itH != NULL)
		{
			TerminateThread(*itH,0);
			CloseHandle(*itH);
			*itH = 0;
		}
	}

	SAFE_CLOSE_HANDLE(m_hPoolMergeEvent);

	//销毁数据库
	sLoginBeton.Destroy();
	sLoongBeton.Destroy();

	m_clsThreadTaskP.Clear();
}
/*
/	初始化DB
/
*/
BOOL CLoongServerMerge::Init(tagConfigDetail	stConfigOne[EM_DB_TYPE_END],
							 tagConfigDetail	stConfigTwo[EM_DB_TYPE_END],
							 EDBMergeType		eMergeType,
							 BOOL				bDataClean,
							 tstring			strSrc2WorldName,
							 INT				nSurviveDay,
							 const std::multimap<ERoleCompensationType, tagCompensation*> mmCompensation)
{
	BOOL bRet = FALSE;

	//如果是跨服合并
	if(eMergeType == EM_MERGE_CROSS_SECTION)
	{
		//初始化Login
		bRet = sLoginBeton.Init(stConfigOne);
		if(bRet == FALSE)
		{
			return FALSE;
		}
	}
	//初始化Loong
	bRet = sLoongBeton.Init(stConfigTwo);
	if(bRet == FALSE)
	{
		return FALSE;
	}
	//合并类型
	m_bDataClean = bDataClean;
	m_eMergeType = eMergeType;
	m_strSrc2WorldName = strSrc2WorldName;

	//保留多少天物品删除备份和装备删除备份
	m_nItemDelEquipDelSurviveDay = nSurviveDay;

	//线程池合并同步
	m_hPoolMergeEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	//关闭句柄
	std::list<HANDLE>::iterator itH = m_listSimpleThreadPool.begin();
	for(;itH != m_listSimpleThreadPool.end(); ++itH)
	{
		if(*itH != NULL)
		{
			TerminateThread(*itH,0);
			CloseHandle(*itH);
			*itH = 0;
		}
	}

	for(INT32 nLoop = 0; nLoop<THREAD_POOL_NUM; nLoop++)
	{
		HANDLE thread_work_handle_ = (HANDLE)_beginthreadex(NULL, 0, (THREADPROC)&MyThreadPool, (LPVOID)this, 0, NULL);
		m_listSimpleThreadPool.push_back(thread_work_handle_);
	}

	//玩家补偿信息
	m_mmCompensation.insert(mmCompensation.begin(),mmCompensation.end());

	return TRUE;
}
/*
/	初始化合并逻辑
*/
BOOL CLoongServerMerge::LoongMergeLogicCtrlInit(CLoongMergeLogicCtrl &clsMergeLogicCtrl)
{	
	//合并类型
	clsMergeLogicCtrl.SetMergeType(m_eMergeType);

	//是否进行数据清理
	clsMergeLogicCtrl.SetIfCleanData(m_bDataClean);

	//源2的游戏世界名
	clsMergeLogicCtrl.SetSrc2WorldName(m_strSrc2WorldName);

	//生存时间
	clsMergeLogicCtrl.SetSurviveDay(m_nItemDelEquipDelSurviveDay);

	//如果是跨大区合并
	if(m_eMergeType == EM_MERGE_CROSS_SECTION)
	{
		//源1的account表最大num
		DWORD dwSrc1LoginMaxNum = sLoginBeton.GetAccountMaxNum(EM_DB_SRC_ONE);
		if(dwSrc1LoginMaxNum == GT_INVALID)
		{
			return FALSE;
		}
		clsMergeLogicCtrl.SetSrc1LoginMaxNum(dwSrc1LoginMaxNum);
		g_clsOutput.Output(_T("InitOK--LoginAccount!\r\n"));
	}

	//源1roledate表最大num
	DWORD dwSrc1RoleMaxNum = sLoongBeton.GetRoleDataMaxNum(EM_DB_SRC_ONE);
	if(dwSrc1RoleMaxNum == GT_INVALID)
	{
		return FALSE;
	}
	clsMergeLogicCtrl.SetSrc1RoleMaxNum(dwSrc1RoleMaxNum);
	g_clsOutput.Output(_T("InitOK--RoleMaxNum!\r\n"));

	//源1petdata表最大num
	DWORD dwSrc1PetMaxNum = sLoongBeton.GetPetDataMaxNum(EM_DB_SRC_ONE);
	if(dwSrc1PetMaxNum == GT_INVALID)
	{
		return FALSE;
	}

	// 第一次合数据时，dwSrc1PetMaxNum为0，应给其赋初值；否则合完的pet_id不在有效范围
	if (dwSrc1PetMaxNum == 0)
	{
		dwSrc1PetMaxNum = MIN_PET_ID;
	}
	clsMergeLogicCtrl.SetSrc1PetMaxNum(dwSrc1PetMaxNum);
	g_clsOutput.Output(_T("InitOK--PetMaxNum!\r\n"));

	//源1划拨物品最小序号
	INT64 n64ItemMinSerialBill = sLoongBeton.GetItemMinSerialBill(EM_DB_SRC_ONE);
	if(n64ItemMinSerialBill == GT_INVALID)
	{
		return FALSE;
	}
	clsMergeLogicCtrl.SetSrc1ItemMinSerialBill(n64ItemMinSerialBill);
	g_clsOutput.Output(_T("InitOK--ItemMinSerialBill!\r\n"));

	//源1内部物品的最大序号
	INT64 n64ItemMaxSerialInternal = sLoongBeton.GetMaxSerialInternal(EM_DB_SRC_ONE);
	if(n64ItemMaxSerialInternal == GT_INVALID)
	{
		return FALSE;
	}
	clsMergeLogicCtrl.SetSrc1ItemMaxSerialInternal(n64ItemMaxSerialInternal);
	g_clsOutput.Output(_T("InitOK--ItemMaxSerialInternal!\r\n"));

	//源1外部物品的最小序号
	INT64 n64ItemMinSerialOutter = sLoongBeton.GetMinSerialOutter(EM_DB_SRC_ONE);
	if(n64ItemMinSerialOutter == GT_INVALID)
	{
		return FALSE;
	}
	clsMergeLogicCtrl.SetSrc1ItemMinSerialOutter(n64ItemMinSerialOutter);
	g_clsOutput.Output(_T("InitOK--ItemMinSerialOutter!\r\n"));

	//初始化划拨相关
	std::set<string> &setData = clsMergeLogicCtrl.GetCANameSet();
	BOOL bRet = sLoongBeton.Loong_QueryCentralbillingLog(setData, EM_DB_SRC_ONE);
	if(bRet == FALSE)
	{
		return FALSE;
	}
	bRet = sLoongBeton.Loong_QueryCentralbillingLog(setData, EM_DB_SRC_TWO);
	if(bRet == FALSE)
	{
		return FALSE;
	}
	g_clsOutput.Output(_T("InitOK--CentralbillingLog!\r\n"));

	//初始化源1角色名字
	std::set<tstring> &setRoleDataName = clsMergeLogicCtrl.GetSrc1RoleName();
	bRet = sLoongBeton.Loong_QueryRoleDataName(setRoleDataName, EM_DB_SRC_ONE);
	if(bRet == FALSE)
	{
		return FALSE;
	}
	g_clsOutput.Output(_T("InitOK--RoleDataName!\r\n"));

	//初始化源1角色名字crc
	std::set<DWORD> &setRoleDataNameCrc = clsMergeLogicCtrl.GetSrc1RoleNameCrc();
	bRet = sLoongBeton.Loong_QueryRoleDataNameCrc(setRoleDataNameCrc, EM_DB_SRC_ONE);
	if(bRet == FALSE)
	{
		return FALSE;
	}
	g_clsOutput.Output(_T("InitOK--RoleDataNameCrc!\r\n"));

	// SRC1 神职角色
	std::set<DWORD> &setSrc1ClergyRoleID = clsMergeLogicCtrl.GetSrc1ClergyRoleID();
	bRet = sLoongBeton.Loong_QueryClergyRoleID(setSrc1ClergyRoleID, EM_DB_SRC_ONE);
	if(bRet == FALSE)
	{
		return FALSE;
	}
	// SRC2 神职角色
	std::set<DWORD> &setSrc2ClergyRoleID = clsMergeLogicCtrl.GetSrc2ClergyRoleID();
	bRet = sLoongBeton.Loong_QueryClergyRoleID(setSrc2ClergyRoleID, EM_DB_SRC_TWO);
	if(bRet == FALSE)
	{
		return FALSE;
	}
	g_clsOutput.Output(_T("InitOK--ClergyRoleID!\r\n"));

	// 注册线程处理函数
	m_clsThreadTaskP.Clear();
	// m_clsThreadTaskP.RegisterTaskProc(Loong_ThreadTaskDel);
	m_clsThreadTaskP.RegisterTaskProc(Loong_ThreadTaskItem);
	// m_clsThreadTaskP.RegisterTaskProc(Loong_ThreadTaskTask);

	return TRUE;
}

/*
/	换名字的补偿
*/
BOOL CLoongServerMerge::LoongRoleCompensationChangeRoleName(const tagRoleCompensation &st)
{
	//道具补偿
	std::multimap<ERoleCompensationType, tagCompensation*>::iterator itLoop =  m_mmCompensation.find(st.Type);

	if(itLoop != m_mmCompensation.end())
	{
		//寻找对应的补偿类型
		INT32 nCount = m_mmCompensation.count(st.Type);
		for(INT32 nLoop = 0; nLoop != nCount; ++nLoop,++itLoop)
		{
			//物品补偿
			INT64 n64SerialNum = m_clsMergeLogicCtrl.GetNewItemSerialOutter();
			if(typeid(*(itLoop->second)) == typeid(tagItemCompensation))
			{
				INT64 n64SerialNum = m_clsMergeLogicCtrl.GetNewItemSerialOutter();
				BOOL bRet = sLoongBeton.CreateItem(n64SerialNum,
													((tagItemCompensation*)(itLoop->second))->dwItemTypeID,
													st.dwAccountID,
													st.dwRoleID);
				if(bRet == FALSE)
				{
					return FALSE;
				}
			}
		}
	}
	return TRUE;
}

/*
/	玩家补偿
*/
BOOL CLoongServerMerge::LoongRoleCompensation()
{
	const std::vector<tagRoleCompensation> &vecRoleCompensation = m_clsMergeLogicCtrl.GetRoleCompensation();
	std::vector<tagRoleCompensation>::const_iterator itLoop =  vecRoleCompensation.begin();
	for(;itLoop != vecRoleCompensation.end(); ++itLoop)
	{
		switch((*itLoop).Type)
		{
		case ERC_CHANGE_ROLE_NAME:
			{
				BOOL bRet = LoongRoleCompensationChangeRoleName((*itLoop));
				if(bRet == FALSE)
				{
					return FALSE;
				}
			}
			break;
		default:
			break;
		}
	}

	return TRUE;
}

/*
/	简单线程池
*/
UINT MyThreadPool(LPVOID pParam)
{
	//异常保护
	EXCEPTION_PROTECT;

	CLoongServerMerge *pPara = (CLoongServerMerge*)pParam;

	WaitForSingleObject(pPara->m_hPoolMergeEvent, INFINITE);

	return pPara->m_clsThreadTaskP.Excute(pPara);
}

/*
/	开始合并
*/
BOOL CLoongServerMerge::ServerMerge()
{	
	//初始化合并逻辑控制

	BOOL bRet = LoongMergeLogicCtrlInit(m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("合并逻辑控制初始化失败!\r\n"));
		return FALSE;
	}

//---------------------------------------------------------------------------------------
//以下方法需要按顺序在合并时最先调用 
//---------------------------------------------------------------------------------------
	if(m_eMergeType == EM_MERGE_CROSS_SECTION)
	{
		g_clsOutput.Output(_T("开始合并--LoginAccount!\r\n"));
		tagTWrap_Longin_Account *TypeAccount = NULL;
		bRet = TMethod_Login_ServerMerge(TypeAccount,m_clsMergeLogicCtrl);
		if(bRet == FALSE)
		{
			g_clsOutput.Output(_T("合并失败--LoginAccount!\r\n"));
			return FALSE;
		}
		g_clsOutput.Output(_T("合并成功--LoginAccount!\r\n"));
	}
	
//---------------------------------------------------------------------------------------
	g_clsOutput.Output(_T("开始合并--AccountCommon!\r\n"));
	tagTWrap_Loong_AccountCommon *TypeAccountCommon = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeAccountCommon,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("合并失败--AccountCommon!\r\n"));
		return FALSE;
	}	
	g_clsOutput.Output(_T("合并成功--AccountCommon!\r\n"));
//---------------------------------------------------------------------------------------
	////帮派
	//g_clsOutput.Output(_T("开始合并--Guild!\r\n"));
	//tagTWrap_Loong_Guild *TypeGuild = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeGuild,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{	
	//	g_clsOutput.Output(_T("合并失败--Guild!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("合并成功--Guild!\r\n"));
//---------------------------------------------------------------------------------------
//FPlan 1.4.3

	//Family
	//g_clsOutput.Output(_T("开始合并--Family!\r\n"));
	//tagTWrap_Loong_Family *pTypeFamily = NULL;
	//bRet = TMethod_Loong_ServerMerge(pTypeFamily,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{
	//	g_clsOutput.Output(_T("合并失败--Family!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("合并成功--Family!\r\n"));

//---------------------------------------------------------------------------------------
	//操作role data
	g_clsOutput.Output(_T("开始合并--RoleData!\r\n"));
	tagTWrap_Loong_RoleData *TypeRoleData = NULL;
	bRet = TMethod_Loong_AccountServerMerge(TypeRoleData,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("合并失败--RoleData!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("合并成功--RoleData!\r\n"));

//---------------------------------------------------------------------------------------
//以上方法需要按顺序在合并时最先调用 且顺序不能变
//---------------------------------------------------------------------------------------

//大区内合并可以跳过login合并
if(m_eMergeType == EM_MERGE_INSIDE_SECTION)
	goto INSIDE_SECTION;
//---------------------------------------------------------------------------------------
//以下方法为跨大区时login库的合并

//	g_clsOutput.Output(_T("开始合并--LoginRoleName!\r\n"));
//	tagTWrap_Longin_RoleName *TypeRoleName = NULL;
//	bRet = TMethod_Login_ServerMerge(TypeRoleName,m_clsMergeLogicCtrl);
//	if(bRet == FALSE)
//	{
//		g_clsOutput.Output(_T("合并失败--LoginRoleName!\r\n"));
//		return FALSE;
//	}
//	g_clsOutput.Output(_T("合并成功--LoginRoleName!\r\n"));
////---------------------------------------------------------------------------------------
//	g_clsOutput.Output(_T("开始合并--LoginGuildName!\r\n"));
//	tagTWrap_Longin_GuildName *TypeGuildName = NULL;
//	bRet = TMethod_Login_ServerMerge(TypeGuildName,m_clsMergeLogicCtrl);
//	if(bRet == FALSE)
//	{
//		g_clsOutput.Output(_T("合并失败--LoginGuildName!\r\n"));
//		return FALSE;
//	}
//	g_clsOutput.Output(_T("合并成功--LoginGuildName!\r\n"));
//---------------------------------------------------------------------------------------
//	g_clsOutput.Output(_T("开始合并--LoginWorldForbid!\r\n"));
//	tagTWrap_Longin_WorldForbid *TypeWorldForbid = NULL;
//	bRet = TMethod_Login_ServerMerge(TypeWorldForbid,m_clsMergeLogicCtrl);
//	if(bRet == FALSE)
//	{
//		g_clsOutput.Output(_T("合并失败--LoginWorldForbid!\r\n"));
//		return FALSE;
//	}
//	g_clsOutput.Output(_T("合并成功--LoginWorldForbid!\r\n"));
////---------------------------------------------------------------------------------------
//	g_clsOutput.Output(_T("开始合并--LoginBlackList!\r\n"));
//	tagTWrap_Longin_BlackList *TypeLoginBlackList = NULL;
//	bRet = TMethod_Login_ServerMerge(TypeLoginBlackList,m_clsMergeLogicCtrl);
//	if(bRet == FALSE)
//	{
//		g_clsOutput.Output(_T("合并失败--LoginBlackList!\r\n"));
//		return FALSE;
//	}
//	g_clsOutput.Output(_T("合并成功--LoginBlackList!\r\n"));
//---------------------------------------------------------------------------------------
//	g_clsOutput.Output(_T("开始合并--LoginFatigueTime!\r\n"));
//	tagTWrap_Longin_FatigueTime *TypeLoginFatigueTime = NULL;
//	bRet = TMethod_Login_ServerMerge(TypeLoginFatigueTime,m_clsMergeLogicCtrl);
//	if(bRet == FALSE)
//	{
//		g_clsOutput.Output(_T("合并失败--LoginFatigueTime!\r\n"));
//		return FALSE;
//	}
//	g_clsOutput.Output(_T("合并成功--LoginFatigueTime!\r\n"));
////---------------------------------------------------------------------------------------
//	g_clsOutput.Output(_T("开始合并--LoginFamilyName!\r\n"));
//	tagTWrap_Longin_FamilyName *TypeFamilyName = NULL;
//	bRet = TMethod_Login_ServerMerge(TypeFamilyName,m_clsMergeLogicCtrl);
//	if(bRet == FALSE)
//	{
//		g_clsOutput.Output(_T("合并失败--LoginFamilyName!\r\n"));
//		return FALSE;
//	}
//	g_clsOutput.Output(_T("合并成功--LoginFamilyName!\r\n"));

//---------------------------------------------------------------------------------------
//以上方法为login库的表
//---------------------------------------------------------------------------------------

INSIDE_SECTION://大区内合并可以跳过login合并

//---------------------------------------------------------------------------------------
	//FPlan 1.4.7
	//GmServerData
	//g_clsOutput.Output(_T("开始合并--GmServerData!\r\n"));
	//tagTWrap_Loong_GmServerData *pGmServerData = NULL;
	//bRet = TMethod_Loong_ServerMerge(pGmServerData,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{
	//	g_clsOutput.Output(_T("合并失败--GmServerData!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("合并成功--GmServerData!\r\n"));

//---------------------------------------------------------------------------------------
//FPlan 1.4.3

	//---------------------------------------------------------------------------------------
	//family_member
	//g_clsOutput.Output(_T("开始合并--family_member!\r\n"));
	//tagTWrap_Loong_FamilyMem *pTypeFamilyMem = NULL;
	//bRet = TMethod_Loong_ServerMerge(pTypeFamilyMem,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{
	//	g_clsOutput.Output(_T("合并失败--family_member!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("合并成功--family_member!\r\n"));

	////---------------------------------------------------------------------------------------
	////family_quest
	//g_clsOutput.Output(_T("开始合并--family_quest!\r\n"));
	//tagTWrap_Loong_FamilyQuset *pTypeFamilyQuset = NULL;
	//bRet = TMethod_Loong_ServerMerge(pTypeFamilyQuset,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{
	//	g_clsOutput.Output(_T("合并失败--family_quest!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("合并成功--family_quest!\r\n"));

	////---------------------------------------------------------------------------------------
	////family_role_sprite
	//g_clsOutput.Output(_T("开始合并--family_role_sprite!\r\n"));
	//tagTWrap_Loong_FamilyRoleSprite *pTypeFamilyRoleSprite = NULL;
	//bRet = TMethod_Loong_ServerMerge(pTypeFamilyRoleSprite,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{
	//	g_clsOutput.Output(_T("合并失败--family_role_sprite!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("合并成功--family_role_sprite!\r\n"));

	////---------------------------------------------------------------------------------------
	////family_sprite
	//g_clsOutput.Output(_T("开始合并--family_sprite!\r\n"));
	//tagTWrap_Loong_FamilySprite *pTypeFamilySprite = NULL;
	//bRet = TMethod_Loong_ServerMerge(pTypeFamilySprite,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{
	//	g_clsOutput.Output(_T("合并失败--family_sprite!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("合并成功--family_sprite!\r\n"));

	////---------------------------------------------------------------------------------------
	////family_sprite_requirement
	//g_clsOutput.Output(_T("开始合并--family_sprite_requirement!\r\n"));
	//tagTWrap_Loong_SpriteRequirement *pTypeSpriteRequirement = NULL;
	//bRet = TMethod_Loong_ServerMerge(pTypeSpriteRequirement,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{
	//	g_clsOutput.Output(_T("合并失败--family_sprite_requirement!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("合并成功--family_sprite_requirement!\r\n"));

//---------------------------------------------------------------------------------------
//FPlan 1.4.0
	//领域
	g_clsOutput.Output(_T("开始合并--Area!\r\n"));
	tagTWrap_Sheng_Area *TypeArea = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeArea,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{	
		g_clsOutput.Output(_T("合并失败--Area!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("合并成功--Area!\r\n"));

	//稀有领域
	//g_clsOutput.Output(_T("开始合并--RareArea!\r\n"));
	//tagTWrap_Sheng_RareArea *TypeRareArea = NULL;
	//bRet = TMethod_Loong_PagingServerMerge(TypeRareArea,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{	
	//	g_clsOutput.Output(_T("合并失败--RareArea!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("合并成功--RareArea!\r\n"));


//---------------------------------------------------------------------------------------
//FPlan 1.3.6
	//衣橱
	g_clsOutput.Output(_T("开始合并--Wardrobe!\r\n"));
	tagTWrap_Sheng_Wardrobe *TypeWardrobe = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeWardrobe,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{	
		g_clsOutput.Output(_T("合并失败--Wardrobe!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("合并成功--Wardrobe!\r\n"));

//---------------------------------------------------------------------------------------
//FPlan1.3.2
	//砸蛋
	//g_clsOutput.Output(_T("开始合并--EggInfo!\r\n"));
	//tagTWrap_Sheng_EggInfo *TypeEggInfo = NULL;
	//bRet = TMethod_Loong_PagingServerMerge(TypeEggInfo,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{	
	//	g_clsOutput.Output(_T("合并失败--EggInfo!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("合并成功--EggInfo!\r\n"));


//---------------------------------------------------------------------------------------
//FPlan1.3.0
	//神之竞标GodBid
	//g_clsOutput.Output(_T("开始合并--GodBid!\r\n"));
	//tagTWrap_Sheng_GodBid *TypeGodBid = NULL;
	//bRet = TMethod_Loong_PagingServerMerge(TypeGodBid,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{	
	//	g_clsOutput.Output(_T("合并失败--GodBid!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("合并成功--GodBid!\r\n"));

	
	//activity
	//g_clsOutput.Output(_T("开始合并--activity!\r\n"));
	//tagTWrap_Loong_ActivityData *TypeActivityData = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeActivityData,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{
	//g_clsOutput.Output(_T("合并失败--activity!\r\n"));
	//return FALSE;
	//}
	//g_clsOutput.Output(_T("合并成功--activity!\r\n"));

//---------------------------------------------------------------------------------------
//FPlan1.2.1

	//妖精重生数据tagTWrap_Sheng_FabaoNirVanaData
	g_clsOutput.Output(_T("开始合并--FabaoNirVanaData!\r\n"));
	tagTWrap_Sheng_FabaoNirVanaData *TypeFabaoNirVanaData = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeFabaoNirVanaData,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{	
		g_clsOutput.Output(_T("合并失败--FabaoNirVanaData!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("合并成功--FabaoNirVanaData!\r\n"));

	//玩家在线时间统计tagTWrap_Sheng_CurMonthOnlineTime
	//g_clsOutput.Output(_T("开始合并--CurMonthOnlineTime!\r\n"));
	//tagTWrap_Sheng_CurMonthOnlineTime *TypeCurMonthOnlineTime = NULL;
	//bRet = TMethod_Loong_PagingServerMerge(TypeCurMonthOnlineTime,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{	
	//	g_clsOutput.Output(_T("合并失败--CurMonthOnlineTime!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("合并成功--CurMonthOnlineTime!\r\n"));

	//tagTWrap_Sheng_GuildWarDeclare
	//g_clsOutput.Output(_T("开始合并--GuildWarDeclare!\r\n"));
	//tagTWrap_Sheng_GuildWarDeclare *TypeGuildWarDeclare = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeGuildWarDeclare,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{	
	//	g_clsOutput.Output(_T("合并失败--GuildWarDeclare!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("合并成功--GuildWarDeclare!\r\n"));


	//tagTWrap_Sheng_BoardQuestSaveData
	//g_clsOutput.Output(_T("开始合并--BoardQuestSaveData!\r\n"));
	//tagTWrap_Sheng_BoardQuestSaveData *TypeBoardQuestSaveData = NULL;
	//bRet = TMethod_Loong_PagingServerMerge(TypeBoardQuestSaveData,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{	
	//	g_clsOutput.Output(_T("合并失败--BoardQuestSaveData!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("合并成功--BoardQuestSaveData!\r\n"));



//---------------------------------------------------------------------------------------
//2.2.1-新增表

	//tagTWrap_Loong_ForceBreakOutApprentice
	//g_clsOutput.Output(_T("开始合并--ForceBreakOutApprentice!\r\n"));
	//tagTWrap_Loong_ForceBreakOutApprentice *TypeForceBreakOutApprentice = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeForceBreakOutApprentice,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{	
	//	g_clsOutput.Output(_T("合并失败--ForceBreakOutApprentice!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("合并成功--ForceBreakOutApprentice!\r\n"));

//---------------------------------------------------------------------------------------
//2.0.0-新增表

	//fabao
	g_clsOutput.Output(_T("开始合并--fabao!\r\n"));
	tagTWrap_Loong_Fabao *TypeFabao = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeFabao,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{	
		g_clsOutput.Output(_T("合并失败--fabao!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("合并成功--fabao!\r\n"));


	// train_state
	//g_clsOutput.Output(_T("开始合并--train_state!\r\n"));
	//tagTWrap_Loong_TrainState *TypeTrainState = NULL;
	//bRet = TMethod_Loong_PagingServerMerge(TypeTrainState,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{	
	//	g_clsOutput.Output(_T("合并失败--train_state!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("合并成功--train_state!\r\n"));

	//ActiveRankData
	//g_clsOutput.Output(_T("开始合并--ActiveRankData!\r\n"));
	//tagTWrap_Loong_ActiveRankData *TypeActiveRankData = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeActiveRankData,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{	
	//	g_clsOutput.Output(_T("合并失败--ActiveRankData!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("合并成功--ActiveRankData!\r\n"));

//---------------------------------------------------------------------------------------
//1.3.2-新增表

	//Master
	//g_clsOutput.Output(_T("开始合并--Master!\r\n"));
	//tagTWrap_Loong_Master *TypeMaster = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeMaster,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{	
	//	g_clsOutput.Output(_T("合并失败--Master!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("合并成功--Master!\r\n"));

	//shortcut_quickbar
	g_clsOutput.Output(_T("开始合并--shortcut_quickbar!\r\n"));
	tagTWrap_Loong_ShortCutBar *TypeShortCutBar = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeShortCutBar,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{	
		g_clsOutput.Output(_T("合并失败--shortcut_quickbar!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("合并成功--shortcut_quickbar!\r\n"));


//1.3.2-新增表
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
//1.3.1-新增表
/*
	//ExternalLink
	g_clsOutput.Output(_T("开始合并--ExternalLink!\r\n"));
	tagTWrap_Loong_ExternalLink *TypeExternalLink = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeExternalLink,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("合并失败--ExternalLink!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("合并成功--ExternalLink!\r\n"));		
*/
	//NameHistory
/*	g_clsOutput.Output(_T("开始合并--NameHistory!\r\n"));
	tagTWrap_Loong_NameHistory *TypeNameHistory = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeNameHistory,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("合并失败--NameHistory!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("合并成功--NameHistory!\r\n"));	*/	


	//City
	//g_clsOutput.Output(_T("开始合并--City!\r\n"));
	//tagTWrap_Loong_City *TypeCity = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeCity,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{
	//	g_clsOutput.Output(_T("合并失败--City!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("合并成功--City!\r\n"));	

//---------------------------------------------------------------------------------------
	//ActClanTreasure
	//g_clsOutput.Output(_T("开始合并--ActClanTreasure!\r\n"));
	//tagTWrap_Loong_ActClanTreasure *TypeActClanTreasure = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeActClanTreasure,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{
	//	g_clsOutput.Output(_T("合并失败--ActClanTreasure!\r\n"));
	//	return FALSE;
	//}	
	//g_clsOutput.Output(_T("合并成功--ActClanTreasure!\r\n"));
//---------------------------------------------------------------------------------------
	//g_clsOutput.Output(_T("开始合并--RepRstTime!\r\n"));
	//tagTWrap_Loong_RepRstTime *TypeRepRstTime = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeRepRstTime,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{
	//	g_clsOutput.Output(_T("合并失败--RepRstTime!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("合并成功--RepRstTime!\r\n"));
//---------------------------------------------------------------------------------------
	//g_clsOutput.Output(_T("开始合并--FameHallEnterSnap!\r\n"));
	//tagTWrap_Loong_FameHallEnterSnap *TypeFameHallEnterSnap = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeFameHallEnterSnap,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{
	//	g_clsOutput.Output(_T("合并失败--FameHallEnterSnap!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("合并成功--FameHallEnterSnap!\r\n"));
//---------------------------------------------------------------------------------------
	//yuanbaoorder
	//g_clsOutput.Output(_T("开始合并--yuanbaoorder!\r\n"));
	//tagTWrap_Loong_YuanBaoOrder *TypeYuanBaoOrder = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeYuanBaoOrder,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{
	//	g_clsOutput.Output(_T("合并失败--yuanbaoorder!\r\n"));
	//	return FALSE;
	//}	
	//g_clsOutput.Output(_T("合并成功--yuanbaoorder!\r\n"));
//---------------------------------------------------------------------------------------
	//vip_stall
/*	g_clsOutput.Output(_T("开始合并--VipStall!\r\n"));
	tagTWrap_Loong_VipStall *TypeVipStall = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeVipStall,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("合并失败--VipStall!\r\n"));
		return FALSE;
	}	
	g_clsOutput.Output(_T("合并成功--VipStall!\r\n"))*/;
//---------------------------------------------------------------------------------------
	//pet_skill
	g_clsOutput.Output(_T("开始合并--PetSkill!\r\n"));
	tagTWrap_Loong_PetSkill *TypePetSkill = NULL;
	bRet = TMethod_Loong_ServerMerge(TypePetSkill,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("合并失败--PetSkill!\r\n"));
		return FALSE;
	}	
	g_clsOutput.Output(_T("合并成功--PetSkill!\r\n"));
//---------------------------------------------------------------------------------------
	//pet_data
	g_clsOutput.Output(_T("开始合并--PetData!\r\n"));
	tagTWrap_Loong_PetData *TypePetData = NULL;
	bRet = TMethod_Loong_ServerMerge(TypePetData,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("合并失败--PetData!\r\n"));
		return FALSE;
	}	
	g_clsOutput.Output(_T("合并成功--PetData!\r\n"));
//---------------------------------------------------------------------------------------
	//BlackList
	//g_clsOutput.Output(_T("开始合并--BlackList!\r\n"));
	//tagTWrap_Loong_BlackList *TypeBlackList = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeBlackList,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{	
	//	g_clsOutput.Output(_T("合并失败--BlackList!\r\n"));
	//	return FALSE;
	//}	
	//g_clsOutput.Output(_T("合并成功--BlackList!\r\n"));
//---------------------------------------------------------------------------------------
	//left msg
	//g_clsOutput.Output(_T("开始合并--left_msg!\r\n"));
	//tagTWrap_Loong_LeftMsg *TypeLeftMsg = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeLeftMsg,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{
	//	g_clsOutput.Output(_T("合并失败--left_msg!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("合并成功--left_msg!\r\n"));
//---------------------------------------------------------------------------------------
	//buff
	g_clsOutput.Output(_T("开始合并--buff!\r\n"));
	tagTWrap_Loong_Buff *TypeBuff = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeBuff,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("合并失败--buff!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("合并成功--buff!\r\n"));	

//---------------------------------------------------------------------------------------
	//clan_data
/*	g_clsOutput.Output(_T("开始合并--clan_data!\r\n"));
	tagTWrap_Loong_ClanData *TypeClanDate = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeClanDate,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{	
		g_clsOutput.Output(_T("合并失败--clan_data!\r\n"));
		return FALSE;
	}	
	g_clsOutput.Output(_T("合并成功--clan_data!\r\n"));*/	
//---------------------------------------------------------------------------------------
	//enemy
	//g_clsOutput.Output(_T("开始合并--enemy!\r\n"));
	//tagTWrap_Loong_Enemy *TypeEnemy = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeEnemy,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{	
	//	g_clsOutput.Output(_T("合并失败--enemy!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("合并成功--enemy!\r\n"));
//---------------------------------------------------------------------------------------
	//friend
	//g_clsOutput.Output(_T("开始合并--Friend!\r\n"));
	//tagTWrap_Loong_Friend *TypeFriend = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeFriend,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{	
	//	g_clsOutput.Output(_T("合并失败--Friend!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("合并成功--Friend!\r\n"));

//---------------------------------------------------------------------------------------
	//friendship
	//g_clsOutput.Output(_T("开始合并--FriendShip!\r\n"));
	//tagTWrap_Loong_FriendshipSave *TypeFriendShip = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeFriendShip,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{	
	//	g_clsOutput.Output(_T("合并FriendShip失败!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("合并成功--FriendShip!\r\n"));

//---------------------------------------------------------------------------------------
	//GroupPurchase
	//g_clsOutput.Output(_T("开始合并--GroupPuichase!\r\n"));
	//tagTWrap_Loong_GroupPurchase *TypeGroupPurchase = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeGroupPurchase,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{
	//	g_clsOutput.Output(_T("合并GroupPuichase失败!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("合并成功--GroupPuichase!\r\n"));

//---------------------------------------------------------------------------------------
	//GuildMem
	//g_clsOutput.Output(_T("开始合并--GuildMem!\r\n"));
	//tagTWrap_Loong_GuildMem *TypeGuildMem = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeGuildMem,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{
	//	g_clsOutput.Output(_T("合并失败--GuildMem!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("合并成功--GuildMem!\r\n"));

//---------------------------------------------------------------------------------------
	//item_cdtime
	g_clsOutput.Output(_T("开始合并--ItemCDTime!\r\n"));
	tagTWrap_Loong_ItemCDTime *TypeItemCDTime = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeItemCDTime,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{	
		g_clsOutput.Output(_T("合并失败--ItemCDTime!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("合并成功--ItemCDTime!\r\n"));

//---------------------------------------------------------------------------------------
	//item_needlog
	//g_clsOutput.Output(_T("开始合并--ItemNeedLog!\r\n"));
	//tagTWrap_Loong_ItemNeedLog *TypeItemNeedLog = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeItemNeedLog,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{
	//	g_clsOutput.Output(_T("合并失败--ItemNeedLog!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("合并成功--ItemNeedLog!\r\n"));
//---------------------------------------------------------------------------------------
	//log_baibao
	g_clsOutput.Output(_T("开始合并--log_baibao!\r\n"));
	tagTWrap_Loong_LogBaiBao *TypeLogBaiBao = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeLogBaiBao,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{	
		g_clsOutput.Output(_T("合并失败--log_baibao!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("合并成功--log_baibao!\r\n"));

//---------------------------------------------------------------------------------------
	//role_del
	//g_clsOutput.Output(_T("开始合并--RoleDel!\r\n"));
	//tagTWrap_Loong_RoleDel *TypeRoleDel = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeRoleDel,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{	
	//	g_clsOutput.Output(_T("合并失败--RoleDel!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("合并成功--RoleDel!\r\n"));

//---------------------------------------------------------------------------------------
	//skill
	g_clsOutput.Output(_T("开始合并--Skill!\r\n"));
	tagTWrap_Loong_Skill *TypeSkill = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeSkill,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("合并失败--Skill!\r\n"));
		return FALSE;
	}		
	g_clsOutput.Output(_T("合并成功--Skill!\r\n"));

//---------------------------------------------------------------------------------------
	//title
	g_clsOutput.Output(_T("开始合并--Title!\r\n"));
	tagTWrap_Loong_Title *TypeTitle = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeTitle,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("合并失败--Title!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("合并成功--Title!\r\n"));

//---------------------------------------------------------------------------------------
	//名片
	g_clsOutput.Output(_T("开始合并--VisitingCard!\r\n"));
	tagTWrap_Loong_VisitingCard *TypeVisitingCard = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeVisitingCard,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("合并失败--VisitingCard!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("合并成功--VisitingCard!\r\n"));

//---------------------------------------------------------------------------------------
	//元宝账号
	//g_clsOutput.Output(_T("开始合并--FameYBAccount!\r\n"));
	//tagTWrap_Loong_YBAccount *TypeYBAccount = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeYBAccount,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{
	//	g_clsOutput.Output(_T("合并失败--FameYBAccount!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("合并成功--FameYBAccount!\r\n"));

//---------------------------------------------------------------------------------------
	//guild_commodity
	//g_clsOutput.Output(_T("开始合并--guild_commodity!\r\n"));
	//tagTWrap_Loong_GuildCommodity *TypeGuildCommodity = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeGuildCommodity,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{
	//	g_clsOutput.Output(_T("合并失败--guild_commodity!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("合并成功--guild_commodity!\r\n"));

//---------------------------------------------------------------------------------------
	//guild_skill
	//g_clsOutput.Output(_T("开始合并--guild_skill!\r\n"));
	//tagTWrap_Loong_GuildSkill *TypeGuildSkill = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeGuildSkill,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{
	//	g_clsOutput.Output(_T("合并失败--guild_skill!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("合并成功--guild_skill!\r\n"));

//---------------------------------------------------------------------------------------
	//vip_netbar
	//g_clsOutput.Output(_T("开始合并--vip_netbar!\r\n"));
	//tagTWrap_Loong_VipNetBar *TypeVipNetBar = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeVipNetBar,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{
	//	g_clsOutput.Output(_T("合并失败--vip_netbar!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("合并成功--vip_netbar!\r\n"));

//---------------------------------------------------------------------------------------
	//guild_upgrade
	//g_clsOutput.Output(_T("开始合并--guild_upgrade!\r\n"));
	//tagTWrap_Loong_GuildFacilities *TypeGuildFacilities = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeGuildFacilities,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{
	//	g_clsOutput.Output(_T("合并失败--guild_upgrade!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("合并成功--guild_upgrade!\r\n"));	

//---------------------------------------------------------------------------------------
	//commerce_rank
/*	g_clsOutput.Output(_T("开始合并--commerce_rank!\r\n"));
	tagTWrap_Loong_CommerceRank *TypeCommerceRank = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeCommerceRank,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("合并失败--commerce_rank!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("合并成功--commerce_rank!\r\n"));*/	

	// F-Plan 1.5.0 新增表 soulcrystal
	g_clsOutput.Output(_T("开始合并--soulcrystal!\r\n"));
	tagTWrap_Loong_SoulCrystal *TypeSoulCrystal = NULL;
	bRet = TMethod_Loong_ServerMerge(TypeSoulCrystal,m_clsMergeLogicCtrl);
	if(bRet == FALSE)
	{
		g_clsOutput.Output(_T("合并失败--soulcrystal!\r\n"));
		return FALSE;
	}
	g_clsOutput.Output(_T("合并成功--soulcrystal!\r\n"));

	// F-Plan 1.5.0 新增表 Account_Reactive
	//g_clsOutput.Output(_T("开始合并--account_reactive!\r\n"));
	//tagTWrap_Loong_AccountReactive *TypeAccountReactive = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeAccountReactive,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{	
	//	g_clsOutput.Output(_T("合并失败--account_reactive!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("合并成功--account_reactive!\r\n"));

	// F-Plan 1.5.0 新增表 Account_Reactiver
	//g_clsOutput.Output(_T("开始合并--account_reactiver!\r\n"));
	//tagTWrap_Loong_AccountReactiver *TypeAccountReactiver = NULL;
	//bRet = TMethod_Loong_ServerMerge(TypeAccountReactiver,m_clsMergeLogicCtrl);
	//if(bRet == FALSE)
	//{	
	//	g_clsOutput.Output(_T("合并失败--account_reactiver!\r\n"));
	//	return FALSE;
	//}
	//g_clsOutput.Output(_T("合并成功--account_reactiver!\r\n"));

//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
//以下是数据量很大的几个表 采用线程池合并
//---------------------------------------------------------------------------------------

	//开启线程池合并
	SetEvent(m_hPoolMergeEvent);

	//等待线程池中所有线程
	EThreadPoolState eThreadState = this->LoongWaitThreadTask();

	//清空逻辑
	m_clsMergeLogicCtrl.ClearMemory();

	//删除掉稀有领域相关的内容
	if(eThreadState == ETPS_SUCCESS)
	{
		if( !sLoongBeton.DeleteRareareaData(EM_DB_SRC_ONE) )
		{
			eThreadState == ETPS_FAILURE;
		}
	}

	if(eThreadState == ETPS_SUCCESS)
	{
		g_clsOutput.Output(_T("成功!\r\n"));
	}
	else if(eThreadState == ETPS_FAILURE)
	{
		//关闭句柄
		std::list<HANDLE>::iterator itH = m_listSimpleThreadPool.begin();
		for(;itH != m_listSimpleThreadPool.end(); ++itH)
		{
			if(*itH != NULL)
			{
				TerminateThread(*itH,0);
				CloseHandle(*itH);
				*itH = 0;
			}
		}
		g_clsOutput.Output(_T("失败 请重启工具 重新合并!\r\n"));
	}

	return TRUE;
}

/*
/	等待线程
*/
EThreadPoolState CLoongServerMerge::LoongWaitThreadTask()
{
	EThreadPoolState e = ETPS_NULL;
	while(TRUE)
	{
		e = GetThreadPoolState();
		if(e == ETPS_STILL_HAVE_ACTIVE)
		{
			Sleep(500); //0.5秒
			continue;
		}
		else if(e == ETPS_FAILURE)
		{
			break;
		}
		else if(e == ETPS_SUCCESS)
		{
			break;
		}
	}

	//如果合服成功 给予玩家相应的补偿
	//if(e == ETPS_SUCCESS)
	//{
	//	g_clsOutput.Output(_T("开始玩家补偿!\r\n"));
	//	BOOL bRet = LoongRoleCompensation();
	//	if(bRet  == FALSE)
	//	{
	//		g_clsOutput.Output(_T("玩家补偿失败!\r\n"));
	//		e = ETPS_FAILURE;
	//		goto END_MERGE;
	//	}	
	//	g_clsOutput.Output(_T("玩家补偿成功!\r\n"));
	//}

	//结束合并
END_MERGE:

	return e;
}


/*
/	线程池内是否还有线程活着
*/
EThreadPoolState CLoongServerMerge::GetThreadPoolState()
{
	std::list<HANDLE>::iterator itLoop = m_listSimpleThreadPool.begin();

	EThreadPoolState eTPState = ETPS_SUCCESS;

	for(;itLoop != m_listSimpleThreadPool.end(); ++itLoop)
	{
		HANDLE hHandle = *itLoop;
		if( hHandle )
		{
			DWORD dwConnectExitCode = STILL_ACTIVE;
			GetExitCodeThread(hHandle, &dwConnectExitCode);
			//失败
			if(FALSE == dwConnectExitCode)
				return ETPS_FAILURE;
			else if( STILL_ACTIVE == dwConnectExitCode )
			{
				eTPState = ETPS_STILL_HAVE_ACTIVE;
			}
		}
	}

	return eTPState;
}