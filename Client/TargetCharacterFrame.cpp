#include "StdAfx.h"
#include "TargetCharacterFrame.h"
#include "RemoteRoleStateFrame.h"
#include <iomanip>
#include "..\WorldDefine\role_att.h"
#include "CombatSysUtil.h"
#include "RoleMgr.h"
#include "BlinkStatic.h"
#include "..\WorldDefine\msg_role_vcard.h"
#include "PlayerNameTab.h"
#include "RoleInfoProtoData.h"
#include "SaveAccount.h"
#include "InformationFrame.h"
#include "SaveAccount.h"
//向FWQ发送请救的延迟时间
#define Max_Require_Time 5000

TargetCharacterFrame::TargetCharacterFrame(void) :
    m_Trunk(this),
    m_dwCurrPlayerID(GT_INVALID),
    m_bneedRefreshRepute(false),
    m_timeFirst(0)
{
}

TargetCharacterFrame::~TargetCharacterFrame(void)
{
}

BOOL TargetCharacterFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
    BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam);
    
    // 注册网络事件
    m_pCmdMgr->Register("NS_GetReputation",     (NETMSGPROC)m_Trunk.sfp2(&TargetCharacterFrame::OnNS_GetReputation),    _T("NS_GetReputation"));
    m_pCmdMgr->Register("NS_GetRoleTitles",     (NETMSGPROC)m_Trunk.sfp2(&TargetCharacterFrame::OnNS_GetRoleTitles),    _T("NS_GetRoleTitles"));
	m_pCmdMgr->Register("NS_CheckRemoteRoleInfoOpenType",     (NETMSGPROC)m_Trunk.sfp2(&TargetCharacterFrame::OnNS_CheckRoleInfo),    _T("NS_CheckRemoteRoleInfoOpenType"));
	m_pCmdMgr->Register("NS_GetRoleVCard", (NETMSGPROC)m_Trunk.sfp2(&TargetCharacterFrame::OnNS_GetRoleVCard), _T("NS_GetRoleVCard"));
    // 注册游戏事件
    m_pFrameMgr->RegisterEventHandle(_T("tagDispPlayerState"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&TargetCharacterFrame::OnDispPlayerState));
	m_pFrameMgr->RegisterEventHandle(_T("Open_Repute"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&TargetCharacterFrame::OnEventOpenRepute)));
	m_pFrameMgr->RegisterEventHandle(_T("Open_Equip"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&TargetCharacterFrame::OnEventOpenEquip)));
	m_pFrameMgr->RegisterEventHandle(_T("Offline_Player_Equip"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&TargetCharacterFrame::OnEventOfflinePlayerEquip)));

    return bRet;
}

BOOL TargetCharacterFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();

    // 注销网络事件
    m_pCmdMgr->UnRegister("NS_GetReputation",   (NETMSGPROC)m_Trunk.sfp2(&TargetCharacterFrame::OnNS_GetReputation));
    m_pCmdMgr->UnRegister("NS_GetRoleTitles",   (NETMSGPROC)m_Trunk.sfp2(&TargetCharacterFrame::OnNS_GetRoleTitles));
	m_pCmdMgr->UnRegister("NS_CheckRemoteRoleInfoOpenType",   (NETMSGPROC)m_Trunk.sfp2(&TargetCharacterFrame::OnNS_CheckRoleInfo));
	m_pCmdMgr->UnRegister("NS_GetRoleVCard", (NETMSGPROC)m_Trunk.sfp2(&TargetCharacterFrame::OnNS_GetRoleVCard));
    // 注销游戏事件
    m_pFrameMgr->UnRegisterEventHandler(_T("tagDispPlayerState"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&TargetCharacterFrame::OnDispPlayerState));
	m_pFrameMgr->UnRegisterEventHandler(_T("Open_Repute"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&TargetCharacterFrame::OnEventOpenRepute)));
	m_pFrameMgr->UnRegisterEventHandler(_T("Open_Equip"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&TargetCharacterFrame::OnEventOpenEquip)));
	m_pFrameMgr->UnRegisterEventHandler(_T("Offline_Player_Equip"), (FRAMEEVENTPROC)(m_Trunk.sfp1(&TargetCharacterFrame::OnEventOfflinePlayerEquip)));
	if( P_VALID(m_pWnd) )
	{
		m_pGUI->AddToDestroyList(m_pWnd);
		m_pWnd = NULL;
	}

    return bRet;
}

