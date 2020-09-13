#include "StdAfx.h"
#include "LevelPromoteFrame.h"
#include "..\WorldDefine\ItemDefine.h"
#include "..\WorldDefine\msg_mall.h"
#include "..\WorldDefine\role_att.h"
#include "ItemButton.h"
#include "IconStatic.h"
#include "ItemProtoData.h"
#include "MallMgr.h"
#include "RoleMgr.h"
#include "LocalPlayer.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "shellapi.h"
#include "HttpPicEx.h"
#include "NetSession.h"
#include "SaveAccount.h"
#include "CombatSysUtil.h"
#include "..\WorldDefine\currency_define.h"
#include "..\WorldDefine\login.h"

const TCHAR *szUnSelectItem_GiftPlan	= _T("data\\ui\\mall\\L-mall07.bmp");
const TCHAR *szSelectItem_GiftPlan		= _T("data\\ui\\Common\\L_icon-l2.bmp");

LevelPromoteFrame::LevelPromoteFrame(void)
:m_Trunk(this)
,m_CurBtnLevel(0)
,m_dwSelectID(GT_INVALID)
,m_nSelectPrice(0)
{
	m_pWnd = NULL;
	for (int i=0; i<MAX_GIFT_PER_PAGE; ++i)
	{
		m_pGiftBack[i] = NULL;
		m_pIBnGift[i] = NULL;
		m_pStcName[i] = NULL;
		m_pStcPrice[i] = NULL;
	}
	for (int i=0; i<LEVEL_NUM_CHOOSE; ++i)
	{
		m_pPickPatch[i] = NULL;
		m_pPBnLevel[i] = NULL;
		m_bOnceGlintFlag[i] = TRUE;
		
	}
	
	m_pBtnBuy = NULL;
	m_pClose  = NULL;
	m_pBtnPay = NULL;
}

LevelPromoteFrame::~LevelPromoteFrame(void)
{
}


BOOL LevelPromoteFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam );

	//--注册网络消息
	m_pCmdMgr->Register("NS_RoleLevelChange",(NETMSGPROC)m_Trunk.sfp2(&LevelPromoteFrame::OnNS_RoleLevelChange),_T("NS_RoleLevelChange"));

	//--注册游戏消息
	m_pFrameMgr->RegisterEventHandle(_T("OpenLevelPromoteFrame"),(FRAMEEVENTPROC)m_Trunk.sfp1(&LevelPromoteFrame::OnOpenLevelPromoteFrame));
	m_pFrameMgr->RegisterEventHandle(_T("Confirm_Buy"),(FRAMEEVENTPROC)m_Trunk.sfp1(&LevelPromoteFrame::OnConfirmMsg));
	for (int i=0; i<LEVEL_NUM_CHOOSE; ++i)
	{
		m_bOnceGlintFlag[i] = TRUE;
	}
	InitData();
	return bRet;
}

BOOL LevelPromoteFrame::Destroy()
{
	//--注销网络消息
	m_pCmdMgr->UnRegister("NS_RoleLevelChange",	(NETMSGPROC)m_Trunk.sfp2(&LevelPromoteFrame::OnNS_RoleLevelChange));

	//--注销游戏消息
	m_pFrameMgr->UnRegisterEventHandler(_T("OpenLevelPromoteFrame"),(FRAMEEVENTPROC)m_Trunk.sfp1(&LevelPromoteFrame::OnOpenLevelPromoteFrame));
	m_pFrameMgr->UnRegisterEventHandler(_T("Confirm_Buy"),(FRAMEEVENTPROC)m_Trunk.sfp1(&LevelPromoteFrame::OnConfirmMsg));

	if(P_VALID(GetObj("Confirm_Group_Buy")))
		KillObj("Confirm_Group_Buy");
	for (int i=0; i<LEVEL_NUM_CHOOSE; ++i)
	{
		m_bOnceGlintFlag[i] = TRUE;
	}
	if( P_VALID(m_pWnd) )
		m_pGUI->AddToDestroyList(m_pWnd);

	return GameFrame::Destroy();
}

BOOL LevelPromoteFrame::EscCancel()
{
	
	if( m_pWnd->IsInvisible() )
		return FALSE;

	if(P_VALID(GetObj("Confirm_Group_Buy")))
		KillObj("Confirm_Group_Buy");

	m_pWnd->SetInvisible( TRUE );

	return TRUE;
}

