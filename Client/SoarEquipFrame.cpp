#include "StdAfx.h"
#include "SoarEquipFrame.h"
#include "CreatureData.h"
#include "DragBtn.h"
#include "ItemMgr.h"
#include "Item.h"
#include "Container.h"
#include "ItemButton.h"
#include "..\WorldDefine\msg_item.h"
#include "..\WorldDefine\msg_compose.h"
#include "..\WorldDefine\shop_define.h"
#include "IconStatic.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "RoleMgr.h"
#include "ItemEvent.h"

const INT SOAR_MIXING_EQUIP_MIN_LEVEL = 75;
const INT SOAR_MINING_EQUIP_MIN_QUALITY =3;
const INT SOAR_MINING_STUFF_TYPEEX = 37;
const TCHAR* szMixSucceed = _T("data\\ui\\compose\\l_smith-sucess.dds");
const TCHAR* szMixFailed = _T("data\\ui\\compose\\l_smith-lose.dds");

SoarEquipFrame::SoarEquipFrame(void)
:m_trunk(this)
,m_pWnd(NULL)
,m_pBtnSoarEquip(NULL)
,m_pBtnMixEquip(NULL)
,m_pBtnMixStuff(NULL)
,m_pBtnConfirmMix(NULL)
,m_pBtnCancel(NULL)
,m_pProMix(NULL)
,m_pPatPercentShow(NULL)
,m_pStcSoarEquip(NULL)
,m_pStcMixEquip(NULL)
,m_pStcMixStuff(NULL)
,m_pStcMixResaultShow(NULL)
,m_dwNPCID(GT_INVALID)
{
	MixDataInit();
}

SoarEquipFrame::~SoarEquipFrame(void)
{
}

VOID  SoarEquipFrame::MixDataInit()
{
	m_pItemSoarEquip = NULL;
	m_pItemMixStuff = NULL;
	m_pItemMixEquip = NULL;
	m_fSoarPercent = 0;
	m_iEquipType = 0;
	m_iEquipLevel = 0;
	m_iEquipQuality = 0;
	m_iSoarEquipType = 0;
	m_iSoarEquipLevel = 0;
	m_iSoarEquipQuality = 0;
	m_bIfMix = FALSE;
	m_bIfProgressing = FALSE;
	m_escapeTime = GT_INVALID;
	m_bSpeedNormal = TRUE;
}

BOOL SoarEquipFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if (!GameFrame::Init(pMgr, pFather, dwParam))
	{
		return FALSE;
	}

	m_pMgr->RegisterEventHandle(_T("Close_SoarEquipFrame"),(FRAMEEVENTPROC)m_trunk.sfp1(&SoarEquipFrame::OnCloseSoarEquipFrame));
	m_pMgr->RegisterEventHandle(_T("EquipDrop"),(FRAMEEVENTPROC)m_trunk.sfp1(&SoarEquipFrame::PocketEquipDrop));
	m_pCmdMgr->Register("NS_BuildEuipment",(NETMSGPROC)m_trunk.sfp2(&SoarEquipFrame::NetRecMixEquipMsg),_T("NS_BuildEuipment"));
	m_pCmdMgr->Register("NS_ItemRemove",(NETMSGPROC)m_trunk.sfp2(&SoarEquipFrame::NetItemRemove), _T("NS_ItemRemove"));
	

	
	MixDataInit();
	return TRUE;
}

BOOL SoarEquipFrame::Destroy()
{
	if (!GameFrame::Destroy())
		return FALSE;
	
	//m_pGUI->UnRegisterEventHandler(m_pWnd->GetFullName().c_str(), m_trunk.sfp1(&SoarEquipFrame::EventHandler));
	
	m_pMgr->UnRegisterEventHandler(_T("Close_SoarEquipFrame"),(FRAMEEVENTPROC)m_trunk.sfp1(&SoarEquipFrame::OnCloseSoarEquipFrame));
	m_pMgr->UnRegisterEventHandler(_T("EquipDrop"),(FRAMEEVENTPROC)m_trunk.sfp1(&SoarEquipFrame::PocketEquipDrop));

	m_pCmdMgr->UnRegister("NS_BuildEuipment",(NETMSGPROC)m_trunk.sfp2(&SoarEquipFrame::NetRecMixEquipMsg));
	m_pCmdMgr->UnRegister("NS_ItemRemove",(NETMSGPROC)m_trunk.sfp2(&SoarEquipFrame::NetItemRemove));
	
	DestroyMsgBox();

	if (P_VALID(m_pWnd))
	{
		m_pGUI->AddToDestroyList(m_pWnd);
		m_pWnd = NULL;
	}

	return TRUE;
}

