#include "StdAfx.h"
#include "SpecialItemFrame.h"
#include "ItemEvent.h"
#include "..\WorldDefine\msg_spec_item.h"
#include "SocialData.h"
#include "SocialMgr.h"
#include "PlayerNameTab.h"
#include "ItemMgr.h"
#include "PetManager.h"
#include "PetDef.h"
#include "PetProtoData.h"
#include "CombatSysUtil.h"

SpecialItemFrame::SpecialItemFrame(void):
m_Trunk(this), 
m_pWnd_WomenFestval(NULL),
m_pBn_Ok_Set(NULL),
m_pBn_Cancel_Set(NULL),
m_pBn_Close_Set(NULL),
m_pEb_FriendName(NULL),
m_pCb_Friends_Box(NULL),
m_curRow(NULL),
m_n64NUM(NULL),
m_bIsPetFood(FALSE)
{
}

SpecialItemFrame::~SpecialItemFrame(void)
{
}
BOOL SpecialItemFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam );

	m_pMgr->RegisterEventHandle( _T("tagSpecItemEvent"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&SpecialItemFrame::On_WomenFestivalShow));
	m_pMgr->RegisterEventHandle(_T("Open_SpecFrame"), (FRAMEEVENTPROC)m_Trunk.sfp1(&SpecialItemFrame::OnEvent_OpenSpecFrame));
	m_pMgr->RegisterEventHandle(_T("Close_SpecFrame"), (FRAMEEVENTPROC)m_Trunk.sfp1(&SpecialItemFrame::OnEvent_CloseSpecFrame));
	//--注册网络消息处理函数
	m_pCmdMgr->Register("NS_SpecScriptItem",		(NETMSGPROC)m_Trunk.sfp2(&SpecialItemFrame::NS_SpecScriptItem),			_T("NS_SpecScriptItem"));

	//m_mapName2pet.clear();
	return bRet;
}

BOOL SpecialItemFrame::Destroy()
{
	m_pMgr->UnRegisterEventHandler( _T("tagSpecItemEvent"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&SpecialItemFrame::On_WomenFestivalShow));
	m_pMgr->UnRegisterEventHandler( _T("Open_SpecFrame"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&SpecialItemFrame::OnEvent_OpenSpecFrame));
	m_pMgr->UnRegisterEventHandler( _T("Close_SpecFrame"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&SpecialItemFrame::OnEvent_CloseSpecFrame));

	//--注销网络消息处理函数
	m_pCmdMgr->UnRegister("NS_SpecScriptItem",			(NETMSGPROC)m_Trunk.sfp2(&SpecialItemFrame::NS_SpecScriptItem));
	
	BOOL bRet = GameFrame::Destroy();
	
	SafeDestroyUI();
	
	return bRet;
}

BOOL SpecialItemFrame::ReloadUI()
{


	//加载38妇女节界面
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\task_female.xml");
	m_pWnd_WomenFestval = m_pGUI->CreateFromFile( "VFS_System", strPath.c_str());

	if( !P_VALID( m_pWnd_WomenFestval )  )
	{
		m_pGUI->DestroyWnd(m_pWnd_WomenFestval);
		m_pWnd_WomenFestval = NULL;		
		return FALSE;
	}

	m_pGUI->RegisterEventHandler( m_pWnd_WomenFestval->GetFullName().c_str(),   m_Trunk.sfp1(&SpecialItemFrame::OnWnd_WomenFestival));
	m_pBn_Ok_Set		= (GUIButton*)m_pWnd_WomenFestval->GetChild( _T("task_female_pic\\closebutt0") );
	m_pBn_Cancel_Set	= (GUIButton*)m_pWnd_WomenFestval->GetChild( _T("task_female_pic\\closebutt") );
	m_pBn_Close_Set		= (GUIButton*)m_pWnd_WomenFestval->GetChild( _T("task_female_pic\\closebutt00") );
	m_pEb_FriendName	= (GUIEditBox*)m_pWnd_WomenFestval->GetChild( _T("task_female_pic\\inputpic\\input") );
	m_pCb_Friends_Box	= (GUIComboBox*)m_pWnd_WomenFestval->GetChild( _T("task_female_pic\\comboback") );
	m_pStc_Title		= (GUIStatic*)m_pWnd_WomenFestval->GetChild( _T("task_female_pic\\word1"));

	m_pCb_Friends_Box->GetListBox()->SetColWidth(0, m_pCb_Friends_Box->GetSize().x);
	m_pWnd_WomenFestval->SetInvisible( TRUE );

	return TRUE;
}

BOOL SpecialItemFrame::On_WomenFestivalShow( tagSpecItemEvent* pEvent )
{
	if (EISF_PetFood == pEvent->eSpecFuncSrc )
	{
		m_n64NUM = pEvent->n64Serial;
		m_pStc_Title->SetText(g_StrTable[_T("SpecItem_PetFoodForTime")]);
		return TRUE;
	}
	if ( EISF_LoadScript != pEvent->eSpecFuncSrc  )
		return FALSE;
	m_n64NUM = pEvent->n64Serial;
	m_pStc_Title->SetText(g_StrTable[_T("SpecItem_WomenFestival")]);
	m_pWnd_WomenFestval->SetInvisible( FALSE );
	return TRUE;
}

