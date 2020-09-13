#include "StdAfx.h"
#include "SpriteRegisterFrame.h"
#include "FamilyProtoData.h"
#include "ItemProtoData.h"
#include "CombatSysUtil.h"
#include "ItemButton.h"
#include "..\WorldDefine\msg_family.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "ItemFrame.h"
#include "ItemEvent.h"
#include "ItemMgr.h"
#include "CombatSysUtil.h"
#include "RoleMgr.h"
void GUIFamilySprite::RefreshUI(DWORD dwItemID ,BYTE byQuality ,INT nLevel , DWORD dwStrength , DWORD dwResonance ,LPCTSTR szPlayerName ,LPCTSTR szSpriteName )
{
	if(0 == dwItemID || GT_INVALID ==dwItemID )
	{
		pPMainBack->SetInvisible(TRUE);
		return;
	}

	dwID = dwItemID;
	bQuality = byQuality;

	pPMainBack->SetInvisible(FALSE);
	TCHAR buffer[X_LONG_NAME];
	if( _tcscmp( szPlayerName, _T("") ) != 0)
		pStPlayerName->SetText(szPlayerName);
	pStSpriteName->SetText(szSpriteName);
	_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%d"),nLevel);
	pStLevel->SetText(buffer);
	_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%d"),dwStrength);
	pStStrength->SetText(buffer);
	_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%d.%d%%"),dwResonance/10,dwResonance%10);
	pStResonance->SetText(buffer);
	pIbPic->RefreshItem(dwItemID,0,byQuality);

}

void GUIFamilySprite::clear()
{
	//pPMainBack->SetInvisible(TRUE);
	dwID = 0;
	bQuality = 0;
	pStPlayerName->SetText(_T(""));
	pStSpriteName->SetText(_T(""));
	pStLevel->SetText(_T(""));
	pStStrength->SetText(_T(""));
	pStResonance->SetText(_T(""));
	pIbPic->RefreshItem();
}

SpriteRegisterFrame::SpriteRegisterFrame(void):m_trunk(this)
,m_n64SpriteID(0)
,m_dwSpriteTypeID(0)
,m_bSpriteQuality(0)
,m_n16Index(0)
,m_pItem(NULL)
{
}

SpriteRegisterFrame::~SpriteRegisterFrame(void)
{
}
BOOL SpriteRegisterFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;
	m_pMgr->RegisterEventHandle(_T("ItemEvent_Move2SpriteRegister"),		(FRAMEEVENTPROC)m_trunk.sfp1(&SpriteRegisterFrame::OnEventPutItem));
	m_pMgr->RegisterEventHandle(_T("MsgBox_SpriteRegister_Check"),			(FRAMEEVENTPROC)m_trunk.sfp1(&SpriteRegisterFrame::OnMsgBox_SpriteRegister_Check));
	m_pMgr->RegisterEventHandle(_T("Close_Family_AllUi"),			(FRAMEEVENTPROC)m_trunk.sfp1(&SpriteRegisterFrame::OnClose_Family_AllUi));

	m_pCmdMgr->Register( "NS_FamilySpriteRegister",		(NETMSGPROC)m_trunk.sfp2(&SpriteRegisterFrame::OnNS_FamilySpriteRegister),		_T("NS_FamilySpriteRegister"));
	m_pCmdMgr->Register( "NS_FamilySpriteGetBack",		(NETMSGPROC)m_trunk.sfp2(&SpriteRegisterFrame::OnNS_FamilySpriteGetBack),		_T("NS_FamilySpriteGetBack"));
	m_pCmdMgr->Register( "NS_GetFamilyRoleSprite",		(NETMSGPROC)m_trunk.sfp2(&SpriteRegisterFrame::OnNS_GetFamilyRoleSprite),		_T("NS_GetFamilyRoleSprite"));
	m_pCmdMgr->Register( "NS_GetFamilySpriteAtt",		(NETMSGPROC)m_trunk.sfp2(&SpriteRegisterFrame::OnNS_GetFamilySpriteAtt),		_T("NS_GetFamilySpriteAtt"));
	
	return TRUE;
}

