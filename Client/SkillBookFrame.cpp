#include "stdafx.h"
#include "SkillBookFrame.h"
#include "TalentFrame.h"
#include "SkillMgr.h"
#include "DragBtn.h"
#include "ToolTipFrame.h"
#include "ToolTipCreator.h"
#include "SkillEvent.h"
#include "CombatSysUtil.h"

SkillBookFrame::SkillBookFrame():m_Trunk(this)
{
	m_pWnd = NULL;
	m_pStcPage = NULL;
	m_pBtnPageUp = NULL;
	m_pBtnPageDown = NULL;
	m_nCurPage = 0;
	m_eCurSkillType = ECDST_Active;
}

SkillBookFrame::~SkillBookFrame()
{

}

BOOL SkillBookFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam);
	
	m_pFrameMgr->RegisterEventHandle(_T("tagUpdateLPSkillEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&SkillBookFrame::OnUpdateLPSkillEvent));
	m_pFrameMgr->RegisterEventHandle(_T("Weapon_Swap"),			  (FRAMEEVENTPROC)m_Trunk.sfp1(&SkillBookFrame::OnWeaponSwap));
	
	return bRet;
}

BOOL SkillBookFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();

	m_pFrameMgr->UnRegisterEventHandler(_T("tagUpdateLPSkillEvent"), (FRAMEEVENTPROC)m_Trunk.sfp1(&SkillBookFrame::OnUpdateLPSkillEvent));
	m_pFrameMgr->UnRegisterEventHandler(_T("Weapon_Swap"),			 (FRAMEEVENTPROC)m_Trunk.sfp1(&SkillBookFrame::OnWeaponSwap));

	return bRet;
}

BOOL SkillBookFrame::ReloadUI()
{
	m_pWnd = ((TalentFrame*)m_pFather)->GetCaption();
	// 注册事件处理函数
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&SkillBookFrame::EventHandler));
	
	m_activesSkill = (GUIPushButton*)m_pWnd->GetChild(_T("winopt\\optbutt0"));
	m_passiveSkill = (GUIPushButton*)m_pWnd->GetChild(_T("winopt\\optbutt1"));
	m_produceSkill = (GUIPushButton*)m_pWnd->GetChild( _T("winopt\\optbutt2") );
	m_GuildSkill   = (GUIPushButton*)m_pWnd->GetChild( _T("winopt\\optbutt3") );
	m_GuildGodArea   = (GUIPushButton*)m_pWnd->GetChild( _T("winopt\\optbutt4") );

	m_pWnd = ((TalentFrame*)m_pFather)->GetSkillBook();


	m_pStcPage = (GUIStatic*)m_pWnd->GetChild(_T("page\\pagenum"));
	m_pBtnPageUp = (GUIButton*)m_pWnd->GetChild(_T("page\\butt2"));
	m_pBtnPageDown = (GUIButton*)m_pWnd->GetChild(_T("page\\butt1"));

	m_pProduceNotice = (GUIStaticEx*)m_pWnd->GetChild(_T("word"));
	if(P_VALID(m_pProduceNotice))m_pProduceNotice->SetInvisible(TRUE);//初始隐藏
	
	for (INT i=0; i<MAX_SKILL_BOOK_NUM; ++i)
	{
		m_SkillItem[i].ReloadUI(m_pWnd, i);
		m_SkillItem[i].SetInvisible(TRUE);
	}
	
	//--技能列表初始化
	m_skillList.Clear();
	//m_skillIter = m_skillList.Begin();

	//--被动技能列表初始化
	m_PassiveSkillList.Clear();
	//m_PassiveSkillIter = m_PassiveSkillList.Begin();

	//生产技能列表初始化
	m_lstProduceSkill.Clear();
	//m_iterProduceSkill = m_lstProduceSkill.Begin();

	m_lstGuildSkill.Clear();
	m_lstGodSkill.Clear();

	SetCurPage(0);
	
	return TRUE;
}