BOOL SpecialItemFrame::OnWnd_WomenFestival( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd( pEvent->dwWndID );
	if( !P_VALID(pWnd) )
		return FALSE;
	if (m_bIsPetFood)
	{
		switch( pEvent->eEvent )
		{
		case EGUIE_Click:
			if( pWnd == m_pBn_Ok_Set )
			{
				tstring name = m_pEb_FriendName->GetText();
				if ( name.empty() )
					break;

				DWORD dwCurrPetID = 0;
				BOOL  bHaveUndeadPetSameName = false;
				const PetManager::_id2pet* pPetList = PetManager::Inst()->GetPetList();
				if (P_VALID(pPetList))
				{
					for (PetManager::_id2pet::const_iterator it = pPetList->begin();
						it != pPetList->end(); ++it)
					{
						if ( name == it->second->strName)
						{
							//防止Undead宠物使用食物
							const tagPetProto* pPetProto = PetProtoData::Inst()->FindPetProto(it->second->dwTypeID);
							if (!P_VALID(pPetProto))
							{
								m_pWnd_WomenFestval->SetInvisible( TRUE );
								return TRUE;
							}	
							if (-1 == pPetProto->nLifeTime)
							{
								bHaveUndeadPetSameName = true;
								continue;
							}
							dwCurrPetID = it->first;
						}
					}
					if ( bHaveUndeadPetSameName && 0 == dwCurrPetID)
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_PetLifeTime_NotTimeLimit")]);
						return TRUE;
					}
				}
				
				tagNC_AddPetLiveTime cmd;
				cmd.n64ItemID = m_n64NUM;
				cmd.dwPetID = dwCurrPetID;
				TObjRef<NetSession>()->Send(&cmd);

				//只有发送了才清空。方便玩家，避免重复输入。
				m_pEb_FriendName->SetText(_T(""));
				m_curRow = 0;
				m_bIsPetFood = FALSE;
				m_pWnd_WomenFestval->SetInvisible( TRUE );
			}
			if( pWnd == m_pBn_Cancel_Set )
			{
				m_bIsPetFood = FALSE;
				m_pWnd_WomenFestval->SetInvisible( TRUE );
			}
			if( pWnd == m_pBn_Close_Set )
			{
				m_bIsPetFood = FALSE;
				m_pWnd_WomenFestval->SetInvisible( TRUE );
			}
			if( pWnd == m_pCb_Friends_Box->GetButton() )
			{
				m_pGUI->SetActive( m_pCb_Friends_Box->GetListBox() );

				m_curRow = 0;
				m_pCb_Friends_Box->GetListBox()->Clear();
				m_vecPetArray.clear();

				//查看本地宠物列表
				const PetManager::_id2pet* pPetList = PetManager::Inst()->GetPetList();
				if (P_VALID(pPetList))
				{
					for (PetManager::_id2pet::const_iterator it = pPetList->begin();
						it != pPetList->end(); ++it)
					{
						const tagPetProto* pPetProto = PetProtoData::Inst()->FindPetProto(it->second->dwTypeID);
						if (P_VALID(pPetProto))
						{
							if (pPetProto->nLifeTime != -1)//说明是时限宠物
							{
								DWORD dwColor = GT_INVALID;	
								tstring name = it->second->strName;
								m_pCb_Friends_Box->GetListBox()->SetColWidth(0,m_pCb_Friends_Box->GetEditBox()->GetSize().x);
								m_pCb_Friends_Box->GetListBox()->SetText( m_curRow, 0,name.c_str() , dwColor );
								m_vecPetArray.push_back(name);
								m_curRow++;
							}
						}
					}
				}
				tagPoint ptSizeListBox = m_pCb_Friends_Box->GetListBox()->GetSize();
				ptSizeListBox.y = m_pCb_Friends_Box->GetListBox()->GetRowHeight() * m_curRow;
				ptSizeListBox.x = m_pCb_Friends_Box->GetEditBox()->GetSize().x;

				m_pCb_Friends_Box->GetListBox()->SetResizable( TRUE );
				m_pCb_Friends_Box->GetListBox()->Resize( ptSizeListBox );
				m_pCb_Friends_Box->GetListBox()->SetResizable( FALSE );
				m_pCb_Friends_Box->GetListBox()->SetRefresh(TRUE);

			}
			break;
		case EGUIE_ItemClick:
			if( pWnd == m_pCb_Friends_Box->GetListBox() )
			{
				if (m_vecPetArray.empty())
					break;
				if (m_vecPetArray.size() >= pEvent->dwParam1)
				{
					tstring petName = m_vecPetArray[pEvent->dwParam1];
					m_pEb_FriendName->SetText( petName.c_str() );
				}	
			}	
			break;
		default:
			break;
		}
	}
	//不是宠物食物
	else
	{
		switch( pEvent->eEvent )
		{
		case EGUIE_Click:
			if( pWnd == m_pBn_Ok_Set )
			{
				tstring name = m_pEb_FriendName->GetText();
				if ( name.empty() )
					break;

				tagNC_SpecScriptItem cmd;
				cmd.n64ItemID = m_n64NUM;
				ZeroMemory( cmd.szDesRoleName, sizeof(cmd.szDesRoleName) );
				_tcsncpy( cmd.szDesRoleName, name.c_str(), sizeof(cmd.szDesRoleName)/sizeof(TCHAR) );
				TObjRef<NetSession>()->Send(&cmd);

				//只有发送了才清空。方便玩家，避免重复输入。
				m_pEb_FriendName->SetText(_T(""));
				m_curRow = 0;
				m_pWnd_WomenFestval->SetInvisible( TRUE );
			}
			if( pWnd == m_pBn_Cancel_Set )
			{
				m_pWnd_WomenFestval->SetInvisible( TRUE );
			}
			if( pWnd == m_pBn_Close_Set )
			{
				m_pWnd_WomenFestval->SetInvisible( TRUE );

			}
			if( pWnd == m_pCb_Friends_Box->GetButton() )
			{
				m_pGUI->SetActive( m_pCb_Friends_Box->GetListBox() );

				m_curRow = 0;
				m_pCb_Friends_Box->GetListBox()->Clear();
				m_vecFriendsArray.clear();

				

				SocialMgr* pSocialMgr = (SocialMgr*)m_pMgr->GetFrame(_T("SocialMgr"));
				if( P_VALID(pSocialMgr) )
				{
					SocialData* pData = pSocialMgr->GetData();
					if( P_VALID(pData))
					{

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

									if( !friendInfo.bOnline )//不显示离线好友或者黑名单
										continue;

									DWORD dwColor = GT_INVALID;		//在线名单字体颜色为白色

									LPCTSTR szRoleName = PlayerNameTab::Inst()->FindNameByID( friendInfo.dwFriendID );
									if( _tcslen(szRoleName) > 0 )
										m_pCb_Friends_Box->GetListBox()->SetText( m_curRow, 0, szRoleName, dwColor );

									m_vecFriendsArray.push_back(szRoleName);
									m_curRow++;
								}
							}
						}
						tagPoint ptSizeListBox = m_pCb_Friends_Box->GetListBox()->GetSize();
						ptSizeListBox.y = m_pCb_Friends_Box->GetListBox()->GetRowHeight() * m_curRow;

						m_pCb_Friends_Box->GetListBox()->SetResizable( TRUE );
						m_pCb_Friends_Box->GetListBox()->Resize( ptSizeListBox );
						m_pCb_Friends_Box->GetListBox()->SetResizable( FALSE );
						m_pCb_Friends_Box->GetListBox()->SetRefresh(TRUE);

					}

				}
			}
			break;
		case EGUIE_Active:
			break;
		case EGUIE_ItemClick:
			if( pWnd == m_pCb_Friends_Box->GetListBox() )
			{
				if (m_vecFriendsArray.empty())
					break;
				if (m_vecFriendsArray.size() >= pEvent->dwParam1)
				{
					LPCTSTR RoleName = m_vecFriendsArray[pEvent->dwParam1];
					m_pEb_FriendName->SetText( RoleName );
				}	
			}	
			break;
		default:
			break;
		}
	}

	return TRUE;

}

