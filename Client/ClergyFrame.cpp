#include "stdafx.h"
#include "ClergyFrame.h"
#include "IconStatic.h"
#include "..\WorldDefine\clergy_define.h"
#include "ClergyProtoData.h"
#include "RoleMgr.h"
#include "LocalPlayer.h"
#include "..\WorldDefine\msg_god.h"
#include "SkillMgr.h"
#include "CombatSysUtil.h"
#include "QuestMgr.h"
#include "GodMgr.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "Item.h"
#include "ItemMgr.h"

ClergyFrame::ClergyFrame()
:m_Trunk(this)
,m_pWnd(NULL)
,m_pWndCaption(NULL)
,m_pStcGodHeadLimit(NULL)			//神格限制
,m_pStcSexLimit(NULL)				//性别限制
,m_pStcAttLimit(NULL)				//属性限制
,m_pStcLevelLimit(NULL)				//级别限制
,m_pStcClassLimit(NULL)				//职业限制
,m_pStcAttValue(NULL)				//属性数值
,m_pIStcGodIcon(NULL)				//图标
,m_pBtnUp(NULL)						//上一个
,m_pBtnDown(NULL)					//下一个
,m_pStcClergyText(NULL)				//神职文本(上位神=)
,m_pStcClergyName(NULL)				//神职名称
,m_pStcHasReached(NULL)				//是否满足条件
,m_pStcDesc(NULL)					//描述
,m_pBtnClose(NULL)					//关闭
,m_pBtnPromotion(NULL)				//晋升按钮
,m_dwClergyID(GT_INVALID)			//神职ID
,m_bIsReach(TRUE)
,m_pLocalPlayer(NULL)
,m_escapeTime(0)
,m_bIsTransform(FALSE)
{
	for(int i = 0; i < ERA_End; i++)
	{
		m_dwRoleAtt[i] = GT_INVALID;
	}
}
ClergyFrame::~ClergyFrame()
{

}

BOOL ClergyFrame::Init(GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam)
{
	if (!GameFrame::Init(pMgr, pFather, dwParam))
		return FALSE;

	m_pFrameMgr->RegisterEventHandle(_T("OpenClergyFrame"),         (FRAMEEVENTPROC)m_Trunk.sfp1( &ClergyFrame::OnEventOpenClergyFrame));
	m_pFrameMgr->RegisterEventHandle(_T("LocalPlayerTransform"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&ClergyFrame::OnGameEvent));
	m_pFrameMgr->RegisterEventHandle(_T("LocalPlayerUnTransform"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&ClergyFrame::OnGameEvent));

	m_pNetCmdMgr->Register("NS_GetAttribute",	            (NETMSGPROC)m_Trunk.sfp2(&ClergyFrame::OnNS_GetAttribute),           _T("NS_GetAttribute"));
	m_pNetCmdMgr->Register("NS_PromotionClergy",			(NETMSGPROC)m_Trunk.sfp2(&ClergyFrame::OnNS_PromotionClergy),		 _T("NS_PromotionClergy"));
	return TRUE;
}

BOOL ClergyFrame::EscCancel()
{
	if(!m_pWnd->IsInvisible())
	{
		m_pWnd->SetInvisible(TRUE);
		return TRUE;
	}
	return FALSE;
}

BOOL ClergyFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();
	if(P_VALID(m_pWnd))
	{
		m_pGUI->AddToDestroyList(m_pWnd);
		m_pWnd = NULL;
	}

	m_pFrameMgr->UnRegisterEventHandler(_T("OpenClergyFrame"),          (FRAMEEVENTPROC)m_Trunk.sfp1(&ClergyFrame::OnEventOpenClergyFrame));
	m_pFrameMgr->UnRegisterEventHandler(_T("LocalPlayerTransform"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&ClergyFrame::OnGameEvent));
	m_pFrameMgr->UnRegisterEventHandler(_T("LocalPlayerUnTransform"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&ClergyFrame::OnGameEvent));

	m_pNetCmdMgr->UnRegister("NS_GetAttribute",	        (NETMSGPROC)m_Trunk.sfp2(&ClergyFrame::OnNS_GetAttribute));
	m_pNetCmdMgr->UnRegister("NS_PromotionClergy",		(NETMSGPROC)m_Trunk.sfp2(&ClergyFrame::OnNS_PromotionClergy));

	return bRet;
}

