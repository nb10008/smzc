#include "stdafx.h"
#include "CityWarStateFrame.h"
#include "IconStatic.h"
#include "GlintStatic.h"
#include "CityEvent.h"
#include "../WorldDefine/city_struggle_define.h"
#include "../WorldDefine/msg_city_struggle.h"

CityWarStateFrame::CityWarStateFrame()
:m_Trunk(this)
{
	m_dwDefenceDragonTime = 0;
	m_dwAttackDragonTime = 0;
	memset(m_dwDoorRepireTime, 0 ,sizeof(m_dwDoorRepireTime));
}

CityWarStateFrame::~CityWarStateFrame()
{

}

BOOL CityWarStateFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if(!GameFrame::Init(pMgr, pFather, dwParam)) return FALSE;

	m_pCmdMgr->Register("NS_CityStruggleInfo",					(NETMSGPROC)m_Trunk.sfp2(&CityWarStateFrame::OnNetCityStruggleInfo),		_T("NS_CityStruggleInfo"));	
	m_pCmdMgr->Register("NS_CityStruggleInfoChange",			(NETMSGPROC)m_Trunk.sfp2(&CityWarStateFrame::OnNetCityStruggleInfoChange),	_T("NS_CityStruggleInfoChange"));	
	return TRUE;
}