VOID SoarEquipFrame::DestroyMsgBox()
{
	if(P_VALID(GetObj("MixSucceed")))
		KillObj("MixSucceed");
	if(P_VALID(GetObj("MixFailed")))
		KillObj("MixFailed");
	if(P_VALID(GetObj("NoEnoughSpace")))
		KillObj("NoEnoughSpace");
	if(P_VALID(GetObj("NoSoarEquip")))
		KillObj("NoSoarEquip");
	if(P_VALID(GetObj("LackOfStuff")))
		KillObj("LackOfStuff");
	if(P_VALID(GetObj("ItemIsNotEquip")))
		KillObj("ItemIsNotEquip");
	if(P_VALID(GetObj("EquipSoared")))
		KillObj("EquipSoared");
	if(P_VALID(GetObj("EquipLevelIsNotEnough")))
		KillObj("EquipLevelIsNotEnough");
	if(P_VALID(GetObj("EquipQualityIsNotEnough")))
		KillObj("EquipQualityIsNotEnough");
	if(P_VALID(GetObj("ItemIsNotRight")))
		KillObj("ItemIsNotRight");
	if(P_VALID(GetObj("SoarEquipInfo")))
		KillObj("SoarEquipInfo");
	if(P_VALID(GetObj("MixStuffInfo")))
		KillObj("MixStuffInfo");
	if(P_VALID(GetObj("MixEquipInfo")))
		KillObj("MixEquipInfo");
	if( P_VALID(GetObj("EquipUnidetify")) )
		KillObj("EquipUnidetify");
	if( P_VALID(GetObj("FaoBaoIsNotOK")) )
		KillObj("FaoBaoIsNotOK");
	if( P_VALID(GetObj("Fashion_Is_Not_OK")) )
		KillObj("Fashion_Is_Not_OK");
	if( P_VALID(GetObj("TimeLimite_Is_Not_OK")) )
		KillObj("TimeLimite_Is_Not_OK");
	
}

DWORD SoarEquipFrame::OnCloseSoarEquipFrame(tagGameEvent* pGameEvent)
{
	//装备还原
	m_pWnd->SetInvisible(TRUE);
	MixDataInit();
	LockItem(FALSE);
	m_pProMix->SetValue(0);
	m_pProMix->SetSpeed(100000000.0f);
	m_bSpeedNormal = FALSE;

	return 0;
}

DWORD SoarEquipFrame::NetRecMixEquipMsg( tagNS_BuildEuipment *pNetCmd, DWORD )
{
	m_pStcMixResaultShow->SetInvisible(FALSE);
	//恢复炼器读条时不可操作项
	LockItem(FALSE);
	MixDataInit();


	DWORD cmd = pNetCmd->dwErrorCode;
	if ( cmd == 0 )  //pNetCmd = succeed
	{
		m_pStcMixResaultShow->SetPic(szMixSucceed, tagRect(0, 0, 220, 100));//显示成功
		m_pStcMixResaultShow->FlipToTop();
		//进度条置0 加速
		return 0;
	} 
	else if ( cmd == E_Compose_NPC_Not_Exist )
	{
		if ( P_VALID(GetObj("NPCNotExist")) )
			KillObj("NPCNotExist");
		CreateObj( "NPCNotExist", "MsgBoxEx" );
		TObjRef<MsgBoxEx>("NPCNotExist")->Init( _T(""), g_StrTable[_T("NPCNotExist")], _T("NPCNotExist") );

	} 
	else if ( cmd == E_Compose_NPC_Distance )
	{
		if ( P_VALID(GetObj("NPCDistance")) )
			KillObj("NPCDistance");
		CreateObj( "NPCDistance", "MsgBoxEx" );
		TObjRef<MsgBoxEx>("NPCDistance")->Init( _T(""), g_StrTable[_T("NPCDistance")], _T("NPCDistance") );

	} 
	else if ( cmd == E_SoaringEquipment_NPC_Invalid )
	{
		if ( P_VALID(GetObj("NPCInvalid")) )
			KillObj("NPCInvalid");
		CreateObj( "NPCInvalid", "MsgBoxEx" );
		TObjRef<MsgBoxEx>("NPCInvalid")->Init( _T(""), g_StrTable[_T("NPCInvalid")], _T("NPCInvalid") );

	} 
	else if ( cmd == E_Consolidate_Equip_Not_Exist )
	{
		if ( P_VALID(GetObj("NoSoarEquip")) )
			KillObj("NoSoarEquip");
		CreateObj( "NoSoarEquip", "MsgBoxEx" );
		TObjRef<MsgBoxEx>("NoSoarEquip")->Init( _T(""), g_StrTable[_T("NoSoarEquip")], _T("NoSoarEquip") );
	} 
	else if ( cmd == E_SoaringStone_NotExist )
	{
		if ( P_VALID(GetObj("LackOfStuff")) )
			KillObj("LackOfStuff");
		CreateObj( "LackOfStuff", "MsgBoxEx" );
		TObjRef<MsgBoxEx>("LackOfStuff")->Init( _T(""), g_StrTable[_T("LackOfStuff")], _T("LackOfStuff") );
	} 
	else if ( cmd == E_SoaringEquipment_NotEquip )
	{
		if( P_VALID(GetObj("ItemIsNotEquip")) )
			KillObj("ItemIsNotEquip");
		CreateObj( "ItemIsNotEquip", "MsgBoxEx" );
		TObjRef<MsgBoxEx>("ItemIsNotEquip")->Init( _T(""), g_StrTable[_T("ItemIsNotEquip")], _T("ItemIsNotEquip") );
	} 
	else if ( cmd == E_SoaringEquipment_LevelLower )
	{
		if( P_VALID(GetObj("EquipLevelIsNotEnough")) )
			KillObj("EquipLevelIsNotEnough");
		CreateObj( "EquipLevelIsNotEnough", "MsgBoxEx" );
		TObjRef<MsgBoxEx>("EquipLevelIsNotEnough")->Init( _T(""), g_StrTable[_T("EquipLevelIsNotEnough")], _T("EquipLevelIsNotEnough") );

	} 
	else if ( cmd == E_SoaringEquipment_NotAccessories )
	{
		if( P_VALID(GetObj("NotAccessories")) )
			KillObj("NotAccessories");
		CreateObj( "NotAccessories", "MsgBoxEx" );
		TObjRef<MsgBoxEx>("NotAccessories")->Init( _T(""), g_StrTable[_T("NotAccessories")], _T("NotAccessories") );
	} 
	else if ( cmd == E_SoaringEquipment_StuffMustBeEquip )
	{
		if( P_VALID(GetObj("ItemIsNotEquip")) )
			KillObj("ItemIsNotEquip");
		CreateObj( "ItemIsNotEquip", "MsgBoxEx" );
		TObjRef<MsgBoxEx>("ItemIsNotEquip")->Init( _T(""), g_StrTable[_T("ItemIsNotEquip")], _T("ItemIsNotEquip") );
	}
	else if ( cmd == E_SoaringEquipment_StuffQltyLower )
	{
		if( P_VALID(GetObj("EquipQualityIsNotEnough")) )
			KillObj("EquipQualityIsNotEnough");
		CreateObj( "EquipQualityIsNotEnough", "MsgBoxEx" );
		TObjRef<MsgBoxEx>("EquipQualityIsNotEnough")->Init( _T(""), g_StrTable[_T("EquipQualityIsNotEnough")], _T("EquipQualityIsNotEnough") );
	}
	else if ( cmd == E_SoaringEquipment_NotIdentify)
	{
		if( P_VALID(GetObj("EquipUnidetify")) )
			KillObj("EquipUnidetify");
		CreateObj( "EquipUnidetify", "MsgBoxEx" );
		TObjRef<MsgBoxEx>("EquipUnidetify")->Init( _T(""), g_StrTable[_T("EquipUnidetify")], _T("EquipUnidetify") );
	} 
	else if ( cmd == E_SoaringEquipment_FabaoHasBeenProhibited)
	{
		if( P_VALID(GetObj("FaoBaoIsNotOK")) )
			KillObj("FaoBaoIsNotOK");
		CreateObj( "FaoBaoIsNotOK", "MsgBoxEx" );
		TObjRef<MsgBoxEx>("FaoBaoIsNotOK")->Init( _T(""), g_StrTable[_T("FaoBaoIsNotOK")], _T("FaoBaoIsNotOK") );

	}
	else if ( cmd == E_SoaringEquipment_FashionHasBeenProhibited)
	{
		if( P_VALID(GetObj("Fashion_Is_Not_OK")) )
			KillObj("Fashion_Is_Not_OK");
		CreateObj( "Fashion_Is_Not_OK", "MsgBoxEx" );
		TObjRef<MsgBoxEx>("Fashion_Is_Not_OK")->Init( _T(""), g_StrTable[_T("Fashion_Is_Not_OK")], _T("Fashion_Is_Not_OK") );
	}
	else if ( cmd == E_SoaringEquipment_TimelimittedEquipHasBeenProhibited)
	{
		if( P_VALID(GetObj("TimeLimite_Is_Not_OK")) )
			KillObj("TimeLimite_Is_Not_OK");
		CreateObj( "TimeLimite_Is_Not_OK", "MsgBoxEx" );
		TObjRef<MsgBoxEx>("TimeLimite_Is_Not_OK")->Init( _T(""), g_StrTable[_T("TimeLimite_Is_Not_OK")], _T("TimeLimite_Is_Not_OK") );

	}
	else if ( cmd == E_SoaringEquipment_Fail )
	{
		m_pStcMixResaultShow->SetPic(szMixFailed,tagRect(0, 0, 220, 100));//显示失败
		m_pStcMixResaultShow->FlipToTop();
	}

	
	return 0;
}

