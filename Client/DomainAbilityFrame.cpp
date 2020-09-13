#include "StdAfx.h"
#include "DomainAbilityFrame.h"
#include "IconStatic.h"
#include "SkillMgr.h"
#include "SkillProtoData.h"
#include "..\WorldDefine\skill_define.h"
#include "..\WorldDefine\clergy_define.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "RoleMgr.h"
#include "LocalPlayer.h"
#include "CondensedProtoData.h"
#include "..\WorldDefine\role_att.h"
#include "ClergyProtoData.h"
#include "GodMgr.h"
#include "RoleEvent.h"
#include "..\WorldDefine\msg_god.h"
#include "CombatSysUtil.h"

const TCHAR*  UNKOWN_PIC = _T("data\\ui\\Bag\\L-main-icon-lock.bmp");
const DWORD PUSH_TIMER = 1000;
const TCHAR* szBackPicUnKnow = _T("data\\ui\\Main\\unknown.bmp");
;

void tagAbilityButton::RefreshAbilityButton(DWORD dwID)
{
	const tagGodAreaName* pProto = GodMgr::Inst()->GetAreaDataByID(dwID);
	if(!P_VALID(pProto))	return;
	dwSkillID =dwID;
	bUnKnow= false;
	if (0 == dwID || GT_INVALID == dwID)
	{
		pIsPic->SetPic(UNKOWN_PIC);
		pStName->SetTextColor(0xFFC0C0C0);
		pStName->SetText(g_StrTable[_T("DomainAbility_UnKnow")]);
		pStDesc->SetText(g_StrTable[_T("DomainAbility_UnKnow")]);
		pStLevel->SetText(_T(""));
	}
	else
	{
		TCHAR szBuffer[X_SHORT_NAME] = {0};
		TCHAR szBigBuffer[X_LONG_NAME] = {0};
		// level
		//const tagSkillData* pSkillData = SkillMgr::Inst()->GetSkillData(pProto->dwSkillID);
		//if (!P_VALID(pSkillData))
		//{
			//pStLevel->SetText(_T("Lv 1"));
		//}
		//else
		//{
			_sntprintf(szBuffer, X_SHORT_NAME, _T("Lv %d"), pProto->nLevel);
			pStLevel->SetText(szBuffer);
		//}

		// name & pic &desc
		//DWORD dwSkillTypeID = MTransSkillTypeID(dwID, 1);
		//const tagSkillProtoClient *pSkillProto = SkillProtoData::Inst()->FindSkillProto(dwID);
		const tagGodAreaName* pSkillProto = GodMgr::Inst()->GetAreaDataByID(dwID);
		if (P_VALID(pSkillProto))
		{
			_sntprintf(szBigBuffer, X_LONG_NAME, _T("data\\ui\\Icon\\%s.tga"), pSkillProto->szIcon);
			pIsPic->SetPic(szBigBuffer);
			pStName->SetTextColor(0xFF00FF00);
			pStName->SetText(pSkillProto->szName);

			if (ESUT_Active == pSkillProto->eUseType )
			{
				pStDesc->SetText(g_StrTable[_T("DomainAbility_SkillType_Active")]);
			}
			else if (ESUT_Passive == pSkillProto->eUseType)
			{
				pStDesc->SetText(g_StrTable[_T("DomainAbility_SkillType_Passive")]);
			}
		}

		// todo 是否已经被激活的刷新，等待服务器人员确定该参数在proto位置以后再填写
	}

	pStMainBack->SetInvisible(FALSE);
}
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
DomainAbilityFrame::DomainAbilityFrame(void):m_Trunk(this)
{
}

