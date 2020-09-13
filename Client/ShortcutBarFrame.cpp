#include "StdAfx.h"
#include "ShortcutBarFrame.h"
#include "ButtonEx.h"
#include "ShortcutButton.h"
#include "DragBtn.h"
#include "GameClientDefine.h"
#include "SkillMgr.h"
#include "ToolTipFrame.h"
#include "ToolTipCreator.h"
#include "RoleEvent.h"
#include "LevelUpData.h"
#include "..\WorldDefine\skill_define.h"
#include "ItemEvent.h"
#include "ItemMgr.h"
#include "SkillEvent.h"
#include "CombatSysUtil.h"
#include "RoleMgr.h"
#include "LocalPlayer.h"
#include "PlayerNameTab.h"
#include "Container.h"
#include "ItemProtoData.h"
#include "ShortcutEvent.h"
#include "WorldPickerFrame.h"
#include "CombatActionChecker.h"
#include "SkillProtoData.h"
#include "..\WorldDefine\msg_virgin_online.h"
#include "PetDef.h"
#include "PetManager.h"
#include "PetProtoData.h"
#include "PersonalityActData.h"
#include "..\WorldDefine\msg_talent.h"
#include "CombatSys.h"
#include "GodMgr.h"
#include "HolyManFrame.h"
#include "HolyManMgr.h"

TCHAR* ShortcutBarFrame::m_szAccArray[BAR_TITLE_COUNT+BAR_COUNT_TIPS] = { _T("1"), _T("2"),_T("3"),_T("4"),_T("5"),_T("6"),_T("7"),_T("8"),_T("9"),_T("0"),_T("-"),_T("="), _T("c.+F1"), _T("c.+F2"), _T("c.+F3"), _T("c.+F4") };
int ShortcutBarFrame::m_nAccArray[BAR_TITLE_COUNT+BAR_COUNT_TIPS] = { DIK_1, DIK_2, DIK_3, DIK_4, DIK_5, DIK_6, DIK_7, DIK_8, DIK_9, DIK_0, DIK_MINUS, DIK_EQUALS, ((DIK_F1<<16)+DIK_LCONTROL), ((DIK_F2<<16)+DIK_LCONTROL), ((DIK_F3<<16)+DIK_LCONTROL), ((DIK_F4<<16)+DIK_LCONTROL) };
TCHAR* ShortcutBarFrame::m_szBarGroup[BAR_COUNT_TIPS] = {_T("Num1"), _T("Num2"), _T("Num3"), _T("Num4")};
byte ShortcutBarFrame::m_byBarGroup[BAR_COUNT_TIPS] = { DIK_NUMPAD1, DIK_NUMPAD2, DIK_NUMPAD3, DIK_NUMPAD4 };
TCHAR* ShortcutBarFrame::m_szMoveArray[BAR_BUTTON_COUNT] = { _T("F1"), _T("F2"), _T("F3"), _T("F4"), _T("F5"), _T("F6"), _T("F7"), _T("F8") };
byte ShortcutBarFrame::m_byMoveArray[BAR_BUTTON_COUNT] = { DIK_F1, DIK_F2, DIK_F3, DIK_F4, DIK_F5, DIK_F6, DIK_F7, DIK_F8 };
TCHAR* ShortcutBarFrame::m_szMoveArray1[BAR_BUTTON_COUNT] = { _T("a.+1"), _T("a.+2"), _T("a.+3"), _T("a.+4"), _T("a.+5"), _T("a.+6"), _T("a.+7"), _T("a.+8") };
int ShortcutBarFrame::m_nMoveArray1[BAR_BUTTON_COUNT] = { ((DIK_1<<16)+DIK_LALT), ((DIK_2<<16)+DIK_LALT), ((DIK_3<<16)+DIK_LALT), ((DIK_4<<16)+DIK_LALT), ((DIK_5<<16)+DIK_LALT), ((DIK_6<<16)+DIK_LALT), ((DIK_7<<16)+DIK_LALT), ((DIK_8<<16)+DIK_LALT) };
TCHAR* ShortcutBarFrame::m_szKeyUpDown[2] = {_T("PageUp"), _T("PageDown")};
int ShortcutBarFrame::m_nKeyUpDown[2] = { DIK_PRIOR, DIK_NEXT };
TCHAR* ShortcutBarFrame::m_szKeyMoveOC[2] = {_T("["), _T("]")};
int ShortcutBarFrame::m_nKeyMoveOC[2] = { DIK_LBRACKET, DIK_RBRACKET };
const float DELAY_TIME = 0.5f;

TCHAR* ShortcutBarFrame::m_szTransSkill[TRANS_BUTTON_COUNT] = { _T("c.+1"), _T("c.+2"), _T("c.+3"), _T("c.+4"), _T("c.+5"), _T("c.+6") };
int ShortcutBarFrame::m_nTransSkill[TRANS_BUTTON_COUNT] = { ((DIK_1<<16)+DIK_LCONTROL), ((DIK_2<<16)+DIK_LCONTROL), ((DIK_3<<16)+DIK_LCONTROL), ((DIK_4<<16)+DIK_LCONTROL), ((DIK_5<<16)+DIK_LCONTROL), ((DIK_6<<16)+DIK_LCONTROL) };

ShortcutBarFrame::ShortcutBarFrame(void):m_Trunk(this)
{
	m_pWnd					= NULL;
	m_pPthBack				= NULL;
	m_pPthShortcutBack		= NULL;
	m_pPthExpBack			= NULL;
	m_pProExp				= NULL;
	m_pStcChoose			= NULL;
	m_pBtnMoveDn			= NULL;
	m_pBtnMoveUp			= NULL;
	m_pWnd_Move1			= NULL;
	m_pWnd_Move2			= NULL;
	m_pWnd_TransSkill		= NULL;
	m_nBarGroupCount		= 0;
	memset( m_pShortcutBtn, 0x00, sizeof( m_pShortcutBtn ) );
	memset( m_QuickBarFlags, 0x00, sizeof( m_QuickBarFlags ) );
	m_nCurExp				= 0;
	m_nCurLev				= 0;
	m_nCurTotalExp			= 0;
	m_bInitShortcut			= FALSE;
	m_nPctNExp				= 0;
	m_bLPAttIsOK			= false;
	m_pBtnDn				= NULL;
	m_pBtnUp				= NULL;
  	m_pAddatt               = NULL;
  	m_pAddTelent            = NULL;
	m_bIsLoadFromDB         = TRUE;
	m_nQuickBar_Col_Count   = 0;
	m_bHaveNotSavedQuickBarData = FALSE ;
	m_bIsChange               = FALSE;
	m_bIsQuit               = FALSE;
	m_bIsReQuest            = FALSE;
	m_fTime = 0.0f;
	m_pWndEvent = NULL;
	m_bSend = false;
	m_dwATTcheckTimer = 0;
	m_pBtnHolyMen = NULL;
}

ShortcutBarFrame::~ShortcutBarFrame(void)
{
}