BOOL ClergyFrame::ReloadUI()
{
	TCHAR szText[256] = {0};
	char temp[256] = {0};
	XmlElement ele;
	tstring strParth = g_strLocalPath + _T("\\ui\\god_job.xml");
	m_pGUI->LoadXml(&ele,"VFS_System",strParth.c_str());

	m_pGUI->ChangeXml(&ele,"godjob_win\\godjob_back\\jobpic","ClassName","IconStatic"); //change
	for(int i = 0; i < MAX_FieldNum; i++)
	{
		sprintf(temp, "godjob_win\\godjob_back\\ability%d", i);
		m_pGUI->ChangeXml(&ele, temp, "ClassName", "IconStatic");
	}

	m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"),&ele);
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(),m_Trunk.sfp1(&ClergyFrame::EventHandler));
	m_pWndCaption		= m_pWnd->GetChild(_T("godjob_back"));
	m_pStcGodHeadLimit	= (GUIStaticEx*)m_pWndCaption->GetChild(_T("word0"));				//神格限制
	m_pStcSexLimit		= (GUIStaticEx*)m_pWndCaption->GetChild(_T("word2"));				//性别限制
	m_pStcAttLimit		= (GUIStaticEx*)m_pWndCaption->GetChild(_T("word4"));				//属性限制
	m_pStcLevelLimit	= (GUIStaticEx*)m_pWndCaption->GetChild(_T("word1"));				//级别限制
	m_pStcClassLimit	= (GUIStaticEx*)m_pWndCaption->GetChild(_T("word3"));				//职业限制
	m_pStcAttValue		= (GUIStaticEx*)m_pWndCaption->GetChild(_T("word5"));				//属性数值
	m_pIStcGodIcon		= (IconStatic*)m_pWndCaption->GetChild(_T("jobpic"));				//图标
	m_pBtnUp			= (GUIButton*)m_pWndCaption->GetChild(_T("backbutt"));				//上一个
	m_pBtnDown			= (GUIButton*)m_pWndCaption->GetChild(_T("nextbutt"));				//下一个
	m_pStcClergyText	= (GUIStatic*)m_pWndCaption->GetChild(_T("godtype"));				//神职文本(上位神=)
	m_pStcClergyName	= (GUIStatic*)m_pWndCaption->GetChild(_T("godname"));				//神职名称
	m_pStcHasReached	= (GUIStatic*)m_pWndCaption->GetChild(_T("word"));					//是否满足条件
	m_pStcDesc			= (GUIStatic*)m_pWndCaption->GetChild(_T("intro_win\\introword"));	//描述
	m_pBtnClose			= (GUIButton*)m_pWndCaption->GetChild(_T("closebutton"));			//关闭
	m_pBtnPromotion		= (GUIButton*)m_pWndCaption->GetChild(_T("getbutt"));				//晋升按钮

	for(int i = 0; i < MAX_FieldNum; i++)
	{
		_stprintf(szText, _T("ability%d"), i);
		m_pIStcFieldIcon[i] = (IconStatic*)m_pWndCaption->GetChild(szText);
	}

	m_pWnd->SetInvisible(TRUE);

	//Show();

	m_pLocalPlayer = RoleMgr::Inst()->GetLocalPlayer();

	m_dwMapNum = ClergyProtoData::Inst()->GetNum();

	return TRUE;
}
VOID ClergyFrame::Update()
{
	if(m_pWnd->IsInvisible())
		return ;
	m_escapeTime += Kernel::Inst()->GetDeltaTimeDW();
	if( m_escapeTime > 1000 )//每隔1秒检测距离目标NPC的距离
	{
		m_escapeTime = 0;

		if (RoleMgr::Inst()->IsOutValidDist( QuestMgr::Inst()->GetTalkNPCGlobalID() ) )
		{
			Hide();
		}
	}
	GameFrame::Update();
}