DomainAbilityFrame::~DomainAbilityFrame(void)
{
}
BOOL DomainAbilityFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	m_pCmdMgr->Register( "NS_GetRoleInitState_Area",		(NETMSGPROC)m_Trunk.sfp2(&DomainAbilityFrame::OnNS_GetRoleInitState_Area),		_T("NS_GetRoleInitState_Area"));
	m_pCmdMgr->Register( "NS_ActivationArea",		(NETMSGPROC)m_Trunk.sfp2(&DomainAbilityFrame::OnNS_ActivationArea),		_T("NS_ActivationArea"));
	m_pCmdMgr->Register( "NS_AddArea",		(NETMSGPROC)m_Trunk.sfp2(&DomainAbilityFrame::OnNS_AddArea),		_T("NS_AddArea"));
	m_pCmdMgr->Register( "NS_RareAreaInfo",		(NETMSGPROC)m_Trunk.sfp2(&DomainAbilityFrame::OnNS_LoseArea),		_T("NS_RareAreaInfo"));
	m_pCmdMgr->Register( "NS_PromotionClergy",		(NETMSGPROC)m_Trunk.sfp2(&DomainAbilityFrame::OnNS_PromotionClergy),		_T("NS_PromotionClergy"));
	m_pCmdMgr->Register( "NS_ChangeClergy",		(NETMSGPROC)m_Trunk.sfp2(&DomainAbilityFrame::OnNS_ChangeClergy),		_T("NS_ChangeClergy"));

	m_pMgr->RegisterEventHandle(_T("tagUpdateLPAttEvent"),	  (FRAMEEVENTPROC)m_Trunk.sfp1(&DomainAbilityFrame::OnUpdateLPAttEvent));
	m_pMgr->RegisterEventHandle(_T("Open_GodArea"),	  (FRAMEEVENTPROC)m_Trunk.sfp1(&DomainAbilityFrame::OnOpenEvent));
	m_pMgr->RegisterEventHandle(_T("Close_GodArea"),	  (FRAMEEVENTPROC)m_Trunk.sfp1(&DomainAbilityFrame::OnCloseEvent));
	m_pMgr->RegisterEventHandle(_T("tagUpdateLPAttEvent"),	  (FRAMEEVENTPROC)m_Trunk.sfp1(&DomainAbilityFrame::OnGameEvent));


	m_eCurrDomain = ED_God;
	m_nCurrPage = 0;
	m_dwPushingTime = 0;
	m_bStartCount = false;
	m_bOpened = false;
	m_dwUpdateTime = 0;

	tagNC_GetRoleInitState m;
	m.eType = ERIT_Area;
	m_pSession->Send(&m);
	return TRUE;
}
BOOL DomainAbilityFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();

	m_pCmdMgr->UnRegister( "NS_GetRoleInitState_Area",							(NETMSGPROC)m_Trunk.sfp2(&DomainAbilityFrame::OnNS_GetRoleInitState_Area) );
	m_pCmdMgr->UnRegister( "NS_ActivationArea",							(NETMSGPROC)m_Trunk.sfp2(&DomainAbilityFrame::OnNS_ActivationArea) );
	m_pCmdMgr->UnRegister( "NS_AddArea",							(NETMSGPROC)m_Trunk.sfp2(&DomainAbilityFrame::OnNS_AddArea) );
	m_pCmdMgr->UnRegister( "NS_RareAreaInfo",							(NETMSGPROC)m_Trunk.sfp2(&DomainAbilityFrame::OnNS_LoseArea) );
	m_pCmdMgr->UnRegister("NS_PromotionClergy",		(NETMSGPROC)m_Trunk.sfp2(&DomainAbilityFrame::OnNS_PromotionClergy));
	m_pCmdMgr->UnRegister("NS_ChangeClergy",		(NETMSGPROC)m_Trunk.sfp2(&DomainAbilityFrame::OnNS_ChangeClergy));

	m_pMgr->UnRegisterEventHandler(_T("tagUpdateLPAttEvent"),	 (FRAMEEVENTPROC)m_Trunk.sfp1(&DomainAbilityFrame::OnUpdateLPAttEvent));
	m_pMgr->UnRegisterEventHandler(_T("Open_GodArea"),	 (FRAMEEVENTPROC)m_Trunk.sfp1(&DomainAbilityFrame::OnOpenEvent));
	m_pMgr->UnRegisterEventHandler(_T("Close_GodArea"),	 (FRAMEEVENTPROC)m_Trunk.sfp1(&DomainAbilityFrame::OnCloseEvent));
	m_pMgr->UnRegisterEventHandler(_T("tagUpdateLPAttEvent"),	 (FRAMEEVENTPROC)m_Trunk.sfp1(&DomainAbilityFrame::OnGameEvent));

	if(P_VALID( m_pWnd ) )
	{
		m_pGUI->DestroyWnd(m_pWnd);
		m_pWnd = NULL;		
	}

	m_vecGodDomain.clear();
	m_mmapFreePickDomain.clear();
	m_vecRareDomain.clear();

	return bRet;
}
BOOL DomainAbilityFrame::ReloadUI()
{
 	XmlElement element;
 	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\god_ability.xml");
 	if( !m_pGUI->LoadXml( &element, "VFS_System", strPath.c_str() ) )
 		return FALSE;

	m_pGUI->ChangeXml( &element, "god_ability_win\\pic4\\backpic\\back0\\backpic\\jobpic", "ClassName", "IconStatic");
	TCHAR buffer[X_LONG_NAME];
	string sTempName;
	for (int i =0; i<DOMAIN_ROW_NUM;i++)
	{
		for(int j =0; j< DOMAIN_COLUNM_NUM; j++)
		{
			_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("god_ability_win\\pic4\\backpic\\back1\\back1\\ability%d\\pic"), i*DOMAIN_COLUNM_NUM + j);
			sTempName = m_pUtil->UnicodeToAnsi(buffer);
			m_pGUI->ChangeXml( &element, sTempName.c_str(), "ClassName", "IconStatic");
		}
		
	}

 	m_pWnd = m_pGUI->CreateWnd( _T("\\desktop"), &element );
 	if( !P_VALID( m_pWnd ) )
 	{
 		m_pGUI->DestroyWnd(m_pWnd);
 		m_pWnd = NULL;		
 		return FALSE;
 	}

 	m_pGUI->RegisterEventHandler( m_pWnd->GetFullName().c_str(),   m_Trunk.sfp1(&DomainAbilityFrame::EventHandler));

	m_pPbnGod = (GUIPushButton*)m_pWnd->GetChild(_T("pic4\\backpic\\choosearea\\area0"));
	m_pPbnFreePick= (GUIPushButton*)m_pWnd->GetChild(_T("pic4\\backpic\\choosearea\\area1"));
	m_pPbnRare= (GUIPushButton*)m_pWnd->GetChild(_T("pic4\\backpic\\choosearea\\area2"));
	m_pPbnActive= (GUIPushButton*)m_pWnd->GetChild(_T("pic4\\backpic\\choosearea\\area3"));

	m_pBnClose =  (GUIButton*)m_pWnd->GetChild(_T("pic4\\closebutton"));
	m_pCurrGodJobPic = (IconStatic*)m_pWnd->GetChild(_T("pic4\\backpic\\back0\\backpic\\jobpic"));
	m_pStAgglomerate = (GUIProgress*)m_pWnd->GetChild(_T("pic4\\backpic\\back0\\progpic0\\proqress"));
	m_pStXinYang = (GUIStatic*)m_pWnd->GetChild(_T("pic4\\backpic\\back0\\expnum"));
	m_pStAgglomerateText = (GUIStatic*)m_pWnd->GetChild(_T("pic4\\backpic\\back0\\progpic0\\num1"));

	m_pBnPer=  (GUIButton*)m_pWnd->GetChild(_T("pic4\\backpic\\back1\\backbutt"));
	m_pBnNext=  (GUIButton*)m_pWnd->GetChild(_T("pic4\\backpic\\back1\\nextbutt"));
	m_pStPage=  (GUIStatic*)m_pWnd->GetChild(_T("pic4\\backpic\\back1\\pagenum"));
	m_pStexDesc = (GUIStaticEx*)m_pWnd->GetChild(_T("pic4\\backpic\\back2\\intro_win\\introword"));
	m_pStPicSide =  (GUIStatic*)m_pWnd->GetChild(_T("pic4\\backpic\\back1\\pic"));
	for (int i =0; i < E_WordNum; i++)
	{
		_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("pic4\\backpic\\back2\\word\\word%d"), i);
		m_sDesc[i].pWord = (GUIStatic*)m_pWnd->GetChild(buffer);
		m_sDesc[i].pNum = (GUIStatic*)m_sDesc[i].pWord->GetChild(_T("num"));
	}
	for (int i =0;i < MAX_GOD_LEVEL; i++)
	{
		_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("pic4\\backpic\\back0\\levelpic\\%d\\get"), i);
		m_pGodLevelPic[i] = (GUIStatic*)m_pWnd->GetChild(buffer);
	}

	for (int i =0; i<DOMAIN_ROW_NUM;i++)
	{
		for(int j =0; j< DOMAIN_COLUNM_NUM; j++)
		{
			_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("pic4\\backpic\\back1\\back1\\ability%d"), i*DOMAIN_COLUNM_NUM + j);
			m_abilityButton[i][j].pStMainBack = (GUIStatic*)m_pWnd->GetChild(buffer);
			m_abilityButton[i][j].pPbSelected = (GUIPushButton*)m_abilityButton[i][j].pStMainBack->GetChild(_T("optbutt"));
			m_abilityButton[i][j].pIsPic= (IconStatic*)m_abilityButton[i][j].pStMainBack->GetChild(_T("picback\\pic"));
			m_abilityButton[i][j].pStName= (GUIStatic*)m_abilityButton[i][j].pStMainBack->GetChild(_T("name"));
			m_abilityButton[i][j].pStLevel= (GUIStatic*)m_abilityButton[i][j].pStMainBack->GetChild(_T("levelEx"));
			m_abilityButton[i][j].pStDesc= (GUIStatic*)m_abilityButton[i][j].pStMainBack->GetChild(_T("type"));
		}

	}

	for (int i =0;i < DOMAIN_ROW_NUM; i++)
	{
		_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("pic4\\backpic\\back1\\state\\state%d"), i);
		m_pStGodLevel[i].pBack = (GUIStatic*)m_pWnd->GetChild(buffer);
		m_pStGodLevel[i].pText = (GUIStatic*)m_pStGodLevel[i].pBack->GetChild(_T("level"));
	}
	
 	m_pWnd->SetInvisible(TRUE);

	return TRUE;
}
BOOL DomainAbilityFrame::EscCancel()
{
	if (m_pWnd->IsInvisible())
	{
		return false;
	} 
	else
	{
		m_pWnd->SetInvisible(TRUE);
		m_pFrameMgr->SendEvent( &tagGameEvent( _T("Close_Area_UI"), this ) );
		return true;
	}
}