BOOL TargetCharacterFrame::ReloadUI()
{
    // 创建
    XmlElement ele;
    tstring strPath = g_strLocalPath + _T("\\ui\\rolestate2.xml");
    m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str());

    // 更改控件
    m_pGUI->ChangeXml(&ele, "ra_win2\\ra_pic\\rstatepic\\equipback\\role", "ClassName", "PlayerViewWindow");
    m_pGUI->ChangeXml(&ele, "ra_win2\\ra_pic\\roletitle\\titlelistpic\\titlelist", "ClassName", "TreeEx");
	m_pGUI->ChangeXml(&ele, "ra_win2\\ra_pic\\rstatepic\\equipback\\sash\\sashpic", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "ra_win2\\ra_pic\\rstatepic\\equipback\\glove\\glovepic", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "ra_win2\\ra_pic\\rstatepic\\equipback\\fashionback\\backpic", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "ra_win2\\ra_pic\\rstatepic\\equipback\\fashionboot\\bootpic", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "ra_win2\\ra_pic\\rstatepic\\equipback\\fashioncloth\\clothpic", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "ra_win2\\ra_pic\\rstatepic\\equipback\\fashionglove\\glovepic", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "ra_win2\\ra_pic\\rstatepic\\equipback\\ring2\\ring2pic", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "ra_win2\\ra_pic\\rstatepic\\equipback\\boot\\bootpic", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "ra_win2\\ra_pic\\rstatepic\\equipback\\minorwp\\minorwppic", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "ra_win2\\ra_pic\\rstatepic\\equipback\\minorwp1\\minorwppic", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "ra_win2\\ra_pic\\rstatepic\\equipback\\talisman\\talismanpic", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "ra_win2\\ra_pic\\rstatepic\\equipback\\mainwp\\mainwppic", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "ra_win2\\ra_pic\\rstatepic\\equipback\\mantle\\mantlepic", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "ra_win2\\ra_pic\\rstatepic\\equipback\\ring1\\ring1pic", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "ra_win2\\ra_pic\\rstatepic\\equipback\\upper\\upperpic", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "ra_win2\\ra_pic\\rstatepic\\equipback\\necklace\\necklacepic", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "ra_win2\\ra_pic\\rstatepic\\equipback\\hat\\hatpic", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "ra_win2\\ra_pic\\rstatepic\\equipback\\fashionhat\\hatpic", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "ra_win2\\ra_pic\\rstatepic\\equipback\\lower\\lowerpic", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "ra_win2\\ra_pic\\rstatepic\\equipback\\sl\\talismanpic", "ClassName", "ItemButton");


	m_pGUI->ChangeXml(&ele, "ra_win2\\ra_pic\\rstatepic\\equipback\\sash\\sashpic\\pic", "ClassName", "BlinkStatic");
	m_pGUI->ChangeXml(&ele, "ra_win2\\ra_pic\\rstatepic\\equipback\\glove\\glovepic\\pic", "ClassName", "BlinkStatic");
	m_pGUI->ChangeXml(&ele, "ra_win2\\ra_pic\\rstatepic\\equipback\\fashionback\\backpic\\pic", "ClassName", "BlinkStatic");
	m_pGUI->ChangeXml(&ele, "ra_win2\\ra_pic\\rstatepic\\equipback\\fashionboot\\bootpic\\pic", "ClassName", "BlinkStatic");
	m_pGUI->ChangeXml(&ele, "ra_win2\\ra_pic\\rstatepic\\equipback\\fashioncloth\\clothpic\\pic", "ClassName", "BlinkStatic");
	m_pGUI->ChangeXml(&ele, "ra_win2\\ra_pic\\rstatepic\\equipback\\fashionglove\\glovepic\\pic", "ClassName", "BlinkStatic");
	m_pGUI->ChangeXml(&ele, "ra_win2\\ra_pic\\rstatepic\\equipback\\ring2\\ring2pic\\pic", "ClassName", "BlinkStatic");
	m_pGUI->ChangeXml(&ele, "ra_win2\\ra_pic\\rstatepic\\equipback\\boot\\bootpic\\pic", "ClassName", "BlinkStatic");
	m_pGUI->ChangeXml(&ele, "ra_win2\\ra_pic\\rstatepic\\equipback\\minorwp\\minorwppic\\pic", "ClassName", "BlinkStatic");
	m_pGUI->ChangeXml(&ele, "ra_win2\\ra_pic\\rstatepic\\equipback\\minorwp1\\minorwppic\\pic", "ClassName", "BlinkStatic");
	m_pGUI->ChangeXml(&ele, "ra_win2\\ra_pic\\rstatepic\\equipback\\talisman\\talismanpic\\pic", "ClassName", "BlinkStatic");
	m_pGUI->ChangeXml(&ele, "ra_win2\\ra_pic\\rstatepic\\equipback\\mainwp\\mainwppic\\pic", "ClassName", "BlinkStatic");
	m_pGUI->ChangeXml(&ele, "ra_win2\\ra_pic\\rstatepic\\equipback\\mantle\\mantlepic\\pic", "ClassName", "BlinkStatic");
	m_pGUI->ChangeXml(&ele, "ra_win2\\ra_pic\\rstatepic\\equipback\\ring1\\ring1pic\\pic", "ClassName", "BlinkStatic");
	m_pGUI->ChangeXml(&ele, "ra_win2\\ra_pic\\rstatepic\\equipback\\upper\\upperpic\\pic", "ClassName", "BlinkStatic");
	m_pGUI->ChangeXml(&ele, "ra_win2\\ra_pic\\rstatepic\\equipback\\necklace\\necklacepic\\pic", "ClassName", "BlinkStatic");
	m_pGUI->ChangeXml(&ele, "ra_win2\\ra_pic\\rstatepic\\equipback\\hat\\hatpic\\pic", "ClassName", "BlinkStatic");
	m_pGUI->ChangeXml(&ele, "ra_win2\\ra_pic\\rstatepic\\equipback\\fashionhat\\hatpic\\pic", "ClassName", "BlinkStatic");
	m_pGUI->ChangeXml(&ele, "ra_win2\\ra_pic\\rstatepic\\equipback\\lower\\lowerpic\\pic", "ClassName", "BlinkStatic");
	m_pGUI->ChangeXml(&ele, "ra_win2\\ra_pic\\rstatepic\\equipback\\sl\\talismanpic\\pic", "Classname", "BlinkStatic");

	m_pGUI->ChangeXml(&ele, "ra_win2\\ra_pic\\informback\\information\\headpicback", "ClassName", "IconStatic");

    for (int i = 0; i < ECLT_NUM; ++i)
    {
        char cbuff[128];
        _snprintf(cbuff, sizeof(cbuff)/sizeof(char),
            "ra_win2\\ra_pic\\repute\\reputebackpic\\propic%d\\pro%d",
            i + 1, i + 1);
        m_pGUI->ChangeXml(&ele, cbuff, "ClassName", "ProgressEx");
    }
    m_pWnd      =   m_pGUI->CreateWnd(_T("\\desktop"), &ele);
    
    m_pEquip    =   m_pWnd->GetChild(_T("ra_pic\\rstatepic"));
    m_pRepute   =   m_pWnd->GetChild(_T("ra_pic\\repute"));
    m_pTitle    =   m_pWnd->GetChild(_T("ra_pic\\roletitle"));
	m_pInfo	=	m_pWnd->GetChild(_T("ra_pic\\informback"));

    m_pBtEquip  =   static_cast<GUIPushButton*>(m_pWnd->GetChild(_T("ra_pic\\winopt\\optbutt1")));
    m_pBtReput  =   static_cast<GUIPushButton*>(m_pWnd->GetChild(_T("ra_pic\\winopt\\optbutt3")));
    m_pBtTitle  =   static_cast<GUIPushButton*>(m_pWnd->GetChild(_T("ra_pic\\winopt\\optbutt30")));
	m_pBtInfo	=	 static_cast<GUIPushButton*>(m_pWnd->GetChild(_T("ra_pic\\winopt\\optbutt2")));

    m_pBtClose  =   static_cast<GUIButton*>(m_pWnd->GetChild(_T("ra_pic\\closebutt")));

	m_pStcGuildExploit      = (GUIStatic*)m_pWnd->GetChild(_T("ra_pic\\repute\\listback1\\num1"));
	m_pStcGuildContribute   = (GUIStatic*)m_pWnd->GetChild(_T("ra_pic\\repute\\listback1\\num2"));

	m_pStcStrengthValue     = (GUIStatic*)m_pWnd->GetChild(_T("ra_pic\\repute\\listback3\\num1"));
	m_pStcLevel				= (GUIStatic*)m_pWnd->GetChild(_T("ra_pic\\repute\\listback3\\num2"));
	m_pStcEquip				= (GUIStatic*)m_pWnd->GetChild(_T("ra_pic\\repute\\listback3\\num3"));
	m_pStcSprite			= (GUIStatic*)m_pWnd->GetChild(_T("ra_pic\\repute\\listback3\\num4"));
	//m_pStcTitle				= (GUIStatic*)m_pWnd->GetChild(_T("ra_pic\\repute\\listback3\\num5"));
	//m_pStcFriend			= (GUIStatic*)m_pWnd->GetChild(_T("ra_pic\\repute\\listback3\\num6"));

	m_pStcWuXun             = (GUIStatic*)m_pWnd->GetChild(_T("ra_pic\\repute\\listback2\\num1"));
	m_pStcWuJi              = (GUIStatic*)m_pWnd->GetChild(_T("ra_pic\\repute\\listback2\\num3"));
	m_pStcTotal		        = (GUIStatic*)m_pWnd->GetChild(_T("ra_pic\\repute\\listback2\\num2"));
	m_pStcWinCount          = (GUIStatic*)m_pWnd->GetChild(_T("ra_pic\\repute\\listback2\\num4"));

	m_pStcFlowerNum			= (GUIStatic*)m_pWnd->GetChild(_T("ra_pic\\repute\\listback3\\num5"));
	m_pStcEggNum			= (GUIStatic*)m_pWnd->GetChild(_T("ra_pic\\repute\\listback3\\num6"));

    m_pTreeTitleList    =   static_cast<TreeEx*>(m_pWnd->GetChild(_T("ra_pic\\roletitle\\titlelistpic\\titlelist")));
	m_pTreeTitleList->InsterItem(NULL, _T("root"), g_StrTable[_T("Title_Init_Root")]);
    m_pStcexDescription =   static_cast<GUIStaticEx*>(m_pWnd->GetChild(_T("ra_pic\\roletitle\\titledescriptionback\\titlelistbackpic\\description")));

	m_pStcSpouse = (GUIStatic*)m_pWnd->GetChild(_T("ra_pic\\informback\\information\\spouse"));			//配偶
	m_pStcGender = (GUIStatic*)m_pWnd->GetChild(_T("ra_pic\\informback\\information\\gender"));			//性别
	m_pStcConstellation = (GUIStatic*)m_pWnd->GetChild(_T("ra_pic\\informback\\information\\constellation"));	//星座
	m_pStcCity = (GUIStatic*)m_pWnd->GetChild(_T("ra_pic\\informback\\information\\city"));				//城市
	m_pStcChZodiac = (GUIStatic*)m_pWnd->GetChild(_T("ra_pic\\informback\\information\\Zodiac"));			//生肖
	m_pEditBoxExPresent = (GUIEditBoxEx*)m_pWnd->GetChild(_T("ra_pic\\informback\\information\\presentationpic\\presentation"));	//个性签名
	m_pEditNameHistory = (GUIEditBoxEx*)m_pWnd->GetChild(_T("ra_pic\\informback\\information\\presentationpic2\\presentation"));	//更名记录
	m_pStcHeadPic = (IconStatic*)m_pWnd->GetChild(_T("ra_pic\\informback\\information\\headpicback"));			//头像
	m_pStcHeadPicC = (GUIStatic*)m_pWnd->GetChild(_T("ra_pic\\informback\\information\\headpicback\\headpic"));			//头像

	m_pBtRefresh = (GUIButton*)m_pWnd->GetChild(_T("ra_pic\\informback\\refresh"));			//刷新
	m_pBtClose2 = (GUIButton*)m_pWnd->GetChild(_T("ra_pic\\informback\\close"));				//关闭

	m_pBtRefresh->SetInvisible(TRUE);
	m_pBtClose2->SetInvisible(TRUE);

    // 注册事件处理函数
    m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&TargetCharacterFrame::EventHandler));

    // 初始化所有title列表
    InitTree();

    // 初始状态
    m_pWnd->SetInvisible(TRUE);

    m_pEquip->SetInvisible(TRUE);
    m_pRepute->SetInvisible(FALSE);
    m_pTitle->SetInvisible(TRUE);
	m_pInfo->SetInvisible(TRUE);

    m_pBtEquip->SetEnable(true);
    m_pBtReput->SetEnable(true);
    m_pBtTitle->SetEnable(true);
	m_pBtInfo->SetEnable(true);

	m_pRRSF = (RemoteRoleStateFrame*)m_pFrameMgr->CreateFrame(m_strName.c_str(), _T("RemoteRoleState"), _T("RemoteRoleStateFrame"), 0);

    m_pBtEquip->SetState(EGUIBS_PushDown, FALSE);

    return TRUE;
}