//-----------------------------------------------------------------------------
// init
//-----------------------------------------------------------------------------
BOOL ShortcutBarFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;
	
	//注册键盘
	RegisterKeyBoard();

	m_pMgr->RegisterEventHandle( _T("tagUpdateLPAttEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&ShortcutBarFrame::OnUpdateLPAttEvent));
	m_pMgr->RegisterEventHandle( _T("tagUpdateLPLevel"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&ShortcutBarFrame::OnEvent));
	m_pMgr->RegisterEventHandle( _T("tagUpdateLPLevelExp"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&ShortcutBarFrame::OnEvent));
	m_pMgr->RegisterEventHandle(_T("Weapon_Swap"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&ShortcutBarFrame::OnWeaponSwap));
	m_pMgr->RegisterEventHandle(_T("Weapon_SwapEx"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&ShortcutBarFrame::OnWeaponSwap));
	m_pMgr->RegisterEventHandle(_T("ItemEvent_UpdateItem"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&ShortcutBarFrame::OnUpdateLPItemEvent));
	m_pMgr->RegisterEventHandle(_T("tagUpdateLPSkillEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&ShortcutBarFrame::OnUpdateLPSkillEvent));
	m_pMgr->RegisterEventHandle(_T("tagRoleGetNameEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&ShortcutBarFrame::OnRecvNetRoleGetName));
	m_pMgr->RegisterEventHandle(_T("tagShortcutCacheEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&ShortcutBarFrame::OnShortcutCacheEvent));
	m_pMgr->RegisterEventHandle(_T("tagRolePickEvent"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&ShortcutBarFrame::OnRolePickEvent));
	m_pMgr->RegisterEventHandle(_T("InitedSkill_Event"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&ShortcutBarFrame::OnInitedSkillEvent));
	m_pMgr->RegisterEventHandle(_T("tagPetAttrChange"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&ShortcutBarFrame::OnDeletePet));	
  	m_pMgr->RegisterEventHandle(_T("ShowAddAtt_Event"),         (FRAMEEVENTPROC)m_Trunk.sfp1(&ShortcutBarFrame::ShowAddAtt));     //注册处理显示隐藏属性投点和天资投点的事件
  	m_pMgr->RegisterEventHandle(_T("HideAddAtt_Event"),         (FRAMEEVENTPROC)m_Trunk.sfp1(&ShortcutBarFrame::HideAddAtt));
  	m_pMgr->RegisterEventHandle(_T("ShowAddTelent_Event"),      (FRAMEEVENTPROC)m_Trunk.sfp1(&ShortcutBarFrame::ShowAddTelent));
  	m_pMgr->RegisterEventHandle(_T("HideAddTelent_Event"),      (FRAMEEVENTPROC)m_Trunk.sfp1(&ShortcutBarFrame::HideAddTelent));
	m_pMgr->RegisterEventHandle(_T("SkillEvent_ClearTalent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&ShortcutBarFrame::OnEventClearTalent));
	m_pMgr->RegisterEventHandle(_T("LocalPlayerTransform"),     (FRAMEEVENTPROC)m_Trunk.sfp1(&ShortcutBarFrame::ShowTransSkill));
	m_pMgr->RegisterEventHandle(_T("LocalPlayerUnTransform"),   (FRAMEEVENTPROC)m_Trunk.sfp1(&ShortcutBarFrame::HideTransSkill));


	m_pMgr->RegisterEventHandle( _T("tagInitLPAttEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&ShortcutBarFrame::OnInitLPAttEvent));
	m_pMgr->RegisterEventHandle( _T("tagResetSkillIdleTime"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&ShortcutBarFrame::OnResetSkillIdleTime));

	m_pMgr->RegisterEventHandle(_T("tagUpdateLPSkillEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&ShortcutBarFrame::OnActiveHolySkillEvent));

    m_pCmdMgr->Register("NS_VirginOnline",          (NETMSGPROC)m_Trunk.sfp2(&ShortcutBarFrame::OnNS_VirginOnline), _T("NS_VirginOnline"));
	m_pCmdMgr->Register("NS_MsgQuickBarData",      (NETMSGPROC)m_Trunk.sfp2(&ShortcutBarFrame::OnNS_MsgQuickBarData),_T("NS_MsgQuickBarData"));
    m_pCmdMgr->Register("NS_MsgIsSavedQuickBarData",      (NETMSGPROC)m_Trunk.sfp2(&ShortcutBarFrame::OnNS_MsgIsSavedQuickBarData),_T("NS_MsgIsSavedQuickBarData"));

	//向服务器要初始值
    DWORD dwLocalID = RoleMgr::Inst()->GetLocalPlayerID();
	tagNC_MsgIsSavedQuickBarData Msgsend;
	Msgsend.dwRoleID = dwLocalID;
	TObjRef<NetSession>()->Send( &Msgsend );

	m_dwIdleTime = 0;
	m_bCheckOn = false;
	m_bNetSessionStartCheck = false;
	
	return TRUE;
}

//-----------------------------------------------------------------------------
// destroy
//-----------------------------------------------------------------------------
BOOL ShortcutBarFrame::Destroy()
{
   
    SaveBtnData();
    
	BOOL ret = GameFrame::Destroy();

	TCHAR szBuff[X_SHORT_NAME] = {0};
	//--注销快捷键函数
	for(int i=0; i<BAR_TITLE_COUNT+BAR_COUNT_TIPS; i++)
	{
		_sntprintf( szBuff, X_SHORT_NAME, g_StrTable[_T("HotKey_Shortcut")], i+1 );
		m_pKeyMap->SetEnable(szBuff,	false);
	}

	//--注销F1-F4
	for(int i=0; i<BAR_COUNT_TIPS; i++)
	{
		_sntprintf( szBuff, X_SHORT_NAME, g_StrTable[_T("HotKey_ShortcutGroup")], i+1 );
		m_pKeyMap->SetEnable(szBuff, false);
	}

	//--注销移动快捷键
	for(int i=0; i<BAR_BUTTON_COUNT; i++)
	{
		_sntprintf( szBuff, X_SHORT_NAME, g_StrTable[_T("HotKey_MoveShortcut1")], i+1 );
		m_pKeyMap->SetEnable(szBuff, false);
		_sntprintf( szBuff, X_SHORT_NAME, g_StrTable[_T("HotKey_MoveShortcut2")], i+1 );
		m_pKeyMap->SetEnable(szBuff, false);
	}

	//--注销上\下切换快捷栏
	m_pKeyMap->SetEnable(g_StrTable[_T("HotKey_TabUp")], false);
	m_pKeyMap->SetEnable(g_StrTable[_T("HotKey_TabDown")], false);

	//--注销打开\关闭移动快捷栏
	for( int i=0; i<2; ++i )
	{
		_sntprintf( szBuff, X_SHORT_NAME, _T("HotKey_OpenOrCloseMove%d"), i );
		m_pKeyMap->SetEnable(g_StrTable[szBuff], false);
	}

	m_pMgr->UnRegisterEventHandler( _T("tagUpdateLPAttEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&ShortcutBarFrame::OnUpdateLPAttEvent));
	m_pMgr->UnRegisterEventHandler( _T("tagUpdateLPLevel"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&ShortcutBarFrame::OnEvent));
	m_pMgr->UnRegisterEventHandler( _T("tagUpdateLPLevelExp"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&ShortcutBarFrame::OnEvent));
	m_pMgr->UnRegisterEventHandler(_T("Weapon_Swap"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&ShortcutBarFrame::OnWeaponSwap));
	m_pMgr->UnRegisterEventHandler(_T("Weapon_SwapEx"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&ShortcutBarFrame::OnWeaponSwap));
	m_pMgr->UnRegisterEventHandler(_T("ItemEvent_UpdateItem"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&ShortcutBarFrame::OnUpdateLPItemEvent));
	m_pMgr->UnRegisterEventHandler(_T("tagUpdateLPSkillEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&ShortcutBarFrame::OnUpdateLPSkillEvent));
	m_pMgr->UnRegisterEventHandler(_T("tagRoleGetNameEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&ShortcutBarFrame::OnRecvNetRoleGetName));
	m_pMgr->UnRegisterEventHandler(_T("tagShortcutCacheEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&ShortcutBarFrame::OnShortcutCacheEvent));
	m_pMgr->UnRegisterEventHandler(_T("tagRolePickEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&ShortcutBarFrame::OnRolePickEvent));
	m_pMgr->UnRegisterEventHandler(_T("InitedSkill_Event"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&ShortcutBarFrame::OnInitedSkillEvent));
	m_pMgr->UnRegisterEventHandler(_T("tagPetAttrChange"),	    (FRAMEEVENTPROC)m_Trunk.sfp1(&ShortcutBarFrame::OnDeletePet));	
  	m_pMgr->UnRegisterEventHandler(_T("ShowAddAtt_Event"),      (FRAMEEVENTPROC)m_Trunk.sfp1(&ShortcutBarFrame::ShowAddAtt));
  	m_pMgr->UnRegisterEventHandler(_T("HideAddAtt_Event"),      (FRAMEEVENTPROC)m_Trunk.sfp1(&ShortcutBarFrame::HideAddAtt));
  	m_pMgr->UnRegisterEventHandler(_T("ShowAddTelent_Event"),   (FRAMEEVENTPROC)m_Trunk.sfp1(&ShortcutBarFrame::ShowAddTelent));
  	m_pMgr->UnRegisterEventHandler(_T("HideAddTelent_Event"),   (FRAMEEVENTPROC)m_Trunk.sfp1(&ShortcutBarFrame::HideAddTelent));
	m_pMgr->UnRegisterEventHandler(_T("SkillEvent_ClearTalent"),(FRAMEEVENTPROC)m_Trunk.sfp1(&ShortcutBarFrame::OnEventClearTalent));

	m_pMgr->UnRegisterEventHandler(_T("LocalPlayerTransform"),     (FRAMEEVENTPROC)m_Trunk.sfp1(&ShortcutBarFrame::ShowTransSkill));
	m_pMgr->UnRegisterEventHandler(_T("LocalPlayerUnTransform"),   (FRAMEEVENTPROC)m_Trunk.sfp1(&ShortcutBarFrame::HideTransSkill));

	m_pMgr->UnRegisterEventHandler( _T("tagInitLPAttEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&ShortcutBarFrame::OnInitLPAttEvent));
	m_pMgr->UnRegisterEventHandler( _T("tagResetSkillIdleTime"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&ShortcutBarFrame::OnResetSkillIdleTime));

	m_pMgr->UnRegisterEventHandler(_T("tagUpdateLPSkillEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&ShortcutBarFrame::OnActiveHolySkillEvent));

    m_pCmdMgr->UnRegister("NS_VirginOnline",      (NETMSGPROC)m_Trunk.sfp2(&ShortcutBarFrame::OnNS_VirginOnline));
	m_pCmdMgr->UnRegister("NS_MsgQuickBarData",      (NETMSGPROC)m_Trunk.sfp2(&ShortcutBarFrame::OnNS_MsgQuickBarData));
	m_pCmdMgr->UnRegister("NS_MsgIsSavedQuickBarData",      (NETMSGPROC)m_Trunk.sfp2(&ShortcutBarFrame::OnNS_MsgIsSavedQuickBarData));
   
	if( P_VALID(m_pWnd) )
	{
		m_pGUI->DestroyWnd(m_pWnd);
		m_pWnd = NULL;
	}
	if( P_VALID(m_pWnd_Move1) )
	{
		m_pGUI->DestroyWnd(m_pWnd_Move1);
		m_pWnd_Move1 = NULL;
	}
	if( P_VALID(m_pWnd_Move2) )
	{
		m_pGUI->DestroyWnd(m_pWnd_Move2);
		m_pWnd_Move2 = NULL;
	}
	if( P_VALID(m_pWnd_TransSkill) )
		m_pGUI->DestroyWnd(m_pWnd_TransSkill);

	if (P_VALID(GetObj("WG_CatchByMe")))
		KillObj("WG_CatchByMe");

	return ret;
}

//-----------------------------------------------------------------------------
// !接收游戏事件
//-----------------------------------------------------------------------------
VOID ShortcutBarFrame::OnEvent(tagGameEvent* pEvent)
{
	if( pEvent->strEventName == _T("tagUpdateLPLevel") )
	{
		tagUpdateLPLevel* pLevel = (tagUpdateLPLevel*)pEvent;
		m_nCurLev = pLevel->nCurLevel;
		const tagExperience* pExp = LevelUpData::Inst()->FindExpData(m_nCurLev);
		if(!P_VALID(pExp))
			return;
		
		m_nCurTotalExp = pExp->nExp;
		if( !P_VALID(m_pProExp) )
			return;
		m_pProExp->SetMaxValue((FLOAT)m_nCurTotalExp);
		m_pProExp->SetValue(0, true);
		m_pPthExpBack->SetText(_T("0%"));
		m_pProExp->SetRefresh(TRUE);
	}
	else if( pEvent->strEventName == _T("tagUpdateLPLevelExp") )
	{
		tagUpdateLPLevelExp* pExp = (tagUpdateLPLevelExp*)pEvent;
		m_nCurExp = pExp->nCurLevelExp;
		if(m_nCurExp>m_nCurTotalExp)
		{
			m_nCurTotalExp = m_nCurExp;
			if( !P_VALID(m_pProExp) )
				return;
			m_pProExp->SetMaxValue((FLOAT)m_nCurTotalExp);
		}
		if( !P_VALID(m_pProExp) )
			return;
		m_pProExp->SetValue((FLOAT)m_nCurExp, true);
		TCHAR szWndText[X_LONG_NAME] = {0};
		_sntprintf(szWndText, X_LONG_NAME, _T("%0.2f%%"), (FLOAT)m_nCurExp/(FLOAT)m_nCurTotalExp*100.0f);
		m_pPthExpBack->SetText(szWndText);
		m_pProExp->SetRefresh(TRUE);

		//发送N%经验达成
		if( m_nCurTotalExp == 0 )
			return;
		INT n = (INT)((FLOAT)m_nCurExp/(FLOAT)m_nCurTotalExp*10.0f);
		if( n>0 && n!=m_nPctNExp )
		{
			m_nPctNExp = n;
			tagPctNExpAchieve evt(_T("tagPctNExpAchieve"), NULL);
			evt.nPctMultiplied = m_nPctNExp;
			m_pMgr->SendEvent(&evt);
		}
	}
}

//-----------------------------------------------------------------------------
// !Update
//-----------------------------------------------------------------------------
VOID ShortcutBarFrame::Update()
{
	m_fTime +=  Cool3D::Kernel::Inst()->GetDeltaTime();
	if( m_bSend && m_fTime > DELAY_TIME && P_VALID(m_pWndEvent) && 
		_tcscmp(m_pWndEvent->GetClassName().c_str(), _T("ShortcutButton")) == 0 )
	{
		tagGUIEvent event(m_pWndEvent, (EGUIEvent)EGUIE_MoveIntoCtrl );
		event.dwParam1 = m_pWndEvent->GetBtnData().GetType();
		event.dwParam2 = m_pWndEvent->GetBtnData().GetData1();
		event.dwParam3 = m_pWndEvent->GetBtnData().GetData2();
		m_pWndEvent->SendEvent(&event);
		m_bSend = false;
	}

	m_dwATTcheckTimer += Kernel::Inst()->GetDeltaTimeDW();
	if (m_dwATTcheckTimer > 1000)
	{
		ReflashShortcutBtn(true);
		m_dwATTcheckTimer = 0;
	}
	//WG_CheckingInPut();

	GameFrame::Update();
}

//注册键盘
void ShortcutBarFrame::RegisterKeyBoard()
{
	TCHAR szBuff[X_SHORT_NAME] = {0};
	//--注册快捷键函数
	for(int i=0; i<BAR_TITLE_COUNT+BAR_COUNT_TIPS; ++i)
	{
		_sntprintf( szBuff, X_SHORT_NAME, g_StrTable[_T("HotKey_Shortcut")], i+1 );
		m_pKeyMap->Register(m_pUtil->Crc32(m_szAccArray[i]), szBuff,	
			(INPUTMAPEVENTHANDLE)m_Trunk.sfp2(&ShortcutBarFrame::OnKey), m_nAccArray[i], TRUE, FALSE, 2, 
			(INPUTMAPCHANGEEVENTHANDLE)m_Trunk.sfp3(&ShortcutBarFrame::OnKeyChange));
	}

	//--注册Num1-Num4
	for(int i=0; i<BAR_COUNT_TIPS; ++i)
	{
		_sntprintf( szBuff, X_SHORT_NAME, g_StrTable[_T("HotKey_ShortcutGroup")], i+1 );
		m_pKeyMap->Register(m_pUtil->Crc32(m_szBarGroup[i]), szBuff, 
			(INPUTMAPEVENTHANDLE)m_Trunk.sfp2(&ShortcutBarFrame::OnKeyEx), m_byBarGroup[i], TRUE, FALSE, 2,
			(INPUTMAPCHANGEEVENTHANDLE)m_Trunk.sfp3(&ShortcutBarFrame::OnKeyExChange));
	}

	//--注册移动快捷键1
	for(int i=0; i<BAR_BUTTON_COUNT; ++i)
	{
		_sntprintf( szBuff, X_SHORT_NAME, g_StrTable[_T("HotKey_MoveShortcut1")], i+1 );
		m_pKeyMap->Register(m_pUtil->Crc32(m_szMoveArray[i]), szBuff,	
			(INPUTMAPEVENTHANDLE)m_Trunk.sfp2(&ShortcutBarFrame::OnKey2Move), m_byMoveArray[i], TRUE, FALSE, 2,
			(INPUTMAPCHANGEEVENTHANDLE)m_Trunk.sfp3(&ShortcutBarFrame::OnKey2MoveChange));
	}

	//--注册移动快捷键2
	for(int i=0; i<BAR_BUTTON_COUNT; ++i)
	{
		_sntprintf( szBuff, X_SHORT_NAME, g_StrTable[_T("HotKey_MoveShortcut2")], i+1 );
		m_pKeyMap->Register(m_pUtil->Crc32(m_szMoveArray1[i]), szBuff,	
			(INPUTMAPEVENTHANDLE)m_Trunk.sfp2(&ShortcutBarFrame::OnKey2Move1), m_nMoveArray1[i], TRUE, FALSE, 2,
			(INPUTMAPCHANGEEVENTHANDLE)m_Trunk.sfp3(&ShortcutBarFrame::OnKey2MoveChange1));
	}

	//--注册变身快捷键
	for(int i=0; i<TRANS_BUTTON_COUNT; ++i)
	{
		_sntprintf( szBuff, X_SHORT_NAME, g_StrTable[_T("HotKey_TransSkill")], i+1 );
		m_pKeyMap->Register(m_pUtil->Crc32(m_szTransSkill[i]), szBuff,	
			(INPUTMAPEVENTHANDLE)m_Trunk.sfp2(&ShortcutBarFrame::OnKeyTransSkill), m_nTransSkill[i], TRUE, FALSE, 2,
			(INPUTMAPCHANGEEVENTHANDLE)m_Trunk.sfp3(&ShortcutBarFrame::OnKeyTransSkillChangel));
	}

	//--注册向上\下切换快捷栏
	m_pKeyMap->Register(m_pUtil->Crc32(m_szKeyUpDown[0]), g_StrTable[_T("HotKey_TabUp")],	
		(INPUTMAPEVENTHANDLE)m_Trunk.sfp2(&ShortcutBarFrame::OnKeyUpDown), m_nKeyUpDown[0], TRUE, FALSE, 2);
	m_pKeyMap->Register(m_pUtil->Crc32(m_szKeyUpDown[1]), g_StrTable[_T("HotKey_TabDown")],	
		(INPUTMAPEVENTHANDLE)m_Trunk.sfp2(&ShortcutBarFrame::OnKeyUpDown), m_nKeyUpDown[1], TRUE, FALSE, 2);

	//--注册打开\关闭移动快捷栏
	for( int i=0; i<2; ++i )
	{
		_sntprintf( szBuff, X_SHORT_NAME, _T("HotKey_OpenOrCloseMove%d"), i );
		m_pKeyMap->Register(m_pUtil->Crc32(m_szKeyMoveOC[i]), g_StrTable[szBuff],	
			(INPUTMAPEVENTHANDLE)m_Trunk.sfp2(&ShortcutBarFrame::OnKey2MoveOpenOrClose), m_nKeyMoveOC[i], TRUE, 0, 2);
	}
	
}

//-----------------------------------------------------------------------------
// !加载控件
//-----------------------------------------------------------------------------
BOOL ShortcutBarFrame::ReloadUI()
{
	XmlElement element;
	tstring strPath = g_strLocalPath +  _T("\\ui\\shortcutbar.xml");
	m_pGUI->LoadXml(&element, "VFS_System", strPath.c_str());

	//调整位置
	GUIWnd* pDesktop = m_pGUI->GetDesktop();
	std::stringstream streamX, streamY;
	streamX << 0;
	streamY << (pDesktop->GetSize().y - 100);
	m_pGUI->ChangeXml(&element, "shortcutbar", "LogicPos_x", streamX.str().c_str());
	m_pGUI->ChangeXml(&element, "shortcutbar", "LogicPos_y", streamY.str().c_str());

	//修改基本快捷栏按钮的类
	for( int i=0; i<2; i++ )
	{
		for(int j=0; j<BAR_BUTTON_COUNT; j++)						
		{
			char temp[256] = {0};
			sprintf(temp, "shortcutbar\\barback\\barpic\\butt%d", i*8+j);
			BOOL m = m_pGUI->ChangeXml(&element, temp, "ClassName", "ShortcutButton");		
			m_pGUI->ChangeXml(&element, temp, "DisableTextColor", "0xFFFFFFFF");
			m_pGUI->ChangeXml(&element, temp, "DisablePicColor", "0xFF5B5B5B");
			m_pGUI->ChangeXml(&element, temp, "PushTextColor", "0xFFFFF2A4");
			m_pGUI->ChangeXml(&element, temp, "MoveTextColor", "0xFFFFF2A4");
		}
	}
	m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &element);


	//移动快捷栏1
	tstring strPath1 = g_strLocalPath +  _T("\\ui\\m_shortcutbar.xml");
	m_pGUI->LoadXml(&element, "VFS_System", strPath1.c_str());
	//m_pGUI->ChangeXml(&element, "mshortcutbar_win", "AlignControl", "\\desktop\\shortcutbar");
	//m_pGUI->ChangeXml(&element, "mshortcutbar_win", "AlignType", "OuterTopRight");
	for( int i=0; i<BAR_BUTTON_COUNT; i++ )
	{
		char temp[256] = {0};
		sprintf(temp, "mshortcutbar_win\\msbackpic\\butt%d", i);
		BOOL m = m_pGUI->ChangeXml(&element, temp, "ClassName", "ShortcutButton");		
		m_pGUI->ChangeXml(&element, temp, "DisableTextColor", "0xFFFFFFFF");
		m_pGUI->ChangeXml(&element, temp, "DisablePicColor", "0xFF5B5B5B");
		m_pGUI->ChangeXml(&element, temp, "PushTextColor", "0xFFFFF2A4");
		m_pGUI->ChangeXml(&element, temp, "MoveTextColor", "0xFFFFF2A4");
	}
	m_pWnd_Move1 = m_pGUI->CreateWnd(_T("\\desktop"), &element);

	//移动快捷栏2
	tstring strPath2 = g_strLocalPath +  _T("\\ui\\m_shortcutbar2.xml");
	m_pGUI->LoadXml(&element, "VFS_System", strPath2.c_str());
	//m_pGUI->ChangeXml(&element, "mshortcutbar_win2", "AlignControl", "\\desktop\\mshortcutbar_win");
	//m_pGUI->ChangeXml(&element, "mshortcutbar_win2", "AlignType", "OuterTopRight");
	for( int i=0; i<BAR_BUTTON_COUNT; i++ )
	{
		char temp[256] = {0};
		sprintf(temp, "mshortcutbar_win2\\msbackpic2\\butt%d", i);
		BOOL m = m_pGUI->ChangeXml(&element, temp, "ClassName", "ShortcutButton");		
		m_pGUI->ChangeXml(&element, temp, "DisableTextColor", "0xFFFFFFFF");
		m_pGUI->ChangeXml(&element, temp, "DisablePicColor", "0xFF5B5B5B");
		m_pGUI->ChangeXml(&element, temp, "PushTextColor", "0xFFFFF2A4");
		m_pGUI->ChangeXml(&element, temp, "MoveTextColor", "0xFFFFF2A4");
	}
	m_pWnd_Move2 = m_pGUI->CreateWnd(_T("\\desktop"), &element);

	// 变身快捷栏
	tstring strPath3 = g_strLocalPath +  _T("\\ui\\xpskill.xml");
	m_pGUI->LoadXml(&element, "VFS_System", strPath3.c_str());
	for( int i=0; i<TRANS_BUTTON_COUNT; i++ )
	{
		char temp[256] = {0};
		sprintf(temp, "xpskill_win\\skillback\\butt%d", i);
		BOOL m = m_pGUI->ChangeXml(&element, temp, "ClassName", "ShortcutButton");		
		m_pGUI->ChangeXml(&element, temp, "DisableTextColor", "0xFFFFFFFF");
		m_pGUI->ChangeXml(&element, temp, "DisablePicColor", "0xFF5B5B5B");
		m_pGUI->ChangeXml(&element, temp, "PushTextColor", "0xFFFFF2A4");
		m_pGUI->ChangeXml(&element, temp, "MoveTextColor", "0xFFFFF2A4");
	}
	m_pWnd_TransSkill = m_pGUI->CreateWnd(_T("\\desktop"), &element);

	//注册事件处理函数
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&ShortcutBarFrame::EventHandler));
	m_pGUI->RegisterEventHandler(m_pWnd_Move1->GetFullName().c_str(), m_Trunk.sfp1(&ShortcutBarFrame::EventHandler));
	m_pGUI->RegisterEventHandler(m_pWnd_Move2->GetFullName().c_str(), m_Trunk.sfp1(&ShortcutBarFrame::EventHandler));
	m_pGUI->RegisterEventHandler(m_pWnd_TransSkill->GetFullName().c_str(), m_Trunk.sfp1(&ShortcutBarFrame::EventHandler));

	m_pPthBack		= (GUIStatic*)m_pWnd->GetChild(_T("barback"));					
	m_pPthShortcutBack  = (GUIStatic*)m_pPthBack->GetChild(_T("barpic"));
	m_pPthExpBack	= (GUIStatic*)m_pPthBack->GetChild(_T("expbarpic"));
	m_pProExp		= (GUIProgress*)m_pPthBack->GetChild(_T("expbarpic\\expbar"));
	m_pStcChoose	= (GUIStatic*)m_pPthBack->GetChild(_T("chossebar"));
	m_pBtnMoveUp	= (GUIButton*)m_pPthBack->GetChild(_T("chossebar\\barup"));
	m_pBtnMoveDn	= (GUIButton*)m_pPthBack->GetChild(_T("chossebar\\bardown"));
	m_pBtnDn		= (GUIButton*)m_pPthBack->GetChild(_T("chossebar0\\bardown"));
	m_pBtnUp		= (GUIButton*)m_pPthBack->GetChild(_T("chossebar0\\barup"));
    m_pAddatt       = (GUIButton*)m_pWnd->GetChild(_T("barback\\butt0"));    //两个投点的小按钮
    m_pAddTelent    = (GUIButton*)m_pWnd->GetChild(_T("barback\\butt1"));
	//m_pBtnHolyMen	= (GUIButton*)m_pPthBack->GetChild(_T("soubutt"));
	//m_pBtnHolyMen->SetInvisible(TRUE);
	//m_pBtnHolyMen->SetEnable(FALSE);
   	m_pAddatt->SetInvisible(TRUE);   //初始隐藏
   	m_pAddTelent->SetInvisible(TRUE);

	//加载基本快捷栏按钮16个
	TCHAR szBarName[50] = {0};
	int row=0;
	for( ; row<2; ++row )
	{
		for(int j=0; j<BAR_BUTTON_COUNT; ++j)
		{
			_stprintf(szBarName, _T("barpic\\butt%d"), row*8+j);
			m_pShortcutBtn[row][j] = (ShortcutButton*)m_pPthBack->GetChild(szBarName);

			if( (row*8+j)<BAR_TITLE_COUNT+BAR_COUNT_TIPS && m_szAccArray[row*8+j]!=NULL && _tcslen(m_szAccArray[row*8+j])>0 )
			{
				m_pShortcutBtn[row][j]->SetKeyBar(m_szAccArray[row*8+j]);			//设置快捷键值
				m_pShortcutBtn[row][j]->SetDIKBar(m_pUtil->Crc32(m_szAccArray[row*8+j]));
			}
		}
	}

	//创建第1组扩展按钮
	CreateShortcutBtnEx("buttA%d");					
	LinkShortcutBtn(_T("barpic\\buttA"), 2);
	//创建第2组扩展按钮
	CreateShortcutBtnEx("buttB%d");					
	LinkShortcutBtn(_T("barpic\\buttB"), 4);
	////创建第3组扩展按钮
	CreateShortcutBtnEx("buttC%d");					
	LinkShortcutBtn(_T("barpic\\buttC"), 6);
	row = 8;

	//加载移动快捷栏1
	for(int j=0; j<BAR_BUTTON_COUNT; ++j)
	{
		_stprintf(szBarName, _T("msbackpic\\butt%d"), j);
		m_pShortcutBtn[row][j] = (ShortcutButton*)m_pWnd_Move1->GetChild(szBarName);
		m_pShortcutBtn[row][j]->SetKeyBar(m_szMoveArray[j]);
		m_pShortcutBtn[row][j]->SetDIKBar(m_pUtil->Crc32(m_szMoveArray[j]));
	}

	++row;
	//加载移动快捷栏2
	for(int j=0; j<BAR_BUTTON_COUNT; ++j)
	{
		_stprintf(szBarName, _T("msbackpic2\\butt%d"), j);
		m_pShortcutBtn[row][j] = (ShortcutButton*)m_pWnd_Move2->GetChild(szBarName);
		m_pShortcutBtn[row][j]->SetKeyBar(m_szMoveArray1[j]);
		m_pShortcutBtn[row][j]->SetDIKBar(m_pUtil->Crc32(m_szMoveArray1[j]));
	}
	ShowMoveShortcutBar(TRUE);
	m_pWnd_Move1->Align();
	ShowMove1ShortcutBar(TRUE);
	m_pWnd_Move2->Align();

	++row;
	// 加载变身快捷栏
	for(int j=0; j<TRANS_BUTTON_COUNT; ++j)
	{
		_stprintf(szBarName, _T("skillback\\butt%d"), j);
		m_pShortcutBtn[row][j] = (ShortcutButton*)m_pWnd_TransSkill->GetChild(szBarName);
		m_pShortcutBtn[row][j]->SetKeyBar(m_szTransSkill[j]);
		m_pShortcutBtn[row][j]->SetDIKBar(m_pUtil->Crc32(m_szTransSkill[j]));
	}

	// 特殊处理了两个
	{
		m_pShortcutBtn[row][6] = (ShortcutButton*)m_pWnd_TransSkill->GetChild( _T("skillback\\butt0"));
		m_pShortcutBtn[row][7] = (ShortcutButton*)m_pWnd_TransSkill->GetChild( _T("skillback\\butt1"));
	}
	m_pWnd_TransSkill->SetInvisible(TRUE);

	//设置进度条
	m_pProExp->SetSpeed(1);
	m_pProExp->SetMaxValue(660);
	m_pProExp->SetTextColor(0xFFFFFF00);
	m_pProExp->SetRefresh();
	m_pPthExpBack->SetText(_T("0.0%"));

	return TRUE;
}

//-----------------------------------------------------------------------------
//创建扩展快捷按钮
//-----------------------------------------------------------------------------
void ShortcutBarFrame::CreateShortcutBtnEx(char* szName)
{
	const int nSize = 36;
	const int nLeft = 10;
	char szBarName[50] = {0};
	
	string strFont = m_pUtil->UnicodeToUnicode8(g_StrTable[_T("Font_MSYH")]);
	for(int i=0; i<BAR_BUTTON_COUNT*2; i++)
	{
		sprintf(szBarName, szName, i);
		XmlElement ele;
		ele.SetAttribute("Name", szBarName);
		ele.SetAttribute("ClassName", "ShortcutButton");
		ele.SetAttribute("Size_x", nSize);
		ele.SetAttribute("Size_y", nSize);
		ele.SetAttribute("LogicPos_x", i*nSize+i*nLeft);
		ele.SetAttribute("LogicPos_y", 1);
		ele.SetAttribute("TextRect_left", 0);
		ele.SetAttribute("TextRect_top", 0);
		ele.SetAttribute("TextRect_right", nSize);
		ele.SetAttribute("TextRect_bottom", nSize);
		ele.SetAttribute("TextAlign", "RightTop");
		ele.SetAttribute("Font", strFont.c_str());
		ele.SetAttribute("FontWidth", 0);
		ele.SetAttribute("FontHeight", 11);
		ele.SetAttribute("TextColor", 0xFFFFFFFF);
		ele.SetAttribute("PicFileName", "data\\ui\\Main\\L_icon_back.bmp");
		ele.SetAttribute("PicColor", 0xFFFFFFFF);
		ele.SetAttribute("DisablePicColor", 0xFF5B5B5B);
		ele.SetAttribute("DisableTextColor", 0xFFFFFFFF);
		ele.SetAttribute("TopColor", 0xFF000000);
		ele.SetAttribute("BottomColor", 0xFFAFAFAF);
		ele.SetAttribute("AlphaWrite", EGUIAW_NoWrite);
		m_pGUI->CreateWnd(_T("\\desktop\\shortcutbar\\barback\\barpic"), &ele);
	}
}

//-----------------------------------------------------------------------------
//加载快捷按钮
//-----------------------------------------------------------------------------
BOOL ShortcutBarFrame::LinkShortcutBtn(TCHAR* szName, int nBar)
{
	TCHAR szBarName[50] = {0};
	int row = 0;
	for( ; row<2; ++row )
	{
		for(int i=0; i<BAR_BUTTON_COUNT; i++)
		{
			_stprintf(szBarName, _T("%s%d"), szName, row*8+i);
			m_pShortcutBtn[nBar+row][i] = (ShortcutButton*)m_pPthBack->GetChild(szBarName);
			m_pShortcutBtn[nBar+row][i]->SetInvisible(TRUE);

			if( (row*8+i)<BAR_TITLE_COUNT+BAR_COUNT_TIPS && m_szAccArray[row*8+i]!=NULL && _tcslen(m_szAccArray[row*8+i])>0 )
			{
				m_pShortcutBtn[nBar+row][i]->SetKeyBar(m_szAccArray[row*8+i]);			//设置快捷键值
				m_pShortcutBtn[nBar+row][i]->SetDIKBar(m_pUtil->Crc32(m_szAccArray[row*8+i]));
			}
		}
	}
	return TRUE;
}	

//-----------------------------------------------------------------------------
//控件事件处理函数
//-----------------------------------------------------------------------------
DWORD ShortcutBarFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	if( !P_VALID(pWnd) )
		return FALSE;
	switch(pEvent->eEvent)
	{
	case EGUIE_Click:
		{
			if( pWnd == m_pBtnMoveUp )				//点击向上按钮 切换快捷栏组
			{
				if( m_pWnd_Move1->IsInvisible()==TRUE )
				{
					m_pWnd_Move1->SetInvisible(FALSE);
					ShowMoveShortcutBar(TRUE);
					m_pWnd_Move1->Align();
				}
				else if( m_pWnd_Move2->IsInvisible()==TRUE )
				{
					m_pWnd_Move2->SetInvisible(FALSE);
					ShowMove1ShortcutBar(TRUE);
					m_pWnd_Move2->Align();
				}
				
			}
			else if( pWnd == m_pBtnMoveDn )
			{
				if( m_pWnd_Move2->IsInvisible()==FALSE )
				{
					m_pWnd_Move2->SetInvisible(TRUE);
					ShowMove1ShortcutBar(FALSE);
				}
				else if( m_pWnd_Move1->IsInvisible()==FALSE )
				{
					m_pWnd_Move1->SetInvisible(TRUE);
					ShowMoveShortcutBar(FALSE);
				}
				
			}
			else if( pWnd == m_pBtnDn )
			{
				int LastBarGroupCount = m_nBarGroupCount;
				m_nBarGroupCount -= 2;
				if( m_nBarGroupCount < 0 )
				m_nBarGroupCount = 6;
				HideBar(LastBarGroupCount);
				ShowBar(m_nBarGroupCount);
				m_pBtnUp->SetRefresh();
			}
			else if( pWnd == m_pBtnUp )
			{
				int LastBarGroupCount = m_nBarGroupCount;
				m_nBarGroupCount += 2;
				if( m_nBarGroupCount > 6 )
					m_nBarGroupCount = 0;
				HideBar(LastBarGroupCount);
				ShowBar(m_nBarGroupCount);
				m_pBtnDn->SetRefresh();
			}
 			else if( pWnd == m_pAddatt)         //处理点击属性投点和天资投点的小按钮事件
  			{
   				m_pMgr->SendEvent( &tagGameEvent( _T("Open_Role_UI"), this ) );
   				m_pMgr->SendEvent( &tagGameEvent( _T("OpenEquipState"), this ) );
  			}
 			else if( pWnd == m_pAddTelent)
 			{
  				m_pMgr->SendEvent( &tagGameEvent( _T("Open_Dower_UI"), this ) );
  			}
			else if ( pWnd == m_pBtnHolyMen )
			{
				HolyManMgr::Inst()->OpenHolyManFrame();
			}
		}
		break;
	case EGUISCB_LeftMouseDown:					//处理左右鼠标按下按钮
	case EGUISCB_RightMouseDown:
		{
			if( _tcscmp(pWnd->GetClassName().c_str(), _T("ShortcutButton")) == 0 )
			{
				ShortcutButton* pBtn = (ShortcutButton*)m_pGUI->GetWnd(pEvent->dwWndID);
				if( !P_VALID(pBtn) )
					return FALSE;
				const BtnData btnData = pBtn->GetBtnData();
				if(pBtn->m_bCanUse && btnData.GetType()==ESBT_Item)
				{
					pBtn->BeginPressBtn();
					pBtn->InvokeGameEvent();
				}
				else if(pBtn->m_bCanUse && btnData.GetType()==ESBT_Skill)
				{
					m_dwIdleTime = 0;
					pBtn->BeginPressBtn();
					pBtn->InvokeGameEvent();
                }
                else if ( pBtn->m_bCanUse && btnData.GetType() == ESBT_PetSkill)
                {
                    pBtn->BeginPressBtn();
                    pBtn->InvokeGameEvent();
                }
				else if( btnData.GetType()==ESBT_Skill && !pBtn->m_bCanUse )
				{
					pBtn->BeginPressBtn();
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("SkillSvrErr5")]);
				}
				else if( btnData.GetType()==ESBT_Item && !pBtn->m_bCanUse )
				{
					pBtn->BeginPressBtn();
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ItemSvrErr33")]);
				}
				else if ( pBtn->m_bCanUse && btnData.GetType() == ESBT_StyleAction)
				{
					pBtn->BeginPressBtn();
					pBtn->InvokeGameEvent();
				}
				else if (pBtn->m_bCanUse && btnData.GetType() == ESBT_WarBag)
				{
					pBtn->BeginPressBtn();
					pBtn->InvokeGameEvent();
				}
			}
		}
		break;
	case EGUIE_Drag:
		{
			if (_tcsnccmp(pWnd->GetName().c_str(), _T("butt"), 4) != 0)//拖动背包中的物品
				return FALSE;

			//无法拖动变身技能
			if(pWnd->GetFather()->GetFather()->GetName() == _T("xpskill_win"))
				return FALSE;

			DragBtn* pDrag = (DragBtn*)m_pGUI->GetWnd(_T("\\desktop\\DragButton"));
			if(P_VALID(pDrag))
				pDrag->OnEvent(pEvent);
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			if( _tcscmp(pWnd->GetClassName().c_str(), _T("ShortcutButton")) == 0 )
			{
				ShortcutButton* pBtn = (ShortcutButton*)m_pGUI->GetWnd(pEvent->dwWndID);
				if( !P_VALID(pBtn) )
					return FALSE;
				if(m_fTime < DELAY_TIME)
				{
					m_pWndEvent = pBtn;
					m_bSend = true;
					break;
				}

				const BtnData btnData = pBtn->GetBtnData();
				switch(btnData.GetType())
				{
				case ESBT_Skill:
					{
						const tagSkillData* pSkill = SkillMgr::Inst()->GetSkillDataByTypeID(pEvent->dwParam2);
						if( !P_VALID(pSkill) )
							return FALSE;

						ShowTip(pWnd, ToolTipCreator::Inst()->GetSkillTips(pEvent->dwParam2).c_str());
					}
					break;
				case ESBT_Item:
					{
						Item* pItem = ItemMgr::Inst()->GetPocketItemByID(btnData.GetData3());
						if( !P_VALID(pItem) )
							return FALSE;
						ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pItem).c_str());
					}
					break;
                case ESBT_PetSkill:
                    {
                        ShowTip(pWnd, ToolTipCreator::Inst()->GetPetSkillTips(btnData.GetData2()).c_str());
                    }
                    break;
				case ESBT_WarBag:
					{
						Item* pItem = ItemMgr::Inst()->GetPocketItemByID(btnData.GetData3());
						if( !P_VALID(pItem) )
							return FALSE;
						ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pItem).c_str());
					}
				}
			}
			if( pWnd == m_pProExp )
			{
				if( m_pSession->GetLoginMode() != 3 )
				{
					tstringstream ss;
					ss << m_nCurExp << _T(" / ") << m_nCurTotalExp;
// 					TCHAR szBuff[X_LONG_NAME] = {0};
// 					_stprintf( szBuff, _T("%f/%f"), m_nCurExp, m_nCurTotalExp );
 					tstring strTip = g_StrTable[_T("TipsDefaultFont")];
					strTip += ss.str().c_str();
					ShowTip( m_pProExp, strTip.c_str() );
				}
			}

  			if( pWnd == m_pAddatt)
  			{
  				ShowTip(pWnd,g_StrTable[_T("Addatt")]);
  			}
  			if( pWnd == m_pAddTelent)
  			{
  				ShowTip(pWnd,g_StrTable[_T("Addtelent")]);
  			}
		}
		break;
	case EGUIE_MoveOutOfCtrl:
		{
			m_fTime = 0.0f;
			m_pWndEvent = NULL;
			m_bSend = false;
			ShowTip(pWnd, NULL);
		}
		break;
	}
	return FALSE;
}

//-----------------------------------------------------------------------------
//显示快捷按钮
//-----------------------------------------------------------------------------
void ShortcutBarFrame::ShowBar(int bar)
{
	int row = 0;
	for( ; row<2; ++row )
	{
		for(int i=0; i<BAR_BUTTON_COUNT; i++)
		{
			m_pShortcutBtn[bar+row][i]->SetInvisible(FALSE);
		}
	}
}

//-----------------------------------------------------------------------------
//隐藏快捷按钮
//-----------------------------------------------------------------------------
void ShortcutBarFrame::HideBar(int bar)
{
	int row = 0;
	for( ; row<2; ++row )
	{
		for(int i=0; i<BAR_BUTTON_COUNT; i++)
		{
			m_pShortcutBtn[bar+row][i]->SetInvisible(TRUE);
		}
	}
}

//-----------------------------------------------------------------------------
//获得指定的快捷按钮
//-----------------------------------------------------------------------------
ShortcutButton* ShortcutBarFrame::GetShortcutButtion(int group, int num)
{
	if(group<0 || group>BAR_COUNT)
		return NULL;
	if(num<0 || num>BAR_BUTTON_COUNT)
		return NULL;
	return m_pShortcutBtn[group][num];
}

DWORD ShortcutBarFrame::OnKey( DWORD dwID, BOOL bDown )
{
	if( !m_bLPAttIsOK )
		return 0;

	if( !bDown )
		return 0;

	for( int i=0; i<BAR_TITLE_COUNT+BAR_COUNT_TIPS; i++ )
	{
		if( dwID==m_pUtil->Crc32(m_szAccArray[i]) )
		{
			int row = i/BAR_BUTTON_COUNT;
			int col = i%BAR_BUTTON_COUNT;
			if( row<0 || row>=BAR_COUNT )
				return 0;
			if( col<0 || col>=BAR_BUTTON_COUNT )
				return 0;

			const BtnData btnData = m_pShortcutBtn[m_nBarGroupCount+row][col]->GetBtnData();
			bool bCanUse = m_pShortcutBtn[m_nBarGroupCount+row][col]->m_bCanUse;
			if(bCanUse && btnData.GetType()==ESBT_Skill && bDown)
			{
				m_dwIdleTime = 0;
				m_pShortcutBtn[m_nBarGroupCount+row][col]->BeginPressBtn();
				m_pShortcutBtn[m_nBarGroupCount+row][col]->InvokeGameEvent();
			}
			else if(bCanUse && (btnData.GetType()==ESBT_Item||btnData.GetType()==ESBT_WarBag) && bDown)
			{
				m_pShortcutBtn[m_nBarGroupCount+row][col]->BeginPressBtn();
				m_pShortcutBtn[m_nBarGroupCount+row][col]->InvokeGameEvent();
			}
            else if(bCanUse && btnData.GetType()==ESBT_PetSkill && bDown)
            {
                m_pShortcutBtn[m_nBarGroupCount+row][col]->BeginPressBtn();
                m_pShortcutBtn[m_nBarGroupCount+row][col]->InvokeGameEvent();
            }
			else if( !bCanUse && btnData.GetType()==ESBT_Skill && bDown)
			{
				m_pShortcutBtn[m_nBarGroupCount+row][col]->BeginPressBtn();
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("SkillSvrErr5")]);
			}
			else if( !bCanUse && (btnData.GetType()==ESBT_Item||btnData.GetType()==ESBT_WarBag) && bDown)
			{
				m_pShortcutBtn[m_nBarGroupCount+row][col]->BeginPressBtn();
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ItemSvrErr33")]);
			}
			else if( bCanUse && btnData.GetType()==ESBT_StyleAction && bDown)
			{
				m_pShortcutBtn[m_nBarGroupCount+row][col]->BeginPressBtn();
				m_pShortcutBtn[m_nBarGroupCount+row][col]->InvokeGameEvent();
			}
		}
	}
	return 0;
}

DWORD ShortcutBarFrame::OnKey2Move(DWORD dwID, BOOL bDown)
{
	if( !m_bLPAttIsOK )
		return 0;

	if( !bDown )
		return 0;

	for( int i=0; i<BAR_BUTTON_COUNT; i++ )
	{
		if( dwID==m_pUtil->Crc32(m_szMoveArray[i]) ) 
		{
			const BtnData btnData = m_pShortcutBtn[8][i]->GetBtnData();
			bool bCanUse = m_pShortcutBtn[8][i]->m_bCanUse;

			if(bCanUse && btnData.GetType()==ESBT_Skill && bDown)
			{
				m_pShortcutBtn[8][i]->BeginPressBtn();
				m_pShortcutBtn[8][i]->InvokeGameEvent();
			}
			else if(bCanUse && (btnData.GetType()==ESBT_Item||btnData.GetType()==ESBT_WarBag) && bDown)
			{
				m_pShortcutBtn[8][i]->BeginPressBtn();
				m_pShortcutBtn[8][i]->InvokeGameEvent();
			}
            else if(bCanUse && btnData.GetType()==ESBT_PetSkill && bDown)
            {
                m_pShortcutBtn[8][i]->BeginPressBtn();
                m_pShortcutBtn[8][i]->InvokeGameEvent();
            }
			else if(bCanUse && btnData.GetType()==ESBT_StyleAction && bDown)
			{
				m_pShortcutBtn[8][i]->BeginPressBtn();
				m_pShortcutBtn[8][i]->InvokeGameEvent();
			}
			else if( !bCanUse && btnData.GetType()==ESBT_Skill && bDown)
			{
				m_pShortcutBtn[8][i]->BeginPressBtn();
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("SkillSvrErr5")]);
			}
			else if( !bCanUse && (btnData.GetType()==ESBT_Item||btnData.GetType()==ESBT_WarBag) && bDown)
			{
				m_pShortcutBtn[8][i]->BeginPressBtn();
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ItemSvrErr33")]);
			}
		}
	}

	return 0;
}

DWORD ShortcutBarFrame::OnKey2Move1( DWORD dwID, BOOL bDown )
{
	if( !m_bLPAttIsOK )
		return 0;

	if( !bDown )
		return 0;

	for( int i=0; i<BAR_BUTTON_COUNT; i++ )
	{
		if( dwID==m_pUtil->Crc32(m_szMoveArray1[i]) ) 
		{
			const BtnData btnData = m_pShortcutBtn[9][i]->GetBtnData();
			bool bCanUse = m_pShortcutBtn[9][i]->m_bCanUse;

			if(bCanUse && btnData.GetType()==ESBT_Skill && bDown)
			{
				m_pShortcutBtn[9][i]->BeginPressBtn();
				m_pShortcutBtn[9][i]->InvokeGameEvent();
			}
			else if(bCanUse && (btnData.GetType()==ESBT_Item||btnData.GetType()==ESBT_WarBag) && bDown)
			{
				m_pShortcutBtn[9][i]->BeginPressBtn();
				m_pShortcutBtn[9][i]->InvokeGameEvent();
			}
            else if(bCanUse && btnData.GetType()==ESBT_PetSkill && bDown)
            {
                m_pShortcutBtn[9][i]->BeginPressBtn();
                m_pShortcutBtn[9][i]->InvokeGameEvent();
            }
			else if(bCanUse && btnData.GetType()==ESBT_StyleAction && bDown)
			{
				m_pShortcutBtn[9][i]->BeginPressBtn();
				m_pShortcutBtn[9][i]->InvokeGameEvent();
			}
			else if( !bCanUse && btnData.GetType()==ESBT_Skill && bDown)
			{
				m_pShortcutBtn[9][i]->BeginPressBtn();
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("SkillSvrErr5")]);
			}
			else if( !bCanUse && (btnData.GetType()==ESBT_Item||btnData.GetType()==ESBT_WarBag) && bDown)
			{
				m_pShortcutBtn[9][i]->BeginPressBtn();
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ItemSvrErr33")]);
			}
		}
	}

	return 0;
}


DWORD ShortcutBarFrame::OnKeyTransSkill(DWORD dwID, BOOL bDown)
{
	if( !m_bLPAttIsOK )
		return 0;

	if( !bDown )
		return 0;

	for( int i=0; i<TRANS_BUTTON_COUNT; i++ )
	{
		if( dwID==m_pUtil->Crc32(m_szTransSkill[i]) ) 
		{
			const BtnData btnData = m_pShortcutBtn[10][i]->GetBtnData();
			bool bCanUse = m_pShortcutBtn[10][i]->m_bCanUse;

			if(bCanUse && btnData.GetType()==ESBT_Skill && bDown)
			{
				m_pShortcutBtn[10][i]->BeginPressBtn();
				m_pShortcutBtn[10][i]->InvokeGameEvent();
			}
			else if( !bCanUse && btnData.GetType()==ESBT_Skill && bDown)
			{
				m_pShortcutBtn[10][i]->BeginPressBtn();
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("SkillSvrErr5")]);
			}
		}
	}

	return 0;
}

DWORD ShortcutBarFrame::OnKeyEx( DWORD dwID, BOOL bDown )
{
	if( !m_bLPAttIsOK )
		return 0;

	if( !bDown )
		return 0;

	for( int i=0; i<BAR_COUNT_TIPS; ++i )
	{
		if( (dwID==m_pUtil->Crc32(m_szBarGroup[i])) && bDown )
		{
			int LastBarGroupCount = m_nBarGroupCount;
			HideBar(LastBarGroupCount);
			m_nBarGroupCount = i*2;
			ShowBar(m_nBarGroupCount);
			break;
		}
	}
	ReflashCDTime();

	return 0;
}

DWORD ShortcutBarFrame::OnKeyUpDown(DWORD dwID, BOOL bDown)
{
	if( dwID == m_pUtil->Crc32(m_szKeyUpDown[0]) && bDown)
	{
		int LastBarGroupCount = m_nBarGroupCount;
		m_nBarGroupCount += 2;
		if( m_nBarGroupCount > 6 )
			m_nBarGroupCount = 0;
		HideBar(LastBarGroupCount);
		ShowBar(m_nBarGroupCount);
		m_pBtnDn->SetRefresh();
	}
	else if( dwID == m_pUtil->Crc32(m_szKeyUpDown[1]) && bDown )
	{
		int LastBarGroupCount = m_nBarGroupCount;
		m_nBarGroupCount -= 2;
		if( m_nBarGroupCount < 0 )
			m_nBarGroupCount = 6;
		HideBar(LastBarGroupCount);
		ShowBar(m_nBarGroupCount);
		m_pBtnUp->SetRefresh();		
	}

	return 0;
}

DWORD ShortcutBarFrame::OnKey2MoveOpenOrClose(DWORD dwID, BOOL bDown)
{
	if( dwID == m_pUtil->Crc32(m_szKeyMoveOC[0]) && bDown )
	{
		if( m_pWnd_Move1->IsInvisible() )
		{
			m_pWnd_Move1->SetInvisible(FALSE);
			ShowMoveShortcutBar(TRUE);
		}
		else 
		{
			m_pWnd_Move1->SetInvisible(TRUE);
			ShowMoveShortcutBar(FALSE);
		}
		m_pWnd_Move1->Align();
	}
	else if( dwID == m_pUtil->Crc32(m_szKeyMoveOC[1]) && bDown )
	{
		if( m_pWnd_Move2->IsInvisible() )
		{
			m_pWnd_Move2->SetInvisible(FALSE);
			ShowMove1ShortcutBar(TRUE);
		}
		else 
		{
			m_pWnd_Move2->SetInvisible(TRUE);
			ShowMove1ShortcutBar(FALSE);
		}
		m_pWnd_Move2->Align();
	}

	return 0;
}

DWORD ShortcutBarFrame::OnWeaponSwap( tagGameEvent* pEvent )
{
	for(int i=0; i<BAR_COUNT; i++)
	{
		for(int j=0; j<BAR_BUTTON_COUNT; j++)
		{
			const BtnData& data = m_pShortcutBtn[i][j]->GetBtnData();
			if( data.GetType() == ESBT_Skill )
			{	
				AutoAdaptFreshHandSkill(m_pShortcutBtn[i][j], data.GetData1());

				//技能必须与主手武器同类
				const tagSkillData* pSkillData=SkillMgr::Inst()->GetSkillDataByTypeID(data.GetData1());
				if( P_VALID(pSkillData) && pSkillData->pProto->nWeaponLimit!=0 
					&& (pSkillData->pProto->nWeaponLimit!=ItemMgr::Inst()->GetHandWeaponType()
					&& pSkillData->pProto->nWeaponLimit!=ItemMgr::Inst()->GetHandWeaponType(EEP_LeftHand)) )
				{
					m_pShortcutBtn[i][j]->SetInvalite(true);
				}
				else if( P_VALID(pSkillData) && pSkillData->pProto->nWeaponLimit!=0
					&& (pSkillData->pProto->nWeaponLimit==ItemMgr::Inst()->GetHandWeaponType()
					|| pSkillData->pProto->nWeaponLimit==ItemMgr::Inst()->GetHandWeaponType(EEP_LeftHand)) )
				{
					m_pShortcutBtn[i][j]->SetInvalite(false);
				}

				DWORD skillID = MTransSkillID(data.GetData1());

				//更换普通攻击贴图				
				if( IsNormalAttackSkill(skillID) )
				{
					tstring strPicFile = SkillMgr::Inst()->GetSkillIconPath(data.GetData1());
					m_pShortcutBtn[i][j]->ClearPic();
					m_pShortcutBtn[i][j]->SetPicFileName(strPicFile);
					m_pShortcutBtn[i][j]->SetInvalite(false);
				}
			}
		}
	}
	return 0;
}

DWORD ShortcutBarFrame::OnUpdateLPItemEvent( tagUpdateLPItemEvent* pEvent )
{
	if( pEvent->eType!= EICT_Bag && pEvent->eType != EICT_WarBag )
		return 0;
	for(int i=0; i<BAR_COUNT; i++)
	{
		for(int j=0; j<BAR_BUTTON_COUNT; j++)
		{
			BtnData btnData = m_pShortcutBtn[i][j]->GetBtnData();
			if(btnData.GetType()==ESBT_Null
				|| btnData.GetType()==ESBT_Skill)
				continue;
			DWORD dwTypeID = btnData.GetData2();
			INT64 n64ID = btnData.GetData3();
			INT16 nItemCount = 0;
			INT16 nItemPos = 0;
			if(EICT_Bag==pEvent->eType && dwTypeID == pEvent->dwTypeID)
			{
				if( !MIsEquipment(dwTypeID) )
				{
					const tagItemProto* pItemProto = ItemProtoData::Inst()->FindItemProto(dwTypeID);
					
					//设置数量
					//是不可堆叠的物品
					if( pItemProto->n16MaxLapNum == 1 )
					{
						ItemContainer* pBag = ItemMgr::Inst()->GetPocket();
						if( P_VALID(pBag) )
						{
							nItemCount = (INT16)pBag ->GetItemQuantity(dwTypeID);
							nItemPos = pBag ->GetItemPosition(dwTypeID);
						}
						if(nItemCount == 0)
							m_pShortcutBtn[i][j]->SetInvalite(true);
						else 
							m_pShortcutBtn[i][j]->SetInvalite(false);
						m_pShortcutBtn[i][j]->SetItemNum(nItemCount);
						DWORD dwParam1 = MAKELONG( nItemPos, nItemCount );

						Item* item = ItemMgr::Inst()->GetPocketItem((INT16)dwParam1);
						if( !P_VALID(item) )
						{
							m_pShortcutBtn[i][j]->SetBtnData( btnData.GetType(), dwParam1, dwTypeID, 0 );
							return 0;
						}

						INT64 n64ID = item->GetItemId();
						m_pShortcutBtn[i][j]->SetBtnData( btnData.GetType(), dwParam1, dwTypeID, n64ID );
					}
					//可堆叠的物品
					else 
					{
						ItemContainer* pBag = ItemMgr::Inst()->GetPocket();
						if( P_VALID(pBag) )
						{
							nItemCount = (INT16)pBag ->GetItemQuantity(dwTypeID);
							nItemPos = pBag ->GetItemPosition(dwTypeID);
						}
						m_pShortcutBtn[i][j]->SetItemNum(nItemCount);
						m_QuickBarFlags[i][j].m_bIsChange = true;
						DWORD dwParam1 = MAKELONG( nItemPos, nItemCount );
						if(nItemCount == 0)
							m_pShortcutBtn[i][j]->SetInvalite(true);
						else 
							m_pShortcutBtn[i][j]->SetInvalite(false);
						
						Item* item = ItemMgr::Inst()->GetPocketItem((INT16)dwParam1);
						if( !P_VALID(item) )
						{
							m_pShortcutBtn[i][j]->SetBtnData( btnData.GetType(), dwParam1, dwTypeID, 0 );
							return 0;
						}

						INT64 n64ID = item->GetItemId();
						m_pShortcutBtn[i][j]->SetBtnData( btnData.GetType(), dwParam1, dwTypeID, n64ID );
					}
				}
				else if( MIsEquipment(dwTypeID) )
				{
					ItemContainer* pBag = ItemMgr::Inst()->GetPocket();
					Item* item;
					if( P_VALID(pBag) )
					{
						//在背包容器里找
						item = pBag->GetItem(n64ID);
						if( P_VALID(item) )
						{
							nItemPos = item->GetPos();
							nItemCount = item->GetItemQuantity();
							DWORD dwParam1 = MAKELONG( nItemPos, nItemCount );
							m_pShortcutBtn[i][j]->SetBtnData( btnData.GetType(), dwParam1, dwTypeID, n64ID );
							m_pShortcutBtn[i][j]->SetInvalite(false);
						}
						else 
						{
							m_pShortcutBtn[i][j]->SetBtnData( btnData.GetType(), GT_INVALID, dwTypeID, n64ID );
							m_pShortcutBtn[i][j]->SetInvalite(true);
						}
					}
					
				}
			}
			else if(EICT_WarBag==pEvent->eType && dwTypeID == pEvent->dwTypeID)
			{
				//只处理物品
				if( !MIsEquipment(dwTypeID) )
				{
					const tagItemProto* pItemProto = ItemProtoData::Inst()->FindItemProto(dwTypeID);

					//设置数量
					//是不可堆叠的物品
					if( pItemProto->n16MaxLapNum == 1 )
					{
						ItemContainer* pBag = ItemMgr::Inst()->GetWarBag();
						if( P_VALID(pBag) )
						{
							nItemCount = (INT16)pBag ->GetItemQuantity(dwTypeID);
							nItemPos = pBag ->GetItemPosition(dwTypeID);
						}
						if(nItemCount == 0)
							m_pShortcutBtn[i][j]->SetInvalite(true);
						else 
							m_pShortcutBtn[i][j]->SetInvalite(false);
						m_pShortcutBtn[i][j]->SetItemNum(nItemCount);
						DWORD dwParam1 = MAKELONG( nItemPos, nItemCount );

						Item* item = ItemMgr::Inst()->GetPocketItem((INT16)dwParam1);
						if( !P_VALID(item) )
						{
							m_pShortcutBtn[i][j]->SetBtnData( btnData.GetType(), dwParam1, dwTypeID, 0 );
							return 0;
						}

						INT64 n64ID = item->GetItemId();
						m_pShortcutBtn[i][j]->SetBtnData( btnData.GetType(), dwParam1, dwTypeID, n64ID );
					}
					//可堆叠的物品
					else 
					{
						ItemContainer* pBag = ItemMgr::Inst()->GetWarBag();
						if( P_VALID(pBag) )
						{
							nItemCount = (INT16)pBag ->GetItemQuantity(dwTypeID);
							nItemPos = pBag ->GetItemPosition(dwTypeID);
						}
						m_pShortcutBtn[i][j]->SetItemNum(nItemCount);
						m_QuickBarFlags[i][j].m_bIsChange = true;
						DWORD dwParam1 = MAKELONG( nItemPos, nItemCount );
						if(nItemCount == 0)
							m_pShortcutBtn[i][j]->SetInvalite(true);
						else 
							m_pShortcutBtn[i][j]->SetInvalite(false);

						Item* item = ItemMgr::Inst()->GetPocketItem((INT16)dwParam1);
						if( !P_VALID(item) )
						{
							m_pShortcutBtn[i][j]->SetBtnData( btnData.GetType(), dwParam1, dwTypeID, 0 );
							return 0;
						}

						INT64 n64ID = item->GetItemId();
						m_pShortcutBtn[i][j]->SetBtnData( btnData.GetType(), dwParam1, dwTypeID, n64ID );
					}
				}
			}

			btnData = m_pShortcutBtn[i][j]->GetBtnData();
			//所有物品都要检查一遍
			{
				DWORD dwTypeID = btnData.GetData2();
				FLOAT fCDCount = 0.0f;
				FLOAT fRestoreRatio = 0.0f;
				ItemMgr::Inst()->GetItemCurCDTime(dwTypeID, fCDCount, fRestoreRatio);
				m_pShortcutBtn[i][j]->SetRestoreRatio(fRestoreRatio);
				m_pShortcutBtn[i][j]->SetCDTime(fCDCount);
			}
		}
	}
	m_pWnd->SetRefresh(TRUE);
	return 0;
}

DWORD ShortcutBarFrame::OnUpdateLPSkillEvent( tagUpdateLPSkillEvent* pEvent )
{
	if(pEvent->eType == ESUT_Passive)
		return 0;

	for(int i=0; i<BAR_COUNT; i++)
	{
		for(int j=0; j<BAR_BUTTON_COUNT; j++)
		{
			const BtnData btnData = m_pShortcutBtn[i][j]->GetBtnData();
			if(btnData.GetType()==ESBT_Null
				|| btnData.GetType()==ESBT_Item)
				continue;
			DWORD dwID = MTransSkillID(btnData.GetData1());

			if(dwID==pEvent->dwID)
			{
				const tagSkillData* pSkill = SkillMgr::Inst()->GetSkillData(dwID);
				if(!P_VALID(pSkill) )
				{
					m_pShortcutBtn[i][j]->ClearPic();
					m_pShortcutBtn[i][j]->SetBtnData(ESBT_Null, 0x0, 0x0, 0);
					m_pShortcutBtn[i][j]->SetInvalite(false);
					continue;
				}
				else 
				{
					INT nLevel = pSkill->nLevel;
					m_pShortcutBtn[i][j]->SetBtnData(ESBT_Skill, MTransSkillTypeID(dwID, nLevel), 0x0, 0);
					AutoAdaptFreshHandSkill(m_pShortcutBtn[i][j], MTransSkillTypeID(dwID, nLevel));
				}
			}
			
			if(ESUT_Active==pEvent->eType
				&& dwID==pEvent->dwID
				&& btnData.GetType()==ESBT_Skill)
			{
				//获取物品冷却旋转律
				FLOAT fCDCount = 0.0f;
				FLOAT fRestoreRatio = 0.0f;
				SkillMgr::Inst()->GetSkillCDTime(dwID, fCDCount, fRestoreRatio);
				m_pShortcutBtn[i][j]->SetRestoreRatio(fRestoreRatio);
				m_pShortcutBtn[i][j]->SetCDTime(fCDCount);
			}
		}
	}
	return 0;
}

void ShortcutBarFrame::ReflashCDTime()
{
	for( int i=0; i<BAR_COUNT; ++i )
	{
		for(int j=0; j<BAR_BUTTON_COUNT; ++j)
		{
			const BtnData btnData = m_pShortcutBtn[i][j]->GetBtnData();
			if(btnData.GetType()==ESBT_Null)
				continue;

			//刷新当前快捷栏组技能或物品的CD
			if(btnData.GetType()==ESBT_Skill)
			{
				DWORD dwID = MTransSkillID(btnData.GetData1());
				FLOAT fCDCount = 0.0f;
				FLOAT fRestoreRatio = 0.0f;
				SkillMgr::Inst()->GetSkillCDTime(dwID, fCDCount, fRestoreRatio);
				m_pShortcutBtn[i][j]->SetRestoreRatio(fRestoreRatio);
				m_pShortcutBtn[i][j]->SetCDTime(fCDCount);
				
			}
			else
			{
				DWORD dwTypeID = btnData.GetData2();
				FLOAT fCDCount = 0.0f;
				FLOAT fRestoreRatio = 0.0f;
				ItemMgr::Inst()->GetItemCurCDTime(dwTypeID, fCDCount, fRestoreRatio);
				m_pShortcutBtn[i][j]->SetRestoreRatio(fRestoreRatio);
				m_pShortcutBtn[i][j]->SetCDTime(fCDCount);
			}
		}
	}
	m_pWnd->SetRefresh(TRUE);
}

void CreateDir(const TCHAR* Path);
void ShortcutBarFrame::SaveBtnData()
{
	/*tstring strPath = Kernel::Inst()->GetWorkPath();
	DWORD dwLPID = RoleMgr::Inst()->GetLocalPlayerID();
	tstring strLocalName = PlayerNameTab::Inst()->FindNameByID(dwLPID);
	if( strLocalName.empty() )
		return;
	/*LocalPlayer* pPlayer = RoleMgr::Inst()->GetLocalPlayer();//已经注释3
	//tstring strLocalName = pPlayer->GetRoleName(); //已经注释3
	  
	strPath += _T("config\\");
	strPath += strLocalName;
	strPath += _T("\\");
	
	CreateDir(strPath.c_str());

	strPath += _T("shortcutbar.dat");

	FILE* fp = NULL;
	fp=_tfopen( strPath.c_str(), _T("wb") );

	if( fp != NULL)
	{
		DWORD dwRoleID = RoleMgr::Inst()->GetLocalPlayerID();
		fwrite(&dwRoleID, sizeof(DWORD), 1, fp);
		for(int i=0; i<BAR_COUNT; i++)
		{
			for(int j=0; j<BAR_BUTTON_COUNT; j++)
			{
				const BtnData btnData = m_pShortcutBtn[i][j]->GetBtnData();
		        fwrite(&btnData, sizeof(BtnData), 1, fp);
			}
		}
		fclose(fp);
	}*/
	SaveQuickBarToDB();
}

void ShortcutBarFrame::SaveQuickBarToDB()
{ 
	
	//如果没有存过数据库就把快捷栏已经有的数据全部发送,否则就发送快捷栏变化的数据
	if ( m_bHaveNotSavedQuickBarData )
	{   
		
		//把状态发生变化的快捷栏数据保存到数据库 否则不保存
		tagNC_MsgQuickBarData MsgSend ;//保存到数据库
		DWORD dwRoleID = RoleMgr::Inst()-> GetLocalPlayerID();
		MsgSend.dwRoleID = dwRoleID;
		for(int i=0; i<BAR_COUNT; i++)
		{
			for(int j=0; j<BAR_BUTTON_COUNT; j++)
			{
				const BtnData btnData = m_pShortcutBtn[i][j]->GetBtnData();
			
				 m_QuickBarFlags[i][j].m_bIsChange = TRUE;
				 m_QuickBarFlags[i][j].m_eType     = (INT16)btnData.GetType();
			     m_QuickBarFlags[i][j].m_dwParam1  =  btnData.GetData1();
                 m_QuickBarFlags[i][j].m_dwParam2  =  btnData.GetData2();
                 m_QuickBarFlags[i][j].m_n64Param3 =  btnData.GetData3();
				if ( m_QuickBarFlags[i][j].m_bIsChange && m_QuickBarFlags[i][j].m_eType != ESBT_Null)
			 {
				
				 m_bIsQuit    = FALSE;
				 MsgSend.nLn  =  i;
				 MsgSend.nCol =  j;
				 MsgSend.btnData.m_eType    =  (INT16)btnData.GetType();
				 MsgSend.btnData.m_dwParam1 =  btnData.GetData1();
				 MsgSend.btnData.m_dwParam2 =  btnData.GetData2();
				 MsgSend.btnData.m_n64Param3=  btnData.GetData3();
				 TObjRef<NetSession>()->Send( &MsgSend );
				
				
			 }
			m_QuickBarFlags[i][j].m_bIsChange = FALSE;
			}
		}

		if ( m_bIsChange && !m_bIsQuit)  //但快捷栏数据变化,且正常退出
		{
			tagNC_MsgInformSaveQuickBarData send;
			TObjRef<NetSession>()->Send( &send );
			m_bIsChange = FALSE;
			m_bIsQuit = TRUE;
			
		}
		m_bHaveNotSavedQuickBarData = FALSE;
          
	}
	else
	{
		
		//把状态发生变化的快捷栏数据保存到数据库 否则不保存
		tagNC_MsgQuickBarData MsgSend ;//保存到数据库
		DWORD dwRoleID = RoleMgr::Inst()->GetLocalPlayerID();
		MsgSend.dwRoleID = dwRoleID;
		for(int i=0; i<BAR_COUNT; i++)
		{
			for(int j=0; j<BAR_BUTTON_COUNT; j++)
			{
				const BtnData btnData = m_pShortcutBtn[i][j]->GetBtnData();
				if (m_QuickBarFlags[i][j].m_eType != (INT16)btnData.GetType())
			    {
				 m_QuickBarFlags[i][j].m_bIsChange = TRUE;
				 m_QuickBarFlags[i][j].m_eType     = (INT16)btnData.GetType();
				 m_QuickBarFlags[i][j].m_dwParam1  =  btnData.GetData1();
				 m_QuickBarFlags[i][j].m_dwParam2  =  btnData.GetData2();
				 m_QuickBarFlags[i][j].m_n64Param3 =  btnData.GetData3();
				}
				else if( m_QuickBarFlags[i][j].m_eType == ESBT_Skill && m_QuickBarFlags[i][j].m_dwParam1 != btnData.GetData1() )
				{
					
					    m_QuickBarFlags[i][j].m_bIsChange = TRUE;
						m_QuickBarFlags[i][j].m_eType     = (INT16)btnData.GetType();
						m_QuickBarFlags[i][j].m_dwParam1  =  btnData.GetData1();
						m_QuickBarFlags[i][j].m_dwParam2  =  btnData.GetData2();
						m_QuickBarFlags[i][j].m_n64Param3 =  btnData.GetData3();

				}
				else if( m_QuickBarFlags[i][j].m_eType == ESBT_PetSkill && m_QuickBarFlags[i][j].m_dwParam2 != btnData.GetData2())
				{
					   m_QuickBarFlags[i][j].m_bIsChange = TRUE;
					   m_QuickBarFlags[i][j].m_eType     = (INT16)btnData.GetType();
					   m_QuickBarFlags[i][j].m_dwParam1  =  btnData.GetData1();
					   m_QuickBarFlags[i][j].m_dwParam2  =  btnData.GetData2();
					   m_QuickBarFlags[i][j].m_n64Param3 =  btnData.GetData3();

				}
				else if( m_QuickBarFlags[i][j].m_eType == ESBT_Item && m_QuickBarFlags[i][j].m_n64Param3 != btnData.GetData3() )
				{
					m_QuickBarFlags[i][j].m_bIsChange = TRUE;
					m_QuickBarFlags[i][j].m_eType     = (INT16)btnData.GetType();
					m_QuickBarFlags[i][j].m_dwParam1  =  btnData.GetData1();
					m_QuickBarFlags[i][j].m_dwParam2  =  btnData.GetData2();
					m_QuickBarFlags[i][j].m_n64Param3 =  btnData.GetData3();
				}
				else if( m_QuickBarFlags[i][j].m_eType == ESBT_StyleAction && m_QuickBarFlags[i][j].m_dwParam1 != btnData.GetData1() )
				{
					m_QuickBarFlags[i][j].m_bIsChange = TRUE;
					m_QuickBarFlags[i][j].m_eType     = (INT16)btnData.GetType();
					m_QuickBarFlags[i][j].m_dwParam1  =  btnData.GetData1();
					m_QuickBarFlags[i][j].m_dwParam2  =  btnData.GetData2();
					m_QuickBarFlags[i][j].m_n64Param3 =  btnData.GetData3();  
				}

				if ( m_QuickBarFlags[i][j].m_bIsChange )
			   {
				 m_bIsQuit    =  FALSE;
				 MsgSend.nLn  =  i;
				 MsgSend.nCol =  j;
				 MsgSend.btnData.m_eType    =  (INT16)btnData.GetType();
				 MsgSend.btnData.m_dwParam1 =  btnData.GetData1();
				 MsgSend.btnData.m_dwParam2 =  btnData.GetData2();
				 MsgSend.btnData.m_n64Param3=  btnData.GetData3();
				 TObjRef<NetSession>()->Send( &MsgSend );
				 m_QuickBarFlags[i][j].m_bIsChange = FALSE;
			   }
			}
		}
	}

	
}

void ShortcutBarFrame::SetIsChange(BOOL bIschange)
{
    m_bIsChange = bIschange;
}

void ShortcutBarFrame::LoadBtnData(DWORD dwLocalID)
{
	tagNC_MsgQuickBarDataLoadFromDB Msg; //如果本地机子已经保存就不要向服务器发送请求数据信息
	Msg.dwRoleID = dwLocalID;
	tstring strPath = Kernel::Inst()->GetWorkPath();
	tstring strLocalName = PlayerNameTab::Inst()->FindNameByID(dwLocalID);
	strPath += _T("config\\");
	strPath += strLocalName;
	strPath += _T("\\");
	strPath += _T("shortcutbar.dat");

	FILE *fp= NULL; //_tfopen( strPath.c_str(), _T("rb") );
	if( fp != NULL )
	{
		DWORD dwRoleID = GT_INVALID;
		fread(&dwRoleID, sizeof(DWORD), 1, fp);
		if(dwRoleID!=dwLocalID)
			return;
		for(int i=0; i<BAR_COUNT; i++)
		{
			for(int j=0; j<BAR_BUTTON_COUNT; j++)
			{
				BtnData btnData;
				fread(&btnData, sizeof(BtnData), 1, fp);
				m_pShortcutBtn[i][j]->SetBtnData(btnData);
				m_QuickBarFlags[i][j].m_eType =(INT16)btnData.GetType();
			    m_QuickBarFlags[i][j].m_bIsChange = FALSE;
			}
		}
		ReflashShortcutBtn();
		ReflashCDTime();
		SetBtnEnable();
		m_bInitShortcut = TRUE;
		tagGameEvent e(_T("Weapon_SwapEx"), NULL);
		m_pMgr->SendEvent(&e);
		tagGameEvent evt(_T("InitedSkill_Event"), NULL);
		m_pMgr->SendEvent(&evt);
        m_bIsLoadFromDB = FALSE ;

	}	
	else
	{
		if ( m_bIsLoadFromDB )
		{

			Msg.n16IsSave =static_cast<INT16>(ENUMQUICK_TOLOAD);
			TObjRef<NetSession>()->Send( &Msg );
			m_bIsLoadFromDB = FALSE;
            
            
			
			 

		}
	}
  
	
}

void CreateDir(const TCHAR* Path)
{
	TCHAR DirName[256];
	const TCHAR* p = Path;
	TCHAR* q = DirName;	

	while(*p)
	{
		if ((_T('\\') == *p) || (_T('/') == *p))
		{
			if (_T(':') != *(p-1))
			{
				CreateDirectory(DirName, NULL);
			}
		}
		*q++ = *p++;
		*q = _T('\0');
	}
	CreateDirectory(DirName, NULL);
}

void ShortcutBarFrame::ReflashShortcutBtn( bool bOnlySkill /* = false */ )
{
	for(int i=0; i<BAR_COUNT; i++)
	{
		for(int j=0; j<BAR_BUTTON_COUNT; j++)
		{
			const BtnData& btnData = m_pShortcutBtn[i][j]->GetBtnData();
			if( btnData.GetType() == ESBT_Null )
				continue;

			if( bOnlySkill && btnData.GetType() != ESBT_Skill )
				continue;
			if( btnData.GetType() == ESBT_Skill )
			{
				if(!AutoAdaptFreshHandSkill(m_pShortcutBtn[i][j], btnData.GetData1()))
				{
					tstring picName = SkillMgr::Inst()->GetSkillIconPath(btnData.GetData1());
					m_pShortcutBtn[i][j]->ClearPic();
					m_pShortcutBtn[i][j]->SetPicFileName(picName);
					m_pShortcutBtn[i][j]->SetItemNum(0);
					LocalPlayer* pLocalPlayer = RoleMgr::Inst()->GetLocalPlayer();
					if( !P_VALID(pLocalPlayer) )
						return;

					DWORD dwID = MTransSkillID(btnData.GetData1());
					const tagSkillData* pSkill = SkillMgr::Inst()->GetSkillData(dwID);
					if(P_VALID(pSkill) )
					{
						INT nLevel = pSkill->nLevel;
						m_pShortcutBtn[i][j]->SetBtnData(ESBT_Skill, MTransSkillTypeID(dwID, nLevel), 0x0, 0);
						if( pSkill->nCost[ESCT_HP] > pLocalPlayer->GetAttribute(ERA_HP) ||
							pSkill->nCost[ESCT_MP] > pLocalPlayer->GetAttribute(ERA_MP) ||
							pSkill->nCost[ESCT_Rage] > pLocalPlayer->GetAttribute(ERA_Rage) ||
							pSkill->nCost[ESCT_Valicity] > pLocalPlayer->GetAttribute(ERA_Vitality) ||
							pSkill->nCost[ESCT_Endurance] > pLocalPlayer->GetAttribute(ERA_Endurance) )
						{
							m_pShortcutBtn[i][j]->SetInvalite(true);
						}
						else
						{
							m_pShortcutBtn[i][j]->SetInvalite(false);
						}
					}
				}
			}
            else if( btnData.GetType() == ESBT_PetSkill)
            {
                // 快捷按钮图片处理
                const tagPetSkill* pPetSkill = PetManager::Inst()->FindPetSkill(btnData.GetData1(), btnData.GetData2());
                if (P_VALID(pPetSkill))
                {
                    const tagPetSkillData* pPetSkillData = PetProtoData::Inst()->FindPetSkill(pPetSkill->dwTypeID);
                    tstring picName = P_VALID(pPetSkillData) ? pPetSkillData->strIconPath : _T("");
                    m_pShortcutBtn[i][j]->ClearPic();
                    m_pShortcutBtn[i][j]->SetPicFileName(picName);
                }
            }
			else if( btnData.GetType() == ESBT_Item||btnData.GetType()==ESBT_WarBag )
			{
				DWORD dwTypeID = btnData.GetData2();
				
				if( MIsEquipment(dwTypeID) )
				{
					tstring picName = ItemMgr::Inst()->GetItemIconPath(dwTypeID);
					m_pShortcutBtn[i][j]->ClearPic();
					m_pShortcutBtn[i][j]->SetPicFileName(picName);
					m_pShortcutBtn[i][j]->SetItemNum(0);
				}
				else if(!MIsEquipment(dwTypeID))
				{
					tstring picName = ItemMgr::Inst()->GetItemIconPath(dwTypeID);
					m_pShortcutBtn[i][j]->ClearPic();
					m_pShortcutBtn[i][j]->SetPicFileName(picName);

					INT16 nItemCount = INT16( (btnData.GetData1() & 0xFFFF0000) >> 16 );
					m_pShortcutBtn[i][j]->SetItemNum(nItemCount);
				}
			}
			else if (btnData.GetType() == ESBT_StyleAction)
			{
				const tagPersonAct* pActData = PersonalityActData::Inst()->FindPersonAct( btnData.GetData1() );
				if( P_VALID( pActData ) )
				{
					m_pShortcutBtn[i][j]->ClearPic();
					m_pShortcutBtn[i][j]->SetPicFileName(pActData->strPicFilename);
				}
				m_pShortcutBtn[i][j]->SetItemNum(0);
			}
		}
	}
	m_pWnd->SetRefresh(TRUE);
}

DWORD ShortcutBarFrame::OnRecvNetRoleGetName( tagRoleGetNameEvent* pMsg )
{
	DWORD dwLocalID = RoleMgr::Inst()->GetLocalPlayerID();
	if( pMsg->dwRoleID == dwLocalID )	
	{
		if(!m_bInitShortcut)
			LoadBtnData(dwLocalID);
	}
	return 0;
}

void ShortcutBarFrame::SetBtnEnable()
{
	for(int i=0; i<BAR_COUNT; i++)
	{
		for(int j=0; j<BAR_BUTTON_COUNT; j++)
		{
			const BtnData btnData = m_pShortcutBtn[i][j]->GetBtnData();
			if( btnData.GetType() == ESBT_Null )
			{
				m_pShortcutBtn[i][j]->SetInvalite(false);
			}
			else if( btnData.GetType() == ESBT_Skill )
			{
				m_pShortcutBtn[i][j]->SetInvalite(false);
			}
			else if(btnData.GetType() == ESBT_Item||btnData.GetType()==ESBT_WarBag)
			{
				DWORD dwTypeID = btnData.GetData2();
				if( MIsEquipment(dwTypeID) )
				{
					//判断背包中该位置是否有物品
					INT64 n64ItemID  =btnData.GetData3();
					Item* pItem1 = ItemMgr::Inst()->GetConItemByID(EICT_Bag, n64ItemID);
					if ( !P_VALID(pItem1) )
					{
						m_pShortcutBtn[i][j]->SetInvalite(true);   
					}
					else
					{
						if(pItem1->GetItemTypeID() != dwTypeID )
						{
							m_pShortcutBtn[i][j]->SetInvalite(true); 
						}
						else
						{
							INT16 n16Num = (INT16)(btnData.GetData1() >> 16);
							if( n16Num==0 || n16Num==GT_INVALID )
								m_pShortcutBtn[i][j]->SetInvalite(true);
							else 
								m_pShortcutBtn[i][j]->SetInvalite(false);
						}
					}
				}
				else if( !MIsEquipment(dwTypeID) )
				{
					//判断背包中该位置是否有物品
					INT64 n64ItemID  =btnData.GetData3();
					Item* pItem1 = ItemMgr::Inst()->GetConItemByID(EICT_Bag, n64ItemID);
					if ( !P_VALID(pItem1) )
					{
						m_pShortcutBtn[i][j]->SetInvalite(true);   
					}
					else
					{
						if(pItem1->GetItemTypeID() != dwTypeID )
						{
							m_pShortcutBtn[i][j]->SetInvalite(true); 
						}
						else
						{
							INT16 n16Num = (INT16)(btnData.GetData1() >> 16);
							if( n16Num==0 || n16Num==GT_INVALID )
								m_pShortcutBtn[i][j]->SetInvalite(true);
							else 
								m_pShortcutBtn[i][j]->SetInvalite(false);
						}
					}
				}
			}
		}
	}
}

DWORD ShortcutBarFrame::OnShortcutCacheEvent( tagShortcutCacheEvent* pGameEvent )
{
	for(int i=0; i<BAR_COUNT; i++)
	{
		for(int j=0; j<BAR_BUTTON_COUNT; j++)
		{
			if( !P_VALID(m_pShortcutBtn[i][j]) )
				continue;
			const BtnData& data = m_pShortcutBtn[i][j]->GetBtnData();
			if( data.GetType() != ESBT_Skill )
				continue;
			if( data.GetData1() == pGameEvent->dwSkillID )
			{
				switch(pGameEvent->eType)
				{
				case ESUSS_Cache:
					m_pShortcutBtn[i][j]->SetCacheImage(TRUE);
					break;
				case ESUSS_CacheUsed:
					m_pShortcutBtn[i][j]->SetCacheImage(FALSE);
					break;
				}
			}
		}
	}
	return 0;
}

DWORD ShortcutBarFrame::OnRolePickEvent( tagRolePickEvent* pEvent )
{
	if(pEvent->eType == EWPT_LClick)
	{
		for( int i=0; i<BAR_COUNT; i++ )
		{
			for( int j=0; j<BAR_BUTTON_COUNT; j++ )
			{
				if( !P_VALID(m_pShortcutBtn[i][j]) )
					continue;
				const BtnData& data = m_pShortcutBtn[i][j]->GetBtnData();
				if( data.GetType() != ESBT_Skill )
				{
					m_pShortcutBtn[i][j]->SetKeyBarEnable(TRUE);
					continue;
				}
				const tagSkillProtoClient* pSkill = SkillProtoData::Inst()->FindSkillProto(data.GetData1());
				if(!P_VALID(pSkill)) continue;
				if( pSkill->nType2 == ESSTE_Produce )
				{
					m_pShortcutBtn[i][j]->SetKeyBarEnable(TRUE);
					continue;
				}
				//取消选择的目标 roldid = GT_INVALID
				if( pEvent->dwRoleID == GT_INVALID )
				{
					m_pShortcutBtn[i][j]->SetKeyBarEnable(TRUE);
					continue;
				}

				if( !CombatActionChecker::Inst()->IfCanAttack(pEvent->dwRoleID) )
				{
					m_pShortcutBtn[i][j]->SetKeyBarEnable(FALSE);
				}
				else
				{
					m_pShortcutBtn[i][j]->SetKeyBarEnable(TRUE);
				}
			}
		}
		m_pWnd->SetRefresh(TRUE);
	}
	return 0;
}

DWORD ShortcutBarFrame::OnInitLPAttEvent( tagGameEvent* pEvent )
{
	if( !m_bLPAttIsOK )
		m_bLPAttIsOK = true;
	return 0;
}

DWORD ShortcutBarFrame::OnKeyChange( DWORD dwID, DWORD key1, DWORD key2 )
{
	for( int i=0; i<BAR_TITLE_COUNT+BAR_COUNT_TIPS; i++ )
	{
		if( dwID==m_pUtil->Crc32(m_szAccArray[i]) )
		{
			tstring strKey1, strKey2;
			if( key2 )
				strKey2 = this->GetKeyName((DIK)key2);
			if( key1 )
			{
				if( !strKey2.empty() )
				{
					strKey1 = this->GetKeyName( (DIK)key1 );
					strKey1.append(_T("+"));
					strKey1.append(strKey2.c_str());
				}
				else 
					strKey1 = this->GetKeyName( (DIK)key1 );
			}

			for( int i=0; i<BAR_COUNT; ++i )
			{
				for( int j=0; j<BAR_BUTTON_COUNT; ++j )
				{
					DWORD dwKeyID = m_pShortcutBtn[i][j]->GetDIKBar();
					if( dwID==dwKeyID )
					{
						if( !strKey1.empty() )
							m_pShortcutBtn[i][j]->SetKeyBar(strKey1.c_str());
						else
							m_pShortcutBtn[i][j]->SetKeyBar(strKey2.c_str());
					}
				}
			}
			break;
		}
	}
	m_pWnd->SetRefresh(TRUE);
	return 0;
}

DWORD ShortcutBarFrame::OnKeyExChange(DWORD dwID, DWORD key1, DWORD key2)
{
	for( int i=0; i<BAR_COUNT_TIPS; ++i )
	{
		if( dwID == m_pUtil->Crc32(m_szBarGroup[i]) )
		{

		}
	}

	return 0;
}

DWORD ShortcutBarFrame::OnKey2MoveChange(DWORD dwID, DWORD key1, DWORD key2)
{
	for( int i=0; i<BAR_BUTTON_COUNT; i++ )
	{
		if( dwID==m_pUtil->Crc32(m_szMoveArray[i]) )
		{
			tstring strKey1, strKey2;
			if( key2 )
				strKey2 = this->GetKeyName((DIK)key2);
			if( key1 )
			{
				if( !strKey2.empty() )
				{
					strKey1 = this->GetKeyName( (DIK)key1 );
					strKey1.append(_T("+"));
					strKey1.append(strKey2.c_str());
				}
				else
					strKey1 = this->GetKeyName( (DIK)key1 );
			}

			for( int i=0; i<BAR_COUNT; ++i )
			{
				for( int j=0; j<BAR_BUTTON_COUNT; ++j )
				{
					DWORD dwKeyID = m_pShortcutBtn[i][j]->GetDIKBar();
					if( dwID==dwKeyID )
					{
						if( !strKey1.empty() )
							m_pShortcutBtn[i][j]->SetKeyBar(strKey1.c_str());
						else
							m_pShortcutBtn[i][j]->SetKeyBar(strKey2.c_str());
					}
				}
			}
			break;
		}
	}
	m_pWnd_Move1->SetRefresh(TRUE);
	return 0;
}



DWORD ShortcutBarFrame::OnKey2MoveChange1( DWORD dwID, DWORD key1, DWORD key2 )
{
	for( int i=0; i<BAR_BUTTON_COUNT; i++ )
	{
		if( dwID==m_pUtil->Crc32(m_szMoveArray1[i]) )
		{
			tstring strKey1, strKey2;
			if( key2 )
				strKey2 = this->GetKeyName((DIK)key2);
			if( key1 )
			{
				if( !strKey2.empty() )
				{
					strKey1 = this->GetKeyName( (DIK)key1 );
					strKey1.append(_T("+"));
					strKey1.append(strKey2.c_str());
				}
				else 
					strKey1 = this->GetKeyName( (DIK)key1 );
			}

			for( int i=0; i<BAR_COUNT; ++i )
			{
				for( int j=0; j<BAR_BUTTON_COUNT; ++j )
				{
					DWORD dwKeyID = m_pShortcutBtn[i][j]->GetDIKBar();
					if( dwID==dwKeyID )
					{
						if( !strKey1.empty() )
							m_pShortcutBtn[i][j]->SetKeyBar(strKey1.c_str());
						else
							m_pShortcutBtn[i][j]->SetKeyBar(strKey2.c_str());
					}
				}
			}
			break;
		}
	}
	m_pWnd_Move2->SetRefresh(TRUE);
	return 0;
}


DWORD ShortcutBarFrame::OnKeyTransSkillChangel(DWORD dwID, DWORD key1, DWORD key2)
{
	for( int i=0; i<TRANS_BUTTON_COUNT; i++ )
	{
		if( dwID==m_pUtil->Crc32(m_szTransSkill[i]) )
		{
			tstring strKey1, strKey2;
			if( key2 )
				strKey2 = this->GetKeyName((DIK)key2);
			if( key1 )
			{
				if( !strKey2.empty() )
				{
					strKey1 = this->GetKeyName( (DIK)key1 );
					strKey1.append(_T("+"));
					strKey1.append(strKey2.c_str());
				}
				else 
					strKey1 = this->GetKeyName( (DIK)key1 );
			}

			for( int i=0; i<BAR_COUNT; ++i )
			{
				for( int j=0; j<BAR_BUTTON_COUNT; ++j )
				{
					if(!P_VALID(m_pShortcutBtn[i][j]))
						continue;
					DWORD dwKeyID = m_pShortcutBtn[i][j]->GetDIKBar();
					if( dwID==dwKeyID )
					{
						if( !strKey1.empty() )
							m_pShortcutBtn[i][j]->SetKeyBar(strKey1.c_str());
						else
							m_pShortcutBtn[i][j]->SetKeyBar(strKey2.c_str());
					}
				}
			}
			break;
		}
	}
	m_pWnd_TransSkill->SetRefresh(TRUE);
	return 0;
}

LPCTSTR ShortcutBarFrame::GetKeyName( DIK key )
{
	switch(key)
	{
	case DIK_ESCAPE:		return _T("ESC");
	case DIK_1:				return _T("1");        
	case DIK_2:				return _T("2");               
	case DIK_3:				return _T("3");               
	case DIK_4:				return _T("4");               
	case DIK_5:				return _T("5");               
	case DIK_6:				return _T("6");               
	case DIK_7:				return _T("7");               
	case DIK_8:				return _T("8");               
	case DIK_9:				return _T("9");               
	case DIK_0:				return _T("0");               
	case DIK_MINUS:			return _T("-");
	case DIK_EQUALS:		return _T("=");
	case DIK_BACK:			return _T("BackSpace");
	case DIK_TAB:			return _T("TAB");             
	case DIK_Q:				return _T("Q");               
	case DIK_W:				return _T("W");               
	case DIK_E:				return _T("E");
	case DIK_R:				return _T("R");
	case DIK_T:				return _T("T");              
	case DIK_Y:				return _T("Y");
	case DIK_U:				return _T("U");
	case DIK_I:				return _T("I");
	case DIK_O:				return _T("O");
	case DIK_P:				return _T("P");
	case DIK_LBRACKET:		return _T("[");
	case DIK_RBRACKET:		return _T("]");
	case DIK_RETURN:		return _T("Enter");
	case DIK_LCONTROL:		return _T("c.");
	case DIK_A:				return _T("A");
	case DIK_S:				return _T("S");
	case DIK_D:				return _T("D");
	case DIK_F:				return _T("F");
	case DIK_G:				return _T("G");
	case DIK_H:				return _T("H");
	case DIK_J:				return _T("J");
	case DIK_K:				return _T("K");
	case DIK_L:				return _T("L");
	case DIK_SEMICOLON:		return _T(";");
	case DIK_APOSTROPHE:	return _T("'");
	case DIK_GRAVE:			return _T("`");					/* accent grave */
	case DIK_LSHIFT:		return _T("s.");
	case DIK_BACKSLASH:		return _T("\\");       
	case DIK_Z:				return _T("Z");               
	case DIK_X:				return _T("X");               
	case DIK_C:				return _T("C");               
	case DIK_V:				return _T("V");               
	case DIK_B:				return _T("B");                              
	case DIK_N:				return _T("N");                              
	case DIK_M:				return _T("M");                              
	case DIK_COMMA:			return _T(",");                  
	case DIK_PERIOD:		return _T(".");					/* . on main keyboard */
	case DIK_SLASH:			return _T("/");					/* / on main keyboard */
	case DIK_RSHIFT:		return _T("s.");                         
	case DIK_MULTIPLY:		return _T("NumPad*");           /* * on numeric keypad */
	case DIK_LMENU:			return _T("a.");             /* left Alt */
	case DIK_SPACE:			return _T("Space");                          
	case DIK_CAPITAL:		return _T("CapsLock");                        
	case DIK_F1:			return _T("F1");                             
	case DIK_F2:			return _T("F2");                                           
	case DIK_F3:			return _T("F3");                                           
	case DIK_F4:			return _T("F4");                                           
	case DIK_F5:			return _T("F5");                                           
	case DIK_F6:			return _T("F6");                                           
	case DIK_F7:			return _T("F7");                                           
	case DIK_F8:			return _T("F8");                                           
	case DIK_F9:			return _T("F9");                                           
	case DIK_F10:			return _T("F10");                                          
	case DIK_NUMLOCK :		return _T("NumLock");        
	case DIK_SCROLL:		return _T("ScrollLock");        /* Scroll Lock */
	case DIK_NUMPAD7:		return _T("NumPad7");         
	case DIK_NUMPAD8:		return _T("NumPad8");         
	case DIK_NUMPAD9:		return _T("NumPad9");         
	case DIK_SUBTRACT:		return _T("NumPad-");           /* - on numeric keypad */
	case DIK_NUMPAD4:		return _T("NumPad4");         
	case DIK_NUMPAD5:		return _T("NumPad5");         
	case DIK_NUMPAD6:		return _T("NumPad6");         
	case DIK_ADD:			return _T("NumPad+");           /* + on numeric keypad */
	case DIK_NUMPAD1:		return _T("NumPad1");         
	case DIK_NUMPAD2:		return _T("NumPad2");         
	case DIK_NUMPAD3:		return _T("NumPad3");         
	case DIK_NUMPAD0:		return _T("NumPad0");         
	case DIK_DECIMAL:		return _T("NumPad.");           /* . on numeric keypad */
	case DIK_OEM_102:		return _T("OEM_102");           /* <> or \| on RT 102-key keyboard (Non-U.S.) */
	case DIK_F11:			return _T("F11");             
	case DIK_F12:			return _T("F12");             
	case DIK_F13:			return _T("F13");               /*                     (NEC PC98) */
	case DIK_F14:			return _T("F14");               /*                     (NEC PC98) */
	case DIK_F15:			return _T("F15");               /*                     (NEC PC98) */
	case DIK_KANA:			return _T("Kana");              /* (Japanese keyboard)            */
	case DIK_ABNT_C1:		return _T("ABNT_C1");           /* /? on Brazilian keyboard */
	case DIK_CONVERT:		return _T("Convert");           /* (Japanese keyboard)            */
	case DIK_NOCONVERT:		return _T("NoConvert");         /* (Japanese keyboard)            */
	case DIK_YEN:			return _T("YEN");               /* (Japanese keyboard)            */
	case DIK_ABNT_C2:		return _T("ABNT_C2");           /* Numpad . on Brazilian keyboard */
	case DIK_NUMPADEQUALS:	return _T("NumPad=");		    /* = on numeric keypad (NEC PC98) */
	case DIK_PREVTRACK:		return _T("PreTrack");			/* Previous Track (DIK_CIRCUMFLEX on Japanese keyboard) */
	case DIK_AT:			return _T("AT");                /*                     (NEC PC98) */
	case DIK_COLON:			return _T("Colon");             /*                     (NEC PC98) */
	case DIK_UNDERLINE:		return _T("UnderLine");         /*                     (NEC PC98) */
	case DIK_KANJI:			return _T("Kanji");             /* (Japanese keyboard)            */
	case DIK_STOP:			return _T("Stop");              /*                     (NEC PC98) */
	case DIK_AX:			return _T("AX");                /*                     (Japan AX) */
	case DIK_UNLABELED:		return _T("UnLabeled");         /*                        (J3100) */
	case DIK_NEXTTRACK:		return _T("NextTrack");         /* Next Track */
	case DIK_NUMPADENTER:	return _T("NumPadEnter");       /* Enter on numeric keypad */
	case DIK_RCONTROL:		return _T("c.");        
	case DIK_MUTE:			return _T("Mute");              /* Mute */
	case DIK_CALCULATOR:	return _T("Calculator");        /* Calculator */
	case DIK_PLAYPAUSE:		return _T("Play");              /* Play / Pause */
	case DIK_MEDIASTOP:		return _T("MediaStop");         /* Media Stop */
	case DIK_VOLUMEDOWN:	return _T("Volume-");           /* Volume - */
	case DIK_VOLUMEUP:		return _T("Volume+");			/* Volume + */
	case DIK_WEBHOME:		return _T("WebHome");           /* Web home */
	case DIK_NUMPADCOMMA:	return _T("NumPadComma");       /* , on numeric keypad (NEC PC98) */
	case DIK_DIVIDE:		return _T("NumPad/");           /* / on numeric keypad */
	case DIK_SYSRQ:			return _T("PrintScreen");           
	case DIK_RMENU:			return _T("a.");             /* right Alt */
	case DIK_PAUSE:			return _T("Pause");             /* Pause */
	case DIK_HOME:			return _T("Home");              /* Home on arrow keypad */
	case DIK_UP:			return _T("Up");                /* UpArrow on arrow keypad */
	case DIK_PRIOR:			return _T("PageUp");            /* PgUp on arrow keypad */
	case DIK_LEFT:			return _T("Left");              /* LeftArrow on arrow keypad */
	case DIK_RIGHT:			return _T("Right");             /* RightArrow on arrow keypad */
	case DIK_END:			return _T("End");               /* End on arrow keypad */
	case DIK_DOWN:			return _T("Down");              /* DownArrow on arrow keypad */
	case DIK_NEXT:			return _T("PageDown");          /* PgDn on arrow keypad */
	case DIK_INSERT:		return _T("Insert");            /* Insert on arrow keypad */
	case DIK_DELETE:		return _T("Delete");            /* Delete on arrow keypad */
	case DIK_LWIN:			return _T("L-Windows");         /* Left Windows key */
	case DIK_RWIN:			return _T("R-Windows");         /* Right Windows key */
	case DIK_APPS:			return _T("AppMenu");           /* AppMenu key */
	case DIK_POWER:			return _T("Power");             /* System Power */
	case DIK_SLEEP:			return _T("Sleep");             /* System Sleep */
	case DIK_WAKE:			return _T("Wake");              /* System Wake */
	case DIK_WEBSEARCH:		return _T("WebSearch");         /* Web Search */
	case DIK_WEBFAVORITES:	return _T("WebFavorites");      /* Web Favorites */
	case DIK_WEBREFRESH:	return _T("WebRefresh");        /* Web Refresh */
	case DIK_WEBSTOP:		return _T("WebStop");           /* Web Stop */
	case DIK_WEBFORWARD:	return _T("WebForward");        /* Web Forward */
	case DIK_WEBBACK:		return _T("WebBack");           /* Web Back */
	case DIK_MYCOMPUTER:	return _T("MyComputer");        /* My Computer */
	case DIK_MAIL:			return _T("Mail");              /* Mail */
	case DIK_MEDIASELECT:	return _T("MediaSelect");       /* Media Select */
	default:				return _T("Unknow");
	}
}

void ShortcutBarFrame::ShowMoveShortcutBar( BOOL bShow )
{
	for(int i=0; i<BAR_BUTTON_COUNT; ++i)
	{
		m_pShortcutBtn[8][i]->SetInvisible(!bShow);
	}
}

void ShortcutBarFrame::ShowMove1ShortcutBar( BOOL bShow )
{
	for(int i=0; i<BAR_BUTTON_COUNT; ++i)
	{
		m_pShortcutBtn[9][i]->SetInvisible(!bShow);
	}
}

DWORD ShortcutBarFrame::OnInitedSkillEvent( tagGameEvent* pEvent )
{
	if( pEvent->strEventName!=_T("InitedSkill_Event") )
		return 0;
	for(int i=0; i<BAR_COUNT; i++)
	{
		for(int j=0; j<BAR_BUTTON_COUNT; j++)
		{
			BtnData data = m_pShortcutBtn[i][j]->GetBtnData();
			if( data.GetType() == ESBT_Skill )
			{
				if( data.GetData1()/1000 == 9000 )
				{
					if ( 9000101 == data.GetData1())
					{
						continue;
					}
					if( !SkillMgr::Inst()->IsHaveProduceSkillByTypeID(data.GetData1()) )		
					{
						m_pShortcutBtn[i][j]->SetBtnData(ESBT_Null, 0,0,0);
						m_pShortcutBtn[i][j]->ClearPic();
						m_pShortcutBtn[i][j]->SetInvalite(false);
					}
				}
			}
		}
	}

	return 1;
}

DWORD ShortcutBarFrame::OnDeletePet( tagGameEvent* pEvent )
{
	if( pEvent->strEventName!=_T("tagPetAttrChange") )
		return 0;
	
	tagPetAttrChange* pPetDeleteEvent = (tagPetAttrChange*)pEvent;
	if(!P_VALID(pPetDeleteEvent) || pPetDeleteEvent->eType != EPACT_Remove)
		return 0;

	for(int i=0; i<BAR_COUNT; i++)
	{
		for(int j=0; j<BAR_BUTTON_COUNT; j++)
		{
			BtnData data = m_pShortcutBtn[i][j]->GetBtnData();
			if( data.GetType() == ESBT_PetSkill && data.GetData1() == pPetDeleteEvent->dwPetID)
			{
				m_pShortcutBtn[i][j]->ClearPic();
				m_pShortcutBtn[i][j]->SetBtnData( ESBT_Null, 0x0, 0x0, 0x0);
				m_pShortcutBtn[i][j]->SetItemNum(0);
				m_pShortcutBtn[i][j]->SetInvisible(TRUE);
				SaveBtnData();
			}
		}
	}
	return 0;
}


DWORD ShortcutBarFrame::OnNS_VirginOnline( tagNS_VirginOnline* pMsg, DWORD )
{
    const tagSkillData* pSkill = SkillMgr::Inst()->GetSkillData(SkillMgr::Inst()->GetNormalAttackID());
    Item* pItem1 = ItemMgr::Inst()->GetConItemByID(EICT_Bag, pMsg->n64Serial1);
    Item* pItem2 = ItemMgr::Inst()->GetConItemByID(EICT_Bag, pMsg->n64Serial2);

	tagNC_MsgQuickBarData MsgSend ;
	DWORD dwRoleID = RoleMgr::Inst()-> GetLocalPlayerID();
	MsgSend.dwRoleID = dwRoleID;
	MsgSend.btnData.m_eType    =  ESBT_Skill;
	MsgSend.btnData.m_dwParam2 =  NULL;
	MsgSend.btnData.m_n64Param3=  NULL;

	//添加普通攻击
    m_pShortcutBtn[0][0]->SetBtnData(ESBT_Skill, 1000101, NULL, NULL);
    tstring picName = SkillMgr::Inst()->GetSkillIconPath(1000101);
    m_pShortcutBtn[0][0]->ClearPic();
    m_pShortcutBtn[0][0]->SetPicFileName(picName);
    m_pShortcutBtn[0][0]->SetItemNum(0);
    //-------------------------------------------------------------------------------
	//保存到数据库	
	MsgSend.nLn  =  0;
	MsgSend.nCol =  0;
	MsgSend.btnData.m_dwParam1 =  1000101;
	/////
	TObjRef<NetSession>()->Send( &MsgSend );

	//添加重击
	m_pShortcutBtn[0][1]->SetBtnData(ESBT_Skill, 1010101, NULL, NULL);
	picName = SkillMgr::Inst()->GetSkillIconPath(1010101);
	m_pShortcutBtn[0][1]->ClearPic();
	m_pShortcutBtn[0][1]->SetPicFileName(picName);
	m_pShortcutBtn[0][1]->SetItemNum(0);
	//-------------------------------------------------------------------------------
	//保存到数据库
	MsgSend.nLn  =  0;
	MsgSend.nCol =  1;
	MsgSend.btnData.m_dwParam1 =  1010101;
	TObjRef<NetSession>()->Send( &MsgSend );
     //-------------------------------------------------------------------------------

	//添加重击
	m_pShortcutBtn[0][3]->SetBtnData(ESBT_Skill, 1012301, NULL, NULL);
	picName = SkillMgr::Inst()->GetSkillIconPath(1012301);
	m_pShortcutBtn[0][3]->ClearPic();
	m_pShortcutBtn[0][3]->SetPicFileName(picName);
	m_pShortcutBtn[0][3]->SetItemNum(0);
	//-------------------------------------------------------------------------------
	//保存到数据库	
	MsgSend.nLn  =  0;
	MsgSend.nCol =  3;
	MsgSend.btnData.m_dwParam1 =  1012301;
	TObjRef<NetSession>()->Send( &MsgSend );
	//-------------------------------------------------------------------------------

	//添加骑术
	m_pShortcutBtn[8][0]->SetBtnData(ESBT_Skill, 9000101, NULL, NULL);
	picName = SkillMgr::Inst()->GetSkillIconPath(9000101);
	m_pShortcutBtn[8][0]->ClearPic();
	m_pShortcutBtn[8][0]->SetPicFileName(picName);
	m_pShortcutBtn[8][0]->SetItemNum(0);
	//-------------------------------------------------------------------------------
	//保存到数据库	
	MsgSend.nLn  =  8;
	MsgSend.nCol =  0;
	MsgSend.btnData.m_dwParam1 =  9000101;
	TObjRef<NetSession>()->Send( &MsgSend );
	//-------------------------------------------------------------------------------

	//添加回城
	m_pShortcutBtn[1][7]->SetBtnData(ESBT_Skill, 1012401, NULL, NULL);
	picName = SkillMgr::Inst()->GetSkillIconPath(1012401);
	m_pShortcutBtn[1][7]->ClearPic();
	m_pShortcutBtn[1][7]->SetPicFileName(picName);
	m_pShortcutBtn[1][7]->SetItemNum(0);
	//-------------------------------------------------------------------------------
	//保存到数据库	
	MsgSend.nLn  =  1;
	MsgSend.nCol =  7;
	MsgSend.btnData.m_dwParam1 =  1012401;
	TObjRef<NetSession>()->Send( &MsgSend );
	//-------------------------------------------------------------------------------

    if (P_VALID(pItem1))
    {   
        INT16 itemCount   = (INT16)pItem1->GetItemQuantity();
        INT16 itemPos     = pItem1->GetPos();
        DWORD dwParam1    = MAKELONG(itemPos, itemCount);

        m_pShortcutBtn[1][2]->SetItemData(dwParam1, pItem1->GetItemTypeID());
        const BtnData btnData = m_pShortcutBtn[1][2]->GetBtnData();
		MsgSend.nLn  =  1;
		MsgSend.nCol =  2;
		MsgSend.btnData.m_eType    =  btnData.GetType();
		MsgSend.btnData.m_dwParam1 =  btnData.GetData1();
		MsgSend.btnData.m_dwParam2 =  btnData.GetData2();
		MsgSend.btnData.m_n64Param3=  btnData.GetData3(); 
		TObjRef<NetSession>()->Send( &MsgSend );
    
    }

    if (P_VALID(pItem2))
    {
        INT16 itemCount   = (INT16)pItem2->GetItemQuantity();
        INT16 itemPos     = pItem2->GetPos();
        DWORD dwParam1    = MAKELONG(itemPos, itemCount);

        m_pShortcutBtn[1][3]->SetItemData(dwParam1, pItem2->GetItemTypeID());
        const BtnData btnData = m_pShortcutBtn[1][3]->GetBtnData();
		MsgSend.nLn  =  1;
		MsgSend.nCol =  3;
		MsgSend.btnData.m_eType    =  btnData.GetType();
		MsgSend.btnData.m_dwParam1 =  btnData.GetData1();
		MsgSend.btnData.m_dwParam2 =  btnData.GetData2();
		MsgSend.btnData.m_n64Param3=  btnData.GetData3(); 
		TObjRef<NetSession>()->Send( &MsgSend );

    }
    return 0;
}

DWORD ShortcutBarFrame::OnNS_MsgQuickBarData(tagNS_MsgQuickBarData* pMsg, DWORD)
{

	if( !P_VALID(pMsg) )
	{
		return 0;
	}
	DWORD dwLocalID = RoleMgr::Inst()->GetLocalPlayerID();

	if ( dwLocalID != pMsg->dwRoleID)
	{
		return 0;
	}
    ENUM_QUICK_BAR eQuick_BarSaveCount = static_cast<ENUM_QUICK_BAR>(pMsg->eQuickSendCount); 
   
	
	if( !m_bInitShortcut  )
	{
		switch(eQuick_BarSaveCount)
		{
		case QUICK_BAR_ONE:
            m_nQuickBar_Col_Count = 0;
			for(int i=0; i<QUICK_BAR_COUNT; i++)
			{
				for(int j=0; j<QUICK_BAR_BUTTON_COUNT; j++)
				{
					BtnData btnData ;
					btnData.Set((EShortcutBtnType)pMsg->btnData[i][j].m_eType,pMsg->btnData[i][j].m_dwParam1,pMsg->btnData[i][j].m_dwParam2,pMsg->btnData[i][j].m_n64Param3) ;
					m_pShortcutBtn[i+m_nQuickBar_Col_Count][j]->SetBtnData(btnData);
					m_QuickBarFlags[i+m_nQuickBar_Col_Count][j].m_eType =(EShortcutBtnType)pMsg->btnData[i][j].m_eType;
					m_QuickBarFlags[i+m_nQuickBar_Col_Count][j].m_bIsChange = FALSE;
					m_QuickBarFlags[i+m_nQuickBar_Col_Count][j].m_dwParam1  =  pMsg->btnData[i][j].m_dwParam1;
					m_QuickBarFlags[i+m_nQuickBar_Col_Count][j].m_dwParam2  =  pMsg->btnData[i][j].m_dwParam2;
					m_QuickBarFlags[i+m_nQuickBar_Col_Count][j].m_n64Param3 =  pMsg->btnData[i][j].m_n64Param3;  					
				}
			}
			break;
		case QUICK_BAR_TWO:
			m_nQuickBar_Col_Count = 2;
			for(int i=0; i<QUICK_BAR_COUNT; i++)
			{
				for(int j=0; j<QUICK_BAR_BUTTON_COUNT; j++)
				{
					BtnData btnData ;
					btnData.Set((EShortcutBtnType)pMsg->btnData[i][j].m_eType,pMsg->btnData[i][j].m_dwParam1,pMsg->btnData[i][j].m_dwParam2,pMsg->btnData[i][j].m_n64Param3) ;

					m_pShortcutBtn[i+m_nQuickBar_Col_Count][j]->SetBtnData(btnData);
					m_QuickBarFlags[i+m_nQuickBar_Col_Count][j].m_eType =(EShortcutBtnType)pMsg->btnData[i][j].m_eType;
					m_QuickBarFlags[i+m_nQuickBar_Col_Count][j].m_bIsChange = FALSE;
					m_QuickBarFlags[i+m_nQuickBar_Col_Count][j].m_dwParam1  =  pMsg->btnData[i][j].m_dwParam1;
					m_QuickBarFlags[i+m_nQuickBar_Col_Count][j].m_dwParam2  =  pMsg->btnData[i][j].m_dwParam2;
					m_QuickBarFlags[i+m_nQuickBar_Col_Count][j].m_n64Param3 =  pMsg->btnData[i][j].m_n64Param3;  
				}
			}
			break;
		case QUICK_BAR_THREE:
			m_nQuickBar_Col_Count = 4;
			for(int i=0; i<QUICK_BAR_COUNT; i++)
			{
				for(int j=0; j<QUICK_BAR_BUTTON_COUNT; j++)
				{
					BtnData btnData ;
					btnData.Set((EShortcutBtnType)pMsg->btnData[i][j].m_eType,pMsg->btnData[i][j].m_dwParam1,pMsg->btnData[i][j].m_dwParam2,pMsg->btnData[i][j].m_n64Param3) ;

					m_pShortcutBtn[i+m_nQuickBar_Col_Count][j]->SetBtnData(btnData);
					m_QuickBarFlags[i+m_nQuickBar_Col_Count][j].m_eType =(EShortcutBtnType)pMsg->btnData[i][j].m_eType;
					m_QuickBarFlags[i+m_nQuickBar_Col_Count][j].m_bIsChange = FALSE;
					m_QuickBarFlags[i+m_nQuickBar_Col_Count][j].m_dwParam1  =  pMsg->btnData[i][j].m_dwParam1;
					m_QuickBarFlags[i+m_nQuickBar_Col_Count][j].m_dwParam2  =  pMsg->btnData[i][j].m_dwParam2;
					m_QuickBarFlags[i+m_nQuickBar_Col_Count][j].m_n64Param3 =  pMsg->btnData[i][j].m_n64Param3;  
				}
			}
			break;
		case QUICK_BAR_FOUR:
			m_nQuickBar_Col_Count = 6;
			for(int i=0; i<QUICK_BAR_COUNT; i++)
			{
				for(int j=0; j<QUICK_BAR_BUTTON_COUNT; j++)
				{
					BtnData btnData ;
					btnData.Set((EShortcutBtnType)pMsg->btnData[i][j].m_eType,pMsg->btnData[i][j].m_dwParam1,pMsg->btnData[i][j].m_dwParam2,pMsg->btnData[i][j].m_n64Param3) ;

					m_pShortcutBtn[i+m_nQuickBar_Col_Count][j]->SetBtnData(btnData);
					m_QuickBarFlags[i+m_nQuickBar_Col_Count][j].m_eType =(EShortcutBtnType)pMsg->btnData[i][j].m_eType;
					m_QuickBarFlags[i+m_nQuickBar_Col_Count][j].m_bIsChange = FALSE;
					m_QuickBarFlags[i+m_nQuickBar_Col_Count][j].m_dwParam1  =  pMsg->btnData[i][j].m_dwParam1;
					m_QuickBarFlags[i+m_nQuickBar_Col_Count][j].m_dwParam2  =  pMsg->btnData[i][j].m_dwParam2;
					m_QuickBarFlags[i+m_nQuickBar_Col_Count][j].m_n64Param3 =  pMsg->btnData[i][j].m_n64Param3;  
				}
			}
			break;
		case QUICK_BAR_FIVE:
			m_nQuickBar_Col_Count = 8;
			for(int i=0; i<QUICK_BAR_COUNT; i++)
			{
				for(int j=0; j<QUICK_BAR_BUTTON_COUNT; j++)
				{
					BtnData btnData ;
					btnData.Set((EShortcutBtnType)pMsg->btnData[i][j].m_eType,pMsg->btnData[i][j].m_dwParam1,pMsg->btnData[i][j].m_dwParam2,pMsg->btnData[i][j].m_n64Param3) ;

					m_pShortcutBtn[i+m_nQuickBar_Col_Count][j]->SetBtnData(btnData);
					m_QuickBarFlags[i+m_nQuickBar_Col_Count][j].m_eType =(EShortcutBtnType)pMsg->btnData[i][j].m_eType;
					m_QuickBarFlags[i+m_nQuickBar_Col_Count][j].m_bIsChange = FALSE;
					m_QuickBarFlags[i+m_nQuickBar_Col_Count][j].m_dwParam1  =  pMsg->btnData[i][j].m_dwParam1;
					m_QuickBarFlags[i+m_nQuickBar_Col_Count][j].m_dwParam2  =  pMsg->btnData[i][j].m_dwParam2;
					m_QuickBarFlags[i+m_nQuickBar_Col_Count][j].m_n64Param3 =  pMsg->btnData[i][j].m_n64Param3;  
				}
			}

			ReflashShortcutBtn();
			ReflashCDTime();
			SetBtnEnable();
			m_bInitShortcut = TRUE;
			tagGameEvent e(_T("Weapon_SwapEx"), NULL);
			m_pMgr->SendEvent(&e);
			tagGameEvent evt(_T("InitedSkill_Event"), NULL);
			m_pMgr->SendEvent(&evt);
			break;
		
			return 0;

		}

 		

		
	}
	return 0; 
}

DWORD ShortcutBarFrame::OnNS_MsgIsSavedQuickBarData(tagNS_MsgIsSavedQuickBarData* pMsg, DWORD)
{
	if( !P_VALID(pMsg) )
	{
		return 0;
	}
	DWORD dwLocalID = RoleMgr::Inst()->GetLocalPlayerID();

	if ( dwLocalID != pMsg->dwRoleID)
	{
		return 0;
	}

	if ( pMsg->nIsSaved == static_cast<INT16>(Enum_NotSaved))
	{
         m_bHaveNotSavedQuickBarData = TRUE ;
	}
	return E_Success;
} 

void ShortcutBarFrame::AddNewSkill( DWORD dwTypeID )
{
	DWORD adaptID = SkillMgr::Inst()->AdaptFreshHandSkillID(dwTypeID);
	const tagSkillData* pSkill = SkillMgr::Inst()->GetSkillDataByTypeID(dwTypeID);
	if(!P_VALID(pSkill))
		return;
	if(adaptID != dwTypeID) return; // 新手技能，且不符合当前武器，则不添加
    for(int i = 0; i < 12; ++i)
    {
        if (m_pShortcutBtn[0][i]->GetBtnData().GetType() == ESBT_Null && 
			pSkill->pProto->nType2 != ESSTE_Transform)
        {
            m_pShortcutBtn[0][i]->SetBtnData(ESBT_Skill, dwTypeID, NULL, NULL);
            tstring picName = SkillMgr::Inst()->GetSkillIconPath(dwTypeID);
            m_pShortcutBtn[0][i]->ClearPic();
            m_pShortcutBtn[0][i]->SetPicFileName(picName);
            m_pShortcutBtn[0][i]->SetItemNum(0);
			//------------------------------------------------
			BtnData data = m_pShortcutBtn[0][i]->GetBtnData();
			const tagSkillData* pSkillData=SkillMgr::Inst()->GetSkillDataByTypeID(data.GetData1());
			if( P_VALID(pSkillData) && pSkillData->pProto->nWeaponLimit!=0 
			&& (pSkillData->pProto->nWeaponLimit!=ItemMgr::Inst()->GetHandWeaponType()
			&& pSkillData->pProto->nWeaponLimit!=ItemMgr::Inst()->GetHandWeaponType(EEP_LeftHand)) )
				m_pShortcutBtn[0][i]->SetInvalite(TRUE);//图标置灰

			//------------------------------------------------
            return;
        }
		//else if( i < TRANS_BUTTON_COUNT && 
		//		m_pShortcutBtn[10][i]->GetBtnData().GetType() == ESBT_Null && 
		//		pSkill->pProto->nType2 == ESSTE_Transform )
		//{
		//	m_pShortcutBtn[10][i]->SetBtnData(ESBT_Skill, dwTypeID, NULL, NULL);
		//	tstring picName = SkillMgr::Inst()->GetSkillIconPath(dwTypeID);
		//	m_pShortcutBtn[10][i]->ClearPic();
		//	m_pShortcutBtn[10][i]->SetPicFileName(picName);
		//	m_pShortcutBtn[10][i]->SetItemNum(0);
		//	//------------------------------------------------
		//	BtnData data = m_pShortcutBtn[10][i]->GetBtnData();
		//	const tagSkillData* pSkillData=SkillMgr::Inst()->GetSkillDataByTypeID(data.GetData1());
		//	if( P_VALID(pSkillData) && pSkillData->pProto->nWeaponLimit!=0 
		//		&& (pSkillData->pProto->nWeaponLimit!=ItemMgr::Inst()->GetHandWeaponType()
		//		&& pSkillData->pProto->nWeaponLimit!=ItemMgr::Inst()->GetHandWeaponType(EEP_LeftHand)) )
		//		m_pShortcutBtn[0][i]->SetInvalite(TRUE);//图标置灰

		//	//------------------------------------------------
		//	return;
		//}
    }
}

DWORD ShortcutBarFrame::ShowTransSkill(tagGameEvent*)
{
	TList<DWORD> listTransSkill;
	SkillMgr::Inst()->GetTransformSkillList( listTransSkill );
	DWORD dwID;
	int i = 0;
	listTransSkill.ResetIterator();
	while( listTransSkill.PeekNext( dwID ) )
	{
		const tagSkillData* pSkill = SkillMgr::Inst()->GetSkillData(dwID);
		if(!P_VALID(pSkill))
			continue;

		if( i < TRANS_BUTTON_COUNT &&  
			pSkill->pProto->nType2 == ESSTE_Transform )
		{
			m_pShortcutBtn[10][i]->SetBtnData(ESBT_Skill, MTransSkillTypeID(dwID, pSkill->nLevel), NULL, NULL);
			tstring picName = SkillMgr::Inst()->GetSkillIconPath( MTransSkillTypeID(dwID, pSkill->nLevel));
			m_pShortcutBtn[10][i]->ClearPic();
			m_pShortcutBtn[10][i]->SetPicFileName(picName);
			m_pShortcutBtn[10][i]->SetItemNum(0);
			//------------------------------------------------
			BtnData data = m_pShortcutBtn[10][i]->GetBtnData();
			const tagSkillData* pSkillData=SkillMgr::Inst()->GetSkillDataByTypeID(data.GetData1());
			if( P_VALID(pSkillData) && pSkillData->pProto->nWeaponLimit!=0 
				&& (pSkillData->pProto->nWeaponLimit!=ItemMgr::Inst()->GetHandWeaponType()
				&& pSkillData->pProto->nWeaponLimit!=ItemMgr::Inst()->GetHandWeaponType(EEP_LeftHand)) )
				m_pShortcutBtn[0][i]->SetInvalite(TRUE);//图标置灰

			i++;
		}
	}
	m_pWnd_TransSkill->SetInvisible(FALSE);
	return 0;
}

DWORD ShortcutBarFrame::HideTransSkill(tagGameEvent*)
{
	m_pWnd_TransSkill->SetInvisible(TRUE);
	return 0;
}

 DWORD ShortcutBarFrame::ShowAddAtt(tagGameEvent* pEvent)
 {
 	m_pAddatt->SetInvisible(FALSE);
	return 0;
 }
 DWORD ShortcutBarFrame::HideAddAtt(tagGameEvent* pEvent)
 {
 	m_pAddatt->SetInvisible(TRUE);
 	return 0;
 }
 DWORD ShortcutBarFrame::ShowAddTelent(tagGameEvent* pEvent)
 {
 	m_pAddTelent->SetInvisible(FALSE);
	return 0;
 }
 DWORD ShortcutBarFrame::HideAddTelent(tagGameEvent* pEvent)
 {
 	m_pAddTelent->SetInvisible(TRUE);
 	return 0;
 }

 //新手技能快捷栏处理
 BOOL ShortcutBarFrame::AutoAdaptFreshHandSkill(ShortcutButton* pBtn, DWORD dwSkillID)
 {
	 // 新手技能 自动切换
	 DWORD adaptID = SkillMgr::Inst()->AdaptFreshHandSkillID(dwSkillID);
	 if(adaptID != pBtn->GetBtnData().GetData1())
	 {
		 if(!GT_VALID(adaptID)) // 错误
		 {						
			 pBtn->SetInvalite(true);
		 }
		 else
		 {
			 pBtn->SetBtnData( ESBT_Skill, adaptID, GT_INVALID, 0 );	
			 //快捷按钮图片处理
			 tstring picName = SkillMgr::Inst()->GetSkillIconPath(adaptID);
			 ((ButtonEx*)pBtn)->ClearPic();
			 pBtn->SetPicFileName(picName);
			 pBtn->SetItemNum(0);
		 }
		 return TRUE;
	 }
	 return FALSE;
 }

 DWORD ShortcutBarFrame::OnUpdateLPAttEvent(tagUpdateLPAttEvent* pEvent)
 {
	 LocalPlayer* pLocalPlayer = RoleMgr::Inst()->GetLocalPlayer();
	 if( !P_VALID(pLocalPlayer) )
		 return 0;

	 switch( pEvent->eType )
	 {
	 case ERA_HP:
	 case ERA_MP:
	 case ERA_Rage:
	 case ERA_Vitality:
	 case ERA_Endurance:
		 {
			 ReflashShortcutBtn(true);
		 }
		 break;
	 }
	 return 0;
 }

 DWORD ShortcutBarFrame::OnActiveHolySkillEvent(tagUpdateLPSkillEvent* pEvent)
 {
	 if ( HolyManMgr::Inst()->IsHolyAwakeSkillID(pEvent->dwID) 
		 && pEvent->bAdd == TRUE )
	 {
		 if (P_VALID(m_pBtnHolyMen))
		 {
			 m_pBtnHolyMen->SetInvisible(FALSE);
			 m_pBtnHolyMen->SetEnable(TRUE);
		 }
	 }
	 return 0;
 }

 DWORD ShortcutBarFrame::OnEventClearTalent( tagGameEvent* pGameEvent )
 {
	 ReflashShortcutBtn(true);
	 return 0;
 }

 VOID ShortcutBarFrame::WG_CheckingInPut()
 {
	 return;

	 CombatSys* pCombatSys = (CombatSys*)m_pMgr->GetFrame(_T("CombatSys"));
	 if( !P_VALID( pCombatSys ) )
		 return;

	 if(!pCombatSys->IsWG_Checking_On())
		return;

	 // 战斗符状态不予计时
 	 LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
 	 if (P_VALID(plp))
 	 {
 		 if (plp->IsHookState())
 			 return;
 	 }

	 // 空闲5分钟就开始检测
	 m_dwIdleTime += Kernel::Inst()->GetDeltaTimeDW();
	 if (m_dwIdleTime > 1000*60*5)
	 {
		
		 if (!m_bNetSessionStartCheck)
		 {
			 // 启动netsession检测
			 TObjRef<NetSession>()->StartWGChecking(true);
			 pCombatSys->ResetIllegalSkillTime();

			 m_bNetSessionStartCheck = true;

		 }
		 else	// netsession检测已经启动中
		 {
			 // 非法技能使用大于30次
			 if (	TObjRef<NetSession>()->GetSkillTimes() > 30 
				 || pCombatSys->GetIllegalSkillTime() > 30)
			 {
				 if (P_VALID(GetObj("WG_CatchByMe")))
					 KillObj("WG_CatchByMe");
				 CreateObj("WG_CatchByMe", "MsgBox");
				 TObjRef<MsgBox>("WG_CatchByMe")->Init(_T(""), g_StrTable[_T("WG_CatchByMe")], _T(""), (MsgBoxFlag)(MBF_OK|MBF_Cancel), TRUE);	

				 TObjRef<NetSession>()->Disconnect();
			 }
		 }
	 }
	 else
	 {
		 if (m_bNetSessionStartCheck)
		 {
			 TObjRef<NetSession>()->StartWGChecking(false);
			 m_bNetSessionStartCheck = false;
		 }	
	 }
 }

 // 重新开始反外挂计时 ( 钓鱼的时候会重置)
 DWORD ShortcutBarFrame::OnResetSkillIdleTime(tagGameEvent* pEvent)
 {
	 m_dwIdleTime = 0;
	 if (m_bNetSessionStartCheck)
	 {
		 m_bNetSessionStartCheck = false;
		 TObjRef<NetSession>()->StartWGChecking(false);
		 CombatSys* pCombatSys = (CombatSys*)m_pMgr->GetFrame(_T("CombatSys"));
		 if( P_VALID( pCombatSys ) )
		 {
			 pCombatSys->ResetIllegalSkillTime();
		 }
	 }

	 return 0;
 }