BOOL ClergyFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	if(!P_VALID(pWnd))
		return FALSE;
	switch (pEvent->eEvent)
	{
	case EGUIE_Click:
		if(pWnd == m_pBtnUp)
		{
		 	m_dwClergyID--;
			if(m_dwClergyID <= 0)
				m_dwClergyID = m_dwMapNum;
			RefrashInfo(m_dwClergyID);
		}
		else if(pWnd == m_pBtnDown)
		{
			m_dwClergyID++;
			if(m_dwClergyID > m_dwMapNum || m_dwClergyID == 0)
				m_dwClergyID = 1;
			RefrashInfo(m_dwClergyID);
		}
		else if(pWnd == m_pBtnPromotion)
		{
			if(pClergyInfo->ePromotionType == EPTT_Campaign)
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("NotDirect")]);
				return FALSE;
			}
			if(!m_bIsReach)
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("ConditionsNotMeet")]);
				return FALSE;
			}
			if(m_dwClergyID == m_pLocalPlayer->GetPlayerClergy())
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("HasGotIt")]);
				return FALSE;
			}
			if(m_bIsTransform)
			{
				CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("TransformLimit")]);
				return FALSE;
			}
			//客户端判断是否含有限时装备
			for(int i = EEP_Equip_Start; i <  EEP_Equip_End; i++)
			{
				Equipment* pEquip = ItemMgr::Inst()->GetCurEquip((EEquipPos)i);
				if(!P_VALID(pEquip))
					continue;

				if(((tagEquipProto*)(pEquip->GetProto()))->dwTimeLimit != GT_INVALID)
				{
					CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipLimit")]);
					return FALSE;
				}
			}


			tagNC_PromotionClergy cmd;
			cmd.dwClergy = m_dwClergyID;
			m_pNetSession->Send(&cmd);
		}
		else if(pWnd == m_pBtnClose)
		{
			Hide();
		}
		break;
	case EGUIE_MoveIntoCtrl:
			{
				for (int i=0 ;i <MAX_FieldNum; i++)
				{
					if (pWnd == m_pIStcFieldIcon[i])
					{
						if (P_VALID(pClergyInfo) && P_VALID(pClergyInfo->dwFieldID[i]))
						{
							const tagGodAreaName* pSkillProto = GodMgr::Inst()->GetAreaDataByID(pClergyInfo->dwFieldID[i]);
							if (!P_VALID(pSkillProto))	break;
							tstring strTip = ToolTipCreator::Inst()->GetAreaShowTips(pSkillProto->dwID);	
							ShowTip(pWnd, strTip.c_str());
						}
					}
				}
			}
			break;
	default:break;
	}
	return FALSE;
}

