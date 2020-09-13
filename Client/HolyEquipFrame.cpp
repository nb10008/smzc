#include "StdAfx.h"
#include "HolyEquipFrame.h"
#include "HolyManFrame.h"
#include "HolyManMgr.h"
#include "ItemButton.h"
#include "DragBtn.h"
#include "Item.h"
#include "PocketFrame.h"
#include "Container.h"
#include "ItemFrame.h"
#include "ItemMgr.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "ItemEvent.h"
#include "..\WorldDefine\msg_compose.h"
#include "..\WorldDefine\compose_define.h"
#include "..\WorldDefine\container_define.h"
#include "..\WorldDefine\msg_yuanshen.h"
//#include "EquipMaintainValueFrame.h"
#include "CurrencyMgr.h"
#include "StaticEx.h"
#include "ItemProtoData.h"
#include "CurrencyMgr.h"

const FLOAT		g_fProHolyComposeTime	= 2.0f;

const TCHAR *szSoulPicBlue		= _T("data\\ui\\soul\\L_soul_blue.bmp");
const TCHAR *szSoulPicGreen	= _T("data\\ui\\soul\\L_soul_green.bmp");
const TCHAR *szSoulPicOrange	= _T("data\\ui\\soul\\L_soul_orange.bmp");
const TCHAR *szSoulPicPurple	= _T("data\\ui\\soul\\L_soul_purple.bmp");
const TCHAR *szSoulPicYellow	= _T("data\\ui\\soul\\L_soul_yellow.bmp");
const TCHAR *szSoulPicNone	= _T("data\\ui\\soul\\L_soul_00.bmp");
const TCHAR *szSoulPicHole		= _T("data\\ui\\soul\\L_soul_01.bmp");

HolyEquipFrame::HolyEquipFrame(void)
:m_Trunk(this)
,m_pWndFather(NULL)
,m_pWnd(NULL)
,m_pIBEquip(NULL)
,m_pEquip(NULL)
//冲星分页
,m_pIBComposeItem(NULL)
,m_pItemCompose(NULL)
,m_pPthCompose(NULL)
,m_pStcStarLevel(NULL)
,m_pStcSuccRate(NULL)
,m_pStcReturnLv(NULL)
,m_pStcAddAttName(NULL)
,m_pStcAddAttValue(NULL)
,m_pStcCostGold(NULL)
,m_pStcCostSilver(NULL)
,m_pProHolyCompose(NULL)
,m_pBtnStartComp(NULL)
,m_pBtnCancelComp(NULL)
,m_pPBItemNum(NULL)
,m_pPBToLevel(NULL)
,m_pEBItemNum(NULL)
,m_pEBToLevel(NULL)
,m_pStcSucc(NULL)
,m_pStcFail(NULL)
,m_eCurCtrlType(EECT_End)
,m_bIfProgress(FALSE)
,m_escapeTime(0)
,m_nItemCostNum(0)
//镶嵌分页
,m_pPthInlay(NULL)
,m_pIBInlayItem(NULL)
,m_pItemInlayItem(NULL)
,m_pIBInlayStuff(NULL)
,m_pItemInlayStuff(NULL)
,m_pBtnInlayMinStuff(NULL)
,m_pBtnInlayAddStuff(NULL)
,m_pStcInlayRate(NULL)
,m_pStcInlayGold(NULL)
,m_pStcInlaySilver(NULL)
,m_pProInlayOnce(NULL)
,m_pBtnInlayBegin(NULL)
,m_pBtnInlayExtirpate(NULL)
,m_pBtnInlayCancel(NULL)
,m_nInlayStuffNum(0)
,m_nInlayPos(GT_INVALID)
,m_eHolyInlayType(EHIT_Null)
//凿孔分页
,m_pPthChisel(NULL)
,m_pIBChiselItem(NULL)
,m_pItemChisel(NULL)
,m_pStcHaveHoles(NULL)
,m_pStcChiselRate(NULL)
,m_pStcChiselGold(NULL)
,m_pStcChiselSilver(NULL)
,m_pProChiselOnce(NULL)
,m_pBtnChiselBegin(NULL)
,m_pBtnChiselCancel(NULL)

{
	for ( int i=0; i<EECT_End; ++i )
	{
		m_pBtnPageCompose[i] = NULL;
		m_pBtnPageInlay[i] = NULL;
		m_pBtnPageChisel[i] = NULL;
	}

	for ( int j=0; j<MAX_STARENHANCE_LEVEL; ++j )
	{
		m_pStcStarNum[j] = NULL;
	}

	for ( int k=0; k<MAX_EQUIPHOLE_NUM; ++k )
	{
		m_pSteInlayHole[k] = NULL;
		m_pSteChiselHole[k] = NULL;
	}



}

HolyEquipFrame::~HolyEquipFrame(void)
{
}

BOOL HolyEquipFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam);

	// 注册游戏事件处理函数
	m_pMgr->RegisterEventHandle(_T("tagSpecItemEvent"),				(FRAMEEVENTPROC)m_Trunk.sfp1(&HolyEquipFrame::OnUseItemEvent));
	m_pMgr->RegisterEventHandle(_T("ItemEvent_Move2Compose"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&HolyEquipFrame::OnEquipPosEvent));
	m_pMgr->RegisterEventHandle(_T("EquipDrop"),							(FRAMEEVENTPROC)m_Trunk.sfp1(&HolyEquipFrame::OnItemDropCheck));
	m_pMgr->RegisterEventHandle(_T("ItemEvent_Move2Equip"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&HolyEquipFrame::OnChangeItemEvent));
	//m_pMgr->RegisterEventHandle(_T("RefreshMaintainValueUI"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&HolyEquipFrame::RevRefreshFrame));

	m_pMgr->RegisterEventHandle(_T("MsgBox_UninlayConfirm"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&HolyEquipFrame::OnMsgBox_UninlayConfirm));

	m_pCmdMgr->Register("NS_ItemRemove",(NETMSGPROC)m_Trunk.sfp2(&HolyEquipFrame::NetItemRemove), _T("NS_ItemRemove"));
	m_pCmdMgr->Register("NS_StarEnhance",(NETMSGPROC)m_Trunk.sfp2(&HolyEquipFrame::OnNS_StarEnhance), _T("NS_StarEnhance"));
	m_pCmdMgr->Register("NS_HolyEquipHole",(NETMSGPROC)m_Trunk.sfp2(&HolyEquipFrame::OnNS_HolyEquipHole), _T("NS_HolyEquipHole"));
	m_pCmdMgr->Register("NS_HolyEquipInlay",(NETMSGPROC)m_Trunk.sfp2(&HolyEquipFrame::OnNS_HolyEquipInlay), _T("NS_HolyEquipInlay"));
	m_pCmdMgr->Register("NS_HolyEquipRemove",(NETMSGPROC)m_Trunk.sfp2(&HolyEquipFrame::OnNS_HolyEquipRemove), _T("NS_HolyEquipRemove"));

	return bRet;
}

BOOL HolyEquipFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();

	// 注销游戏事件处理函数
	m_pMgr->UnRegisterEventHandler(_T("tagSpecItemEvent"),					(FRAMEEVENTPROC)m_Trunk.sfp1(&HolyEquipFrame::OnUseItemEvent));
	m_pMgr->UnRegisterEventHandler(_T("ItemEvent_Move2Compose"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&HolyEquipFrame::OnEquipPosEvent));
	m_pMgr->UnRegisterEventHandler(_T("EquipDrop"),								(FRAMEEVENTPROC)m_Trunk.sfp1(&HolyEquipFrame::OnItemDropCheck));
	m_pMgr->UnRegisterEventHandler(_T("ItemEvent_Move2Equip"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&HolyEquipFrame::OnChangeItemEvent));
	//m_pMgr->UnRegisterEventHandler(_T("RefreshMaintainValueUI"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&HolyEquipFrame::RevRefreshFrame));

	m_pMgr->UnRegisterEventHandler(_T("MsgBox_UninlayConfirm"),			(FRAMEEVENTPROC)m_Trunk.sfp1(&HolyEquipFrame::OnMsgBox_UninlayConfirm));

	m_pCmdMgr->UnRegister( "NS_StarEnhance", (NETMSGPROC)m_Trunk.sfp2(&HolyEquipFrame::OnNS_StarEnhance) );
	m_pCmdMgr->UnRegister("NS_ItemRemove",(NETMSGPROC)m_Trunk.sfp2(&HolyEquipFrame::NetItemRemove));
	
	m_pCmdMgr->UnRegister( "NS_HolyEquipHole", (NETMSGPROC)m_Trunk.sfp2(&HolyEquipFrame::OnNS_HolyEquipHole) );
	m_pCmdMgr->UnRegister("NS_HolyEquipInlay",(NETMSGPROC)m_Trunk.sfp2(&HolyEquipFrame::OnNS_HolyEquipInlay));
	m_pCmdMgr->UnRegister("NS_HolyEquipRemove",(NETMSGPROC)m_Trunk.sfp2(&HolyEquipFrame::OnNS_HolyEquipRemove));

	m_pEquip = NULL;
	m_pItemCompose = NULL;
	m_pItemInlayItem = NULL;
	m_pItemInlayStuff = NULL;
	m_pItemChisel = NULL;

	if(P_VALID(GetObj("MsgBox_UninlayConfirm")))
		KillObj("MsgBox_UninlayConfirm");

	return bRet;
}

