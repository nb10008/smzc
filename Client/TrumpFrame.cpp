#include "StdAfx.h"
#include "TrumpFrame.h"
#include "IconStatic.h"
#include "ItemButton.h"
#include "ItemMgr.h"
#include "Item.h"
#include "CharacterFrame.h"
#include "TrumpRefineFrame.h"
#include "TrumpMixBeadFrame.h"
#include "TrumpRebornFrame.h"
#include "ComposeMgr.h"
#include "ConsolidateData.h"
#include "ItemProtoData.h"
#include "ItemFrame.h"
#include "ItemEvent.h"
#include "RoleMgr.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "FairyAutoFeedFrame.h"
#include "RoleEvent.h"
#include "SkillProtoData.h"
#include "GameSet.h"
#include "CombatSysUtil.h"
#include "..\WorldDefine\msg_compose.h"

const DWORD FAIRY_SOUL_FUSE_LEVEL = 10;
const DWORD FAIRY_FUSE_LEVEL = 60;

TrumpFrame::TrumpFrame(void) : m_Trunk(this),
m_pWnd(NULL),
m_pWndFather(NULL),
m_pIconTrump(NULL),
m_pStcTrumpName(NULL),
m_pStcTrumpType(NULL),
m_pStcTrumpQuality(NULL),
m_pStcTrumpLevel(NULL),
m_pStcEquipLevel(NULL),
m_pBtnRefineTrump(NULL),
m_pBtnTrumpMixBead(NULL),
m_pBtnTrumpReborn(NULL),
m_pTrump(NULL),
m_dwFairyMoodBuffer(GT_INVALID),
m_pPbnFairyExp(NULL),
m_pPbnFairySoul(NULL),
m_bProcessing(false),
m_bSaveExp(false),
m_nIndex(0)
{
	for( int i = 0; i < EWX_End; i++ )
	{
		m_pStcLingCao[i] = NULL;
		m_pIconLingCao[i] = NULL;
		m_pStcTrumpWuXing[i] = NULL;
		m_pStcTrumpWuXing1[i] = NULL;
		m_pStcTrumpWuXing2[i] = NULL;
		m_pStcTrumpWuXingAward[i] = NULL;
	}
	for( int i = 0; i < 3; i++ )
	{
		m_pStcRoleAtt[i] = NULL;
		m_pStcRoleAttValue[i] = NULL;
	}
}

TrumpFrame::~TrumpFrame(void)
{
}

BOOL TrumpFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam);
	m_pCmdMgr->Register("NS_FabaoInlay",				(NETMSGPROC)m_Trunk.sfp2(&TrumpFrame::NetTrumpMixBead),			_T("NS_FabaoInlay"));
	m_pCmdMgr->Register("NS_FabaoPractice",				(NETMSGPROC)m_Trunk.sfp2(&TrumpFrame::NetTrumpRefine),			_T("NS_FabaoPractice"));
	m_pCmdMgr->Register("NS_FabaoWuxingReversal",		(NETMSGPROC)m_Trunk.sfp2(&TrumpFrame::NS_FabaoWuxingReversal),	_T("NS_FabaoWuxingReversal"));
	m_pCmdMgr->Register("NS_FabaoWuxingMerging",		(NETMSGPROC)m_Trunk.sfp2(&TrumpFrame::NS_FabaoWuxingMerging),	_T("NS_FabaoWuxingMerging"));
	m_pCmdMgr->Register("NS_FabaoMoodChanged",			(NETMSGPROC)m_Trunk.sfp2(&TrumpFrame::NetFairyMoodChange),		_T("NS_FabaoMoodChanged"));
	m_pCmdMgr->Register("NS_FairyStuntBook",			(NETMSGPROC)m_Trunk.sfp2(&TrumpFrame::NetLearnFairyBook),		_T("NS_FairyStuntBook"));
	m_pCmdMgr->Register("NS_FairyNirvana",				(NETMSGPROC)m_Trunk.sfp2(&TrumpFrame::NetFairyNirvana),			_T("NS_FairyNirvana"));
	m_pCmdMgr->Register("NS_FabaoFeeding",				(NETMSGPROC)m_Trunk.sfp2(&TrumpFrame::NetFairyFeeding),			_T("NS_FabaoFeeding"));
	m_pCmdMgr->Register("NS_ExpBall2ExpPilular",		(NETMSGPROC)m_Trunk.sfp2(&TrumpFrame::NetFairyExpChange2Ball),	_T("NS_ExpBall2ExpPilular"));
	m_pCmdMgr->Register("NS_SyncFaBaoStoreExpInfo",		(NETMSGPROC)m_Trunk.sfp2(&TrumpFrame::NetFairySaveExp),			_T("NS_SyncFaBaoStoreExpInfo"));
	m_pCmdMgr->Register("NS_ExpBallBind2Role",		(NETMSGPROC)m_Trunk.sfp2(&TrumpFrame::NoNS_ExpBallBind2Role),			_T("NS_ExpBallBind2Role"));
	m_pCmdMgr->Register("NS_InquireSpirteBornState",		(NETMSGPROC)m_Trunk.sfp2(&TrumpFrame::NoNS_InquireSpirteBornState),			_T("NS_InquireSpirteBornState"));

	m_pFrameMgr->RegisterEventHandle(_T("tagFairyExcited"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&TrumpFrame::OnFairyExcited));
	m_pFrameMgr->RegisterEventHandle(_T("UpdateFairyMood"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&TrumpFrame::OnUpdateFairyMood));
	m_pFrameMgr->RegisterEventHandle(_T("CloseAllTrumpChildFrame"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&TrumpFrame::CloseAllTrumpChildFrame));
	m_pFrameMgr->RegisterEventHandle(_T("ItemEvent_Move2FairyTransferExp"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&TrumpFrame::OnEventPutItem));

	return bRet;
}

DWORD TrumpFrame::NetFairyFeeding( tagNS_FabaoFeeding *pNetCmd, DWORD )
{
	if(pNetCmd->dwErrorCode == E_Success)
		UpdateUI();
	else if (E_FabaoFeeding_NotBirthFabao == pNetCmd->dwErrorCode )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_FabaoFeeding_NotBirthFabao")]);
	}
	else if (E_FabaoFeeding_MatingPregnancyCDTimeLT1h == pNetCmd->dwErrorCode )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_FabaoFeeding_MatingPregnancyCDTimeLT1h")]);
	}
	else if (E_FabaoFeeding_MatingRestCDTimeLT1h == pNetCmd->dwErrorCode )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_FabaoFeeding_MatingRestCDTimeLT1h")]);
	}
	return 0;
}

DWORD TrumpFrame::NetLearnFairyBook( tagNS_FairyStuntBook *pNetCmd, DWORD )
{
	//if(pNetCmd->dwErrorCode == E_Success)
	//	CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FairyLearnSkillSucc")]);
	UpdateUI();
	return 0;
}

DWORD TrumpFrame::NetFairyNirvana( tagNS_FairyNirvana *pNetCmd, DWORD )
{
	if(pNetCmd->dwErrorCode == E_Success)
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FairyNirvanaSucc")]);
	UpdateUI();
	return 0;
}

DWORD TrumpFrame::NetFairySaveExp( tagNS_SyncFaBaoStoreExpInfo *pNetCmd, DWORD )
{
	if( !P_VALID(m_pTrump) )
		return 0;
	if(m_bSaveExp != pNetCmd->bStoreExpOpen)
	{
		if(m_bSaveExp)
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("CloseFairySaveExpFail")]);
		else
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("OpenFairySaveExpSucc")]);
		m_bSaveExp = pNetCmd->bStoreExpOpen;
		m_pTrump->SetFairySaveEXpOrNot(pNetCmd->bStoreExpOpen);
	}
	TCHAR szTemp[X_LONG_NAME] = {0};
	_sntprintf( szTemp, X_LONG_NAME, g_StrTable[_T("FairyCurSaveExp")], pNetCmd->nCurStoreExp);
	CombatSysUtil::Inst()->ShowScreenCenterMsg(szTemp);
	
	m_pTrump->SetFairySaveExp(pNetCmd->nCurStoreExp);
	UpdateUI();
	LockItem(false);

	return 0;
}

