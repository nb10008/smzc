#include "stdafx.h"
#include "FamilyQuestFrame.h"
#include "ToolTipFrame.h"
#include "FamilyFrame.h"
#include "FamilyProtoData.h"
#include "CombatSysUtil.h"
#include "IconStatic.h"
#include "..\WorldDefine\msg_family.h"
#include "Item.h"
#include "ItemMgr.h"
#include "DragBtn.h"
#include "RoleMgr.h"
#include "LocalPlayer.h"
#include "ToolTipCreator.h"

FamilyQuestFrame::FamilyQuestFrame():m_Trunk(this)
{
	m_pWnd = NULL;
	m_bIsGetReward = FALSE;
	m_n8QuestIndex = GT_INVALID;
	m_dwItemTypeID = GT_INVALID;
	m_n64EquipID = GT_INVALID;
}

FamilyQuestFrame::~FamilyQuestFrame()
{
}

BOOL FamilyQuestFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam);

	m_pFrameMgr->RegisterEventHandle(_T("SetFamilyQuestInfo"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&FamilyQuestFrame::OnSetFamilyQuestInfo));

	m_pCmdMgr->Register("NS_RewardFamilyMember",		(NETMSGPROC)m_Trunk.sfp2(&FamilyQuestFrame::OnNS_RewardFamilyMember), _T("NS_RewardFamilyMember"));
	m_pCmdMgr->Register("NS_GiveInFamilyQuestItem",		(NETMSGPROC)m_Trunk.sfp2(&FamilyQuestFrame::OnNS_GiveInFamilyQuestItem), _T("NS_GiveInFamilyQuestItem"));


	tagNC_OpenFamilyQuestPage cmd;
	m_pSession->Send(&cmd);
	return bRet;
}

BOOL FamilyQuestFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();
	if( P_VALID(m_pWnd) )
	{
		m_pGUI->AddToDestroyList(m_pWnd);
		m_pWnd = NULL;
	}
	if(P_VALID(m_pWndHandIn))
	{
		m_pGUI->AddToDestroyList(m_pWndHandIn);
		m_pWndHandIn = NULL;
	}
	if(P_VALID(m_pWndReward))
	{
		m_pGUI->AddToDestroyList(m_pWndReward);
		m_pWndReward = NULL;
	}

	m_pFrameMgr->UnRegisterEventHandler(_T("SetFamilyQuestInfo"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&FamilyQuestFrame::OnSetFamilyQuestInfo));

	m_pCmdMgr->UnRegister("NS_RewardFamilyMember",		(NETMSGPROC)m_Trunk.sfp2(&FamilyQuestFrame::OnNS_RewardFamilyMember));
	m_pCmdMgr->UnRegister("NS_GiveInFamilyQuestItem",	(NETMSGPROC)m_Trunk.sfp2(&FamilyQuestFrame::OnNS_GiveInFamilyQuestItem));

	return bRet;
}