DWORD TargetCharacterFrame::OnDispPlayerState( tagDispPlayerState* pMsg )
{
	tagNC_CheckRemoteRoleInfoOpenType msg;
	msg.dwRoleID = pMsg->dwRoleID;
	m_pSession->Send(&msg);
	return 0;
}

VOID TargetCharacterFrame::ShowWnd( BOOL bVal )
{
    m_pWnd->SetInvisible(!bVal);

    if (bVal)
    {

    }
    else
    {
		tagGameEvent evt(_T("DeleRemoteRoleEquipInfo"), NULL);
		m_pFrameMgr->SendEvent(&evt);
    }
}

DWORD TargetCharacterFrame::EventHandler( tagGUIEvent* pEvent )
{
    GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

    switch (pEvent->eEvent)
    {
    case EGUIE_Click:
        {
            if (pWnd == m_pBtClose)
            {
                ShowWnd(FALSE);
            }
        }
        break;
    case EGUIE_PushButtonDown:
        {
            if (pEvent->dwParam1 != 1) // 不是按下不处理
                return 0;

            if (pWnd == m_pBtEquip)
            {
                // 装备
                m_pEquip->SetInvisible(FALSE);
                m_pRepute->SetInvisible(TRUE);
                m_pTitle->SetInvisible(TRUE);
				m_pInfo->SetInvisible(TRUE);

				ResetRemoteRoleEquip();
            }
            else if (pWnd == m_pBtReput)
            {
                // 声望
                m_pEquip->SetInvisible(TRUE);
                m_pRepute->SetInvisible(FALSE);
                m_pTitle->SetInvisible(TRUE);
				m_pInfo->SetInvisible(TRUE);
                
                // 获得声望请求
                ResetRepute();
                if (m_dwCurrPlayerID != GT_INVALID && !m_pWnd->IsInvisible())
                    BeginNC_GetReputation(m_dwCurrPlayerID);
            }
            else if (pWnd == m_pBtTitle)
            {
                // 称号
                m_pEquip->SetInvisible(TRUE);
                m_pRepute->SetInvisible(TRUE);
                m_pTitle->SetInvisible(FALSE);
				m_pInfo->SetInvisible(TRUE);

                // 获得称号请求
                ResetTitle();
                if (m_dwCurrPlayerID != GT_INVALID && !m_pWnd->IsInvisible())
                    BeginNC_GetRoleTitles(m_dwCurrPlayerID);
            }
			else if (pWnd == m_pBtInfo)
			{
				SendGetRoleVCard();

				m_pEquip->SetInvisible(TRUE);
				m_pRepute->SetInvisible(TRUE);
				m_pTitle->SetInvisible(TRUE);
				m_pInfo->SetInvisible(FALSE);
			}
        }
        break;
    case EGUIE_ItemClick:
        if (pWnd == m_pTreeTitleList)	// 选中某一称号
        {
            SelectRoleTitle();
        }
        break;
    }
	return 0;
}