BOOL HolyEquipFrame::ReloadUI()
{
	// 创建

	m_pWndFather = m_pGUI->GetWnd(((HolyManFrame*)m_pFather)->GetGUIFullName().c_str());
	m_pWnd = m_pWndFather->GetChild(_T("equip"));

	// 注册事件处理函数
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&HolyEquipFrame::EventHandler));

	m_pIBEquip					= (ItemButton*)m_pWnd->GetChild( _T("itemback\\item") );	
	m_pStcSucc					= (GUIStatic*)m_pWnd->GetChild( _T("success") );	 
	m_pStcFail					= (GUIStatic*)m_pWnd->GetChild( _T("fail") );	
	//----------冲星----------
	m_pPthCompose			= (GUIPatch*)m_pWnd->GetChild( _T("starback") );	
	m_pIBComposeItem	= (ItemButton*)m_pPthCompose->GetChild( _T("itemback\\item") );  
	m_pBtnPageCompose[EECT_Compose]	= (GUIPushButton*)m_pPthCompose->GetChild(_T("starbutt"));	//冲星
	m_pBtnPageCompose[EECT_Chisel]		= (GUIPushButton*)m_pPthCompose->GetChild(_T("holebutt"));	//凿孔
	m_pBtnPageCompose[EECT_Inlay]			= (GUIPushButton*)m_pPthCompose->GetChild(_T("studbutt"));	//镶嵌
	m_pStcStarLevel			= (GUIStatic*)m_pPthCompose->GetChild( _T("word1\\num") );		
	m_pStcSuccRate			= (GUIStatic*)m_pPthCompose->GetChild( _T("word2\\num") );	
	m_pStcReturnLv			= (GUIStatic*)m_pPthCompose->GetChild( _T("word3\\num") );	
	m_pStcAddAttName	= (GUIStatic*)m_pPthCompose->GetChild( _T("word4\\type") );	
	m_pStcAddAttValue	= (GUIStatic*)m_pPthCompose->GetChild( _T("word4\\num") );	
	m_pStcCostGold			= (GUIStatic*)m_pPthCompose->GetChild( _T("word5\\num0") );	
	m_pStcCostSilver		= (GUIStatic*)m_pPthCompose->GetChild( _T("word5\\num1") );	
	m_pProHolyCompose	= (GUIProgress*)m_pPthCompose->GetChild( _T("progpic\\pro") );	
	m_pBtnStartComp		= (GUIButton*)m_pPthCompose->GetChild( _T("start") );	
	m_pBtnCancelComp	= (GUIButton*)m_pPthCompose->GetChild( _T("cancle") );
	m_pPBItemNum			= (GUIPushButton*)m_pPthCompose->GetChild( _T("fra\\opt0") );
	m_pPBToLevel			= (GUIPushButton*)m_pPthCompose->GetChild( _T("fra\\opt1") );
	m_pEBItemNum			= (GUIEditBox*)m_pPthCompose->GetChild( _T("fra\\word0\\num") );
	m_pEBToLevel				= (GUIEditBox*)m_pPthCompose->GetChild( _T("fra\\word1\\num") );
	TCHAR buffer1[X_LONG_NAME];
	for ( int i=0; i<MAX_STARENHANCE_LEVEL; ++i )
	{
		_sntprintf(buffer1, sizeof(buffer1)/sizeof(TCHAR), _T("star%d"), i );
		GUIStatic*	 pStcStarFather = (GUIStatic*)m_pPthCompose->GetChild( buffer1 );	
		pStcStarFather->SetInvisible(FALSE);
		m_pStcStarNum[i]		= (GUIStatic*)pStcStarFather->GetChild( _T("get") );	
	}

	//----------镶嵌----------
	m_pPthInlay			=		(GUIPatch*)m_pWnd->GetChild( _T("studback") );	
	m_pIBInlayItem	=		(ItemButton*)m_pPthInlay->GetChild( _T("itemback0\\item") );	
	m_pIBInlayStuff	=		(ItemButton*)m_pPthInlay->GetChild( _T("itemback1\\item") );	
	m_pBtnInlayMinStuff	=		(GUIButton*)m_pPthInlay->GetChild( _T("minusbutt") );	
	m_pBtnInlayAddStuff	=		(GUIButton*)m_pPthInlay->GetChild( _T("plusbutt") );	
	m_pStcInlayRate			=		(GUIStatic*)m_pPthInlay->GetChild( _T("word3\\num") );	
	m_pStcInlayGold			=		(GUIStatic*)m_pPthInlay->GetChild( _T("word4\\num0") );	
	m_pStcInlaySilver		=		(GUIStatic*)m_pPthInlay->GetChild( _T("word4\\num1") );	
	m_pProInlayOnce		=		(GUIProgress*)m_pPthInlay->GetChild( _T("progpic\\pro") );	
	m_pBtnInlayBegin		=		(GUIButton*)m_pPthInlay->GetChild( _T("addbutt") );	
	m_pBtnInlayExtirpate	=		(GUIButton*)m_pPthInlay->GetChild( _T("delbutt") );	
	m_pBtnInlayCancel		=		(GUIButton*)m_pPthInlay->GetChild( _T("cancle") );	
	m_pBtnPageInlay[EECT_Inlay]			=		(GUIPushButton*)m_pPthInlay->GetChild( _T("studbutt") );	
	m_pBtnPageInlay[EECT_Chisel]		=		(GUIPushButton*)m_pPthInlay->GetChild( _T("holebutt") );	
	m_pBtnPageInlay[EECT_Compose]	=		(GUIPushButton*)m_pPthInlay->GetChild( _T("starbutt") );	
	for ( int i=0; i<MAX_EQUIPHOLE_NUM; ++i )
	{
		_sntprintf(buffer1, sizeof(buffer1)/sizeof(TCHAR), _T("holeback\\hole%d"), i );
		m_pSteInlayHole[i]		= (StaticEx*)m_pPthInlay->GetChild( buffer1 );	
		m_pBtnInlayHole[i]		= (GUIPushButton*)m_pSteInlayHole[i]	->GetChild( _T("choose") );	
	}

	//----------凿孔----------
	m_pPthChisel				=	(GUIPatch*)m_pWnd->GetChild( _T("holeback") );	
	m_pIBChiselItem			=	(ItemButton*)m_pPthChisel->GetChild( _T("itemback\\item") );	
	m_pStcHaveHoles		=	(GUIStatic*)m_pPthChisel->GetChild( _T("word1\\num") );	
	m_pStcChiselRate		=	(GUIStatic*)m_pPthChisel->GetChild( _T("word2\\num") );	
	m_pStcChiselGold		= (GUIStatic*)m_pPthChisel->GetChild( _T("word3\\num0") );	
	m_pStcChiselSilver		= (GUIStatic*)m_pPthChisel->GetChild( _T("word3\\num1") );	
	m_pProChiselOnce		= (GUIProgress*)m_pPthChisel->GetChild( _T("progpic\\pro") );	
	m_pBtnChiselBegin		= (GUIButton*)m_pPthChisel->GetChild( _T("start") );	
	m_pBtnChiselCancel	= (GUIButton*)m_pPthChisel->GetChild( _T("cancle") );	
	m_pBtnPageChisel[EECT_Inlay]			=		(GUIPushButton*)m_pPthChisel->GetChild( _T("studbutt") );	
	m_pBtnPageChisel[EECT_Chisel]			=		(GUIPushButton*)m_pPthChisel->GetChild( _T("holebutt") );	
	m_pBtnPageChisel[EECT_Compose]	=		(GUIPushButton*)m_pPthChisel->GetChild( _T("starbutt") );	
	for ( int i=0; i<MAX_EQUIPHOLE_NUM; ++i )
	{
		_sntprintf(buffer1, sizeof(buffer1)/sizeof(TCHAR), _T("holeback\\hole%d"), i );
		m_pSteChiselHole[i]	= (StaticEx*)m_pPthChisel->GetChild( buffer1 );	
	}

	m_pWnd->SetInvisible(TRUE);

	return TRUE;
}

VOID HolyEquipFrame::Update()
{
	GameFrame::Update();

	if ( !P_VALID(m_pWnd) || m_pWnd->IsInvisible() )
		return;

	//检测是否离NPC过远，过远则关闭frame
	m_escapeTime += Kernel::Inst()->GetDeltaTimeDW();
	if( m_escapeTime > 1000 )//每隔1秒检测
	{
		m_escapeTime = 0;
		if (P_VALID(m_pStcSucc))
			m_pStcSucc->SetInvisible(TRUE);
		if (P_VALID(m_pStcFail))
			m_pStcFail->SetInvisible(TRUE);
	}

	if (m_bIfProgress)
	{
		if (P_VALID(m_pWnd)&&(!m_pWnd->IsInvisible()))
		{

			if ( EECT_Chisel == m_eCurCtrlType )
			{
				float fCurVal = m_pProChiselOnce->GetCurrentValue();
				if ( fCurVal >= g_fProHolyComposeTime )
					SendHolyChiselMsg();
			}
			else if ( EECT_Compose == m_eCurCtrlType )
			{
				float fCurVal = m_pProHolyCompose->GetCurrentValue();
				if ( fCurVal >= g_fProHolyComposeTime )
					SendHolyComposeMsg();
			}
			else if ( EECT_Inlay == m_eCurCtrlType )
			{
				float fCurVal = m_pProInlayOnce->GetCurrentValue();
				if ( fCurVal >= g_fProHolyComposeTime && EHIT_Inlay == m_eHolyInlayType )
					SendHolyInlayMsg();
			}


			
		}
	}

}

