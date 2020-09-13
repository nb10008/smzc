//-----------------------------------------------------------------------------
//!\file World.cpp
//!\author Aslan
//!
//!\date 2008-06-10
//! last 2008-06-10
//!
//!\brief 游戏世界管理器
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#include "StdAfx.h"

#include "../CA/CAClient.h"
#include "../WorldDefine/svn_revision.h"
#include "../ServerDefine/role_data.h"
#include "vip_netbar.h"
#include "world.h"
#include "world_session.h"
#include "login_session.h"
#include "db_session.h"
#include "bill_session.h"
#include "rt_session.h"
#include "map_creator.h"
#include "map_creator.h"
#include "role.h"
#include "creature.h"
#include "role_mgr.h"
#include "att_res.h"
#include "quest_mgr.h"
#include "script_mgr.h"
#include "loot_mgr.h"
#include "social_mgr.h"
#include "group_mgr.h"
#include "buff_effect.h"
#include "world_net_cmd_mgr.h"
#include "mall.h"
#include "TradeYuanBao.h"
#include "game_guarder.h"
#include "famehall.h"
#include "vip_stall.h"
#include "register_test_func.h"
#include "activity_mgr.h"
#include "ps_bomb.h"
#include "ps_ipchecker.h"
#include "guild_mgr.h"
#include "MsgInfoMgr.h"
#include "game_guarder.h"
#include "gm_policy.h"
#include "NPCTeam_mgr.h"
#include "TreasureChest_mgr.h"
#include "TimeChecker.h"
#include "WorldRankings.h"
#include "ClergyRank.h"
#include "../WorldDefine/athletics_define.h"
#include "../WorldDefine/msg_athletics_system.h"
#include "athletics_room.h"
#include "athletics_room_mgr.h"
#include "athletics_system.h"

#include "../WorldDefine/msg_VoteShow_define.h"
#include "../WorldDefine/role_att.h"
#include "../ServerDefine/msg_draft_show.h"
#include "draft_show.h"

#include "marriage.h"
#include "city_mgr.h"
#include "city_struggle.h"
#include "chat_logger.h"
#include "netcmd_viewer.h"

// Jason 2010-1-19 v1.3.2
#include "player_regression.h"

#include "pet_define.h"

#include "offline_reward.h"
#include "online_reward.h"
// Jason 2010-3-30
#include "title_mgr.h"
#include "map_mgr.h"
#include "..\ServerDefine\family_define.h"

#include "GuildBattle.h"
#include "force_breakout_apprentice_mgr.h"
#include "guild_war_declare_mgr.h"
#include "keycode_bridge_session.h"
// 便捷组队，Jason, 2010-9-26
#include "convenient_team.h"
// 友好关闭服务器 Jason 2010-10-8
#include "shutdown_server.h"
#include "fbbs_session.h"
#include "godbid_mgr.h"
#include "../ServerDefine/role_data.h"
#include "fairy_birth.h"
#include "shielding_mgr.h"
#include "../serverdefine/protocol_counter.h"
#include "xtrap_guarder.h"
#include "global_limited_item.h"
#include "family_join.h"
#include "family_mgr.h"
#include "family_sprite_mgr.h"
#include "TigerMgr.h"
#include "..\ServerDefine\msg_yuanbao.h"

#include "guild_apply_to_join.h"

#include <iphlpapi.h>
#include <NtDDNdis.h>
#include <winioctl.h>
#include "data_collect.h"
//#include "ipg_session.h"
//#include "ipg_session_cash.h"
#include "IPGDragonfly.h"
#ifdef ON_INTERNET_CAFE
#include "internet_cafes_session.h"
#endif

#ifdef ON_GAMEGUARD
#include "../GameGuard/ggsrv30.h"
#endif

#pragma comment(lib, "IPHlpAPI.lib")

// Jason 2010-1-29 v1.3.2 初始化功能线程化，并去掉相关变量的外部链接属性
static LONG volatile g_InitFlag = 1;
static HANDLE g_hIniThread = NULL;

static DWORD WINAPI ClearAllGuildState (LPCTSTR szMask);
static VOID RegisterClearGuildState();
// 2010-4-26 v2.1.0
static INT nRunawayTickCount = TICK_PER_SECOND * 20;
static VOID RegisterNPIR(VOID);
static INT	sg_nMaxTickValve = 300;
static VOID RegisterResetMaxTickValve(VOID);
static VOID LogMaxTickAlert(INT value);
static BOOL bSoaringSwitch = FALSE;
static VOID SetCurTickOfWord(DWORD t);
static DWORD GetCurTickOfWord(VOID);
static INT nMaxRoleLevel = 0;
static INT nDistance = 0;
static BOOL bTianShenZhongOpen = FALSE;
static BOOL bQiHuaZhiHaiOpen = FALSE;
static BOOL bBenLeiLieGuOpen = FALSE;
static BOOL bKillCounterOpen = FALSE;
static LONG VOLATILE s_lFairyContractSwitch=FALSE;
// 神铸相关参数
static BOOL bGodStrengthOpen = FALSE;
static INT	nGodStengthEquipMinQlty = 5;
static DWORD dwGodStrengthCoolDown = 24*60*60;
static INT nGodStrengthAssitEquipMinQlty = 2;
static INT nGodStrengthAssitEquipMinLevel = 70;

static VOID RegisterEasyTeamSwitch(VOID);
static VOID RegisterShutdownServer(VOID);
static INT nFabaoLevelLimit = 100;

static VOID RegisterDBReconnectFunc(VOID);
static DWORD WINAPI ReconnectDBWithNewTransObj(LPCTSTR szValue);

// 妖精资质、食物、元素伤害、特技等
static VOID RegisterFabaoFoodQualifFunc(VOID);
static DWORD WINAPI FabaoFoodQuali(LPCTSTR szValue);
static VOID RegisterFabaoElemStuntFunc(VOID);
static DWORD WINAPI FabaoElemStunf(LPCTSTR szValue);
// 妖精资质以及食物开关
static VOLATILE LONG lFabaoFoodQualifFlag = FALSE;
// 妖精元素伤害以及特技开关
static VOLATILE LONG lFabaoElemStuntFlag = FALSE;

static VOID RegisterReloadItemNeedlog(VOID);
static BOOL bUseConfigWANIP = FALSE;
static VOLATILE BOOL bOpenFairyMating = FALSE;
static VOID RegisterOpenFabaoMating(VOID);
static VOLATILE BOOL bOpenShielding = FALSE;
static VOLATILE BOOL bOpenEggBroker = FALSE;
static VOID RegisterShielding(VOID);
static BOOL bPlayerBack = FALSE;
static tagDWORDTime dwOldSerNewPlayerDate = 0;
static DWORD dwOldSerNewPlayerMaxLevel = 0;

static ProtocolCounter _protocol_counter;	// 协议计数器
static VOLATILE LONG lLoadProtocolIDFile = 0; // 是否及时读取协议id文件
static VOLATILE LONG lCountProtocolSwitch = 1;
#define DUMP_NETCMDCOUNTER_TICk	TICK_PER_SECOND * 60 * 30
static DWORD	dwDumpNetCmdCounterTicks = DUMP_NETCMDCOUNTER_TICk;
static VOID RegisterMsgCounter(VOID);
static LPCSTR strNetCmgIdFileName = "server_config\\netcmdid.txt";
static LPCSTR GetDumpNetCmdCountersFileName(VOID);

static VOID RegisterTigerInfo(VOID);
static DWORD WINAPI TigerInfo();
// 限制pk等级开关以及最小等级
static LONG lPKConstraintsOpen = 0;
static INT  nPKConstraintsMinLevel = 30;
static INT  nDeductMorality = 30;
static INT  nMoralityTime = 60;
static INT  nReturnMorality = 1;

static VOLATILE LONG	lGodSystemSwitch = 0;

static INT nLoginTimeOutTicks = 50;			// 设置超时时间为300个Tick，也就是一分钟，一分钟之后没有收到客户端登陆，就返回Login超时

static CHAR s_szMac[64] = {0};
static CHAR s_szIP[64] = {0};
static CHAR s_szMD5[48] = {0};
static CHAR s_szSign[32] = {0};

static BOOL	bUseIPGSystem = FALSE;

static CHAR s_szIPGIP[20] = {0};
static DWORD s_dwGetCashPort = 0;
static DWORD s_dwIOCashPort = 0;

//元神开关
static bool bHolySoulOn = false;

//冲星强化
static bool bStarEnhance = false;

static CHAR s_szSignArray[40][32] = 
{
	"44947036854656676189",
	"40131761527960881991",
	"31200905561953051798",
	"34657963963594008356",
	"03268110008857406933",
	"26959836914235902941",
	"12972596881894038635",
	"82266789004683433586",
	"17078043318814708595",
	"21520970608461636208",
	"91921016628460615405",
	"20807652089140293912",
	"99471621531714997250",
	"31133826970386335671",
	"75758914877425228417",
	"23203874381301615991",
	"17766844464084232132",
	"26856521471250182879",
	"76780880889459004617",
	"72917297023107818960",
	"78845287483350158915",
	"12860251788787545561",
	"59840150459330989540",
	"44876405613051846438",
	"04536827852986239181",
	"17387579721834035799",
	"56610623713988754761",
	"46944312166173206193",
	"99816697259980638866",
	"88128358985210513753",
	"96568823646809417259",
	"17524898584165127800",
	"98960114394645457596",
	"04219659123100364911",
	"11247404074704893617",
	"44966065362962579930",
	"74144696322990687911",
	"84194341194100679452",
	"42496013020084748398",
	"80259086261571474831"
};
//-----------------------------------------------------------------------------
// construction
//-----------------------------------------------------------------------------
World::World()
{
	m_bTerminateUpdate = FALSE;
	m_bShutingdown = FALSE;
	m_bOverloadAlert = FALSE;
	m_dwMaxTickTime = 0;

	m_dwWorldTick		= 0;
	m_dwLastSaveTick	= m_dwWorldTick;

	m_dwMaxMsgSize = 0;

	// 初始化服务器时间
	m_CurrentTime = GetCurrentDWORDTime();
	m_lFabaoOpen = 0;
	m_lWillShutdown = 0;
	m_lWGCheck_On  = 0;
	m_n16ConfigWanPort = 0;
	m_lPrintRoleInfo = 0;
	memset(m_strConfigWanIP,0,sizeof(m_strConfigWanIP));

	for(int i = 0; i < 10; i++)
	{
		m_LPuperStars[i] = 0;
	}

	m_hKoreaLogFile = NULL;
}

//-----------------------------------------------------------------------------
// destruction
//-----------------------------------------------------------------------------
World::~World()
{

}


