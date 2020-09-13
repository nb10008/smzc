#include "stdafx.h"
#include "MallGiftFrame.h"
#include "SocialData.h"
#include "SocialMgr.h"
#include "PlayerNameTab.h"
#include "FilterData.h"

MallGiftFrame::MallGiftFrame( void ):m_Trunk(this)
{
	m_pWnd					= NULL;
	m_pWndCaption			= NULL;	
	m_pBtnOK				= NULL;
	m_pBtnCancel			= NULL;
	m_pEdtName				= NULL;
	m_pLBNameList			= NULL;
	m_pEdtNum				= NULL;
	m_pEditWord				= NULL;
	//m_curRow				= NULL;
}

MallGiftFrame::~MallGiftFrame( void )
{

}

BOOL MallGiftFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam );
	
	// 注册游戏事件
	m_pFrameMgr->RegisterEventHandle(_T("tagRefreshFriendsList"), (FRAMEEVENTPROC)m_Trunk.sfp1(&MallGiftFrame::OnRefreshFriendsList));
	m_pFrameMgr->RegisterEventHandle( _T("Close_Gift_UI"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&MallGiftFrame::OnClose_UIEvent));

	return bRet;
}

BOOL MallGiftFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();

	// 注销游戏事件
	m_pFrameMgr->UnRegisterEventHandler( _T("tagRefreshFriendsList"), (FRAMEEVENTPROC)m_Trunk.sfp1(&MallGiftFrame::OnRefreshFriendsList));
	m_pFrameMgr->UnRegisterEventHandler( _T("Close_Gift_UI"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&MallGiftFrame::OnClose_UIEvent));

	if(P_VALID(GetObj("Send_Gift_Words_Too_Long")))
		KillObj("Send_Gift_Words_Too_Long");

	if(P_VALID(m_pWnd))
	{
		m_pGUI->AddToDestroyList(m_pWnd);
		m_pWnd = NULL;
	}
	m_vecFriendsArray.clear();
	return bRet;
}

BOOL MallGiftFrame::ReloadUI()
{
	XmlElement ele;
	tstring strPath = g_strLocalPath + _T("\\ui\\mall_gift.xml");
	m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str());

	m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &ele);

	m_pWndCaption	= m_pWnd->GetChild(_T("gift_pic"));
	m_pBtnOK		= (GUIButton*)m_pWndCaption->GetChild(_T("surebutt"));
	m_pBtnCancel	= (GUIButton*)m_pWndCaption->GetChild(_T("canclebutt"));
	m_pEdtName		= (GUIEditBox*)m_pWndCaption->GetChild(_T("putinpic\\putin"));
	m_pLBNameList	= (GUIListBox*)m_pWndCaption->GetChild(_T("listback1\\friends_list"));
	m_pLBNameList->SetColWidth(0,131);
	//m_pCbNameBox	= (GUIComboBox*)m_pWndCaption->GetChild( _T("comboback") );
	//m_pCbNameBox->GetListBox()->SetColWidth(0, m_pCbNameBox->GetSize().x);
	m_pEdtNum		= (GUIEditBox*)m_pWndCaption->GetChild(_T("putinpic0\\putin"));
	m_pEditWord		= (GUIEditBox*)m_pWndCaption->GetChild(_T("putinpic2\\putin"));

	// 注册事件处理函数
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&MallGiftFrame::EventHandler));
	
	UpdateNameList();

	return TRUE;
}