DWORD HolyEquipFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	switch (pEvent->eEvent)
	{
	case EGUIE_Click:
		{	
			if ( pWnd ==(GUIWnd*)m_pIBEquip )
			{
				if (P_VALID(m_pEquip)&&(!m_bIfProgress)) 		
				{
					StopHolyCompose();
					StopHolyInlay();
					StopHolyCompose();
					ClearEquip();
				}
			}
			else if ( EECT_Compose == m_eCurCtrlType )//充星
			{
				if (pWnd == m_pBtnStartComp) 
				{
					if (CanHolyCompose()&&(!m_bIfProgress))
						BeginHolyCompose();
				}
				else if (pWnd == m_pBtnCancelComp) 
				{
					if (m_bIfProgress)
						StopHolyCompose();
				}
				else if ( pWnd ==(GUIWnd*)m_pIBComposeItem )
				{
					if (P_VALID(m_pItemCompose)&&(!m_bIfProgress)) 		
					{
						ClearUI(EECT_Compose);
						m_pItemCompose = NULL;
						m_nItemCostNum = 0;
					}
				}
				
			}
			else if ( EECT_Chisel == m_eCurCtrlType )//凿空
			{
				if ( pWnd ==(GUIWnd*)m_pBtnChiselBegin )
				{
					if (CanHolyChisel()&&(!m_bIfProgress))
						BeginHolyChisel();
				}
				else if ( pWnd ==(GUIWnd*)m_pBtnChiselCancel )
				{					
					if (m_bIfProgress)
						StopHolyChisel();
				}
				else if ( pWnd ==(GUIWnd*)m_pIBChiselItem )
				{
					if (P_VALID(m_pItemChisel)&&(!m_bIfProgress)) 		
					{
						StopHolyChisel();
						ClearUI(EECT_Chisel);
						m_pItemChisel = NULL;
					}
				}
			}
			else if ( EECT_Inlay == m_eCurCtrlType )//镶嵌
			{
				if ( pWnd ==(GUIWnd*)m_pBtnInlayBegin )
				{
					if (CanHolyInlay()&&(!m_bIfProgress))
						BeginHolyInlay();
				}
				else if ( pWnd ==(GUIWnd*)m_pBtnInlayExtirpate )
				{
					CanHolyUninlay();
				}
				else if ( pWnd ==(GUIWnd*)m_pBtnInlayCancel )
				{					
					if (m_bIfProgress)
						StopHolyInlay();
				}
				else if ( pWnd ==(GUIWnd*)m_pIBInlayItem )
				{
					if (P_VALID(m_pItemInlayItem)&&(!m_bIfProgress)) 		
					{
						StopHolyInlay();
						m_pIBInlayItem->RefreshItem();
						m_pItemInlayItem = NULL;
						m_nInlayStuffNum	= 0;
						m_nInlayPos			= GT_INVALID;
						RefreshInlayUI();
					}
				}
				else if ( pWnd ==(GUIWnd*)m_pIBInlayStuff )
				{
					if (P_VALID(m_pItemInlayStuff)&&(!m_bIfProgress)) 		
					{
						StopHolyInlay();
						m_pIBInlayStuff->RefreshItem();
						m_pItemInlayStuff = NULL;
						m_nInlayStuffNum	= 0;
						m_nInlayPos			= GT_INVALID;
						RefreshInlayUI();
					}
				}
				else if ( pWnd ==(GUIWnd*)m_pBtnInlayMinStuff )
				{
					if ( P_VALID(m_pEquip) && MIsEquipment(m_pEquip->GetItemTypeID()) && P_VALID(m_pItemInlayStuff) )
					{
						Equipment* pEquip = (Equipment*)m_pEquip;
						if ( m_nInlayStuffNum > 0 && m_nInlayPos>=0 && m_nInlayPos<pEquip->GetEquipMaxHoleNum() )
						{
							-- m_nInlayStuffNum ;
							RefreshInlayUI();
						}
					}
				}
				else if ( pWnd ==(GUIWnd*)m_pBtnInlayAddStuff )
				{
					ItemContainer* pPocket =  ItemMgr::Inst()->GetPocket();
					if ( P_VALID(m_pItemInlayStuff) && P_VALID(pPocket) 
						&& P_VALID(m_pEquip) && MIsEquipment(m_pEquip->GetItemTypeID()) )
					{	 
						Equipment* pEquip = (Equipment*)m_pEquip; //已知是装备了才转换
						INT nCount = 0;//= pPocket->GetItemQuantity(TRANS_EQUIPSOUL_ITEMID1);
						//nCount += pPocket->GetItemQuantity(TRANS_EQUIPSOUL_ITEMID2);
						if ( m_nInlayPos>=0 && m_nInlayPos<pEquip->GetEquipMaxHoleNum() )
						{
							INT nRate = (HolyManMgr::Inst()->GetInlayRate(m_nInlayPos+1))/100;
							INT nTmpCount = (100-nRate)/IMPROVE_SOULINLAY_RATE;
							if ( nTmpCount < nCount )
								nCount = nTmpCount;
							if ( nCount > m_nInlayStuffNum  )
							{
								++ m_nInlayStuffNum ;
								RefreshInlayUI();
							}
						}	 
					}
				}
			}

		}
		break;
	case EGUISCB_Drop:
		{
			if ( FALSE == m_bIfProgress )//读条过程中不能Drop
			{
				if (  pWnd == (GUIWnd*)m_pIBEquip 
					|| pWnd == (GUIWnd*)m_pIBComposeItem 
					|| pWnd == (GUIWnd*)m_pIBInlayItem 
					|| pWnd == (GUIWnd*)m_pIBInlayStuff 
					|| pWnd == (GUIWnd*)m_pIBChiselItem		)
				{
					if( EDT_Item == (EDragType)pEvent->dwParam1 )
						AddItem((INT16)LOWORD(pEvent->dwParam2));
				}
			}
		}
		break;
	case EGUIE_PushButtonDown:
		{
			if( pEvent->dwParam1 != 1 )
				return 0;

			if ( EECT_Compose == m_eCurCtrlType )
			{
				if (!m_bIfProgress)
				{
					if ( pWnd ==(GUIWnd*)m_pPBItemNum )
					{
						ChangeItemNum();
						m_pEBToLevel->SetText(_T("0"));
					} 
					else if ( pWnd ==(GUIWnd*)m_pPBToLevel )
					{
						m_pEBItemNum->SetText(_T("0"));
						m_pEBToLevel->SetText(_T("0"));
						m_nItemCostNum = 0;
					}
				}

				for ( int i=0; i<EECT_End; ++i )
				{
					if ( pWnd ==(GUIWnd*)m_pBtnPageCompose[i] && EECT_Compose != EHolyEquipCtrlType(i) )
					{
						SwitchPage( EHolyEquipCtrlType(i) );
						break;
					}
				}
			}
			else if ( EECT_Chisel == m_eCurCtrlType )
			{
				for ( int i=0; i<EECT_End; ++i )
				{
					if ( pWnd ==(GUIWnd*)m_pBtnPageChisel[i] && EECT_Chisel != EHolyEquipCtrlType(i) )
					{
						SwitchPage( EHolyEquipCtrlType(i) );
						break;
					}
				}
			}
			else if ( EECT_Inlay == m_eCurCtrlType )
			{
				 if ( (_tcsnccmp(pWnd->GetName().c_str(), _T("choose"), 6) == 0)  && !m_bIfProgress )
				 {
					 for ( int j=0; j<MAX_EQUIPHOLE_NUM; ++j )
					 {
						 if ( pWnd ==(GUIWnd*)m_pBtnInlayHole[j] )
						 {
							 for ( int k=0; k<MAX_EQUIPHOLE_NUM; ++k )
							 {
								 if ( j != k )
									 m_pBtnInlayHole[k]->SetState( EGUIBS_PopUp, FALSE, FALSE );
							 }
							 ChooseInlayPos(j);
							 RefreshInlayUI();
							 break;
						 }
					 }
				 }
				 else
				 {
					 for ( int i=0; i<EECT_End; ++i )
					 {
						 if ( pWnd ==(GUIWnd*)m_pBtnPageInlay[i] && EECT_Inlay != EHolyEquipCtrlType(i) )
						 {
							 SwitchPage( EHolyEquipCtrlType(i) );
							 break;
						 }
					 }
				 }	
			}
		}
		break;
	case EGUIE_Drag:
		{
			if (!m_bIfProgress)
			{
				if ( pWnd ==(GUIWnd*)m_pIBEquip )
				{
					if (P_VALID(m_pEquip))
					{
						StopHolyCompose();
						StopHolyChisel();
						StopHolyInlay();
						ClearEquip();
					}
				}
				else if ( pWnd ==(GUIWnd*)m_pIBComposeItem )
				{
					if (P_VALID(m_pItemCompose)) 	
					{
						StopHolyCompose();
						ClearUI(EECT_Compose);
						m_pItemCompose = NULL;
						m_nItemCostNum = 0;
					}
				}
				else if ( pWnd ==(GUIWnd*)m_pIBChiselItem )
				{
					if (P_VALID(m_pItemChisel)) 		
					{
						StopHolyChisel();
						ClearUI(EECT_Chisel);
						m_pItemChisel = NULL;
					}
				}
				else if ( pWnd ==(GUIWnd*)m_pIBInlayItem )
				{
					if (P_VALID(m_pItemInlayItem)) 		
					{
						StopHolyInlay();
						m_pIBInlayItem->RefreshItem();
						m_pItemInlayItem = NULL;
						m_nInlayStuffNum	= 0;
						m_nInlayPos			= GT_INVALID;
						RefreshInlayUI();
					}
				}
				else if ( pWnd ==(GUIWnd*)m_pIBInlayStuff )
				{
					if (P_VALID(m_pItemInlayStuff)) 		
					{
						StopHolyInlay();
						m_pIBInlayStuff->RefreshItem();
						m_pItemInlayStuff = NULL;
						m_nInlayStuffNum	= 0;
						m_nInlayPos			= GT_INVALID;
						RefreshInlayUI();
					}
				}
			}
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			if ( pWnd ==(GUIWnd*)m_pIBEquip )
			{
				if (P_VALID(m_pEquip)) 		
					ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(m_pEquip).c_str());
			}
			else if ( EECT_Compose == m_eCurCtrlType )
			{
				if ( pWnd ==(GUIWnd*)m_pIBComposeItem )
				{
					if (P_VALID(m_pItemCompose)) 		
						ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(m_pItemCompose).c_str());
				}
			}
			else if ( EECT_Chisel == m_eCurCtrlType )
			{
				if ( pWnd ==(GUIWnd*)m_pIBChiselItem )
				{
					if (P_VALID(m_pItemChisel)) 		
						ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(m_pItemChisel).c_str());
				}
				else if (_tcsnccmp(pWnd->GetName().c_str(), _T("hole"), 4) == 0)
				{
					for ( int j=0; j<MAX_EQUIPHOLE_NUM; ++j )
					{
						if ( pWnd ==(GUIWnd*)m_pSteChiselHole[j] )
						{
							DWORD dwQ = 0;
							tstring str = GetHoleInfo( j, EECT_Chisel ,dwQ );
							ShowTip(pWnd,  str.c_str(), NULL);
							break;
						}
					}
				}
			}
			else if ( EECT_Inlay == m_eCurCtrlType )
			{
				if ( pWnd ==(GUIWnd*)m_pIBInlayItem )
				{
					if (P_VALID(m_pItemInlayItem)) 		
						ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(m_pItemInlayItem).c_str());
				}
				else if ( pWnd ==(GUIWnd*)m_pIBInlayStuff )
				{
					if (P_VALID(m_pItemInlayStuff)) 		
						ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(m_pItemInlayStuff).c_str());
				}
				else if (_tcsnccmp(pWnd->GetName().c_str(), _T("choose"), 6) == 0)
				{
					for ( int j=0; j<MAX_EQUIPHOLE_NUM; ++j )
					{
						if ( pWnd ==(GUIWnd*)m_pBtnInlayHole[j] )
						{
							DWORD dwQ = 0;
							tstring str = GetHoleInfo( j, EECT_Inlay ,dwQ );
							DWORD dwcolor = 0xff00ffff;//= ItemMgr::Inst()->GetItemQualityColor(dwQ);
							ShowTip(pWnd,  str.c_str());//, NULL, dwcolor);
							break;
						}
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
	}
	return 0;
}