BOOL SpriteRegisterFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();

	m_pMgr->UnRegisterEventHandler(_T("ItemEvent_Move2SpriteRegister"),	(FRAMEEVENTPROC)m_trunk.sfp1(&SpriteRegisterFrame::OnEventPutItem));
	m_pMgr->UnRegisterEventHandler(_T("MsgBox_SpriteRegister_Check"),				(FRAMEEVENTPROC)m_trunk.sfp1(&SpriteRegisterFrame::OnMsgBox_SpriteRegister_Check));
	m_pMgr->UnRegisterEventHandler(_T("Close_Family_AllUi"),				(FRAMEEVENTPROC)m_trunk.sfp1(&SpriteRegisterFrame::OnClose_Family_AllUi));

	m_pCmdMgr->UnRegister( "NS_FamilySpriteRegister",							(NETMSGPROC)m_trunk.sfp2(&SpriteRegisterFrame::OnNS_FamilySpriteRegister) );
	m_pCmdMgr->UnRegister( "NS_FamilySpriteGetBack",							(NETMSGPROC)m_trunk.sfp2(&SpriteRegisterFrame::OnNS_FamilySpriteGetBack) );
	m_pCmdMgr->UnRegister( "NS_GetFamilyRoleSprite",							(NETMSGPROC)m_trunk.sfp2(&SpriteRegisterFrame::OnNS_GetFamilyRoleSprite) );
	m_pCmdMgr->UnRegister( "NS_GetFamilySpriteAtt",							(NETMSGPROC)m_trunk.sfp2(&SpriteRegisterFrame::OnNS_GetFamilySpriteAtt) );

	if(P_VALID( m_pWnd ) )
	{
		m_pGUI->DestroyWnd(m_pWnd);
		m_pWnd = NULL;		
	}

	if(P_VALID(GetObj("SpriteRegister_Check")))
		KillObj("SpriteRegister_Check");

	LockItem(false);
	m_pItem = NULL;


	return bRet;
}

BOOL SpriteRegisterFrame::ReloadUI()
{
	XmlElement ele;
	tstring strPath = g_strLocalPath + _T("\\ui\\fairy_check.xml");
	m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str());

	// chang your xml here
	m_pGUI->ChangeXml(&ele, "fairy_check\\checkback\\fairyback\\own\\back\\icon", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "fairy_check\\checkback\\fairyback\\fairy0\\back\\icon", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "fairy_check\\checkback\\fairyback\\fairy1\\back\\icon", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "fairy_check\\checkback\\fairyback\\fairy2\\back\\icon", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "fairy_check\\checkback\\fairyback\\fairy3\\back\\icon", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "fairy_check\\checkback\\fairyback\\fairy4\\back\\icon", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "fairy_check\\checkback\\fairyback\\fairy5\\back\\icon", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "fairy_check\\checkback\\fairyback\\fairy6\\back\\icon", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "fairy_check\\checkback\\fairyback\\fairy7\\back\\icon", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "fairy_check\\checkback\\fairyback\\fairy8\\back\\icon", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "fairy_check\\checkback\\fairyback\\fairy9\\back\\icon", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "fairy_check\\checkback\\fairyback\\fairy10\\back\\icon", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "fairy_check\\checkback\\fairypic\\icon", "ClassName", "ItemButton");

	m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &ele);

	m_pClose= (GUIButton*)m_pWnd->GetChild(_T("checkback\\close"));
	m_pCloseX= (GUIButton*)m_pWnd->GetChild(_T("checkback\\leave"));
	m_pGetBack= (GUIButton*)m_pWnd->GetChild(_T("checkback\\back"));
	m_pRegister= (GUIButton*)m_pWnd->GetChild(_T("checkback\\check"));
	m_pPic= (ItemButton*)m_pWnd->GetChild(_T("checkback\\fairypic\\icon"));

	TCHAR buffer[X_LONG_NAME];
	for (int i = EFSA_Begin; i < EFSA_Num; ++i)
	{
		_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("checkback\\listback\\ability%d\\num"),i);
		m_pStSptireAtt[i] = (GUIStaticEx*)m_pWnd->GetChild(buffer);

		_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("checkback\\listback\\ability%d\\close"),i);
		m_pStSptireAttClose[i] = (GUIStatic*)m_pWnd->GetChild(buffer);
		m_pStSptireAttClose[i]->SetUnClickable(TRUE);
		
	}

	for (int i = 0; i < MAX_FAMILY_MEMBER; ++i)
	{
		if(0 == i)
			_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("checkback\\fairyback\\own"));
		else
			_sntprintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("checkback\\fairyback\\fairy%d"),i-1);

		m_pFamilySpriteInfo[i].pPMainBack = (GUIPatch*)m_pWnd->GetChild(buffer);
		m_pFamilySpriteInfo[i].pIbPic = (ItemButton*)m_pFamilySpriteInfo[i].pPMainBack->GetChild(_T("back\\icon"));
		m_pFamilySpriteInfo[i].pStPlayerName = (GUIStaticEx*)m_pFamilySpriteInfo[i].pPMainBack->GetChild(_T("playerinfo"));
		m_pFamilySpriteInfo[i].pStLevel = (GUIStatic*)m_pFamilySpriteInfo[i].pPMainBack->GetChild(_T("level\\num"));
		m_pFamilySpriteInfo[i].pStSpriteName = (GUIStatic*)m_pFamilySpriteInfo[i].pPMainBack->GetChild(_T("name"));
		m_pFamilySpriteInfo[i].pStStrength = (GUIStatic*)m_pFamilySpriteInfo[i].pPMainBack->GetChild(_T("strength\\num"));
		m_pFamilySpriteInfo[i].pStResonance = (GUIStatic*)m_pFamilySpriteInfo[i].pPMainBack->GetChild(_T("percent\\num"));

		m_pFamilySpriteInfo[i].pIbPic->RefreshItem();
		if(0 != i)
			m_pFamilySpriteInfo[i].pStPlayerName->SetText(_T(""));
		m_pFamilySpriteInfo[i].pStLevel->SetText(_T(""));
		m_pFamilySpriteInfo[i].pStSpriteName->SetText(_T(""));
		m_pFamilySpriteInfo[i].pStStrength->SetText(_T(""));
		m_pFamilySpriteInfo[i].pStResonance->SetText(_T(""));

		m_pFamilySpriteInfo[i].dwID = 0;
		m_pFamilySpriteInfo[i].bQuality = 0;
	}

	m_pPic->RefreshItem();
	
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_trunk.sfp1(&SpriteRegisterFrame::EventHandler));

	m_pWnd->SetInvisible(TRUE);

	return TRUE;
	
}