DWORD MallGiftFrame::EventHandler( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	if( !P_VALID(pWnd) )
		return 0;
	switch (pEvent->eEvent)
	{
	case EGUIE_Click:
		{
			tagMsgMallGiftEvent event(_T("tagMsgMallGiftEvent"), NULL);
			if( pWnd == m_pBtnOK )
			{
				event.eRet = MGF_OK;
				event.strName = m_pEdtName->GetText();
				TCHAR szBuff[X_SHORT_NAME] = {0};
				_tcsncpy( szBuff, m_pEdtNum->GetText(), sizeof(szBuff)/sizeof(TCHAR) );
				event.n16Num = _ttoi(szBuff);
				event.strWord = m_pEditWord->GetText();
				event.dwTime = timeGetTime();

				tstring sendMsg = m_pEditWord->GetText();
				FilterData::Inst()->WordsFilteror(sendMsg);    //屏蔽字
				DeleteRETURNS(sendMsg);
				INT len = sendMsg.size();//_tcslen(sendMsg.c_str());
				if ( len > 20 )
				{
					if(P_VALID(GetObj("Send_Gift_Words_Too_Long")))
						KillObj("Send_Gift_Words_Too_Long");
					CreateObj("Send_Gift_Words_Too_Long", "MsgBoxEx");
					TObjRef<MsgBoxEx>("Send_Gift_Words_Too_Long")->Init( 
						_T(""), g_StrTable[_T("Send_Gift_Words_Too_Long")],       
						_T("Send_Gift_Words_Too_Long") );
				}
				else
				{
					m_pFrameMgr->SendEvent(&event);		
					ClearUIData();
				}
			}
			else if( pWnd == m_pBtnCancel )
			{
				event.eRet = MGF_Cancel;
				m_pFrameMgr->SendEvent(&event);
				ClearUIData();
			}
			//else if( pWnd == m_pCbNameBox->GetButton() )
			//{
			//	m_pGUI->SetActive( m_pCbNameBox->GetListBox() );
			//	m_curRow = 0;
			//	m_pCbNameBox->GetListBox()->Clear();
			//	UpdateNameList();
			//	std::vector<LPCTSTR>::iterator  iterFriends = m_vecFriendsArray.begin();
			//	for ( ; iterFriends != m_vecFriendsArray.end(); iterFriends++ )
			//	{
			//		LPCTSTR szRoleName = (*iterFriends);
			//		DWORD dwColor = GT_INVALID;		//在线名单字体颜色为白色
			//		if( _tcslen(szRoleName) > 0 )
			//			m_pCbNameBox->GetListBox()->SetText( m_curRow, 0, szRoleName, dwColor );
			//		m_curRow++;
			//	}

			//	tagPoint ptSizeListBox = m_pCbNameBox->GetListBox()->GetSize();
			//	ptSizeListBox.y = m_pCbNameBox->GetListBox()->GetRowHeight() * m_curRow;
			//	m_pCbNameBox->GetListBox()->SetResizable( TRUE );
			//	m_pCbNameBox->GetListBox()->Resize( ptSizeListBox );
			//	m_pCbNameBox->GetListBox()->SetResizable( FALSE );
			//	m_pCbNameBox->GetListBox()->SetRefresh(TRUE);
			//}
		}
		break;
	case EGUIE_ItemClick:
		/*if( pWnd == m_pCbNameBox->GetListBox() )
		{
			UpdateNameList();
			if (m_vecFriendsArray.empty())
				break;
			if (m_vecFriendsArray.size() >= pEvent->dwParam1)
			{
				LPCTSTR RoleName = m_vecFriendsArray[pEvent->dwParam1];
				m_pEdtName->SetText( RoleName );
			}	
			m_pGUI->SetActive(m_pWnd);
		}	
		break;*/
		if( pWnd == m_pLBNameList )
		{
			DWORD curRow = m_vecFriendsArray.size();
			if( curRow > pEvent->dwParam1 )
			{
				if ( m_pLBNameList->GetText(pEvent->dwParam1,0) != (LPCTSTR)GT_INVALID )
				{
					tstring tsName = m_pLBNameList->GetText(pEvent->dwParam1,0);
					if ( !tsName.empty() )
						m_pEdtName->SetText( tsName.c_str() );
				}
			}
		}
		break;
	default:
		break;
	}
	return 0;
}

VOID MallGiftFrame::ClearUIData()
{
	if( P_VALID(m_pWnd) )
		m_pWnd->SetInvisible(TRUE);
	if( P_VALID(m_pEdtName) )
		m_pEdtName->SetText(_T(""));
	if( P_VALID(m_pEdtNum) )
		m_pEdtNum->SetText(_T("1"));
	if( P_VALID(m_pEditWord) )
		m_pEditWord->SetText(_T(""));

	if(P_VALID(GetObj("Send_Gift_Words_Too_Long")))
		KillObj("Send_Gift_Words_Too_Long");
	//m_curRow = 0;
}