VOID TargetCharacterFrame::ResetRepute()
{
//    for (INT i = 0; i != ECLT_NUM; ++i)
//    {
//        m_pPrgrssReputeValue[i]->SetValue(0);
//        m_pStcReputeLevel[i]->SetText(_T(""));
//        //m_pStcContrib[i]->SetText(_T(""));
//        //m_pStcHero[i]->SetText(_T(""));
//    }
}

VOID TargetCharacterFrame::BeginNC_GetReputation( DWORD dwRoleID )
{
    tagNC_GetReputation e;
    e.dwRoleID      =   m_dwCurrPlayerID;
    if (m_pSession->IsConnect())
        m_pSession->Send(&e);
}

DWORD TargetCharacterFrame::OnNS_GetReputation( tagNS_GetReputation* pMsg, DWORD )
{
    if (pMsg->dwRoleID != m_dwCurrPlayerID)
        return 0;

	TCHAR szTemp[X_LONG_NAME] = {0};
	_sntprintf( szTemp, X_LONG_NAME, _T("%d"), pMsg->ReputeData.nGuildExploit );
	m_pStcGuildExploit->SetText(szTemp);
	_sntprintf( szTemp, X_LONG_NAME, _T("%d"), pMsg->ReputeData.nGuildContribute );
	m_pStcGuildContribute->SetText(szTemp);
	_sntprintf( szTemp, X_LONG_NAME, _T("%d"), pMsg->ReputeData.nWuXun );
	m_pStcWuXun->SetText(szTemp);
	_sntprintf( szTemp, X_LONG_NAME, _T("%d"), pMsg->ReputeData.nWuJi );
	m_pStcWuJi->SetText(szTemp);
	_sntprintf( szTemp, X_LONG_NAME, _T("%d"), pMsg->ReputeData.nPVPTimes );
	m_pStcTotal->SetText(szTemp);
	//_sntprintf( szTemp, X_LONG_NAME, _T("%d"), pMsg->ReputeData.nRate );
	_sntprintf( szTemp, X_LONG_NAME, _T("%2.2f%%"), pMsg->ReputeData.nWinTimes==0?0.:double(pMsg->ReputeData.nWinTimes*100)/pMsg->ReputeData.nPVPTimes );
	m_pStcWinCount->SetText(szTemp);
	_sntprintf( szTemp, X_LONG_NAME, _T("%d"), pMsg->ReputeData.nTotalStrength );
	m_pStcStrengthValue->SetText(szTemp);
	_sntprintf( szTemp, X_LONG_NAME, _T("%d"), pMsg->ReputeData.nLevel );
	m_pStcLevel->SetText(szTemp);
	_sntprintf( szTemp, X_LONG_NAME, _T("%d"), pMsg->ReputeData.nEquip );
	m_pStcEquip->SetText(szTemp);
	_sntprintf( szTemp, X_LONG_NAME, _T("%d"), pMsg->ReputeData.nFairy );
	m_pStcSprite->SetText(szTemp);
	//_sntprintf( szTemp, X_LONG_NAME, _T("%d"), pMsg->ReputeData.nTitle );
	//m_pStcTitle->SetText(szTemp);
	//_sntprintf( szTemp, X_LONG_NAME, _T("%d"), pMsg->ReputeData.nFriend );
	//m_pStcFriend->SetText(szTemp);
	_sntprintf( szTemp, X_LONG_NAME, _T("%d"), pMsg->ReputeData.nFlowerNum);
	m_pStcFlowerNum->SetText(szTemp);
	_sntprintf( szTemp, X_LONG_NAME, _T("%d"), pMsg->ReputeData.nEggNum);
	m_pStcEggNum->SetText(szTemp);

    m_bneedRefreshRepute = true;
    m_timeFirst = Kernel::Inst()->GetAccumTimeDW();
	return 0;
}


