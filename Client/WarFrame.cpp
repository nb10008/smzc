#include "stdafx.h"
#include "WarFrame.h"
#include "RoleEvent.h"
#include "WarMgr.h"
#include "PlayerNameTab.h"
#include "RoleMgr.h"
#include "ItemMgr.h"
#include "IconStatic.h"
#include "LocalPlayer.h"
#include "ItemProtoData.h"
#include "..\WorldDefine\role_att.h"
#include "ItemFrame.h"
#include "ToolTipFrame.h"
#include "ToolTipCreator.h"

WarFrame::WarFrame(void) : m_Trunk(this)
{
}

WarFrame::~WarFrame(void)
{

}

BOOL WarFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam);
	
	m_pMgr->RegisterEventHandle(_T("RefreshWarInfo_Event"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&WarFrame::On_RefreshWarInfo));
	m_pMgr->RegisterEventHandle(_T("EnterWar"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&WarFrame::OnEnterWar));
	m_pMgr->RegisterEventHandle(_T("EnterNormalMap"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&WarFrame::OnEnterNormalMap));
	m_pMgr->RegisterEventHandle(_T("Show_WarResult"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&WarFrame::OnShow_WarResult));
	m_pMgr->RegisterEventHandle(_T("Close_WarFrame"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&WarFrame::OnClose_WarFrame));

	return bRet;
}

BOOL WarFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();

	m_pMgr->UnRegisterEventHandler(_T("RefreshWarInfo_Event"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&WarFrame::On_RefreshWarInfo));
	m_pMgr->UnRegisterEventHandler(_T("EnterWar"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&WarFrame::OnEnterWar));
	m_pMgr->UnRegisterEventHandler(_T("EnterNormalMap"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&WarFrame::OnEnterNormalMap));
	m_pMgr->UnRegisterEventHandler(_T("Show_WarResult"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&WarFrame::OnShow_WarResult));
	m_pMgr->UnRegisterEventHandler(_T("Close_WarFrame"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&WarFrame::OnClose_WarFrame));

	if(P_VALID(m_pWndRoleInfo))
		m_pGUI->AddToDestroyList(m_pWndRoleInfo);

	if(P_VALID(m_pWndShowRoleInfo))
		m_pGUI->AddToDestroyList(m_pWndShowRoleInfo);

	if(P_VALID(m_pWndJieSuan))
		m_pGUI->AddToDestroyList(m_pWndJieSuan);

	return bRet;
}