DWORD SkillBookFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	switch (pEvent->eEvent)
	{
	case EGUIE_Click:
		{
			if(pWnd == m_pBtnPageDown)
			{
				OnPageUp(FALSE);
			}
			else if(pWnd == m_pBtnPageUp)
			{
				OnPageUp(TRUE);
			}
			else if(_tcsnccmp(pWnd->GetName().c_str(), _T("skbutt"), 6) == 0)
			{

				for(INT i=0; i<MAX_SKILL_BOOK_NUM; ++i)
				{
					if(pWnd == m_SkillItem[i].GetSkillWnd())
					{
						SkillMgr::Inst()->OnUseSkill(m_SkillItem[i].GetSkillID());
						break;
					}
				}
			}
		}
		break;
	case EGUIE_PushButtonDown:
		{
			if( pWnd == m_activesSkill )
			{
				m_nCurPage = 0;
				SetCurPage( m_nCurPage );
				m_eCurSkillType = ECDST_Active;
				ReflashCurPage();
			}
			else if( pWnd == m_passiveSkill )
			{
				m_nCurPage = 0;
				SetCurPage( m_nCurPage );
				m_eCurSkillType = ECDST_Passive;
				ReflashCurPage();
			}
			else if( pWnd == m_produceSkill )
			{
				m_nCurPage = 0;
				SetCurPage( m_nCurPage );
				m_eCurSkillType = ECDST_Produce;
				ReflashCurPage();
			}
			else if( pWnd == m_GuildSkill )
			{
				m_nCurPage = 0;
				SetCurPage( m_nCurPage );
				m_eCurSkillType = ECDST_Guild;
				ReflashCurPage();
			}
			else if( pWnd == m_GuildGodArea )
			{
				m_nCurPage = 0;
				SetCurPage( m_nCurPage );
				m_eCurSkillType = ECDST_GodArea;
				ReflashCurPage();
			}
		}
		break;
	case EGUIE_Drag:
		{
			if(_tcsnccmp(pWnd->GetName().c_str(), _T("skbutt"), 6) == 0)
			{
				for(INT i=0; i<MAX_SKILL_BOOK_NUM; ++i)
				{
					//TODO 也可以根据技能ID,查看技能的适用类型，来判断能否拖动
					if( pWnd == m_SkillItem[i].GetSkillWnd() && (m_eCurSkillType == ECDST_Active || m_eCurSkillType == ECDST_Produce || m_eCurSkillType == ECDST_Guild || m_eCurSkillType == ECDST_GodArea) )
					{
						const tagSkillData* data = SkillMgr::Inst()->GetSkillDataByTypeID(m_SkillItem[i].GetSkillID());
						if (!P_VALID(data))
							break;

						if(data->pProto->eUseType == ESUT_Passive)
						{
							CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("SkillBook_CantDargPassiveSkill")]);
							break;
						}
							
						if(data->pProto->nType2 == ESSTE_Transform)
						{
							CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("SkillBook_CantDargTransSkill")]);
							break;
						}

						DragBtn* pDrag = (DragBtn*)m_pGUI->GetWnd(_T("\\desktop\\DragButton"));
						if (P_VALID(pDrag))
						{
							pEvent->dwParam1 = EDT_Skill;
							pEvent->dwParam2 = m_SkillItem[i].GetSkillID();
							pDrag->OnEvent(pEvent);
							// 需要隐藏Tip
							ShowTip(pWnd, NULL);
						}
						break;
					}
				}
			}
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			if(_tcsnccmp(pWnd->GetName().c_str(), _T("skbutt"), 6) == 0)
			{

				for(INT i=0; i<MAX_SKILL_BOOK_NUM; ++i)
				{
					if(pWnd == m_SkillItem[i].GetSkillWnd())
					{
						DWORD dwID = m_SkillItem[i].GetSkillID();
						ShowTip(pWnd, ToolTipCreator::Inst()->GetSkillTips(dwID).c_str());
						return 0;
					}
				}
			}

			if(_tcsnccmp(pWnd->GetName().c_str(), _T("pro"), 3) == 0)
			{
				for(INT i=0; i<MAX_SKILL_BOOK_NUM; ++i)
				{
					if(pWnd == m_SkillItem[i].GetProcessWnd())
					{
						tstring strTemp = m_SkillItem[i].GetExp();
						ShowTip( pWnd, strTemp.c_str() );
						return 0;
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
	default :
		break;
	}
	return 0;
}


VOID SkillBookFrame::OnSkillBookOpen()
{
	if( m_skillList.Size() <= 0 )
	{
		SkillMgr::Inst()->GetSkillList( m_skillList );
	}
	
	if( m_lstProduceSkill.Size() <= 0 )
	{
		SkillMgr::Inst()->GetProduceSkillList( m_lstProduceSkill );
	}

	if( m_PassiveSkillList.Size() <=0 )
	{
		SkillMgr::Inst()->GetPassiveSkillList( m_PassiveSkillList, m_lstOtherPassiveSkill );
	}

	if (m_lstGuildSkill.Size() <= 0)
	{
		SkillMgr::Inst()->GetGuildSkillList( m_lstGuildSkill );
	}
	if (m_lstGodSkill.Size() <= 0)
	{
		SkillMgr::Inst()->GetGodSkillList( m_lstGodSkill );
	}
	ReflashCurPage();
}

DWORD SkillBookFrame::OnUpdateLPSkillEvent( tagUpdateLPSkillEvent* pGameEvent )
{
	//--只更新主动技能
	if( ESUT_Active == pGameEvent->eType )
	{
		//if(pGameEvent->bAdd)//
		//{
		//	if(IsNormalAttackSkill(pGameEvent->dwID) && m_skillList.Size() > 0)
		//		return;
		//	
		//	m_skillList.PushBack(pGameEvent->dwID);
		//	//--如果界面可见且添加的技能为当前页，则刷新技能栏
		//	if(!m_pWnd->IsInvisible() && IsCurPage(m_skillList.Size()-1))
		//	{
		//		m_SkillItem[(m_skillList.Size()-1) % MAX_SKILL_BOOK_NUM].UpdateSkill(pGameEvent->dwID);
		//	}

		//	// 刷新按钮
		//	m_pBtnPageUp->SetEnable(m_nCurPage < m_skillList.Size()/MAX_SKILL_BOOK_NUM);
		//	m_pBtnPageDown->SetEnable(m_nCurPage > 0);
		//}
		//else
		//{
		SkillMgr::Inst()->GetSkillList( m_skillList );
		SkillMgr::Inst()->GetProduceSkillList( m_lstProduceSkill );
		SkillMgr::Inst()->GetGuildSkillList( m_lstGuildSkill );
		SkillMgr::Inst()->GetGodSkillList( m_lstGodSkill );
		/*}*/
	}
	else if( ESUT_Passive == pGameEvent->eType )             //获取被动技能列表
	{
		SkillMgr::Inst()->GetPassiveSkillList( m_PassiveSkillList, m_lstOtherPassiveSkill );  
		SkillMgr::Inst()->GetGuildSkillList( m_lstGuildSkill );
		SkillMgr::Inst()->GetGodSkillList( m_lstGodSkill );
	}

	ReflashCurPage();
	return 0;
}

VOID SkillBookFrame::ReflashCurPage()
{
	CheckCurPage();
	switch( m_eCurSkillType )
	{
	case ECDST_Active:
		{
			// 如果技能列表数量小于当前技能页的最小值，则直接显示第一页的技能
			if( m_skillList.Size() <= MAX_SKILL_BOOK_NUM*m_nCurPage )
			{
				SetCurPage( 0 );
			}
			
			INT nIndex = 0;
			INT	nCount = 0;
			DWORD dwID;
			m_skillList.ResetIterator();
			while( m_skillList.PeekNext( dwID ) )
			{
				if( !IsCurPage(nIndex) )
				{
					++nIndex;
					continue;
				}
				
				if( nCount >= MAX_SKILL_BOOK_NUM )
					break;

				m_SkillItem[nCount].UpdateSkill( dwID );
				++nIndex;
				++nCount;
			}

			// 如果没有到最后一个格，剩余格不显示
			for( ; nCount<MAX_SKILL_BOOK_NUM; ++nCount )
			{
				m_SkillItem[nCount].UpdateSkill( GT_INVALID );
			}

			// 刷新按钮
			m_pBtnPageUp->SetEnable( m_nCurPage < m_skillList.Size()/MAX_SKILL_BOOK_NUM );
			m_pBtnPageDown->SetEnable( m_nCurPage > 0 );
			if(P_VALID(m_pProduceNotice))m_pProduceNotice->SetInvisible(TRUE);
		}
		break;
	case ECDST_Passive:
		{
			// 如果技能列表数量小于当前技能页的最小值，则直接显示第一页的技能
			if( m_PassiveSkillList.Size() <= MAX_SKILL_BOOK_NUM*m_nCurPage )
			{
				SetCurPage(0);
			}
			
			INT nIndex = 0;
			INT	nCount = 0;
			DWORD dwID;
			m_PassiveSkillList.ResetIterator();
			while( m_PassiveSkillList.PeekNext( dwID ) )
			{
				if( !IsCurPage(nIndex) )
				{
					++nIndex;
					continue;
				}

				if( nCount >= MAX_SKILL_BOOK_NUM )
					break;

				m_SkillItem[nCount].UpdateSkill( dwID );
				++nIndex;
				++nCount;
			}

			// 如果没有到最后一个格，剩余格不显示
			for( ; nCount<MAX_SKILL_BOOK_NUM; ++nCount )
			{
				m_SkillItem[nCount].UpdateSkill( GT_INVALID );
			}

			// 刷新按钮
			m_pBtnPageUp->SetEnable( m_nCurPage < m_PassiveSkillList.Size()/MAX_SKILL_BOOK_NUM );
			m_pBtnPageDown->SetEnable( m_nCurPage > 0 );
			if(P_VALID(m_pProduceNotice))m_pProduceNotice->SetInvisible(TRUE);
		}	
		break;
	case ECDST_Produce:
		{
			// 如果技能列表数量小于当前技能页的最小值，则直接显示第一页的技能
			if( m_lstProduceSkill.Size() <= MAX_SKILL_BOOK_NUM*m_nCurPage )
			{
				SetCurPage( 0 );
			}

			INT nIndex = 0;
			INT	nCount = 0;
			DWORD dwID;
			m_lstProduceSkill.ResetIterator();
			while( m_lstProduceSkill.PeekNext( dwID ) )
			{
				if( !IsCurPage(nIndex) )
				{
					++nIndex;
					continue;
				}

				if( nCount >= MAX_SKILL_BOOK_NUM )
					break;

				m_SkillItem[nCount].UpdateSkill( dwID );
				++nIndex;
				++nCount;
			}

			// 如果没有到最后一个格，剩余格不显示
			for( ; nCount < MAX_SKILL_BOOK_NUM; ++nCount )
			{
				m_SkillItem[nCount].UpdateSkill( GT_INVALID );
			}

			// 刷新按钮
			m_pBtnPageUp->SetEnable( m_nCurPage < m_lstProduceSkill.Size()/MAX_SKILL_BOOK_NUM );
			m_pBtnPageDown->SetEnable( m_nCurPage > 0 );			
			if(m_lstProduceSkill.Empty() && P_VALID(m_pProduceNotice))
			{
				m_pProduceNotice->SetText(g_StrTable[_T("noProduceSkill")]);
				m_pProduceNotice->SetInvisible(FALSE);
				m_pWnd->SetRefresh(TRUE);
			}
			else
			{
				if(P_VALID(m_pProduceNotice)) m_pProduceNotice->SetInvisible(TRUE);
				m_pWnd->SetRefresh(TRUE);
			}
		}
		break;
	case ECDST_Guild:
		{
			// 如果技能列表数量小于当前技能页的最小值，则直接显示第一页的技能
			if( m_lstGuildSkill.Size() <= MAX_SKILL_BOOK_NUM*m_nCurPage )
			{
				SetCurPage( 0 );
			}

			INT nIndex = 0;
			INT	nCount = 0;
			DWORD dwID;
			m_lstGuildSkill.ResetIterator();
			while( m_lstGuildSkill.PeekNext( dwID ) )
			{
				if( !IsCurPage(nIndex) )
				{
					++nIndex;
					continue;
				}

				if( nCount >= MAX_SKILL_BOOK_NUM )
					break;

				m_SkillItem[nCount].UpdateSkill( dwID );
				++nIndex;
				++nCount;
			}

			// 如果没有到最后一个格，剩余格不显示
			for( ; nCount<MAX_SKILL_BOOK_NUM; ++nCount )
			{
				m_SkillItem[nCount].UpdateSkill( GT_INVALID );
			}

			// 刷新按钮
			m_pBtnPageUp->SetEnable( m_nCurPage < m_lstGuildSkill.Size()/MAX_SKILL_BOOK_NUM );
			m_pBtnPageDown->SetEnable( m_nCurPage > 0 );
			if(P_VALID(m_pProduceNotice))m_pProduceNotice->SetInvisible(TRUE);
		}
		break;
	case ECDST_GodArea:
		{
			// 如果技能列表数量小于当前技能页的最小值，则直接显示第一页的技能
			if( m_lstGodSkill.Size() <= MAX_SKILL_BOOK_NUM*m_nCurPage )
			{
				SetCurPage( 0 );
			}

			INT nIndex = 0;
			INT	nCount = 0;
			DWORD dwID;
			m_lstGodSkill.ResetIterator();
			while( m_lstGodSkill.PeekNext( dwID ) )
			{
				if( !IsCurPage(nIndex) )
				{
					++nIndex;
					continue;
				}

				if( nCount >= MAX_SKILL_BOOK_NUM )
					break;

				m_SkillItem[nCount].UpdateSkill( dwID );
				++nIndex;
				++nCount;
			}

			// 如果没有到最后一个格，剩余格不显示
			for( ; nCount<MAX_SKILL_BOOK_NUM; ++nCount )
			{
				m_SkillItem[nCount].UpdateSkill( GT_INVALID );
			}

			// 刷新按钮
			m_pBtnPageUp->SetEnable( m_nCurPage < m_lstGodSkill.Size()/MAX_SKILL_BOOK_NUM );
			m_pBtnPageDown->SetEnable( m_nCurPage > 0 );
			if(P_VALID(m_pProduceNotice))m_pProduceNotice->SetInvisible(TRUE);
		}
		break;
	}
}

VOID SkillBookFrame::OnPageUp( BOOL bPageUp )
{
	INT nMaxPage = GetMaxPages();
	if( nMaxPage == GT_INVALID )
		return;

	CheckCurPage();
	if( bPageUp )
	{
		if( m_nCurPage < nMaxPage )
		{
			m_nCurPage++;
		}
	}
	else 
	{
		if( m_nCurPage > 0 )
		{
			m_nCurPage--;
		}
	}
	
	// 刷新界面
	ReflashCurPage();

	SetCurPage( m_nCurPage );
}

VOID SkillBookFrame::SetCurPage( INT nPage )
{
	m_nCurPage = nPage;
	TCHAR szTmp[X_SHORT_NAME] = {0};
	_sntprintf( szTmp, sizeof(szTmp)/sizeof(TCHAR), _T( "%d" ), nPage+1 );
	m_pStcPage->SetText( szTmp );
}

DWORD SkillBookFrame::OnWeaponSwap( tagGameEvent* pGameEvent )
{	
	m_skillList.Clear();

	SkillMgr::Inst()->GetSkillList( m_skillList );

	ReflashCurPage();	
	return 0;
}

VOID SkillBookFrame::Update()
{
	float deltaTime =  Cool3D::Kernel::Inst()->GetDeltaTime();
	
	BOOL b = m_produceSkill->IsInvisible();
	b = m_passiveSkill->IsInvisible();
	b = m_activesSkill->IsInvisible();
	b = m_GuildSkill->IsInvisible();

	GameFrame::Update();
}

INT SkillBookFrame::GetMaxPages()
{
	switch( m_eCurSkillType )
	{
	case ECDST_Active:
		return m_skillList.Size() / MAX_SKILL_BOOK_NUM;
	case ECDST_Passive:
		return m_PassiveSkillList.Size() / MAX_SKILL_BOOK_NUM;
	case ECDST_Produce:
		return m_lstProduceSkill.Size() / MAX_SKILL_BOOK_NUM;
	case ECDST_Guild:
		return m_lstGuildSkill.Size() / MAX_SKILL_BOOK_NUM;
	case ECDST_GodArea:
		return m_lstGodSkill.Size() / MAX_SKILL_BOOK_NUM;
	}

	return 0;
}

void SkillBookFrame::CheckCurPage()
{
	//校正当前页数
	if( m_nCurPage < 0 )
		m_nCurPage = 0;

	if( m_nCurPage > GetMaxPages() )
		m_nCurPage = GetMaxPages();
}