// Jason 2010-1-29 v1.3.2 初始化功能线程化，这个函数，是为那些必须在其他线程开始前被初始化的变量或对象，而准备的
BOOL World::FirstInit(HINSTANCE hInst)
{
	// vEngine相关对象
	m_pLog = "Log";
	CreateObj("LogNetAnalyze", "Log");
	m_pLogNetAnalyze = "LogNetAnalyze";
	m_pThread = "Thread";
	m_pRender = "GDIRender";
	m_pWindowWrap = "WindowWrap";
	m_pUtil = "Util";
	m_pFastCode = "FastCode";
	m_pGUI = "GUISystem";
	m_pConsole = "Console";

	// 系统参数
	m_nCpuNum = TObjRef<SysInfo>()->GetProcessorNum();

	//创建VFS文件系统,读取XML需要用到
	CreateObj("VFS_System", "VirtualFileSys");

	IFS* pIFS = (IFS*)(VOID*)TObjRef<VirtualFileSys>("VFS_System");
	ResMgr::Inst()->SetFS(ResMgr::EFST_System, pIFS);


	//	初始化随机数种子
	//m_pUtil->Randomize(timeGetTime() * 12345);
	m_pUtil->Randomize((unsigned)time( NULL ));

	// 加载配置文件
	TObjRef<VarContainer> pVar = "VarContainer";
	pVar->Load("VirtualFileSys", _T("server_config/world/world.ini"));

	TObjRef<StreamTransport> pTran = "StreamTransport";

	m_dwViewWidth = (DWORD)pVar->GetDword(_T("width"), _T("window"));
	m_dwViewHeight = (DWORD)pVar->GetDword(_T("height"), _T("window"));
	m_bySaveOneNeedTicks = (BYTE)pVar->GetDword(_T("save_num_per_tick"), _T("world"));
	m_dwGoldenCode = (DWORD)pVar->GetDword(_T("golden_code"), _T("server"));
	m_strWorldName = pVar->GetString(_T("name"), _T("world"));

	// 作弊检测
	m_fLaxCheatDistanceSQ = pVar->GetFloat(_T("lax_cheat_distance"), _T("cheat")) * FLOAT(TILE_SCALE);
	m_fLaxCheatDistanceSQ = m_fLaxCheatDistanceSQ * m_fLaxCheatDistanceSQ;
	m_fStrictCheatDistanceSQ = pVar->GetFloat(_T("strict_cheat_distance"), _T("cheat")) * FLOAT(TILE_SCALE);
	m_fStrictCheatDistanceSQ = m_fStrictCheatDistanceSQ * m_fStrictCheatDistanceSQ;

	// 默认情况下，骑宠和跟随宠是总数的一半，两者之和就是可以佩戴宠物的总数
	MAX_PETSOUL_NUM = pVar->GetDword  (_T("packet_valve pet"));
	MAX_FOLLOWPETSOUL_NUM = MAX_PETSOUL_NUM / 2;
	MAX_RIDINGPETSOUL_NUM = MAX_PETSOUL_NUM - MAX_FOLLOWPETSOUL_NUM;
	// Jason 2010-3-30
	SetMaxTitleNum( pVar->GetDword(_T("title_num title")) );
	m_lFabaoOpen = pVar->GetDword(_T("on fabao"));
	m_lPrintRoleInfo = pVar->GetDword(_T("on PrintRoleInfo"));
	// Jason 2010-4-26 v2.1.0
	nRunawayTickCount = pVar->GetDword(_T("pk_runaway_ticks"),_T("world"),nRunawayTickCount);
	nLoginTimeOutTicks = pVar->GetDword(_T("login_timeout_ticks"), _T("world"), nLoginTimeOutTicks);
	// Jason 2010-5-18 v2.1.2
	bSoaringSwitch = pVar->GetDword(_T("on"),_T("soaring"),FALSE);
	nMaxRoleLevel = pVar->GetDword(_T("role_level_limit"),_T("world"));
	nDistance = pVar->GetDword(_T("sale_distance"),_T("world"));

	m_lWGCheck_On = (LONG)pVar->GetDword(_T("on wgcheck_on") );
	bTianShenZhongOpen = (pVar->GetDword(_T("tshzh_open"),_T("battle_field"),FALSE) == FALSE)?FALSE:TRUE;
	bQiHuaZhiHaiOpen   = (pVar->GetDword(_T("qhzhh_open"),_T("battle_field"),FALSE) == FALSE?FALSE:TRUE);
	bBenLeiLieGuOpen   = (pVar->GetDword(_T("bllg_open"),_T("battle_field"),FALSE) == FALSE?FALSE:TRUE);
	bKillCounterOpen   = (pVar->GetDword(_T("open"),_T("kill_counter"),FALSE) == FALSE?FALSE:TRUE);
	// 妖精契约开工
	s_lFairyContractSwitch = pVar->GetDword(_T("open fairy_contract"));
	// 神铸相关参数读取
	bGodStrengthOpen		= pVar->GetDword(_T("open god_strengthen"));
	nGodStengthEquipMinQlty	= pVar->GetDword(_T("gs_quality god_strengthen"));
	dwGodStrengthCoolDown		= pVar->GetDword(_T("gs_cooldown god_strengthen")) * 60 * 60;
	nGodStrengthAssitEquipMinQlty	= pVar->GetDword(_T("gs_assist_quality god_strengthen"));
	nGodStrengthAssitEquipMinLevel		= pVar->GetDword(_T("gs_assist_equiplevel god_strengthen"));

	DWORD dwYear = pVar->GetDword(_T("year old_server_new_player"));
	DWORD dwMonth = pVar->GetDword(_T("month old_server_new_player"));
	DWORD dwDay = pVar->GetDword(_T("day old_server_new_player"));
	dwOldSerNewPlayerDate = tagDWORDTime(0, 0, 0, (BYTE)dwDay, (BYTE)dwMonth, (BYTE)(dwYear%100));
	dwOldSerNewPlayerMaxLevel = pVar->GetDword(_T("max_level old_server_new_player"));

	CHAR szIP[X_IP_LEN] = {0};
	bUseConfigWANIP = pVar->GetDword(_T("open_mapping server"));
	if( bUseConfigWANIP )
	{
		strncpy(szIP, m_pUtil->UnicodeToUnicode8(pVar->GetString(_T("mapping_ip server"))), _countof(szIP) - 1);
		szIP[X_IP_LEN-1] = 0;
		IFASTCODE->MemCpy(m_strConfigWanIP , szIP,X_IP_LEN-1);
		m_n16ConfigWanPort = pVar->GetDword(_T("mapping_port server"));
	}

	// 初始化需要记录PK时的log的角色的ID
	INT nNum = pVar->GetInt(_T("num"), _T("role_pk_log"));
	for (int i=0;i<nNum && i<X_NEED_PK_LOG_ROLE_NUM;++i)
	{
		TCHAR szTmp[X_SHORT_STRING] = {0};
		_sntprintf(szTmp, X_SHORT_STRING, _T("roleid%d"), i+1);
		g_dwNeedPKLogRoleID[i] = pVar->GetInt(szTmp, _T("role_pk_log"));
	}

	CGodMiracleMgr::GetInst()->Init();

	OfflineReward::OfflineRewardSwitch( pVar->GetDword(_T("offline_reward"),_T("preventlost"),1) );
	OnlieRewardParam::LoongLessingSwitch( pVar->GetDword(_T("loonglessing"),_T("preventlost"),1) );
	bPlayerBack = pVar->GetDword(_T("playerback preventlost")) ;
	PlayerRegression * playerregression = TPlayerRegression::Instance  ();
	if( P_VALID(playerregression) )
	{
		playerregression->Open(bPlayerBack);
	}

	ConvenientTeam& EasyTeam = ConvenientTeam::GetInstance();
	EasyTeam.Open(pVar->GetDword(_T("open easy_team")));
	if( EasyTeam.IsOpen() )
	{
		EasyTeam.OpenTeamNotice(pVar->GetDword(_T("listen_team easy_team")));
		EasyTeam.LimitTeamInfoNum(pVar->GetInt(_T("team_limit easy_team")));
		EasyTeam.LimitPckSize(pVar->GetInt(_T("pck_limit easy_team")));
	}

	nFabaoLevelLimit = pVar->GetInt(_T("level_limit fabao")); // 妖精等级限制
	lFabaoFoodQualifFlag = pVar->GetInt(_T("food_qualif fabao")); // 资质等开关
	lFabaoElemStuntFlag = pVar->GetInt(_T("elem_stunt fabao")); // 妖精元素伤害以及特技开关

	bOpenFairyMating = pVar->GetDword(_T("birth_open fabao"));

	bOpenShielding = pVar->GetDword(_T("shielding_on overseas_version"));

	lPKConstraintsOpen = pVar->GetDword(_T("on pk_constraints"));
	if( lPKConstraintsOpen )
		nPKConstraintsMinLevel = pVar->GetDword(_T("min_level pk_constraints"));

	nDeductMorality = pVar->GetDword(_T("deduct_morality pk_constraints"));
	nMoralityTime = pVar->GetDword(_T("morality_time pk_constraints"));
	nReturnMorality = pVar->GetDword(_T("return_morality pk_constraints"));

	lGodSystemSwitch = pVar->GetDword(_T("on GodSystem"));

	bUseIPGSystem = pVar->GetDword(_T("on IPGSystem"));


	s_dwGetCashPort = pVar->GetDword(_T("port_getcash ipg_server"));
	s_dwIOCashPort = pVar->GetDword(_T("port_takecash ipg_server"));
	strncpy(s_szIPGIP, m_pUtil->UnicodeToUnicode8(pVar->GetString(_T("ip ipg_server"))), _countof(s_szIPGIP) - 1);
	s_szIPGIP[20-1] = 0;
	
	

	// 神之竞标相关
	g_GodBidMgr.InitFixedBidStartTimeList(pVar, FALSE);
	g_GodBidMgr.InitFixedBidStartTimeList(pVar, TRUE);
	g_GodBidMgr.InitGodBidSwitch(pVar);

	// 砸金蛋
	bOpenEggBroker = pVar->GetDword(_T("on egg_broker"));

	//
	lCountProtocolSwitch = pVar->GetDword(_T("on pro_counter"));
	if( lCountProtocolSwitch )
	{
		_protocol_counter.LoadIDNameFromFile(strNetCmgIdFileName);
// 		_protocol_counter.SetLocale(pVar->GetString(_T("local"),_T("pro_counter"),_T("")));
	}
	
	BOOL bXtrapEnable = (pVar->GetDword(_T("on xtrap")) == 0)?FALSE:TRUE;
	g_XTrapGuarder.SetEnable(bXtrapEnable);
	g_XTrapGuarder.OnServerStart();

	DWORD dwStarEnhanceOn	 = pVar->GetDword(_T("StarEnhance_On"),_T("holy_soul_equip"),0);
	bStarEnhance			 = ((dwStarEnhanceOn == 1) ? true : false);
	// 创建GUI渲染
	m_pGUIRender = new IGUIRenderNative;
	if( !P_VALID(m_pGUIRender))	return FALSE;
	m_pGUIRender->Init(m_pRender);

	// 创建窗口
	m_pWindowWrap->Init(m_pGUIRender, m_dwViewWidth, m_dwViewHeight, TRUE);
	if( m_strWorldName.length() < 200 )
	{
		TCHAR strTitleBuff[256] = {0};
		_stprintf(strTitleBuff,_T("WorldServer[ %s ]"),m_strWorldName.c_str());
		m_pWindowWrap->CreateWnd(strTitleBuff, hInst);
	}
	else
	{
		m_pWindowWrap->CreateWnd(_T("WorldServer"), hInst);
	}

	// 创建GUI系统
	if( !m_pRender->Init(m_pWindowWrap->GetWidth(), m_pWindowWrap->GetHeight()) )
		return FALSE;

	m_pGUI->Init(m_pWindowWrap->GetWidth(), m_pWindowWrap->GetHeight(), m_pGUIRender);

	// 创建Console
	m_pConsole->Init(NULL, 1000, TRUE);

	// 生成log
	m_pLog->Create();
	m_pLogNetAnalyze->Create(CreateLogAnalyzeFileName());

	return TRUE;
}

#ifdef ON_GAMEGUARD
//// 韩国CS认证 Log callback
//VOID _stdcall CS3LogCallback(int nLogType, char *szLog)
//{
//	switch(nLogType)
//	{
//	case LOG_UPDATE:
//		printf("this is update log: %s\n", szLog);
//		break;
//	case LOG_NORMAL:
//		printf("this is normal log: %s\n", szLog);
//		break;
//	case LOG_DEBUG:
//		printf("this is debug log: %s\n", szLog);
//		break;	
//	}
//}

// 韩国CS认证 update callback
VOID _stdcall CS3UpdateInfoCallback(int nUpdateType, int nBefore, int nAfter)
{
	if ( nUpdateType == UPDATE_PROTOCOL )
	{
		printf("this is protocol update notification: %d -> %d \n", nBefore, nAfter);
	}
	else if ( nUpdateType == UPDATE_GGVERSION )
	{
		printf("this is ggversion update notification: %d -> %d \n", nBefore, nAfter);
	}
}
#endif