DWORD TargetCharacterFrame::OnNS_CheckRoleInfo( tagNS_CheckRemoteRoleInfoOpenType* pMsg, DWORD )
{
	if( pMsg->dwErrorCode == E_Success )
	{
		m_dwCurrPlayerID        =   pMsg->dwRoleID;
		ShowWnd(TRUE);
		if (!m_pEquip->IsInvisible() && IS_PLAYER(m_dwCurrPlayerID) )
		{
			// 装备
			ResetRemoteRoleEquip();
		}
		else if (!m_pRepute->IsInvisible())
		{
			// 声望
			ResetRepute();
			BeginNC_GetReputation(m_dwCurrPlayerID);
		}
		else if (!m_pTitle->IsInvisible())
		{
			// 称号
			ResetTitle();
			BeginNC_GetRoleTitles(m_dwCurrPlayerID);
		}
		m_pBtReput->SetEnable(TRUE);
		m_pBtTitle->SetEnable(TRUE);
	}
	else
	{
		if(SaveAccount::Inst()->GetLoginMode() != 7)
		{
			ShowWnd(TRUE);
			m_dwCurrPlayerID        =   pMsg->dwRoleID;
			if (!m_pEquip->IsInvisible() && IS_PLAYER(m_dwCurrPlayerID) )
			{
				// 装备
				ResetRemoteRoleEquip();
			}
			m_pBtReput->SetEnable(FALSE);
			m_pBtTitle->SetEnable(FALSE);
		}
		else
		{
			ShowErrorMsg(pMsg->dwErrorCode);
		}
	}
	return 0;
}

VOID TargetCharacterFrame::ResetTitle()
{
    // 清空所有获得的
    for (map<DWORD, tagTitleList>::iterator iter = m_mapOwnedTitles.begin();
        iter != m_mapOwnedTitles.end(); ++iter)
        iter->second.bOwend = false;

    // 拥有数清零
    for (map<DWORD, tagTitleTypeActivedCount>::iterator itActCount = m_mapActivedCount.begin();
        itActCount != m_mapActivedCount.end(); ++itActCount)
        itActCount->second.bCount = 0;

    //! 更新title列表
    UpdateTree();

    //! 刷新界面
    SelectRoleTitle();
}

VOID TargetCharacterFrame::BeginNC_GetRoleTitles( DWORD dwRoleID )
{
    //! 取得角色已获得的所有称号
    tagNC_GetRoleTitles msg;
    msg.dwRoleID = dwRoleID;
    m_pSession->Send(&msg);
}