BOOL SpriteRegisterFrame::EscCancel()
{
	if (!m_pWnd->IsInvisible())
	{
		ShowFrame(FALSE);
		return TRUE;
	}

	return FALSE;
}

VOID SpriteRegisterFrame::Update()
{
	
}

DWORD SpriteRegisterFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

	switch (pEvent->eEvent)
	{
	case EGUIE_Click:
		{
			if (pWnd == m_pClose || pWnd == m_pCloseX)
			{
				ShowFrame(FALSE);
			}
			else if (pWnd == m_pGetBack)
			{
				tagNC_FamilySpriteGetBack cmd;
				m_pSession->Send(&cmd);

			}
			else if (pWnd == m_pRegister)
			{
				if(!P_VALID(m_n64SpriteID))
					break;
				
				if(P_VALID(GetObj("SpriteRegister_Check")))
					KillObj("SpriteRegister_Check");

				CreateObj("SpriteRegister_Check", "MsgBox");
				TObjRef<MsgBox>("SpriteRegister_Check")->Init(_T(""), g_StrTable[_T("SpriteRegister_Check")], _T("MsgBox_SpriteRegister_Check"));
			}
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			if (pWnd == m_pPic)
			{
				ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(m_dwSpriteTypeID,m_bSpriteQuality).c_str());
			}

			for (int i = 0; i < MAX_FAMILY_MEMBER; ++i)
			{
				if (pWnd == m_pFamilySpriteInfo[i].pIbPic)
				{
					ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(m_pFamilySpriteInfo[i].dwID,m_pFamilySpriteInfo[i].bQuality).c_str());
					break;
				}
			}
			
		}
		break;
	case EGUISCB_Drop:
	case EGUISCB_DropCheck:
		{
			if( EDT_Item != (EDragType)pEvent->dwParam1 )
				break;
			if (m_pPic == pWnd)
			{
				INT nPos = (INT16)LOWORD(pEvent->dwParam2);
				PutSprite(nPos);
			}
		}
		break;

	}
	return 0;
}

VOID SpriteRegisterFrame::ShowFrame(BOOL bShow)
{
	m_pWnd->SetInvisible(!bShow);

	if (bShow)
	{
		SetStrategy(EIST_FamilySpriteRegster);
		tagNC_GetFamilyRoleSprite cmd;
		m_pSession->Send(&cmd);

		tagNC_GetFamilySpriteAtt cmd2;
		m_pSession->Send(&cmd2);

		m_pWnd->FlipToTop();
	}
	else
	{
		LockItem(false);
		m_pItem = NULL;
		SetStrategy(EIST_ItemPocket);
		m_n64SpriteID = 0;
		m_dwSpriteTypeID = 0;
		m_bSpriteQuality = 0;
		m_n16Index = 0;
		m_pPic->RefreshItem();
		

	}
}