BOOL FamilyQuestFrame::ReloadUI()
{
	// 创建
	TCHAR szText[128] = {0};
	m_pWndFather = m_pGUI->GetWnd(((FamilyFrame*)m_pFather)->GetGUIFullName().c_str());
	m_pWnd = m_pWndFather->GetChild(_T("taskback"));
	m_pStcYFinishDegree		= (GUIStatic*)m_pWnd->GetChild(_T("yesterday\\backpic\\word\\num"));								//昨日完成度
	m_pBtnReceive			= (GUIButton*)m_pWnd->GetChild(_T("yesterday\\backpic\\draw"));								//领取奖励
	m_pStcFinishTip			= (GUIStatic*)m_pWnd->GetChild(_T("yesterday\\backpic\\word0"));								//是否可领取奖励提示

	m_pStcTFinishDegree = (GUIStatic*)m_pWnd->GetChild(_T("today\\word\\num"));						//今日完成度
	for(int i = 0; i < MAX_FAMILY_QUESTS_NUM; i++)
	{
		_stprintf(szText, _T("today\\taskback\\task%d\\percent"), i);
		m_pStcPersent[i] = (GUIStatic*)m_pWnd->GetChild(szText);		//任务完成度
		_stprintf(szText, _T("today\\taskback\\task%d\\content"), i);
		m_pStcQuestDesc[i] = (GUIStatic*)m_pWnd->GetChild(szText);		//任务描述
		_stprintf(szText, _T("today\\taskback\\task%d\\word\\num"), i);
		m_pStcCurDegree[i] = (GUIStatic*)m_pWnd->GetChild(szText);		//当前完成进度
		_stprintf(szText, _T("today\\taskback\\task%d\\butt"), i);
		m_pBtnHandIn[i] = (GUIButton*)m_pWnd->GetChild(szText);		//上交
		_stprintf(szText, _T("today\\taskback\\task%d\\available"), i);
		m_pStcKillTip[i] = (GUIStatic*)m_pWnd->GetChild(szText);		//击杀生效..
		_stprintf(szText, _T("today\\taskback\\task%d\\finish"), i);
		m_pStcHasFinishIcon[i] = (GUIStatic*)m_pWnd->GetChild(szText);	//已完成的那个标志..
	}

	XmlElement ele;
	tstring strPath = g_strLocalPath + _T("\\ui\\family_task_award.xml");
	if( !m_pGUI->LoadXml( &ele, "VFS_System", strPath.c_str() ) )
		return FALSE;

	char cText[128];
	for(int i = 0; i < MAX_FQ_RewardNum; i++)
	{
		_snprintf(cText, sizeof(cText)/sizeof(char), "familytaskaward\\awardback\\item_award0\\itemback%d\\icon", i);
		m_pGUI->ChangeXml(&ele, cText,"ClassName","IconStatic");
		_snprintf(cText, sizeof(cText)/sizeof(char), "familytaskaward\\awardback\\item_award1\\itemback%d\\icon", i);
		m_pGUI->ChangeXml(&ele, cText, "ClassName", "IconStatic");
	}
	m_pWndReward		= m_pGUI->CreateWnd( _T("\\desktop"), &ele );
	m_pStcExp			= (GUIStatic*)m_pWndReward->GetChild(_T("awardback\\basic_award\\expback\\num"));
	m_pStcGold			= (GUIStatic*)m_pWndReward->GetChild(_T("awardback\\basic_award\\moneyback\\goldnum"));
	m_pStcSilver		= (GUIStatic*)m_pWndReward->GetChild(_T("awardback\\basic_award\\moneyback\\silvernum"));
	m_pBtnClose			= (GUIButton*)m_pWndReward->GetChild(_T("awardback\\closebutt"));
	m_pBtnGetReward		= (GUIButton*)m_pWndReward->GetChild(_T("awardback\\get"));
	m_pBtnCancel		= (GUIButton*)m_pWndReward->GetChild(_T("awardback\\close"));
	m_pStc80Got			= (GUIStatic*)m_pWndReward->GetChild(_T("awardback\\item_award0\\word0\\state"));
	m_pStc100Got		= (GUIStatic*)m_pWndReward->GetChild(_T("awardback\\item_award1\\word0\\state"));
	for(int i = 0; i < MAX_FQ_RewardNum; i++)
	{
		_stprintf(szText, _T("awardback\\item_award0\\itemback%d\\icon"), i);
		m_pIStcItem[i]		= (IconStatic*)m_pWndReward->GetChild(szText);	
		m_pIStcItem[i]->SetPic(NullIconPatch);
		_stprintf(szText, _T("awardback\\item_award1\\itemback%d\\icon"), i);
		m_pIStcItemEx[i]	= (IconStatic*)m_pWndReward->GetChild(szText);
		m_pIStcItemEx[i]->SetPic(NullIconPatch);
	}
	m_pWndReward->SetInvisible(TRUE);


	tstring strPath1 = g_strLocalPath + _T("\\ui\\common18.xml");
	if(!m_pGUI->LoadXml(&ele, "VFS_System", strPath1.c_str()))
		return FALSE;
	m_pGUI->ChangeXml(&ele, "comm18_win\\pic\\iconback\\icon", "ClassName", "IconStatic");
	m_pWndHandIn			= m_pGUI->CreateWnd(_T("\\desktop"), &ele);
	m_pWndHandInCaption		= (GUIPatch*)m_pWndHandIn->GetChild(_T("pic"));						//
	m_pIStcItemIcon			= (IconStatic*)m_pWndHandInCaption->GetChild(_T("iconback\\icon"));
	m_pStcItemName			= (GUIStatic*)m_pWndHandInCaption->GetChild(_T("name"));
	m_pBtnHandInClose		= (GUIButton*)m_pWndHandInCaption->GetChild(_T("close"));
	m_pBtnOK				= (GUIButton*)m_pWndHandInCaption->GetChild(_T("surebutt"));
	m_pEBoxNum				= (GUIEditBox*)m_pWndHandInCaption->GetChild(_T("word\\inputpic\\num"));
	m_pIStcItemIcon->SetPic(NullIconPatch);
	m_pWndHandIn->SetInvisible(TRUE);

	// 注册事件处理函数
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&FamilyQuestFrame::EventHandler));
	m_pGUI->RegisterEventHandler(m_pWndReward->GetFullName().c_str(), m_Trunk.sfp1(&FamilyQuestFrame::EventHandler));
	m_pGUI->RegisterEventHandler(m_pWndHandIn->GetFullName().c_str(), m_Trunk.sfp1(&FamilyQuestFrame::EventHandler));

	return TRUE;
}