BOOL WarFrame::ReloadUI()
{
	TCHAR szText[128] = {0};
	// 角色战场信息
	XmlElement ele;
	tstring strPath = g_strLocalPath + _T("\\ui\\information.xml");
	m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str());

	m_pWndRoleInfo = m_pGUI->CreateWnd(_T("\\desktop"), &ele);
	m_pLstRoleInfo		= (GUIListBox*)m_pWndRoleInfo->GetChild(_T("infor\\listback\\member_list"));
	m_pBtnHideRoleInfo	= (GUIButton*)m_pWndRoleInfo->GetChild(_T("infor\\zk"));
	m_pLstRoleInfo->SetColNum(5, 84);
	m_pLstRoleInfo->SetColWidth(0, 90);
	m_pLstRoleInfo->SetColWidth(1, 65);
	m_pLstRoleInfo->SetColWidth(2, 50);
	m_pLstRoleInfo->SetColWidth(3, 55);
	m_pLstRoleInfo->SetColWidth(4, 55);

	m_pWndRoleInfo->SetInvisible(TRUE);

	// 隐藏战场信息
	XmlElement ele1;
	tstring strPath1 = g_strLocalPath + _T("\\ui\\infor3.xml");
	m_pGUI->LoadXml(&ele1, "VFS_System", strPath1.c_str());
	m_pWndShowRoleInfo = m_pGUI->CreateWnd(_T("\\desktop"), &ele1);
	m_pBtnShowRoleInfo	= (GUIButton*)m_pWndShowRoleInfo->GetChild(_T("infor\\hide"));

	m_pWndShowRoleInfo->SetInvisible(TRUE);

	// 结算界面
	XmlElement ele2;
	tstring strPath2 = g_strLocalPath + _T("\\ui\\settlement.xml");
	m_pGUI->LoadXml(&ele2, "VFS_System", strPath2.c_str());
	m_pGUI->ChangeXml(&ele2, "settlement\\settlement\\itempic1\\item1", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&ele2, "settlement\\settlement\\itempic2\\item2", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&ele2, "settlement\\settlement\\itempic3\\item3", "ClassName", "IconStatic");

	m_pWndJieSuan		= m_pGUI->CreateWnd(_T("\\desktop"), &ele2);
	m_pLstJieSuan		= (GUIListBox*)m_pWndJieSuan->GetChild(_T("settlement\\listback\\member_list"));	//列表
	m_pBtnQuitInfo		= (GUIButton*)m_pWndJieSuan->GetChild(_T("settlement\\butt"));	//退出副本
	m_pBtnCloseWnd		= (GUIButton*)m_pWndJieSuan->GetChild(_T("settlement\\closebutt"));	//关闭窗口
	m_pIStcItem1		= (IconStatic*)m_pWndJieSuan->GetChild(_T("settlement\\itempic1\\item1"));
	m_pIStcItem2		= (IconStatic*)m_pWndJieSuan->GetChild(_T("settlement\\itempic2\\item2"));
	m_pIStcItem3		= (IconStatic*)m_pWndJieSuan->GetChild(_T("settlement\\itempic3\\item3"));
	m_pStcItemMeng1		= (GUIStatic*)m_pWndJieSuan->GetChild(_T("settlement\\itempic1\\pic1"));
	m_pStcItemMeng2		= (GUIStatic*)m_pWndJieSuan->GetChild(_T("settlement\\itempic2\\pic2"));
	m_pStcItemMeng3		= (GUIStatic*)m_pWndJieSuan->GetChild(_T("settlement\\itempic3\\pic3"));
	m_pStcExpReward		= (GUIStatic*)m_pWndJieSuan->GetChild(_T("settlement\\num2"));
	m_pStcGold			= (GUIStatic*)m_pWndJieSuan->GetChild(_T("settlement\\moneyback\\goldnum"));
	m_pStcSilver		= (GUIStatic*)m_pWndJieSuan->GetChild(_T("settlement\\moneyback\\silvernum"));
	m_pBtnRefreshReward = (GUIButton*)m_pWndJieSuan->GetChild(_T("settlement\\butt1"));
	m_pBtnReciveReward	= (GUIButton*)m_pWndJieSuan->GetChild(_T("settlement\\butt"));
	m_pBtnCloseScore	= (GUIButton*)m_pWndJieSuan->GetChild(_T("settlement\\closebutt"));
	m_pStcItemMeng1->SetInvisible(TRUE);
	m_pStcItemMeng2->SetInvisible(TRUE);
	m_pStcItemMeng3->SetInvisible(TRUE);

	m_pLstJieSuan->SetColNum(7, 84);
	m_pLstJieSuan->SetColWidth(0, 90);
	m_pLstJieSuan->SetColWidth(1, 65);
	m_pLstJieSuan->SetColWidth(2, 50);
	m_pLstJieSuan->SetColWidth(3, 55);
	m_pLstJieSuan->SetColWidth(4, 55);
	m_pLstJieSuan->SetColWidth(5, 55);
	m_pLstJieSuan->SetColWidth(6, 50);

	m_pWndJieSuan->SetInvisible(TRUE);

	m_pGUI->RegisterEventHandler(m_pWndRoleInfo->GetFullName().c_str(), m_Trunk.sfp1(&WarFrame::EventHandler));
	m_pGUI->RegisterEventHandler(m_pWndShowRoleInfo->GetFullName().c_str(), m_Trunk.sfp1(&WarFrame::EventHandler));
	m_pGUI->RegisterEventHandler(m_pWndJieSuan->GetFullName().c_str(), m_Trunk.sfp1(&WarFrame::EventHandler));

	return TRUE;
}

VOID WarFrame::Update()
{
	if(m_pWndRoleInfo->IsInvisible() == FALSE)
		m_pWndShowRoleInfo->Align();

	if(m_pWndShowRoleInfo->IsInvisible() == FALSE)
		m_pWndRoleInfo->Align();
}

BOOL WarFrame::EscCancel()
{
	return FALSE;
}