BOOL DomainAbilityFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd( pEvent->dwWndID );
	if( !P_VALID(pWnd) )
		return FALSE;

	switch( pEvent->eEvent )
	{
	case EGUIE_Click:
		{
			if (pWnd == m_pBnPer)
			{
				m_nCurrPage--;
				if (m_nCurrPage <0)
				{
					m_nCurrPage =0;
					break;
				}
				RefreshUI();
			}
			else if (pWnd == m_pBnNext)
			{
				m_nCurrPage++;
				if (m_nCurrPage > m_nCurrTotalPage)
				{
					m_nCurrPage =m_nCurrTotalPage;
					break;
				}
				RefreshUI();
			}
			else if (pWnd == m_pBnClose)
			{
				CloseFrame();
				m_pFrameMgr->SendEvent( &tagGameEvent( _T("Close_Area_UI"), this ) );
			}
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			for (int i =0;i < DOMAIN_ROW_NUM ;i++)
			{
				for (int j =0;j < DOMAIN_COLUNM_NUM; j++)
				{
					if (pWnd == m_abilityButton[i][j].pIsPic)
					{
						tstringstream ss;
						const tagGodAreaName* pSkillProto = GodMgr::Inst()->GetAreaDataByID(m_abilityButton[i][j].dwSkillID);
						if (!P_VALID(pSkillProto))	break;

						if (m_abilityButton[i][j].bUnKnow)
						{
							ss << g_StrTable[_T("AreaDesc_1")] <<_T("\\n") <<  pSkillProto->szDesc << _T("\\n") << _T("\\n")
								<< g_StrTable[_T("AreaDesc_2")] <<_T("\\n") <<  pSkillProto->szGetDesc;
							ShowTip(pWnd, ss.str().c_str());
						}
						else
						{
							tstring strTip = ToolTipCreator::Inst()->GetAreaTips(pSkillProto->dwID);
							if(strTip.c_str() ==NULL || _tcsnlen(strTip.c_str(), sizeof(strTip.c_str()))==0)
								ShowTip(pWnd, g_StrTable[_T("NotActiveAbilitySkill")]);
							else
							{
								if (ED_Rare == m_eCurrDomain)
								{
									for (vector<DomianData>::iterator iter = m_vecRareDomain.begin();
										iter != m_vecRareDomain.end(); ++iter)
									{
										if(iter->dwTime && m_abilityButton[i][j].dwSkillID == iter->dwSkillID)
										{
											DWORD time = iter->dwTime;
											DWORD hour = time/60/60;
											DWORD min = (time/60)%60;
											DWORD second = time%60;
											ss  <<_T("\\n") <<  g_StrTable[_T("E_God_Area_Lefttime")];
											if(hour) ss << hour << g_StrTable[_T("AreaTips_Hour")];
											if(min)	ss << min << g_StrTable[_T("AreaTips_Min")];
											if(second)	ss << second << g_StrTable[_T("AreaTips_Second")];
											strTip += ss.str();
											break;
										}
									}
								}
								ShowTip(pWnd, strTip.c_str());
							}
						}
					}
				}
			}
		}
		break;
	case EGUIE_PushButtonDown:
		{
			if (pWnd == m_pPbnGod)
			{
				m_eCurrDomain = ED_God;
				m_nCurrPage =0;
				RefreshUI();
			}
			else if (pWnd == m_pPbnFreePick)
			{	
				m_eCurrDomain = ED_FreePick;
				m_nCurrPage =0;
				RefreshUI();
			}
			else if (pWnd == m_pPbnRare)
			{
				m_eCurrDomain = ED_Rare;
				m_nCurrPage =0;
				RefreshUI();
			}
			else if (pWnd == m_pPbnActive)
			{
				m_eCurrDomain = ED_Enabled;
				m_nCurrPage =0;
				RefreshUI();
			}
			else
			{
				//if (!m_bStartCount)	// 不是由刷新整个UI而触发的 EGUIE_PushButtonDown 事件
				{
					for (int i =0;i < DOMAIN_ROW_NUM ;i++)
					{
						for (int j =0;j < DOMAIN_COLUNM_NUM; j++)
						{
							if (pWnd == m_abilityButton[i][j].pPbSelected)
							{
								tagNC_ActivationArea cmd;
								cmd.dwAreaID = m_abilityButton[i][j].dwSkillID;
								cmd.bActive = EGUIBS_PushDown == m_abilityButton[i][j].pPbSelected->GetState() ? true :false ;
								m_pSession->Send(&cmd);

								m_abilityButton[i][j].pPbSelected->SetEnable(FALSE);
							}
						}
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

void DomainAbilityFrame::GetShowFreePickDomainInfo(int &level,int &pos)
{
	if(0 == m_nCurrPage)
	{
		level = 1;
		pos =0;
	}
	else
	{
		int nStartShowRow = m_nCurrPage * DOMAIN_ROW_NUM;	// 显示数据的是实际的第几行
		int nTempRowCount = 0;	// 临时数据，记录行数
		for (int i =1; i<= MAX_GodHead ; i++)
		{
			int thisLevelStartRow = nTempRowCount;	// i对应的神格是第几行开始的
			int nCount = GodMgr::Inst()->GetFreePickAreaNumByGodHead(i);
			if(0 == nCount)
				continue;

			nTempRowCount += (nCount/DOMAIN_COLUNM_NUM);
			if( 0 != nCount % DOMAIN_COLUNM_NUM )	nTempRowCount += 1;
		
			// 正好显示完本神格的所有技能
			if (nTempRowCount == nStartShowRow )
			{
				level = i+1;
				pos =0;
				return;
			}
			else if (nTempRowCount > nStartShowRow)
			{
				level = i;
				pos = (nStartShowRow - thisLevelStartRow) * DOMAIN_COLUNM_NUM ;
				return;
			}
		}
	}
}

void DomainAbilityFrame::RefreshUI()
{

	RefreshPlayerInfo();

	if (ED_God == m_eCurrDomain)
	{
		RefreshGodDomain();
	}
	else if (ED_FreePick == m_eCurrDomain)
	{
		m_dwPushingTime = 0;
		m_bStartCount = true;
		EnableAllPushButtons(TRUE);
		RefreshFreePickDomain();
	}
	else if (ED_Rare == m_eCurrDomain)
	{
		m_dwPushingTime = 0;
		m_bStartCount = true;
		EnableAllPushButtons(TRUE);
		RefreshRareDomain();
	}
	else if (ED_Enabled == m_eCurrDomain)
	{
		RefreshAllDomains();
	}
	RefReshDesc(m_eCurrDomain);

	RefreshPage();
}

void DomainAbilityFrame::RefreshFreePickDomain()
{
	int nStartShowRow = 0;

	int nStartLevel = 0;
	int nStartPos = 0;
	GetShowFreePickDomainInfo(nStartLevel,nStartPos);

	HideAllAbilityButton();
	HideAllGodLevelPic(true);
	m_pStPicSide->SetInvisible(TRUE);

	int PosMark = 0;
	while (nStartShowRow < DOMAIN_ROW_NUM	&&	nStartLevel <= MAX_GodHead)
	{
		multimap<int,DomianData>::iterator itBegin, itEnd;
		itBegin = m_mmapFreePickDomain.lower_bound(nStartLevel);
		itEnd   = m_mmapFreePickDomain.upper_bound(nStartLevel);
		int distances = std::distance(itBegin,itEnd);	// 改神格已知的领域数量
		int LastDis = GodMgr::Inst()->GetFreePickAreaNumByGodHead(nStartLevel) - distances;	// 未知领域的数量
		TList<DWORD>	m_lstMaxNum;
		GodMgr::Inst()->GetFreePickAreaListByNum(nStartLevel,m_lstMaxNum);
		if (0 == distances && 0 ==LastDis )
		{
			nStartPos = 0;
			nStartLevel++;
			continue;
		}

		if(0 != nStartPos)
			LastDis = LastDis + distances -nStartPos;		// 剩余的需要显示的个数(这里LastDis 代表的是剩余的一共有多少个数量)

		if( 0 != nStartPos && nStartPos < distances)		// 说明LastDis里面包含有 已经知道的领域
			LastDis -= (distances - nStartPos);		// 这里就是实际上剩余的未知领域技能的数量。

		int DisMarl = 0;
		// 检索已经显示了多少，设置一些引用计数
		for (int i =0; i<nStartPos ;i++)
		{
			if (itBegin != itEnd )
			{
				DWORD tempID = itBegin->second.dwSkillID/100;
				m_lstMaxNum.Erase(tempID);
				++itBegin;
			}
			else
			{
				m_lstMaxNum.PopFront();
			}
		}

		//	设置行号
		if(itBegin != itEnd || LastDis >0 )
			SetGodLevel(nStartShowRow,nStartLevel);
		else
			m_pStGodLevel[nStartShowRow].pBack->SetInvisible(TRUE);

		// 新的神格对应的位置信息开始重新计算了 ，把POS的位置放到下一行的开始位置
		while(PosMark != 0 && PosMark%DOMAIN_COLUNM_NUM != 0)
			PosMark++;

		int RowMark =0;
		while(itBegin != itEnd)
		{
			if(nStartShowRow >= DOMAIN_ROW_NUM)	break;
			if(PosMark >= DOMAIN_COLUNM_NUM*DOMAIN_ROW_NUM)	break;

			m_abilityButton[PosMark/DOMAIN_COLUNM_NUM][PosMark%DOMAIN_COLUNM_NUM].RefreshAbilityButton(itBegin->second.dwSkillID);
			m_abilityButton[PosMark/DOMAIN_COLUNM_NUM][PosMark%DOMAIN_COLUNM_NUM].SetActive(itBegin->second.bActive);
			m_abilityButton[PosMark/DOMAIN_COLUNM_NUM][PosMark%DOMAIN_COLUNM_NUM].HideSelectBtn(false);
			DWORD tempID = itBegin->second.dwSkillID/100;
			m_lstMaxNum.Erase(tempID);
			
			// 增加各种计数信息
			//++DisMarl;
			++PosMark;
			++itBegin;
			RowMark++;
			// 查看是否需要换行
			if (DOMAIN_COLUNM_NUM == RowMark)
			{
				nStartShowRow++;
				RowMark =0;

				if(nStartShowRow >= DOMAIN_ROW_NUM)	break;
				if(RowMark >= DOMAIN_COLUNM_NUM*DOMAIN_ROW_NUM)	break;
				//	隐藏间隔行号
				//if(itBegin != itEnd)
					//m_pStGodLevel[nStartShowRow].pBack->SetInvisible(TRUE);
			}
		}

		// 显示未知领域图标
		while(LastDis - DisMarl >0)
		{
			if(nStartShowRow >= DOMAIN_ROW_NUM)	break;
			if(PosMark >= DOMAIN_COLUNM_NUM*DOMAIN_ROW_NUM)	break;

			m_abilityButton[PosMark/DOMAIN_COLUNM_NUM][PosMark%DOMAIN_COLUNM_NUM].pStMainBack->SetInvisible(FALSE);
			m_abilityButton[PosMark/DOMAIN_COLUNM_NUM][PosMark%DOMAIN_COLUNM_NUM].pPbSelected->SetEnable(FALSE);
			m_abilityButton[PosMark/DOMAIN_COLUNM_NUM][PosMark%DOMAIN_COLUNM_NUM].pIsPic->SetPic(szBackPicUnKnow);
			m_abilityButton[PosMark/DOMAIN_COLUNM_NUM][PosMark%DOMAIN_COLUNM_NUM].pStLevel->SetText(_T("Lv -"));
			m_abilityButton[PosMark/DOMAIN_COLUNM_NUM][PosMark%DOMAIN_COLUNM_NUM].pStDesc->SetText(g_StrTable[_T("AbilityUnKnow")]);
			m_abilityButton[PosMark/DOMAIN_COLUNM_NUM][PosMark%DOMAIN_COLUNM_NUM].pStName->SetTextColor(0xFFC0C0C0);
			m_abilityButton[PosMark/DOMAIN_COLUNM_NUM][PosMark%DOMAIN_COLUNM_NUM].pStName->SetText(g_StrTable[_T("AbilityUnKnow")]);
			m_abilityButton[PosMark/DOMAIN_COLUNM_NUM][PosMark%DOMAIN_COLUNM_NUM].HideSelectBtn(false);
			m_abilityButton[PosMark/DOMAIN_COLUNM_NUM][PosMark%DOMAIN_COLUNM_NUM].bUnKnow = true;
			m_abilityButton[PosMark/DOMAIN_COLUNM_NUM][PosMark%DOMAIN_COLUNM_NUM].dwSkillID = m_lstMaxNum.PopFront()*100+1;	// 保存一级的ID
	
			// 增加各种计数信息
			++DisMarl;
			++PosMark;
			RowMark++;
			if (DOMAIN_COLUNM_NUM == RowMark)
			{
				//	隐藏间隔行号
				//m_pStGodLevel[nStartShowRow].pBack->SetInvisible(TRUE);

				nStartShowRow++;
				RowMark =0;

				if(nStartShowRow >= DOMAIN_ROW_NUM)	break;
				if(RowMark >= DOMAIN_COLUNM_NUM*DOMAIN_ROW_NUM)	break;

			}
		}

		//	隐藏最后一行没有结束的行号
// 		if(GodMgr::Inst()->GetFreePickAreaNumByGodHead(nStartLevel) -  nStartPos > DOMAIN_COLUNM_NUM)
// 			m_pStGodLevel[nStartShowRow].pBack->SetInvisible(TRUE);

		nStartPos = 0;
		nStartLevel++;

		if(0 != PosMark%DOMAIN_COLUNM_NUM)
			nStartShowRow++;		// 本级神格对应领域技能完毕，起始行另起一行
	}

	// 隐藏不用显示的剩余LEVEL标记
	if (nStartShowRow < DOMAIN_ROW_NUM )
	{
		for (int i = nStartShowRow ; i < DOMAIN_ROW_NUM; ++i)
		{
				//m_pStGodLevel[i].pBack->SetInvisible(TRUE);
		}
	}
	// 获得最大页数
	int CurrLevel = 0;
	int CurrRow = 0;
	while (CurrLevel <= MAX_GodHead)
	{
// 		multimap<int,DomianData>::iterator itBegin, itEnd;
// 		itBegin = m_mmapFreePickDomain.lower_bound(CurrLevel);
// 		itEnd   = m_mmapFreePickDomain.upper_bound(CurrLevel);
//		int dis = std::distance(itBegin,itEnd);
		int dis = GodMgr::Inst()->GetFreePickAreaNumByGodHead(CurrLevel);
		CurrRow += dis%DOMAIN_COLUNM_NUM + dis/DOMAIN_COLUNM_NUM; 
		CurrLevel++;
	}
	m_nCurrTotalPage = CurrRow /(DOMAIN_ROW_NUM);

}

void DomainAbilityFrame::RefreshRareDomain()
{
	HideAllAbilityButton();
	HideAllGodLevelPic(true);
	m_pStPicSide->SetInvisible(FALSE);
	// 鉴于稀有领域技能不会很多。目前只考虑有一页的情况
	int PosMark =0;
	vector<DomianData>::iterator iter =m_vecRareDomain.begin() + m_nCurrPage*DOMAIN_COLUNM_NUM*DOMAIN_ROW_NUM;
	for (; iter != m_vecRareDomain.end(); ++iter)
	{
		m_abilityButton[PosMark/DOMAIN_COLUNM_NUM][PosMark%DOMAIN_COLUNM_NUM].RefreshAbilityButton(iter->dwSkillID);
		m_abilityButton[PosMark/DOMAIN_COLUNM_NUM][PosMark%DOMAIN_COLUNM_NUM].SetActive(iter->bActive);
		m_abilityButton[PosMark/DOMAIN_COLUNM_NUM][PosMark%DOMAIN_COLUNM_NUM].HideSelectBtn(false);
	
		PosMark++;
		if(PosMark >=DOMAIN_COLUNM_NUM*DOMAIN_ROW_NUM )
			break;
	}
	m_nCurrTotalPage = m_vecRareDomain.size()/(DOMAIN_COLUNM_NUM*DOMAIN_ROW_NUM);
}

void DomainAbilityFrame::RefreshGodDomain()
{
	BuildGodDomains();
	HideAllAbilityButton();
	HideAllGodLevelPic(true);
	m_pStPicSide->SetInvisible(FALSE);

	int nStartPos = m_nCurrPage * DOMAIN_COLUNM_NUM * DOMAIN_ROW_NUM;
	if(nStartPos > m_vecGodDomain.size())	return;
	int PosMark = 0;
	vector<DWORD>::iterator iter;
	for (iter=m_vecGodDomain.begin();
		iter != m_vecGodDomain.end(); ++iter)
	{
		if(PosMark == nStartPos) 
			break;

		PosMark++;
	}

	PosMark =0;
	while (iter !=m_vecGodDomain.end() && PosMark <  DOMAIN_COLUNM_NUM*DOMAIN_ROW_NUM)
	{
	
		m_abilityButton[PosMark/DOMAIN_COLUNM_NUM][PosMark%DOMAIN_COLUNM_NUM].RefreshAbilityButton(*iter);
		m_abilityButton[PosMark/DOMAIN_COLUNM_NUM][PosMark%DOMAIN_COLUNM_NUM].HideSelectBtn(true);

		++iter;
		++PosMark;
	}

	// 获得最大页数
	int CurrRow = 0;

	vector<DWORD>::iterator itBegin, itEnd;
	itBegin = m_vecGodDomain.begin();
	itEnd   = m_vecGodDomain.end();
	int dis = std::distance(itBegin,itEnd);
	CurrRow += dis%DOMAIN_COLUNM_NUM + dis/DOMAIN_COLUNM_NUM; 
	
	if (CurrRow < DOMAIN_ROW_NUM)
		m_nCurrTotalPage = 0;
	else
		m_nCurrTotalPage = CurrRow /(DOMAIN_ROW_NUM);
}

void DomainAbilityFrame::RefreshAllDomains()
{
	HideAllAbilityButton();
	HideAllGodLevelPic(true);
	m_pStPicSide->SetInvisible(FALSE);
	vector<DWORD> vec_AllDomain;
	for (vector<DWORD>::iterator iter3 = m_vecGodDomain.begin(); iter3 != m_vecGodDomain.end(); ++iter3)
	{
		vec_AllDomain.push_back(*iter3);
	}

	for (multimap<int,DomianData>::iterator it = m_mmapFreePickDomain.begin();
		it != m_mmapFreePickDomain.end(); ++it)
	{
		if (it->second.bActive)
		{
			vec_AllDomain.push_back(it->second.dwSkillID);
		}
	}

	for (vector<DomianData>	::iterator it = m_vecRareDomain.begin();
		it != m_vecRareDomain.end(); ++it)
	{
		if (it->bActive)
		{
			vec_AllDomain.push_back(it->dwSkillID);
		}
	}

	int PosMark =0;
	vector<DWORD>::iterator iter=vec_AllDomain.begin() + m_nCurrPage * DOMAIN_COLUNM_NUM * DOMAIN_ROW_NUM;
	while (iter !=vec_AllDomain.end() && PosMark <  DOMAIN_COLUNM_NUM*DOMAIN_ROW_NUM)
	{
		m_abilityButton[PosMark/DOMAIN_COLUNM_NUM][PosMark%DOMAIN_COLUNM_NUM].RefreshAbilityButton(*iter);
		m_abilityButton[PosMark/DOMAIN_COLUNM_NUM][PosMark%DOMAIN_COLUNM_NUM].HideSelectBtn(true);

		++iter;
		++PosMark;
	}

	// 获得最大页数
	int CurrRow = 0;

	vector<DWORD>::iterator itBegin, itEnd;
	itBegin = vec_AllDomain.begin();
	itEnd   = vec_AllDomain.end();
	int dis = std::distance(itBegin,itEnd);
	//CurrRow += dis%DOMAIN_COLUNM_NUM + dis/DOMAIN_COLUNM_NUM; 
	CurrRow += dis/DOMAIN_COLUNM_NUM;
	if(dis%DOMAIN_COLUNM_NUM != 0)
		CurrRow += 1;

	m_nCurrTotalPage = CurrRow /(DOMAIN_ROW_NUM);
	if (!(CurrRow%DOMAIN_ROW_NUM))
		m_nCurrTotalPage -=1;
}

void DomainAbilityFrame::HideAllAbilityButton()
{
	for (int i = 0; i < DOMAIN_ROW_NUM; i++)
	{
		for (int j = 0; j <DOMAIN_COLUNM_NUM ; j++)
		{
			m_abilityButton[i][j].pStMainBack->SetInvisible(TRUE);
			m_abilityButton[i][j].Clear();
		}
	}

	for (int j =0; j< DOMAIN_ROW_NUM; j++)
	{
		m_pStGodLevel[j].pBack->SetInvisible(TRUE);
	}
}

void DomainAbilityFrame::SetGodLevel(int row,int num)
{
	TCHAR szBuffer[X_SHORT_NAME] = {0};
	_sntprintf(szBuffer, X_SHORT_NAME, _T("Lv %d"), num);
	m_pStGodLevel[row].pText->SetText(szBuffer);
	m_pStGodLevel[row].pBack->SetInvisible(FALSE);
}

void DomainAbilityFrame::RefreshPage()
{
	TCHAR szBuffer[X_SHORT_NAME] = {0};
	_sntprintf(szBuffer, X_SHORT_NAME, _T("%d/%d"), m_nCurrPage+1,m_nCurrTotalPage+1);
	m_pStPage->SetText(szBuffer);
	m_pStPage->SetInvisible(FALSE);
}

void DomainAbilityFrame::RefreshPlayerInfo()
{
	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	if(!P_VALID(pLp))	return;


	// 刷新神格
	int nCurrLocalplayerGodLevel = pLp->GetAttribute(ERA_God_Godhead);
	if (0 ==nCurrLocalplayerGodLevel)
	{
		for (int i = 0; i < MAX_GOD_LEVEL; i++)
		{
				m_pGodLevelPic[i]->SetInvisible(TRUE);
		}
	}
	else
	{
		for (int i = 0; i < MAX_GOD_LEVEL; i++)
		{
			if (i < nCurrLocalplayerGodLevel)
				m_pGodLevelPic[i]->SetInvisible(FALSE);
			else
				m_pGodLevelPic[i]->SetInvisible(TRUE);
		}
	}

	// 刷新凝聚的
	

	const tagCondensedProto* pCondensedProto = CondensedProtoData::Inst()->FindCondensedProto(pLp->GetAttribute(ERA_God_Godhead)+1);
	if (P_VALID(pCondensedProto))
	{
		int nValue = pCondensedProto->dwAgglomerate_val;
		TCHAR szTmp2[X_SHORT_NAME] = {0};
		_sntprintf(szTmp2, sizeof(szTmp2)/sizeof(TCHAR), _T("%d/%d"), pLp->GetAttribute(ERA_God_Condenced),nValue);
		float fProValue = (float)pLp->GetAttribute(ERA_God_Condenced)/(float)nValue;
		m_pStAgglomerate->SetValue(pLp->GetAttribute(ERA_God_Condenced));
		m_pStAgglomerate->SetMaxValue(nValue);
		m_pStAgglomerate->SetSpeed(10000000.0f);
		m_pStAgglomerate->SetPicColor(0x00000000); 
		m_pStAgglomerate->SetForePicColor(0xFF00A2FF);
		m_pStAgglomerateText->SetText(szTmp2);
	}

	

	// 刷新信仰
	TCHAR zBuff[X_SHORT_NAME];
	_sntprintf(zBuff,sizeof(zBuff)/sizeof(TCHAR),_T("%d"),pLp->GetAttribute(ERA_God_Faith));
	m_pStXinYang->SetText(zBuff);

	const tagClergyName* pProto = ClergyProtoData::Inst()->FindClergyProto(pLp->GetPlayerClergy());
	if(!P_VALID(pProto))
	{
		m_pCurrGodJobPic->SetInvisible(TRUE);
		return;
	}
	else
	{
		DWORD dwGodID = pLp->GetPlayerClergy();
		if (0 == dwGodID || GT_INVALID == dwGodID)
			return;

		TCHAR szPath[512];
		_stprintf(szPath,_T("data\\ui\\Main\\s_%02d.bmp"),dwGodID);
		m_pCurrGodJobPic->SetPic(szPath);
		m_pCurrGodJobPic->SetInvisible(FALSE);	
	}
}

void DomainAbilityFrame::HideAllGodLevelPic(bool bHide)
{
	for (int i =0;i < DOMAIN_ROW_NUM; i++)
	{
		m_pStGodLevel[i].pBack->SetInvisible(bHide);
	}
}

DWORD DomainAbilityFrame::OnNS_GetRoleInitState_Area(tagNS_GetRoleInitState_Area* pNetCmd,DWORD)
{
	m_mmapFreePickDomain.clear();
	m_vecRareDomain.clear();
	for (int i=0;i<pNetCmd->nNum; i++)
	{
		tagAreaMsgInfo data = pNetCmd->Area[i];

		const tagGodAreaName* pProto = GodMgr::Inst()->GetAreaDataByID(data.dwAreaID);
		if(!P_VALID(pProto))	continue;

		if ( EGAT_OPTIONAL == pProto->eType)
		{
			DomianData freedata;
			freedata.bActive = data.bActivation;
			freedata.dwSkillID = data.dwAreaID;
			m_mmapFreePickDomain.insert(make_pair(pProto->nGodheadLimit,freedata));

		}
		else if (EGAT_RARE == pProto->eType)
		{
			DomianData ratedata;
			ratedata.bActive = data.bActivation;
			ratedata.dwSkillID = data.dwAreaID;
			ratedata.dwTime = data.dwTime;
			m_vecRareDomain.push_back(ratedata);
		}
	}

	RefreshUI();
	return 0;
}

void DomainAbilityFrame::BuildGodDomains()
{
		m_vecGodDomain.clear();
		LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
		if(!P_VALID(pLp))	return;

		const tagClergyName* pProto = ClergyProtoData::Inst()->FindClergyProto(pLp->GetPlayerClergy());
		if(!P_VALID(pProto))	return;

		for (int i =0; i < pProto->n8FieldNum; i++)
		{
			if (pProto->dwFieldID[i]	!=0  &&pProto->dwFieldID[i]	!=GT_INVALID )
			{
				m_vecGodDomain.push_back(pProto->dwFieldID[i]);
			}
		}
}

DWORD DomainAbilityFrame::OnUpdateLPAttEvent(tagUpdateLPAttEvent* pEvent)
{
	if ( ERA_God_Godhead == pEvent->eType )
	{
		m_vecGodDomain.clear();
		BuildGodDomains();
		if (ED_God == m_eCurrDomain)
		{
			RefreshGodDomain();
			RefreshPage();
		}
		RefreshPlayerInfo();
	}
	else if (ERA_God_Condenced == pEvent->eType || ERA_God_Faith == pEvent->eType )
	{
		RefreshPlayerInfo();
	}
	
	return 0;
}

DWORD DomainAbilityFrame::OnNS_AddArea(tagNS_AddArea* pNetCmd,DWORD)
{
	if (E_Success ==  pNetCmd->dwErrorCode)
	{
		const tagGodAreaName* pProto = GodMgr::Inst()->GetAreaDataByID(pNetCmd->dwAreaID);
		if(!P_VALID(pProto))	return 0;

		if ( EGAT_OPTIONAL == pProto->eType)
		{
			if (pNetCmd->bAdd)
			{
				DomianData freedata;
				freedata.bActive = false;
				freedata.dwSkillID = pNetCmd->dwAreaID;
				m_mmapFreePickDomain.insert(make_pair(pProto->nGodheadLimit,freedata));


				TCHAR zBuff[X_LONG_NAME];
				_sntprintf(zBuff,sizeof(zBuff)/sizeof(TCHAR),g_StrTable[_T("AbilityGet")],pProto->szName);
				CombatSysUtil::Inst()->ShowScreenCenterMsg(zBuff);
			}
			else
			{
				for (multimap<int,DomianData>::iterator it = m_mmapFreePickDomain.begin();
					it != m_mmapFreePickDomain.end(); ++it)
				{
					if (it->second.dwSkillID == pNetCmd->dwAreaID)
					{
						m_mmapFreePickDomain.erase(it);
						break;
					}
				}	
			}
			

			if (ED_FreePick == m_eCurrDomain   || ED_Enabled  ==  m_eCurrDomain)
			{
				RefreshUI();
			}

		}
		else if (EGAT_RARE == pProto->eType)
		{
			if (pNetCmd->bAdd)
			{
				DomianData ratedata;
				ratedata.bActive = false;
				ratedata.dwSkillID = pNetCmd->dwAreaID;
				ratedata.dwTime = pNetCmd->dwTime;
				m_vecRareDomain.push_back(ratedata);

				TCHAR zBuff[X_LONG_NAME];
				_sntprintf(zBuff,sizeof(zBuff)/sizeof(TCHAR),g_StrTable[_T("AbilityGet")],pProto->szName);
				CombatSysUtil::Inst()->ShowScreenCenterMsg(zBuff);
			}
			else
			{
				for (vector<DomianData>::iterator it = m_vecRareDomain.begin();
					it != m_vecRareDomain.end();++it)
				{
					if (it->dwSkillID == pNetCmd->dwAreaID)
					{
						m_vecRareDomain.erase(it);
						break;
					}
				}
			}

			if (ED_Rare == m_eCurrDomain   || ED_Enabled  ==  m_eCurrDomain)
			{
				RefreshUI();
			}
		}

		RefReshDesc(m_eCurrDomain);
	}
	else
	{
		switch (pNetCmd->dwErrorCode)
		{
		case E_God_Area_None:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("addE_God_Area_None")]);
			break;
		case E_God_Area_Max:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("addE_God_Area_Max")]);
			break;
		case E_God_Area_Condition:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("addE_God_Area_Condition")]);
			break;
		default:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("addAbilityActive_failed")]);
			break;
		}
	}
	return 0;
}