DWORD TrumpFrame::NetFairyExpChange2Ball( tagNS_ExpBall2ExpPilular *pNetCmd, DWORD )
{
	m_pBtnSaveExp->SetEnable(true);
	m_pBtnPre->SetEnable(true);
	m_pBtnNext->SetEnable(true);
	if(pNetCmd->dwErrorCode == E_Success)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FairyExpChang2BallSucc")]);
		ClearExpSaveInfo();
	}
	else if( E_FabaoStoreExp_BagFull == pNetCmd->dwErrorCode)
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("SkillSvrErr40")]);
	else if( E_FabaoStoreExp_NoEnoughExp == pNetCmd->dwErrorCode)
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_FabaoStoreExp_NoEnoughExp")]);
	m_pProTime->SetValue(0.0f);
	return 0;
}

BOOL TrumpFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();
	m_pCmdMgr->UnRegister("NS_FabaoInlay",				(NETMSGPROC)m_Trunk.sfp2(&TrumpFrame::NetTrumpMixBead));
	m_pCmdMgr->UnRegister("NS_FabaoPractice",			(NETMSGPROC)m_Trunk.sfp2(&TrumpFrame::NetTrumpRefine));
	m_pCmdMgr->UnRegister("NS_FabaoWuxingReversal",		(NETMSGPROC)m_Trunk.sfp2(&TrumpFrame::NS_FabaoWuxingReversal));
	m_pCmdMgr->UnRegister("NS_FabaoWuxingMerging",		(NETMSGPROC)m_Trunk.sfp2(&TrumpFrame::NS_FabaoWuxingMerging));
	m_pCmdMgr->UnRegister("NS_FabaoMoodChanged",		(NETMSGPROC)m_Trunk.sfp2(&TrumpFrame::NetFairyMoodChange));
	m_pCmdMgr->UnRegister("NS_FairyStuntBook",			(NETMSGPROC)m_Trunk.sfp2(&TrumpFrame::NetLearnFairyBook));
	m_pCmdMgr->UnRegister("NS_FairyNirvana",			(NETMSGPROC)m_Trunk.sfp2(&TrumpFrame::NetFairyNirvana));
	m_pCmdMgr->UnRegister("NS_FabaoFeeding",			(NETMSGPROC)m_Trunk.sfp2(&TrumpFrame::NetFairyFeeding));
	m_pCmdMgr->UnRegister("NS_ExpBall2ExpPilular",		(NETMSGPROC)m_Trunk.sfp2(&TrumpFrame::NetFairyExpChange2Ball));
	m_pCmdMgr->UnRegister("NS_SyncFaBaoStoreExpInfo",	(NETMSGPROC)m_Trunk.sfp2(&TrumpFrame::NetFairySaveExp));
	m_pCmdMgr->UnRegister("NS_ExpBallBind2Role",	(NETMSGPROC)m_Trunk.sfp2(&TrumpFrame::NoNS_ExpBallBind2Role));
	m_pCmdMgr->UnRegister("NS_InquireSpirteBornState",	(NETMSGPROC)m_Trunk.sfp2(&TrumpFrame::NoNS_InquireSpirteBornState));

	m_pFrameMgr->UnRegisterEventHandler(_T("tagFairyExcited"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&TrumpFrame::OnFairyExcited));
	m_pFrameMgr->UnRegisterEventHandler(_T("UpdateFairyMood"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&TrumpFrame::OnUpdateFairyMood));
	m_pFrameMgr->UnRegisterEventHandler(_T("CloseAllTrumpChildFrame"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&TrumpFrame::CloseAllTrumpChildFrame));
	m_pFrameMgr->UnRegisterEventHandler(_T("ItemEvent_Move2FairyTransferExp"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&TrumpFrame::OnEventPutItem));
	return bRet;
}

DWORD TrumpFrame::OnEventPutItem(tagItemPosChangeExEvent* pGameEvent)
{
	if( pGameEvent->eTypeSrc != EICT_Bag )
		return 0;
	PutItem(pGameEvent->n16IndexSrc);
	return 0;
}


VOID TrumpFrame::PutItem(INT16 nPos)
{
	m_nIndex = 0;
	Item *pItem = (Item*)ItemMgr::Inst()->GetPocketItem(nPos);
	if(!P_VALID(pItem))
		return;

	const tagItemProto *pItemProto = pItem->GetProto();
	if(!P_VALID(pItemProto) || pItemProto->eSpecFunc != EISF_ExpBall)
		return;
	
	LockItem(false);
	tstring strIcon = ItemMgr::Inst()->GetItemIconPath(pItemProto->dwTypeID);
	m_pIconExpBall->SetPic(strIcon);
	m_n64ExpBall = pItem->GetItemId();
	LockItem(true);
	m_pBtnNext->SetEnable(true);
	ChangeExpVector();
}

VOID TrumpFrame::ChangeExpVector()
{
	if(m_vecExpBallID.empty() || m_nIndex >= (INT)m_vecExpBallID.size())
		return;
	const tagItemProto *pProto = ItemProtoData::Inst()->FindItemProto(m_vecExpBallID[m_nIndex]);
	if(!P_VALID(pProto))
		return;
	DWORD dwExp = pProto->nSpecFuncVal1;
	TCHAR szTemp[X_LONG_NAME] = {0};
	if(dwExp > m_dwCurSaveExp)
	{
		_sntprintf(szTemp, X_LONG_NAME, _T("<color=0xFFFF0000>%d"), dwExp);
		m_pBtnTransferExp->SetEnable(false);
	}
	else
	{
		_sntprintf(szTemp, X_LONG_NAME, _T("%d"), dwExp );
		m_pBtnTransferExp->SetEnable(true);
	}
	m_pStcFairyTransExp->SetText(szTemp);
	m_pStcFairyTransExp->SetRefresh(TRUE);
	
	tstring strPic = ItemMgr::Inst()->GetItemIconPath(m_vecExpBallID[m_nIndex]);
	m_pIconExpVector->SetPic(strPic);
}

VOID TrumpFrame::LockItem(bool bOperable)
{
	Item *pItem = ItemMgr::Inst()->GetPocketItemByID(m_n64ExpBall);
	if(P_VALID(pItem))
		pItem->SetOperable(!bOperable);
}

