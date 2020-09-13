#include "StdAfx.h"
#include "ReputeFrame.h"
#include "CharacterFrame.h"
#include "ProgressEx.h"
#include "GuildMgr.h"
#include "RoleMgr.h"
#include "../WorldDefine/role_att.h"
#include "StrengthRankingFrame.h"
#include "WorldFrame.h"
#include <iomanip>
#include "LocalPlayer.h"
#include "RoleMgr.h"
#include "PvPMgr.h"
#include "RoleEvent.h"

/**
	*struct ReputeGUIs
	*brief 八个氏话声望的结构体，包括，进度条，声望级别，贡献值显示框，名人显示框
	*author hyu
	*remark
*/
//struct ReputeFrame::tagReputeGUICollect
//{
//	tagReputeGUICollect() : pPrgrssReputeValue(NULL), 
//		pStcReputeLevel(NULL),
//		pStcContrib(NULL),
//		pStcHero(NULL) {}
//
//	ProgressEx*			pPrgrssReputeValue;
//	GUIStatic*			pStcReputeLevel;
//	GUIStatic*			pStcContrib;
//	GUIStatic*			pStcHero;
//};

ReputeFrame::ReputeFrame(void) : m_Trunk(this),
	m_pWnd(NULL),
	m_pWndFather(NULL),
	//m_pReputeGUICollect(NULL),
	//m_bisFamousOpen(false),
    m_bneedRefreshRepute(false),
    m_timeFirst(0)
{
}

ReputeFrame::~ReputeFrame(void)
{
	//SAFE_DELETEA(m_pReputeGUICollect);
}

