#include "StdAfx.h"
#include "RoleTitleFrame.h"
#include "CharacterFrame.h"
#include "RoleMgr.h"
#include "LocalPlayer.h"
#include "CombatSysUtil.h"
#include "RoleEvent.h"
#include "EffectMgr.h"

RoleTitleFrame::RoleTitleFrame(void) : m_Trunk(this),
	m_pWnd(NULL),
	m_pWndFather(NULL),
	m_un16ActivedTitleID(GT_INVALID)
{
}

RoleTitleFrame::~RoleTitleFrame(void)
{
}

BOOL RoleTitleFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam);

	//! 注册网络消息处理函数
    m_pCmdMgr->Register("NS_ActiveRoleTitle",    (NETMSGPROC)m_Trunk.sfp2(&RoleTitleFrame::OnNS_ActiveRoleTitle),   _T("NS_ActiveRoleTitle"));
	m_pCmdMgr->Register("NS_GetRoleTitles",	     (NETMSGPROC)m_Trunk.sfp2(&RoleTitleFrame::OnNS_GetRoleTitles),     _T("NS_GetRoleTitles"));
	m_pCmdMgr->Register("NS_NewTitles",          (NETMSGPROC)m_Trunk.sfp2(&RoleTitleFrame::OnNS_NewTitles),         _T("NS_NewTitles"));
    m_pCmdMgr->Register("NS_DelTitles",          (NETMSGPROC)m_Trunk.sfp2(&RoleTitleFrame::OnNS_DelTitles),         _T("NS_DelTitles"));
	m_pCmdMgr->Register("NS_OutOfDataTitle",     (NETMSGPROC)m_Trunk.sfp2(&RoleTitleFrame::OnNS_GetRoleOutOfDataTitles),         _T("NS_OutOfDataTitle"));
	m_pCmdMgr->Register("NS_TimeRemain",		 (NETMSGPROC)m_Trunk.sfp2(&RoleTitleFrame::OnNs_LimitTimeTitleTips),         _T("NS_TimeRemain"));
	m_pCmdMgr->Register("NS_TitleRemainTime",	 (NETMSGPROC)m_Trunk.sfp2(&RoleTitleFrame::OnNS_GetTimeLimitTitle),         _T("NS_TitleRemainTime"));

	m_pBtUse->SetEnable(false);
	m_pBtClear->SetEnable(true);

	return bRet;
}

BOOL RoleTitleFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();
	
	//! 注销网络消息处理函数
    m_pCmdMgr->UnRegister("NS_ActiveRoleTitle",   (NETMSGPROC)m_Trunk.sfp2(&RoleTitleFrame::OnNS_ActiveRoleTitle));
	m_pCmdMgr->UnRegister("NS_GetRoleTitles",     (NETMSGPROC)m_Trunk.sfp2(&RoleTitleFrame::OnNS_GetRoleTitles));
	m_pCmdMgr->UnRegister("NS_NewTitles",         (NETMSGPROC)m_Trunk.sfp2(&RoleTitleFrame::OnNS_NewTitles));
    m_pCmdMgr->UnRegister("NS_DelTitles",         (NETMSGPROC)m_Trunk.sfp2(&RoleTitleFrame::OnNS_DelTitles));
	m_pCmdMgr->UnRegister("NS_OutOfDataTitle",    (NETMSGPROC)m_Trunk.sfp2(&RoleTitleFrame::OnNS_GetRoleOutOfDataTitles));
	m_pCmdMgr->UnRegister("NS_TimeRemain",        (NETMSGPROC)m_Trunk.sfp2(&RoleTitleFrame::OnNs_LimitTimeTitleTips));
	m_pCmdMgr->UnRegister("NS_TitleRemainTime",	  (NETMSGPROC)m_Trunk.sfp2(&RoleTitleFrame::OnNS_GetTimeLimitTitle));


	m_pGUI->UnRegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&RoleTitleFrame::EventHandler));

	return bRet;
}