BOOL MallGiftFrame::UpdateListData()
{
	//数据更新
	m_vecFriendsArray.clear();
	SocialMgr* pSocialMgr = (SocialMgr*)m_pFrameMgr->GetFrame(_T("SocialMgr"));
	if( !P_VALID(pSocialMgr) )
		return FALSE;
	SocialData* pData = pSocialMgr->GetData();
	if( !P_VALID(pData))
	    return FALSE;
	const SocialData::FriendsArray *friends;
	SocialData::EListType listType;
	for ( INT FriendType = 1; FriendType <= 4 ; FriendType++)
	{
		switch( FriendType )
		{
		case 1:
			listType = SocialData::ELT_Friend1;
			break;
		case 2:
			listType = SocialData::ELT_Friend2;
			break;
		case 3:
			listType = SocialData::ELT_Friend3;
			break;
		case 4:
			listType = SocialData::ELT_Friend4;
			break;
		default:
			break;
		}

		if( pData->GetFriendList( listType, &friends ) )
		{
			SocialData::FriendsArray::const_iterator iteri;
			for( iteri=friends->begin(); iteri!=friends->end(); ++iteri )
			{
				const tagFriendInfo& friendInfo = *iteri;

				LPCTSTR szRoleName = PlayerNameTab::Inst()->FindNameByID( friendInfo.dwFriendID );
				m_vecFriendsArray.push_back(szRoleName);
			}
		}
	}	
	return TRUE;
}

BOOL MallGiftFrame::UpdateListFrame()
{
	//页面更新
	if (!P_VALID(m_pWnd))
		return FALSE;
	if (m_pWnd->IsInvisible())
		return FALSE;
			
	INT curRow = 0;
	m_pLBNameList->Clear();
	std::vector<LPCTSTR>::iterator  iterFriends = m_vecFriendsArray.begin();
	for ( ; iterFriends != m_vecFriendsArray.end(); iterFriends++ )
	{
		LPCTSTR szRoleName = (*iterFriends);
		DWORD dwColor = GT_INVALID;		//在线名单字体颜色为白色
		if( _tcslen(szRoleName) > 0 )
		{
			m_pLBNameList->SetTextColor(0xFFFFFFFF);
			m_pLBNameList->SetText( curRow, 0, szRoleName );
			curRow++;
		}
	}

	tagPoint ptSizeOldListBox = m_pLBNameList->GetSize();
	tagPoint ptSizeListBox = ptSizeOldListBox;
	ptSizeListBox.y = m_pLBNameList->GetRowHeight() * curRow;
	m_pLBNameList->SetResizable( TRUE );
	m_pLBNameList->Resize( ptSizeListBox );
	m_pLBNameList->SetResizable( FALSE );
	m_pLBNameList->SetRefresh(TRUE);
	return TRUE;		
	
}

VOID MallGiftFrame::UpdateNameList()
{
	BOOL bNeedRefresh = UpdateListData();
	if (bNeedRefresh)
	{
		UpdateListFrame();
	}
}

void MallGiftFrame::SetBuyNum()
{
	if( P_VALID(m_pEdtNum) )
	{
		if (!m_pWnd->IsInvisible())
			m_pEdtNum->SetText(_T("1"));
	}		
}

void MallGiftFrame::SetMaxBuyNum(INT nMaxNum)
{
	if( P_VALID(m_pEdtNum) )
		m_pEdtNum->SetMaxValue(nMaxNum);
}

void MallGiftFrame::ShowGiftFrame(BOOL bShow)
{
	if( P_VALID(m_pWnd) )
	{
		m_pWnd->SetInvisible(!bShow);
		if (bShow)
		{
			UpdateNameList();
			SetBuyNum();
			m_pWnd->FlipToTop();
		}
	}
}

BOOL MallGiftFrame::EscCancel()
{
	if( P_VALID(m_pWnd) && m_pWnd->IsInvisible() )
		return FALSE;

	tagMsgMallGiftEvent event(_T("tagMsgMallGiftEvent"), NULL);
	event.eRet = MGF_Cancel;
	m_pFrameMgr->SendEvent(&event);

	ClearUIData();

	return TRUE;
}

DWORD MallGiftFrame::OnRefreshFriendsList(tagRefreshFriendsList* pEvent)
{
	UpdateNameList();
	return 0;
}

DWORD MallGiftFrame::OnClose_UIEvent(tagGameEvent* pEvent)
{
	ShowGiftFrame(FALSE);
	return 0;
}

//删除richEditBox为了进行排版在文字（strText）里自动加上的回车键
VOID MallGiftFrame::DeleteRETURNS(tstring& strText)
{
	tstring::iterator new_end;
	new_end = remove(strText.begin(), strText.end(), (TCHAR)0x0d);
	strText.erase(new_end, strText.end());
	new_end = remove(strText.begin(), strText.end(), (TCHAR)0x0a);
	strText.erase(new_end, strText.end());
}