VOID SoarEquipFrame::ShowMixFrame()
{
	m_pWnd->SetInvisible(FALSE);
	m_pProMix->SetSpeed(0.25);
	MixDataInit();
}

void SoarEquipFrame::Update()
{
	GameFrame::Update();

	//检测是否离NPC过远，过远则关闭frame
	m_escapeTime += Kernel::Inst()->GetDeltaTimeDW();
	if( m_escapeTime > 1000 )//每隔1秒检测距离目标NPC的距离
	{
		m_escapeTime = 0;
		m_pStcMixResaultShow->SetNoPic();//图片只显示1秒
		LocalPlayer* pLocalPlayer = RoleMgr::Inst()->GetLocalPlayer();
		if( !P_VALID(pLocalPlayer) )
		{
			Destroy();
			return;
		}
		if (RoleMgr::Inst()->IsOutValidDist( m_dwNPCID ) ) 
		{
			LockItem(FALSE);
			tagGameEvent event(_T("Close_SoarEquipFrame"), this);
			m_pMgr->SendEvent(&event);
		}
	}

	MixPercentCompute();
	RefreshBtnUI();
	RefreshProUI();
	
}

VOID  SoarEquipFrame::RefreshProUI()
{
	//进度条走满后
	if ( (m_pProMix->GetCurrentValue()<1.0+FOALT_ZERO) 
	  && (m_pProMix->GetCurrentValue()>1.0-FOALT_ZERO) 
	  &&  m_bIfProgressing == TRUE )
	{
		m_pProMix->SetValue(0);
		m_pProMix->SetSpeed(1000000000.0f);
		m_bSpeedNormal = FALSE;
		//先向服务器发送消息
		SendMixInfoToServer();
	}
	//读条速度恢复
	if ( (m_pProMix->GetCurrentValue()<FOALT_ZERO) 
	  && (m_pProMix->GetCurrentValue()>-FOALT_ZERO) 
	  && m_bSpeedNormal == FALSE )
	{
		m_pProMix->SetSpeed(0.25);
		m_bSpeedNormal = TRUE;
	}

}