BOOL RoleTitleFrame::ReloadUI()
{
	//! 加载界面
	m_pWndFather			=	m_pGUI->GetWnd(((CharacterFrame*)m_pFather)->GetGUIFullName().c_str());
	m_pWnd					=	m_pWndFather->GetChild(_T("roletitle"));

	m_pTreeTitleList		=	(TreeEx*)m_pWnd->GetChild(_T("titlelistpic\\titlelist"));
	m_pScrBarTitleList		=	(GUIScrollBar*)m_pWnd->GetChild(_T("titlelistpic\\titlelist\\rt_scr"));
	m_pBtTitleListUp		=	(GUIButton*)m_pWnd->GetChild(_T("titlelistpic\\titlelist\\rt_scr\\up"));
	m_pBtTitleListDown		=	(GUIButton*)m_pWnd->GetChild(_T("titlelistpic\\titlelist\\rt_scr\\down"));

	m_pStcexDescription		=	(GUIStaticEx*)m_pWnd->GetChild(_T("titledescriptionback\\titledescripwin\\description"));
	m_pScrBarDescription	=	(GUIScrollBar*)m_pWnd->GetChild(_T("titledescriptionback\\titledescripwin\\description\\rt_scr"));
	m_pBtDescriptionUp		=	(GUIButton*)m_pWnd->GetChild(_T("titledescriptionback\\titledescripwin\\description\\rt_scr\\up"));
	m_pBtDescriptionDown	=	(GUIButton*)m_pWnd->GetChild(_T("titledescriptionback\\titledescripwin\\description\\rt_scr\\down"));

	m_pBtClear				=	(GUIButton*)m_pWnd->GetChild(_T("clearbutt"));
	m_pBtUse				=	(GUIButton*)m_pWnd->GetChild(_T("usebutt"));
    m_pPbtOption            =   (GUIPushButton*)m_pWnd->GetChild(_T("optbutt"));

	m_pStcPercentage		=	(GUIStatic*)m_pWnd->GetChild(_T("percentage"));

	//! 注册GUI事件处理函数
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&RoleTitleFrame::EventHandler));

	//! 初始化所有title列表
	InitTree();

	return TRUE;
}

VOID RoleTitleFrame::Update()
{
}

VOID RoleTitleFrame::UpdateRoleTitle()
{
	//! 取得角色已获得的所有称号
	tagNC_GetRoleTitles msg1;
	msg1.dwRoleID = RoleMgr::Inst()->GetLocalPlayerID();
	m_pSession->Send(&msg1);

}

DWORD RoleTitleFrame::OnGameEvent( tagGameEvent* pGameEvent )
{
	return 0;
}

DWORD RoleTitleFrame::EventHandler( tagGUIEvent* pEvent )
{
	tstring strb = ((CharacterFrame*)m_pFather)->GetGUIFullName();
	GUIWnd* aaa = m_pGUI->GetWnd(strb.c_str());
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

	switch (pEvent->eEvent)
	{
	case EGUIE_ItemClick:
		if (pWnd == m_pTreeTitleList)	//! 选中某一称号
		{
			SelectRoleTitle();
		}
		break;
	case EGUIE_Click:
		if (pWnd == m_pBtUse)			//! 使用
		{
			ActiveRoleTitle();
		}
		else if (pWnd == m_pBtClear)	//! 清除
		{
			ClearRoleTitle();
		}
	case EGUIE_PushButtonDown:
        if (pWnd == m_pPbtOption)  //! 显示未获得称号
        {
			UpdateRoleTitle();
        }
	default:
		break;
	}
	return 0;
}

VOID RoleTitleFrame::InitTree()
{
	//! 从数据中读取称号内容到tree控件中
	//! root
	m_pTreeTitleList->InsterItem(NULL, _T("root"), g_StrTable[_T("Title_Init_Root")]);

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
		ttl.dwItemID = m_pTreeTitleList->InsterItem(cBuff1, cBuff2, szTemp);
		ttl.dwTitleID = citer->second.m_dwTitleID; 
		m_mapOwnedTitles.insert(make_pair(citer->first, ttl));

		m_pTreeTitleList->SetItemColor(ttl.dwItemID, citer->second.m_dwColor);
	}

}

DWORD RoleTitleFrame::OnNS_GetRoleTitles( tagNS_GetRoleTitles* pMsg, DWORD dwParam )
{
	switch (pMsg->dwErrorCode)
	{
	case E_Title_Success:
		if (RoleMgr::Inst()->GetLocalPlayerID() == pMsg->dwRoleID)
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

            //! 获得正在使用的称号
            m_un16ActivedTitleID = (UINT16)RoleMgr::Inst()->GetLocalPlayer()->GetTitleID();

			//! 更新title列表
			UpdateTree();
			//! 刷新界面
			SelectRoleTitle();
		}
		break;
	}
	return 0;
}

