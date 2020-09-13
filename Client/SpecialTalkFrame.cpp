#include "StdAfx.h"
#include "SpecialTalkFrame.h"
#include "QuestMgr.h"

SpecialTalkFrame::SpecialTalkFrame(void): m_Trunk(this), 
m_pWnd(0), 
m_pStcCondition(0),
m_pListOptions(0),
m_pBtnCloseWnd(0),
m_curRow(0),
m_pCaption(0)
{

}

SpecialTalkFrame::~SpecialTalkFrame(void)
{
}


//-----------------------------------------------------------------------------
// 初始化
//-----------------------------------------------------------------------------
BOOL SpecialTalkFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	return TRUE;
}

//-----------------------------------------------------------------------------
// 销毁
//-----------------------------------------------------------------------------
BOOL SpecialTalkFrame::Destroy()
{
	GameFrame::Destroy();

	if( P_VALID( m_pWnd ) )
		m_pGUI->AddToDestroyList( m_pWnd );
	
	m_pWnd = NULL;
	return TRUE;
}

//-----------------------------------------------------------------------------
// 重新读入窗口界面
//-----------------------------------------------------------------------------
BOOL SpecialTalkFrame::ReloadUI()
{
	XmlElement element;
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\npctalk.xml");
	if( !m_pGUI->LoadXml( &element, "VFS_System", strPath.c_str() ) )
		return FALSE;

	m_pWnd = m_pGUI->CreateWnd( _T("\\desktop"), &element );

	// 注册事件处理函数
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&SpecialTalkFrame::EventHandler) );

	m_pCaption		= (GUIPatch*)		m_pWnd->GetChild( _T("nt_pic") );
	m_pStcCondition	= (GUIStaticEx*)	m_pWnd->GetChild( _T("nt_pic\\nt_pic2\\npctalkpic\\contentWnd\\npctalk") );
	m_pListOptions	= (GUIListBox*)		m_pWnd->GetChild( _T("nt_pic\\nt_pic2\\optpic\\optlist") );
	m_pBtnCloseWnd	= (GUIButton*)		m_pWnd->GetChild( _T("nt_pic\\new0") );

	m_pListOptions->SetColNum( 1, 39 );
	m_pWnd->SetInvisible( TRUE );

	return TRUE;
}

//-----------------------------------------------------------------------------
// 界面消息事件处理
//-----------------------------------------------------------------------------
BOOL SpecialTalkFrame::EventHandler( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd( pEvent->dwWndID );

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		{
			//关闭窗口
			if( pWnd == m_pBtnCloseWnd )
			{
				m_pMgr->AddToDestroyList(this);
			}
		}
		break;

	case EGUIE_ItemClick:
		{
			if( pWnd == m_pListOptions )
			{
				if( m_curRow > pEvent->dwParam1 )
				{
					QuestScript *pQS = QuestMgr::Inst()->GetScript();
					if( !P_VALID(pQS) )
						return TRUE;

					const tagOptAttr& optAttr = m_optAttrs[pEvent->dwParam1];
					switch( optAttr.type )
					{
					case ESTOT_WalkWare:			//随身仓库，选择事件(背包扩展，仓库扩展)
						{
							pQS->OnScenarioTalk4WalkWare( optAttr.id, optAttr.step );
						}
						break;
					}
				}
			}
		}
		break;

	default:
		break;
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// 设置NPC对象名称
//-----------------------------------------------------------------------------
VOID SpecialTalkFrame::SetCaption( LPCTSTR szCaption )
{
	m_pCaption->SetText( szCaption );
}

//-----------------------------------------------------------------------------
// 设置对话内容
//-----------------------------------------------------------------------------
VOID SpecialTalkFrame::SetContent( LPCTSTR szContent )
{
	tstring strContent = szContent;
	vector<tagIDLink> idLinks;
	vector<tagPosLink> posLinks;
	QuestMgr::Inst()->ParseQuestView( strContent,idLinks,posLinks );
	m_pStcCondition->SetText( strContent.c_str() );

	tagPoint size = m_pStcCondition->GetSize();
	if( m_pStcCondition->GetTextTotalHeight() > size.y )
	{
		size.y = m_pStcCondition->GetTextTotalHeight();
		m_pStcCondition->SetResizable(TRUE);
		m_pStcCondition->Resize( size );
		m_pStcCondition->SetResizable( FALSE );
	}
}

//-----------------------------------------------------------------------------
// 清除所有对话选项
//-----------------------------------------------------------------------------
VOID SpecialTalkFrame::ClearOptions( void )
{
	if( P_VALID( m_pWnd ) )
	{
		m_pListOptions->Clear();
		m_pListOptions->SetCurSelectedRow( 0 );
		m_curRow = 0;
		m_optAttrs.clear();
	}
}

//-----------------------------------------------------------------------------
// 增加对话选项
//-----------------------------------------------------------------------------
VOID SpecialTalkFrame::AddTalkOption( LPCTSTR szOption, DWORD param1, DWORD param2, ESpecialTalkOptionType optionType )
{
	m_pListOptions->SetText( m_curRow++, 0, szOption );

	tagOptAttr optAttr;
	optAttr.id = param1;
	optAttr.step = param2;
	optAttr.type = optionType;
	m_optAttrs.push_back( optAttr );
}

//-----------------------------------------------------------------------------
// 显示对话窗口
//-----------------------------------------------------------------------------
VOID SpecialTalkFrame::Show()
{
	if( P_VALID( m_pWnd ) )
	{
		m_pWnd->SetInvisible(FALSE);
		m_pWnd->FlipToTop();
	}
}

BOOL SpecialTalkFrame::EscCancel()
{
	if( P_VALID( m_pWnd ) && !m_pWnd->IsInvisible() )
	{
		m_pMgr->AddToDestroyList( this );
		return TRUE;
	}

	return GameFrame::EscCancel();
}