BOOL TrumpFrame::ReloadUI()
{
	m_pWndFather = m_pGUI->GetWnd(((CharacterFrame*)m_pFather)->GetGUIFullName().c_str());
	m_pWnd = m_pWndFather->GetChild(_T("fairy_back"));

	m_pIconTrump = (ItemButton*)m_pWnd->GetChild(_T("basic\\pic_back\\pic"));
	TCHAR szTemp[X_LONG_NAME] = {0};
	for( int i = 0; i < 5; i++ )
	{
		_sntprintf( szTemp, X_LONG_NAME, _T("picback\\pic\\pic%d"), i );
		m_pIconLingCao[i] = (ItemButton*)m_pWnd->GetChild(szTemp);
		_sntprintf( szTemp, X_LONG_NAME, _T("picback\\pic\\word%d"), i );
		m_pStcLingCao[i] = (GUIStatic*)m_pWnd->GetChild(szTemp);

		_sntprintf( szTemp, X_LONG_NAME, _T("picback\\pic\\num%d0"), i );
		m_pStcTrumpWuXing[i] = (GUIStaticEx*)m_pWnd->GetChild(szTemp);
		_sntprintf( szTemp, X_LONG_NAME, _T("picback\\pic\\num%d1"), i );
		m_pStcTrumpWuXing1[i] = (GUIStaticEx*)m_pWnd->GetChild(szTemp);
		_sntprintf( szTemp, X_LONG_NAME, _T("picback\\pic\\num%d2"), i );

		m_pStcTrumpWuXing2[i] = (GUIStaticEx*)m_pWnd->GetChild(szTemp);
		_sntprintf( szTemp, X_LONG_NAME, _T("picback\\pic\\charactor%d"), i );
		m_pStcTrumpWuXingAward[i] = (GUIStaticEx*)m_pWnd->GetChild(szTemp);
	}

	m_pStcTrumpName = (GUIStatic*)m_pWnd->GetChild(_T("basic\\name"));
	m_pStcTrumpType = (GUIStatic*)m_pWnd->GetChild(_T("basic\\type"));
	m_pStcTrumpQuality = (GUIStatic*)m_pWnd->GetChild(_T("basic\\cultivationprogress\\number"));
	m_pProFairyQuality = (GUIProgress*)m_pWnd->GetChild(_T("basic\\cultivationprogress\\progressbar"));
	m_pStcFairyExp = (GUIStatic*)m_pWnd->GetChild(_T("basic\\aptitudeprogress\\number"));
	m_pProFairyExp = (GUIProgress*)m_pWnd->GetChild(_T("basic\\aptitudeprogress\\progressbar"));
	m_pStcTrumpLevel = (GUIStatic*)m_pWnd->GetChild(_T("basic\\num"));
	m_pStcEquipLevel = (GUIStatic*)m_pWnd->GetChild(_T("basic\\num0"));

	m_pStcRoleAttValue[0] = (GUIStatic*)m_pWnd->GetChild(_T("basic\\num1"));
	m_pStcRoleAttValue[0]->SetInvisible(true);
	m_pStcRoleAttValue[1] = (GUIStatic*)m_pWnd->GetChild(_T("basic\\num2"));
	m_pStcRoleAttValue[1]->SetInvisible(true);
	m_pStcRoleAttValue[2] = (GUIStatic*)m_pWnd->GetChild(_T("basic\\num3"));
	m_pStcRoleAttValue[2]->SetInvisible(true);
	m_pStcRoleAtt[0] = (GUIStatic*)m_pWnd->GetChild(_T("basic\\word1"));
	m_pStcRoleAtt[0]->SetInvisible(true);
	m_pStcRoleAtt[1] = (GUIStatic*)m_pWnd->GetChild(_T("basic\\word2"));
	m_pStcRoleAtt[1]->SetInvisible(true);
	m_pStcRoleAtt[2] = (GUIStatic*)m_pWnd->GetChild(_T("basic\\word3"));
	m_pStcRoleAtt[2]->SetInvisible(true);

	m_pBtnRefineTrump = (GUIButton*)m_pWnd->GetChild(_T("cultivate_butt"));
	m_pBtnTrumpMixBead = (GUIButton*)m_pWnd->GetChild(_T("soul_fuse_butt"));
	m_pBtnTrumpReborn = (GUIButton*)m_pWnd->GetChild(_T("fairy_fuse_butt"));

	m_pStcFairyMoodInfo = (GUIStatic*)m_pWnd->GetChild(_T("basic\\mood"));
	m_pStcFairyExpInfo = (GUIStatic*)m_pWnd->GetChild(_T("basic\\add"));
	m_pStcFairyExpRate = (GUIStatic*)m_pWnd->GetChild(_T("basic\\addpercent"));
	m_pStcFairyMood = (GUIStatic*)m_pWnd->GetChild(_T("basic\\moodprogress"));
	m_pProFairyMood = (GUIProgress*)m_pWnd->GetChild(_T("basic\\moodprogress\\progressbar"));
	m_pProFairyMood->SetMaxValue(2000);
	m_pIconMoodPic = (IconStatic*)m_pWnd->GetChild(_T("basic\\moodpic"));
	m_pIconAutoFeed = (IconStatic*)m_pWnd->GetChild(_T("basic\\feedpic"));
	m_pIconAutoFeed->SetPic(_T("data\\ui\\pet\\SM_fairy_feed.bmp"));
	m_pBtnAutoFeed = (GUIButton*)m_pWnd->GetChild(_T("feed_butt"));
	m_pStcEffect = (GUIStatic*)m_pWnd->GetChild(_T("basic\\effecttype"));
	m_pStcEffectInfo = (GUIStatic*)m_pWnd->GetChild(_T("basic\\effect"));
	for( int i = 0; i < 3; i++ )
	{
		_sntprintf( szTemp, X_LONG_NAME, _T("basic\\percent%d"), i + 1 );
		m_pStcRoleAttValueRate[i] = (GUIStatic*)m_pWnd->GetChild(szTemp);
	}
	m_pPbnFairyExp = (GUIPushButton*)m_pWnd->GetChild(_T("fairyopt\\optbutt1"));
	m_pPbnFairyExp->SetEnable(false);
	m_pPbnFairySoul = (GUIPushButton*)m_pWnd->GetChild(_T("fairyopt\\optbutt0"));
	m_pPbnFairySoul->SetState(EGUIBS_PushDown);

	m_pPbnFairyBorn = (GUIPushButton*)m_pWnd->GetChild(_T("fairyopt\\optbutt2"));
	m_pPbnFairyBorn->SetInvisible(FALSE);
	m_pPbnFairyBorn->SetEnable(TRUE);
// 	m_pPbnFairyBorn->SetEnable(FALSE);	
// 		m_pPbnFairyBorn->SetInvisible(TRUE);

	m_pStcFairySoulBack = (GUIPatch*)m_pWnd->GetChild(_T("picback"));
	m_pStcFairyExpBack = (GUIPatch*)m_pWnd->GetChild(_T("expback"));
	m_pStcFairyBornBack = (GUIPatch*)m_pWnd->GetChild(_T("breedback"));
	m_pStcFairyBornBack->SetInvisible(TRUE);
	m_pStcFairyExpBack->SetInvisible(TRUE);

	m_pProFairySaveExp = (GUIProgress*)m_pWnd->GetChild(_T("expback\\picback\\progpic0\\progress"));
	m_pProTime = (GUIProgress*)m_pWnd->GetChild(_T("expback\\picback\\progpic1\\progress"));

	m_pStcFairySaveExp = (GUIStaticEx*)m_pWnd->GetChild(_T("expback\\picback\\exp0\\num0"));
	m_pStcFairyMaxSaveExp = (GUIStaticEx*)m_pWnd->GetChild(_T("expback\\picback\\exp0\\num1"));

	m_pStcFairyTransExp = (GUIStaticEx*)m_pWnd->GetChild(_T("expback\\picback\\exp1\\num0"));
	m_pStcFairyCurSaveExp = (GUIStaticEx*)m_pWnd->GetChild(_T("expback\\picback\\exp1\\num1"));

	m_pBtnSaveExp = (GUIButton*)m_pWnd->GetChild(_T("expback\\picback\\openbutt"));
	m_pBtnTransferExp = (GUIButton*)m_pWnd->GetChild(_T("expback\\picback\\transbutt"));

	m_pBtnPre = (GUIButton*)m_pWnd->GetChild(_T("expback\\picback\\backbutt"));
	m_pBtnNext = (GUIButton*)m_pWnd->GetChild(_T("expback\\picback\\nextbutt"));

	m_pIconExpBall = (IconStatic*)m_pWnd->GetChild(_T("expback\\picback\\pic0"));
	m_pIconExpVector = (IconStatic*)m_pWnd->GetChild(_T("expback\\picback\\pic1"));

	m_pStMatingTime= (GUIStatic*)m_pWnd->GetChild(_T("breedback\\pic\\breedback\\num"));
	m_pStSpriteStateNormal= (GUIStatic*)m_pWnd->GetChild(_T("breedback\\pic\\stageback\\stage0"));
	m_pStSpriteStatePregnancy= (GUIStatic*)m_pWnd->GetChild(_T("breedback\\pic\\stageback\\stage1"));
	m_pStSpriteStateBorn= (GUIStatic*)m_pWnd->GetChild(_T("breedback\\pic\\stageback\\stage2"));
	m_pStSpriteStateRest= (GUIStatic*)m_pWnd->GetChild(_T("breedback\\pic\\stageback\\stage3"));

	m_pStHour= (GUIStatic*)m_pWnd->GetChild(_T("breedback\\pic\\timeback\\time\\hour"));
	m_pStMIn= (GUIStatic*)m_pWnd->GetChild(_T("breedback\\pic\\timeback\\time\\minute"));


	m_pWnd->SetInvisible(TRUE);
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&TrumpFrame::EventHandler));
	m_vecExpBallID = ItemProtoData::Inst()->GetFairyExpBall();

	m_pProFairySaveExp->SetSpeed(100);
	UpdateUI();
	return TRUE;
}