VOID FamilyQuestFrame::Update()
{	

}

BOOL FamilyQuestFrame::EscCancel()
{
	BOOL bRet = FALSE;
	if(!m_pWndHandIn->IsInvisible())
	{
		m_pWndHandIn->SetInvisible(TRUE);
		bRet = TRUE;
	}
	if(!m_pWndReward->IsInvisible())
	{
		m_pWndReward->SetInvisible(TRUE);
		bRet = TRUE;
	}
	return bRet;
}

DWORD FamilyQuestFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	switch (pEvent->eEvent)
	{
	case EGUIE_Click:
		{
			if(pWnd == m_pBtnReceive)	//领取礼物
			{
				if(m_bIsGetReward)
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FQRewardHasGot")]);
					return 0;
				}
				if(m_dwYesterdayDegree < 3000)
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CanNotReceive")]);
					return 0;
				}
				m_pWndReward->SetInvisible(FALSE);
				m_pWndReward->FlipToTop();
				ClearRewardWnd();
				SetRewardInfo();
			}
			else if(pWnd == m_pBtnClose || pWnd == m_pBtnCancel)	
			{
				m_pWndReward->SetInvisible(TRUE);
			}
			else if(pWnd == m_pBtnGetReward)	
			{
				//发送领取奖励消息
				tagNC_RewardFamilyMember cmd;
				m_pSession->Send(&cmd);
				m_pWndReward->SetInvisible(TRUE);
			}
			else if(pWnd == m_pBtnOK)
			{
				//发送确认上交消息
				if(m_dwItemTypeID == GT_INVALID)
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("HandInItemNotFound")]);
					return 0;
				}

				tagNC_GiveInFamilyQuestItem cmd;
				cmd.dwQuestID = m_sFamilyQuestInfo[m_n8QuestIndex].QuestID;
				cmd.dwTypeID = m_dwItemTypeID;
				cmd.n64EquipSerial = m_n64EquipID;
				cmd.nNum = _ttoi(m_pEBoxNum->GetText());
				if(cmd.nNum <= 0)
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("FQGiveInItemNum0")]);
					return 0;
				}
				if(m_n64EquipID != GT_INVALID)
					cmd.dwTypeID = GT_INVALID;
				m_pSession->Send(&cmd);

				m_pWndHandIn->SetInvisible(TRUE);
				ClearHandInWnd();
			}
			else if(pWnd == m_pBtnHandInClose)
			{
				m_pWndHandIn->SetInvisible(TRUE);
				ClearHandInWnd();
			}
			else 
			{
				for(int i = 0; i < MAX_FAMILY_QUESTS_NUM; i++)
				{
					if(pWnd == m_pBtnHandIn[i])
					{
						m_pWndHandIn->FlipToTop();
						//显示上交物品那个小窗口
						ClearHandInWnd();
						m_pWndHandIn->SetInvisible(FALSE);
						m_n8QuestIndex = i;
					}
				}
			}	

		}
		break;
	case EGUIE_Drag:
		{
			
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			for(int i = 0; i < MAX_FQ_RewardNum; i++)
			{
				if(pWnd == m_pIStcItem[i])
				{
					int nRoleLevel = RoleMgr::Inst()->GetLocalPlayer()->GetRoleLevel();
					const tagFamilyQuestRewardProto* pProto = FamilyProtoData::Inst()->FindQuestRewardProto(nRoleLevel);
					if(P_VALID(pProto) && pProto->dwItemID[i] != GT_INVALID)
					{
						ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pProto->dwItemID[i]).c_str());
						break;
					}
				}
			}
			for(int i = 0; i < MAX_FQ_RewardNum; i++)
			{
				if(pWnd == m_pIStcItemEx[i])
				{
					int nRoleLevel = RoleMgr::Inst()->GetLocalPlayer()->GetRoleLevel();
					const tagFamilyQuestRewardProto* pProto = FamilyProtoData::Inst()->FindQuestRewardProto(nRoleLevel);
					if(P_VALID(pProto) && pProto->dwItemIDEx[i] != GT_INVALID)
					{
						ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pProto->dwItemIDEx[i]).c_str());
						break;
					}
				}
			}
		}
		break;
	case EGUIE_MoveOutOfCtrl:
		{
			ShowTip(pWnd, NULL);
		}
		break;
	case EGUISCB_Drop:
		{
			if(pWnd == m_pIStcItemIcon)
			{
				const tagFamilyQuestName* pName = FamilyProtoData::Inst()->FindQuestName(m_sFamilyQuestInfo[m_n8QuestIndex].QuestID);
				if(!P_VALID(pName))
					return 0;

				const tagFamilyQuestAttProto* pAttProto = FamilyProtoData::Inst()->FindQuestAttProto(pName->dwConditionID);
				if(!P_VALID(pAttProto))
					return 0;

				Item* item = ItemMgr::Inst()->GetPocketItem(static_cast<INT16>(LOWORD(pEvent->dwParam2)));

				bool bIsRightItem = FALSE;
				for(int i  = 0; i < MAX_ItemOrMonsterID; i++)
				{
					if(pAttProto->dwItemOrMonsterID[i] == 0 || pAttProto->dwItemOrMonsterID[i] == GT_INVALID)
						break;
					if(pAttProto->dwItemOrMonsterID[i] == item->GetItemTypeID())
						bIsRightItem = TRUE;
				}
				if(!bIsRightItem)
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("HandInItemNotFound")]);
					return 0;
				}

				if(P_VALID(item))
				{
					m_dwItemTypeID = item->GetItemTypeID();
					if(m_dwItemTypeID != GT_INVALID)
					{
						m_pIStcItemIcon->SetPic(ItemMgr::Inst()->GetItemIconPath(m_dwItemTypeID));
						m_pEBoxNum->SetText(_T("1"));
					}
					if(MIsEquipment(m_dwItemTypeID))
					{
						m_n64EquipID = item->GetItemId();
					}
					else 
						m_n64EquipID = GT_INVALID;

					if(m_n64EquipID != GT_INVALID)
						m_pEBoxNum->SetMaxValue(1);
					else
						m_pEBoxNum->SetMaxValue(10000);
				}

			}
		}

	}
	return 0;
}