BOOL CityWarStateFrame::Destroy()
{
	if(P_VALID(m_pWnd)) m_pGUI->AddToDestroyList(m_pWnd);
	if(P_VALID(m_pRuleDescription)) m_pGUI->AddToDestroyList(m_pRuleDescription);

	m_pFrmMgr->UnRegisterEventHandler(_T("tagLeaveCityWarEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&CityWarStateFrame::OnLeaveCityWarEvent));

	m_pCmdMgr->UnRegister("NS_CityStruggleInfo",				(NETMSGPROC)m_Trunk.sfp2(&CityWarStateFrame::OnNetCityStruggleInfo));	
	m_pCmdMgr->UnRegister("NS_CityStruggleInfoChange",			(NETMSGPROC)m_Trunk.sfp2(&CityWarStateFrame::OnNetCityStruggleInfoChange));	
	return GameFrame::Destroy();
}

BOOL CityWarStateFrame::ReloadUI()
{
	XmlElement ele;
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\city_war_state.xml");
	m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str());
	m_pGUI->ChangeXml(&ele, "mostwar_win\\mostwar_pic\\pearl1\\skbutt2", "ClassName", "GlintStatic");
	m_pGUI->ChangeXml(&ele, "mostwar_win\\mostwar_pic\\pearl\\skbutt2", "ClassName", "GlintStatic");
	m_pGUI->ChangeXml(&ele, "mostwar_win\\mostwar_pic\\back\\num", "ClassName", "GlintStatic");
	m_pGUI->ChangeXml(&ele, "mostwar_win\\mostwar_pic\\back1\\num", "ClassName", "GlintStatic");
	m_pGUI->ChangeXml(&ele, "mostwar_win\\mostwar_pic\\pearl2", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&ele, "mostwar_win\\mostwar_pic\\pearl3", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&ele, "mostwar_win\\mostwar_pic\\pearl4", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&ele, "mostwar_win\\mostwar_pic\\pearl5", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&ele, "mostwar_win\\mostwar_pic\\pearl6", "ClassName", "IconStatic");
	m_pGUI->ChangeXml(&ele, "mostwar_win", "AlignType", "InnerRight");
	m_pGUI->ChangeXml(&ele, "mostwar_win", "AlignOffset_y", "0");
	m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &ele);
	m_pWnd->SetInvisible(TRUE);
	m_pWnd->FlipToTop();
	//m_pWnd->SetMovable(FALSE);
	//m_pWnd->SetUnClickable(TRUE);

	m_pBtnWarResult = (GUIButton*)m_pWnd->GetChild(_T("mostwar_pic\\fightbutt"));
	m_pBtnWarRule = (GUIButton*)m_pWnd->GetChild(_T("mostwar_pic\\warbutt"));
	m_pBtnFolder = (GUIButton*)m_pWnd->GetChild(_T("mostwar_pic\\butt2"));
	m_pStaticDefenceCount = (GUIStatic*)m_pWnd->GetChild(_T("mostwar_pic\\word2\\num"));
	m_pStaticAttackCount = (GUIStatic*)m_pWnd->GetChild(_T("mostwar_pic\\word3\\num1"));
	m_pGSAttackDragon = (GlintStatic*)m_pWnd->GetChild(_T("mostwar_pic\\pearl1\\skbutt2"));
	m_pGSDefenceDragon = (GlintStatic*)m_pWnd->GetChild(_T("mostwar_pic\\pearl\\skbutt2"));
	m_pGSAttackTime = (GlintStatic*)m_pWnd->GetChild(_T("mostwar_pic\\back1\\num"));
	m_pGSDefenceTime = (GlintStatic*)m_pWnd->GetChild(_T("mostwar_pic\\back\\num"));
	m_pISColumn[0] = (IconStatic*)m_pWnd->GetChild(_T("mostwar_pic\\pearl2"));
	m_pISColumn[1] = (IconStatic*)m_pWnd->GetChild(_T("mostwar_pic\\pearl3"));
	m_pISColumn[2] = (IconStatic*)m_pWnd->GetChild(_T("mostwar_pic\\pearl4"));
	m_pISColumn[3] = (IconStatic*)m_pWnd->GetChild(_T("mostwar_pic\\pearl5"));
	m_pISColumn[4] = (IconStatic*)m_pWnd->GetChild(_T("mostwar_pic\\pearl6"));
	m_pStaticNumOfWare = (GUIStatic*)m_pWnd->GetChild(_T("mostwar_pic\\word6\\num"));
	m_pStaticDoor[0] = (GUIStatic*)m_pWnd->GetChild(_T("mostwar_pic\\word0\\num"));
	m_pStaticDoor[1] = (GUIStatic*)m_pWnd->GetChild(_T("mostwar_pic\\word12\\num"));
	m_pStaticDoor[2] = (GUIStatic*)m_pWnd->GetChild(_T("mostwar_pic\\word11\\num"));

	m_strColumPic[0][0] = _T("data\\ui\\god\\xuanji_white.bmp"); 
	m_strColumPic[0][1] = _T("data\\ui\\god\\xuanji_blue.bmp"); 
	m_strColumPic[0][2] = _T("data\\ui\\god\\xuanji_red.bmp"); 
	m_strColumPic[1][0] = _T("data\\ui\\god\\taichong_white.bmp"); 
	m_strColumPic[1][1] = _T("data\\ui\\god\\taichong_blue.bmp"); 
	m_strColumPic[1][2] = _T("data\\ui\\god\\taichong_red.bmp"); 
	m_strColumPic[2][0] = _T("data\\ui\\god\\tianfu_white.bmp"); 
	m_strColumPic[2][1] = _T("data\\ui\\god\\tianfu_blue.bmp"); 
	m_strColumPic[2][2] = _T("data\\ui\\god\\tianfu_red.bmp"); 
	m_strColumPic[3][0] = _T("data\\ui\\god\\shenhuo_white.bmp"); 
	m_strColumPic[3][1] = _T("data\\ui\\god\\shenhuo_blue.bmp"); 
	m_strColumPic[3][2] = _T("data\\ui\\god\\shenhuo_red.bmp"); 
	m_strColumPic[4][0] = _T("data\\ui\\god\\xingwang_white.bmp"); 
	m_strColumPic[4][1] = _T("data\\ui\\god\\xingwang_blue.bmp"); 
	m_strColumPic[4][2] = _T("data\\ui\\god\\xingwang_red.bmp"); 

	m_strDragonPic[0] = _T("data\\ui\\god\\L_weikaiqi.bmp"); 
	m_strDragonPic[1] = _T("data\\ui\\god\\L_kaiqi.bmp"); 
	m_strDragonPic[2] = _T("data\\ui\\god\\L_kaiqizhong.bmp"); 

	m_pGSDefenceDragon->SetGlintPic(m_strDragonPic[2].c_str());
	m_pGSAttackDragon->SetGlintPic(m_strDragonPic[2].c_str());

	tstring spritedescpath = g_strLocalPath + _T("\\ui\\msgbox3.xml");
	XmlElement element;
	m_pGUI->LoadXml(&element, "VFS_System", spritedescpath.c_str());
	m_pRuleDescription = m_pGUI->CreateWnd(_T("\\desktop"), &element);

	GUIStaticEx* descriptiontext = (GUIStaticEx*)m_pRuleDescription->GetChild(_T("back\\textback\\win\\text"));
	tstring desc = g_StrTable[_T("CityWar_Rule0")];
	desc += g_StrTable[_T("CityWar_Rule1")];
	desc += g_StrTable[_T("CityWar_Rule2")];
	descriptiontext->SetText(desc.c_str());
	tagPoint size = descriptiontext->GetSize();
	descriptiontext->SetResizable(TRUE);
	size.y = descriptiontext->GetTextTotalHeight();
	descriptiontext->Resize(size);
	descriptiontext->SetResizable(FALSE);
	m_pRuleDescription->SetInvisible(TRUE);
	
	m_pBtnRuleDescOK = (GUIButton*)m_pRuleDescription->GetChild(_T("back\\ok"));
	m_pBtnRuleDescCancel = (GUIButton*)m_pRuleDescription->GetChild(_T("back\\cancel"));

//	m_pBtnFolder->SetInvisible(TRUE);

	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&CityWarStateFrame::EventHandler));
	m_pGUI->RegisterEventHandler(m_pRuleDescription->GetFullName().c_str(), m_Trunk.sfp1(&CityWarStateFrame::RuleEventHandler));
	m_pFrmMgr->RegisterEventHandle(_T("tagLeaveCityWarEvent"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&CityWarStateFrame::OnLeaveCityWarEvent));
	return TRUE;
}