DWORD RoleTitleFrame::OnNS_NewTitles( tagNS_NewTitles* pMsg, DWORD dwParam )
{
	if (RoleMgr::Inst()->GetLocalPlayerID() == pMsg->dwRoleID)
	{

		for (int i = 0, offset = 0;
			i < (int)pMsg->u16TitleNum;
			++i, offset += sizeof(UINT16))
		{
			UINT16 dwTitleID = 0;
			memcpy(&dwTitleID, pMsg->byData + offset, sizeof(UINT16));

			bool bHave = false;
			vector<DWORD>::iterator iterr = m_vecOutOfDataTitles.begin();
			for( iterr; iterr != m_vecOutOfDataTitles.end(); iterr++ )
			{
				if( *iterr == (DWORD)dwTitleID )
				{
					bHave = true;
					m_vecOutOfDataTitles.erase(iterr);
					break;
				}
			}
			
			map<DWORD, tagTitleList>::iterator iter =
				m_mapOwnedTitles.find((DWORD)dwTitleID);
			if (iter != m_mapOwnedTitles.end())
			{
				iter->second.bOwend = true;

                map<DWORD, tagTitleTypeActivedCount>::iterator itCount = m_mapActivedCount.find(iter->second.dwTypeID);
                if (itCount != m_mapActivedCount.end() && !bHave)
                {
                    itCount->second.bCount++;
                }
			}

			const tagRoleTitleProto* pTData = RoleTitleProtoData::Inst()->GetTitlebyID(dwTitleID);
			if (P_VALID(pTData))
			{
				tstring str = g_StrTable[_T("RoleTitle3")];
				str += pTData->m_szTitleName;
				CombatSysUtil::Inst()->ShowScreenCenterMsgEx(str.c_str());
                tagNewTitleDisp e(_T("tagNewTitleDisp"), this);
                e.strEventName = pTData->m_szTitleName;
                m_pFrameMgr->SendEvent(&e);
			}
		}
		m_un16ActivedTitleID = (UINT16)RoleMgr::Inst()->GetLocalPlayer()->GetTitleID();
		//! 更新title列表
		UpdateTree();

		EffectMgr::Inst()->PlayRoleEffect( RoleMgr::Inst()->GetLocalPlayerID(), _T("Txyy10") );
	}
	return 0;
}

VOID RoleTitleFrame::SelectRoleTitle()
{

	DWORD pSelItemID = m_pTreeTitleList->GetCurrentSelItem();
	tagGUITreeItem* pTreeItem = m_pTreeTitleList->GetItem(pSelItemID);
	if (P_VALID(pTreeItem))
	{
		UINT16 selTID = (UINT16)_tstoi(pTreeItem->strText.c_str());
		bool bOwend = m_mapOwnedTitles[(DWORD)selTID].bOwend;
		if (true == bOwend &&
			2 == pTreeItem->nLayer &&
			selTID != m_un16ActivedTitleID)
		{
			m_pBtUse->SetEnable(true);
			m_pBtClear->SetEnable(false);
			for( size_t i = 0; i < m_vecOutOfDataTitles.size(); i++ )
			{
				if( m_mapOwnedTitles[(DWORD)selTID].dwTitleID == m_vecOutOfDataTitles[i] )
				{
					m_pBtUse->SetEnable(false);
					m_pBtClear->SetEnable(false);
					break;
				}
			}
		}
		else if (true == bOwend &&
			2 == pTreeItem->nLayer &&
			selTID == m_un16ActivedTitleID)
		{
			m_pBtUse->SetEnable(false);
			m_pBtClear->SetEnable(true);
		}
		else
		{
			m_pBtUse->SetEnable(false);
			m_pBtClear->SetEnable(false);
		}
		
		if (/*true == bOwend &&*/ 2 == pTreeItem->nLayer)
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

		m_pBtUse->SetRefresh();
		m_pBtClear->SetRefresh();
	}


}

VOID RoleTitleFrame::ActiveRoleTitle()
{
	DWORD pSelItemID = m_pTreeTitleList->GetCurrentSelItem();
	tagGUITreeItem* pTreeItem = m_pTreeTitleList->GetItem(pSelItemID);
	if (P_VALID(pTreeItem))
	{
		tagNC_ActiveRoleTitle msg;
		msg.dwRoleID = RoleMgr::Inst()->GetLocalPlayerID();
		msg.u16TitleID = (UINT16)_tstoi(pTreeItem->strText.c_str());

		m_pSession->Send(&msg);
	}
	
}