VOID SoarEquipFrame::RefreshBtnUI()
{
	//读条过程中不用刷新
	if ( m_bIfProgressing == TRUE )
	{  
		return;
	}

	if ( P_VALID( m_pItemSoarEquip ) )
	{
		m_pBtnSoarEquip->RefreshItem( m_pItemSoarEquip->GetItemTypeID(), 0, m_iSoarEquipQuality );   
	}
	else
	{
		//m_pBtnConfirmMix->SetEnable(FALSE);
		m_pBtnSoarEquip->RefreshItem();
	}

	if ( P_VALID( m_pItemMixEquip ) )
	{
		m_pBtnMixEquip->RefreshItem(m_pItemMixEquip->GetItemTypeID(), 0, m_iEquipQuality );
	}
	else
	{
		m_pBtnMixEquip->RefreshItem();
	}

	if ( P_VALID( m_pItemMixStuff ) )
	{
		m_pBtnMixStuff->RefreshItem(m_pItemMixStuff->GetItemTypeID(),0,m_pItemMixStuff->GetItemQuality()); 
	}
	else
	{
		m_pBtnMixStuff->RefreshItem();
	}

}

VOID   SoarEquipFrame::MixPercentCompute()
{
	//读条过程中不用计算
	if ( m_bIfProgressing == TRUE )
	{  
		return;
	}
	//计算提升百分比
 	m_fSoarPercent = (FLOAT)((0.1 + m_iEquipLevel*m_iEquipLevel*(m_iEquipQuality*m_iEquipQuality-8))/200000);
	
	if ( m_pItemSoarEquip == NULL || m_fSoarPercent <= FOALT_ZERO )  
	{
		m_pPatPercentShow->SetText(_T(""));
		return;
	} 

	if (m_fSoarPercent>FOALT_ZERO&&m_fSoarPercent<0.1+FOALT_ZERO)
	{
		m_pPatPercentShow->SetText(g_StrTable[_T("0to10Percent")]);
		m_pPatPercentShow->SetTextColor(0xFFFFFFFF);
	} 
	else if (m_fSoarPercent>0.11 && m_fSoarPercent<0.15+FOALT_ZERO)
	{
		m_pPatPercentShow->SetText(g_StrTable[_T("11to15Percent")]);
		m_pPatPercentShow->SetTextColor(0xFFFFFF5B);
	} 
	else if (m_fSoarPercent>0.16 && m_fSoarPercent<0.3+FOALT_ZERO)
	{
		m_pPatPercentShow->SetText(g_StrTable[_T("16to30Percent")]);
		m_pPatPercentShow->SetTextColor(0xFF00FF00);
	} 
	else if (m_fSoarPercent>0.31 && m_fSoarPercent<0.5+FOALT_ZERO)
	{
		m_pPatPercentShow->SetText(g_StrTable[_T("31to50Percent")]);
		m_pPatPercentShow->SetTextColor(0xFF2828FF);
	} 
	else if (m_fSoarPercent>0.5 && m_fSoarPercent<0.8+FOALT_ZERO)
	{
		m_pPatPercentShow->SetText(g_StrTable[_T("51to80Percent")]);
		m_pPatPercentShow->SetTextColor(0xFFFF8000);
	}
	else if (m_fSoarPercent>0.8 && m_fSoarPercent<1.0+FOALT_ZERO)
	{
		m_pPatPercentShow->SetText(g_StrTable[_T("81to100Percent")]);
		m_pPatPercentShow->SetTextColor(0xFF8F3F94);
	}

}
BOOL SoarEquipFrame::ReloadUI()
{

	if (P_VALID(m_pWnd))
		m_pGUI->AddToDestroyList(m_pWnd);
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\equip_mixing.xml");
	XmlElement ele;
	if (!m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str()))
		return FALSE;
	m_pGUI->ChangeXml(&ele, "equip_mixing_win\\equip_mixing_back\\equip1back_pic\\equip1_back\\equip1", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "equip_mixing_win\\equip_mixing_back\\equip2back_pic\\equip2_back\\equip2", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "equip_mixing_win\\equip_mixing_back\\equip3back_pic\\equip3_back\\equip3", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&ele, "equip_mixing_win\\equip_mixing_back\\show", "ClassName", "IconStatic");

	m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &ele);
	m_pPatPercentShow = (GUIPatch*)m_pWnd->GetChild(_T("equip_mixing_back\\percentword"));
	m_pStcSoarEquip = (GUIStatic*)m_pWnd->GetChild(_T("equip_mixing_back\\equip1back_pic\\equip1_back"));
	m_pStcMixStuff	= (GUIStatic*)m_pWnd->GetChild(_T("equip_mixing_back\\equip2back_pic\\equip2_back"));
	m_pStcMixEquip	= (GUIStatic*)m_pWnd->GetChild(_T("equip_mixing_back\\equip3back_pic\\equip3_back"));
	m_pStcMixResaultShow = (IconStatic*)m_pWnd->GetChild(_T("equip_mixing_back\\show"));

	m_pBtnSoarEquip = (ItemButton*)m_pStcSoarEquip->GetChild(_T("equip1"));
	m_pBtnMixStuff	= (ItemButton*)m_pStcMixStuff->GetChild(_T("equip2"));
	m_pBtnMixEquip	= (ItemButton*)m_pStcMixEquip->GetChild(_T("equip3"));

	m_pBtnConfirmMix  = (GUIButton*)m_pWnd->GetChild(_T("equip_mixing_back\\surebutton")); 
	m_pBtnCancel	  = (GUIButton*)m_pWnd->GetChild(_T("equip_mixing_back\\closebutton0"));
	m_pProMix		  = (GUIProgress*)m_pWnd->GetChild(_T("equip_mixing_back\\progressbarback\\progressbar"));
	m_pProMix->SetMaxValue(1.0);
	m_pProMix->SetSpeed(0.25); //4秒走完
	m_bSpeedNormal = TRUE;
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_trunk.sfp1(&SoarEquipFrame::EventHandler)); 
	m_pWnd->SetInvisible(FALSE);

	return TRUE;
}