DWORD TargetCharacterFrame::OnNS_GetRoleTitles( tagNS_GetRoleTitles* pMsg, DWORD )
{
    switch (pMsg->dwErrorCode)
    {
    case E_Title_Success:
        if (m_dwCurrPlayerID == pMsg->dwRoleID)
        {
            for (map<DWORD, tagTitleList>::iterator iter = m_mapOwnedTitles.begin();
                iter != m_mapOwnedTitles.end(); ++iter)
                iter->second.bOwend = false;

            // 拥有数清零
            for (map<DWORD, tagTitleTypeActivedCount>::iterator itActCount = m_mapActivedCount.begin();
                itActCount != m_mapActivedCount.end(); ++itActCount)
                itActCount->second.bCount = 0;

            for (int i = 0, offset = 0;
                i < (int)pMsg->u16TitleNum;
                ++i, offset += sizeof(UINT16))
            {
                UINT16 titleID = 0;
                memcpy(&titleID, pMsg->byData + offset, sizeof(UINT16));

                map<DWORD, tagTitleList>::iterator iter =
                    m_mapOwnedTitles.find((DWORD)titleID);
                if (iter != m_mapOwnedTitles.end())
                {
                    iter->second.bOwend = true;

                    map<DWORD, tagTitleTypeActivedCount>::iterator itCount = m_mapActivedCount.find(iter->second.dwTypeID);
                    if (itCount != m_mapActivedCount.end())
                    {
                        itCount->second.bCount++;
                    }
                }
            }

            //! 更新title列表
            UpdateTree();
            //! 刷新界面
            SelectRoleTitle();
        }
        break;
    }

    return 0;
}

VOID TargetCharacterFrame::InitTree()
{
    //! 从数据中读取称号内容到tree控件中
    //! root
    m_pTreeTitleList->InsterItem(NULL, _T("root"), _T(""));

    const map<DWORD, tagRoleTitleTypeInfo>* pMapType = RoleTitleProtoData::Inst()->GetTypeMap();
    for (map<DWORD, tagRoleTitleTypeInfo>::const_iterator citer = pMapType->begin();
        citer != pMapType->end(); ++citer)
    {
        TCHAR cBuff1[32];
        TCHAR cBuff2[32];
        _sntprintf(cBuff1, sizeof(cBuff1)/sizeof(TCHAR), _T("%d"), citer->first);
        _sntprintf(cBuff2, sizeof(cBuff2)/sizeof(TCHAR),
            _T("%s [%d/%d]"),
            citer->second.strName.c_str(),
            0, citer->second.nTitlesCount);
        DWORD abc = m_pTreeTitleList->InsterItem(_T("\\root"), cBuff1, cBuff2);

        tagTitleTypeActivedCount aData;
        aData.strName       =   citer->second.strName;
        aData.bCount        =   0;
        aData.nTotal        =   citer->second.nTitlesCount;
        aData.dwTreeItemID  =   abc;
        m_mapActivedCount.insert(make_pair(citer->first, aData));
    }

    const map<DWORD, tagRoleTitleProto>* pMapTitle
        = RoleTitleProtoData::Inst()->GetTitleMap();
    for (map<DWORD, tagRoleTitleProto>::const_iterator citer = pMapTitle->begin();
        citer != pMapTitle->end(); ++citer)
    {

        TCHAR cBuff1[32];
        TCHAR cBuff2[32];
        _sntprintf(cBuff1, sizeof(cBuff1)/sizeof(TCHAR), _T("\\root\\%d"), citer->second.m_dwTypeID);
        _sntprintf(cBuff2, sizeof(cBuff2)/sizeof(TCHAR), _T("%d"), citer->first);
        //! 在每个item的text里存放title id

        LPCTSTR szTemp = g_StrTable[_T("RoleTitle2")];

        tagTitleList ttl;
        ttl.bOwend = false;
        ttl.dwTypeID = citer->second.m_dwTypeID;
        ttl.dwItemID = m_pTreeTitleList->InsterItem(cBuff1, cBuff2, szTemp);;
        m_mapOwnedTitles.insert(make_pair(citer->first, ttl));

        m_pTreeTitleList->SetItemColor(ttl.dwItemID, citer->second.m_dwColor);
    }
}

VOID TargetCharacterFrame::UpdateTree()
{
    map<DWORD, tagRoleTitleProto>::const_iterator tIter;
    for (map<DWORD, tagTitleList>::const_iterator citer = m_mapOwnedTitles.begin();
        citer != m_mapOwnedTitles.end(); ++citer)
    {
        tIter = RoleTitleProtoData::Inst()->GetTitleMap()->find(citer->first);
        if (tIter != RoleTitleProtoData::Inst()->GetTitleMap()->end())
        {
            LPCTSTR szTemp = NULL;
            tstring strTemp;
            if (citer->second.bOwend)
            {
                szTemp = tIter->second.m_szTitleName;
            }
            else
                szTemp = g_StrTable[_T("RoleTitle2")];

            m_pTreeTitleList->SetShowText(citer->second.dwItemID, szTemp);
        }
    }

    for (map<DWORD, tagTitleTypeActivedCount>::const_iterator itCount = m_mapActivedCount.begin();
        itCount != m_mapActivedCount.end(); ++itCount)
    {
        tstringstream sstream;
        sstream << itCount->second.strName << _T(" [") << itCount->second.bCount << _T("/") << itCount->second.nTotal << _T("]");
        m_pTreeTitleList->SetShowText(itCount->second.dwTreeItemID, sstream.str().c_str());
    }

    m_pTreeTitleList->SetRefresh();
}