void FamilyQuestFrame::SetFamilyQuestInfo(tagFamilyQuestDisplay* pData)
{
	for(int i = 0; i < MAX_FAMILY_QUESTS_NUM; i++)
	{
		m_sFamilyQuestInfo[i] = pData->m_sFamilyQuestInfo[i];
	}
	m_dwYesterdayDegree = pData->m_dwYesterdayDegree;	//昨天完成度
	m_bIsGetReward = pData->m_bIsGetReward;			//是否领取昨天的奖励

	ClearInfo();
	RefrashInfo();
}


void FamilyQuestFrame::ClearInfo()
{
	m_pStcFinishTip->SetInvisible(TRUE);
	for(int i = 0; i < MAX_FAMILY_QUESTS_NUM; i++)
	{
		//任务描述
		m_pStcQuestDesc[i]->SetText(_T(""));
		//任务完成度百分比
		m_pStcCurDegree[i]->SetText(_T(""));

		m_pBtnHandIn[i]->SetInvisible(TRUE);
		m_pStcKillTip[i]->SetInvisible(TRUE);
		
		m_pStcHasFinishIcon[i]->SetInvisible(TRUE);
		m_pBtnHandIn[i]->SetInvisible(TRUE);
		m_pStcKillTip[i]->SetInvisible(TRUE);
	
		m_pStcCurDegree[i]->SetText(_T(""));
		m_pStcPersent[i]->SetText(_T(""));
	}
}