VOID TrumpFrame::Update()
{
	if(m_bProcessing)
	{
		float deltaTime =  Cool3D::Kernel::Inst()->GetDeltaTime();
		float fcur = m_pProTime->GetCurrentValue();
		float fMax = m_pProTime->GetMaxValue();
		if(fcur < fMax)
		{
			fcur += deltaTime;
			m_pProTime->SetValue(fcur, true);
			m_pProTime->SetRefresh();
		}
		if(fcur >= fMax)
		{
			tagNC_ExpBall2ExpPilular m;
			m.n64ExpBallSerialID = m_n64ExpBall;
			m.dwExpPilularID = m_vecExpBallID[m_nIndex];
			m_pSession->Send(&m);
			m_bProcessing = false;
		}
	}
}

DWORD TrumpFrame::EventHandler( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

	switch (pEvent->eEvent)
	{
	case EGUIE_Click:
		if( pWnd == m_pBtnRefineTrump )
		{
			TrumpMixBeadFrame *pMixBeadFrame = (TrumpMixBeadFrame*)m_pFrameMgr->GetFrame(_T("TrumpMixBead"));
			if(P_VALID(pMixBeadFrame))
				pMixBeadFrame->ShowUI(false);
			TrumpRebornFrame *pRebornFrame = (TrumpRebornFrame*)m_pFrameMgr->GetFrame(_T("TrumpReborn"));
			if(P_VALID(pRebornFrame))
				pRebornFrame->ShowUI(false);

			RefineTrumpFrame *pFrame = (RefineTrumpFrame*)m_pFrameMgr->GetFrame(_T("RefineTrump"));
			if(!P_VALID(pFrame))
				pFrame = (RefineTrumpFrame*)m_pFrameMgr->CreateFrame( _T("World"), _T("RefineTrump"), _T("RefineTrumpFrame"), 0 );
			if(P_VALID(pFrame))
			{
				SetStrategy(EIST_TrumpRefine);
				pFrame->ShowUI(true);
			}
		}
		else if( pWnd == m_pBtnTrumpMixBead )
		{
			RefineTrumpFrame *pRefineFrame = (RefineTrumpFrame*)m_pFrameMgr->GetFrame(_T("RefineTrump"));
			if(P_VALID(pRefineFrame))
				pRefineFrame->ShowUI(false);
			TrumpRebornFrame *pRebornFrame = (TrumpRebornFrame*)m_pFrameMgr->GetFrame(_T("TrumpReborn"));
			if(P_VALID(pRebornFrame))
				pRebornFrame->ShowUI(false);

			TrumpMixBeadFrame *pFrame = (TrumpMixBeadFrame*)m_pFrameMgr->GetFrame(_T("TrumpMixBead"));
			if(!P_VALID(pFrame))
				pFrame = (TrumpMixBeadFrame*)m_pFrameMgr->CreateFrame( _T("World"), _T("TrumpMixBead"), _T("TrumpMixBeadFrame"), 0 );
			if(P_VALID(pFrame))
			{
				SetStrategy(EIST_MixBead);
				pFrame->ShowUI(true);
			}
		}
		else if( pWnd == m_pBtnTrumpReborn )
		{
			RefineTrumpFrame *pRefineFrame = (RefineTrumpFrame*)m_pFrameMgr->GetFrame(_T("RefineTrump"));
			if(P_VALID(pRefineFrame))
				pRefineFrame->ShowUI(false);
			TrumpMixBeadFrame *pMixBeadFrame = (TrumpMixBeadFrame*)m_pFrameMgr->GetFrame(_T("TrumpMixBead"));
			if(P_VALID(pMixBeadFrame))
				pMixBeadFrame->ShowUI(false);

			TrumpRebornFrame *pFrame = (TrumpRebornFrame*)m_pFrameMgr->GetFrame(_T("TrumpReborn"));
			if(!P_VALID(pFrame))
				pFrame = (TrumpRebornFrame*)m_pFrameMgr->CreateFrame( _T("World"), _T("TrumpReborn"), _T("TrumpRebornFrame"), 0 );
			if(P_VALID(pFrame))
			{
				SetStrategy(EIST_TrumpReborn);
				pFrame->ShowUI(true);
			}
		}
		else if( pWnd == m_pBtnAutoFeed )
		{
			FairyAutoFeedFrame *pFrame = (FairyAutoFeedFrame*)(TObjRef<GameFrameMgr>()->GetFrame(_T("FairyAutoFeed")));
			if(!P_VALID(pFrame))
				pFrame = (FairyAutoFeedFrame*)TObjRef<GameFrameMgr>()->CreateFrame( _T("World"), _T("FairyAutoFeed"), _T("FairyAutoFeedFrame"), 0 );
			if(P_VALID(pFrame))
			{
				m_pMgr->SendEvent( &tagGameEvent( _T("Close_Role_UI"), this ) );
				pFrame->ShowUI(true);
				SetStrategy(EIST_FairyAutoFeed);
			}
		}
		else if( pWnd == m_pBtnPre )
		{
			m_nIndex--;
			if( m_nIndex <= 0 )
			{
				m_pBtnPre->SetEnable(false);
				m_nIndex = 0;
			}
			else
				m_pBtnNext->SetEnable(true);
			ChangeExpVector();
		}
		else if( pWnd == m_pBtnNext )
		{
			m_nIndex++;
			if( m_nIndex >= m_vecExpBallID.size() - 1 )
			{
				m_pBtnNext->SetEnable(false);
				m_nIndex = m_vecExpBallID.size() - 1;
			}
			else
				m_pBtnPre->SetEnable(true);
			ChangeExpVector();
		}
		else if( pWnd == m_pBtnTransferExp )
		{
			m_pProTime->SetSpeed(100);
			m_pProTime->SetValue(0, true);
			m_pProTime->SetMaxValue(3.0f);
			m_pProTime->SetRefresh();
			m_bProcessing = true;
			m_pBtnTransferExp->SetEnable(false);
			m_pBtnSaveExp->SetEnable(false);
			m_pBtnPre->SetEnable(false);
			m_pBtnNext->SetEnable(false);
		}
		else if( pWnd == m_pBtnSaveExp )
		{
			tagNC_SyncFaBaoStoreExpInfo m;
			m.bStoreExpOpen = !m_bSaveExp;
			m_pSession->Send(&m);
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			for( int i = 0; i < EWX_End; i ++ )
			{
				if( pWnd == m_pIconLingCao[i] && P_VALID(m_pTrump) )
				{
					const tagFabaoSpec *pTrumpSpe = m_pTrump->GetFaBaoSpe();
					DWORD dwTypeID = pTrumpSpe->dwWuxingBeadID[i];
					if( P_VALID(pTrumpSpe) && dwTypeID != GT_INVALID && dwTypeID != 0 )
					{
						tstring strTip = ToolTipCreator::Inst()->GetItemTips(dwTypeID);
						ShowTip( m_pIconLingCao[i], strTip.c_str() );
					}
					else if( dwTypeID == GT_INVALID )
					{
						INT nLevel = 0;
						switch(i)
						{
						case 0:
							nLevel = 10;
							break;
						case 1:
							nLevel = 20;
							break;
						case 2:
							nLevel = 30;
							break;
						case 3:
							nLevel = 40;
							break;
						case 4:
							nLevel = 60;
							break;
						}

						TCHAR szTemp[X_LONG_NAME] = {0};
						_sntprintf( szTemp, X_LONG_NAME, g_StrTable[_T("OpenLingCao")], nLevel);
						ShowTip( m_pIconLingCao[i], szTemp );
					}
					else if( dwTypeID == 0 )
					{
						ShowTip( m_pIconLingCao[i], g_StrTable[_T("CanHaveLingZhu")] );
					}
				}
			}
			if( pWnd == m_pIconTrump && P_VALID(m_pTrump) )
				ShowTip( pWnd, ToolTipCreator::Inst()->GetItemTips(m_pTrump).c_str() );
			else if( pWnd == m_pStcEffect )
			{
				const tagBuffProtoClient* pBuffProto = SkillProtoData::Inst()->FindBuffProto(m_dwFairyMoodBuffer);
				if(P_VALID(pBuffProto))
					ShowTip( pWnd, pBuffProto->szDesc ); 
			}
			else if(pWnd == m_pIconExpBall)
			{
				Item *pItem = ItemMgr::Inst()->GetPocketItemByID(m_n64ExpBall);
				if(P_VALID(pItem))
					ShowTip( pWnd, ToolTipCreator::Inst()->GetItemTips(pItem).c_str() );
			}
			else if(pWnd == m_pIconExpVector)
			{
				if(m_n64ExpBall != GT_INVALID && m_nIndex >= 0 && m_nIndex < m_vecExpBallID.size() )
					ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(m_vecExpBallID[m_nIndex], GT_INVALID).c_str());
			}
		}
		break;
	case EGUIE_MoveOutOfCtrl:
		{
			ShowTip(pWnd, NULL);
		}
		break;
	case EGUIE_PushButtonDown:
		{
			if(pWnd == m_pPbnFairyExp)
			{
				m_pStcFairySoulBack->SetInvisible(TRUE);
				m_pStcFairyExpBack->SetInvisible(FALSE);
				m_pStcFairyBornBack->SetInvisible(TRUE);
				SetStrategy(EIST_FairyTransferExp);
			}
			else if(pWnd == m_pPbnFairySoul)
			{
				m_pStcFairySoulBack->SetInvisible(FALSE);
				m_pStcFairyExpBack->SetInvisible(TRUE);
				m_pStcFairyBornBack->SetInvisible(TRUE);
				ClearExpSaveInfo();
			}
			else if(pWnd == m_pPbnFairyBorn)
			{
				Equipment* pEquip = ItemMgr::Inst()->GetCurEquip(EEP_Face);
				if( P_VALID(pEquip) )
				{
					tagNC_InquireSpirteBornState cmd;
					cmd.n64ExpBallSerialID = pEquip->GetItemId();
					m_pSession->Send(&cmd);
				}
				
				RefreshSpriteBornUI();
				m_pStcFairySoulBack->SetInvisible(TRUE);
				m_pStcFairyExpBack->SetInvisible(TRUE);
				m_pStcFairyBornBack->SetInvisible(FALSE);
			}
		}
		break;
	case EGUISCB_Drop:
	case EGUISCB_DropCheck:
		{
			if( EDT_Item != (EDragType)pEvent->dwParam1 )
				break;
				if (m_pIconExpBall == pWnd)
				{
					INT nPos = (INT16)LOWORD(pEvent->dwParam2);
					PutItem(nPos);
				}
		}
		break;
	}
	return 0;
}