BOOL  SoarEquipFrame::EquipMixing()
{
	
	ItemContainer* pItemCon = ItemMgr::Inst()->GetPocket();
	
	if( !P_VALID(pItemCon))	
	{
		return FALSE;
	}

	if ( m_pItemSoarEquip == NULL )  //请放入想要炼制的装备
	{
		if ( P_VALID(GetObj("NoSoarEquip")) )
			KillObj("NoSoarEquip");
		CreateObj( "NoSoarEquip", "MsgBoxEx" );
		TObjRef<MsgBoxEx>("NoSoarEquip")->Init( _T(""), g_StrTable[_T("NoSoarEquip")], _T("NoSoarEquip") );
		return FALSE;
	}
	
	//INT  restspace = pItemCon->MaxSize() - pItemCon->Size();
	//if ( restspace <= 5 )  //剩余空间小于5，炼器失败
	//{
	//	if ( P_VALID(GetObj("NoEnoughSpace")) )
	//		KillObj("NoEnoughSpace");
	//	CreateObj( "NoEnoughSpace", "MsgBoxEx" );
	//	TObjRef<MsgBoxEx>("NoEnoughSpace")->Init( _T(""), g_StrTable[_T("NoEnoughSpace")], _T("NoEnoughSpace") );
	//	return FALSE;
	//}
	
	if ( m_pItemMixStuff == NULL )    //道具栏没有材料，炼器失败
	{
		if ( P_VALID(GetObj("LackOfStuff")) )
			KillObj("LackOfStuff");
		CreateObj( "LackOfStuff", "MsgBoxEx" );
		TObjRef<MsgBoxEx>("LackOfStuff")->Init( _T(""), g_StrTable[_T("LackOfStuff")], _T("LackOfStuff") );
		return FALSE;
	}

	return TRUE;
}

VOID  SoarEquipFrame::TestSoarEquip(INT nPos)
{
	m_pBtnConfirmMix->SetEnable(TRUE);

	m_pItemSoarEquip = ItemMgr::Inst()->GetPocketItem(nPos);
	if (!P_VALID(m_pItemSoarEquip))
	{
		m_pItemSoarEquip = NULL;
		return;
	}
	m_iSoarEquipType = m_pItemSoarEquip->GetItemId();
	Equipment* pEquip = (Equipment*)ItemMgr::Inst()->GetPocketItemByID(m_iSoarEquipType);
	if( !P_VALID(pEquip) || !P_VALID(pEquip->GetEquip()) )
	{
		m_pItemSoarEquip = NULL;
		return;
	}
	m_iSoarEquipQuality = pEquip->GetItemQuality();
	m_iSoarEquipLevel =  m_pItemSoarEquip->GetItemLevel();

	//检验是否非装备
	if ( !MIsEquipment(m_pItemSoarEquip->GetItemTypeID()) )
	{ 
		if( P_VALID(GetObj("ItemIsNotEquip")) )
			KillObj("ItemIsNotEquip");
		CreateObj( "ItemIsNotEquip", "MsgBoxEx" );
		TObjRef<MsgBoxEx>("ItemIsNotEquip")->Init( _T(""), g_StrTable[_T("ItemIsNotEquip")], _T("ItemIsNotEquip") );
		m_pItemSoarEquip = NULL;
		return;
	}

	//未鉴定装备不可炼器
	int qlty = pEquip->GetItemQuality();
	if( qlty < EIQ_White || qlty >= EIQ_End )
	{
		if( P_VALID(GetObj("EquipUnidetify")) )
			KillObj("EquipUnidetify");
		CreateObj( "EquipUnidetify", "MsgBoxEx" );
		TObjRef<MsgBoxEx>("EquipUnidetify")->Init( _T(""), g_StrTable[_T("EquipUnidetify")], _T("EquipUnidetify") );
		m_pItemSoarEquip = NULL;
		return;
	}

	//是否已经飞升判断
	if ( pEquip->GetEquip()->bSoaringFlag )  
	{
		if( P_VALID(GetObj("EquipSoared")) )
			KillObj("EquipSoared");
		CreateObj( "EquipSoared", "MsgBoxEx" );
		TObjRef<MsgBoxEx>("EquipSoared")->Init( _T(""), g_StrTable[_T("EquipSoared")], _T("EquipSoared") );
		m_pItemSoarEquip = NULL;
		return;
	}

	//是否满足大于75级
	if ( m_iSoarEquipLevel < SOAR_MIXING_EQUIP_MIN_LEVEL )
	{
		if( P_VALID(GetObj("EquipLevelIsNotEnough")) )
			KillObj("EquipLevelIsNotEnough");
		CreateObj( "EquipLevelIsNotEnough", "MsgBoxEx" );
		TObjRef<MsgBoxEx>("EquipLevelIsNotEnough")->Init( _T(""), g_StrTable[_T("EquipLevelIsNotEnough")], _T("EquipLevelIsNotEnough") );
		m_pItemSoarEquip = NULL;
		return;
	}

	//法宝不能炼器
	if ( m_pItemSoarEquip->GetProto()->eType == EIT_FaBao )
	{
		if( P_VALID(GetObj("FaoBaoIsNotOK")) )
			KillObj("FaoBaoIsNotOK");
		CreateObj( "FaoBaoIsNotOK", "MsgBoxEx" );
		TObjRef<MsgBoxEx>("FaoBaoIsNotOK")->Init( _T(""), g_StrTable[_T("FaoBaoIsNotOK")], _T("FaoBaoIsNotOK") );
		m_pItemSoarEquip = NULL;
		return;
	}

	//时装不能炼器
	if ( m_pItemSoarEquip->GetProto()->eType == EIT_Fashion )
	{
		if( P_VALID(GetObj("Fashion_Is_Not_OK")) )
			KillObj("Fashion_Is_Not_OK");
		CreateObj( "Fashion_Is_Not_OK", "MsgBoxEx" );
		TObjRef<MsgBoxEx>("Fashion_Is_Not_OK")->Init( _T(""), g_StrTable[_T("Fashion_Is_Not_OK")], _T("Fashion_Is_Not_OK") );
		m_pItemSoarEquip = NULL;
		return;
	}

	//限时装备不能炼器
	DWORD dwTempTimeLimit = m_pItemSoarEquip->GetProto()->dwTimeLimit;
	if ( dwTempTimeLimit!= 0  && dwTempTimeLimit != -1  )
	{
		if( P_VALID(GetObj("TimeLimite_Is_Not_OK")) )
			KillObj("TimeLimite_Is_Not_OK");
		CreateObj( "TimeLimite_Is_Not_OK", "MsgBoxEx" );
		TObjRef<MsgBoxEx>("TimeLimite_Is_Not_OK")->Init( _T(""), g_StrTable[_T("TimeLimite_Is_Not_OK")], _T("TimeLimite_Is_Not_OK") );
		m_pItemSoarEquip = NULL;
		return;
	}

	//防止同一道具放两个槽
	if ( m_pItemMixEquip != NULL )   
	{
		if ( m_iSoarEquipType == m_iEquipType )
		{
			m_pItemMixEquip = NULL;
			m_iEquipLevel = 0;    //飞升合成道具的装备等级
			m_iEquipQuality = 0;  //飞升合成道具装备品质
		}
		return;
	}

}