BOOL LevelPromoteFrame::ReloadUI()
{
	if (P_VALID(m_pWnd))
		m_pGUI->AddToDestroyList(m_pWnd);

	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\preferential_bag.xml");

	XmlElement ele;
	if (!m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str()))
		return FALSE;

	for (int i=0; i<MAX_GIFT_PER_PAGE; ++i)
	{
		CHAR szFullName[X_LONG_NAME] = {0};
		_snprintf( szFullName, X_LONG_NAME, "preferential_bag_win\\preferential_bag_pic\\back0\\item_fra\\itemback%d\\itempic\\item", i+1);
		m_pGUI->ChangeXml(&ele, szFullName, "ClassName", "ItemButton");

		CHAR szFullName1[X_LONG_NAME] = {0};
		_snprintf( szFullName1, X_LONG_NAME, "preferential_bag_win\\preferential_bag_pic\\back0\\item_fra\\itemback%d", i+1);
		m_pGUI->ChangeXml(&ele, szFullName1, "ClassName", "IconStatic");

	}
	/*for (int i=0; i<LEVEL_NUM_CHOOSE; ++i)
	{
		CHAR szFullName[X_LONG_NAME] = {0};
		_snprintf( szFullName, X_LONG_NAME, "preferential_bag_win\\preferential_bag_pic\\com_fra\\optbutt%d", i );
		m_pGUI->ChangeXml(&ele, szFullName, "ClassName", "GlintPushButton");
	}*/

	//创建
	m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &ele);
	 
	for (int i=0; i<MAX_GIFT_PER_PAGE; ++i)
	{
		TCHAR szName1[X_LONG_NAME] = {0};
		_sntprintf( szName1, X_LONG_NAME, _T("preferential_bag_pic\\back0\\item_fra\\itemback%d\\itempic\\item"), i+1);
		m_pIBnGift[i] = (ItemButton*)m_pWnd->GetChild(szName1);

		TCHAR szName2[X_LONG_NAME] = {0};
		_sntprintf( szName2, X_LONG_NAME, _T("preferential_bag_pic\\back0\\item_fra\\itemback%d\\name"), i+1);
		m_pStcName[i] = (GUIStatic*)m_pWnd->GetChild(szName2);

		TCHAR szName3[X_LONG_NAME] = {0};
		_sntprintf( szName3, X_LONG_NAME, _T("preferential_bag_pic\\back0\\item_fra\\itemback%d\\num1"), i+1);
		m_pStcPrice[i] = (GUIStatic*)m_pWnd->GetChild(szName3);

		TCHAR szName4[X_LONG_NAME] = {0};
		_sntprintf( szName4, X_LONG_NAME, _T("preferential_bag_pic\\back0\\item_fra\\itemback%d"), i+1);
		m_pGiftBack[i] = (IconStatic*)m_pWnd->GetChild(szName4);
		
	}

	for (int i=0; i<LEVEL_NUM_CHOOSE; ++i)
	{
		TCHAR szName1[X_LONG_NAME] = {0};
		_sntprintf( szName1, X_LONG_NAME, _T("preferential_bag_pic\\com_fra\\optbutt%d"), i );
		m_pPBnLevel[i] = (GUIPushButton*)m_pWnd->GetChild(szName1);	
		//m_pPBnLevel[i]->SetEnable(FALSE);

		TCHAR szName2[X_LONG_NAME] = {0};
		_sntprintf( szName2, X_LONG_NAME, _T("preferential_bag_pic\\back0\\item_fra\\itemback%d"), i+1 );
		m_pPickPatch[i] = (GUIStatic*)m_pWnd->GetChild(szName2);	

	}
	m_pClose = (GUIButton*)m_pWnd->GetChild(_T("preferential_bag_pic\\closebutt"));
	m_pBtnBuy = (GUIButton*)m_pWnd->GetChild(_T("preferential_bag_pic\\buybutton"));
	m_pBtnPay = (GUIButton*)m_pWnd->GetChild(_T("preferential_bag_pic\\paybutton"));

	// 注册事件处理函数
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&LevelPromoteFrame::EventHandler));

	ClearUI();
	ButtonEnableSet();


	return TRUE;
}

VOID LevelPromoteFrame::Update()
{

}