DWORD HolyEquipFrame::OnUseItemEvent(tagSpecItemEvent* pGameEvent)
{
	if ( !P_VALID(m_pWnd) )
		return 0;

	if (!m_pWnd->IsInvisible())
	{
		Item* pItem = ItemMgr::Inst()->GetPocketItemByID(pGameEvent->n64Serial);
		if (P_VALID(pItem))
		{
			INT16 n16Pos = pItem->GetPos();
			AddItem( n16Pos);
		}
	}

	return 0;

}

DWORD HolyEquipFrame::OnEquipPosEvent(tagItemPosChangeExEvent* pGameEvent)
{
	if ( P_VALID(m_pWnd) && !m_pWnd->IsInvisible()
		&&pGameEvent->eTypeSrc == EICT_Bag )
			AddItem( pGameEvent->n16IndexSrc);
	
	return 0;
}


DWORD HolyEquipFrame::OnItemDropCheck(tagEquipDropEvent* pGameEvent)
{
	if ( !P_VALID(pGameEvent) || !P_VALID(m_pWnd) || m_pWnd->IsInvisible() )
		return 0;

	if ( P_VALID(m_pEquip) && m_pEquip->GetItemId() == pGameEvent->n64ItemID )
	{
		ClearEquip();
		StopHolyCompose();
		StopHolyInlay();
		StopHolyChisel();
	}
	else if ( P_VALID(m_pItemCompose) && m_pItemCompose->GetItemId() == pGameEvent->n64ItemID )
	{
		StopHolyCompose();
		ClearUI(EECT_Compose);
		m_pItemCompose = NULL;
	}
	else if ( P_VALID(m_pItemInlayItem) && m_pItemInlayItem->GetItemId() == pGameEvent->n64ItemID )
	{
		StopHolyInlay();
		m_pIBInlayItem->RefreshItem();
		m_pItemInlayItem = NULL;
		m_nInlayStuffNum	= 0;
		m_nInlayPos			= GT_INVALID;
		RefreshInlayUI();
	}
	else if ( P_VALID(m_pItemInlayStuff) && m_pItemInlayStuff->GetItemId() == pGameEvent->n64ItemID )
	{
		StopHolyInlay();
		m_pIBInlayStuff->RefreshItem();
		m_pItemInlayStuff = NULL;
		m_nInlayStuffNum	= 0;
		m_nInlayPos			= GT_INVALID;
		RefreshInlayUI();
	}
	else if ( P_VALID(m_pItemChisel) && m_pItemChisel->GetItemId() == pGameEvent->n64ItemID )
	{
		StopHolyChisel();
		ClearUI(EECT_Chisel);
		m_pItemChisel = NULL;
	}
	 
	return 0;
}

DWORD HolyEquipFrame::OnChangeItemEvent(tagItemPosChangeExEvent* pGameEvent)
{
	if ( !P_VALID(pGameEvent) || !P_VALID(m_pWnd) || m_pWnd->IsInvisible() )
		return 0;

	RefreshUI();
	return 0;
}


// DWORD HolyEquipFrame::RevRefreshFrame( tagRefreshMaintainValueUIEvent* pGameEvent)
// {
// 	if ( !P_VALID(m_pWnd) || m_pWnd->IsInvisible() )
// 		return 0;
// 
// 	if ( EDT_Item == pGameEvent->eBagType )
// 	{
// 		if (P_VALID(m_pEquip) 
// 		&& (m_pEquip->GetPos()==pGameEvent->dwPos||m_pEquip->GetPos()==pGameEvent->dwDst) )
// 		{
// 			ClearEquip();
// 			StopHolyCompose();
// 			StopHolyInlay();
// 			StopHolyChisel();
// 		}
// 		else if ( P_VALID(m_pItemCompose) 
// 		&& (m_pItemCompose->GetPos()==pGameEvent->dwPos||m_pItemCompose->GetPos()==pGameEvent->dwDst) )
// 		{
// 			StopHolyCompose();
// 			ClearUI(EECT_Compose);
// 			m_pItemCompose = NULL;
// 		}
// 		else if ( P_VALID(m_pItemInlayItem) 
// 		&& (m_pItemInlayItem->GetPos()==pGameEvent->dwPos||m_pItemInlayItem->GetPos()==pGameEvent->dwDst) )
// 		{
// 			StopHolyInlay();
// 			m_pIBInlayItem->RefreshItem();
// 			m_pItemInlayItem = NULL;
// 			m_nInlayStuffNum	= 0;
// 			m_nInlayPos			= GT_INVALID;
// 			RefreshInlayUI();
// 		}
// 		else if ( P_VALID(m_pItemInlayStuff)
// 		&& (m_pItemInlayStuff->GetPos()==pGameEvent->dwPos||m_pItemInlayStuff->GetPos()==pGameEvent->dwDst) )
// 		{
// 			StopHolyInlay();
// 			m_pIBInlayStuff->RefreshItem();
// 			m_pItemInlayStuff = NULL;
// 			m_nInlayStuffNum	= 0;
// 			m_nInlayPos			= GT_INVALID;
// 			RefreshInlayUI();
// 		}
// 		else if ( P_VALID(m_pItemChisel) 
// 		&& (m_pItemChisel->GetPos()==pGameEvent->dwPos||m_pItemChisel->GetPos()==pGameEvent->dwDst) )
// 		{
// 			StopHolyChisel();
// 			ClearUI(EECT_Chisel);
// 			m_pItemChisel = NULL;
// 		}
// 
// 	}
// 
// 	return 0;
// }

DWORD HolyEquipFrame::NetItemRemove(tagNS_ItemRemove* pMsg, DWORD pPrama)
{
	if(pMsg->eConType != EICT_Bag)
		return 0;

	if ( P_VALID(m_pEquip) && m_pEquip->GetItemId() == pMsg->n64Serial )
	{
		ClearEquip();
		StopHolyCompose();
		StopHolyInlay();
		StopHolyChisel();
	}
	else if ( P_VALID(m_pItemCompose) && m_pItemCompose->GetItemId() == pMsg->n64Serial  )
	{
		if ( 0 == pMsg->n16Num )
		{
			StopHolyCompose();
			ClearUI(EECT_Compose);
			m_pItemCompose = NULL;
		}
		RefreshComposeUI();
	}
	else if ( P_VALID(m_pItemInlayItem) && m_pItemInlayItem->GetItemId() ==  pMsg->n64Serial  )
	{
		if ( 0 == pMsg->n16Num )
			StopHolyInlay();
		RefreshInlayUI();
	}
	else if ( P_VALID(m_pItemInlayStuff) && m_pItemInlayStuff->GetItemId() ==  pMsg->n64Serial  )
	{
		if ( 0 == pMsg->n16Num )
			StopHolyInlay();
		RefreshInlayUI();
	}
	else if ( P_VALID(m_pItemChisel) && m_pItemChisel->GetItemId() ==  pMsg->n64Serial  )
	{
		if ( 0 == pMsg->n16Num )
			StopHolyChisel();
		RefreshChiselUI();
	}

	return 0;
}

DWORD HolyEquipFrame::OnNS_StarEnhance(tagNS_StarEnhance* pMsg, DWORD pPrama)
{
	DWORD dwErr = pMsg->dwErrCode;
	RefreshUI();
	if ( (dwErr == E_Success || dwErr == E_StarEnhanceCondensate_Failed ) &&P_VALID(m_pEquip) )
	{
		m_escapeTime = 0;
		Equipment* pEquip = (Equipment*)m_pEquip; //已知是装备了才转换
		BYTE byLevel = pEquip->GetHolyComposeLv();
		const tagStarEnhanceProto* pProto = HolyManMgr::Inst()->GetHolyComposeInfo( byLevel );
		if ( P_VALID(pProto) )
		{
			m_nItemCostNum +=pProto->nCostItemNum;
			if ( m_pPBItemNum->GetState() ==EGUIBS_PushDown )
			{
				INT nNum = 0;
				tstringstream tssOldNum;
				tstring strNum = m_pEBItemNum->GetText();
				tssOldNum<<strNum;
				tssOldNum>>nNum;
				if ( nNum >= (INT(pProto->nCostItemNum)) )
				{
					nNum -= INT(pProto->nCostItemNum);
					tstringstream tssNewNum;
					tssNewNum<<nNum;
					m_pEBItemNum->SetText( tssNewNum.str().c_str() );
				}
			}
			else
				m_pEBItemNum->SetText( _T("0") );
		}
		if ( dwErr == E_Success )
		{
			m_pStcSucc->SetInvisible(FALSE);
			m_pStcSucc->FlipToTop();
		}
		KeepHolyCompose();
		return 0;
	}

	HolyManMgr::Inst()->ShowErrorTips(dwErr);
	StopHolyCompose();
	return 0;
}