VOID TrumpFrame::UpdateUI()
{
	ClearText();
	m_pTrump = ItemMgr::Inst()->GetCurEquip(EEP_Face);
	if( !P_VALID(m_pTrump) )
		return;

	const tagFabaoSpec *pTrumpSpe = m_pTrump->GetFaBaoSpe();
	const tagFabaoProto *pTrumpProto = (tagFabaoProto*)m_pTrump->GetProto();
	if(!P_VALID(pTrumpProto) && !(pTrumpSpe))
		return;

	m_dwFairyMoodBuffer = pTrumpSpe->dwMoodBuffID;
	if( pTrumpSpe->n16Stage < 30 )
	{
		m_pStcFairyMood->SetInvisible(TRUE);
		m_pIconMoodPic->SetInvisible(TRUE);
		m_pBtnAutoFeed->SetInvisible(TRUE);
		m_pStcEffect->SetInvisible(TRUE);
		m_pStcEffectInfo->SetInvisible(TRUE);
		m_pStcFairyExpRate->SetInvisible(TRUE);
		m_pStcFairyExpInfo->SetInvisible(TRUE);
		m_pStcFairyMoodInfo->SetInvisible(TRUE);
		m_pIconAutoFeed->SetInvisible(TRUE);
		for( int i = 0; i < 3; i++ )
			m_pStcRoleAttValueRate[i]->SetInvisible(TRUE);
	}
	else
		UpdateMoodInfo(pTrumpSpe->nMood);

	if (pTrumpSpe->n16Stage < FAIRY_FUSE_LEVEL )
		m_pBtnTrumpReborn->SetEnable(false);
	else
		m_pBtnTrumpReborn->SetEnable(true);

	if (pTrumpSpe->n16Stage < FAIRY_SOUL_FUSE_LEVEL )
		m_pBtnTrumpMixBead->SetEnable(false);
	else
		m_pBtnTrumpMixBead->SetEnable(true);

	if (pTrumpSpe->n16Stage < MIN_STORE_EXP_FABAO_LEVEL )
		m_pPbnFairyExp->SetEnable(false);
	else
		m_pPbnFairyExp->SetEnable(true);

	if (pTrumpSpe->n16Stage < MIN_Sprite_Born_LEVEL )
		m_pPbnFairyBorn->SetEnable(false);
	else
		m_pPbnFairyBorn->SetEnable(true);
	

	DWORD dwTypeID = m_pTrump->GetItemTypeID();
	BYTE byQuality = m_pTrump->GetItemQuality();
	m_pIconTrump->RefreshItem( dwTypeID, 0, byQuality );
	tstring strName = m_pTrump->GetDisplay()->szName;
	m_pStcTrumpName->SetText(strName.c_str());

	TCHAR szTemp[X_LONG_NAME] = {0};
	_sntprintf( szTemp, X_LONG_NAME, _T("TrumpType_%d"), pTrumpProto->eFarbaoType );
	m_pStcTrumpType->SetText(g_StrTable[szTemp]);

	_sntprintf( szTemp, X_LONG_NAME, _T("%d"), pTrumpSpe->n16NativeIntelligence );
	m_pStcTrumpQuality->SetText(szTemp);
	m_pProFairyQuality->SetValue(pTrumpSpe->n16NativeIntelligence);
	m_pProFairyQuality->SetMaxValue(100.0f);
	m_pProFairyQuality->SetSpeed(10000.0f);
	m_pStcTrumpQuality->FlipToTop();

	for( int i = 0; i < 3; i++ )
	{
		
		if( pTrumpSpe->eAttType[i] != ERA_Null )
		{
			if(pTrumpSpe->eAttType[i]>=92&&pTrumpSpe->eAttType[i]<=97)//add by xp
			{
				_sntprintf( szTemp, X_LONG_NAME, _T("ERoleAttribute_133"));//这里写死了，133为元素伤害
				m_pStcRoleAtt[i]->SetText(g_StrTable[szTemp]);
				m_pStcRoleAtt[i]->SetInvisible(false);

			}//add end
			else
			{
				_sntprintf( szTemp, X_LONG_NAME, _T("ERoleAttribute_%d"), pTrumpSpe->eAttType[i]);
				m_pStcRoleAtt[i]->SetText(g_StrTable[szTemp]);
				m_pStcRoleAtt[i]->SetInvisible(false);

			}
			_sntprintf( szTemp, X_LONG_NAME, _T("%d"), pTrumpSpe->nAttGrowing[i]);
			m_pStcRoleAttValue[i]->SetText(szTemp);
			m_pStcRoleAttValue[i]->SetInvisible(false);
		}
	}

	INT nCurExp = pTrumpSpe->nPracticeValue - ItemMgr::Inst()->GetTrumpCurExp( pTrumpSpe->n16Stage );
	INT nTotalExp = ItemMgr::Inst()->GetTrumpNextLevelExp( pTrumpSpe->n16Stage );
	m_pProFairyExp->SetMaxValue((float)nTotalExp);
	m_pProFairyExp->SetValue((float)nCurExp);
	m_pProFairyExp->SetSpeed(10000000000.0f);
	_sntprintf( szTemp, X_LONG_NAME, _T("%d/%d"), nCurExp, nTotalExp );
	m_pStcFairyExp->SetText(szTemp);
	m_pStcFairyExp->FlipToTop();

	_sntprintf( szTemp, X_LONG_NAME, _T("%d"), pTrumpSpe->n16Stage );
	m_pStcTrumpLevel->SetText(szTemp);

	_sntprintf( szTemp, X_LONG_NAME, _T("%d"), max(pTrumpSpe->nUseLevel, pTrumpProto->byMinUseLevel) );
	m_pStcEquipLevel->SetText(szTemp);

	INT nLingCaoNum = GetLingCaoNum();
	for( int i = 0; i < EWX_End; i++ )
	{
		_sntprintf( szTemp, X_LONG_NAME, _T("EWuXingHotel_%d"), pTrumpSpe->eWuxingHole[i] );
		m_pStcLingCao[i]->SetText(g_StrTable[szTemp]);

		dwTypeID = pTrumpSpe->dwWuxingBeadID[i];
		if( i < nLingCaoNum )
		{
			const tagItemProto *pProto  = ItemProtoData::Inst()->FindItemProto(dwTypeID);
			if( P_VALID(pProto) )
			{
				dwTypeID = pProto->dwTypeID;
				byQuality = pProto->byQuality;
				m_pIconLingCao[i]->RefreshItem( dwTypeID, 0, byQuality );
				m_pIconLingCao[i]->SetRefresh(true);
			}
			else
				m_pIconLingCao[i]->RefreshItem();
		}

		bool bRelocate = true;
		int nRelocateNum = 0;

		const tagConsolidateItem* pConItem = ConsolidateData::Inst()->FindConsolidateItemProto(dwTypeID);
		if(P_VALID(pConItem))
		{
			for (int j =0; j<MAX_CONSOLIDATE_ROLEATT; ++j )
			{
				if(GT_INVALID == pConItem->tagRoleAtt[j].nAttVal)
					continue;
				nRelocateNum++;

				_sntprintf( szTemp, X_LONG_NAME, _T("ERoleAttribute_%d"), pConItem->tagRoleAtt[j].eRoleAtt );
				TCHAR szName[X_HUGE_NAME] = {0};
				if( MIsValuePct(pConItem->tagRoleAtt[j].nAttVal) )
				{
					INT nValue = MValuePctTrans(pConItem->tagRoleAtt[j].nAttVal) / 1000;
					_sntprintf( szName, X_HUGE_NAME, _T("%s+%d%s"), g_StrTable[szTemp], nValue, _T("%") );	
				}
				else
					_sntprintf( szName, X_HUGE_NAME, _T("%s+%d"), g_StrTable[szTemp], pConItem->tagRoleAtt[j].nAttVal );
				TCHAR szFinal[X_HUGE_NAME] = {0};
				_sntprintf( szFinal, X_LONG_NAME, g_StrTable[_T("TrumpReward")], szName );
				if(0 == j)
					m_pStcTrumpWuXing[i]->SetText(szFinal);
				else if (1 == j)
					m_pStcTrumpWuXing1[i]->SetText(szFinal);
				else if (2 == j)
					m_pStcTrumpWuXing2[i]->SetText(szFinal);
			}
			
		}
		else
		{
			m_pStcTrumpWuXing[i]->SetText(g_StrTable[_T("NoTrumpReward")]);
			//m_pStcTrumpWuXing1[i]->SetText(g_StrTable[_T("NoTrumpReward")]);
			//m_pStcTrumpWuXing2[i]->SetText(g_StrTable[_T("NoTrumpReward")]);
			bRelocate = false;
		}
			

		_sntprintf( szTemp, X_LONG_NAME, _T("ERoleAttribute_%d"), pTrumpSpe->eSlotAttType[i]  );
		if( pTrumpSpe->nSlotAttValue[i] != 0 )
		{
			TCHAR szName[X_HUGE_NAME] = {0};
			if( MIsValuePct(pTrumpSpe->nSlotAttValue[i]) )
			{
				INT nValue = MValuePctTrans(pTrumpSpe->nSlotAttValue[i]) / 100;
				_sntprintf( szName, X_HUGE_NAME, _T("%s+%d%s"), g_StrTable[szTemp], nValue, _T("%") );

			}
			else
				_sntprintf( szName, X_HUGE_NAME, _T("%s+%d"), g_StrTable[szTemp], pTrumpSpe->nSlotAttValue[i] );
			TCHAR szFinal[X_HUGE_NAME] = {0};
			if( pTrumpSpe->bySlotMacthNdx[i] )
					_sntprintf( szFinal, X_LONG_NAME, g_StrTable[_T("TrumpReward")], szName );
			else
				_sntprintf( szFinal, X_LONG_NAME, g_StrTable[_T("TrumpRewardUnusful")], szName );

			if(!bRelocate)
				m_pStcTrumpWuXing1[i]->SetText(szFinal);
			else
			{
				if(1 == nRelocateNum)
					m_pStcTrumpWuXing1[i]->SetText(szFinal);
				else if (2 ==nRelocateNum)
					m_pStcTrumpWuXing2[i]->SetText(szFinal);
				else if (3 == nRelocateNum)
					m_pStcTrumpWuXingAward[i]->SetText(szFinal);
			}
			

		}
		else
		{
			if(!bRelocate)
				m_pStcTrumpWuXing1[i]->SetText(g_StrTable[_T("NoTrumpReward")]);
			else
			{
				if(1 == nRelocateNum)
					m_pStcTrumpWuXing1[i]->SetText(g_StrTable[_T("NoTrumpReward")]);
				else if (2 ==nRelocateNum)
					m_pStcTrumpWuXing2[i]->SetText(g_StrTable[_T("NoTrumpReward")]);
				else if (3 == nRelocateNum)
					m_pStcTrumpWuXingAward[i]->SetText(g_StrTable[_T("NoTrumpReward")]);
			}
			
		}
	}

	if(pTrumpSpe->n16Stage<MIN_STORE_EXP_FABAO_LEVEL)
		return;
	// 经验封存
	DWORD dwCurSaveExp = pTrumpSpe->nCurStoredExp;
	DWORD dwMaxSaveExp = MFaBaoStoredMaxExp(pTrumpSpe->n16Stage, pTrumpSpe->n16NativeIntelligence);
	_sntprintf( szTemp, X_LONG_NAME, _T("%d"), dwCurSaveExp );
	m_pStcFairySaveExp->SetText(szTemp);
	m_pStcFairyCurSaveExp->SetText(szTemp);
	_sntprintf( szTemp, X_LONG_NAME, _T("%d"), dwMaxSaveExp );
	m_pStcFairyMaxSaveExp->SetText(szTemp);
	
	m_pProFairySaveExp->SetMaxValue((float)dwMaxSaveExp);
	m_pProFairySaveExp->SetValue((float)dwCurSaveExp,true);
	m_pProFairySaveExp->SetRefresh();
	if(pTrumpSpe->bStoredExpOpen)
		m_pBtnSaveExp->SetText(g_StrTable[_T("CloseFairySaveExp")]);
	else
		m_pBtnSaveExp->SetText(g_StrTable[_T("OpenFairySaveExp")]);
	m_pIconExpBall->SetPic(_T("data\\ui\\common\\L_diban-b.bmp"));
	m_pIconExpVector->SetPic(_T("data\\ui\\common\\L_diban-b.bmp"));
	m_pBtnTransferExp->SetEnable(false);
	m_pBtnPre->SetEnable(false);
	m_pBtnNext->SetEnable(false);
	m_pProTime->SetMaxValue(3.0f);
	m_pProTime->SetValue(0.0f);
	m_pStcFairyTransExp->SetText(_T("0"));
	m_bSaveExp = pTrumpSpe->bStoredExpOpen;
	m_dwCurSaveExp = pTrumpSpe->nCurStoredExp;
}