//-----------------------------------------------------------------------------
// init
//-----------------------------------------------------------------------------
BOOL World::Init(HINSTANCE hInst)
{
#ifdef _MEM_TRACE_
	ILOG->Write(EnvString());
#endif
	// 初始化字典
	g_ipDict.Init();
	
	// 炸弹初始化
	g_pSGuarder.Init();

	// 加载配置文件
	TObjRef<VarContainer> pVar = "VarContainer";
	//pVar->Load("VirtualFileSys", _T("server_config/world/world.ini"));

	g_DataCollection.SetOpen((bool)pVar->GetInt(_T("on data_collect")));

	// ca info
	TObjRef<StreamTransport> pTran = "StreamTransport";
	DWORD dwLocalWanIP = GT_INVALID;
	if( TRUE == pTran->GetLocalWANIP(dwLocalWanIP) )
	{
		struct in_addr address;
		address.S_un.S_addr = dwLocalWanIP;
		// 去掉了
		if(!(address.S_un.S_un_b.s_b1 == 120 && address.S_un.S_un_b.s_b2 == 70 && address.S_un.S_un_b.s_b3 == 227)) //不是新疆
		{
		}
		//
		{
			string strArea = m_pUtil->UnicodeToAnsi(pVar->GetString(_T("area CA_info")));
			string strBlackList = m_pUtil->UnicodeToAnsi(pVar->GetString(_T("black_list CA_info")));
			string strCertificate = m_pUtil->UnicodeToAnsi(pVar->GetString(_T("certificate CA_info")));

			DWORD dwZoneID	=	pVar->GetDword(_T("section_id world"));
			DWORD dwGroupID		=	pVar->GetDword(_T("id world"));

			LPCSTR szLocalWanIP = pTran->IP2String(dwLocalWanIP);
			

			BOOL bCheckOK = FALSE;
			INT nCANum = pVar->GetInt(_T("num CA_svrlist"));
			for (INT i=0; i<nCANum; ++i)
			{
				tstringstream tsTmp;

				tsTmp.clear();
				tsTmp << _T("ca_") << i+1;
				LPSTR szCAIP = (LPSTR)m_pUtil->UnicodeToAnsi(pVar->GetString(tsTmp.str().c_str(), _T("CA_svrlist"), _T("")));

				tsTmp.str(_T(""));
				tsTmp << _T("port_") << i+1;
				INT nCAPort = pVar->GetInt(tsTmp.str().c_str(), _T("CA_svrlist"), GT_INVALID);

				if (P_VALID(szCAIP) && P_VALID(nCAPort))
				{
					if(0 == CheckValid("SM", strArea.c_str(), dwZoneID, dwGroupID, szLocalWanIP, strCertificate.c_str(), strBlackList.c_str(), szCAIP, nCAPort ))
					{
						bCheckOK = TRUE;
						break;
					}
				}

			}
			if (!bCheckOK) return FALSE;
		}
	}


	LPCSTR szLocalWanIP = pTran->IP2String(dwLocalWanIP);
	strncpy( s_szIP, szLocalWanIP, 64 );

	string strMAC;
	GetMAC(strMAC);
	strncpy( s_szMac, strMAC.c_str(), 64 );

	TCHAR tszTemp[MAX_PATH] = {0};
	GetModuleFileNameW(NULL, tszTemp, MAX_PATH);
	BYTE byMD5[16] = {0};
	IUTIL->MD5ForFile(tszTemp,byMD5);
	string strMD5;
	IUTIL->MD5ToString(strMD5,byMD5);
	strncpy( s_szMD5, strMD5.c_str(), 48 );

	string strSign = m_pUtil->UnicodeToAnsi(pVar->GetString(_T("strength_item_0 spirit")));
	strncpy( s_szSign, strSign.c_str(), 32 );

	if(!IsOneSign(s_szSign))
		return FALSE;


	// 脚本初始化
	if( FALSE == g_ScriptMgr.Init() )
	{
		this->OutPutInitStepInfo(EWI_ScriptMgr,FALSE);
		return FALSE;
	}
	// 资源初始化
	if( FALSE == g_attRes.Init() )
	{
		this->OutPutInitStepInfo(EWI_AttRes,FALSE);
		return FALSE;
	}
	// 帮派
	if( FALSE == g_guildMgr.Init() )
	{
		this->OutPutInitStepInfo(EWI_GuildMgr,FALSE);
		return FALSE;
	}
	// 社会管理器初始化
	if( FALSE == g_socialMgr.Init() )
	{
		this->OutPutInitStepInfo(EWI_SocialMgr,FALSE);
		return FALSE;
	}
	// 商城管理器初始化
	if( FALSE == g_mall.Init() )
	{
		this->OutPutInitStepInfo(EWI_Mall,FALSE);
		return FALSE;
	}
	// 元宝交易管理类初始化
	if( FALSE == g_tradeYB.Init())
	{
		this->OutPutInitStepInfo(EWI_TradeYB,FALSE);
		return FALSE;
	}
	// 团队管理器初始化
	if( FALSE == g_groupMgr.Init())
	{
		this->OutPutInitStepInfo(EWI_GroupMgr,FALSE);
		return FALSE;
	}
	// 名人堂初始化
	if (FALSE == g_fameHall.Init())
	{
		this->OutPutInitStepInfo(EWI_FameHall,FALSE);
		return FALSE;
	}
	// 怪物小队队形初始化
	if(FALSE == NPCTeamMgr::LoadNPCOrder())
	{
		this->OutPutInitStepInfo(EWI_NPCTeamMgr,FALSE);
		return FALSE;
	}
	// VIP摊位初始化
	if (FALSE == g_VIPStall.Init())
	{
		this->OutPutInitStepInfo(EWI_VIPStall,FALSE);
		return FALSE;
	}
	IMSG( _T("Loading map informations...\r\n") );
	// 创建所有地图
	if( FALSE == g_mapCreator.Init() )
	{
		this->OutPutInitStepInfo(EWI_MapCreator,FALSE);
		return FALSE;
	}
	// 所有在地图上层处理的消息管理器初始化
	if( FALSE == g_worldNetCmdMgr.Init() )
	{
		this->OutPutInitStepInfo(EWI_WorldNetCmdMgr,FALSE);
		return FALSE;
	}

	// 人物管理器初始化
	if( FALSE == g_roleMgr.Init() )
	{
		this->OutPutInitStepInfo(EWI_RoleMgr,FALSE);
		return FALSE;
	}
	//任务管理器初始化
	if (FALSE == g_questMgr.Init())
	{
		this->OutPutInitStepInfo(EWI_QuestMgr,FALSE);
		return FALSE;
	}
	// 脚本通用消息流管理类
	if( FALSE == g_MsgInfoMgr.Init())
	{
		this->OutPutInitStepInfo(EWI_MsgInfoMgr,FALSE);
		return FALSE;
	}
	// 金牌网吧
	if( FALSE == g_VipNetBarMgr.Init() )
	{
		this->OutPutInitStepInfo(EWI_VipNetBarMgr,FALSE);
		return FALSE;
	}

	//掉落
	new LootMgr;	//注意 此处 new
	if( FALSE == sLootMgr.Init() )
	{
		this->OutPutInitStepInfo(EWI_LootMgr,FALSE);
		return FALSE;
	}
	//夫妻系统
	g_MarriageMgr.Init();

	g_activityMgr.Init();
    //选秀
	g_DraftShow.Init();
    
	// PvP 系统初始化
	if( FALSE == g_AthleticsSystem.Init())
	{
		this->OutPutInitStepInfo(EWI_PvPAthleticsSystem,FALSE);
		return FALSE;
	}

	// 城市系统初始化　要在g_activityMgr.Init();之后
	if ( FALSE == g_cityMgr.Init() )
	{
		this->OutPutInitStepInfo(EWI_CityMgr,FALSE);
		return FALSE;
	}
	// 创建所有session
	if( FALSE == g_worldSession.Init() )
	{
		this->OutPutInitStepInfo(EWI_WorldSession,FALSE);
		return FALSE;
	}
	if( FALSE == g_dbSession.Init() )
	{
		this->OutPutInitStepInfo(EWI_DBSession,FALSE);
		return FALSE;
	}
	if( FALSE == g_loginSession.Init() )
	{
		this->OutPutInitStepInfo(EWI_LoginSession,FALSE);
		return FALSE;
	}
	if (FALSE == g_billSession.Init())
	{
		this->OutPutInitStepInfo(EWI_BillSession,FALSE);
		return FALSE;
	}
	if( FALSE == g_rtSession.Init() )
	{
		this->OutPutInitStepInfo(EWI_RtSession,FALSE);
		return FALSE;
	}
	if( FALSE == g_keyCodeBridgeSession.Init() )
	{
		this->OutPutInitStepInfo(EWI_KEYCODEBRIDGESESSION,FALSE);
		return FALSE;
	}
	
	if( g_world.IsUseIPGSystem() )
	{
		//if( FALSE == g_IpgSession.Init() )
		//{
		//	this->OutPutInitStepInfo(EWI_IPGSession,FALSE);
		//	return FALSE;
		//}

		//if( FALSE == g_IpgCashSession.Init() )
		//{
		//	this->OutPutInitStepInfo(EWI_IPGCashSession,FALSE);
		//	return FALSE;
		//}

		IPGDragonfly* pIPGDragonfly = IPGDragonfly::GetInstance();
		if( NULL != pIPGDragonfly )
		{
			if( !pIPGDragonfly->InitIPGDragonfly(GetIPGIP(), GetGetCashPort(),GetIOCashPort()) )
			{
				this->OutPutInitStepInfo(EWI_IPGSession,FALSE);
				return FALSE;
			}

			m_hKoreaLogFile = (HANDLE)CreateFile( _T("log\\ipg_log.txt"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, NULL, NULL); 
			if (m_hKoreaLogFile == INVALID_HANDLE_VALUE) 
			{ 
				m_hKoreaLogFile = NULL; 
			}

			//if( NULL != pIPGDragonfly && pIPGDragonfly->IsInitialize() )
			//{
			//	int nCash = 0;
			//	pIPGDragonfly->GetDFCashReq( 11, "1", "", nCash);
			//}

		}
	}
#ifdef ON_INTERNET_CAFE
	if( FALSE == g_InternetCafeSession.Init() )
	{
		this->OutPutInitStepInfo(EWI_InternetCafeSession,FALSE);
		return FALSE;
	}
#endif

	PlayerNetCmdMgr::Init();

	// 状态特殊类初始化
	BuffEffect::Init();

	// 移动初始化
	MoveData::m_Timer.Init();

	// 宝箱开启物品初始化
	g_TreasureChestMgr.Init();

#ifdef ON_FBBS
	// 台湾与facebook边界服务器接口部分，开关已经内嵌与g_FBBSSession
	g_FBBSSession.Init();
#endif

#ifdef ON_GAMEGUARD
	// 初始化CS认证
	if ( ERROR_SUCCESS != InitCSAuth3("GameGuard/"))
	{
		return FALSE;
	}

	// 注册 Log  callback
	// SetCallbackFunction(CALLBACK_LOG, (PVOID)CS3LogCallback); 
	// 注册 Update  callback
	SetCallbackFunction(CALLBACK_UPDATE, (PVOID)CS3UpdateInfoCallback);	
#endif

	// 老虎机
	TigerMgr::ClearTimeMark();
	// todo: gmsession
	// todo: 这些session是否也可以做成单个线程呢
	
	// 记录运行计时
	m_dwLastMinute = timeGetTime();
	m_dwLastHour = m_dwLastMinute;
	m_dwTotalRunMinute = 0;

	if(  ShieldingMgr::Create() == NULL )
	{
		return FALSE;
	}

	// 启动update线程
	if( FALSE == m_pThread->CreateThread(_T("thread_update"),
		(vEngine::THREADPROC)m_Trunk.sfp1(&World::ThreadUpdate), NULL) )
		return FALSE;

	// 注册服务器控制台命令(测试专用:正式版本需注释掉)
	//RegisterTestCmd(m_pConsole);

	// 注册脚本trace的命令
	RegisterTraceCmd();
	RegisterSysTracerCmd();

	RegisterClearGuildState();
	RegisterNPIR();
	RegisterResetMaxTickValve();

	TheChatLogger.Init();
	TheNetCmdViewer.Init();
	sLootMgr.RegisterCmd();

	RegisterEasyTeamSwitch();
	RegisterShutdownServer();
	RegisterDBReconnectFunc();

	RegisterFabaoFoodQualifFunc();
	RegisterFabaoElemStuntFunc();
	RegisterReloadItemNeedlog();

	RegisterOpenFabaoMating();
	RegisterShielding();

	RegisterMsgCounter();
	RegisterTigerInfo();

	InterlockedExchange((LPLONG VOLATILE)&g_InitFlag,2);

	IMSG(_T("=========================================\r\n"));
	IMSG(_T("==                                     ==\r\n"));
	IMSG(_T("==     System initialize finished.     ==\r\n"));
	IMSG(_T("==                                     ==\r\n"));
	IMSG(_T("=========================================\r\n"));
	IMSG(_T("\r\n"));
	IMSG(_T("\r\n"));
	IMSG(_T("Preparing to start world server, please wait.\r\n"));
	IMSG(_T("\r\n"));

	return TRUE;
}
	
//-----------------------------------------------------------------------------
// destroy
//-----------------------------------------------------------------------------
VOID World::Destroy()
{
	// 关闭主循环线程
	InterlockedExchange((LONG*)&m_bTerminateUpdate, TRUE);
	m_pThread->WaitForThreadDestroy(_T("thread_update"), INFINITE);

	// 关闭所有的地图线程
	g_mapCreator.StopAllMapMgr();

	// 保存所有玩家的信息
	g_roleMgr.SaveAllRolesToDB();

	// 保存其余信息到数据库
	SaveWorldInfoToDB();

	// 等待db处理完消息
	WaitDBTerminate();

	// 得到当前时间
	SYSTEMTIME sys_time;
	GetLocalTime(&sys_time);

	NPCTeamMgr::UnLoadNPCOrder();

	g_VipNetBarMgr.Destroy();

	// 记录到log文件
	m_pLog->Write(_T("shutdown at %02d/%02d/%04d %02d:%02d:%02d\r\n"),
		sys_time.wMonth, sys_time.wDay, sys_time.wYear,
		sys_time.wHour, sys_time.wMinute, sys_time.wSecond);
	OutputStarsLog();
	//掉落
	delete sLootMgr.getSingletonPtr();
    g_DraftShow.Destroy();
    g_AthleticsSystem.Destroy();
	g_groupMgr.Destroy();
	g_roleMgr.Destroy();
	g_mapCreator.Destroy();
	if( g_world.IsUseIPGSystem() )
	{
		CloseHandle(m_hKoreaLogFile);
		m_hKoreaLogFile = NULL;
		//g_IpgSession.Destroy();
		//g_IpgCashSession.Destroy();
		IPGDragonfly::ReleaseInstance();

		//IPGDragonfly* pIPGDragonfly = IPGDragonfly::GetInstance();
		//if( NULL != pIPGDragonfly && pIPGDragonfly->IsInitialize() )
		//{
		//	if( !pIPGDragonfly->InitIPGDragonfly(NULL) )
		//	{
		//		this->OutPutInitStepInfo(EWI_IPGSession,FALSE);
		//		return FALSE;
		//	}
		//}
	}
	g_rtSession.Destroy();
	g_billSession.Destroy();
	g_loginSession.Destroy();
	g_worldSession.Destroy();
	g_dbSession.Destroy();	
	g_guildMgr.Destroy();
	g_questMgr.Destroy();
	g_mall.Destroy();
	g_ScriptMgr.Destroy();
	g_TreasureChestMgr.Destroy();
	g_cityMgr.Destory();
	g_attRes.Destroy();
	g_keyCodeBridgeSession.Destroy();
	g_GlobalLimitedItemMgr.Destroy();
#ifdef ON_INTERNET_CAFE
	if( FALSE == g_InternetCafeSession.Init() )
	{
		g_InternetCafeSession.Destroy();
	}
#endif

	// 便捷组队
	ConvenientTeam::Destroy();

#ifdef ON_FBBS
	// 台湾与facebook边界服务器接口部分，开关已经内嵌与g_FBBSSession
	g_FBBSSession.Destroy();
#endif

	FairyMatingMgr::Destroy();

	// 销毁窗口及GUI等系统
	m_pWindowWrap->Destroy();
	m_pGUI->Destroy();
	m_pRender->Destroy();
	m_pConsole->Destroy();
	m_pLog->Close();
	m_pLogNetAnalyze->Close();
	TheChatLogger.Destroy();
	TheNetCmdViewer.Destroy();

#ifdef ON_GAMEGUARD
	// 关闭韩国CS认证
	CloseCSAuth3();
#endif

	ShieldingMgr::FinalRelease();

	SAFE_DEL(m_pGUIRender);

	TPlayerRegression::Reclaim  ();
	NavKernel::Inst()->Destroy();
}

//---------------------------------------------------------------------------------
// 保存游戏世界的信息
//---------------------------------------------------------------------------------
VOID World::SaveWorldInfoToDB()
{
	g_guildMgr.SendAllGuild2DB();
	g_activityMgr.SaveAllActivityData2DB();
	g_GlobalLimitedItemMgr.SaveAllData2DB();
	g_FamilySpriteMgr.SaveAll2DB();
	g_FamilyMgr.SendAllFamilyInfo2DB();

	tagNDBC_SaveTigerProfit send;
	send.n64Profit = TigerMgr::GetProfitNum();
	g_dbSession.Send(&send, send.dwSize);
}

//-----------------------------------------------------------------------------
// Main loop
//-----------------------------------------------------------------------------
VOID World::Mainloop()
{
	DWORD dwMsg=0, dwParam1=0, dwParam2=0;

	while( FALSE == m_pWindowWrap->MsgLoop() && FALSE == IsShutingdown() )
	{
		if( 0 == g_InitFlag )
		{
			if( g_hIniThread )
			{
				IMSG(_T("Initial failed,please quit, and then check the config files. \r\n") );
				WaitForSingleObject  (g_hIniThread,INFINITE);
				CloseHandle  ( g_hIniThread );
				g_hIniThread = NULL;
			}
		}

		if( FALSE == m_pWindowWrap->IsWindowActive() )
		{
			Sleep(30);
			continue;
		}

		while( m_pWindowWrap->PeekWindowsMsg(dwMsg, dwParam1, dwParam2) )
		{
			m_pGUI->OnWindowsMsg(dwMsg, dwParam1, dwParam2);
			if( dwMsg == WM_QUIT )
				return;
		}

		m_pGUI->Update();
		
		// render
		m_pRender->Clear(0, 0, 0);
		m_pGUI->Render();
		m_pRender->Render();

		UpdateMemoryUsage();

		// 加入要观察的实时变量
		m_pConsole->Watch(_T("version"), LOONGWORLD_BUILD_REVISION);
		m_pConsole->Watch(_T("cpu_num:"), m_nCpuNum);
		m_pConsole->Watch(_T("mem_total:"), m_dwTotalPhys/1024/1024);
		m_pConsole->Watch(_T("mem_avail:"), m_dwAvailPhys/1024/1024);
		m_pConsole->Watch(_T("non_paged_pool_usage:"), m_nQuotaNonPagedPoolUsage);
		m_pConsole->Watch(_T("run_minute"), m_dwTotalRunMinute);
		m_pConsole->Watch(_T("world_tick"), GetWorldTick());
		m_pConsole->Watch(_T("max_tick"), GetMaxTickTime());
		// 当前tick
		m_pConsole->Watch(_T("cur_tick"), GetCurTickOfWord());
		m_pConsole->Watch(_T("idle_time"), GetIdleTime());

		if( 2 == g_InitFlag )
		{
			m_pConsole->Watch(_T("online_num"), g_worldSession.GetPlayerNumDisplay());
			m_pConsole->Watch(_T("online_peek"), g_worldSession.GetPlayerNumPeek());
			m_pConsole->Watch(_T("online_limit"), g_worldSession.GetPlayerNumLimit());
			m_pConsole->Watch(_T("login_times"), g_worldSession.GetPlayerLoginTimes());
			m_pConsole->Watch(_T("logout_times"), g_worldSession.GetPlayerLogoutTimes());
			m_pConsole->Watch(_T("max_msg"), GetMaxMsgSize());
			m_pConsole->Watch(_T("send_cast"), g_worldSession.GetMsgSendCast());
			m_pConsole->Watch(_T("send_size"), g_worldSession.GetTotalSendSize());
			m_pConsole->Watch(_T("real_send"), g_worldSession.GetTotalRealSendSize());
			m_pConsole->Watch(_T("com_ratio"), g_worldSession.GetSendComRatio());
			m_pConsole->Watch(_T("-- DB Status --"), 0);
			m_pConsole->Watch(_T("db_connected"), g_dbSession.GetToDBConnected());
			m_pConsole->Watch(_T("db_recv"), g_dbSession.GetReceivedPackageNum());
			m_pConsole->Watch(_T("db_unsend"), g_dbSession.GetUnsendPackageNum());
			m_pConsole->Watch(_T("-- Map Status --"), 0);
			m_pConsole->Watch(_T("instance_nolimit"), g_mapCreator.GetInstanceNum(EMIR_NoLimit));
			m_pConsole->Watch(_T("instance_512"), g_mapCreator.GetInstanceNum(EMIR_512));
			m_pConsole->Watch(_T("instance_1024"), g_mapCreator.GetInstanceNum(EMIR_1024));
			m_pConsole->Watch(_T("instance_2048"), g_mapCreator.GetInstanceNum(EMIR_2048));
			m_pConsole->Watch(_T("instance_3072"), g_mapCreator.GetInstanceNum(EMIR_3072));
			m_pConsole->Watch(_T("instance_coefnum"), g_mapCreator.GetInstanceCoefNum());
			m_pConsole->Watch(_T("instance_coeflimit"), g_mapCreator.GetInstanceCoefNumLimit());
		}
		else
		{
			m_pConsole->Watch(_T("online_num"), 0);
			m_pConsole->Watch(_T("online_peek"), 0);
			m_pConsole->Watch(_T("online_limit"), 0);
			m_pConsole->Watch(_T("login_times"), 0);
			m_pConsole->Watch(_T("logout_times"), 0);
			m_pConsole->Watch(_T("max_msg"), GetMaxMsgSize());
			m_pConsole->Watch(_T("send_cast"), 0);
			m_pConsole->Watch(_T("send_size"), 0);
			m_pConsole->Watch(_T("real_send"), 0);
			m_pConsole->Watch(_T("com_ratio"), 0);
			m_pConsole->Watch(_T("-- DB Status --"), 0);
			m_pConsole->Watch(_T("db_connected"), 0);
			m_pConsole->Watch(_T("db_recv"), 0);
			m_pConsole->Watch(_T("db_unsend"),0);
			m_pConsole->Watch(_T("-- Map Status --"), 0);
			m_pConsole->Watch(_T("instance_nolimit"), 0);
			m_pConsole->Watch(_T("instance_512"), 0);
			m_pConsole->Watch(_T("instance_1024"), 0);
			m_pConsole->Watch(_T("instance_2048"), 0);
			m_pConsole->Watch(_T("instance_3072"), 0);
			m_pConsole->Watch(_T("instance_coefnum"), 0);
			m_pConsole->Watch(_T("instance_coeflimit"), 0);

		}

		

		Sleep(50);
	}
}

//-----------------------------------------------------------------------------
// Update线程
//-----------------------------------------------------------------------------
DWORD World::ThreadUpdate(LPVOID)
{
	INT nGarbage = 0x7fffffff;
	INT nLastTimeGarbage = 0x7fffffff;
	INT nIncreaseCounter = 0;

	DWORD dwLastMapReportTick = 0;
	DWORD dwCurMapReportTick = 0;
	CONST DWORD dwMapReportTimeout = 3 * 60 * TICK_PER_SECOND;

#ifdef _DEBUG
	_set_se_translator(vEngine::SEH_Translation);

	try
	{
#endif
		while( FALSE == m_bTerminateUpdate )
		{
			CHECK_TMIE_WITH_TIMEOUT(250);
			LARGE_INTEGER m_liPerfFreq;
			LARGE_INTEGER m_liPerfStart;
			QueryPerformanceFrequency(&m_liPerfFreq);
			QueryPerformanceCounter(&m_liPerfStart);

			// 锁住网络
//			LockNetWork();

			// 移动update
			MoveData::m_Timer.Update();

			// g_GMPolicy
			g_GMPolicy.Update();

			// g_worldSession
			g_worldSession.Update();

			// g_loginSession
			g_loginSession.Update();

#ifdef ON_INTERNET_CAFE
			// g_InternetCafeSession
			if (TRUE == g_InternetCafeSession.IsOn())
			{
				g_InternetCafeSession.Update();
			}
#endif

			// g_dbSession
			g_dbSession.Update();

			// g_billSession
			g_billSession.Update();

			// g_rtSession
			g_rtSession.Update();

			// g_keyCodeBridgeSession
			g_keyCodeBridgeSession.Update();

			//if( g_world.IsUseIPGSystem() )
			//{
			//	// g_IpgSession
			//	g_IpgSession.Update();

			//	g_IpgCashSession.Update();
			//}

			// g_mapCreator
			g_mapCreator.Update();

			// g_socialmgr
			g_socialMgr.Update();

			// g_fameHall
			g_fameHall.Update();

			// g_worldNetCmdMgr
			g_worldNetCmdMgr.Update();

			// g_groupmgr
			g_groupMgr.Update();

			// g_activityMgr
			g_activityMgr.Update();

			// g_pSGuarder
			g_pSGuarder.Update();

			// g_guildMgr
			g_guildMgr.Update();

			// g_cityMgr
			g_cityMgr.Update();

			// g_mall
			g_mall.Update();

			// g_VIPStall			
			g_VIPStall.Update();

			// g_scriptMgr			
			g_ScriptMgr.Update();

			// g_gameGuarder
			g_gameGuarder.Update();

            //g_AthleticsSystem
			g_AthleticsSystem.Update();

			// g_MarriageMgr
			g_MarriageMgr.Update();

			g_DraftShow.Update();

			// 更新公会战宣战关系
			g_guildWarDeclareMgr.Update();

			//g_DataCollection.Update();

			// 基本周期运算，每分钟执行一次
			if( timeGetTime() - m_dwLastMinute >= 60*1000 )
			{ 
				m_dwLastMinute += 60*1000;
				OnMinute();
			}

			// 排行更新
			//g_WorldRankingMgr.Update();

			// 保存玩家
			SaveRolePerTick();

			g_FriendlyShutdownServer.Update();

			// 做最后的处理，这个必须要发到所有update之后
			//g_worldSession.DoHouseKeeping();

			// 掉落的全局属性更新，运行时动态加载掉落表
			sLootMgr.Update();

#ifdef ON_FBBS
			// 台湾facebook边界服务器相关
			// 主要开关已经内置于g_FBBSSession模块内部
			g_FBBSSession.Update();
#endif

			// 神之竞标
			g_GodBidMgr.Update();

			// 神系统-稀有物品/领域
			g_GlobalLimitedItemMgr.Update();

			// 家族管理器
			g_FamilyMgr.Update();

			g_FamilySpriteMgr.Update();

			// 做最后的处理，这个必须要发到所有update之后
			g_worldSession.DoHouseKeeping();

			// 解锁
//			UnlockNetWork();
			// 尝试重新加载itemneedlog，注意只有在输入指令，db返回消息后，才有数据过来
			g_dbSession.ReInitItemNeedLog();

			// 导出信令统计数据
			if(lCountProtocolSwitch)
			{
				CHECK_TIME()
				if(dwDumpNetCmdCounterTicks > 0)
					--dwDumpNetCmdCounterTicks;
				else
				{
					dwDumpNetCmdCounterTicks = DUMP_NETCMDCOUNTER_TICk;
					_protocol_counter.DumpCountersToFile(GetDumpNetCmdCountersFileName());
					_protocol_counter.Clear();
				}
			}
			if(lLoadProtocolIDFile)
			{
				CHECK_TIME()
				InterlockedExchange((LPLONG)&lLoadProtocolIDFile,0);
				_protocol_counter.LoadIDNameFromFile(strNetCmgIdFileName);
			}

			PlayerSession::LogNetAnalyze();
			g_worldNetCmdMgr.LogNetAnalyze();

			// 计算sleep时间
			LARGE_INTEGER liPerfNow;
			QueryPerformanceCounter(&liPerfNow);
			__int64 totalTime = (liPerfNow.QuadPart- m_liPerfStart.QuadPart) * 1000000 / m_liPerfFreq.QuadPart;
			/*IMSG(_T("Use Tick %ld\r\n"), totalTime);*/

			DWORD dwTime = (DWORD)(totalTime / 1000);

			// 如果超过了最大最大心跳时间，则设置最大心跳时间
			if( dwTime > m_dwMaxTickTime )
			{
				InterlockedExchange((LPLONG)&m_dwMaxTickTime, dwTime);
			}
			
			// 设置sleep时间			
			if( dwTime < TICK_TIME )
			{
				m_dwIdleTime = TICK_TIME - dwTime;
				Sleep(TICK_TIME - dwTime);
			}
			else
			{
				m_dwIdleTime = 0;
				Sleep(30);
			}
			SetCurTickOfWord(dwTime);

			// 下面这些函数必须在最后几行
			LogMaxTickAlert(dwTime);

			SetWorldTickInc();
			UpdateWorldTime();

			++dwCurMapReportTick ;
			if( dwCurMapReportTick - dwLastMapReportTick >= dwMapReportTimeout )
			{
				dwLastMapReportTick = dwCurMapReportTick;
				g_mapCreator.ReportMapDynInfo(TRUE);
			}

			//
			tagDWORDTime now = GetCurrentDWORDTime();

			// 每天凌晨0点输出昨天生成的紫星装备数量log
			if ( now.hour == 0 && now.min == 0 && now.sec == 0) 
			{
				OutputStarsLog();
			}

		}
#ifdef _DEBUG
	} 
	catch(vEngine::Exception)
	{
		if( m_pUtil->IsDebuggerPresent() )
			throw;
		else
		{
			// 设置ShutDown
			ShutDown();
		}
	}
#endif
	_endthreadex(0);

	return 0;
}

//-----------------------------------------------------------------------------
// 每分钟执行一次
//-----------------------------------------------------------------------------
VOID World::OnMinute()
{
	CHECK_TIME();
	// 基本周期运算，每小时执行一次
	DWORD dw = timeGetTime();
	// 当前的时间
	tagDWORDTime cur_time = GetCurrentDWORDTime();

	if( dw - m_dwLastHour >= 60*60*1000 )
	{ 
		m_dwLastHour += 60*60*1000;
		OnHour();
	}

	if (cur_time.min == 0)
	{
		OnClock(cur_time.hour);
	}

	g_activityMgr.OnMinute(cur_time);
	// Jason 2010-3-16 v1.5.0 
	g_tradeYB.OnMinute();

	// 每分钟更新一次强制脱离师门申请列表
	g_forceBreakOutApprenticeMgr.OnMinute();

	// 每分钟更新一下神职竞选的报名人数
	g_ClergyRankingMgr.Ask4CandidateCount();

	// 显示lua使用内存信息
	g_ScriptMgr.MemMsg();
	// 更新数据库中的老虎机总币数
	tagNDBC_SaveTigerProfit send;
	send.n64Profit = TigerMgr::GetProfitNum();
	g_dbSession.Send(&send, send.dwSize);
}

//-----------------------------------------------------------------------------
// 每小时执行一次
//-----------------------------------------------------------------------------
VOID World::OnHour()
{
	//g_tradeYB.OnHour();
}

//-----------------------------------------------------------------------------
// 每整点执行一次
//-----------------------------------------------------------------------------
VOID World::OnClock(BYTE byHour)
{
	switch (byHour)
	{
	case 0:
		// 清除数据库中过期的公会宣战关系
		g_guildWarDeclareMgr.ClearExpireGuildDeclareInDB();
		g_roleMgr.ClearBuyMallItemCount();
		TigerMgr::ClearTimeMark();
		break;
	case 2:
		// 扣除帮派每日消耗并重置帮务状态
		g_guildMgr.DailyGuildReset();
		break;
	case 3:
		// 每天凌晨3点，定时更新帮派排行
		ApplyToJoinGuildToolkit * pToolkit = ApplyToJoinGuildToolkit::GetApplyToJoinGuildToolkit();
		if( P_VALID(pToolkit) )
		{
			pToolkit->GuildRanking();
		}
		break;
	}

	TigerMgr::HideProfitNum();
	// 所有整点动作
	g_guildMgr.UpgradeGuildSkill();
}

//-----------------------------------------------------------------------------
// 保存玩家信息
//-----------------------------------------------------------------------------
VOID World::SaveRolePerTick()
{
	CHECK_TIME();
	if(m_dwWorldTick - m_dwLastSaveTick < m_bySaveOneNeedTicks)
	{
		return;
	}

	m_dwLastSaveTick = m_dwWorldTick;

	g_roleMgr.SaveOneRoleToDB();
}

//-----------------------------------------------------------------------------
// crc32方法封装 -- 先转换为小写再计算
//-----------------------------------------------------------------------------
DWORD World::LowerCrc32(LPCTSTR str)
{
	tstring strTmp = str;
	transform(strTmp.begin(), strTmp.end(), strTmp.begin(), tolower);

	return IUTIL->Crc32(strTmp.c_str());
}

//-----------------------------------------------------------------------------
// 记录服务器主线调用堆栈信息
//-----------------------------------------------------------------------------
// DWORD World::RecordStack(LPCSTR)
// {
// 	HANDLE handle = m_pThread->GetThreadHandle(_T("thread_update"));
// 	if( GT_VALID(SuspendThread(handle)) )
// 	{
// 		CONTEXT context;
// 		ZeroMemory(&context, sizeof(context));
// 		context.ContextFlags = CONTEXT_CONTROL;
// 		BOOL bResult = GetThreadContext(handle, &context);
// 
// 		// 输出堆栈信息
//  		vEngine::Exception exception(&context);
// 
// 		ResumeThread(handle);
// 		return 1;
// 	}
// 
// 	return 0;
// }

//-----------------------------------------------------------------------------
// 更新内存的占用情况
//-----------------------------------------------------------------------------
VOID World::UpdateMemoryUsage()
{
	MEMORYSTATUS memStatus;
	memStatus.dwLength = sizeof(memStatus);
	GlobalMemoryStatus(&memStatus);

	m_dwTotalPhys = memStatus.dwTotalPhys;
	m_dwAvailPhys = memStatus.dwAvailPhys;

	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));

	m_nQuotaNonPagedPoolUsage = pmc.QuotaNonPagedPoolUsage;
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
VOID World::SwitchGameGuarder( BOOL bOn )
{
	g_gameGuarder.Switch(bOn);
}