VOID LevelPromoteFrame::ShowWnd(BOOL b)
{
	if (P_VALID(m_pWnd))
	{
		if(P_VALID(GetObj("Confirm_Group_Buy")))
			KillObj("Confirm_Group_Buy");
		m_pWnd->SetInvisible(!b);
		if (b)
		{
			RefreshUI();
			m_pWnd->FlipToTop();
		}
		else
		{
			InitData();
		}
	}
}

BOOL LevelPromoteFrame::IsShow()
{
	if (P_VALID(m_pWnd))
	{
		if (m_pWnd->IsInvisible())
		{
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}
	return FALSE;
}

VOID  LevelPromoteFrame::InitData()
{
	m_CurBtnLevel = 0;
	m_dwSelectID = GT_INVALID;
	m_nSelectPrice = 0;
}

VOID  LevelPromoteFrame::RefreshUI()
{

	INT nRealLevel = 0;
	switch (m_CurBtnLevel) 
	{//按钮编号对应等级号
	case 1:
		nRealLevel = 60;
		break;
	case 2:
		nRealLevel = 70;
		break;
	case 3:
		nRealLevel = 80;
		break;
	default:
		nRealLevel = 50;
		break;
	}
	ClearUI();

	std::vector<DWORD>& vecPlanList = MallMgr::Inst()->GetGiftPlanList(nRealLevel);
	for (int i=0; i<(int)vecPlanList.size()&&i<MAX_GIFT_PER_PAGE; ++i)
	{
		DWORD dwTypeID = vecPlanList[i];
		const tagItemDisplayInfo *pInfo = ItemProtoData::Inst()->FindItemDisplay(dwTypeID);
		if( P_VALID(pInfo) )
		{
			m_pStcPrice[i]->SetText(_T(""));
			m_pStcName[i]->SetText(pInfo->szName);

			DWORD dwID = GT_INVALID;
			DWORD dwTypeIDTemp = GT_INVALID;
			INT nPrice = 0;

			TMap<DWORD, tagMallItemProto*> mapAll = MallMgr::Inst()->GetMallProto();
			TMap<DWORD, tagMallItemProto*>::TMapIterator it = mapAll.Begin();

			for ( ; it!=mapAll.End(); ++it)
			{
				tagMallItemProto* pMallItemProto = it->second;
				if (P_VALID(pMallItemProto))
				{
					if ( pMallItemProto->dwTypeID == dwTypeID )
					//  && PLAN_GIFT_TYPE_NUM == pMallItemProto->n8Kind )
					{
						tstringstream tsPrice;
						nPrice = pMallItemProto->nPrice;
						tsPrice<<nPrice<<g_StrTable[_T("Dak_YuanBao")];
						m_pStcPrice[i]->SetText(tsPrice.str().c_str());
						dwID = pMallItemProto->dwID;
						dwTypeIDTemp = pMallItemProto->dwTypeID;
					}
				}
			}

			m_pIBnGift[i]->RefreshItem(dwTypeID, 1, 0);
			m_pIBnGift[i]->SetBtnData(ESBT_Item, dwID, (DWORD)nPrice, (INT64)dwTypeIDTemp);
			m_pPickPatch[i]->SetInvisible(FALSE);
		}
	}

	INT nLpLevel = 1;
	BOOL bCanBuy = MallMgr::Inst()->InquireIndexForBuy(m_CurBtnLevel);
	LocalPlayer *pLP = RoleMgr::Inst()->GetLocalPlayer();
	if (P_VALID(pLP))
	{
		nLpLevel = pLP->GetRoleLevel();
		if (nLpLevel<nRealLevel)
		{
			bCanBuy = FALSE;
		}
	}
	//m_pBtnBuy->SetEnable(bCanBuy);

}

DWORD LevelPromoteFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	switch (pEvent->eEvent)
	{
	case EGUIE_Click:
		{
			if( pWnd == m_pBtnBuy )
			{
				if (m_dwSelectID!=GT_INVALID&&m_dwSelectID!=0)
				{
					TCHAR szNum[X_LONG_NAME] = {0};
					_stprintf( szNum, g_StrTable[_T("MallTrade5")], m_nSelectPrice );

					tstringstream tssInfo;
					tssInfo <<szNum;

					if(P_VALID(GetObj("Confirm_Buy")))
						KillObj("Confirm_Buy");
					CreateObj("Confirm_Buy", "MsgBox");

					TObjRef<MsgBox>("Confirm_Buy")->Init( 
						_T(""), tssInfo.str().c_str(),       
						_T("Confirm_Buy"),(MsgBoxFlag)(MBF_OK|MBF_Cancel), FALSE );
				}
			}
			else if ( pWnd == m_pClose )
			{
				ShowWnd(FALSE);
			}
			else if ( pWnd == m_pBtnPay )
			{
				YuanBaoAbundance();
			}
		}
		break;
	case EGUIE_Drag:
		{
			//左键单击,选中商品
			if(_tcsnccmp(pWnd->GetName().c_str(), _T("item"), 4) == 0)
			{
				for(int i=0; i<MAX_GIFT_PER_PAGE; i++)
				{
					if( pWnd == m_pIBnGift[i] )
					{
						RefreshItemFlag();
						m_pGiftBack[i]->SetPic(szSelectItem_GiftPlan);
						m_dwSelectID = m_pIBnGift[i]->GetBtnData().GetData1();
						m_nSelectPrice =  m_pIBnGift[i]->GetBtnData().GetData2();
						if(P_VALID(GetObj("Confirm_Buy")))
							KillObj("Confirm_Buy");
					}
				}
			}
		}
		break;
	case EGUIE_PushButtonDown:
		{
			if( pEvent->dwParam1 != 1 )
				return 0;
			
			//左键单击,选中商品
			if(_tcsnccmp(pWnd->GetName().c_str(), _T("optbutt"), 7) == 0)
			{
				for(int i=0; i<LEVEL_NUM_CHOOSE; i++)
				{
					if( pWnd == m_pPBnLevel[i] )
					{
						RefreshItemFlag();
						m_CurBtnLevel = i;
						ShowWnd(TRUE);
					}
				}
			}

		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			if(_tcsnccmp(pWnd->GetName().c_str(), _T("item"), 4) == 0)
			{
				for(int i=0; i<MAX_GIFT_PER_PAGE; i++)
				{
					if( pWnd == m_pIBnGift[i] )
					{
						DWORD dwID =  (DWORD)(m_pIBnGift[i]->GetBtnData().GetData3());
						ShowTip(m_pIBnGift[i], ToolTipCreator::Inst()->GetItemTips(dwID).c_str());
						return 0;
					}
				}
			}
		}
		break;
	case EGUIE_MoveOutOfCtrl:
		{
			ShowTip(NULL, _T(""));
		}
		break;
	default:
		break;
	}
	return 1;
}