void FamilyQuestFrame::RefrashInfo()
{
	TCHAR szText[128] = {0};

	//
	if(m_bIsGetReward)
	{
		m_pBtnReceive->SetEnable(FALSE);
		m_pBtnReceive->SetText(g_StrTable[_T("FamilyQuestReceived")]);
	}
	else
	{
		m_pBtnReceive->SetEnable(TRUE);
		m_pBtnReceive->SetText(g_StrTable[_T("FamilyQuestNotReceive")]);
	}

	//刷新昨日信息
	if(m_dwYesterdayDegree <= 3000)
	{
		_stprintf(szText, _T("%d%%"), m_dwYesterdayDegree/100);
		m_pStcYFinishDegree->SetTextColor(0xffff0000);
		m_pStcFinishTip->SetInvisible(FALSE);
	}
	else if(m_dwYesterdayDegree < 8000)
	{
		_stprintf(szText, _T("%d%%"), m_dwYesterdayDegree/100);
		m_pStcYFinishDegree->SetTextColor(0xffffff00);
		m_pStcFinishTip->SetInvisible(TRUE);
	}
	else if(m_dwYesterdayDegree < 10000)
	{
		_stprintf(szText, _T("%d%%"), m_dwYesterdayDegree/100);
		m_pStcYFinishDegree->SetTextColor(0xffff6600);
		m_pStcFinishTip->SetInvisible(TRUE);
	}
	else if(m_dwYesterdayDegree == 10000)
	{
		_stprintf(szText, _T("%d%%"), m_dwYesterdayDegree/100);
		m_pStcYFinishDegree->SetTextColor(0xff00ff00);
		m_pStcFinishTip->SetInvisible(TRUE);
	}
	m_pStcYFinishDegree->SetText(szText);

	//刷新今日信息

	DWORD dwTodayDegree = 0;
	DWORD dwTemp = 0;
	int nQuestNum = 0;
	for(int i = 0; i < MAX_FAMILY_QUESTS_NUM; i++)
	{
		const tagFamilyQuestName* pName = FamilyProtoData::Inst()->FindQuestName(m_sFamilyQuestInfo[i].QuestID);
		if(P_VALID(pName))
		{
			if(pName->nQuestNum != 0)
				dwTemp = m_sFamilyQuestInfo[i].dwCurDegree*100/pName->nQuestNum;
			dwTodayDegree += dwTemp;
			nQuestNum++;
		}
	}
	if(nQuestNum != 0)
		dwTodayDegree /= nQuestNum; 
	else
		dwTodayDegree = 0;
	_stprintf(szText, _T("%d%%"), dwTodayDegree);
	m_pStcTFinishDegree->SetText(szText);
	m_pWnd->SetRefresh(TRUE);

	for(int i = 0; i < MAX_FAMILY_QUESTS_NUM; i++)
	{
		const tagFamilyQuestName* pName = FamilyProtoData::Inst()->FindQuestName(m_sFamilyQuestInfo[i].QuestID);
		if(!P_VALID(pName))
			return;

		const tagFamilyQuestAttProto* pAttProto = FamilyProtoData::Inst()->FindQuestAttProto(pName->dwConditionID);
		if(!P_VALID(pName) || !P_VALID(pAttProto))
		{
			return;
		}

		if(pName->nQuestNum == 0)
			return ;

		//任务描述
		m_pStcQuestDesc[i]->SetText(pName->szDesc);
		//任务完成度百分比
		_stprintf(szText, _T("%d%%"), (m_sFamilyQuestInfo[i].dwCurDegree)/(pName->nQuestNum));
		m_pStcCurDegree[i]->SetText(szText);
		//任务类型
		if(pName->eType == EQT_Collect)
		{
			m_pBtnHandIn[i]->SetInvisible(FALSE);
			m_pStcKillTip[i]->SetInvisible(TRUE);
		}
		else if(pName->eType == EQT_Kill)
		{
			m_pBtnHandIn[i]->SetInvisible(TRUE);
			m_pStcKillTip[i]->SetInvisible(FALSE);
		}
		//任务完成度
		if(m_sFamilyQuestInfo[i].dwCurDegree == pName->nQuestNum)
		{
			_stprintf(szText, _T("%d/%d"), m_sFamilyQuestInfo[i].dwCurDegree, pName->nQuestNum);
			m_pStcHasFinishIcon[i]->SetInvisible(FALSE);
			m_pBtnHandIn[i]->SetInvisible(TRUE);
			m_pStcKillTip[i]->SetInvisible(TRUE);
		}
		else
		{
			_stprintf(szText, _T("%d/%d"), m_sFamilyQuestInfo[i].dwCurDegree, pName->nQuestNum);
		}
		m_pStcCurDegree[i]->SetText(szText);
		_stprintf(szText, _T("%d%%"), m_sFamilyQuestInfo[i].dwCurDegree*100/pName->nQuestNum);
		m_pStcPersent[i]->SetText(szText);
	}
}

