#include "stdafx.h"

#include "CityWarResultFrame.h"
#include "../WorldDefine/city_struggle_define.h"
#include "../WorldDefine/msg_city_struggle.h"
#include "LocalPlayer.h"
#include "RoleMgr.h"


CityWarResultFrame::CityWarResultFrame()
:m_Trunk(this)
{

}

CityWarResultFrame::~CityWarResultFrame()
{

}

BOOL CityWarResultFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if(!GameFrame::Init(pMgr, pFather, dwParam))
		return FALSE;

	m_pCmdMgr->Register("NS_CSRoleRecord",			(NETMSGPROC)m_Trunk.sfp2(&CityWarResultFrame::OnNetCityWarRoleRecord),	_T("NS_CSRoleRecord"));	
	return TRUE;
}

BOOL CityWarResultFrame::Destroy()
{
	if(P_VALID(m_pWnd)) m_pGUI->AddToDestroyList(m_pWnd);
	m_pFrmMgr->UnRegisterEventHandler(_T("tagOpenCityWarResult"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&CityWarResultFrame::OnEventOpenCityWarResult));
	m_pCmdMgr->UnRegister("NS_CSRoleRecord",			(NETMSGPROC)m_Trunk.sfp2(&CityWarResultFrame::OnNetCityWarRoleRecord));	
	return GameFrame::Destroy();
}

BOOL CityWarResultFrame::ReloadUI()
{
	XmlElement ele;
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\city_war_result.xml");
	m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str());

	m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &ele);
	m_pWnd->SetInvisible(TRUE);
	m_pWnd->FlipToTop();

	m_pBtnCancel = (GUIButton*)m_pWnd->GetChild(_T("citywarresult_pic\\closebutt"));
	m_pBtnClose = (GUIButton*)m_pWnd->GetChild(_T("citywarresult_pic\\shutbutt"));
	m_pStaticKill = (GUIStatic*)m_pWnd->GetChild(_T("citywarresult_pic\\listback\\listback1\\word1\\num1"));
	m_pStaticDestroyDoor = (GUIStatic*)m_pWnd->GetChild(_T("citywarresult_pic\\listback\\listback1\\word2\\num2"));
	m_pStaticRepairDoor = (GUIStatic*)m_pWnd->GetChild(_T("citywarresult_pic\\listback\\listback1\\word3\\num3"));
	m_pStaticAcquireWare = (GUIStatic*)m_pWnd->GetChild(_T("citywarresult_pic\\listback\\listback1\\word4\\num4"));
	m_pStaticAcquireColumn = (GUIStatic*)m_pWnd->GetChild(_T("citywarresult_pic\\listback\\listback1\\word5\\num5"));
	m_pStaticAcquireDragon = (GUIStatic*)m_pWnd->GetChild(_T("citywarresult_pic\\listback\\listback1\\word6\\num6"));
	m_pStaticGuildReward = (GUIStatic*)m_pWnd->GetChild(_T("citywarresult_pic\\listback\\listback2\\num1"));
	m_pStaticGuildContribution = (GUIStatic*)m_pWnd->GetChild(_T("citywarresult_pic\\listback\\listback2\\num2"));
	m_pStaticMoneyReward = (GUIStatic*)m_pWnd->GetChild(_T("citywarresult_pic\\listback\\listback2\\num3"));
	m_pLstRanking = (GUIListBox*) m_pWnd->GetChild(_T("citywarresult_pic\\listback\\listback0\\member_list"));

	m_pLstRanking->SetColNum(5, 65);
	m_pLstRanking->SetColWidth(0, 40);
	m_pLstRanking->SetColWidth(1, 111);

	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&CityWarResultFrame::EventHandler));
	m_pFrmMgr->RegisterEventHandle(_T("tagOpenCityWarResult"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&CityWarResultFrame::OnEventOpenCityWarResult));
	return TRUE;
}

BOOL CityWarResultFrame::EscCancel()
{
	if(P_VALID(m_pWnd) && !m_pWnd->IsInvisible())
	{
		m_pWnd->SetInvisible(TRUE);
		return TRUE;
	}
	return FALSE;
}

DWORD CityWarResultFrame::EventHandler(tagGUIEvent *pGUIEvent)
{
	switch(pGUIEvent->eEvent)
	{
	case EGUIE_Click:
		if((P_VALID(m_pBtnCancel) && pGUIEvent->dwWndID==m_pBtnCancel->GetID()) || (P_VALID(m_pBtnClose) && pGUIEvent->dwWndID==m_pBtnClose->GetID()))
		{
			m_pWnd->SetInvisible(TRUE);
		}
		break;
	}
	return 0;
}