VOID TrumpFrame::ClearText()
{
	m_pStcTrumpName->SetText(_T(""));
	m_pStcTrumpType->SetText(_T(""));
	m_pStcTrumpQuality->SetText(_T(""));
	m_pStcTrumpLevel->SetText(_T(""));
	m_pStcEquipLevel->SetText(_T(""));
	m_pStcFairyExp->SetText(_T(""));
	for( int i = 0; i < EWX_End; i++ )
	{
		m_pIconLingCao[i]->SetText(_T(""));
		m_pStcTrumpWuXing[i]->SetText(_T(""));
		m_pStcTrumpWuXing1[i]->SetText(_T(""));
		m_pStcTrumpWuXing2[i]->SetText(_T(""));
		m_pStcTrumpWuXingAward[i]->SetText(_T(""));;
		m_pIconLingCao[i]->RefreshItem(0);
	}
	for( int i = 0; i < 3; i++ )
	{
		m_pStcRoleAtt[i]->SetText(_T(""));
		m_pStcRoleAttValue[i]->SetText(_T(""));
	}
	m_pIconTrump->RefreshItem();
	m_pWnd->SetRefresh();
}

DWORD TrumpFrame::NetTrumpMixBead( tagNS_FabaoInlay *pNetCmd, DWORD )
{
	if( pNetCmd->dwErrorCode == E_Success )
		if( !m_pWnd->IsInvisible() )
		{
			UpdateUI();
			//tagGameEvent m(_T("UpdateFairyAbility"), NULL);
			//m_pMgr->SendEvent(&m);
		}
	return 0;
}