DWORD DomainAbilityFrame::OnNS_ActivationArea(tagNS_ActivationArea* pNetCmd,DWORD)
{
	if (E_Success ==  pNetCmd->dwErrorCode)
	{
		const tagGodAreaName* pProto = GodMgr::Inst()->GetAreaDataByID(pNetCmd->dwAreaID);
		if(!P_VALID(pProto))	return 0;

		if ( EGAT_OPTIONAL == pProto->eType)
		{
			multimap<int,DomianData>::iterator itBegin, itEnd;
			itBegin = m_mmapFreePickDomain.lower_bound(pProto->nGodheadLimit);
			itEnd   = m_mmapFreePickDomain.upper_bound(pProto->nGodheadLimit);
			while(itBegin != itEnd)
			{
				if(itBegin->second.dwSkillID == pNetCmd->dwAreaID)
				{
					itBegin->second.bActive = pNetCmd->bActive;
					break;
				}

				++itBegin;
			}

			if (ED_FreePick == m_eCurrDomain   || ED_Enabled  ==  m_eCurrDomain)
			{
				//RefreshUI();
			}
			
		}
		else if (EGAT_RARE == pProto->eType)
		{
			for (vector<DomianData>::iterator it = m_vecRareDomain.begin();
				it != m_vecRareDomain.end();++it)
			{
				if (it->dwSkillID == pNetCmd->dwAreaID)
				{
					it->bActive = pNetCmd->bActive;
					break;
				}
			}

			if (ED_Rare == m_eCurrDomain   || ED_Enabled  ==  m_eCurrDomain)
			{
				//RefreshUI();
			}
		}
	}
	else
	{
		switch (pNetCmd->dwErrorCode)
		{
		case E_God_Area_None:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_God_Area_None")]);
			break;
		case E_God_Area_Max:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_God_Area_Max")]);
			break;
		case E_God_Area_Condition:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_God_Area_Condition")]);
			break;
		case E_God_Area_CD:
			{
				tstringstream ss;
				if(pNetCmd->dwCD >0)
				{
					DWORD time = pNetCmd->dwCD;
					DWORD hour = time/60/60;
					DWORD min = (time/60)%60;
					DWORD second = time%60;
					if(hour) ss << hour << g_StrTable[_T("AreaTips_Hour")];
					if(min)	ss << min << g_StrTable[_T("AreaTips_Min")];
					if(second)	ss << second << g_StrTable[_T("AreaTips_Second")];
					TCHAR zBuff[X_LONG_NAME];
					if(pNetCmd->bActive)
						_sntprintf(zBuff,sizeof(zBuff)/sizeof(TCHAR),g_StrTable[_T("E_God_Area_CDUnactive")],ss.str().c_str());
					else
						_sntprintf(zBuff,sizeof(zBuff)/sizeof(TCHAR),g_StrTable[_T("E_God_Area_CD")],ss.str().c_str());
					CombatSysUtil::Inst()->ShowScreenCenterMsg(zBuff);
				}
			}
			break;
		case E_God_Area_Career:
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_God_Area_Career")]);
			}
			break;
		default:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("AbilityActive_failed")]);
			break;
		}
	}

	for (int i =0;i < DOMAIN_ROW_NUM ;i++)
	{
		for (int j =0;j < DOMAIN_COLUNM_NUM; j++)
		{
			if (m_abilityButton[i][j].dwSkillID == pNetCmd->dwAreaID)
			{
				m_abilityButton[i][j].pPbSelected->SetState(pNetCmd->bActive?EGUIBS_PushDown : EGUIBS_PopUp ,TRUE,FALSE);
				m_abilityButton[i][j].pPbSelected->SetEnable(TRUE);
			}
		}
	}

	RefReshDesc(m_eCurrDomain);
	return 0;
}
DWORD DomainAbilityFrame::OnOpenEvent(tagGameEvent* pEvent)
{
	OpenFrame();
	return 0;
}
DWORD DomainAbilityFrame::OnCloseEvent(tagGameEvent* pEvent)
{
	CloseFrame();
	return 0;
}