VOID  SoarEquipFrame::TestMixEquip(INT nPos)
{
	m_pItemMixEquip = ItemMgr::Inst()->GetPocketItem(nPos); 
	if (!P_VALID(m_pItemMixEquip))
	{
		m_pItemMixEquip = NULL;
		return;
	}

	//检验是否非装备
	if ( !MIsEquipment(m_pItemMixEquip->GetItemTypeID()) )
	{
		if( P_VALID(GetObj("ItemIsNotEquip")) )
			KillObj("ItemIsNotEquip");
		CreateObj( "ItemIsNotEquip", "MsgBoxEx" );
		TObjRef<MsgBoxEx>("ItemIsNotEquip")->Init( _T(""), g_StrTable[_T("ItemIsNotEquip")], _T("ItemIsNotEquip") );
		m_pItemMixEquip = NULL;
		return;
	}

	//炼器装备合成材料品质是否大于3
	m_iEquipType = m_pItemMixEquip->GetItemId();
	Equipment* pEquip = (Equipment*)ItemMgr::Inst()->GetPocketItemByID(m_iEquipType);
	m_iEquipQuality = pEquip->GetItemQuality();
	m_iEquipLevel = m_pItemMixEquip->GetItemLevel();
	if ( m_iEquipQuality < SOAR_MINING_EQUIP_MIN_QUALITY ) 
	{
		if( P_VALID(GetObj("EquipQualityIsNotEnough")) )
			KillObj("EquipQualityIsNotEnough");
		CreateObj( "EquipQualityIsNotEnough", "MsgBoxEx" );
		TObjRef<MsgBoxEx>("EquipQualityIsNotEnough")->Init( _T(""), g_StrTable[_T("EquipQualityIsNotEnough")], _T("EquipQualityIsNotEnough") );
		m_iEquipQuality = 0;
		m_iEquipLevel   = 0;
		m_pItemMixEquip = NULL;
		return;
	}
	
	//未鉴定装备不可炼器
	int qlty = pEquip->GetItemQuality();
	if( qlty < EIQ_White || qlty >= EIQ_End )
	{
		if( P_VALID(GetObj("EquipUnidetify")) )
			KillObj("EquipUnidetify");
		CreateObj( "EquipUnidetify", "MsgBoxEx" );
		TObjRef<MsgBoxEx>("EquipUnidetify")->Init( _T(""), g_StrTable[_T("EquipUnidetify")], _T("EquipUnidetify") );
		m_pItemMixEquip = NULL;
		return;
	}

	//法宝不能炼器
	if ( m_pItemMixEquip->GetProto()->eType == EIT_FaBao )
	{
		if( P_VALID(GetObj("FaoBaoIsNotOK")) )
			KillObj("FaoBaoIsNotOK");
		CreateObj( "FaoBaoIsNotOK", "MsgBoxEx" );
		TObjRef<MsgBoxEx>("FaoBaoIsNotOK")->Init( _T(""), g_StrTable[_T("FaoBaoIsNotOK")], _T("FaoBaoIsNotOK") );
		m_pItemMixEquip = NULL;
		return;
	}

	//时装不能炼器
	if ( m_pItemMixEquip->GetProto()->eType == EIT_Fashion )
	{
		if( P_VALID(GetObj("Fashion_Is_Not_OK")) )
			KillObj("Fashion_Is_Not_OK");
		CreateObj( "Fashion_Is_Not_OK", "MsgBoxEx" );
		TObjRef<MsgBoxEx>("Fashion_Is_Not_OK")->Init( _T(""), g_StrTable[_T("Fashion_Is_Not_OK")], _T("Fashion_Is_Not_OK") );
		m_pItemMixEquip = NULL;
		return;
	}

	//限时装备不能炼器
	DWORD dwTempTimeLimit = m_pItemMixEquip->GetProto()->dwTimeLimit;
	if ( dwTempTimeLimit!= 0  && dwTempTimeLimit != -1  )
	{
		if( P_VALID(GetObj("TimeLimite_Is_Not_OK")) )
			KillObj("TimeLimite_Is_Not_OK");
		CreateObj( "TimeLimite_Is_Not_OK", "MsgBoxEx" );
		TObjRef<MsgBoxEx>("TimeLimite_Is_Not_OK")->Init( _T(""), g_StrTable[_T("TimeLimite_Is_Not_OK")], _T("TimeLimite_Is_Not_OK") );
		m_pItemMixEquip = NULL;
		return;
	}

	//防止同一道具放两个槽
	if ( m_pItemSoarEquip != NULL )   //防止同一道具放分别两次在两个槽
	{
		if ( m_iSoarEquipType == m_iEquipType )
		{
			m_pItemSoarEquip = NULL;
			m_iSoarEquipLevel = 0; //待飞升装备等级
			m_iSoarEquipQuality = 0;//待飞升装备品质
		}
		return;
	}

}