//-----------------------------------------------------------------------------
// 服务器状态是否良好
//-----------------------------------------------------------------------------
BOOL World::IsWell() const
{
	if( FALSE == g_dbSession.IsWell() )
		return FALSE;

	return TRUE;
}

//-----------------------------------------------------------------------------
// 等待db返回
//-----------------------------------------------------------------------------
VOID World::WaitDBTerminate()
{
	tagNDBC_WorldShutDown send;

	g_dbSession.Send(&send, send.dwSize);

	DWORD dwSize = 0;
	tagNetCmd* pRecv = (tagNetCmd*)g_dbSession.Recv(dwSize);
	tagNDBS_WorldShutDown RtMsg;

	while(!P_VALID(pRecv) || (P_VALID(pRecv) && RtMsg.dwID != pRecv->dwID))
	{
		Sleep(100);
		pRecv = (tagNetCmd*)g_dbSession.Recv(dwSize);
	}

}
World g_world;

//-----------------------------------------------------------------------------
// main
//-----------------------------------------------------------------------------

// Jason 2010-1-29 v1.3.2 初始化功能线程化，并去掉相关变量的外部链接属性
static DWORD __stdcall InitialWorldFunc(void * p)
{
	HINSTANCE hinst = *((HINSTANCE*)p);
	if( g_world.Init  (hinst) )
		InterlockedExchange((LPLONG VOLATILE)&g_InitFlag,2);
	else
		InterlockedExchange((LPLONG VOLATILE)&g_InitFlag,0);
	return 0;
}