VOID DomainAbilityFrame::Update()
{
	m_dwUpdateTime += Kernel::Inst()->GetDeltaTimeDW();
	if (m_dwUpdateTime > 1000 )
	{
		for (vector<DomianData>::iterator iter = m_vecRareDomain.begin();
			iter != m_vecRareDomain.end(); ++iter)
		{
			if(iter->dwTime)
				iter->dwTime--;
		}
		m_dwUpdateTime =0;
	}
}

void DomainAbilityFrame::EnableAllPushButtons(BOOL bEnable)
{
	for (int i =0;i < DOMAIN_ROW_NUM ;i++)
	{
		for (int j =0;j < DOMAIN_COLUNM_NUM; j++)
		{
			m_abilityButton[i][j].pPbSelected->SetEnable(bEnable);
		}
	}
}

int DomainAbilityFrame::GetCurAcitveAbilityNum(int& nFreeNum, int& nRateNum)
{
	for (multimap<int,DomianData>::iterator it = m_mmapFreePickDomain.begin();
		it != m_mmapFreePickDomain.end(); ++it)
	{
		if(it->second.bActive)
			nFreeNum++;
	}

	for (vector<DomianData>::iterator iter = m_vecRareDomain.begin();
		iter != m_vecRareDomain.end(); ++iter)
	{
		if((*iter).bActive)
			nRateNum++;
	}
	
	return nFreeNum + nRateNum;
}