DWORD SpriteRegisterFrame::OnEventPutItem(tagItemPosChangeExEvent* pGameEvent)
{
	if( pGameEvent->eTypeSrc != EICT_Bag )
		return 0;
	
// 	if(P_VALID(GetObj("SpriteRegister_Check")))
// 		KillObj("SpriteRegister_Check");
// 
// 	CreateObj("SpriteRegister_Check", "MsgBox");
// 	TObjRef<MsgBox>("SpriteRegister_Check")->Init(_T(""), g_StrTable[_T("SpriteRegister_Check")], _T("MsgBox_SpriteRegister_Check"));
	
	PutSprite(pGameEvent->n16IndexSrc);
	return 0;
}

VOID SpriteRegisterFrame::PutSprite( INT16 nIndex )
{
	Item *pItem = (Item*)ItemMgr::Inst()->GetPocketItem(nIndex);
	if(!P_VALID(pItem))	return;

	LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
	if(!P_VALID(plp))	return;

	
	const tagItemProto	*pItemProto = ItemProtoData::Inst()->FindItemProto(pItem->GetItemTypeID());
	if(P_VALID(pItemProto) && MIsEquipment(pItemProto->dwTypeID) &&  pItemProto->eType == EIT_FaBao )
	{
		const tagFabaoProto *pTrumpProto = (tagFabaoProto*)pItemProto;
		const tagEquipSpec *pEquipSpec = ((Equipment*)pItem)->GetEquip();
		const tagFabaoSpec* pFaBaoSpec =  ((Equipment*)pItem)->GetFaBaoSpe();
		if (pFaBaoSpec->n16Stage > plp->GetRoleLevel())
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("SpriteRegister_RoleLevelLess")]);
			return;
		}
		if ( pEquipSpec->byQuality <= 0 )
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("SpriteRegister_QuailtyLess")]);
			return;
		}
		LockItem(false);
		m_n64SpriteID = pItem->GetItemId();
		m_dwSpriteTypeID = pItemProto->dwTypeID;
		m_bSpriteQuality = pEquipSpec->byQuality;
		m_pPic->RefreshItem(pItemProto->dwTypeID,0,pEquipSpec->byQuality);

		m_n16Index = nIndex;

		m_pItem = pItem;
		LockItem(true);
	}
}

DWORD SpriteRegisterFrame::OnNS_FamilySpriteRegister(tagNS_FamilySpriteRegister* pNetCmd,DWORD)
{
	if (E_Success == pNetCmd->dwErrorCode)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("SpriteRegister_Success")]);

		m_n64SpriteID = 0;
		m_dwSpriteTypeID = 0;
		m_bSpriteQuality = 0;
		m_n16Index = 0;
		
		m_pPic->RefreshItem();

		tagNC_GetFamilyRoleSprite cmd;
		m_pSession->Send(&cmd);

		tagNC_GetFamilySpriteAtt cmd2;
		m_pSession->Send(&cmd2);
	}
	else if (EFSE_FABAO_STATU ==pNetCmd->dwErrorCode )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("SpriteRegister_BadState")]);
	}

	return 0;
}
DWORD SpriteRegisterFrame::OnNS_FamilySpriteGetBack(tagNS_FamilySpriteGetBack* pNetCmd,DWORD)
{
	if (E_Success == pNetCmd->dwErrorCode)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("SpriteCallBack_Success")]);

		m_pFamilySpriteInfo[0].pIbPic->RefreshItem();
		m_pFamilySpriteInfo[0].pStLevel->SetText(_T(""));
		m_pFamilySpriteInfo[0].pStSpriteName->SetText(_T(""));
		m_pFamilySpriteInfo[0].pStStrength->SetText(_T(""));
		m_pFamilySpriteInfo[0].pStResonance->SetText(_T(""));

		m_pFamilySpriteInfo[0].dwID = 0;
		m_pFamilySpriteInfo[0].bQuality = 0;

		tagNC_GetFamilyRoleSprite cmd;
		m_pSession->Send(&cmd);

		tagNC_GetFamilySpriteAtt cmd2;
		m_pSession->Send(&cmd2);
	}
	return 0;
}