DWORD HolyEquipFrame::OnNS_HolyEquipHole(tagNS_HolyEquipHole* pMsg, DWORD pPrama)
{
	DWORD dwErr = pMsg->dwErrorCode;
	RefreshUI();
	if ( dwErr == E_Success )
	{
		m_escapeTime = 0;
		m_pStcSucc->SetInvisible(FALSE);
		m_pStcSucc->FlipToTop();
	}
	else if ( dwErr == E_HolyEquipHole_Fail )
	{
		m_pStcFail->SetInvisible(FALSE);
		m_pStcFail->FlipToTop();
		KeepHolyChisel();
		return 0;
	}
	else 
		HolyManMgr::Inst()->ShowErrorTips(dwErr);
	StopHolyChisel();

	return 0;
}

DWORD HolyEquipFrame::OnNS_HolyEquipInlay(tagNS_HolyEquipInlay* pMsg, DWORD pPrama)
{
	DWORD dwErr = pMsg->dwErrorCode;
	RefreshUI();
	if ( E_Success == dwErr)
	{
		m_escapeTime = 0;
		m_pStcSucc->SetInvisible(FALSE);
		m_pStcSucc->FlipToTop();
	}
	else if ( E_HolyEquipInlay_Fail == dwErr )
	{
		m_escapeTime = 0;
		m_pStcFail->SetInvisible(FALSE);
		m_pStcFail->FlipToTop();
	}
	else 
		HolyManMgr::Inst()->ShowErrorTips(dwErr);
	StopHolyInlay();

	return 0;
}

DWORD HolyEquipFrame::OnNS_HolyEquipRemove(tagNS_HolyEquipRemove* pMsg, DWORD pPrama)
{
	DWORD dwErr = pMsg->dwErrorCode;
	RefreshUI();
	if ( dwErr == E_Success )
	{
		m_escapeTime = 0;
		m_pStcSucc->SetInvisible(FALSE);
		m_pStcSucc->FlipToTop();
	}
	else 
		HolyManMgr::Inst()->ShowErrorTips(dwErr);
	StopHolyInlay();

	return 0;
}

BOOL HolyEquipFrame::AddItem(INT16 nPos)
{
	if ( !P_VALID(m_pWnd) || m_pWnd->IsInvisible() || m_bIfProgress )
		return FALSE;

	Item* pItem = ItemMgr::Inst()->GetPocketItem(nPos); 
	if ( !P_VALID(pItem) )
		return FALSE;

	ItemContainer* pPocket	 =  ItemMgr::Inst()->GetPocket();
	DWORD	dwItemTypeID =  pItem->GetItemTypeID();
	INT			nCount				 = pPocket->GetItemQuantity(dwItemTypeID);
	BYTE		byQuality			 = pItem->GetItemQuality();

	const tagItemProto* pProto = pItem->GetProto();
	if ( !P_VALID(pProto) )
		return FALSE;
		
	if (MIsEquipment(dwItemTypeID))
	{
		Equipment* pEquip = (Equipment*)pItem; //已知是装备了才转换
 		if ( pProto->eType != EIT_HolyEquip )
 		{
 			HolyManMgr::Inst()->ShowErrorTips(2001);
 			return FALSE;
 		}
 		else if ( pProto->dwTimeLimit!=GT_INVALID)
		{
			HolyManMgr::Inst()->ShowErrorTips(2006);
			return FALSE;
		}
		
		ClearEquip();
		m_pEquip = pItem;
		RefreshUI();
	}
	else if ( P_VALID(m_pEquip) && MIsEquipment(m_pEquip->GetItemTypeID()) )
	{
		
		Equipment* pEquip = (Equipment*)m_pEquip; //已知是装备了才转换
		if ( EECT_Compose==m_eCurCtrlType )
		{
			BYTE byLevel		= pEquip->GetHolyComposeLv();
			if ( byLevel >= MAX_STARENHANCE_LEVEL )
			{
				HolyManMgr::Inst()->ShowErrorTips(2004);
				return FALSE;
			}
			const tagStarEnhanceProto* pProto = HolyManMgr::Inst()->GetHolyComposeInfo( byLevel+1 );
			if ( P_VALID(pProto) && pProto->dwCostItemId == dwItemTypeID )
			{
				m_pItemCompose = pItem;
				m_pIBComposeItem->RefreshItem( dwItemTypeID, nCount, byQuality );
				ChangeItemNum();
			}
		}
		else if ( EECT_Inlay==m_eCurCtrlType )
		{
// 			if ( EISF_SoulInlay == pProto->eSpecFunc )
// 			{
// 				m_pItemInlayItem = pItem;
// 				m_pIBInlayItem->RefreshItem( dwItemTypeID, nCount, byQuality );
// 			}
// 			else if ( TRANS_EQUIPSOUL_ITEMID1 == dwItemTypeID 
// 					  || TRANS_EQUIPSOUL_ITEMID2 == dwItemTypeID )
// 			{
// 				m_pItemInlayStuff = pItem;
// 				ItemContainer* pPocket =  ItemMgr::Inst()->GetPocket();
// 				INT nTmpCount = pPocket->GetItemQuantity(TRANS_EQUIPSOUL_ITEMID1);
// 				nTmpCount += pPocket->GetItemQuantity(TRANS_EQUIPSOUL_ITEMID2);
// 				if ( nTmpCount > 0 ) //有则初始化为1
// 					m_nInlayStuffNum = 1;
// 				m_pIBInlayStuff->RefreshItem( dwItemTypeID, m_nInlayStuffNum, byQuality, TRUE  );
// 			}
			RefreshInlayUI();
		}
		else if ( EECT_Chisel==m_eCurCtrlType )
		{
// 			if ( EISF_SoulChisel == pProto->eSpecFunc )
// 			{
// 				m_pItemChisel = pItem;
// 				m_pIBChiselItem->RefreshItem( dwItemTypeID, nCount, byQuality );
// 			}
// 			RefreshChiselUI();
		}
	}

	return TRUE;

}

VOID  HolyEquipFrame::RefreshUI()
{
	HolyManMgr::Inst()->UpdateItem(&m_pEquip);
 
	if (!P_VALID(m_pWnd)||m_pWnd->IsInvisible())
		return ;

	if ( P_VALID(m_pEquip) )
		m_pIBEquip->RefreshItem( m_pEquip->GetItemTypeID(), 0, m_pEquip->GetItemQuality() );
	else
		m_pIBEquip->RefreshItem();

	if ( EECT_Compose == m_eCurCtrlType )
	{
		RefreshComposeUI( );
	}
	else 	if ( EECT_Chisel == m_eCurCtrlType )
	{
		RefreshChiselUI( );
	}
	else 	if ( EECT_Inlay ==  m_eCurCtrlType )
	{
		RefreshInlayUI( );
	}

}

VOID HolyEquipFrame::LockItem(bool bOperable,	EHolyEquipCtrlType eType/*=EECT_End*/)
{
	if( P_VALID(m_pEquip) )
		m_pEquip->SetOperable(!bOperable);

	//若为"锁"，则只锁当前页
	if (bOperable)
		eType = m_eCurCtrlType;

	if ( EECT_Compose == eType ||  EECT_End == eType )
	{
		if( P_VALID(m_pItemCompose) )
			m_pItemCompose->SetOperable(!bOperable);
		m_pPBItemNum->SetUnClickable(bOperable);
		m_pPBToLevel->SetUnClickable(bOperable);
	}
	
	if ( EECT_Inlay == eType ||  EECT_End == eType )
	{
		m_pBtnInlayMinStuff->SetEnable(!bOperable);
		m_pBtnInlayAddStuff->SetEnable(!bOperable);
		if (P_VALID(m_pItemInlayItem))
			m_pItemInlayItem->SetOperable(!bOperable);
		if (P_VALID(m_pItemInlayStuff))
			m_pItemInlayStuff->SetOperable(!bOperable);
		for ( int i=0; i<MAX_EQUIPHOLE_NUM; ++i )
			m_pBtnInlayHole[i]->SetUnClickable(bOperable);

	}
	
	if ( EECT_Chisel == eType ||  EECT_End == eType )
	{
		if (P_VALID(m_pItemChisel))
			m_pItemChisel->SetOperable(!bOperable);
	}
	
}

BOOL HolyEquipFrame::ClearEquip()
{
	if ( !P_VALID(m_pWnd) || m_pWnd->IsInvisible() )
		return FALSE;

	LockItem(FALSE);
	m_pEquip = NULL;
	m_pIBEquip->RefreshItem();

	ClearUI(m_eCurCtrlType);

	//EECT_Compose
	ClearSubData();

	m_pWnd->SetRefresh(true);

	return TRUE;
}

VOID HolyEquipFrame::ClearSubData()
{
	m_nItemCostNum	= 0;
	m_nInlayStuffNum	= 0;
	m_nInlayPos			= GT_INVALID;

	m_pItemCompose	= NULL;
	m_pItemInlayItem	= NULL;
	m_pItemInlayStuff	= NULL;
	m_pItemChisel		= NULL;

}