VOID  SoarEquipFrame::TestMixStuff(INT nPos)
{
	m_pItemMixStuff = ItemMgr::Inst()->GetPocketItem(nPos);
	if (!P_VALID(m_pItemMixStuff) )
	{
		m_pItemMixStuff = NULL;
		return;
	}

	//检测设否是为道具，且道具TypeEx = 37
	if ( MIsEquipment(m_pItemMixStuff->GetItemTypeID()) 
		 || m_pItemMixStuff->GetItemTypeEx() != SOAR_MINING_STUFF_TYPEEX )
	{
		if( P_VALID(GetObj("ItemIsNotRight")) )
			KillObj("ItemIsNotRight");
		CreateObj( "ItemIsNotRight", "MsgBoxEx" );
		TObjRef<MsgBoxEx>("ItemIsNotRight")->Init( _T(""), g_StrTable[_T("ItemIsNotRight")], _T("ItemIsNotRight") );
		m_pItemMixStuff = NULL;
		return;
	}

}

VOID  SoarEquipFrame::DragBtnInMixFrame(ItemButton*	btn,Item* itm)
{

	DragBtn* pDrag = (DragBtn*)m_pGUI->GetWnd(_T("\\desktop\\DragButton"));
	INT16    n16Num = itm->GetItemQuantity();
	if (P_VALID(pDrag))
	{
		//发送游戏事件
		tagGUIEvent event(btn, EGUIE_Drag, NULL);
		POINT pt;
		::GetCursorPos(&pt);
		event.dwParam1 = EDT_Item;
		event.dwParam2 = itm->GetPos(); 
		event.dwParam3 = itm->GetItemTypeID();
		pDrag->OnEvent(&event);
	}

}

VOID  SoarEquipFrame::SendMixInfoToServer()
{
	//向服务器发消息	
	tagNC_BuildEuipment  msg;
	msg.dwNPCID = m_dwNPCID;
	// 有可能没有放加成炼器百分比的装备
	// 待修炼装备64位id
	if ( m_pItemSoarEquip == NULL )
	{
		msg.n64ToBuildEquipID = GT_INVALID; 
	}
	else
	{
		msg.n64ToBuildEquipID = m_pItemSoarEquip->GetItemId();	
	}
	// 炼石ID
	if ( m_pItemMixStuff == NULL )
	{
		msg.n64StoneID = GT_INVALID; 
	}
	else
	{
		msg.n64StoneID = m_pItemMixStuff->GetItemId();	
	}
	// 辅料id，也是装备
	if ( m_pItemMixEquip == NULL )	 
	{
		msg.n64StuffID = GT_INVALID;  
	} 
	else
	{
		msg.n64StuffID = m_pItemMixEquip->GetItemId();	
	}
	m_pSession->Send(&msg);

}

VOID  SoarEquipFrame::ReceiveNPCID(DWORD dwNPCID)
{
	m_dwNPCID = dwNPCID;
}