BOOL CityWarStateFrame::EscCancel()
{
	return FALSE;
}

VOID CityWarStateFrame::Update()
{
	if(P_VALID(m_pWnd) && !m_pWnd->IsInvisible())
	{
		if(m_dwDefenceDragonTime > 0)
		{
			DWORD countDownTime = max((INT)m_dwDefenceDragonTime - (INT)timeGetTime(), 0) / 1000;
			TCHAR temp[20];
			wsprintf(temp, _T("%2d:%02d"),int(countDownTime/60), countDownTime % 60);
			if(m_pGSDefenceTime->GetText().compare(temp) != 0)
			{
				if(countDownTime < 5* 60)
					m_pGSDefenceTime->BeginGlint();
				else m_pGSDefenceTime->EndGlint();
				m_pGSDefenceTime->SetText(temp);
				m_pGSDefenceTime->SetRefresh(TRUE);
			}
		}
		else
		{
			if(m_pGSDefenceTime->GetText().compare(_T("∞")) != 0)
			{
				m_pGSDefenceTime->EndGlint();
				m_pGSDefenceTime->SetText(_T("∞"));
				m_pGSDefenceTime->SetRefresh(TRUE);
			}
		}
		if(m_dwAttackDragonTime > 0)
		{
			DWORD countDownTime = max((INT)m_dwAttackDragonTime - (INT)timeGetTime(), 0) / 1000;
			TCHAR temp[20];
			wsprintf(temp, _T("%2d:%02d"),int(countDownTime/60), countDownTime % 60);
			if(m_pGSAttackTime->GetText().compare(temp) != 0)
			{
				if(countDownTime < 5 * 60)
					m_pGSAttackTime->BeginGlint();
				else m_pGSAttackTime->EndGlint();
				m_pGSAttackTime->SetText(temp);
				m_pGSAttackTime->SetRefresh(TRUE);
			}
		}
		else
		{
			if(m_pGSAttackTime->GetText().compare(_T("∞")) != 0)
			{				
				m_pGSAttackTime->EndGlint();
				m_pGSAttackTime->SetText(_T("∞"));
				m_pGSAttackTime->SetRefresh(TRUE);
			}
		}

		for(int i=0; i<NUM_OF_DOOR; i++)
		{
			if(m_dwDoorRepireTime[i] > 0)
			{
				DWORD countDownTime = max((INT)m_dwDoorRepireTime[i] - (INT)timeGetTime(), 0) / (60*1000);
				tstringstream ts;
				ts <<  g_StrTable[_T("CityWar_State_Door_State1")] << countDownTime  << g_StrTable[_T("CityWar_State_Door_State2")];
				if(m_pStaticDoor[i]->GetText().compare(ts.str()) != 0)
				{
					m_pStaticDoor[i]->SetText(ts.str().c_str());
					m_pStaticDoor[i]->SetRefresh(TRUE);
				}
			}
		}
	}
}