VOID TargetCharacterFrame::SelectRoleTitle()
{
    DWORD pSelItemID = m_pTreeTitleList->GetCurrentSelItem();
    tagGUITreeItem* pTreeItem = m_pTreeTitleList->GetItem(pSelItemID);
    if (P_VALID(pTreeItem))
    {
        UINT16 selTID = (UINT16)_tstoi(pTreeItem->strText.c_str());
        bool bOwend = m_mapOwnedTitles[(DWORD)selTID].bOwend;

        if (true == bOwend && 2 == pTreeItem->nLayer)
        {
            map<DWORD, tagRoleTitleProto>::const_iterator citer
                = RoleTitleProtoData::Inst()->GetTitleMap()->find((DWORD)selTID);
            if (citer != RoleTitleProtoData::Inst()->GetTitleMap()->end())
            {
                m_pStcexDescription->SetText(citer->second.m_strDescription.c_str());
                m_pStcexDescription->SetRefresh();
            }
        }
        else
        {
            m_pStcexDescription->SetText(_T(""));
            m_pStcexDescription->SetRefresh();
        }
    }
}

BOOL TargetCharacterFrame::EscCancel()
{
	if(m_pWnd->IsInvisible())
		return FALSE;
	
	ShowWnd(FALSE);
	return TRUE;
}

VOID TargetCharacterFrame::ResetRemoteRoleEquip()
{
	if( !P_VALID(m_pRRSF) )
		return;

	DWORD time = timeGetTime();
	//5秒内如果查看的还是当前这个人则不向服务器发消息了
	if( m_dwCurrPlayerID==m_pRRSF->GetDisplayRemoteRoleID() && time-m_pRRSF->GetRequireTime()<Max_Require_Time )
	{
		m_pRRSF->UpdateEquipAvatar();
		m_pRRSF->UpdateAllEquip();
		return;
	}
	// 装备
	tagNC_GetRemoteRoleEquipInfo msg;
	msg.dwRoleID = m_dwCurrPlayerID;
	m_pSession->Send(&msg);
	m_pRRSF->SetDisplayRemoteRoleID(m_dwCurrPlayerID);
	m_pRRSF->SetRequireTime(time);
	m_pRRSF->UpdateEquipAvatar();
}

VOID TargetCharacterFrame::Update()
{
    if (m_pWnd->IsInvisible())
        return;

    if (m_bneedRefreshRepute && Kernel::Inst()->GetAccumTimeDW() - m_timeFirst > 50)
    {
        m_pWnd->SetRefresh(TRUE);
        m_bneedRefreshRepute = false;
    }

	m_pRRSF->Update();
}

DWORD TargetCharacterFrame::OnEventOpenRepute(tagOpenReputeEvent *pEvent)
{
	ShowWnd(TRUE);
	m_dwCurrPlayerID = pEvent->dwRoleID;
	m_pBtReput->SetState(EGUIBS_PushDown, TRUE);
	return 0;
}

DWORD TargetCharacterFrame::OnEventOpenEquip(tagOpenEquipEvent *pEvent)
{
	ShowWnd(TRUE);
	m_dwCurrPlayerID = pEvent->dwRoleID;
	tagGameEvent evt(_T("DeleRemoteRoleEquipInfo"), this);
	m_pFrameMgr->SendEvent(&evt);
	m_pBtEquip->SetState(EGUIBS_PushDown, TRUE);
	return 0;
}

void TargetCharacterFrame::ShowErrorMsg( DWORD dwError )
{
	TCHAR szTmp[X_SHORT_NAME];
	_sntprintf(szTmp,sizeof(szTmp)/sizeof(TCHAR), _T("ShowRoleInfo_Code%d"), dwError);
	CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[szTmp]);
}

DWORD TargetCharacterFrame::OnEventOfflinePlayerEquip(tagGameEvent *pEvent)
{
	m_pRRSF->SetDisplayRemoteRoleID(GT_INVALID);
	m_pRRSF->UpdateEquipAvatar();
	return 0;
}

VOID TargetCharacterFrame::SendGetRoleVCard()
{
	m_pStcSpouse->SetText(_T(""));
	m_pStcGender->SetText(_T(""));
	m_pStcConstellation->SetText(_T(""));
	m_pStcChZodiac->SetText(_T(""));
	m_pStcCity->SetText(_T(""));
	m_pEditBoxExPresent->SetText(_T(""));
	m_pEditNameHistory->SetText(_T(""));
	m_pStcHeadPic->SetPic(DEFAULT_PIC_PATH);
	m_pStcHeadPicC->SetRefresh(TRUE);


	//请求获得远程玩家名贴数据
	tagNC_GetRoleVCard msg;
	msg.dwRoleID = m_dwCurrPlayerID;
	m_pSession->Send(&msg);
}