VOID ClergyFrame::RefrashInfo(DWORD dwID)
{
	if(dwID != GT_INVALID)
		m_dwClergyID = dwID;
	pClergyInfo = ClergyProtoData::Inst()->FindClergyProto(m_dwClergyID);
	if(!P_VALID(pClergyInfo))
	{	
		m_dwClergyID = 1;
		pClergyInfo = ClergyProtoData::Inst()->FindClergyProto(1);
	}
	//调用清除函数（主要还原控件文字颜色）
	ClearInfo(); 

	TCHAR szTemp[X_LONG_NAME] = {0};
	int nTemp = 0;

	//神格限制
	nTemp = m_pLocalPlayer->GetAttribute(ERA_God_Godhead);
	if(nTemp >= pClergyInfo->n8ClergyLevel)
	{
		//_stprintf(szTemp, _T("<color=%x>%d<color=%x>/%d"), dwGreenColor, nTemp, dwWhiteColor, pClergyInfo->n8ClergyLevel );
		m_pStcGodHeadLimit->SetTextColor(dwGreenColor);
		_stprintf(szTemp, _T("%d/%d"), nTemp, pClergyInfo->n8ClergyLevel );
	}
	else
	{
		//_stprintf(szTemp, _T("<color=%x>%d<color=%x>/%d"), dwRedColor, nTemp, dwWhiteColor, pClergyInfo->n8ClergyLevel );
		m_pStcGodHeadLimit->SetTextColor(dwRedColor);
		_stprintf(szTemp, _T("%d/%d"), nTemp, pClergyInfo->n8ClergyLevel );
		m_bIsReach = FALSE;
	}
	m_pStcGodHeadLimit->SetText(szTemp);

	//性别
	if(pClergyInfo->eSexLimit != GT_INVALID)
	{
		if(m_pLocalPlayer->GetPlayerSex() != pClergyInfo->eSexLimit)
		{
			m_pStcSexLimit->SetTextColor(dwRedColor);
			m_bIsReach = FALSE;
		}
		_stprintf(szTemp, _T("Sex%d"), pClergyInfo->eSexLimit);
		m_pStcSexLimit->SetText(g_StrTable[szTemp]);
	}
	else
	{
		m_pStcSexLimit->SetText(g_StrTable[_T("Sex2")]);
	}

	//属性
	if(pClergyInfo->eAttType == GT_INVALID)
		m_pStcAttLimit->SetText(_T(""));
	else
	{
		_stprintf(szTemp, _T("ERoleAttribute_%d"), pClergyInfo->eAttType);
		m_pStcAttLimit->SetText(g_StrTable[szTemp]);

		if(m_dwRoleAtt[pClergyInfo->eAttType] != GT_INVALID)
		{
			//属性数值
			if(pClergyInfo->dwAttValue <= pClergyInfo->eAttType)
			{
				_stprintf(szTemp, _T("%d/%d"), pClergyInfo->eAttType, pClergyInfo->dwAttValue);
				m_pStcAttValue->SetTextColor(dwGreenColor);
			}
			else
			{
				_stprintf(szTemp, _T("%d/%d"), pClergyInfo->eAttType, pClergyInfo->dwAttValue);
				m_pStcAttValue->SetTextColor(dwRedColor);
				m_bIsReach = FALSE;
			}
			m_pStcAttValue->SetText(szTemp);
			SetIsObtain();
		}
		else
		{
			tagNC_GetAttribute cmd;
			cmd.eType = pClergyInfo->eAttType;
			m_pNetSession->Send(&cmd);

			LockButt();
		}
		
	}

	//级别
	nTemp = m_pLocalPlayer->GetRoleLevel();
	if( nTemp>= pClergyInfo->nLevelLimit)
	{
		//_stprintf(szTemp, _T("<color=%x>%d<color=%x>/%d"), dwGreenColor, nTemp, dwWhiteColor, pClergyInfo->nLevelLimit);
		_stprintf(szTemp, _T("%d/%d"), nTemp, pClergyInfo->nLevelLimit);
		m_pStcLevelLimit->SetTextColor(dwGreenColor);
	}
	else
	{
		//_stprintf(szTemp, _T("<color=%x>%d<color=%x>/%d"), dwRedColor, nTemp, dwWhiteColor, pClergyInfo->nLevelLimit);
		_stprintf(szTemp, _T("%d/%d"), nTemp, pClergyInfo->nLevelLimit);
		m_pStcLevelLimit->SetTextColor(dwRedColor);
		m_bIsReach = FALSE;
	}
	m_pStcLevelLimit->SetText(szTemp);	

	//职业
	DWORD dwClassVal;
	GetClassEnumBinary(m_pLocalPlayer->GetPlayerClass(), dwClassVal);
	if(!(dwClassVal & pClergyInfo->dwClassLimit))
	{
		m_pStcClassLimit->SetTextColor(dwRedColor);
		m_bIsReach = FALSE;
	}
	switch(pClergyInfo->dwClassLimit)
	{
	case 8191:	//无
		dwClassVal = 0;
		break;
	case 98:	//神兵
		dwClassVal = 2;
		break;
	case 388:	//玄翎
		dwClassVal = 3;
		break;
	case 1544:	//乾坤
		dwClassVal = 4;
		break;
	case 6160:	//仙音
		dwClassVal = 5;
		break;
	default:dwClassVal = 0;
	}
	if(dwClassVal == 0)
	{
		m_pStcClassLimit->SetText(g_StrTable[_T("NULLText")]);
	}
	else
	{
		_stprintf(szTemp, _T("ClassName_%d"), dwClassVal);
		m_pStcClassLimit->SetText(g_StrTable[szTemp]);
	}
		

	//图标
	tstring szClergyIcon = GetClergyIconPath(m_dwClergyID);
	m_pIStcGodIcon->SetPic(szClergyIcon.c_str());	
	TCHAR szText[128] = {0};

	for(int i = 0; i < pClergyInfo->n8FieldNum; i++)
	{
		const tagGodAreaName* pName = GodMgr::Inst()->GetAreaDataByID(pClergyInfo->dwFieldID[i]);
		_stprintf(szText, _T("data\\ui\\Icon\\%s.tga"), pName->szIcon);
		m_pIStcFieldIcon[i]->SetPic(szText);
		m_pIStcFieldIcon[i]->SetInvisible(FALSE);
	}

	//神职文本
	m_pStcClergyText->SetText(pClergyInfo->szClergyText);	

	//神职名称
	m_pStcClergyName->SetText(pClergyInfo->szName);	

	//m_pStcHasReached;			//是否满足条件

	//描述
	m_pStcDesc->SetText(pClergyInfo->szText);	

	SetIsObtain();
}

VOID ClergyFrame::ClearInfo()
{
	m_pStcGodHeadLimit->SetTextColor(dwWhiteColor);
	m_pStcSexLimit->SetTextColor(dwWhiteColor);	
	m_pStcAttLimit->SetTextColor(dwWhiteColor);	
	m_pStcLevelLimit->SetTextColor(dwWhiteColor);	
	m_pStcClassLimit->SetTextColor(dwWhiteColor);	
	m_pStcAttValue->SetTextColor(dwWhiteColor);	

	for(int i = 0; i < MAX_FieldNum; i++)
	{
		m_pIStcFieldIcon[i]->SetInvisible(TRUE);
	}

	m_pStcAttValue->SetText(_T(""));
	m_bIsReach = TRUE;
}