VOID LevelPromoteFrame::ButtonEnableSet()
{
	INT nLpLevel = 1;
	LocalPlayer *pLP = RoleMgr::Inst()->GetLocalPlayer();
	if (P_VALID(pLP))
	{
		nLpLevel = pLP->GetRoleLevel();
		if (nLpLevel>=50)
		{
			
		}
		if (nLpLevel>=60)
		{

		}
		if (nLpLevel>=70)
		{

		}
		if (nLpLevel>=80)
		{

		}
	}

}


DWORD LevelPromoteFrame::OnOpenLevelPromoteFrame(tagOpenLevelPromoteFrame* pEvent)
{
	if (!P_VALID(m_pWnd))
	{
		return 0 ;
	}
	m_CurBtnLevel = pEvent->nIndex;
	for (int i=0;i<LEVEL_NUM_CHOOSE;++i)
	{
		m_pPBnLevel[i]->SetState(EGUIBS_PopUp);
	}
	m_pPBnLevel[m_CurBtnLevel]->SetState(EGUIBS_PushDown);
	ShowWnd(TRUE);
	return 0;
}

VOID  LevelPromoteFrame::ClearUI()
{
	if (!P_VALID(m_pWnd))
		return;

	RefreshItemFlag();
	for (int i=0; i<MAX_GIFT_PER_PAGE; ++i)
	{
		m_pIBnGift[i]->RefreshItem();
		m_pStcName[i]->SetText(_T(""));
		m_pStcPrice[i]->SetText(_T(""));
	}

	for (int i=0; i<LEVEL_NUM_CHOOSE; ++i)
	{
		m_pPickPatch[i]->SetInvisible(TRUE);
	}
	
	//m_pBtnBuy->SetEnable(FALSE);

}