DWORD FamilyQuestFrame::OnNS_RewardFamilyMember(tagNS_RewardFamilyMember* pCmd, DWORD)
{
	if(pCmd->dwErrorCode != E_Success)
	{
		ShowErrorCode(pCmd->dwErrorCode);
		return 0;
	}

	RefrashInfo();
	CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Success")]);
	m_bIsGetReward = TRUE;
	return 0;
}

DWORD FamilyQuestFrame::OnSetFamilyQuestInfo(tagGameEvent* pEvent)
{
	tagFamilyQuestDisplay* pData = FamilyMgr::Inst()->GetFamilyQuestInfo();
	SetFamilyQuestInfo(pData);

	return 0;
}

void FamilyQuestFrame::ShowFrame(BOOL bIsShow)
{
	m_pWnd->SetInvisible(!bIsShow);
	m_pWnd->FlipToTop();
}

void FamilyQuestFrame::ClearHandInWnd()
{
	m_n8QuestIndex = GT_INVALID;
	m_dwItemTypeID = GT_INVALID;
	m_pIStcItemIcon->SetPic(NullIconPatch);
	m_pStcItemName->SetText(_T(""));
	m_pEBoxNum->SetText(_T(""));
}

void FamilyQuestFrame::ClearRewardWnd()
{
	m_pStc80Got->SetTextColor(0xffff0000);
	m_pStc80Got->SetText(g_StrTable[_T("FamilyQuestHasNotGot")]);
	m_pStc100Got->SetTextColor(0xffff0000);
	m_pStc100Got->SetText(g_StrTable[_T("FamilyQuestHasNotGot")]);
	
	m_pStcExp->SetText(_T(""));
	m_pStcGold->SetText(_T(""));
	m_pStcSilver->SetText(_T(""));
	for(int i = 0; i < MAX_FQ_RewardNum; i++)
	{
		m_pIStcItem[i]->SetPic(NullIconPatch);
		m_pIStcItem[i]->SetText(_T(""));
		m_pIStcItemEx[i]->SetPic(NullIconPatch);
		m_pIStcItemEx[i]->SetText(_T(""));
	}
}