DWORD SpecialItemFrame::NS_SpecScriptItem(tagNS_SpecScriptItem* pNetCmd, DWORD dwParam)
{
	if(pNetCmd->dwErrCode)
	{
		ItemMgr::Inst()->ShowSpecItemErrorMsg(pNetCmd->dwErrCode);
		return false;
	}
	return 0;
}

VOID SpecialItemFrame::SafeDestroyUI()
{
	if( P_VALID(m_pWnd_WomenFestval) )
	{
		m_pGUI->DestroyWnd( m_pWnd_WomenFestval );
		m_pWnd_WomenFestval = NULL;
	}
}

BOOL  SpecialItemFrame::OnEvent_OpenSpecFrame(tagGameEvent *pEvent)
{
	//如果通过此事件打开窗口，则表明是宠物食物
	m_bIsPetFood = TRUE;
	m_pWnd_WomenFestval->SetInvisible( FALSE );
	return 0;
}

BOOL  SpecialItemFrame::OnEvent_CloseSpecFrame(tagGameEvent *pEvent)
{
	m_pWnd_WomenFestval->SetInvisible( TRUE );
	return 0;
}

BOOL SpecialItemFrame::EscCancel()
{
	if (m_pWnd_WomenFestval->IsInvisible())
		return FALSE;
	m_pWnd_WomenFestval->SetInvisible( TRUE );
	return TRUE;
}