void LevelPromoteFrame::RefreshItemFlag()
{
	m_dwSelectID = GT_INVALID;
	for(int i=0; i<MAX_GIFT_PER_PAGE; ++i)
	{
		m_pGiftBack[i]->SetPic(szUnSelectItem_GiftPlan);
	}
}


VOID   LevelPromoteFrame::OnConfirmMsg(tagMsgBoxEvent* pGameEvent)
{ 
	if (!P_VALID(m_pWnd))
		return;
	MsgBoxFlag eResault = TObjRef<MsgBox>("Confirm_Buy")->GetResult();
	if ( eResault != 1 )
		return;

	const tagMallItemProto* pItem = MallMgr::Inst()->GetMallItemByID(m_dwSelectID);
	if (P_VALID(pItem))
	{
		INT nRealLevel = 0;
		switch (m_CurBtnLevel) 
		{//按钮编号对应等级号
		case 1:
			nRealLevel = 60;
			break;
		case 2:
			nRealLevel = 70;
			break;
		case 3:
			nRealLevel = 80;
			break;
		default:
			nRealLevel = 50;
			break;
		}

		tagNC_MallBuyItem msg;
		msg.eShopingDistrict = Shopping_Item;
		msg.nIndexInServer = pItem->nIndexInServer;
		msg.dwTypeID = pItem->dwID;
		msg.n16BuyNum = 1;
		msg.nPrice = pItem->nPrice;
		msg.n16OfferItemLvl = (INT16)nRealLevel; 
		m_pSession->Send(&msg);
	}
	
}

DWORD  LevelPromoteFrame::OnNS_RoleLevelChange( tagNS_RoleLevelChange* pNetCmd, DWORD )
{
	if( !P_VALID(pNetCmd) )
		return 0;

	if( pNetCmd->dwRoleID != RoleMgr::Inst()->GetLocalPlayerID() )
		return 0;

	INT nLpLevel = pNetCmd->nLevel;
	if (nLpLevel>=50)
	{
		INT index = GT_INVALID;
		if (nLpLevel==50)
		{
			index = 0;
		}
		else if (nLpLevel==60)
		{
			index = 1;
		}
		else if (nLpLevel==70)
		{
			index = 2;
		}
		else if (nLpLevel==80)
		{
			index = 3;
		}
		
		if (index != GT_INVALID)
		{
			//BOOL bCanbuy = MallMgr::Inst()->InquireIndexForBuy(index);
			MallMgr::Inst()->SetIndexForBuy( index,TRUE );
			BOOL bGlint = m_bOnceGlintFlag[index];
			if (bGlint)
			{
				MallMgr::Inst()->SetIndexForClint( index,TRUE );
			}
			m_bOnceGlintFlag[index] = FALSE;
		}
	}

	if (!P_VALID(m_pWnd))
		return 0;
	
	if (!m_pWnd->IsInvisible())
	{
		RefreshUI();
	}
	return 0;
}