BOOL HolyEquipFrame::ClearUI(EHolyEquipCtrlType eType /*=EECT_End*/)
{
	if ( !P_VALID(m_pWnd) || m_pWnd->IsInvisible() )
		return FALSE;

	LockItem(FALSE,eType);

	if ( EECT_Compose == eType || EECT_End == eType )
	{
		m_pIBComposeItem->RefreshItem();
		m_pStcStarLevel->SetText(_T(""));
		m_pStcSuccRate->SetText(_T(""));
		m_pStcReturnLv->SetText(_T(""));
		m_pStcAddAttName->SetText(_T(""));
		m_pStcAddAttValue->SetText(_T(""));
		m_pStcCostGold->SetText(_T(""));
		m_pStcCostSilver->SetText(_T(""));
		m_pProHolyCompose->SetValue(0,TRUE);
		for ( int i=0; i<MAX_STARENHANCE_LEVEL; ++i )
			m_pStcStarNum[i]->SetInvisible(TRUE);
	}

	if ( EECT_Inlay == eType || EECT_End == eType )
	{
		m_pIBInlayItem->RefreshItem();
		m_pIBInlayStuff->RefreshItem();
		m_pStcInlayRate->SetText(_T(""));
		m_pStcInlayGold->SetText(_T(""));
		m_pStcInlaySilver->SetText(_T(""));
		m_pProInlayOnce->SetValue(0,TRUE);
		for ( int i=0; i<MAX_EQUIPHOLE_NUM; ++i )
		{
			m_pSteInlayHole[i]->SetPicFile(szSoulPicNone);
			m_pBtnInlayHole[i]->SetState( EGUIBS_PopUp, FALSE, FALSE );
		}
	}

	if ( EECT_Chisel == eType || EECT_End == eType )
	{
		m_pIBChiselItem->RefreshItem();
		m_pStcHaveHoles->SetText(_T(""));
		m_pStcChiselRate->SetText(_T(""));
		m_pStcChiselGold->SetText(_T(""));
		m_pStcChiselSilver->SetText(_T(""));
		m_pProChiselOnce->SetValue(0,TRUE);
		for ( int i=0; i<MAX_EQUIPHOLE_NUM; ++i )
			m_pSteChiselHole[i]->SetPicFile(szSoulPicNone);
	}

	if(P_VALID(GetObj("MsgBox_UninlayConfirm")))
		KillObj("MsgBox_UninlayConfirm");

	if ( !P_VALID(m_pEquip) )
		return FALSE;

	return TRUE;
} 

VOID  HolyEquipFrame::RefreshComposeUI()
{
	HolyManMgr::Inst()->UpdateItem(&m_pItemCompose);
	if ( !ClearUI(EECT_Compose) || EECT_Compose != m_eCurCtrlType )
		return;

	if (P_VALID(m_pItemCompose))
	{
		DWORD dwTypeID	= m_pItemCompose->GetItemTypeID();
		ItemContainer* pPocket =  ItemMgr::Inst()->GetPocket();
		INT		 nCount		= pPocket->GetItemQuantity(dwTypeID);
		BYTE		 byQuality		= m_pItemCompose->GetItemQuality();
		m_pIBComposeItem->RefreshItem( dwTypeID, nCount, byQuality );
	}
	if ( P_VALID(m_pEquip) && MIsEquipment(m_pEquip->GetItemTypeID()) )
	{
		Equipment* pEquip = (Equipment*)m_pEquip; //已知是装备了才转换
		BYTE byLevel = pEquip->GetHolyComposeLv();
		if ( byLevel >= MAX_STARENHANCE_LEVEL )
			return ;
		const tagStarEnhanceProto* pProto = HolyManMgr::Inst()->GetHolyComposeInfo( byLevel+1 );
		if ( P_VALID(pProto) )
		{
			tstringstream tssStarLv;
			tssStarLv<<(INT)byLevel;
			m_pStcStarLevel->SetText(tssStarLv.str().c_str());
			for ( int i=0; i<(INT)byLevel; ++i )
				m_pStcStarNum[i]->SetInvisible(FALSE);
			tstringstream tssRate;
			tssRate<< INT((pProto->nRate)/100)<<_T("%") ;
			m_pStcSuccRate->SetText(tssRate.str().c_str());
			tstringstream tssLv;
			tssLv<<pProto->byFailLevel;
			m_pStcReturnLv->SetText(tssLv.str().c_str());
			DWORD dwEquipID = pEquip->GetItemTypeID();
			EHolyEquipAttType  eType	= HolyManMgr::Inst()->GetHolyEquipAttType(dwEquipID);
			if ( eType>EHEAT_NULLEX && eType<EHEAT_Covalue )  //只取中间10个属性
			{
				tstring strAtt		= HolyManMgr::Inst()->GetHolyEquipAttTypeName(dwEquipID);
				INT nAttValue	= HolyManMgr::Inst()->GetStarEnhanceAttValue( byLevel + 1, eType );				
				m_pStcAddAttName->SetText( strAtt.c_str() );
				tstringstream tssAttValue;
				tssAttValue<<nAttValue;
				m_pStcAddAttValue->SetText(tssAttValue.str().c_str());
			}
			INT nCostGold		= (pProto->dwCost)/10000;
			INT nCostSilv		= (pProto->dwCost)%10000;
			tstringstream tssCostG;
			tssCostG<<nCostGold;
			m_pStcCostGold->SetText(tssCostG.str().c_str());
			tstringstream tssCostS;
			tssCostS<<nCostSilv;
			m_pStcCostSilver->SetText(tssCostS.str().c_str());
		}
	}
	
}

DWORD	HolyEquipFrame::IfPosCanInlay(INT nIndex)
{
	INT nPos = nIndex+1; 
	if ( !P_VALID(m_pEquip) || !(MIsEquipment(m_pEquip->GetItemTypeID())) )
		return 3000;

	if ( !P_VALID(m_pItemInlayItem) )
		return 3006;
	
	Equipment* pEquip = (Equipment*)m_pEquip; //已知是装备了才转换
	if ( nPos<=0 )
		return 3002;
	if ( nPos> INT(pEquip->GetEquipMaxHoleNum()))
		return 3003;

	DWORD dwTypeID =  pEquip->GetEquipHoldGemID(nIndex);
	const tagItemProto	*pProto = ItemProtoData::Inst()->FindItemProto(dwTypeID);
	if ( P_VALID(pProto) )
		return 3004;

	//BYTE nNum = pEquip->GetEquipMaxHoleNum();
	INT64 n64Cost	= INT64(HolyManMgr::Inst()->GetInlayCost(nPos));
	INT64 n64Silver	= CurrencyMgr::Inst()->GetBagSilver();
	if ( n64Silver<n64Cost )
		return 3008;

	return E_Success;
}

BOOL	HolyEquipFrame::ChooseInlayPos(INT nIndex)
{
	m_nInlayPos = GT_INVALID;
 
	if ( nIndex<0 || nIndex>= MAX_EQUIPHOLE_NUM )
		return FALSE;

	m_nInlayPos = nIndex;

	ItemContainer* pPocket =  ItemMgr::Inst()->GetPocket();
	INT nTmpCount = 0; //= pPocket->GetItemQuantity(TRANS_EQUIPSOUL_ITEMID1);
	//nTmpCount += pPocket->GetItemQuantity(TRANS_EQUIPSOUL_ITEMID2);
	if ( nTmpCount > 0 ) //有则初始化为1
		m_nInlayStuffNum = 1;
	else
		m_nInlayStuffNum = 0;

	return TRUE;
}

BOOL	HolyEquipFrame::CanHolyInlay()
{
	DWORD dwErr = IfPosCanInlay(m_nInlayPos);
	if ( dwErr == E_Success )
		return TRUE;

	HolyManMgr::Inst()->ShowErrorTips(dwErr);
	return FALSE;
}

VOID	HolyEquipFrame::BeginHolyInlay()
{
	m_pBtnInlayBegin->SetEnable(FALSE);
	m_pBtnInlayExtirpate->SetEnable(FALSE);
	m_pProInlayOnce->SetValue(0,TRUE);
	m_pProInlayOnce->SetValue(g_fProHolyComposeTime);
	m_bIfProgress = TRUE;
	LockItem(TRUE, EECT_Inlay);
	m_eHolyInlayType = EHIT_Inlay; 
}

BOOL	HolyEquipFrame::CanHolyUninlay()
{
	//if (!m_bIfProgress)
	//	return FALSE;
	
	//判断
	if ( !P_VALID(m_pEquip) || !(MIsEquipment(m_pEquip->GetItemTypeID())) )
		return 3000;

	Equipment* pEquip = (Equipment*)m_pEquip; //已知是装备了才转换
	if ( m_nInlayPos < 0 )
		return 3002;
	if ( m_nInlayPos >= INT(pEquip->GetEquipMaxHoleNum()) )
		return 3003;

	DWORD dwTypeID =  pEquip->GetEquipHoldGemID(m_nInlayPos);
	const tagItemProto	*pProto = ItemProtoData::Inst()->FindItemProto(dwTypeID);
	if ( !P_VALID(pProto) )
		return 3005;

	if(P_VALID(GetObj("MsgBox_UninlayConfirm")))
		KillObj("MsgBox_UninlayConfirm");
	CreateObj("MsgBox_UninlayConfirm", "MsgBox");
	TObjRef<MsgBox>("MsgBox_UninlayConfirm")->Init(_T(""), g_StrTable[_T("HolyMan_Tips3001")] , _T("MsgBox_UninlayConfirm"));

	return TRUE;
}

VOID	HolyEquipFrame::BeginHolyUninlay()
{
	m_pBtnInlayBegin->SetEnable(FALSE);
	m_pBtnInlayExtirpate->SetEnable(FALSE);
	//m_pProInlayOnce->SetValue(0,TRUE);
	//m_pProInlayOnce->SetValue(g_fProHolyComposeTime);
	m_bIfProgress = TRUE;
	LockItem(TRUE, EECT_Inlay);
	m_eHolyInlayType = EHIT_Uninlay; 

	SendHolyUninlayMsg();
}

VOID	HolyEquipFrame::StopHolyInlay()
{
	m_bIfProgress = FALSE;
	m_pBtnInlayBegin->SetEnable(TRUE);
	m_pBtnInlayExtirpate->SetEnable(TRUE);
	m_pProInlayOnce->SetValue(0,TRUE);
	LockItem(FALSE, EECT_Inlay);
}