DWORD WarFrame::EventHandler( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

	switch (pEvent->eEvent)
	{
	case EGUIE_Drag:
		{
			if(pWnd == m_pIStcItem1)
			{
				m_pStcItemMeng1->SetInvisible(FALSE);
				m_pStcItemMeng2->SetInvisible(TRUE);
				m_pStcItemMeng3->SetInvisible(TRUE);
			}
			else if(pWnd == m_pIStcItem2)
			{
				m_pStcItemMeng1->SetInvisible(TRUE);
				m_pStcItemMeng2->SetInvisible(FALSE);
				m_pStcItemMeng3->SetInvisible(TRUE);
			}
			else if(pWnd == m_pIStcItem3)
			{
				m_pStcItemMeng1->SetInvisible(TRUE);
				m_pStcItemMeng2->SetInvisible(TRUE);
				m_pStcItemMeng3->SetInvisible(FALSE);
			}
		}
		break;
	case EGUIE_Click:
		{
			if(pWnd == m_pBtnHideRoleInfo)
			{
				m_pWndRoleInfo->SetInvisible(TRUE);
				m_pWndShowRoleInfo->SetInvisible(FALSE);
			}
			else if(pWnd == m_pBtnShowRoleInfo)
			{
				m_pWndRoleInfo->SetInvisible(FALSE);
				m_pWndShowRoleInfo->SetInvisible(TRUE);
			}
			//结算相关
			else if(pWnd == m_pBtnRefreshReward)
			{
				tagNC_RefreshBaoXiang cmd;
				m_pSession->Send(&cmd);
			}
			else if(pWnd == m_pBtnReciveReward)
			{
				tagWarResult* pWarResult = WarMgr::Inst()->GetWarResult();
				tagNC_GetWarBaoXiang cmd;
				if(!m_pStcItemMeng1->IsInvisible())
				{
					cmd.byIndex = 0;
					cmd.dwItemID = pWarResult->dwItem1;
				}
				else if(!m_pStcItemMeng2->IsInvisible())
				{
					cmd.byIndex = 1;
					cmd.dwItemID = pWarResult->dwItem2;
				}
				else if(!m_pStcItemMeng3->IsInvisible())
				{
					cmd.byIndex = 2;
					cmd.dwItemID = pWarResult->dwItem3;
				}
				m_pSession->Send(&cmd);
			}
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			if(pWnd == m_pIStcItem1)
			{
				tagWarResult* pWarResult = WarMgr::Inst()->GetWarResult();
				ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pWarResult->dwItem1).c_str());
			}
			else if(pWnd == m_pIStcItem2)
			{
				tagWarResult* pWarResult = WarMgr::Inst()->GetWarResult();
					ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pWarResult->dwItem2).c_str());
			}
			else if(pWnd == m_pIStcItem3)
			{
				tagWarResult* pWarResult = WarMgr::Inst()->GetWarResult();
					ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pWarResult->dwItem3).c_str());
			}
		}
		break;
	case EGUIE_MoveOutOfCtrl:
		{
			ShowTip(pWnd, NULL);
		}
		break;
	}
	return 0;
}

DWORD WarFrame::On_RefreshWarInfo(tagRefreshWarInfo* evt)
{
	std::map<DWORD, tagWarnfo>::iterator it;
	std::map<DWORD, tagWarnfo> tempMap = WarMgr::Inst()->GetWarInfo();
	int nIndex = 0;
	for(it = tempMap.begin(); it != tempMap.end(); ++it)
	{
		if((*it).first == evt->dwRoleID)
			break;
		nIndex++;
	}
	
	TCHAR szText[128] = {0};

	if(m_pLstRoleInfo->GetText(nIndex, 0) == (LPCTSTR)GT_INVALID )
	{
		//取名字
		m_pLstRoleInfo->SetText(nIndex, 0, PlayerNameTab::Inst()->FindNameByID(evt->dwRoleID));  
		m_pLstRoleInfo->SetText(nIndex, 1, _T("0"));
		m_pLstRoleInfo->SetText(nIndex, 2, _T("0"));
		m_pLstRoleInfo->SetText(nIndex, 3, _T("0"));
		m_pLstRoleInfo->SetText(nIndex, 4, _T("0"));
	}

	_stprintf(szText, _T("%d"), evt->dwValue);
	if( evt->eType == ERoleAttrBossKillCount || evt->eType == ERoleAttrRareBossKillCount)
	{
		it = tempMap.find(evt->dwRoleID);
		if(it != tempMap.end())
		{
			_stprintf( szText, _T("%d"), ((*it).second.AWarInfo[ERoleAttrBossKillCount]+(*it).second.AWarInfo[ERoleAttrRareBossKillCount]) );
		}
		
		m_pLstRoleInfo->SetText(nIndex, ERoleAttrBossKillCount+1, szText);
	}
	else
		m_pLstRoleInfo->SetText(nIndex, evt->eType+1, szText);

	return 0;
}

DWORD WarFrame::OnEnterWar(tagGameEvent* evt)
{
	m_pWndRoleInfo->SetInvisible(FALSE);
	m_pWndShowRoleInfo->SetInvisible(TRUE);

	return 0;
}

DWORD WarFrame::OnEnterNormalMap(tagGameEvent* evt)
{
	m_pWndRoleInfo->SetInvisible(TRUE);
	m_pWndShowRoleInfo->SetInvisible(TRUE);
	m_pWndJieSuan->SetInvisible(TRUE);

	m_pLstRoleInfo->Clear();

	return 0;
}