DWORD CityWarResultFrame::OnNetCityWarRoleRecord(tagNS_CSRoleRecord *pMsg, DWORD)
{
	m_pLstRanking->Clear();
	int rowIndex = 0;
	for(int i=0; i< MAX_CS_RANK_NUM; i++)
	{
		if(pMsg->RoleRank[i].dwRoleID <= 0) break;
		Role* pRole = RoleMgr::Inst()->FindRole(pMsg->RoleRank[i].dwRoleID, true);
		if(!P_VALID(pRole)) continue;
		tstringstream ts;
		ts << rowIndex+1;
		m_pLstRanking->SetText(rowIndex, 0, ts.str().c_str());
		m_pLstRanking->SetText(rowIndex, 1, pRole->IsAttributeInited()?pRole->GetRoleName().c_str(): _T("-"));
		if(pMsg->RoleRank[i].eForce == ECFT_Attack)
			m_pLstRanking->SetText(rowIndex, 2, g_StrTable[_T("CityWar_Result_AttackSide")]);
		else if(pMsg->RoleRank[i].eForce == ECFT_Defence)
			m_pLstRanking->SetText(rowIndex, 2, g_StrTable[_T("CityWar_Result_DefenceSide")]);
		else
			ASSERT(0);
		ts.str(_T(""));
		ts << pMsg->RoleRank[i].byKills;
		m_pLstRanking->SetText(rowIndex, 3, ts.str().c_str());
		ts.str(_T(""));
		ts << pMsg->RoleRank[i].nScore;
		m_pLstRanking->SetText(rowIndex, 4, ts.str().c_str()); 
		rowIndex++;
	}
	m_pLstRanking->SetRefresh(TRUE);

	tstringstream ts;
	ts << pMsg->RoleData.byKills;
	m_pStaticKill->SetText(ts.str().c_str());
	m_pStaticKill->SetRefresh(TRUE);
	ts.str(_T(""));
	ts << pMsg->RoleData.byDestroyDoor;
	m_pStaticDestroyDoor->SetText(ts.str().c_str());
	m_pStaticDestroyDoor->SetRefresh(TRUE);
	ts.str(_T(""));
	ts << pMsg->RoleData.byUseRepairStone;
	m_pStaticRepairDoor->SetText(ts.str().c_str());
	m_pStaticRepairDoor->SetRefresh(TRUE);
	ts.str(_T(""));
	ts << pMsg->RoleData.byDestroyMaterial;
	m_pStaticAcquireWare->SetText(ts.str().c_str());
	m_pStaticAcquireWare->SetRefresh(TRUE);
	ts.str(_T(""));
	ts << pMsg->RoleData.byOpenLongZhu;
	m_pStaticAcquireDragon->SetText(ts.str().c_str());
	m_pStaticAcquireDragon->SetRefresh(TRUE);
	ts.str(_T(""));
	ts << pMsg->RoleData.byOpenYinYang;
	m_pStaticAcquireColumn->SetText(ts.str().c_str());
	m_pStaticAcquireColumn->SetRefresh(TRUE);
	ts.str(_T(""));
	ts << pMsg->nContribute;
	m_pStaticGuildContribution->SetText(ts.str().c_str());
	m_pStaticGuildContribution->SetRefresh(TRUE);
	ts.str(_T(""));
	ts << pMsg->nMoney;
	m_pStaticMoneyReward->SetText(ts.str().c_str());
	m_pStaticMoneyReward->SetRefresh(TRUE);
	ts.str(_T(""));
	ts << pMsg->nExploit;
	m_pStaticGuildReward->SetText(ts.str().c_str());
	m_pStaticGuildReward->SetRefresh(TRUE);

	m_pWnd->SetInvisible(FALSE);
	m_pWnd->FlipToTop();
	return 0;
}

void CityWarResultFrame::QueryCityWarRoleRecord()
{
	tagNC_CSRoleRecord msg;
	m_pSession->Send(&msg);
}

DWORD CityWarResultFrame::OnEventOpenCityWarResult(tagOpenCityWarResult* pEvent)
{
	if(P_VALID(m_pWnd))
	{
		if(m_pWnd->IsInvisible())
		{
			QueryCityWarRoleRecord();
		}
		else
		{
			m_pWnd->SetInvisible(TRUE);
		}
	}
	return 0;
}