DWORD CityWarStateFrame::EventHandler(tagGUIEvent *pGUIEvent)
{
	switch(pGUIEvent->eEvent)
	{
	case EGUIE_Click:
		if(P_VALID(m_pBtnWarResult) && m_pBtnWarResult->GetID() == pGUIEvent->dwWndID) // show result
		{
			tagOpenCityWarResult evt(this);
			m_pFrmMgr->SendEvent(&evt);
		}
		else if(P_VALID(m_pBtnWarRule) && m_pBtnWarRule->GetID() == pGUIEvent->dwWndID) // show rule
		{
			if(m_pRuleDescription->IsInvisible())
			{
				m_pRuleDescription->SetInvisible(FALSE);
				m_pRuleDescription->FlipToTop();
			}
			else
			{
				m_pRuleDescription->SetInvisible(TRUE);
			}
		}
		else if(P_VALID(m_pBtnFolder) && m_pBtnFolder->GetID() == pGUIEvent->dwWndID) //not used
		{

		}
		break;
	}
	return 0;
}

DWORD CityWarStateFrame::RuleEventHandler(tagGUIEvent *pGUIEvent)
{
	switch(pGUIEvent->eEvent)
	{
	case EGUIE_Click:
		if((P_VALID(m_pBtnRuleDescOK) && m_pBtnRuleDescOK->GetID() == pGUIEvent->dwWndID)
			||(P_VALID(m_pBtnRuleDescCancel) && m_pBtnRuleDescCancel->GetID() == pGUIEvent->dwWndID)) 
		{
			m_pRuleDescription->SetInvisible(TRUE);
		}
		break;
	}
	return 0;
}

DWORD CityWarStateFrame::OnLeaveCityWarEvent(tagLeaveCityWarEvent* pGameEvent)
{
	if(P_VALID(m_pWnd)) m_pWnd->SetInvisible(TRUE);
	return 0;
}

DWORD CityWarStateFrame::OnNetCityStruggleInfo(tagNS_CityStruggleInfo* pMsg, DWORD)
{
	SetDefenceCount(pMsg->nDefPlayerNum);
	SetAttackCount(pMsg->nAtkPlayerNum);

	switch(pMsg->sLongZhuState.eForce)
	{
	case ECFT_Attack:
		{
			SetDefenceDragon(FALSE, pMsg->sLongZhuState.bSealing, 0);
			SetAttackDragon(TRUE, FALSE, pMsg->sLongZhuState.nCountDown);			
		}
		break;
	case ECFT_Defence:
		{
			SetDefenceDragon(TRUE, FALSE, pMsg->sLongZhuState.nCountDown);
			SetAttackDragon(FALSE, pMsg->sLongZhuState.bSealing, 0);
		}
		break;
	}

	for(int i=0; i< NUM_OF_COLUMN; i++)
	{
		SetColumnState(i, pMsg->sYinYangZhuState[i].eForce == ECFT_Attack);
	}

	SetWare(pMsg->nWuZiKuNum);

	for(int i=0; i< MAX_CITY_DOOR_NUM; i++)
	{
		SetDoor(pMsg->sDoorState[i].nDoorType-1, pMsg->sDoorState[i].bDestroied, pMsg->sDoorState[i].nCountDown);
	}

	if(P_VALID(m_pWnd)) m_pWnd->SetInvisible(FALSE);
	return 0;
}