void FamilyQuestFrame::SetRewardInfo()
{
	int nRoleLevel = RoleMgr::Inst()->GetLocalPlayer()->GetRoleLevel();
	TCHAR szText[128] = {0};
	const tagFamilyQuestRewardProto* pProto = FamilyProtoData::Inst()->FindQuestRewardProto(nRoleLevel);
	if(P_VALID(pProto))
	{		
		_itot(pProto->dwExp, szText, 10);
		m_pStcExp->SetText(szText);
		_itot(pProto->dwMoney/10000, szText, 10);
		m_pStcGold->SetText(szText);
		_itot(pProto->dwMoney%10000, szText, 10);
		m_pStcSilver->SetText(szText);
		for(int i = 0; i < MAX_FQ_RewardNum; i++)
		{
			if(pProto->dwItemID[i] != GT_INVALID && pProto->dwItemID[i] != 0)
			{
				m_pIStcItem[i]->SetPic(ItemMgr::Inst()->GetItemIconPath(pProto->dwItemID[i]));
				_itot(pProto->dwItemNum[i], szText, 10);
				m_pIStcItem[i]->SetText(szText);
			}

			if(pProto->dwItemIDEx[i] != GT_INVALID && pProto->dwItemIDEx[i] != 0)
			{
				m_pIStcItemEx[i]->SetPic(ItemMgr::Inst()->GetItemIconPath(pProto->dwItemIDEx[i]));
				_itot(pProto->dwItemNumEx[i], szText, 10);
				m_pIStcItemEx[i]->SetText(szText);
			}

		}
	}
	if(m_dwYesterdayDegree >= 8000)
	{
		m_pStc80Got->SetTextColor(0xff00ff00);
		m_pStc80Got->SetText(g_StrTable[_T("FamilyQuestHasGot")]);
	}
	if(m_dwYesterdayDegree == 10000)
	{
		m_pStc100Got->SetTextColor(0xff00ff00);
		m_pStc100Got->SetText(g_StrTable[_T("FamilyQuestHasGot")]);
	}
	m_pWndReward->SetRefresh(TRUE);
}

DWORD FamilyQuestFrame::OnNS_GiveInFamilyQuestItem(tagNS_GiveInFamilyQuestItem* pCmd, DWORD)
{
	if(pCmd->dwErrorCode != E_Success)
	{
		ShowErrorCode(pCmd->dwErrorCode);
		return 0;
	}

	CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Success")]);
	for(int i = 0; i < MAX_FAMILY_QUESTS_NUM; i++)
	{
		if(m_sFamilyQuestInfo[i].QuestID == pCmd->dwQuestID)
		{
			m_sFamilyQuestInfo[i].dwCurDegree = pCmd->nCount;
			ClearInfo();
			RefrashInfo();
			return 0;
		}
	}
	
	return 0;
}

void FamilyQuestFrame::ShowErrorCode(DWORD dwErrCode)
{
	switch(dwErrCode)
	{
	case E_FamilyQuest_GiveInItem_RoleInvalid:			// 上缴物品的角色无效
	case E_FamilyQuest_GiveInItem_QuestInvalid:			// 家族任务ID无效
	case E_FamilyQuest_GiveInItem_QuestNotMatch:		// 角色的家族无此任务
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GiveInItem_CommonError")]);
			break;
		}
	case E_FamilyQuest_GiveInItem_ItemNotEnough:		// 背包内物品不足 
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("GiveInItem_ItemNotEnough")]);
			break;
		}
	case E_FamilyQuest_Rewarded_Error:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Rewarded_Error")]);
			break;
		}
	case E_FamilyQuest_Rewarded_ProcessLimit:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Rewarded_ProcessLimit")]);
			break;
		}
	case E_FamilyQuest_Rewarded_NewJoiner:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Rewarded_NewJoiner")]);
			break;
		}
	case E_FamilyQuest_Rewarded_AlreadyDone:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Rewarded_AlreadyDone")]);
			break;
		}
	case E_FamilyQuest_Rewarded_BagFull:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Rewarded_BagFull")]);
			break;
		}
	case E_FamilyQuest_GiveInItem_NotQuestItem:
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("HandInItemNotFound")]);
			break;
		}
	}
}