/*====================================================================
Base64编码函数
btSrc指向被编码的数据缓冲区
iSrcLen被编码的数据的大小（字节）
btRet指向存放Base64编码的数据缓冲区
piRetLen指向存放Base64编码的数据缓冲区的长度
若btRet为NULL函数返回0，piRetLen传回btSrc的Base64编码所需缓冲区的大小
若btRet指向的缓冲区太小，函数返回-1
否则函数返回实际btSrc的Base64编码所需缓冲区的大小
=====================================================================*/
int LevelPromoteFrame::EncodeBase64(BYTE *btSrc, int iSrcLen, BYTE *btRet, int *piRetLen) 
{
	int i = 0, j = 0, k = 0 ;
	BYTE EncodeBase64Map[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	i = (iSrcLen+2) / 3 * 4 ; //获取btSrc的Base64编码所需缓冲区的大小
	if(btRet != NULL)
	{
		if(*piRetLen < i) //Base64编码所需缓冲区偏小
			return -1 ;
		*piRetLen = i ; //*piRetLen返回btSrc的Base64编码的缓冲区大小
	}
	else
	{
		*piRetLen = i ;
		return 0 ;
	}

	k = iSrcLen - iSrcLen % 3 ;
	for(i=j=0; i<k; i+=3) //编码
	{
		btRet[j++] = EncodeBase64Map[(btSrc[i]>>2)&0x3F] ;
		btRet[j++] = EncodeBase64Map[((btSrc[i]<<4) + (btSrc[i+1]>>4))&0x3F] ;
		btRet[j++] = EncodeBase64Map[((btSrc[i+1]<<2) + (btSrc[i+2]>>6))&0x3F] ;
		btRet[j++] = EncodeBase64Map[btSrc[i+2]&0x3F] ;
	}
	k = iSrcLen - k ;
	if(1 == k)
	{
		btRet[j++] = EncodeBase64Map[(btSrc[i]>>2)&0x3F] ;
		btRet[j++] = EncodeBase64Map[(btSrc[i]<<4)&0x3F] ;
		btRet[j++] = '=' ;
		btRet[j] = '=' ;
	}
	else if(2 == k)
	{
		btRet[j++] = EncodeBase64Map[(btSrc[i]>>2)&0x3F] ;
		btRet[j++] = EncodeBase64Map[((btSrc[i]<<4) + (btSrc[i+1]>>4))&0x3F] ;
		btRet[j++] = EncodeBase64Map[(btSrc[i+1]<<2)&0x3F] ;
		btRet[j] = '=' ;
	}

	return ++j ;
}

void LevelPromoteFrame::YuanBaoAbundance()
{	
	tstring url;
	switch( TObjRef<NetSession>()->GetLoginMode())
	{
	case 1:
		{
			url = TObjRef<NetSession>()->GetExpressPayUrl();
		}
		break;
	case 2:  //迅雷特殊处理
		{
			url = _T("http://pay.xunlei.com/ourproducts_long.html");
			ShellExecute(NULL, _T("open"), url.c_str(), _T(""), NULL, SW_MAXIMIZE);
			return ;
		}
		break;
	default:
		{
			url = TObjRef<NetSession>()->GetExpressPayUrl();
			ShellExecute(NULL, _T("open"), url.c_str(), _T(""), NULL, SW_MAXIMIZE);
			return ;
		}
		break;
	}

	//读取帐号
	tstring strAccountTemp = SaveAccount::Inst()->GetAccount();

	tstring strBoroghID = SaveAccount::Inst()->GetWorldID();
	DWORD dwAccountID = SaveAccount::Inst()->GetAccountID();
	string strCannelID = SaveAccount::Inst()->GetJSKK_Channel();
	DWORD dwCropid = SaveAccount::Inst()->GetCropID();
	/*TObjRef<VarContainer> serInfo = CreateObj("lastlogin", "VarContainer");
	const TCHAR* szFileName = _T("config\\lastlogin.xml");
	if( serInfo->Load(NULL, szFileName) )
	{
	strBoroghID = serInfo->GetString(_T("world_id"), _T("last_login_server"), _T(""));
	}
	else
	{	
	strBoroghID = _T("");
	}
	KillObj("lastlogin");*/

	//Base64编码
	tstringstream ss2;
	if( ECT_JinShanKuaiKuai ==  dwCropid )
	{
		ss2 << strAccountTemp << _T("||") << ECT_JinShanKuaiKuai;
	}
	else
	{
		ss2 << strAccountTemp;
	}

	if (ECT_YYYuYinPingTai ==SaveAccount::Inst()->GetYYCropID() )	
		dwCropid = ECT_YYYuYinPingTai;

	string strAtemp = m_pUtil->UnicodeToAnsi(ss2.str().c_str());
	char btSrc[X_LONG_NAME] = {0};
	char btRet[X_LONG_NAME] = {0};
	int iRetLen = sizeof(btRet);
	_snprintf( btSrc, X_LONG_NAME, strAtemp.c_str() );
	EncodeBase64((BYTE*)btSrc, strlen(btSrc), (BYTE*)btRet, &iRetLen);

	tstring strAccount = m_pUtil->AnsiToUnicode(btRet);
	tstring strWchannel = m_pUtil->AnsiToUnicode(strCannelID.c_str());
	tstringstream ss;
	ss << _T("?cropid=") << dwCropid << _T("&accountname=") << strAccount << _T("&worldid=") << strBoroghID<< _T("&channel=") << strWchannel; 
	tstring strYuanBaoAbundance = url + ss.str();
	//打开充值网页

	ShellExecute(NULL, _T("open"), strYuanBaoAbundance.c_str(), _T(""), NULL, SW_MAXIMIZE);
}