VOID RoleTitleFrame::ClearRoleTitle()
{
	DWORD pSelItemID = m_pTreeTitleList->GetCurrentSelItem();
	tagGUITreeItem* pTreeItem = m_pTreeTitleList->GetItem(pSelItemID);
	if (P_VALID(pTreeItem))
	{
		tagNC_ActiveRoleTitle msg;
		msg.dwRoleID = RoleMgr::Inst()->GetLocalPlayerID();
		msg.u16TitleID = GT_INVALID;

		m_pSession->Send(&msg);
	}
}

VOID RoleTitleFrame::UpdateTree()
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
				DWORD dwLeftTime = GetLeftSeconds(citer->first);
				szTemp = tIter->second.m_szTitleName;
				if (m_un16ActivedTitleID == (UINT16)citer->first)
				{
					if( dwLeftTime == GT_INVALID )
					{
						strTemp = szTemp;
						strTemp += _T(" [");
						strTemp += g_StrTable[_T("RoleTitle0")];
						strTemp += _T("]");
					}
					else
					{
						strTemp = szTemp;
						strTemp += _T(" [");
						strTemp += g_StrTable[_T("RoleTitle5")];
						strTemp += GetLeftTime(dwLeftTime);
						strTemp += _T("]");
					}
					szTemp = strTemp.c_str();
					m_pTreeTitleList->SetItemColor( citer->second.dwItemID, tIter->second.m_dwColor );
				}
				else if( dwLeftTime != GT_INVALID )
				{
					strTemp = szTemp;
					strTemp += _T(" [");
					strTemp += g_StrTable[_T("RoleTitle6")];
					strTemp += GetLeftTime(dwLeftTime);
					strTemp += _T("]");
					szTemp = strTemp.c_str();
					m_pTreeTitleList->SetItemColor( citer->second.dwItemID, tIter->second.m_dwColor );
				}
				if( !m_vecOutOfDataTitles.empty() )
				{
					for( size_t i = 0; i < m_vecOutOfDataTitles.size(); i++ )
					{
						if( m_vecOutOfDataTitles[i] == tIter->second.m_dwTitleID )
						{
							strTemp = tIter->second.m_szTitleName;
							strTemp += _T(" [");
							strTemp += g_StrTable[_T("RoleTitle4")];
							strTemp += _T("]");
							szTemp = strTemp.c_str();
							m_pTreeTitleList->SetItemColor( citer->second.dwItemID, 0xFF808080 );
							break;
						}
					}
				}
				
				m_pTreeTitleList->SetItemVisable(citer->second.dwItemID, true);
			}
			else
			{
				if (m_pPbtOption->GetState() == EGUIBS_PushDown
					|| !tIter->second.m_bVisable)
					m_pTreeTitleList->SetItemVisable(citer->second.dwItemID, false);
				else
				{
					m_pTreeTitleList->SetItemVisable(citer->second.dwItemID, true);
				}
				szTemp = g_StrTable[_T("RoleTitle2")];
			}

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

DWORD RoleTitleFrame::OnNS_ActiveRoleTitle( tagNS_ActiveRoleTitle* pMsg, DWORD dwParam )
{
    switch (pMsg->dwErrorCode)
    {
    case E_Title_Success:
        if (RoleMgr::Inst()->GetLocalPlayerID() == pMsg->dwRoleID)
        {
            m_un16ActivedTitleID = pMsg->u16TitleID;

            //! 更新title列表
            UpdateTree();
            //! 刷新界面
            SelectRoleTitle();
        }

        break;
    }
	return 0;
}

DWORD RoleTitleFrame::OnNS_DelTitles( tagNS_DelTitles* pMsg, DWORD dwParam )
{
    if (RoleMgr::Inst()->GetLocalPlayerID() == pMsg->dwRoleID)
    {

        for (int i = 0, offset = 0;
            i < (int)pMsg->u16TitleNum;
            ++i, offset += sizeof(UINT16))
        {
            UINT16 dwTitleID = 0;
            memcpy(&dwTitleID, pMsg->byData + offset, sizeof(UINT16));

            map<DWORD, tagTitleList>::iterator iter =
                m_mapOwnedTitles.find((DWORD)dwTitleID);
            if (iter != m_mapOwnedTitles.end())
            {
                iter->second.bOwend = false;

                map<DWORD, tagTitleTypeActivedCount>::iterator itCount = m_mapActivedCount.find(iter->second.dwTypeID);
                if (itCount != m_mapActivedCount.end())
                {
                    itCount->second.bCount--;
                }

                LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
                if (P_VALID(pLp) && pLp->GetTitleID() == iter->second.dwTypeID)
                {
                    m_un16ActivedTitleID = GT_INVALID;
                    pLp->SetPlayerTitle(GT_INVALID);
                }
            }

            const tagRoleTitleProto* pTData = RoleTitleProtoData::Inst()->GetTitlebyID(dwTitleID);
            if (P_VALID(pTData))
            {
                tagDelTitleDisp e(_T("tagDelTitleDisp"), this);
                e.strEventName = pTData->m_szTitleName;
                m_pFrameMgr->SendEvent(&e);
            }
        }

        //! 更新title列表
        UpdateTree();
    }
    return 0;
}