DWORD TargetCharacterFrame::OnNS_GetRoleVCard( tagNS_GetRoleVCard* pMsg, DWORD dwParam )
{
	if (pMsg->dwErrorCode != E_Success)
	{
		if (pMsg->dwErrorCode == E_VCard_NotVisible)
		{
			TCHAR ch[100] = {0};
			_sntprintf(ch, sizeof(ch) / sizeof(TCHAR),
				g_StrTable[_T("InformationInvisable")],
				PlayerNameTab::Inst()->FindNameByID(pMsg->dwRoleID));
			CombatSysUtil::Inst()->ShowScreenCenterMsg(ch);
		}

		return 0;
	}

	if (pMsg->dwRoleID != m_dwCurrPlayerID)
		return 0;

	TCHAR szMMPic[X_LONG_NAME] ;
	TCHAR szFullPath[X_LONG_NAME];
	_stprintf( szFullPath, _T("data\\ui\\photo\\%d_%d.jpg"), SaveAccount::Inst()->GetLoginID(),pMsg->dwAccountID);
	tstring strPath;
	strPath += szFullPath;
	TObjRef<VirtualFileSys> pVFS;
	DWORD dwFileSize =  TObjRef<VirtualFileSys>("VFS_System")->GetSize(strPath.c_str());
	if(dwFileSize != GT_INVALID && dwFileSize != (DWORD)GT_INVALID)	//文件存在
	{
		_stprintf( szMMPic, _T("data\\ui\\photo\\%d_%d.jpg"), SaveAccount::Inst()->GetLoginID(),pMsg->dwAccountID);
		m_pStcHeadPic->SetPic(szMMPic);
	}
	else	//文件不存在
	{
		m_pStcHeadPic->SetPic(DEFAULT_PIC_PATH);
	}
	m_pStcHeadPicC->SetRefresh(TRUE);

	//获得已设置数据
	tagVCardData* pVCardData = (tagVCardData*)pMsg->pData;

	map<DWORD, INT>::iterator it;

	//配偶
	DWORD m_dwCurrMateID = pVCardData->dwMateRoleID;
	if(m_dwCurrMateID)    //有配偶
	{
		LPCTSTR szMateName = PlayerNameTab::Inst()->FindNameByID(m_dwCurrMateID);
		if(szMateName != NULL&&tstring(_T("GFC_NULL")) != szMateName)   //配偶名有效并且在缓存中
		{
			m_pStcSpouse->SetText(szMateName);
		}
		else if(szMateName == NULL)
		{
			m_pStcSpouse->SetText(_T(""));
		}
	}

	//性别
	LPCTSTR szSexText;
	switch (pVCardData->customVCardData.bySex)
	{
	case 1:
		szSexText = g_StrTable[_T("RoleInfoMale")];
		break;
	case 2:
		szSexText = g_StrTable[_T("RoleInfoFemale")];
		break;
	case 3:
		szSexText = g_StrTable[_T("RoleInfoDefault")];
		break;
	default:
		szSexText = _T("");
		break;
	}
	m_pStcGender->SetText(szSexText);

	//星座
	const map<DWORD, tagConstellationProto>* pCstMap = RoleInfoProtoData::inst()->GetConstellation();
	map<DWORD, tagConstellationProto>::const_iterator itCst = pCstMap->find((DWORD)pVCardData->customVCardData.byConstellation);
	if (itCst != pCstMap->end())
	{
		m_pStcConstellation->SetText(itCst->second.strName.c_str());
	}
	else
	{
		m_pStcConstellation->SetText(_T(""));
	}


	//生肖
	const map<DWORD, tagChinesZodiacProto>* pCzcMap = RoleInfoProtoData::inst()->GetChineseZodiac();
	map<DWORD, tagChinesZodiacProto>::const_iterator itCzc = pCzcMap->find((DWORD)pVCardData->customVCardData.byChineseZodiac);
	if (itCzc != pCzcMap->end())
	{
		m_pStcChZodiac->SetText(itCzc->second.strName.c_str());
	}
	else
	{
		m_pStcChZodiac->SetText(_T(""));
	}

	// 区域，省份，城市
	tstring strCity = _T("");

	const map<DWORD, tagAreaProto>* pAreaMap = RoleInfoProtoData::inst()->GetArea();
	map<DWORD, tagAreaProto>::const_iterator itArea = pAreaMap->find((DWORD)pVCardData->customVCardData.byArea);
	if (itArea != pAreaMap->end())
	{
		strCity += itArea->second.strName;

		// 省份
		map<DWORD, tagProvinceProto>::const_iterator itProvince = itArea->second.mapProvince.find((DWORD)pVCardData->customVCardData.byProvince);
		if (itProvince != itArea->second.mapProvince.end())
		{
			strCity += _T(" ");
			strCity += itProvince->second.strName;
		}
	}

	// 城市
	strCity += _T(" ");
	strCity += pVCardData->customVCardData.chCity;
	m_pStcCity->SetText(strCity.c_str());

	char szout[X_LONG_NAME] = {0};
	m_pUtil->UnicodeToUnicode8(m_pStcCity->GetText().c_str(),szout);
	if(memcmp(szout," N/A",4) == 0)    
		m_pStcCity->SetText(_T(""));
	if(memcmp(szout,"N/A",3) == 0)
		m_pStcCity->SetText(_T(""));

	// 个性签名
	TCHAR szbuff[200] = {_T('\0')};
	INT nOffset = (INT)pVCardData->customVCardData.byHeadUrlSize;
	INT nSize = (INT)pVCardData->customVCardData.bySignatureSize;
	memcpy(szbuff, pVCardData->customVCardData.byData + nOffset, nSize);
	m_pEditBoxExPresent->SetText(szbuff);
	if(m_pEditBoxExPresent->GetText(false) == _T("N/A"))
		m_pEditBoxExPresent->SetText(_T(""));

	// 更名记录
	TCHAR szNameHistory[X_HUGE_NAME] = {_T('\0')};
	nOffset += nSize;
	nSize = (INT)pVCardData->customVCardData.byNameHistory;
	memcpy(szNameHistory, pVCardData->customVCardData.byData + nOffset, nSize);
	m_pEditNameHistory->SetText(szNameHistory);
	if(m_pEditNameHistory->GetText(false) == _T("N/A"))
		m_pEditNameHistory->SetText(_T(""));

	return 0;
}