BOOL ReputeFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	//m_pReputeGUICollect = new tagReputeGUICollect[ECLT_NUM];

	BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam);

    /*! 注册网络事件 */
    m_pCmdMgr->Register("NS_SynTotalStrength",  (NETMSGPROC)m_Trunk.sfp2(&ReputeFrame::OnNS_SynTotalStrength),      _T("NS_SynTotalStrength"));
    m_pCmdMgr->Register("NS_GetStrengthDetail", (NETMSGPROC)m_Trunk.sfp2(&ReputeFrame::OnNS_GetStrengthDetail),     _T("NS_GetStrengthDetail"));
	m_pCmdMgr->Register("NS_SynPvPStatics",		(NETMSGPROC)m_Trunk.sfp2(&ReputeFrame::OnNS_SynPvPStatics),			_T("NS_SynPvPStatics"));
	
	/*! 注册游戏事件 */
	//m_pFrameMgr->RegisterEventHandle(_T("FamousUIOpen"), (FRAMEEVENTPROC)m_Trunk.sfp1(&ReputeFrame::OnFamousUIOpen));
	//m_pFrameMgr->RegisterEventHandle(_T("FamousUIClose"), (FRAMEEVENTPROC)m_Trunk.sfp1(&ReputeFrame::OnFamousUIClose));
    m_pFrameMgr->RegisterEventHandle(_T("ReputeChange"), (FRAMEEVENTPROC)m_Trunk.sfp1(&ReputeFrame::OnReputeChange));
    m_pFrameMgr->RegisterEventHandle(_T("tagBecomeEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&ReputeFrame::OnBecomeEvent));
    m_pFrameMgr->RegisterEventHandle(_T("ClanContributeChange"), (FRAMEEVENTPROC)m_Trunk.sfp1(&ReputeFrame::OnRoleClanContributeChange));

	return bRet;
}

BOOL ReputeFrame::Destroy()
{
	//SAFE_DELETEA(m_pReputeGUICollect);

	BOOL bRet = GameFrame::Destroy();

    /*! 注销网络事件 */
    m_pCmdMgr->UnRegister("NS_SynTotalStrength",  (NETMSGPROC)m_Trunk.sfp2(&ReputeFrame::OnNS_SynTotalStrength));
    m_pCmdMgr->UnRegister("NS_GetStrengthDetail", (NETMSGPROC)m_Trunk.sfp2(&ReputeFrame::OnNS_GetStrengthDetail));
	m_pCmdMgr->UnRegister("NS_SynPvPStatics",		(NETMSGPROC)m_Trunk.sfp2(&ReputeFrame::OnNS_SynPvPStatics));
	
	/*! 注销游戏事件 */
	//m_pFrameMgr->UnRegisterEventHandler(_T("FamousUIOpen"), (FRAMEEVENTPROC)m_Trunk.sfp1(&ReputeFrame::OnFamousUIOpen));
	//m_pFrameMgr->UnRegisterEventHandler(_T("FamousUIClose"), (FRAMEEVENTPROC)m_Trunk.sfp1(&ReputeFrame::OnFamousUIClose));
    m_pFrameMgr->UnRegisterEventHandler(_T("ReputeChange"), (FRAMEEVENTPROC)m_Trunk.sfp1(&ReputeFrame::OnReputeChange));
    m_pFrameMgr->UnRegisterEventHandler(_T("tagBecomeEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&ReputeFrame::OnBecomeEvent));
    m_pFrameMgr->UnRegisterEventHandler(_T("ClanContributeChange"), (FRAMEEVENTPROC)m_Trunk.sfp1(&ReputeFrame::OnRoleClanContributeChange));

	return bRet;
}

BOOL ReputeFrame::ReloadUI()
{
	// 创建
	m_pWndFather = m_pGUI->GetWnd(((CharacterFrame*)m_pFather)->GetGUIFullName().c_str());
	m_pWnd = m_pWndFather->GetChild(_T("repute"));
	//m_pBtEnterFameHall = (GUIButton*)m_pWnd->GetChild(_T("personbutt"));
	//m_pBtEnterFameHall->SetEnable(false);
    m_pBtRanking       = (GUIButton*)m_pWnd->GetChild(_T("listback3\\rankingbutt"));

    m_pStcGuildExploit      = (GUIStatic*)m_pWnd->GetChild(_T("listback1\\num1"));
    //m_pStcGuildExploitTxt   = (GUIStatic*)m_pWnd->GetChild(_T("listback1\\word4"));
    m_pStcGuildContribute   = (GUIStatic*)m_pWnd->GetChild(_T("listback1\\num2"));
    //m_pStcExperiace         = (GUIStatic*)m_pWnd->GetChild(_T("listback1\\num3"));

    m_pStcStrengthValue     = (GUIStatic*)m_pWnd->GetChild(_T("listback3\\num1"));
	m_pStcLevel				= (GUIStatic*)m_pWnd->GetChild(_T("listback3\\num2"));
	m_pStcEquip				= (GUIStatic*)m_pWnd->GetChild(_T("listback3\\num3"));
	m_pStcSprite			= (GUIStatic*)m_pWnd->GetChild(_T("listback3\\num4"));
	//m_pStcTitle				= (GUIStatic*)m_pWnd->GetChild(_T("listback3\\num5"));
	//m_pStcFriend			= (GUIStatic*)m_pWnd->GetChild(_T("listback3\\num6"));
    //m_pStcStrengthBase      = (GUIStatic*)m_pWnd->GetChild(_T("listback2\\num2"));
    //m_pStcStrengthEx        = (GUIStatic*)m_pWnd->GetChild(_T("listback2\\num3"));
    //m_pBtStrengthDetail     = (GUIButton*)m_pWnd->GetChild(_T("listback2\\detailbutt"));

	//m_pStcWuXunTxt          = (GUIStatic*)m_pWnd->GetChild(_T("listback3\\word4"));
    m_pStcWuXun             = (GUIStatic*)m_pWnd->GetChild(_T("listback2\\num1"));
	//m_pStcWuXunNextLevel    = (GUIStatic*)m_pWnd->GetChild(_T("listback3\\num10"));
    m_pStcWuJi              = (GUIStatic*)m_pWnd->GetChild(_T("listback2\\num3"));
	//m_pStcWuJiMax           = (GUIStatic*)m_pWnd->GetChild(_T("listback3\\num30"));
    m_pStcTotal		        = (GUIStatic*)m_pWnd->GetChild(_T("listback2\\num2"));
    m_pStcWinCount          = (GUIStatic*)m_pWnd->GetChild(_T("listback2\\num4"));

	m_pFlowerNum			= (GUIStatic*)m_pWnd->GetChild(_T("listback3\\num5"));
	m_pEggNum				= (GUIStatic*)m_pWnd->GetChild(_T("listback3\\num6"));

	//for (int i = 0; i < ECLT_NUM; ++i)
	//{
	//	TCHAR cbuff1[128];
	//	TCHAR cbuff2[128];
	//	int num = i + 1;
	//	_sntprintf(cbuff1, sizeof(cbuff1)/sizeof(TCHAR), _T("%s%d"), _T("propic"), num);
	//	_sntprintf(cbuff2, sizeof(cbuff2)/sizeof(TCHAR), _T("%s%d"), _T("pro"), num);
	//	m_pReputeGUICollect[i].pPrgrssReputeValue = (ProgressEx*)m_pWnd->GetChild(_T("reputebackpic"))->GetChild(cbuff1)->GetChild(cbuff2);
	//	m_pReputeGUICollect[i].pPrgrssReputeValue->SetMaxValue(100);
	//	m_pReputeGUICollect[i].pPrgrssReputeValue->SetSpeed(100000000);

	//	_sntprintf(cbuff2, sizeof(cbuff2)/sizeof(TCHAR), _T("%s%d\\%s%d"), _T("pro"), num, _T("extent"), num);
	//	m_pReputeGUICollect[i].pStcReputeLevel = (GUIStatic*)m_pWnd->GetChild(_T("reputebackpic"))->GetChild(cbuff1)->GetChild(cbuff2);

	//	_sntprintf(cbuff1, sizeof(cbuff1)/sizeof(TCHAR), _T("%s%d"), _T("num"), num);
	//	m_pReputeGUICollect[i].pStcContrib = (GUIStatic*)m_pWnd->GetChild(_T("reputebackpic"))->GetChild(cbuff1);

	//	_sntprintf(cbuff1, sizeof(cbuff1)/sizeof(TCHAR), _T("%s%d"), _T("hero"), num);
	//	m_pReputeGUICollect[i].pStcHero = (GUIStatic*)m_pWnd->GetChild(_T("reputebackpic"))->GetChild(cbuff1);
	//}

	// 注册事件处理函数
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&ReputeFrame::EventHandler));

	/* 创建名人堂Frame */
	//m_pFameHallFrame = (FameHallFrame*)m_pFrameMgr->CreateFrame(m_strName.c_str(), _T("FameHall"), _T("FameHallFrame"), 0);
	//if (!P_VALID(m_pFameHallFrame))
	//	IMSG(_T("Create FameHallFrame failed"));
	
	return TRUE;
}

VOID ReputeFrame::Update()
{
    if (m_pWnd->IsInvisible())
        return;

    if (m_bneedRefreshRepute && Kernel::Inst()->GetAccumTimeDW() - m_timeFirst > 50)
    {
        m_pWnd->SetRefresh(TRUE);
        m_bneedRefreshRepute = false;
    }
}

DWORD ReputeFrame::EventHandler( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

	switch (pEvent->eEvent)
	{
	case EGUIE_Click:
		//if (pWnd == m_pBtEnterFameHall)
		//{

		//	/*! 打开/关闭名人堂窗体 */
		//	if (m_bisFamousOpen)
		//		m_pFrameMgr->SendEvent(&tagGameEvent(_T("SetFamousClose"), this));
		//	else
		//		m_pFrameMgr->SendEvent(&tagGameEvent(_T("SetFamousOpen"), this));

		//}
        //if (pWnd == m_pBtStrengthDetail)
        //{
        //    GameFrame *pFrame = m_pFrameMgr->GetFrame(_T("StrengthShow"));
        //    if (!P_VALID(pFrame))
        //    {
        //        m_pFrameMgr->CreateFrame(_T("World"), _T("StrengthShow"), _T("StrengthShowFrame"), 0);
        //    }
        //    else
        //    {
        //        m_pFrameMgr->AddToDestroyList(pFrame);
        //    }
        //}
        if (pWnd == m_pBtRanking)
        {            
			StrengthRankingFrame *pFrame = (StrengthRankingFrame*)(TObjRef<GameFrameMgr>()->GetFrame(_T("StrengthRanking")));
            if (!P_VALID(pFrame))
            {
                StrengthRankingFrame *pFrame = (StrengthRankingFrame*)(TObjRef<GameFrameMgr>()->CreateFrame(GetWorldFrm()->GetName().c_str(), _T("StrengthRanking"), _T("StrengthRankingFrame"), 0));
            }
            else
            {
                TObjRef<GameFrameMgr>()->AddToDestroyList(pFrame);
            }
        }
		break;
	}
	return 0;
}

VOID ReputeFrame::UpdateRepute()
{
	// 帮派功勋
    tstringstream tss;
    const tagGuildMember* pLpMember = GuildMgr::Inst()->GetLpInfo();

    if (P_VALID(pLpMember))
    {
        tss << GuildMgr::Inst()->GetLocalPlayerExploit();
        m_pStcGuildExploit->SetText(tss.str().c_str());

        //tss.str(_T(""));
        //tss.clear();
        //tss << _T("(") << GuildMgr::Inst()->GetExpoitTxt(GuildMgr::Inst()->GetLocalPlayerExploit()) << _T(")");
        //m_pStcGuildExploitTxt->SetText(tss.str().c_str());

        tss.str(_T(""));
        tss.clear();
        tss << GuildMgr::Inst()->GetLpInfo()->nContribution;
        m_pStcGuildContribute->SetText(tss.str().c_str());
    }
	else
	{
		tss << 0;
		m_pStcGuildExploit->SetText(tss.str().c_str());

		//tss.str(_T(""));
		//tss.clear();
		//tss << _T("(") << GuildMgr::Inst()->GetExpoitTxt(0) << _T(")");
		//m_pStcGuildExploitTxt->SetText(tss.str().c_str());

		tss.str(_T(""));
		tss.clear();
		tss << 0;
		m_pStcGuildContribute->SetText(tss.str().c_str());
	}

    //const tagGuildMemberClient* pInfo = GuildMgr::Inst()->GetMemberByID(RoleMgr::Inst()->GetLocalPlayerID());
    //if (P_VALID(pInfo))
    //{
    //    tss.str(_T(""));
    //    tss.clear();
    //    tss << pInfo->sInfoEx.nTotalContrib;
    //    m_pStcExperiace->SetText(tss.str().c_str());
    //}

    // 实力
    tagNC_SynTotalStrength msgTotalStrength;
    m_pSession->Send(&msgTotalStrength);

    tagNC_GetStrengthDetail msgStrengthDetail;
    m_pSession->Send(&msgStrengthDetail);

	// 3v3战况OnNc_GetStrengthDetail
	tagNC_SynPvPStatics	msgPvPResult;
	m_pSession->Send(&msgPvPResult);

	//for (INT i = ECLT_BEGIN; i < ECLT_END; ++i)
	//{
	//	/*! 读取声望级别 */
	//	const tagReputeLevel* pReputeLevel = ReputeMgr::Inst()->GetReputeLevel((ECLanType)i);
	//	/*! 读取声望值 */
	//	INT nRepute = ReputeMgr::Inst()->GetReputeValue((ECLanType)i);
	//	/*! 设置颜色 */
	//	ReputeMgr::Inst()->UlitySetPrgrssColor(m_pReputeGUICollect[i].pPrgrssReputeValue, pReputeLevel->eColor);
	//	/*! 设置状态条数据 */
	//	m_pReputeGUICollect[i].pPrgrssReputeValue->SetMaxValue((FLOAT)pReputeLevel->nDiffValue);
	//	m_pReputeGUICollect[i].pPrgrssReputeValue->SetValue((FLOAT)nRepute - pReputeLevel->nFrom);
	//	/*! 设置贡献值 */
	//	TCHAR szTemp[128];
	//	_itot(CurrencyMgr::Inst()->GetClanContribute((ECLanType)i), szTemp, 10);
	//	m_pReputeGUICollect[i].pStcContrib->SetText(szTemp);
 //       /*! 设置名人标记 */
 //       LPCTSTR szFame = ReputeMgr::Inst()->IsFame((ECLanType)i) ? g_StrTable[_T("ClanFame")] : _T("");
 //       m_pReputeGUICollect[i].pStcHero->SetText(szFame);
 //       /*! 设置级别文字 */
 //       tstringstream str;
 //       str << pReputeLevel->szLevel << _T(" ") << setw(6) << ReputeMgr::Inst()->GetReputeValue(static_cast<ECLanType>(i));
 //       m_pReputeGUICollect[i].pStcReputeLevel->SetText(str.str().c_str());
	//}
	
    m_bneedRefreshRepute = true;
    m_timeFirst = Kernel::Inst()->GetAccumTimeDW();

	LocalPlayer* pPlayer = RoleMgr::Inst()->GetLocalPlayer();
	INT iWuXun = pPlayer->GetAttribute(ERA_WuXun);
	INT iWuJi = pPlayer->GetAttribute(ERA_WuJi);
	INT nLevel = PvPMgr::Inst()->Wuxun2Level(iWuXun);
	INT iMaxWuJi = (INT)PvPMgr::Inst()->GetWuXunLevelPro(nLevel).dwWuJiMax;
	tstringstream ts;
	
	//ts << _T("PvP_UI_WuXin") << nLevel;
	//m_pStcWuXunTxt->SetText(g_StrTable[ts.str().c_str()]);
	//m_pStcWuXunTxt->SetRefresh(TRUE);

	ts.str(_T(""));
	ts << iWuXun;
	m_pStcWuXun->SetText(ts.str().c_str());
	m_pStcWuXun->SetRefresh(TRUE);

	//ts.str(_T(""));
	//if(nLevel < 10) ts << _T("/") <<PvPMgr::Inst()->GetWuXunLevelPro(nLevel+1).dwWuXunLowerBound;
	//else ts << _T("/-----");
	//m_pStcWuXunNextLevel->SetText(ts.str().c_str());
	//m_pStcWuXunNextLevel->SetRefresh(TRUE);

	ts.str(_T(""));
	ts << iWuJi;
	m_pStcWuJi->SetText(ts.str().c_str());
	m_pStcWuJi->SetRefresh(TRUE);

	//ts.str(_T(""));
	//ts << _T("/") << iMaxWuJi;
	//m_pStcWuJiMax->SetText(ts.str().c_str());
	//m_pStcWuJiMax->SetRefresh(TRUE);

	//鲜花鸡蛋
	TCHAR szTemp[128] = {0};
	_stprintf(szTemp, _T("%d"), pPlayer->GetAttribute(ERA_FlowerNum));
	m_pFlowerNum->SetText(szTemp);
	_stprintf(szTemp, _T("%d"), pPlayer->GetAttribute(ERA_EggsNum));
	m_pEggNum->SetText(szTemp);
}

//DWORD ReputeFrame::OnFamousUIClose( tagGameEvent* pMsg )
//{
//	m_bisFamousOpen = false;
//	return 0;
//}
//
//DWORD ReputeFrame::OnFamousUIOpen( tagGameEvent* pMsg )
//{
//	m_bisFamousOpen = true;
//	return 0;
//}

DWORD ReputeFrame::OnReputeChange( tagReputeChange* pMsg )
{
    UpdateRepute();
	return 0;
}

DWORD ReputeFrame::OnBecomeEvent( tagBecomeEvent* pMsg )
{
    UpdateRepute();

	return 0;
}

DWORD ReputeFrame::OnRoleClanContributeChange( tagRoleClanContributeChange* pMsg )
{
    UpdateRepute();

	return 0;
}

DWORD ReputeFrame::OnNS_SynTotalStrength( tagNS_SynTotalStrength* pMsg, DWORD )
{
    tstringstream tss;
    tss << pMsg->nTotalStrength;

    m_pStcStrengthValue->SetText(tss.str().c_str());
    m_pStcStrengthValue->SetRefresh(TRUE);

    return 0;
}

DWORD ReputeFrame::OnNS_GetStrengthDetail( tagNS_GetStrengthDetail* pMsg, DWORD )
{
    tstringstream tss;
    tss << pMsg->nLevel;
    m_pStcLevel->SetText(tss.str().c_str());
    m_pStcLevel->SetRefresh(TRUE);

    tss.str(_T(""));
    tss.clear();
    tss << pMsg->nEquip;
    m_pStcEquip->SetText(tss.str().c_str());
    m_pStcEquip->SetRefresh(TRUE);

	tss.str(_T(""));
	tss.clear();
	tss << pMsg->nPet;
	m_pStcSprite->SetText(tss.str().c_str());
	m_pStcSprite->SetRefresh(TRUE);

	//tss.str(_T(""));
	//tss.clear();
	//tss << pMsg->nFriendship;
	//m_pStcFriend->SetText(tss.str().c_str());
	//m_pStcFriend->SetRefresh(TRUE);

	//tss.str(_T(""));
	//tss.clear();
	//tss << pMsg->nTitle;
	//m_pStcTitle->SetText(tss.str().c_str());
	//m_pStcTitle->SetRefresh(TRUE);

    return 0;
}

DWORD ReputeFrame::OnNS_SynPvPStatics(tagNS_SynPvPStatics* pMsg, DWORD)
{
	TCHAR temp[128];
	_stprintf_s(temp,128, _T("%d"), pMsg->nTotal);
	temp[127] = 0;
	m_pStcTotal->SetText(temp);
	m_pStcTotal->SetRefresh(TRUE);

	_stprintf_s(temp,128, _T("%2.2f%%"), pMsg->nTotal==0?0.:double(pMsg->nWinCount*100)/pMsg->nTotal);
	temp[127] = 0;
	m_pStcWinCount->SetText(temp);
	m_pStcWinCount->SetRefresh(TRUE);

	return 0;
}

DWORD ReputeFrame::OnAttibuteChange(tagUpdateLPAttEvent* pMsg)
{
	switch(pMsg->eType)
	{
	case ERA_WuJi:		
	case ERA_WuXun:
		{
			LocalPlayer* pPlayer = RoleMgr::Inst()->GetLocalPlayer();
			INT iWuXun = pPlayer->GetAttribute(ERA_WuXun);
			INT iWuJi = pPlayer->GetAttribute(ERA_WuJi);
			INT nLevel = PvPMgr::Inst()->Wuxun2Level(iWuXun);
			INT iMaxWuJi = (INT)PvPMgr::Inst()->GetWuXunLevelPro(nLevel).dwWuJiMax;
			tstringstream ts;

			//ts << _T("PvP_UI_WuXin") << nLevel;
			//m_pStcWuXunTxt->SetText(g_StrTable[ts.str().c_str()]);
			//m_pStcWuXunTxt->SetRefresh(TRUE);

			ts.str(_T(""));
			ts << iWuXun;
			m_pStcWuXun->SetText(ts.str().c_str());
			m_pStcWuXun->SetRefresh(TRUE);

			//ts.str(_T(""));
			//if(nLevel < 10) ts << _T("/") <<PvPMgr::Inst()->GetWuXunLevelPro(nLevel+1).dwWuXunLowerBound;
			//else ts << _T("/-----");
			//m_pStcWuXunNextLevel->SetText(ts.str().c_str());
			//m_pStcWuXunNextLevel->SetRefresh(TRUE);

			ts.str(_T(""));
			ts << iWuJi;
			m_pStcWuJi->SetText(ts.str().c_str());
			m_pStcWuJi->SetRefresh(TRUE);

			//ts.str(_T(""));
			//ts << _T("/") << iMaxWuJi;
			//m_pStcWuJiMax->SetText(ts.str().c_str());
			//m_pStcWuJiMax->SetRefresh(TRUE);
		}
		break;
	}
	return 0;
}