DWORD TrumpFrame::NetTrumpRefine( tagNS_FabaoPractice *pNetCmd, DWORD )
{
	if( pNetCmd->dwErrorCode == E_Success )
		if( !m_pWnd->IsInvisible() )
		{
			UpdateUI();
			tagGameEvent m(_T("UpdateFairyAbility"), NULL);
			m_pMgr->SendEvent(&m);
		}
	return 0;
}

DWORD TrumpFrame::NS_FabaoWuxingMerging(tagNS_FabaoWuxingMerging *pNetCmd,DWORD)
{
	if( pNetCmd->dwErrorCode == E_Success )
		if( !m_pWnd->IsInvisible() )
		{
			UpdateUI();
			//tagGameEvent m(_T("UpdateFairyAbility"), NULL);
			//m_pMgr->SendEvent(&m);
		}
	return 0;
}

DWORD TrumpFrame::NetFairyMoodChange(tagNS_FabaoMoodChanged *pNetCmd,DWORD)
{
	//tagFairyMood m(_T("UpdateFairyMood"), NULL);
	//m.nMood = pNetCmd->nNewMood;
	//m_pMgr->SendEvent(&m);
	UpdateUI();
	return 0;
}

DWORD TrumpFrame::NS_FabaoWuxingReversal(tagNS_FabaoWuxingReversal *pNetCmd,DWORD)
{
	if( pNetCmd->dwErrorCode == E_Success )
		if( !m_pWnd->IsInvisible() )
		{
			UpdateUI();
			//tagGameEvent m(_T("UpdateFairyAbility"), NULL);
			//m_pMgr->SendEvent(&m);
		}
	return 0;
}

DWORD TrumpFrame::CloseAllTrumpChildFrame(tagGameEvent* pEvent)
{
	TrumpMixBeadFrame *pMixBeadFrame = (TrumpMixBeadFrame*)m_pFrameMgr->GetFrame(_T("TrumpMixBead"));
	if(P_VALID(pMixBeadFrame))
		pMixBeadFrame->ShowUI(false);
	TrumpRebornFrame *pRebornFrame = (TrumpRebornFrame*)m_pFrameMgr->GetFrame(_T("TrumpReborn"));
	if(P_VALID(pRebornFrame))
		pRebornFrame->ShowUI(false);
	RefineTrumpFrame *pRefineFrame = (RefineTrumpFrame*)m_pFrameMgr->GetFrame(_T("RefineTrump"));
	if(P_VALID(pRefineFrame))
		pRefineFrame->ShowUI(false);

	return 0;
}

INT TrumpFrame::GetLingCaoNum()
{
	if( !P_VALID(m_pTrump) )
		return 0;

	const tagFabaoSpec *pTrumpSpe = m_pTrump->GetFaBaoSpe();
	if( !P_VALID(pTrumpSpe) )
		return 0;
	if( pTrumpSpe->n16Stage >= 60 )
		return 5;
	else if( pTrumpSpe->n16Stage >= 40 )
		return 4;
	else if( pTrumpSpe->n16Stage >= 30 )
		return 3;
	else if( pTrumpSpe->n16Stage >= 20 )
		return 2;
	else if( pTrumpSpe->n16Stage >= 10 )
		return 1;
	else
		return 0;
}

DWORD TrumpFrame::OnUpdateFairyMood(tagFairyMood* pEvent)
{
	UpdateMoodInfo(pEvent->nMood);
	return 0;
}

DWORD TrumpFrame::OnFairyExcited(tagFairyExcited* pEvent)
{
	m_dwFairyMoodBuffer = pEvent->dwBuffID;
	const tagBuffProtoClient* pBuffProto = SkillProtoData::Inst()->FindBuffProto(pEvent->dwBuffID);
	if(P_VALID(pBuffProto))
		m_pStcEffect->SetText(pBuffProto->szDesc);
	else
		m_pStcEffect->SetText(g_StrTable[_T("FashionCompose5")]);
	return 0;
}

VOID TrumpFrame::UpdateMoodInfo(INT nMood)
{
	INT nIndex = 0;
	INT nExp = 50;
	INT nAbility = 60;
	if( nMood < 200 )
	{
		nIndex = 1;
		nExp = 50;
		nAbility = 60;
	}
	else if( nMood < 400 )
	{
		nIndex = 2;
		nExp = 60;
		nAbility = 80;
	}
	else if( nMood < 600 )
	{
		nIndex = 3;
		nExp = 70;
		nAbility = 85;
	}
	else if( nMood < 800 )
	{
		nIndex = 4;
		nExp = 80;
		nAbility = 90;
	}
	else if( nMood < 1000 )
	{
		nIndex = 5;
		nExp = 100;
		nAbility = 100;
	}
	else
	{
		nIndex = 6;
		nExp = 110;
		nAbility = 110;
	}
	m_pProFairyMood->SetValue((FLOAT)nMood, true);
	TCHAR szTemp[X_LONG_NAME] = {0};
	_sntprintf( szTemp, X_LONG_NAME, _T("%d/%d"), nMood, 2000);
	m_pProFairyMood->SetText(szTemp);
	_sntprintf( szTemp, X_LONG_NAME, _T("data\\ui\\pet\\SM_fairy_%d.bmp"), nIndex);
	m_pIconMoodPic->SetPic(szTemp);
	_sntprintf( szTemp, X_LONG_NAME, _T("%d%s"), nExp, _T("%"));
	m_pStcFairyExpRate->SetText(szTemp);
	_sntprintf( szTemp, X_LONG_NAME, _T("%d%s"), nAbility, _T("%"));
	TCHAR szFinal[X_LONG_NAME] = {0};
	if(nAbility < 100)
		_sntprintf( szFinal, X_LONG_NAME, g_StrTable[_T("FairyAbilityNoFull")], szTemp );
	else
		_sntprintf( szFinal, X_LONG_NAME, g_StrTable[_T("FairyAbilityFull")], szTemp );

	for( int i = 0; i < 3; i++ )
	{
		m_pStcRoleAttValueRate[i]->SetInvisible(TRUE);
		if( m_pTrump->GetFaBaoSpe()->eAttType[i] != ERA_Null )
		{
			m_pStcRoleAttValueRate[i]->SetText(szFinal);
			m_pStcRoleAttValueRate[i]->SetInvisible(FALSE);
		}
	}

	if(GameSet::Inst()->Get(EGS_FairyAutoFeed))
		m_pIconAutoFeed->SetInvisible(FALSE);
	else
		m_pIconAutoFeed->SetInvisible(TRUE);

	const tagBuffProtoClient* pBuffProto = SkillProtoData::Inst()->FindBuffProto(m_dwFairyMoodBuffer);
	if(P_VALID(pBuffProto))
		m_pStcEffect->SetText(pBuffProto->szName);
	else
		m_pStcEffect->SetText(g_StrTable[_T("FashionCompose5")]);

	m_pStcFairyMood->SetInvisible(FALSE);
	m_pIconMoodPic->SetInvisible(FALSE);
	m_pBtnAutoFeed->SetInvisible(FALSE);
	m_pStcEffect->SetInvisible(FALSE);
	m_pStcEffectInfo->SetInvisible(FALSE);
	m_pStcFairyExpInfo->SetInvisible(FALSE);
	m_pStcFairyExpInfo->SetInvisible(FALSE);
	m_pStcFairyMoodInfo->SetInvisible(FALSE);
	m_pStcFairyExpRate->SetInvisible(FALSE);
}