DWORD RoleTitleFrame::OnNS_GetRoleOutOfDataTitles( tagNS_OutOfDataTitle* pMsg, DWORD dwParam )
{
	if (RoleMgr::Inst()->GetLocalPlayerID() == pMsg->dwRoleID)
	{
		for (int i = 0, offset = 0; i < (int)pMsg->u16TitleNum; ++i, offset += sizeof(UINT16))
		{
			UINT16 titleID = 0;
			memcpy(&titleID, pMsg->byData + offset, sizeof(UINT16));
			bool bFind = false;
			for( size_t i = 0; i < m_vecOutOfDataTitles.size(); i++ )
			{
				if( m_vecOutOfDataTitles[i] == (DWORD)titleID )
				{
					bFind = true;
					break;
				}
			}
			if( !bFind )
			{
				m_vecOutOfDataTitles.push_back(DWORD(titleID));
				LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
				if ( P_VALID(pLp) && pLp->GetTitleID() == titleID )
				{
					m_un16ActivedTitleID = GT_INVALID;
					pLp->SetPlayerTitle(GT_INVALID);
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("OutOfDataTitle")]);
				}
			}
		}
		m_un16ActivedTitleID = (UINT16)RoleMgr::Inst()->GetLocalPlayer()->GetTitleID();
		UpdateTree();
		SelectRoleTitle();
	}
	return 0;
}

DWORD RoleTitleFrame::OnNs_LimitTimeTitleTips( tagNS_TimeRemain *pMsg, DWORD dwParam )
{
	if( pMsg->dwTime )
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Title_LeftOneHour")]);
	else
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("Title_LeftOneDay")]);
	return 0;
}


tstring RoleTitleFrame::GetLeftTime( DWORD dwLeftTime )
{
	DWORD dwTime = dwLeftTime / 1000;
	if(dwTime > 365*24*60*60)// 大于1年
	{
		TCHAR szTmp[X_SHORT_NAME];
		_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), g_StrTable[_T("TitleLeft_Year")], dwTime/(365*24*60*60));
		return szTmp;
	}
	else if(dwTime > 30*24*60*60)// 大于1个月
	{
		TCHAR szTmp[X_SHORT_NAME];
		_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), g_StrTable[_T("TitleLeft_Month")], dwTime/(30*24*60*60));
		return szTmp;
	}
	else if(dwTime > 24*60*60)// 大于1天
	{
		TCHAR szTmp[X_SHORT_NAME];
		_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), g_StrTable[_T("TitleLeft_Day")], dwTime/(24*60*60));
		return szTmp;
	}
	else if(dwTime > 60*60)// 大于1小时
	{
		TCHAR szTmp[X_SHORT_NAME];
		_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), g_StrTable[_T("TitleLeft_Hour")], dwTime/(60*60));
		return szTmp;
	}
	else// 小于一小时
	{
		return g_StrTable[_T("TitleLeft_Min")];
	}
}

DWORD RoleTitleFrame::OnNS_GetTimeLimitTitle(tagNS_TitleRemainTime* pMsg, DWORD dwParam)
{
	if (RoleMgr::Inst()->GetLocalPlayerID() == pMsg->dwRoleID)
	{
		m_mapTimeLimitTitles.clear();
		for ( int i = 0; i < (int)pMsg->u16TitleNum; ++i )
			m_mapTimeLimitTitles.insert(make_pair(pMsg->byData[i].dwTitleID, pMsg->byData[i].dwRemainTime));
	}
	return 0;
}

DWORD RoleTitleFrame::GetLeftSeconds( DWORD dwTitleID )
{
	map<UINT16, DWORD>::const_iterator iter;
	iter = m_mapTimeLimitTitles.find((UINT16)dwTitleID);
	if( iter != m_mapTimeLimitTitles.end() )
		return iter->second;
	else
		return GT_INVALID;
}