void DomainAbilityFrame::RefReshDesc(EDomain eDomain)
{
	TCHAR szBuff[X_SHORT_NAME];
	if ( ED_God ==  eDomain)
	{
		m_sDesc[E_MidWord].pWord->SetText(g_StrTable[_T("AbilityWordGodNum")]);
		_sntprintf(szBuff, sizeof(szBuff)/sizeof(TCHAR), _T("%d"),m_vecGodDomain.size());
		m_sDesc[E_MidWord].pNum->SetText(szBuff);

		m_sDesc[E_LeftWord].pWord->SetInvisible(TRUE);
		m_sDesc[E_RightWord].pWord->SetInvisible(TRUE);
		m_sDesc[E_MidWord].pWord->SetInvisible(FALSE);

		m_pStexDesc->SetText(g_StrTable[_T("AbilityDesc_God")]);
	}
	else if (ED_FreePick ==  eDomain || ED_Rare ==eDomain )
	{
		
		LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
		if(!P_VALID(pLp))	return;
		int tempNum = 0;
		const tagCondensedProto* pCondensedProto = CondensedProtoData::Inst()->FindCondensedProto(pLp->GetAttribute(ERA_God_Godhead));
		if (P_VALID(pCondensedProto))
			tempNum = pCondensedProto->nCanActNum;
		int nFree=0, nRate =0;
		m_sDesc[E_LeftWord].pWord->SetText(g_StrTable[_T("AbilityWordActiveNum")]);
		_sntprintf(szBuff, sizeof(szBuff)/sizeof(TCHAR), _T("%d/%d"),GetCurAcitveAbilityNum(nFree,nRate),tempNum );
		m_sDesc[E_LeftWord].pNum->SetText(szBuff);

		m_sDesc[E_RightWord].pWord->SetText(g_StrTable[_T("AbilityWordFreePickNum")]);
		_sntprintf(szBuff, sizeof(szBuff)/sizeof(TCHAR), _T("%d"),nFree);
		m_sDesc[E_RightWord].pNum->SetText(szBuff);

		m_sDesc[E_MidWord].pWord->SetText(g_StrTable[_T("AbilityWordRateNum")]);
		_sntprintf(szBuff, sizeof(szBuff)/sizeof(TCHAR), _T("%d"),nRate);
		m_sDesc[E_MidWord].pNum->SetText(szBuff);

		m_sDesc[E_LeftWord].pWord->SetInvisible(FALSE);
		m_sDesc[E_RightWord].pWord->SetInvisible(FALSE);
		m_sDesc[E_MidWord].pWord->SetInvisible(FALSE);

		if(ED_FreePick ==  eDomain)
			m_pStexDesc->SetText(g_StrTable[_T("AbilityDesc_FreePick")]);
		else
			m_pStexDesc->SetText(g_StrTable[_T("AbilityDesc_Rate")]);
	}
	else if (ED_Enabled ==  eDomain)
	{
		int nFree=0, nRate =0;
		GetCurAcitveAbilityNum(nFree,nRate);
		m_sDesc[E_LeftWord].pWord->SetText(g_StrTable[_T("AbilityWordGodNum")]);
		_sntprintf(szBuff, sizeof(szBuff)/sizeof(TCHAR), _T("%d"),m_vecGodDomain.size());
		m_sDesc[E_LeftWord].pNum->SetText(szBuff);

		m_sDesc[E_RightWord].pWord->SetText(g_StrTable[_T("AbilityWordFreePickNum")]);
		_sntprintf(szBuff, sizeof(szBuff)/sizeof(TCHAR), _T("%d/%d"),nFree,m_mmapFreePickDomain.size());
		m_sDesc[E_RightWord].pNum->SetText(szBuff);

		m_sDesc[E_MidWord].pWord->SetText(g_StrTable[_T("AbilityWordRateNum")]);
		_sntprintf(szBuff, sizeof(szBuff)/sizeof(TCHAR), _T("%d/%d"),nRate,m_vecRareDomain.size());
		m_sDesc[E_MidWord].pNum->SetText(szBuff);

		m_sDesc[E_LeftWord].pWord->SetInvisible(FALSE);
		m_sDesc[E_RightWord].pWord->SetInvisible(FALSE);
		m_sDesc[E_MidWord].pWord->SetInvisible(FALSE);

		m_pStexDesc->SetText(g_StrTable[_T("AbilityDesc_All")]);
	}

	tagPoint size = m_pStexDesc->GetSize();
	if( m_pStexDesc->GetTextTotalHeight() > size.y )
	{
		size.y = m_pStexDesc->GetTextTotalHeight();
		m_pStexDesc->SetResizable(TRUE);
		m_pStexDesc->Resize( size );
		m_pStexDesc->SetResizable(FALSE);
	}
	m_pStexDesc->SetRefresh(true);
}