DWORD CityWarStateFrame::OnNetCityStruggleInfoChange(tagNS_CityStruggleInfoChange* pMsg, DWORD)
{
	switch(pMsg->eType1)
	{
	case ECSCT_PlayerSumChange:					// 城战一方人数改变
		if(pMsg->nType2 == ECFT_Attack)
		{
			SetAttackCount(pMsg->nData);
		}else if(pMsg->nType2 == ECFT_Defence)
		{
			SetDefenceCount(pMsg->nData);
		}
		break;
	case ECSCT_LongZhuBeSealing:					// 龙珠被封印中
		if(pMsg->nData == ECFT_Attack)
		{
			SetAttackDragon(FALSE, TRUE, 0);
		}else if(pMsg->nData == ECFT_Defence)
		{
			SetDefenceDragon(FALSE, TRUE, 0);
		}
		break;
	case ECSCT_LongZhuCancelSeal:				// 龙珠取消封印
		if(pMsg->nData == ECFT_Attack)
		{
			SetAttackDragon(FALSE, FALSE, 0);
		}else if(pMsg->nData == ECFT_Defence)
		{
			SetDefenceDragon(FALSE, FALSE, 0);
		}
		break;
	case ECSCT_LongZhuBeSeized:					// 龙珠被另一方占领
		if(pMsg->nData == ECFT_Attack)
		{
			SetAttackDragon(TRUE, FALSE, pMsg->nType2*60);
			SetDefenceDragon(FALSE, FALSE, 0);
		}else if(pMsg->nData == ECFT_Defence)
		{
			SetDefenceDragon(TRUE, FALSE, pMsg->nType2*60);
			SetAttackDragon(FALSE, FALSE, 0);
		}
		break;
	case ECSCT_YinYangZhuSeized:					// 阴阳柱被另一方占领
		{
			SetColumnState(pMsg->nType2, pMsg->nData == ECFT_Attack);
		}
		break;
	case ECSCT_WuZiKu:							// 防御联盟物资库
		SetWare(pMsg->nData);
		break;
	case ECSCT_DoorDestroied:					// 城门被击破
		SetDoor(pMsg->nType2-1, TRUE, pMsg->nData);
		break;
	case ECSCT_DoorRepaired:					// 城门修复
		SetDoor(pMsg->nType2-1, FALSE, 0);
		break;
	}
	return 0;
}

void CityWarStateFrame::SetDefenceDragon(BOOL bSealed, BOOL bSealing, INT countDownTime)
{
	if(bSealed)
	{
		m_pGSDefenceDragon->EndGlint();
		IconStatic* pHelper = (IconStatic*)(GUIStatic*)m_pGSDefenceDragon; // downcast is safe, for no member field is added
		pHelper->SetPic(m_strDragonPic[1].c_str());
		m_pGSDefenceDragon->SetPicColor(0xFFFFFFFF);
		TCHAR temp[20];
		wsprintf(temp, _T("%2d:%02d"),int(countDownTime/60), countDownTime % 60);
		m_pGSDefenceTime->SetText(temp);
		m_pGSDefenceTime->SetRefresh(TRUE);
		m_pGSDefenceDragon->SetRefresh(TRUE);
		m_dwDefenceDragonTime = timeGetTime() + countDownTime * 1000;
		m_dwAttackDragonTime = 0;
	}
	else 
	{
		if(bSealing)
		{
			IconStatic* pHelper = (IconStatic*)(GUIStatic*)m_pGSDefenceDragon; // downcast is safe, for no member field is added
			pHelper->SetNoPic();
			m_pGSDefenceDragon->SetGlintPic(m_strDragonPic[2].c_str());
			m_pGSDefenceDragon->BeginGlint();
			m_pGSDefenceDragon->SetRefresh(TRUE);
		}
		else
		{
			m_pGSDefenceDragon->EndGlint();
			IconStatic* pHelper = (IconStatic*)(GUIStatic*)m_pGSDefenceDragon; // downcast is safe, for no member field is added
			pHelper->SetPic(m_strDragonPic[0].c_str());
			m_pGSDefenceDragon->SetPicColor(0xFFFFFFFF);
			m_pGSDefenceDragon->SetRefresh(TRUE);
		}
		m_pGSDefenceTime->SetText(_T("∞"));
		m_pGSDefenceTime->EndGlint();
		m_pGSDefenceTime->SetRefresh(TRUE);
		m_dwDefenceDragonTime = 0;
	}
}