INT APIENTRY _tWinMain(HINSTANCE hInst, HINSTANCE, LPTSTR, INT)
{
	HANDLE hLoongWorldExe = OpenMutex(0,FALSE,_T("SMWorld"));
	if( hLoongWorldExe == NULL && ::GetLastError() == ERROR_FILE_NOT_FOUND )
	{
		hLoongWorldExe = ::CreateMutex(NULL,FALSE,_T("SMWorld"));
	}
	else 
	{
		if( hLoongWorldExe )
			ReleaseMutex(hLoongWorldExe);
		return 0;
	}


	// 设置进程优先级
	SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS);
	
	// 禁止屏幕保护及电源管理
	::SystemParametersInfo(SPI_SETLOWPOWERTIMEOUT, 0, NULL, 0);
	::SystemParametersInfo(SPI_SETPOWEROFFTIMEOUT, 0, NULL, 0);
	::SystemParametersInfo(SPI_SETSCREENSAVETIMEOUT, 0, NULL, 0);

	vEngine::InitNetwork();
	vEngine::InitEngine();


#ifdef _DEBUG
	EXCEPTION_PROTECT;
#endif

	if( FALSE == g_world.FirstInit(hInst) )
	{
		printf("GUI initialized false.");
		Sleep(10000);
		return 0;
	}

	//if( g_world.Init(hInst) )
	DWORD threadid;
	g_hIniThread = CreateThread(NULL,0,InitialWorldFunc,&hInst,0,&threadid);
	{
		g_world.Mainloop();
	}
	g_world.Destroy();

	vEngine::DestroyEngine();
	vEngine::DestroyNetwork();

	WaitForSingleObject  (g_hIniThread,INFINITE);
	CloseHandle  ( g_hIniThread );
	g_hIniThread = NULL;

	ReleaseMutex(hLoongWorldExe);

	return 0;
}