VOID DomainAbilityFrame::OpenFrame()
{
	m_pWnd->SetInvisible(FALSE);
	if(!m_bOpened)
	{
		m_eCurrDomain = ED_God;
		m_nCurrPage =0;
		RefreshUI();
		m_bOpened = true;
	}
}

DWORD DomainAbilityFrame::OnNS_LoseArea(tagNS_RareAreaInfo* pNetCmd,DWORD)
{
	if (pNetCmd->bSuccess)
	{
		const tagGodAreaName* pProto = GodMgr::Inst()->GetAreaDataByID(pNetCmd->dwAreaID);
		if(!P_VALID(pProto))	return 0;

		LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
		if(!P_VALID(plp))		return 0;

		TCHAR zBuff[X_LONG_NAME];
		if (plp->GetID() == pNetCmd->dwRobRoleID)
		{
			_sntprintf(zBuff,sizeof(zBuff)/sizeof(TCHAR),g_StrTable[_T("AbilityGet")],pProto->szName);
			CombatSysUtil::Inst()->ShowScreenCenterMsg(zBuff);
		}
		else if (plp->GetID() == pNetCmd->dwRoleID)
		{
			_sntprintf(zBuff,sizeof(zBuff)/sizeof(TCHAR),g_StrTable[_T("AbilityLost")],pProto->szName,pNetCmd->szRoalName);
			CombatSysUtil::Inst()->ShowScreenCenterMsg(zBuff);
		}
	}
	else
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("addAbilityActive_failed")]);
	}

	RefReshDesc(m_eCurrDomain);
	return 0;
}

DWORD DomainAbilityFrame::OnNS_PromotionClergy(tagNS_PromotionClergy* pCmd, DWORD)
{
	if (E_Success == pCmd->dwErrorCode )
	{
		if (ED_God == m_eCurrDomain)
		{
			RefreshUI();
		}
	}
	return 0;
}

DWORD DomainAbilityFrame::OnNS_ChangeClergy(tagNS_ChangeClergy* pCmd, DWORD)
{
	LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
	if (P_VALID(plp) && pCmd->dwRoleID == plp->GetID())
	{
		if (ED_God == m_eCurrDomain )
		{
			RefreshUI();
		}
	}
	return 0;
}

DWORD DomainAbilityFrame::OnGameEvent(tagGameEvent* pGameEvent)
{
	tagUpdateLPAttEvent* pEvent = (tagUpdateLPAttEvent*)pGameEvent;
	if ( ERA_God_Godhead ==  pEvent->eType)
	{
		RefreshUI();
	}
	return 0;
}