DWORD SoarEquipFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	switch (pEvent->eEvent)
	{
	case EGUIE_Click:
		{
			if (pWnd == m_pBtnCancel) //取消
			{
				LockItem(FALSE);
				tagGameEvent event(_T("Close_SoarEquipFrame"), this);
				m_pMgr->SendEvent(&event);
				//m_pMgr->AddToDestroyList(this);

			} 
			else if (pWnd == m_pBtnConfirmMix) //确定炼器
			{
				m_bIfMix = EquipMixing();
				if ( m_bIfMix ) //炼器通过基本检测
				{
					//进度条
					m_pProMix->SetValue(1.0);
					m_bIfProgressing = TRUE;
					//炼器读条时不可操作项
					LockItem(TRUE);
					m_pBtnConfirmMix->SetEnable(FALSE);
				}
			}
		}
		break;
	case EGUIE_Drag:

		if ( m_bIfProgressing == TRUE )
		{  //读条过程中不能drag
			break;
		}
		if ( pWnd == (GUIWnd*)m_pBtnSoarEquip )
		{
			if ( m_pItemSoarEquip != NULL )
			{
				DragBtnInMixFrame(m_pBtnSoarEquip,m_pItemSoarEquip);
				m_pItemSoarEquip = NULL;
				m_iSoarEquipLevel = 0; //待飞升装备等级置0
				m_iSoarEquipQuality = 0;//待飞升装备品质置0
			}
		} 
		else if ( pWnd == (GUIWnd*)m_pBtnMixEquip )
		{
			if ( m_pItemMixEquip != NULL )
			{
				DragBtnInMixFrame(m_pBtnMixEquip,m_pItemMixEquip);
				m_pItemMixEquip = NULL;
				m_iEquipLevel = 0;    //飞升合成道具的装备等级置0
				m_iEquipQuality = 0;  //飞升合成道具装备品质置0
			}
		} 
		else if ( pWnd == (GUIWnd*)m_pBtnMixStuff )
		{
			if ( m_pItemMixStuff != NULL )
			{
				DragBtnInMixFrame(m_pBtnMixStuff,m_pItemMixStuff);
				m_pItemMixStuff = NULL;
			}
		}
		//UpdateUI();
		break;
	case EGUISCB_Drop:
		{
			if ( m_bIfProgressing == TRUE )
			{  //读条过程中不能Drop
				break;
			}
			if ( pWnd == (GUIWnd*)m_pBtnSoarEquip )
			{
				if( EDT_Item == (EDragType)pEvent->dwParam1 )
					TestSoarEquip((INT16)LOWORD(pEvent->dwParam2));
			} 
			else if ( pWnd == (GUIWnd*)m_pBtnMixEquip )
			{
				if( EDT_Item == (EDragType)pEvent->dwParam1 )
					TestMixEquip((INT16)LOWORD(pEvent->dwParam2));
			} 
			else if ( pWnd == (GUIWnd*)m_pBtnMixStuff )
			{
				if( EDT_Item == (EDragType)pEvent->dwParam1 )
					TestMixStuff((INT16)LOWORD(pEvent->dwParam2));
			} 
			//UpdateUI();
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			if ( pWnd == (GUIWnd*)m_pBtnSoarEquip )
			{
				 ShowItemTips( m_pBtnSoarEquip, m_pItemSoarEquip );
			} 
			else if ( pWnd == (GUIWnd*)m_pBtnMixEquip )
			{
				 ShowItemTips( m_pBtnMixEquip, m_pItemMixEquip );
			} 
			else if ( pWnd == (GUIWnd*)m_pBtnMixStuff )
			{
				ShowItemTips( m_pBtnMixStuff, m_pItemMixStuff );
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

void SoarEquipFrame::LockItem( bool bOperable )
{
	if( P_VALID(m_pItemSoarEquip) )
		m_pItemSoarEquip->SetOperable(!bOperable);
	if( P_VALID(m_pItemMixEquip) )
		m_pItemMixEquip->SetOperable(!bOperable);
	if( P_VALID(m_pItemMixStuff) )
		m_pItemMixStuff->SetOperable(!bOperable);
}

DWORD SoarEquipFrame::NetItemRemove(tagNS_ItemRemove* pMsg, DWORD pPrama)
{
	if(pMsg->eConType != EICT_Bag)
		return 0;

	UpdateItem(m_pItemMixEquip);
	UpdateItem(m_pItemMixStuff);
	UpdateItem(m_pItemSoarEquip);
	RefreshBtnUI();
	return 0;
}

VOID SoarEquipFrame::UpdateItem(Item *pItem)
{
	if(P_VALID(pItem))
	{
		INT64 n64ID = pItem->GetData()->n64Serial;
		Item *pTempItem = ItemMgr::Inst()->GetPocketItemByID(n64ID);
		if(!P_VALID(pTempItem))
			pItem = NULL;
	}
}

BOOL SoarEquipFrame::EscCancel()
{
	LockItem(FALSE);
	m_pGameFrameMgr->AddToDestroyList(this);
	return TRUE;
}

VOID  SoarEquipFrame::ShowItemTips(GUIWnd* pWnd, Item* pTempItem)
{
	Item* pItem = pTempItem;
	if(P_VALID(pItem))
	{
		tstring strPrice = _T("");
		
		tagItemCost cost(pItem->IsVendible() ? pItem->GetItemPrice() : GT_INVALID, ECCT_Null, 0, 0, GT_INVALID);
		strPrice = ToolTipCreator::Inst()->GetPriceTips(&cost);
		ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pItem).c_str(), strPrice.c_str());

		// 如果是装备显示装备比较
		if(MIsEquipment(pItem->GetItemTypeID()))
		{
			ToolTipCreator::Inst()->ShowEquipCompare(pItem->GetItemTypeID());
		}
	}
}


DWORD SoarEquipFrame::PocketEquipDrop( tagEquipDropEvent* pGameEvent)
{
	//ItemFrame* pItemFrame = (ItemFrame*)(pGameEvent->pSender);
	INT64  nTempItemID = pGameEvent->n64ItemID;
 	
	
	if ( m_pItemSoarEquip!=NULL )
	{
		if ( m_pItemSoarEquip->GetItemId() == nTempItemID )
		{
			m_pItemSoarEquip = NULL;
		}
	}
	else if ( m_pItemMixEquip!=NULL )
	{
		if ( m_pItemMixEquip->GetItemId() == nTempItemID )
		{
			m_pItemMixEquip = NULL;
		}
	}
	else if ( m_pItemMixStuff!=NULL )
	{
		if ( m_pItemMixStuff->GetItemId() == nTempItemID )
		{
			m_pItemMixStuff = NULL;
		}
	}

	return 0;
}