//-----------------------------------------------------------------------------
// 初始化信息
//-----------------------------------------------------------------------------
VOID World::OutPutInitStepInfo(EWorldInitStep stp, BOOL bSuccess, DWORD dwPara1, DWORD dwPara2)
{
	tstringstream strInfo;
	if(bSuccess)
	{
		strInfo << _T("Success!	World ");
	}
	else
	{
		strInfo << _T("Failed!	 World ");
	}

	switch(stp)
	{
	case (EWI_ScriptMgr):
		{
			strInfo << _T("ScriptMgr Init!\n");	
		}
		break;
	case (EWI_AttRes):
		{
			strInfo << _T("AttRes Init!\n");	
		}
		break;
	case (EWI_GuildMgr):
		{
			strInfo << _T("GuildMgr Init!\n");	
		}
		break;
	case (EWI_SocialMgr):
		{
			strInfo << _T("SocialMgr Init!\n");	
		}
		break;
	case (EWI_Mall):
		{
			strInfo << _T("Mall Init!\n");	
		}
		break;
	case (EWI_TradeYB):
		{
			strInfo << _T("TradeYB Init!\n");	
		}
		break;
	case (EWI_GroupMgr):
		{
			strInfo << _T("GroupMgr Init!\n");	
		}
		break;
	case (EWI_FameHall):
		{
			strInfo << _T("FameHall Init!\n");	
		}
		break;
	case (EWI_NPCTeamMgr):
		{
			strInfo << _T("NPCTeamMgr Init!\n");	
		}
		break;
	case (EWI_VIPStall):
		{
			strInfo << _T("VIPStall Init!\n");	
		}
		break;
	case (EWI_MapCreator):
		{
			strInfo << _T("MapCreator Init!\n");	
		}
		break;
	case (EWI_WorldNetCmdMgr):
		{
			strInfo << _T("WorldNetCmdMgr Init!\n");	
		}
		break;
	case (EWI_RoleMgr):
		{
			strInfo << _T("RoleMgr Init!\n");	
		}
		break;
	case (EWI_PvPAthleticsSystem):
		{
            strInfo << _T("AthleticsRoom Init!\n");
		}
	case (EWI_QuestMgr):
		{
			strInfo << _T("QuestMgr Init!\n");	
		}
		break;
	case (EWI_MsgInfoMgr):
		{
			strInfo << _T("MsgInfoMgr Init!\n");	
		}
		break;
	case (EWI_VipNetBarMgr):
		{
			strInfo << _T("VipNetBarMgr Init!\n");	
		}
		break;
	case (EWI_LootMgr):
		{
			strInfo << _T("LootMgr Init!\n");	
		}
		break;
	case (EWI_CityMgr):
		{
			strInfo << _T("CityMgr Init!\n");	
		}
		break;
	case (EWI_WorldSession):
		{
			strInfo << _T("WorldSession Init!\n");	
		}
		break;
	case (EWI_DBSession):
		{
			strInfo << _T("DBSession Init!\n");	
		}
		break;
	case (EWI_LoginSession):
		{
			strInfo << _T("LoginSession Init!\n");	
		}
		break;
	case (EWI_BillSession):
		{
			strInfo << _T("BillSession Init!\n");	
		}
		break;
	case (EWI_RtSession):
		{
			strInfo << _T("RtSession Init!\n");	
		}
		break;
	case (EWI_IPGSession):
		{
			strInfo << _T("IpgSession Init!\n");	
		}
		break;
	case (EWI_IPGCashSession):
		{
			strInfo << _T("IpgCashSession Init!\n");	
		}
		break;
#ifdef ON_INTERNET_CAFE
	case (EWI_InternetCafeSession):
		{
			strInfo << _T("InternetCafeSession Init!\n");	
		}
		break;
#endif
	default:
		goto ERROR_END;
		break;
	}

	if(dwPara1 != GT_INVALID &&  dwPara2 == GT_INVALID)
	{
		IMSG(strInfo.str().c_str(),dwPara1,dwPara2);
		ILOG->Write(strInfo.str().c_str(),dwPara1,dwPara2);
	}
	else if(dwPara1 != GT_INVALID &&  dwPara2 != GT_INVALID)
	{
		IMSG(strInfo.str().c_str(),dwPara1,dwPara2);
		ILOG->Write(strInfo.str().c_str(),dwPara1,dwPara2);
	}
	else if(dwPara1 == GT_INVALID &&  dwPara2 == GT_INVALID)
	{
		IMSG(strInfo.str().c_str());
		ILOG->Write(strInfo.str().c_str());
	}

ERROR_END:

	return;
}

static VOID RegisterClearGuildState()
{
	TObjRef<Console> pConsole = "Console";
	pConsole->Register(_T("maskguildstate"), ClearAllGuildState, _T("clear guild state"), 1);
};

static DWORD WINAPI ClearAllGuildState (LPCTSTR szMask)
{
	INT mask = _ttoi(szMask);
	DWORD re = g_guildMgr.MaskAllGuildState(mask);
	IMSG(_T("mask %d guild state(%d)\r\n"),re,mask);
	return 0;
}


BOOL	World::IsFabaoOpen(VOID)
{
	return m_lFabaoOpen == 1;
}

BOOL	World::IsPrintRoleInfoOpen(VOID)
{
	return m_lPrintRoleInfo == 1;
}

INT		World::GetPKRunawayTickCount(VOID)
{
	return nRunawayTickCount;
}

INT World::GetLoginTimeOutTicks(VOID)
{
	return nLoginTimeOutTicks;
}
static DWORD WINAPI CountNumberOfPersonInARegion(LPCTSTR szMap,LPCTSTR szX,LPCTSTR szZ,LPCTSTR szRange);
static VOID RegisterNPIR(VOID)
{
	TObjRef<Console> pConsole = "Console";
	pConsole->Register(_T("rolenum"), CountNumberOfPersonInARegion, _T("rolenum 1 2 3 4"), 4);
}
static DWORD WINAPI CountNumberOfPersonInARegion(LPCTSTR szMap,LPCTSTR szX,LPCTSTR szZ,LPCTSTR szRange)
{
	TObjRef<Util> pUtil = "Util";
	if( pUtil )
	{
		DWORD dwMapID = pUtil->Crc32(szMap);
		MapMgr * pMapMgr = g_mapCreator.GetMapMgr(dwMapID);
		if( !P_VALID(pMapMgr) )
			return GT_INVALID;
		Map * pMap = NULL;
		if( pMapMgr->IsNormalMap() )
		{
			pMap = pMapMgr->GetSingleMap();
			if( !P_VALID(pMap) )
				return GT_INVALID;

			vector<tagVisTile*> vecVisTile;
			INT totalRoleNum = 0;
			Vector3 pos;
			pos.x = _ttoi(szX) * TILE_SCALE;
			pos.z = _ttoi(szZ) * TILE_SCALE;
			float range = _ttoi(szRange) * TILE_SCALE;
			INT num = pMap->GetVisTileEx(pos,range,vecVisTile);
			for( int i = 0 ; i < num; ++i )
			{
				totalRoleNum += vecVisTile[i]->mapRole.Size();
			}

			IMSG(_T("role number is %d \r\n"),totalRoleNum);
			return totalRoleNum;
		}
	}
	return GT_INVALID;
}

static DWORD WINAPI ResetMaxTickValve(LPCTSTR szValue)
{
	sg_nMaxTickValve = _ttoi(szValue);
	return sg_nMaxTickValve;
}
static VOID RegisterResetMaxTickValve(VOID)
{
	TObjRef<Console> pConsole = "Console";
	pConsole->Register(_T("maxtick"), ResetMaxTickValve, _T("maxtick 1"), 1);
}

static VOID LogMaxTickAlert(INT value)
{
	if( sg_nMaxTickValve <= value )
	{
		static INT lastMaxValue = 0;
		static TCHAR tsTime[20] = {0};
		static TCHAR tsMsg[300] = {0};

		if( 0 == lastMaxValue || value > lastMaxValue )
		{
			tsTime[19] = 0;
			SYSTEMTIME st;
			GetSystemTime(&st);
			_stprintf(tsTime,_T("%d-%d-%d %d:%d:%d"),st.wYear,
				st.wMonth,st.wDay,st.wHour + 8,st.wMinute,st.wSecond);
			memset(&st,0,sizeof(st));
			_stprintf(tsMsg,_T("maxtick %d, time %s, online num %d\r\n"),
				value,
				tsTime,
				g_roleMgr.GetOnlineRoleNum());
			tsMsg[299] = 0;
			IMSG(_T("%s"),tsMsg);
			lastMaxValue = value;
		}
	}
}

static VOID RenderMaxTickLog(Console* pConsole)
{
	//if( P_VALID(pConsole) )
	//{
	//	static TCHAR tsTime[20] = {0};
	//	SYSTEMTIME st;
	//	GetSystemTime(&st);
	//	_stprintf(tsTime,_T("%d-%d-%d %d:%d:%d"),st.wYear,
	//		st.wMonth,st.wDay,st.wHour + 8,st.wMinute,st.wSecond);
	//	pConsole->Watch(_T("maxtick"),0 )
	//}
}

BOOL	World::IsSoaringOpen(VOID)
{
	return bSoaringSwitch;
}

BOOL World::IsTianShenZhongOpen()
{
	return bTianShenZhongOpen;
}