DWORD SpriteRegisterFrame::OnMsgBox_SpriteRegister_Check(tagMsgBoxEvent* pGameEvent)
{
	if(MBF_OK == pGameEvent->eResult )
	{
		if(!P_VALID(m_n64SpriteID))
			return 0;

		tagNC_FamilySpriteRegister cmd;
		cmd.n64id = m_n64SpriteID;
		m_pSession->Send(&cmd);
	}
	return 0;
}

VOID SpriteRegisterFrame::RefreshUI()
{
	LocalPlayer* plp = RoleMgr::Inst()->GetLocalPlayer();
	if(!P_VALID(plp))	return;

	// 找出loaclplayer的信息
	
	for (int i =0; i <MAX_FAMILY_MEMBER; i++ )
	{
		if(m_sFamilySprites[i].dwRoleID == plp->GetID())
		{
			const tagItemDisplayInfo* pProto = ItemProtoData::Inst()->FindItemDisplay(m_sFamilySprites[i].dwSpriteType);
			if(!P_VALID(pProto)) return;

			m_pFamilySpriteInfo[0].RefreshUI(m_sFamilySprites[i].dwSpriteType,m_sFamilySprites[i].bQuality,m_sFamilySprites[i].un2Level,m_sFamilySprites[i].fShili,
				m_sFamilySprites[i].fGongming*1000,_T(""),pProto->szName);
		}	
	}

	// 刷新剩下的信息
	int j =1;
	for (int i =0; i<MAX_FAMILY_MEMBER;i++ )
	{
		if(  m_sFamilySprites[i].dwRoleID && m_sFamilySprites[i].dwRoleID != plp->GetID())
		{
			const tagItemDisplayInfo* pProto = ItemProtoData::Inst()->FindItemDisplay(m_sFamilySprites[i].dwSpriteType);
			if(!P_VALID(pProto)) break;

			m_pFamilySpriteInfo[j].RefreshUI(m_sFamilySprites[i].dwSpriteType,m_sFamilySprites[i].bQuality,m_sFamilySprites[i].un2Level,m_sFamilySprites[i].fShili,
				m_sFamilySprites[i].fGongming*1000,m_sFamilySprites[i].tszRoleName,pProto->szName);

			j++;
		}
	}

	for (int i =j; i<MAX_FAMILY_MEMBER;i++)
	{
		m_pFamilySpriteInfo[i].clear();
	}
}

DWORD SpriteRegisterFrame::OnNS_GetFamilyRoleSprite(tagNS_GetFamilyRoleSprite* pNetCmd,DWORD)
{
	if (E_Success == pNetCmd->dwErrorCode)
	{
		for (int i = 0; i < pNetCmd->nNum && i < MAX_FAMILY_MEMBER ;i++)
		{
			m_sFamilySprites[i] = pNetCmd->frsi[i];
		}
		for (int i = 0; i < MAX_FAMILY_MEMBER - pNetCmd->nNum  ;i++)
		{
			if(i < pNetCmd->nNum)
				continue;

			m_sFamilySprites[i].Clear();
		}
		RefreshUI();
	}
	return 0;
}

DWORD SpriteRegisterFrame::OnNS_GetFamilySpriteAtt(tagNS_GetFamilySpriteAtt* pNetCmd,DWORD)
{
	TCHAR szNum[32];
	for (int i = EFSA_Begin; i< EFSA_Num; ++i)
	{
		if(GT_INVALID == pNetCmd->dwSpriteAtt[i])
		{
			m_pStSptireAtt[i]->SetText(g_StrTable[_T("FamilySprite_Att_UnOpen")]);
			m_pStSptireAttClose[i]->SetInvisible(TRUE);
		}
		else
		{
			_stprintf(szNum,_T("%d"),pNetCmd->dwSpriteAtt[i]);
			m_pStSptireAtt[i]->SetText(szNum);
			m_pStSptireAttClose[i]->SetInvisible(FALSE);
		}
		
	}
	return 0;
}

void SpriteRegisterFrame::LockItem( bool bOperable )
{
	if( P_VALID(m_pItem) )
		m_pItem->SetOperable(!bOperable);
}

DWORD SpriteRegisterFrame::OnClose_Family_AllUi(tagGameEvent* pGameEvent)
{
	ShowFrame(FALSE);
	return 0;
}