void TrumpFrame::ClearExpSaveInfo()
{
	ItemFrame* pFrame = (ItemFrame*)TObjRef<GameFrameMgr>()->GetFrame(_T("Item"));
	if(P_VALID(pFrame) && EIST_FairyTransferExp == pFrame->GetCurStrategy() )
		SetStrategy(EIST_ItemPocket);
	if(m_bProcessing)
	{
		m_bProcessing = false;
		m_pProTime->SetValue(0.0f);
	}
	LockItem(false);
	if(m_pPbnFairySoul->GetState() != EGUIBS_PushDown)
		m_pPbnFairySoul->SetState(EGUIBS_PushDown);
	m_n64ExpBall = GT_INVALID;
	UpdateUI();
}

DWORD TrumpFrame::NoNS_ExpBallBind2Role( tagNS_ExpBallBind2Role *pNetCmd, DWORD)
{
	if (pNetCmd->dwErrorCode != E_Success)
	{
		switch(pNetCmd->dwErrorCode)
		{
		case E_FabaoStoreExp_NoExpBall:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_FabaoStoreExp_NoExpBall")]);
			break;
		case E_FabaoStoreExp_NoFabao:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_FabaoStoreExp_NoFabao")]);
			break;
		case E_FabaoStoreExp_BagFull:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_FabaoStoreExp_BagFull")]);
			break;
		case E_FabaoStoreExp_BallBinded:
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("E_FabaoStoreExp_BallBinded")]);
			break;
		}
	}
	return 0;
}

VOID TrumpFrame::RefreshSpriteBornUI()
{
	const tagFabaoSpec *pTrumpSpe = m_pTrump->GetFaBaoSpe();
	const tagFabaoProto *pTrumpProto = (tagFabaoProto*)m_pTrump->GetProto();
	if(!P_VALID(pTrumpProto) || !(pTrumpSpe))
		return;

	TCHAR szBuffer[X_SHORT_NAME] = {0};
	_sntprintf(szBuffer, X_SHORT_NAME, _T("%d"), pTrumpSpe->nBirthTimes);
	m_pStMatingTime->SetText(szBuffer);
	/*
	switch(pTrumpSpe->eFabaoStatus)
	{
	case EEFS_Normal:
		{
			m_pStSpriteStateNormal->SetInvisible(FALSE);		
			m_pStSpriteStatePregnancy->SetInvisible(TRUE);	
			m_pStSpriteStateBorn->SetInvisible(TRUE);			
			m_pStSpriteStateRest->SetInvisible(TRUE);			
		}
		break;
	case EEFS_Pregnancy:
		{
			m_pStSpriteStateNormal->SetInvisible(TRUE);		
			m_pStSpriteStatePregnancy->SetInvisible(FALSE);	
			m_pStSpriteStateBorn->SetInvisible(TRUE);			
			m_pStSpriteStateRest->SetInvisible(TRUE);	
		}
		break;
	case EEFS_GiveBirth:
		{
			m_pStSpriteStateNormal->SetInvisible(TRUE);		
			m_pStSpriteStatePregnancy->SetInvisible(TRUE);	
			m_pStSpriteStateBorn->SetInvisible(FALSE);			
			m_pStSpriteStateRest->SetInvisible(TRUE);	
		}
		break;
	case EEFS_Rest:
		{
			m_pStSpriteStateNormal->SetInvisible(TRUE);		
			m_pStSpriteStatePregnancy->SetInvisible(TRUE);	
			m_pStSpriteStateBorn->SetInvisible(TRUE);			
			m_pStSpriteStateRest->SetInvisible(FALSE);	
		}
		break;
	}

	if (pTrumpSpe->eFabaoStatus ==EEFS_Normal ||  pTrumpSpe->eFabaoStatus ==EEFS_GiveBirth )
	{
		m_pStHour->SetText(_T("--"));
		m_pStMIn->SetText(_T("--"));
	}
	else
	{
		// todo		改成向服务器要时间
		int hour = pTrumpSpe->dwStatusChangeTime/(60*60);
		int min = pTrumpSpe->dwStatusChangeTime%60;
		if (pTrumpSpe->dwStatusChangeTime == 0)
		{
			m_pStHour->SetText(_T("--"));
			m_pStMIn->SetText(_T("--"));
		}
		else
		{
			_sntprintf(szBuffer, X_SHORT_NAME, _T("%d"), hour);
			m_pStHour->SetText(szBuffer);
			_sntprintf(szBuffer, X_SHORT_NAME, _T("%d"), min);
			m_pStMIn->SetText(szBuffer);
		}
	}
	*/
}

DWORD TrumpFrame::NoNS_InquireSpirteBornState( tagNS_InquireSpirteBornState *pNetCmd, DWORD)
{
	TCHAR szBuffer[X_SHORT_NAME] = {0};

	switch(pNetCmd->eFabaoStatus)
	{
	case EEFS_Normal:
		{
			m_pStSpriteStateNormal->SetInvisible(FALSE);		
			m_pStSpriteStatePregnancy->SetInvisible(TRUE);	
			m_pStSpriteStateBorn->SetInvisible(TRUE);			
			m_pStSpriteStateRest->SetInvisible(TRUE);			
		}
		break;
	case EEFS_Pregnancy:
		{
			m_pStSpriteStateNormal->SetInvisible(TRUE);		
			m_pStSpriteStatePregnancy->SetInvisible(FALSE);	
			m_pStSpriteStateBorn->SetInvisible(TRUE);			
			m_pStSpriteStateRest->SetInvisible(TRUE);	
		}
		break;
	case EEFS_GiveBirth:
		{
			m_pStSpriteStateNormal->SetInvisible(TRUE);		
			m_pStSpriteStatePregnancy->SetInvisible(TRUE);	
			m_pStSpriteStateBorn->SetInvisible(FALSE);			
			m_pStSpriteStateRest->SetInvisible(TRUE);	
		}
		break;
	case EEFS_Rest:
		{
			m_pStSpriteStateNormal->SetInvisible(TRUE);		
			m_pStSpriteStatePregnancy->SetInvisible(TRUE);	
			m_pStSpriteStateBorn->SetInvisible(TRUE);			
			m_pStSpriteStateRest->SetInvisible(FALSE);	
		}
		break;
	}

	if (pNetCmd->eFabaoStatus==EEFS_Normal ||  pNetCmd->eFabaoStatus ==EEFS_GiveBirth )
	{
		m_pStHour->SetText(_T("--"));
		m_pStMIn->SetText(_T("--"));
	}
	else
	{
		// todo		改成向服务器要时间
		int hour = pNetCmd->dwtagDwordTime/(60*60);
		int min = (pNetCmd->dwtagDwordTime - hour*60*60)/60;
		if (pNetCmd->dwtagDwordTime == 0)
		{
			m_pStHour->SetText(_T("--"));
			m_pStMIn->SetText(_T("--"));
		}
		else
		{
			_sntprintf(szBuffer, X_SHORT_NAME, _T("%d"), hour);
			m_pStHour->SetText(szBuffer);
			_sntprintf(szBuffer, X_SHORT_NAME, _T("%d"), min);
			m_pStMIn->SetText(szBuffer);
		}
	}
	return 0;
}