BOOL World::IsQiHuaZhiHaiOpen()
{
	return bQiHuaZhiHaiOpen;
}

BOOL World::IsBenLeiLieGuOpen()
{
	return bBenLeiLieGuOpen;
}

BOOL World::IsKillCounterOpen()
{
	return bKillCounterOpen;
}

static DWORD volatile dwStaCurTick = 0;
static DWORD GetCurTickOfWord(VOID)
{
	return dwStaCurTick;
}
static VOID SetCurTickOfWord(DWORD t)
{
	InterlockedExchange((LPLONG)&dwStaCurTick, t);
}



INT		World::GetMaxRoleLevel(VOID)
{
	return nMaxRoleLevel;
}

VOID World::WatchKeyValue( LPCTSTR szFind, LPCTSTR key, DWORD val )
{
	m_pConsole->FuzzyWatch(szFind,key,val);
}

BOOL	World::IsFairyContractOpen(VOID) const
{
	return s_lFairyContractSwitch != 0;
}

BOOL    World::IsWGCheck_OnOpen(VOID)const
{
	return  ((m_lWGCheck_On == 1) ? TRUE : FALSE);
}

BOOL	World::IsGodStrengthenOpen(VOID) const
{
	return bGodStrengthOpen;
}

INT		World::GetGodStrengthenEquipMinQlty(VOID) const
{
	return nGodStengthEquipMinQlty;
}
DWORD	World::GetGodStrengthenCoolDownTime(VOID) CONST
{
	return dwGodStrengthCoolDown;
}
INT		World::GetGodStrengthenAssitEquipMinQlty(VOID) CONST
{
	return nGodStrengthAssitEquipMinQlty;
}
INT		World::GetGodStengthenAssitEquipMinLevel(VOID) CONST
{
	return nGodStrengthAssitEquipMinLevel;
}

static DWORD WINAPI EasyTeamSwitch(LPCTSTR szValue,LPCTSTR szValue1)
{
	BOOL bOpen = _ttoi(szValue);
	ConvenientTeam& EasyTeam = ConvenientTeam::GetInstance();
	EasyTeam.Open(bOpen);
	if( bOpen == FALSE )
	{
		EasyTeam.OpenTeamNotice(FALSE);
	}
	else
	{
		BOOL bNotice = _ttoi(szValue1);
		EasyTeam.OpenTeamNotice(bNotice);
	}
	
	return 0;
}
static VOID RegisterEasyTeamSwitch(VOID)
{
	TObjRef<Console> pConsole = "Console";
	pConsole->Register(_T("easyteam"), EasyTeamSwitch, _T("easyteam 1 1"), 2);
}
static DWORD WINAPI ShutdownServer(LPCTSTR szValue)
{
	LONG lShutdownSecondsDelay = _ttoi(szValue);
	if( lShutdownSecondsDelay <= 0 )
		return 0;
	g_world.SetWillShutdownFlat();
	g_FriendlyShutdownServer.ShutDownTick(lShutdownSecondsDelay*TICK_PER_SECOND);
	return 0;
}
static VOID RegisterShutdownServer(VOID)
{
	TObjRef<Console> pConsole = "Console";
	pConsole->Register(_T("shutdown"), ShutdownServer, _T("shutdown 1"), 1);
}
VOID	World::SetWillShutdownFlat(VOID)
{
	::InterlockedExchange((LPLONG)&m_lWillShutdown ,1);
}
BOOL	World::IsWillShutdown() CONST
{
	if( m_lWillShutdown && g_FriendlyShutdownServer.CanEnterGame() == FALSE )
		return TRUE;
	return FALSE;
}

INT		World::GetFabaoLevelLimit(VOID) CONST
{
	return nFabaoLevelLimit;
}

static VOLATILE LONG lDbReconnectFlag = FALSE;

BOOL	World::GetDBReconnectFlag(VOID) CONST
{
	return lDbReconnectFlag;
}

static VOID RegisterDBReconnectFunc(VOID)
{
	TObjRef<Console> pConsole = "Console";
	pConsole->Register(_T("recondb"), ReconnectDBWithNewTransObj, _T("recondb 1") );
}
static DWORD WINAPI ReconnectDBWithNewTransObj(LPCTSTR szValue)
{
	LONG lConnect = _ttoi(szValue);
	::InterlockedExchange((LPLONG)&lDbReconnectFlag,lConnect);
	return 0;
}


BOOL	World::GetFabaoFoodQualifFlag(VOID) CONST
{
	return lFabaoFoodQualifFlag;
}
BOOL	World::GetFabaoElemStuntFlag(VOID) CONST
{
	return lFabaoElemStuntFlag;
}
tagDWORDTime World::GetOldSerNewPlayerDate(VOID) const
{
	return dwOldSerNewPlayerDate;
}
DWORD World::GetOldSerNewPlayerMaxLevel(VOID) const
{
	return dwOldSerNewPlayerMaxLevel;
}

static VOID RegisterFabaoFoodQualifFunc(VOID)
{
	TObjRef<Console> pConsole = "Console";
	pConsole->Register(_T("fabaofq"), FabaoFoodQuali, _T("fabaofq 1") );
}

static VOID RegisterTigerInfo(VOID)
{
	TObjRef<Console> pConsole = "Console";
	pConsole->Register(_T("tiger"), TigerInfo, _T("tiger") );
}

static DWORD WINAPI TigerInfo()
{
	IMSG( _T("Tiger Profit Info...\r\n") );	
	IMSG( _T("pool_num ...%d\r\n"),TigerMgr::GetProfitNum() );
	IMSG( _T("X1_1info	Time:%d : %d  NotTake:%d \r\n"), TigerMgr::m_nRanTime_X1_1[0],TigerMgr::m_nRanTimeMin_X1_1[0],TigerMgr::m_bRanMark_X1_1[0]);	
	IMSG( _T("X1_1info	Time:%d : %d  NotTake:%d \r\n"), TigerMgr::m_nRanTime_X1_1[1],TigerMgr::m_nRanTimeMin_X1_1[1],TigerMgr::m_bRanMark_X1_1[1]);	
	IMSG( _T("X1_1info	Time:%d : %d  NotTake:%d \r\n"), TigerMgr::m_nRanTime_X1_1[2],TigerMgr::m_nRanTimeMin_X1_1[2],TigerMgr::m_bRanMark_X1_1[2]);	
	IMSG( _T("X1_1info	Time:%d : %d  NotTake:%d \r\n"), TigerMgr::m_nRanTime_X1_1[3],TigerMgr::m_nRanTimeMin_X1_1[3],TigerMgr::m_bRanMark_X1_1[3]);	
	IMSG( _T("X1_1info	Time:%d : %d  NotTake:%d \r\n"), TigerMgr::m_nRanTime_X1_1[4],TigerMgr::m_nRanTimeMin_X1_1[4],TigerMgr::m_bRanMark_X1_1[4]);	

	IMSG( _T("X1_2info	Time:%d : %d  NotTake:%d \r\n"), TigerMgr::m_nRanTime_X1_2[0],TigerMgr::m_nRanTimeMin_X1_2[0],TigerMgr::m_bRanMark_X1_2[0]);	
	IMSG( _T("X1_2info	Time:%d : %d  NotTake:%d \r\n"), TigerMgr::m_nRanTime_X1_2[1],TigerMgr::m_nRanTimeMin_X1_2[1],TigerMgr::m_bRanMark_X1_2[1]);
	IMSG( _T("X1_2info	Time:%d : %d  NotTake:%d \r\n"), TigerMgr::m_nRanTime_X1_2[2],TigerMgr::m_nRanTimeMin_X1_2[2],TigerMgr::m_bRanMark_X1_2[2]);
	IMSG( _T("X1_2info	Time:%d : %d  NotTake:%d \r\n"), TigerMgr::m_nRanTime_X1_2[3],TigerMgr::m_nRanTimeMin_X1_2[3],TigerMgr::m_bRanMark_X1_2[3]);
	IMSG( _T("X1_2info	Time:%d : %d  NotTake:%d \r\n"), TigerMgr::m_nRanTime_X1_2[4],TigerMgr::m_nRanTimeMin_X1_2[4],TigerMgr::m_bRanMark_X1_2[4]);
	IMSG( _T("X1_2info	Time:%d : %d  NotTake:%d \r\n"), TigerMgr::m_nRanTime_X1_2[5],TigerMgr::m_nRanTimeMin_X1_2[5],TigerMgr::m_bRanMark_X1_2[5]);
	IMSG( _T("X1_2info	Time:%d : %d  NotTake:%d \r\n"), TigerMgr::m_nRanTime_X1_2[6],TigerMgr::m_nRanTimeMin_X1_2[6],TigerMgr::m_bRanMark_X1_2[6]);
	IMSG( _T("X1_2info	Time:%d : %d  NotTake:%d \r\n"), TigerMgr::m_nRanTime_X1_2[7],TigerMgr::m_nRanTimeMin_X1_2[7],TigerMgr::m_bRanMark_X1_2[7]);
	IMSG( _T("X1_2info	Time:%d : %d  NotTake:%d \r\n"), TigerMgr::m_nRanTime_X1_2[8],TigerMgr::m_nRanTimeMin_X1_2[8],TigerMgr::m_bRanMark_X1_2[8]);
	IMSG( _T("X1_2info	Time:%d : %d  NotTake:%d \r\n"), TigerMgr::m_nRanTime_X1_2[9],TigerMgr::m_nRanTimeMin_X1_2[9],TigerMgr::m_bRanMark_X1_2[9]);

	IMSG( _T("X5_1info	Time:%d : %d  NotTake:%d \r\n"), TigerMgr::m_nRanTime_X5_1[0],TigerMgr::m_nRanTimeMin_X5_1[0],TigerMgr::m_bRanMark_X5_1[0]);	
	IMSG( _T("X5_1info	Time:%d : %d  NotTake:%d \r\n"), TigerMgr::m_nRanTime_X5_1[1],TigerMgr::m_nRanTimeMin_X5_1[1],TigerMgr::m_bRanMark_X5_1[1]);	
	IMSG( _T("X5_1info	Time:%d : %d  NotTake:%d \r\n"), TigerMgr::m_nRanTime_X5_1[2],TigerMgr::m_nRanTimeMin_X5_1[2],TigerMgr::m_bRanMark_X5_1[2]);	

	IMSG( _T("X5_2info	Time:%d : %d  NotTake:%d \r\n"), TigerMgr::m_nRanTime_X5_2[0],TigerMgr::m_nRanTimeMin_X5_2[0],TigerMgr::m_bRanMark_X5_2[0]);	
	IMSG( _T("X5_2info	Time:%d : %d  NotTake:%d \r\n"), TigerMgr::m_nRanTime_X5_2[1],TigerMgr::m_nRanTimeMin_X5_2[1],TigerMgr::m_bRanMark_X5_2[1]);
	IMSG( _T("X5_2info	Time:%d : %d  NotTake:%d \r\n"), TigerMgr::m_nRanTime_X5_2[2],TigerMgr::m_nRanTimeMin_X5_2[2],TigerMgr::m_bRanMark_X5_2[2]);
	IMSG( _T("X5_2info	Time:%d : %d  NotTake:%d \r\n"), TigerMgr::m_nRanTime_X5_2[3],TigerMgr::m_nRanTimeMin_X5_2[3],TigerMgr::m_bRanMark_X5_2[3]);
	IMSG( _T("X5_2info	Time:%d : %d  NotTake:%d \r\n"), TigerMgr::m_nRanTime_X5_2[4],TigerMgr::m_nRanTimeMin_X5_2[4],TigerMgr::m_bRanMark_X5_2[4]);
	IMSG( _T("X5_2info	Time:%d : %d  NotTake:%d \r\n"), TigerMgr::m_nRanTime_X5_2[5],TigerMgr::m_nRanTimeMin_X5_2[5],TigerMgr::m_bRanMark_X5_2[5]);

	IMSG( _T("X10_1info	Time:%d : %d  NotTake:%d \r\n"), TigerMgr::m_nRanTime_X10_1[0],TigerMgr::m_nRanTimeMin_X10_1[0],TigerMgr::m_bRanMark_X10_1[0]);	

	IMSG( _T("X10_2info	Time:%d : %d  NotTake:%d \r\n"), TigerMgr::m_nRanTime_X10_2[0],TigerMgr::m_nRanTimeMin_X10_2[0],TigerMgr::m_bRanMark_X10_2[0]);	
	IMSG( _T("X10_2info	Time:%d : %d  NotTake:%d \r\n"), TigerMgr::m_nRanTime_X10_2[1],TigerMgr::m_nRanTimeMin_X10_2[1],TigerMgr::m_bRanMark_X10_2[1]);

	IMSG( _T("X50_1info	Time:%d : %d  NotTake:%d \r\n"), TigerMgr::m_nRanTime_X50_1[0],TigerMgr::m_nRanTimeMin_X50_1[0],TigerMgr::m_bRanMark_X50_1[0]);	

	IMSG( _T("X50_2info	Time:%d : %d  NotTake:%d \r\n"), TigerMgr::m_nRanTime_X50_2[0],TigerMgr::m_nRanTimeMin_X50_2[0],TigerMgr::m_bRanMark_X50_2[0]);	
	IMSG( _T("X50_2info	Time:%d : %d  NotTake:%d \r\n"), TigerMgr::m_nRanTime_X50_2[1],TigerMgr::m_nRanTimeMin_X50_2[1],TigerMgr::m_bRanMark_X50_2[1]);
	return 0;
}
static DWORD WINAPI FabaoFoodQuali(LPCTSTR szValue)
{
	LONG lValue = _ttoi(szValue);
	::InterlockedExchange((LPLONG)&lFabaoFoodQualifFlag,lValue);
	return 0;
}