VOID HolyEquipFrame::RefreshInlayUI()
{
	HolyManMgr::Inst()->UpdateItem(&m_pItemInlayItem);
	HolyManMgr::Inst()->UpdateItem(&m_pItemInlayStuff);
	if ( !ClearUI(EECT_Inlay) || EECT_Inlay != m_eCurCtrlType )
		return;

	if (P_VALID(m_pItemInlayItem))
		m_pIBInlayItem->RefreshItem( m_pItemInlayItem->GetItemTypeID(), 0, m_pItemInlayItem->GetItemQuality() );
	
	ItemContainer* pPocket =  ItemMgr::Inst()->GetPocket();
	INT nCount = 0;
	if (P_VALID(m_pItemInlayStuff)&&P_VALID(pPocket))
	{
		nCount = 0; //+= pPocket->GetItemQuantity(TRANS_EQUIPSOUL_ITEMID1);
		//nCount += pPocket->GetItemQuantity(TRANS_EQUIPSOUL_ITEMID2);
		if ( nCount > m_nInlayStuffNum )
			nCount = m_nInlayStuffNum;
		else
			m_nInlayStuffNum = nCount;
		m_pIBInlayStuff->RefreshItem( m_pItemInlayStuff->GetItemTypeID(), nCount, m_pItemInlayStuff->GetItemQuality(), TRUE );
	}
	else
			m_nInlayStuffNum	= 0;

	if ( P_VALID(m_pEquip) && MIsEquipment(m_pEquip->GetItemTypeID()) )
	{
		Equipment* pEquip = (Equipment*)m_pEquip; //已知是装备了才转换
		INT nCost = 0;
		INT nRate = 0;
		if ( m_nInlayPos>=0 && m_nInlayPos<pEquip->GetEquipMaxHoleNum() )
		{
			nCost = HolyManMgr::Inst()->GetInlayCost(m_nInlayPos+1);
			nRate = (HolyManMgr::Inst()->GetInlayRate(m_nInlayPos+1))/100 ;
			nRate += nCount*IMPROVE_SOULINLAY_RATE;
		}
		tstringstream tssCostGold;
		tssCostGold<<(nCost/10000);
		m_pStcInlayGold->SetText(tssCostGold.str().c_str());
		tstringstream tssCostSilver;
		tssCostSilver<<(nCost%10000);
		m_pStcInlaySilver->SetText(tssCostSilver.str().c_str());
		tstringstream tssRate;
		tssRate<<nRate<<_T("%");
		m_pStcInlayRate->SetText(tssRate.str().c_str());
		BYTE nNum = pEquip->GetEquipMaxHoleNum();
 		for ( int i=0; i<(int)nNum ; ++i )
		{
			DWORD dwTypeID = pEquip->GetEquipHoldGemID( i );
			const tagItemProto	*pProto = ItemProtoData::Inst()->FindItemProto(dwTypeID);
			if ( P_VALID(pProto) )
			{
				tstring strPic = GetPicFileByQuality( INT(pProto->byQuality) );
				if ( !strPic.empty() )
					m_pSteInlayHole[i]->SetPicFile( strPic.c_str() );
			}
			else
				m_pSteInlayHole[i]->SetPicFile( szSoulPicHole );
		}
		if ( m_nInlayPos>=0 && m_nInlayPos<nNum )
			m_pBtnInlayHole[m_nInlayPos]->SetState( EGUIBS_PushDown, FALSE, FALSE );
	}

}

DWORD HolyEquipFrame::OnMsgBox_UninlayConfirm(tagMsgBoxEvent* pGameEvent)
{
	if(MBF_OK == pGameEvent->eResult)
	{
		BeginHolyUninlay();
	}
	return 0;
}

BOOL	HolyEquipFrame::CanHolyChisel()
{
	if ( !P_VALID(m_pEquip) || !P_VALID(m_pItemChisel) || !(MIsEquipment(m_pEquip->GetItemTypeID())) )
	{
		HolyManMgr::Inst()->ShowErrorTips(3006);
		return FALSE;
	}

	Equipment* pEquip = (Equipment*)m_pEquip; //已知是装备了才转换
	BYTE nNum = pEquip->GetEquipMaxHoleNum();
	if ( (INT)nNum >= MAX_EQUIPHOLE_NUM )
	{
		HolyManMgr::Inst()->ShowErrorTips(3007);
		return FALSE;
	}

	INT64 n64Cost	= INT64(HolyManMgr::Inst()->GetChiselCost(nNum+1));
	INT64 n64Silver	= CurrencyMgr::Inst()->GetBagSilver();
	if ( n64Silver<n64Cost )
	{
		HolyManMgr::Inst()->ShowErrorTips(3008);
		return FALSE;
	}

	return TRUE;
}

VOID	HolyEquipFrame::SendHolyInlayMsg()
{
	if ( !P_VALID(m_pItemInlayItem) || !P_VALID(m_pEquip) )
	{
		StopHolyInlay(); 
		return ;	
	}

	tagNC_HolyEquipInlay  msg;
	msg.n64SrcEquipID	= m_pEquip->GetItemId();
	msg.n64StoneID		= m_pItemInlayItem->GetItemId();
	if ( P_VALID(m_pItemInlayStuff) )
	{
		msg.n64ItemID	= 	m_pItemInlayStuff->GetItemId();
		msg.nItemNum	= m_nInlayStuffNum;
	}
	else
	{
		msg.n64ItemID	= 	GT_INVALID;
		msg.nItemNum	= 0;
	}
	msg.byHoleID			= (BYTE)(m_nInlayPos+1);
	m_pSession->Send(&msg);

	m_bIfProgress = FALSE;

}

VOID	HolyEquipFrame::SendHolyUninlayMsg()
{
	if ( !P_VALID(m_pEquip) )
	{
		/*StopHolyInlay(); */
		return ;	
	}

	tagNC_HolyEquipRemove  msg;
	msg.n64SrcEquipID		= m_pEquip->GetItemId();
	msg.byHoleID				= (BYTE)(m_nInlayPos+1);
	m_pSession->Send(&msg);
}

VOID	HolyEquipFrame::BeginHolyChisel()
{
	m_pBtnChiselBegin->SetEnable(FALSE);
	m_pProChiselOnce->SetValue(0,TRUE);
	m_pProChiselOnce->SetValue(g_fProHolyComposeTime);
	m_bIfProgress = TRUE;
	LockItem(TRUE, EECT_Chisel);
}

VOID   HolyEquipFrame::KeepHolyChisel()
{
	if (!P_VALID(m_pPthChisel)||m_pPthChisel->IsInvisible())
		return;

	if (CanHolyChisel())
		BeginHolyChisel();
	else
		StopHolyChisel();
}

VOID	HolyEquipFrame::StopHolyChisel()
{
	m_bIfProgress = FALSE;
	m_pBtnChiselBegin->SetEnable(TRUE);
	m_pProChiselOnce->SetValue(0,TRUE);
	LockItem(FALSE, EECT_Chisel);
}

VOID HolyEquipFrame::RefreshChiselUI()
{
	HolyManMgr::Inst()->UpdateItem(&m_pItemChisel);
	if ( !ClearUI(EECT_Chisel) || EECT_Chisel != m_eCurCtrlType )
		return;

	if (P_VALID(m_pItemChisel))
	{
		DWORD  dwTypeID		= m_pItemChisel->GetItemTypeID();
		ItemContainer* pPocket =  ItemMgr::Inst()->GetPocket();
		INT		 nCount				= pPocket->GetItemQuantity(dwTypeID);
		BYTE	 byQuality			= m_pItemChisel->GetItemQuality();
		m_pIBChiselItem->RefreshItem( dwTypeID, nCount, byQuality );
	}

	if ( P_VALID(m_pEquip) && MIsEquipment(m_pEquip->GetItemTypeID()) )
	{
		Equipment* pEquip = (Equipment*)m_pEquip; //已知是装备了才转换
		BYTE nMaxNum = MAX_EQUIPHOLE_NUM;
		if ( ( pEquip->GetEquipMaxHoleNum() ) < MAX_EQUIPHOLE_NUM)
		{
			nMaxNum	= pEquip->GetEquipMaxHoleNum();
			INT nCost	= HolyManMgr::Inst()->GetChiselCost(nMaxNum+1);
			
			INT nRate1	= HolyManMgr::Inst()->GetChiselRate(nMaxNum+1);
			INT nRate2	= 0;
			if (P_VALID(m_pItemChisel))
			{
				const tagItemProto* pItemProto = m_pItemChisel->GetProto();
				if (P_VALID(pItemProto))
					nRate2 = pItemProto->nSpecFuncVal1;
			}
			INT nFinalRate	= (nRate1/100)*(nRate2/100);
			tstringstream tssRate;
			tssRate<<(nFinalRate/100)<<_T("%");
			m_pStcChiselRate->SetText(tssRate.str().c_str());
			tstringstream tssCostGold;
			tssCostGold<<(nCost/10000);
			m_pStcChiselGold->SetText(tssCostGold.str().c_str());
			tstringstream tssCostSilver;
			tssCostSilver<<(nCost%10000);
			m_pStcChiselSilver->SetText(tssCostSilver.str().c_str());
			tstringstream tssHoles;
			tssHoles<<(INT)nMaxNum;
			m_pStcHaveHoles->SetText(tssHoles.str().c_str());
		}
		for ( int i=0; i<MAX_EQUIPHOLE_NUM; ++i )
		{
			if ( i < (int)nMaxNum )
				m_pSteChiselHole[i]->SetPicFile( szSoulPicHole );
		}
	}
}

VOID	HolyEquipFrame::SendHolyChiselMsg()
{

	if ( !P_VALID(m_pItemChisel) || !P_VALID(m_pEquip) )
	{
		StopHolyChisel(); 
		return ;	
	}

	tagNC_HolyEquipHole  msg;
	msg.n64SrcEquipID	= m_pEquip->GetItemId();
	msg.n64ItemID		= m_pItemChisel->GetItemId();
	m_pSession->Send(&msg);

	m_bIfProgress = FALSE;
}