DWORD WarFrame::OnShow_WarResult(tagGameEvent* evt)
{
	m_pWndJieSuan->SetInvisible(FALSE);
	DWORD dwMyScore = 0;
	tagWarResult* pWarResult = WarMgr::Inst()->GetWarResult();

	//刷新界面
	TCHAR szText[128] = {0};
	std::map<DWORD, tagWarnfo>::iterator it;
	std::map<DWORD, tagWarnfo> tempMap = WarMgr::Inst()->GetWarInfo();
	int i = 0;
	for(it = tempMap.begin(); it != tempMap.end(); ++it, i++)
	{
		//名字
		m_pLstJieSuan->SetText(i, 0, PlayerNameTab::Inst()->FindNameByID((*it).first));
		//金币
		_stprintf(szText, _T("%d"), (*it).second.AWarInfo[ERoleAttrWarGold]);
		m_pLstJieSuan->SetText(i, 1, szText);
		//杀怪数
		_stprintf(szText, _T("%d"), (*it).second.AWarInfo[ERoleAttrMonsterKillCount]);
		m_pLstJieSuan->SetText(i, 2, szText);
		//击杀普通boss数(首领)
		_stprintf(szText, _T("%d"), (*it).second.AWarInfo[ERoleAttrBossKillCount]);
		m_pLstJieSuan->SetText(i, 3, szText);
		//击杀稀有boss数(稀有)
		_stprintf(szText, _T("%d"),	(*it).second.AWarInfo[ERoleAttrRareBossKillCount]);
		m_pLstJieSuan->SetText(i, 5, szText);
		//阵亡次数
		_stprintf(szText, _T("%d"),	(*it).second.AWarInfo[ERoleAttrRoleKillCount]);
		m_pLstJieSuan->SetText(i, 4, szText);
		//算总分=击杀怪物数*1+击杀首领数*10+击杀稀有boss*30-死亡次数*50
		INT nTotalSocre = (*it).second.AWarInfo[ERoleAttrMonsterKillCount]+(*it).second.AWarInfo[ERoleAttrBossKillCount]*10+(*it).second.AWarInfo[ERoleAttrRareBossKillCount]*30-(*it).second.AWarInfo[ERoleAttrRoleKillCount]*50;
		
		if(nTotalSocre <= 0)
			nTotalSocre = 0;

		if(!pWarResult->bIsSuccess)	//如果输了积分除以2
		{
			nTotalSocre /= 2;
		}

		if((*it).first == RoleMgr::Inst()->GetLocalPlayerID())
			dwMyScore = nTotalSocre;
		//总分
		_stprintf(szText, _T("%d"),	nTotalSocre);
		m_pLstJieSuan->SetText(i, 6, szText);
	}
	//根据等级和积分算奖励
	BYTE byRoleLevel = RoleMgr::Inst()->GetLocalPlayer()->GetRoleLevel();
	const tagWarReward* pWarReward = ItemProtoData::Inst()->GetWarReward(byRoleLevel);
	if(P_VALID(pWarReward))
	{
		_stprintf(szText, _T("%d"), pWarReward->dwExpReward*dwMyScore);
		m_pStcExpReward->SetText(szText);
		_stprintf(szText, _T("%d"), (pWarReward->dwMoneyReward*dwMyScore/10000));
		m_pStcGold->SetText(szText);
		_stprintf(szText, _T("%d"), (pWarReward->dwMoneyReward*dwMyScore%10000));
		m_pStcSilver->SetText(szText);
	}

	//显示宝箱
	if(P_VALID(pWarResult))
	{
		m_pIStcItem1->SetPic(ItemMgr::Inst()->GetItemIconPath(pWarResult->dwItem1));
		m_pIStcItem2->SetPic(ItemMgr::Inst()->GetItemIconPath(pWarResult->dwItem2));
		m_pIStcItem3->SetPic(ItemMgr::Inst()->GetItemIconPath(pWarResult->dwItem3));
	}

	return 0;
}

DWORD WarFrame::OnClose_WarFrame(tagGameEvent* evt)
{
	m_pWndJieSuan->SetInvisible(TRUE);
	m_pLstJieSuan->Clear();
	m_pStcItemMeng1->SetInvisible(TRUE);
	m_pStcItemMeng2->SetInvisible(TRUE);
	m_pStcItemMeng3->SetInvisible(TRUE);
	m_pStcExpReward->SetText(_T("0"));
	m_pStcGold->SetText(_T("0"));
	m_pStcSilver->SetText(_T("0"));

	m_pWndRoleInfo->SetInvisible(TRUE);
	m_pLstRoleInfo->Clear();

	m_pWndShowRoleInfo->SetInvisible(TRUE);

	return 0;
}