void CityWarStateFrame::SetAttackDragon(BOOL bSealed, BOOL bSealing, INT countDownTime)
{
	if(bSealed)
	{
		m_pGSAttackDragon->EndGlint();
		IconStatic* pHelper = (IconStatic*)(GUIStatic*)m_pGSAttackDragon; // downcast is safe, for no member field is added
		pHelper->SetPic(m_strDragonPic[1].c_str());
		m_pGSAttackDragon->SetPicColor(0xFFFFFFFF);
		TCHAR temp[20];
		wsprintf(temp, _T("%2d:%02d"),int(countDownTime/60), countDownTime % 60);
		m_pGSAttackTime->SetText(temp);
		m_pGSAttackTime->SetRefresh(TRUE);
		m_pGSAttackDragon->SetRefresh(TRUE);
		m_dwAttackDragonTime = timeGetTime() + countDownTime * 1000;
		m_dwDefenceDragonTime = 0;
	}
	else 
	{
		if(bSealing)
		{
			IconStatic* pHelper = (IconStatic*)(GUIStatic*)m_pGSAttackDragon; // downcast is safe, for no member field is added
			pHelper->SetNoPic();
			m_pGSAttackDragon->SetGlintPic(m_strDragonPic[2].c_str());
			m_pGSAttackDragon->BeginGlint();
			m_pGSAttackDragon->SetRefresh(TRUE);
		}
		else
		{
			m_pGSAttackDragon->EndGlint();
			IconStatic* pHelper = (IconStatic*)(GUIStatic*)m_pGSAttackDragon; // downcast is safe, for no member field is added
			pHelper->SetPic(m_strDragonPic[0].c_str());
			m_pGSAttackDragon->SetPicColor(0xFFFFFFFF);
			m_pGSAttackDragon->SetRefresh(TRUE);
		}
		m_pGSAttackTime->SetText(_T("∞"));
		m_pGSAttackTime->EndGlint();
		m_pGSAttackTime->SetRefresh(TRUE);
		m_dwAttackDragonTime = 0;
	}
}

void CityWarStateFrame::SetDefenceCount(INT iCount)
{
	tstringstream ts;
	ts << iCount;
	m_pStaticDefenceCount->SetText(ts.str().c_str());
	m_pStaticDefenceCount->SetRefresh(TRUE);
}

void CityWarStateFrame::SetAttackCount(INT iCount)
{
	tstringstream ts;
	ts << iCount;
	m_pStaticAttackCount->SetText(ts.str().c_str());
	m_pStaticAttackCount->SetRefresh(TRUE);
}

void CityWarStateFrame::SetColumnState(INT iIndex, BOOL bSealed)
{
	if(iIndex<0 || iIndex>= NUM_OF_COLUMN) return ;
	if(bSealed) m_pISColumn[iIndex]->SetPic(m_strColumPic[iIndex][2].c_str());
	else m_pISColumn[iIndex]->SetPic(m_strColumPic[iIndex][1].c_str());
}

void CityWarStateFrame::SetWare(INT iCount)
{
	tstringstream ts;
	ts << iCount;
	m_pStaticNumOfWare->SetText(ts.str().c_str());
	m_pStaticNumOfWare->SetRefresh(TRUE);
}

void CityWarStateFrame::SetDoor(INT iDoorIndex, BOOL bDestroyed, INT repairTime)
{
	if( iDoorIndex>=NUM_OF_DOOR || iDoorIndex<0) return;
	if(!bDestroyed)
	{
		m_pStaticDoor[iDoorIndex]->SetText(g_StrTable[_T("CityWar_State_Door_State0")]);
		m_dwDoorRepireTime[iDoorIndex] = 0;
	}
	else
	{
		if(repairTime<0)
		{
			m_pStaticDoor[iDoorIndex]->SetText(g_StrTable[_T("CityWar_State_Door_State3")]);
			m_dwDoorRepireTime[iDoorIndex] = 0;
		}
		else
		{
			tstringstream ts;
			ts <<  g_StrTable[_T("CityWar_State_Door_State1")] << repairTime  << g_StrTable[_T("CityWar_State_Door_State2")];
			m_pStaticDoor[iDoorIndex]->SetText(ts.str().c_str());
			m_dwDoorRepireTime[iDoorIndex] = timeGetTime() + repairTime * 60 * 1000;
		}
	}
	m_pStaticDoor[iDoorIndex]->SetRefresh(TRUE);
}