VOID ClergyFrame::Show()
{
	if(!P_VALID(m_pLocalPlayer))
		return;
	if(m_pLocalPlayer->GetPlayerClergy() == 0 || m_pLocalPlayer->GetPlayerClergy() == GT_INVALID)
		RefrashInfo(1);
	else
		RefrashInfo(m_pLocalPlayer->GetPlayerClergy());
	m_pWnd->SetInvisible(FALSE);
}

VOID ClergyFrame::Hide()
{
	m_pWnd->SetInvisible(TRUE);

	for(int i = 0; i < ERA_End; i++)
	{
		m_dwRoleAtt[i] = GT_INVALID;
	}
}

DWORD ClergyFrame::OnNS_GetAttribute(tagNS_GetAttribute* pCmd, DWORD)
{
	TCHAR szTemp[X_LONG_NAME] = {0};
	//属性数值
	if(pClergyInfo->dwAttValue <= pCmd->dwValue)
	{
		_stprintf(szTemp, _T("%d/%d"), pCmd->dwValue, pClergyInfo->dwAttValue);
		m_pStcAttValue->SetTextColor(dwGreenColor);
	}
	else
	{
		_stprintf(szTemp, _T("%d/%d"), pCmd->dwValue, pClergyInfo->dwAttValue);
		m_pStcAttValue->SetTextColor(dwRedColor);
		m_bIsReach = FALSE;
	}
	m_pStcAttValue->SetText(szTemp);
	SetIsObtain();

	UnLockButt();

	return 0;
}

DWORD ClergyFrame::OnNS_PromotionClergy(tagNS_PromotionClergy* pCmd, DWORD)
{
	if(pCmd->dwErrorCode != E_Success)
	{
		switch(pCmd->dwErrorCode)
		{
		case E_TimeEquipLimit:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("EquipLimit")]);
			return 0;
		}
	}
	m_pLocalPlayer->SetPlayerClergy(pCmd->dwClergy);
	CombatSysUtil::Inst()->ShowScreenCenterMsgEx(g_StrTable[_T("GetClergySuccess")]);
	RefrashInfo(pCmd->dwClergy);
	return 0;
}

VOID ClergyFrame::LockButt()
{
	m_pBtnUp->SetEnable(FALSE);
	m_pBtnDown->SetEnable(FALSE);
}

VOID ClergyFrame::UnLockButt()
{
	m_pBtnUp->SetEnable(TRUE);
	m_pBtnDown->SetEnable(TRUE);
}

VOID ClergyFrame::SetIsObtain()
{
	//是否满足
	TCHAR szTemp[X_LONG_NAME] = {0};

	if(m_dwClergyID == m_pLocalPlayer->GetPlayerClergy())
	{
		_stprintf(szTemp, g_StrTable[_T("Optain")], dwObtainColor);
		m_pStcHasReached->SetText(szTemp);
	}
	else if(m_bIsReach)
	{
		_stprintf(szTemp, g_StrTable[_T("HasGot")], dwGreenColor);
		m_pStcHasReached->SetText(szTemp);
	}
	else
	{
		_stprintf(szTemp, g_StrTable[_T("HasNotGet")], dwRedColor);
		m_pStcHasReached->SetText(szTemp);
	}
	m_pWnd->SetRefresh(TRUE);
}

DWORD ClergyFrame::OnEventOpenClergyFrame(tagGameEvent* pEvent)
{
	Show();

	return 0;
}

tstring ClergyFrame::GetClergyIconPath(DWORD dwClergyID)
{
	TCHAR szText[X_LONG_NAME] = {0};
	const tagClergyName* pClergyName = ClergyProtoData::Inst()->FindClergyProto(dwClergyID);
	if(!P_VALID(pClergyName))
		return _T("");
	_stprintf(szText, _T("data\\ui\\main\\%s.bmp"), pClergyName->szIcon);
	return szText;
}

DWORD ClergyFrame::OnGameEvent(tagGameEvent* pGameEvent)
{
	if(pGameEvent->strEventName == _T("LocalPlayerTransform"))
	{
		m_bIsTransform = TRUE;
	}
	else if(pGameEvent->strEventName == _T("LocalPlayerUnTransform"))
	{
		m_bIsTransform = FALSE;
	}
	return 0;
}