VOID HolyEquipFrame::SwitchPage(EHolyEquipCtrlType eType /*=EECT_End*/)
{
	if ( EECT_End== eType )
		return;

	StopHolyCompose();
	StopHolyInlay();
	StopHolyChisel();

	m_eCurCtrlType = eType;

	if ( EECT_Compose == m_eCurCtrlType )
	{
		m_pPthCompose->SetInvisible(FALSE);
		m_pPthInlay->SetInvisible(TRUE);
		m_pPthChisel->SetInvisible(TRUE);
		for ( int i=0; i<EECT_End; ++i )
		{
			if ( EHolyEquipCtrlType(i) == m_eCurCtrlType )
				m_pBtnPageCompose[i]->SetState(EGUIBS_PushDown,FALSE);
			else
				m_pBtnPageCompose[i]->SetState(EGUIBS_PopUp,FALSE);
		}
	}
	else 	if ( EECT_Chisel == m_eCurCtrlType )
	{
		m_pPthCompose->SetInvisible(TRUE);
		m_pPthInlay->SetInvisible(TRUE);
		m_pPthChisel->SetInvisible(FALSE);
		for ( int i=0; i<EECT_End; ++i )
		{
			if ( EHolyEquipCtrlType(i) == m_eCurCtrlType )
				m_pBtnPageChisel[i]->SetState(EGUIBS_PushDown,FALSE);
			else
				m_pBtnPageChisel[i]->SetState(EGUIBS_PopUp,FALSE);
		}
	}
	else 	if ( EECT_Inlay ==  m_eCurCtrlType )
	{
		m_pPthCompose->SetInvisible(TRUE);
		m_pPthInlay->SetInvisible(FALSE);
		m_pPthChisel->SetInvisible(TRUE);
		for ( int i=0; i<EECT_End; ++i )
		{
			if ( EHolyEquipCtrlType(i) == m_eCurCtrlType )
				m_pBtnPageInlay[i]->SetState(EGUIBS_PushDown,FALSE);
			else
				m_pBtnPageInlay[i]->SetState(EGUIBS_PopUp,FALSE);
		}
	}

	ClearUI(); 
	ClearSubData();
	RefreshUI();

}

tstring	HolyEquipFrame::GetPicFileByQuality(INT nQuality)
{
	switch (nQuality)
	{
	case 1:		return	szSoulPicYellow;	 
	case 2:		return	szSoulPicGreen;	 
	case 3:		return	szSoulPicBlue;	 
	case 4:		return	szSoulPicOrange; 
	case 5:		return	szSoulPicPurple;	 
	default:	return	_T("");
	}
}

tstring  HolyEquipFrame::GetHoleInfo( INT index, EHolyEquipCtrlType eType,DWORD &dwQuality  )
{
	dwQuality = 0;
	tstringstream tsstips;
	if (P_VALID(m_pEquip) 
		&& ( EECT_Inlay==eType || EECT_Chisel==eType ) )
	{
		Equipment* pEquip = (Equipment*)m_pEquip;
		if ( index >= (pEquip->GetEquipMaxHoleNum()) ) //未开砸
			tsstips<<g_StrTable[_T("HolyMan_Tips3003")];
		else
		{
			DWORD dwGemID = pEquip->GetEquipHoldGemID(index);
			const tagItemProto* pGem = ItemProtoData::Inst()->FindItemProto(dwGemID);
			if( P_VALID(pGem) )
			{
				tagItemDisplayInfo* pDisplayInfo = pGem->pDisplayInfo;
				if ( P_VALID(pGem->pDisplayInfo) )
				{
					if ( EECT_Inlay==eType )
					{
						tsstips<<(pDisplayInfo->szName);	
						dwQuality = DWORD(pGem->byQuality);
					}
					else
						tsstips<<g_StrTable[_T("HolyMan_Tips3004")];
				}	
			}
			else  //未镶嵌 
				tsstips<<g_StrTable[_T("HolyMan_Tips3005")];
		}
	}

	if ( !tsstips.str().empty() )
		return  tsstips.str();
	else
		return _T("");
}

VOID HolyEquipFrame::SendHolyComposeMsg()
{
	if ( !P_VALID(m_pItemCompose) || !P_VALID(m_pEquip) )
	{
		StopHolyCompose(); 
		return ;	
	}
 
	tagNC_StarEnhance  msg;
	msg.n64EquipId = m_pEquip->GetItemId();
	m_pSession->Send(&msg);

	m_bIfProgress = FALSE;

}

BOOL HolyEquipFrame::CanHolyCompose()
{
	//缺道具
	if ( !P_VALID(m_pEquip) || !P_VALID(m_pItemCompose) )
	{
		HolyManMgr::Inst()->ShowErrorTips(2003);
		return FALSE;
	}
	//非装备
	if (!MIsEquipment(m_pEquip->GetItemTypeID()))
	{
		HolyManMgr::Inst()->ShowErrorTips(2003);
		return FALSE;
	}
	Equipment* pEquip = (Equipment*)m_pEquip; //已知是装备了才转换
	//非元神装
	if ( pEquip->GetProto()->eType != EIT_HolyEquip )
	{
		HolyManMgr::Inst()->ShowErrorTips(2001);
		return FALSE;
	}
	//达到要求返回
	if ( m_pPBItemNum->GetState() ==EGUIBS_PushDown )			//达到使用数目
	{
		tstringstream tssNum;
		tstring strNum = m_pEBItemNum->GetText();
		tssNum<<strNum;
		INT nNum = 0;
		tssNum>>nNum;
		BYTE byLevel = pEquip->GetHolyComposeLv();
		const tagStarEnhanceProto* pProto = HolyManMgr::Inst()->GetHolyComposeInfo( byLevel+1 );
		if ( P_VALID(pProto) && nNum < ((INT)pProto->nCostItemNum))
		{
			HolyManMgr::Inst()->ShowErrorTips(2008);
			return FALSE;
		}
	}
	else if ( m_pPBToLevel->GetState() ==EGUIBS_PushDown )     //达到级数
	{
		tstringstream tssLv;
		tstring strLv = m_pEBToLevel->GetText();
		tssLv<<strLv;
		INT nLv = 0;
			tssLv>>nLv;
		if ( nLv <= pEquip->GetHolyComposeLv() )
		{
			HolyManMgr::Inst()->ShowErrorTips(2007);
			return FALSE;
		}
	}
	//级满
	if ( pEquip->GetHolyComposeLv() >= MAX_STARENHANCE_LEVEL )
	{
		HolyManMgr::Inst()->ShowErrorTips(2004);
		return FALSE;
	}
	//缺钱
	BYTE byLevel = pEquip->GetHolyComposeLv();
	const tagStarEnhanceProto* pProto = HolyManMgr::Inst()->GetHolyComposeInfo( byLevel+1 );
	if ( !P_VALID(pProto) || pProto->dwCost > CurrencyMgr::Inst()->GetBagSilver() )
	{
		HolyManMgr::Inst()->ShowErrorTips(2005);
		return FALSE;
	}
	//缺数
	ItemContainer* pPocket = ItemMgr::Inst()->GetPocket();
	DWORD dwTypeID = m_pItemCompose->GetItemTypeID();
	INT nCount = pPocket->GetItemQuantity( dwTypeID );
	if ( nCount <= 0 )
	{
		HolyManMgr::Inst()->ShowErrorTips(2003);
		return FALSE;
	}

	return TRUE;
}

VOID HolyEquipFrame::KeepHolyCompose()
{
	if (!P_VALID(m_pPthCompose)||m_pPthCompose->IsInvisible())
		return;

	if (CanHolyCompose())
		BeginHolyCompose();
	else
		StopHolyCompose();

}

VOID HolyEquipFrame::BeginHolyCompose()
{
	m_pBtnStartComp->SetEnable(FALSE);
	m_pProHolyCompose->SetValue(0,TRUE);
	m_pProHolyCompose->SetValue(g_fProHolyComposeTime);
	m_bIfProgress = TRUE;
	LockItem(TRUE, EECT_Compose);
}

VOID HolyEquipFrame::StopHolyCompose()
{
	m_bIfProgress = FALSE;
	m_pBtnStartComp->SetEnable(TRUE);
	m_pProHolyCompose->SetValue(0,TRUE);
	LockItem(FALSE, EECT_Compose);
}

//打开界面 
VOID HolyEquipFrame::ShowWnd(BOOL bVal)
{
	if( P_VALID(m_pWnd) ) 
	{
		StopHolyCompose();
		StopHolyInlay();
		StopHolyChisel();
		ClearEquip();
		m_pWnd->SetInvisible(!bVal);
		if (bVal)
		{
			m_pWnd->FlipToTop();	
			ItemFrame* pFrame = (ItemFrame*)m_pFrameMgr->GetFrame(_T("Item"));
			if (P_VALID(pFrame))
				pFrame->SetCurStrategy(EIST_HolyCompose);
			m_pPBItemNum->SetState(EGUIBS_PushDown,TRUE,TRUE);
			m_eCurCtrlType = EECT_Compose;
			SwitchPage(m_eCurCtrlType);
		}
		else
		{
			ItemFrame* pFrame = (ItemFrame*)m_pFrameMgr->GetFrame(_T("Item"));
			if (P_VALID(pFrame))
				pFrame->SetCurStrategy(EIST_ItemPocket);
			m_pEBItemNum->SetText(_T("0"));
		}
	}
}

VOID   HolyEquipFrame::ChangeItemNum()
{
	if ( !P_VALID(m_pWnd)||m_pWnd->IsInvisible() )
		return ;
	INT nNum = 0;
	m_nItemCostNum = 0;
	ItemContainer* pContainer = ItemMgr::Inst()->GetPocket();
	if (P_VALID(m_pItemCompose))
		nNum = pContainer->GetItemQuantity(m_pItemCompose->GetItemTypeID());
	if( nNum >= 0 )
	{
		TCHAR szTemp[X_LONG_NAME];
		_sntprintf( szTemp, X_LONG_NAME, _T("%d"), nNum );
		//m_pEBItemNum->SetMaxValue(nNum);
		m_pEBItemNum->SetText(szTemp);
	}
}