static VOID RegisterFabaoElemStuntFunc(VOID)
{
	TObjRef<Console> pConsole = "Console";
	pConsole->Register(_T("fabaoes"), FabaoElemStunf, _T("fabaoes 1") );
}
static DWORD WINAPI FabaoElemStunf(LPCTSTR szValue)
{
	LONG lValue = _ttoi(szValue);
	::InterlockedExchange((LPLONG)&lFabaoElemStuntFlag,lValue);
	return 0;
}

static DWORD WINAPI ReloadItemNeedLog();
static VOID RegisterReloadItemNeedlog(VOID)
{
	TObjRef<Console> pConsole = "Console";
	pConsole->Register(_T("reloaditemneedlog"), ReloadItemNeedLog, _T("reloaditemneedlog") );
}
static DWORD WINAPI ReloadItemNeedLog()
{
	// 发送获取物品是否保存信息
	tagNDBC_LoadItemNeedLog itemNeedLog;

	g_dbSession.Send(&itemNeedLog, itemNeedLog.dwSize);
	return 0;
}
// 是否使用配置文件设置的映射ip和端口
BOOL	World::IsUsingConfigWanIP() const
{
	return bUseConfigWANIP;
}
BOOL	World::IsFairyMatingOpen() const
{
	return bOpenFairyMating;
}

static DWORD WINAPI OpenFabaoMating(LPCTSTR szValue)
{
	LONG lValue = _ttoi(szValue);
	::InterlockedExchange((LPLONG)&bOpenFairyMating,lValue);
	return 0;
}
static VOID RegisterOpenFabaoMating(VOID)
{
	TObjRef<Console> pConsole = "Console";
	pConsole->Register(_T("fabaomating"), OpenFabaoMating, _T("fabaomating 1") );
}

DWORD	World::GetCurTick() const
{
	return GetCurTickOfWord();
}
BOOL	World::IsOverseasVersionShieldingOpen() const
{
	return bOpenShielding;
}
BOOL	World::IsEggBrokerOpen() const
{
	return bOpenEggBroker;
}

static DWORD WINAPI OpenShielding(LPCTSTR szValue)
{
	LONG lValue = _ttoi(szValue);
	::InterlockedExchange((LPLONG)&bOpenShielding,lValue);
	return 0;
}

static VOID RegisterShielding(VOID)
{
	TObjRef<Console> pConsole = "Console";
	pConsole->Register(_T("shielding"), OpenShielding, _T("shielding 1") );
}

static DWORD WINAPI OpenEggBroker(LPCTSTR szValue)
{
	LONG lValue = _ttoi(szValue);
	::InterlockedExchange((LPLONG)&bOpenEggBroker,lValue);
	return 0;
}

static VOID RegisterEggBroker(VOID)
{
	TObjRef<Console> pConsole = "Console";
	pConsole->Register(_T("eggbroker"), OpenEggBroker, _T("eggbroker 1") );
}
BOOL 	World::IsPlayerBack() const
{
	return bPlayerBack;
}
VOID	World::InputConsoleCmd(LPCTSTR strCmd)
{
	TObjRef<Console> pConsole = "Console";
	pConsole->SetCommand( strCmd );
}
VOID	World::CountNetMsg(LPVOID pMsg,DWORD dwSize)
{
	if(lCountProtocolSwitch == 0)
		return;
	struct tagTemHeader
	{
		DWORD	dwID;
		DWORD	dwSize;
	} * pHeader ;
	pHeader = (tagTemHeader*)pMsg;
	if(P_VALID(pHeader) && pHeader->dwSize == dwSize)
		_protocol_counter.Count(pHeader->dwID, dwSize);
}
static DWORD WINAPI OpenNetMsgCounter(LPCTSTR szValue)
{
	LONG lValue = _ttoi(szValue);
	BOOL b = lCountProtocolSwitch;
	::InterlockedExchange((LPLONG)&lCountProtocolSwitch,lValue);
	if( !b && lCountProtocolSwitch )
	{
		dwDumpNetCmdCounterTicks = DUMP_NETCMDCOUNTER_TICk;
		::InterlockedExchange((LPLONG)&lLoadProtocolIDFile,1);
	}
	return 0;
}

static VOID RegisterMsgCounter(VOID)
{
	TObjRef<Console> pConsole = "Console";
	pConsole->Register(_T("netmsgcounter"), OpenNetMsgCounter, _T("netmsgcounter 1") );
}

static LPCSTR GetDumpNetCmdCountersFileName(VOID)
{
	static char _filename[MAX_PATH+100] = {0};
	_filename[0] = 0;
	tagDWORDTime now = GetCurrentDWORDTime();
	sprintf/*_stprintf*/(_filename,"log\\netcmdcounter%02d-%02d %02d%02d.txt",now.month,now.day,now.hour,now.min);
	return _filename;
}

#undef DUMP_NETCMDCOUNTER_TICk

BOOL	World::IsPkConstraintsOpen() const
{
	return lPKConstraintsOpen;
}
INT		World::GetPKMiniLevel() const
{
	return nPKConstraintsMinLevel;
}
BOOL	World::IsGodSystemOpen(VOID) CONST
{
	return lGodSystemSwitch;
}

INT World::GetDeductMorality() const
{
	return nDeductMorality;
}

INT World::GetMoralityTime() const
{
	return nMoralityTime;
}

INT World::GetReturnMorality() const
{
	return nReturnMorality;
}

INT World::GetDistance() const
{
	return nDistance;
}

BOOL World::IsUseIPGSystem() const
{
	return bUseIPGSystem;
}

DWORD World::GetGetCashPort() const
{
	return s_dwGetCashPort;
}

DWORD World::GetIOCashPort() const
{
	return s_dwIOCashPort;
}

CHAR* World::GetIPGIP() const
{
	return s_szIPGIP;
}
//获得本机mac地址
void __fastcall World::GetMAC(string& strMAC)   
{
	DWORD   Err;     
	PIP_ADAPTER_INFO   pAdapterInfo,   pAdapt;   
	DWORD   AdapterInfoSize;   

	AdapterInfoSize = 0;   
	if((Err = ::GetAdaptersInfo(NULL,&AdapterInfoSize))!=0)   
	{   
		if(Err != ERROR_BUFFER_OVERFLOW)   
		{   
			IMSG(_T("GetAdaptersInfo sizing failed with error: %u\n"), Err);   
			strMAC = "";
			return;   
		}   
	}   

	if((pAdapterInfo = (PIP_ADAPTER_INFO)GlobalAlloc(GPTR, AdapterInfoSize))==NULL)   
	{   
		IMSG(_T("Memory   allocation   error\n"));   
		strMAC = "";
		return;   
	}   

	//   Get   actual   adapter   information   
	if((Err = ::GetAdaptersInfo(pAdapterInfo,  &AdapterInfoSize))!= 0)   
	{   
		IMSG(_T("GetAdaptersInfo failed with error %u\n"), Err);   
		strMAC = "";
		return;   
	}   

	pAdapt = pAdapterInfo;
	char szBuff[128] = {0};
	std::string str("");   
	if(pAdapt) 
	{
		int  i=1;   	
		for(int i=0; i<6; i++)   
		{      
			INT64 n64Value = INT64(pAdapt->Address[i]);
			std::string strTemp(_i64toa(n64Value, szBuff, 16));
			transform(strTemp.begin(), strTemp.end(), strTemp.begin(), toupper);
			str = str + strTemp;
		}   
	}
	strMAC = str;
}

CHAR* World::GetIP(VOID) const 
{
	return s_szIP;
}

CHAR* World::GetMAC(VOID) const
{
	return s_szMac;
}

CHAR* World::GetMD5(VOID) const
{
	return s_szMD5;
}

CHAR* World::GetSign(VOID) const
{
	return s_szSign;
}

BOOL World::IsOneSign(CHAR *pszSign)
{
	if(NULL == pszSign)
		return FALSE;

	for( int i = 0; i < 40; ++i )
	{
		if( 0 == strcmp( s_szSignArray[i], pszSign ) )
			return TRUE;
	}

	return FALSE;
}

void World::SetPuperStarsNum(BYTE n)
{
	if(n >= 0 && n < 10)
	{
		InterlockedExchangeAdd((LPLONG)&m_LPuperStars[n], 1); 
	}
}

void World::OutputStarsLog()
{
	//输出log
	SYSTEMTIME sys_time;
	GetLocalTime(&sys_time);
	TCHAR szTemp;
	// 记录到log文件
	m_pLog->Write(_T("EquipGrowStars 1star:%d 2stars:%d 3stars:%d 4stars:%d 5stars:%d 6stars:%d 7stars:%d 8stars:%d 9stars:%d 10stars:%d %02d/%02d/%04d %02d:%02d:%02d\r\n"),
		m_LPuperStars[0],m_LPuperStars[1],m_LPuperStars[2],m_LPuperStars[3],m_LPuperStars[4],m_LPuperStars[5],m_LPuperStars[6],m_LPuperStars[7],m_LPuperStars[8],m_LPuperStars[9], 
		sys_time.wMonth, sys_time.wDay, sys_time.wYear,
		sys_time.wHour, sys_time.wMinute, sys_time.wSecond);
	for(int i = 0; i < 10; i++)
	{
		m_LPuperStars[i] = 0;	//清空
	}
}

VOID World::WriteKroeaLog(TCHAR *ptszLog)
{
	return;
	if(NULL == m_hKoreaLogFile)
		return;

	if(NULL == ptszLog)
		return;

	DWORD dwWrite = 0;
	WriteFile(m_hKoreaLogFile,ptszLog,_tcslen(ptszLog)*sizeof(TCHAR),&dwWrite,NULL);
}

//元神是否开放
bool   World::IsHolySoulOpen(void)
{
	return bHolySoulOn;
}

LPTSTR World::CreateLogAnalyzeFileName()
{
	TCHAR _filename[MAX_PATH+100] = {0};
	_filename[0] = 0;
	tagDWORDTime now = GetCurrentDWORDTime();
	_stprintf(_filename, _T("log\\LogNetAnalyze%02d-%02d %02d%02d.log"), now.month,now.day,now.hour,now.min);
	return _filename;
}

//冲星强化
bool World::IsStarEnhanceOpent()const
{
	return bStarEnhance;
}