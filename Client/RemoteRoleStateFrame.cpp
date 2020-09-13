#include "stdafx.h"
#include "RemoteRoleStateFrame.h"
#include "TargetCharacterFrame.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"
#include "ItemButton.h"
#include "PlayerViewWindow.h"
#include "Item.h"
#include "ItemMgr.h"
#include "Role.h"
#include "Player.h"
#include "RoleMgr.h"
#include "..\WorldDefine\role_att.h"
#include "BlinkStatic.h"
#include "SaveAccount.h"

extern const TCHAR* BlinkPic;

RemoteRoleStateFrame::RemoteRoleStateFrame( void ):m_Trunk(this)
{
	m_pWnd = NULL;
	m_pWndFather = NULL;
	for (INT i=0; i<EEP_End; ++i)
	{
		m_pStcEquipBack[i] = NULL;
		m_pBtnEquip[i] = NULL;
	}
	m_bIsFashionDisplay = FALSE;
	m_dwTime = GT_INVALID;
	m_dwTimer = 0;
}

RemoteRoleStateFrame::~RemoteRoleStateFrame( void )
{

}

BOOL RemoteRoleStateFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam);

	//注册网络消息
	m_pCmdMgr->Register("NS_GetRemoteRoleEquipInfo",			(NETMSGPROC)m_Trunk.sfp2(&RemoteRoleStateFrame::OnNS_GetRemoteRoleEquipInfo),    _T("NS_GetRemoteRoleEquipInfo"));

	//注册游戏事件
	m_pMgr->RegisterEventHandle(_T("DeleRemoteRoleEquipInfo"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&RemoteRoleStateFrame::OnDeleRemoteRoleEquipInfo));

	return bRet;
}

BOOL RemoteRoleStateFrame::Destroy()
{
	BOOL bRet = GameFrame::Destroy();

	//注销网络消息
	m_pCmdMgr->UnRegister("NS_GetRemoteRoleEquipInfo",			(NETMSGPROC)m_Trunk.sfp2(&RemoteRoleStateFrame::OnNS_GetRemoteRoleEquipInfo));

	//注销游戏事件
	m_pMgr->UnRegisterEventHandler(_T("DeleRemoteRoleEquipInfo"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&RemoteRoleStateFrame::OnDeleRemoteRoleEquipInfo));

	return bRet;
}

BOOL RemoteRoleStateFrame::ReloadUI()
{
	// 创建
	m_pWndFather = m_pGUI->GetWnd(((TargetCharacterFrame*)m_pFather)->GetGUIFullName().c_str());
	m_pWnd = m_pWndFather->GetChild(_T("ra_pic\\rstatepic\\equipback"));

	// 注册事件处理函数
	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&RemoteRoleStateFrame::EventHandler));

	// 获取控件
	// 装备预览
	m_pWndEquipView = (PlayerViewWindow*)m_pWnd->GetChild(_T("role"));

	for (int i = 0; i < EEP_End; i++)
	{
		m_pBlkEquip[i] = 0;
	}


	// 头部
	m_pStcEquipBack[EEP_Head] = (GUIStatic*)m_pWnd->GetChild(_T("hat"));
	m_pBtnEquip[EEP_Head] = (ItemButton*)m_pStcEquipBack[EEP_Head]->GetChild(_T("hatpic"));
	m_pBlkEquip[EEP_Head] = (BlinkStatic*)m_pBtnEquip[EEP_Head]->GetChild(_T("pic"));
	// 项链
	m_pStcEquipBack[EEP_Neck] = (GUIStatic*)m_pWnd->GetChild(_T("necklace"));
	m_pBtnEquip[EEP_Neck] = (ItemButton*)m_pStcEquipBack[EEP_Neck]->GetChild(_T("necklacepic"));
	m_pBlkEquip[EEP_Neck] = (BlinkStatic*)m_pBtnEquip[EEP_Neck]->GetChild(_T("pic"));
	// 上身
	m_pStcEquipBack[EEP_Body] = (GUIStatic*)m_pWnd->GetChild(_T("upper"));
	m_pBtnEquip[EEP_Body] = (ItemButton*)m_pStcEquipBack[EEP_Body]->GetChild(_T("upperpic"));
	m_pBlkEquip[EEP_Body] = (BlinkStatic*)m_pBtnEquip[EEP_Body]->GetChild(_T("pic"));
	// 下身
	m_pStcEquipBack[EEP_Legs] = (GUIStatic*)m_pWnd->GetChild(_T("lower"));
	m_pBtnEquip[EEP_Legs] = (ItemButton*)m_pStcEquipBack[EEP_Legs]->GetChild(_T("lowerpic"));
	m_pBlkEquip[EEP_Legs] = (BlinkStatic*)m_pBtnEquip[EEP_Legs]->GetChild(_T("pic"));
	// 背部
	m_pStcEquipBack[EEP_Back] = (GUIStatic*)m_pWnd->GetChild(_T("mantle"));
	m_pBtnEquip[EEP_Back] = (ItemButton*)m_pStcEquipBack[EEP_Back]->GetChild(_T("mantlepic"));
	m_pBlkEquip[EEP_Back] = (BlinkStatic*)m_pBtnEquip[EEP_Back]->GetChild(_T("pic"));
	// 手部
	m_pStcEquipBack[EEP_Wrist] = (GUIStatic*)m_pWnd->GetChild(_T("glove"));
	m_pBtnEquip[EEP_Wrist] = (ItemButton*)m_pStcEquipBack[EEP_Wrist]->GetChild(_T("glovepic"));
	m_pBlkEquip[EEP_Wrist] = (BlinkStatic*)m_pBtnEquip[EEP_Wrist]->GetChild(_T("pic"));
	// 脚部
	m_pStcEquipBack[EEP_Feet] = (GUIStatic*)m_pWnd->GetChild(_T("boot"));
	m_pBtnEquip[EEP_Feet] = (ItemButton*)m_pStcEquipBack[EEP_Feet]->GetChild(_T("bootpic"));
	m_pBlkEquip[EEP_Feet] = (BlinkStatic*)m_pBtnEquip[EEP_Feet]->GetChild(_T("pic"));
	// 戒指
	m_pStcEquipBack[EEP_Finger1] = (GUIStatic*)m_pWnd->GetChild(_T("ring1"));
	m_pBtnEquip[EEP_Finger1] = (ItemButton*)m_pStcEquipBack[EEP_Finger1]->GetChild(_T("ring1pic"));
	m_pBlkEquip[EEP_Finger1] = (BlinkStatic*)m_pBtnEquip[EEP_Finger1]->GetChild(_T("pic"));
	// 戒指
	m_pStcEquipBack[EEP_Finger2] = (GUIStatic*)m_pWnd->GetChild(_T("ring2"));
	m_pBtnEquip[EEP_Finger2] = (ItemButton*)m_pStcEquipBack[EEP_Finger2]->GetChild(_T("ring2pic"));
	m_pBlkEquip[EEP_Finger2] = (BlinkStatic*)m_pBtnEquip[EEP_Finger2]->GetChild(_T("pic"));
	// 腰饰
	m_pStcEquipBack[EEP_Waist] = (GUIStatic*)m_pWnd->GetChild(_T("sash"));
	m_pBtnEquip[EEP_Waist] = (ItemButton*)m_pStcEquipBack[EEP_Waist]->GetChild(_T("sashpic"));
	m_pBlkEquip[EEP_Waist] = (BlinkStatic*)m_pBtnEquip[EEP_Waist]->GetChild(_T("pic"));
	// 主手武器
	m_pStcEquipBack[EEP_RightHand] = (GUIStatic*)m_pWnd->GetChild(_T("mainwp"));
	m_pBtnEquip[EEP_RightHand] = (ItemButton*)m_pStcEquipBack[EEP_RightHand]->GetChild(_T("mainwppic"));
	m_pBlkEquip[EEP_RightHand] = (BlinkStatic*)m_pBtnEquip[EEP_RightHand]->GetChild(_T("pic"));
	// 副手武器
	m_pStcEquipBack[EEP_LeftHand] = (GUIStatic*)m_pWnd->GetChild(_T("minorwp"));
	m_pBtnEquip[EEP_LeftHand] = (ItemButton*)m_pStcEquipBack[EEP_LeftHand]->GetChild(_T("minorwppic"));
	m_pBlkEquip[EEP_LeftHand] = (BlinkStatic*)m_pBtnEquip[EEP_LeftHand]->GetChild(_T("pic"));

	// 魂晶
	m_pStcEquipBack[EEP_HunJing] = (GUIStatic*)m_pWnd->GetChild(_T("minorwp1"));
	m_pBtnEquip[EEP_HunJing] = (ItemButton*)m_pStcEquipBack[EEP_HunJing]->GetChild(_T("minorwppic"));
	m_pBlkEquip[EEP_HunJing] = (BlinkStatic*)m_pBtnEquip[EEP_HunJing]->GetChild(_T("pic"));

	// 法宝
	m_pStcEquipBack[EEP_Face] = (GUIStatic*)m_pWnd->GetChild(_T("talisman"));
	m_pBtnEquip[EEP_Face] = (ItemButton*)m_pStcEquipBack[EEP_Face]->GetChild(_T("talismanpic"));
	m_pBlkEquipFb = (BlinkStatic*)m_pBtnEquip[EEP_Face]->GetChild(_T("pic"));

	//圣灵
 	m_pStcEquipBack[EEP_Holy] = (GUIStatic*)m_pWnd->GetChild(_T("sl"));
 	m_pBtnEquip[EEP_Holy] = (ItemButton*)m_pStcEquipBack[EEP_Holy]->GetChild(_T("talismanpic"));
// 	m_pBlkEquip[EEP_Holy] = (BlinkStatic*)m_pBtnEquip[EEP_Holy]->GetChild(_T("pic"));

	// 时装头
	m_pStcEquipBack[EEP_FashionHead] = (GUIStatic*)m_pWnd->GetChild(_T("fashionhat"));
	m_pBtnEquip[EEP_FashionHead] = (ItemButton*)m_pStcEquipBack[EEP_FashionHead]->GetChild(_T("hatpic"));
	m_pBtnEquip[EEP_FashionHead]->SetInvisible(TRUE);
	m_pStcEquipBack[EEP_FashionHead]->SetInvisible(TRUE);
	// 时装上身
	m_pStcEquipBack[EEP_FashionBody] = (GUIStatic*)m_pWnd->GetChild(_T("fashioncloth"));
	m_pBtnEquip[EEP_FashionBody] = (ItemButton*)m_pStcEquipBack[EEP_FashionBody]->GetChild(_T("clothpic"));
	m_pBtnEquip[EEP_FashionBody]->SetInvisible(TRUE);
	m_pStcEquipBack[EEP_FashionBody]->SetInvisible(TRUE);
	// 时装背部
	m_pStcEquipBack[EEP_FashionBack] = (GUIStatic*)m_pWnd->GetChild(_T("fashionback"));
	m_pBtnEquip[EEP_FashionBack] = (ItemButton*)m_pStcEquipBack[EEP_FashionBack]->GetChild(_T("backpic"));
	m_pBtnEquip[EEP_FashionBack]->SetInvisible(TRUE);
	m_pStcEquipBack[EEP_FashionBack]->SetInvisible(TRUE);
	// 时装手部
	m_pStcEquipBack[EEP_FashionWrist] = (GUIStatic*)m_pWnd->GetChild(_T("fashionglove"));
	m_pBtnEquip[EEP_FashionWrist] = (ItemButton*)m_pStcEquipBack[EEP_FashionWrist]->GetChild(_T("glovepic"));
	m_pBtnEquip[EEP_FashionWrist]->SetInvisible(TRUE);
	m_pStcEquipBack[EEP_FashionWrist]->SetInvisible(TRUE);
	// 时装足部
	m_pStcEquipBack[EEP_FashionFeet] = (GUIStatic*)m_pWnd->GetChild(_T("fashionboot"));
	m_pBtnEquip[EEP_FashionFeet] = (ItemButton*)m_pStcEquipBack[EEP_FashionFeet]->GetChild(_T("bootpic"));
	m_pBtnEquip[EEP_FashionFeet]->SetInvisible(TRUE);
	m_pStcEquipBack[EEP_FashionFeet]->SetInvisible(TRUE);


	{
		m_pStcSilverVIP = (GUIStatic*)m_pWnd->GetChild(_T("role\\silvervip"));
		m_pStcGoldVIP = (GUIStatic*)m_pWnd->GetChild(_T("role\\goldvip"));
		m_pStcPlatinumVIP = (GUIStatic*)m_pWnd->GetChild(_T("role\\supervip"));
		
	}

	for (int i = 0; i < EEP_End; i++)
	{
		if (P_VALID(m_pBlkEquip[i] ))
		{
			m_pBlkEquip[i]->SetPic(BlinkPic);
			m_pBlkEquip[i]->SetLoopNum(16);
			m_pBlkEquip[i]->SetSize(44,44);
			m_pBlkEquip[i]->SetStartPoint(317,57);
		}

	}
	m_pBlkEquipFb->SetPic(BlinkPic);
	m_pBlkEquipFb->SetLoopNum(16);
	m_pBlkEquipFb->SetSize(44,44);
	m_pBlkEquipFb->SetStartPoint(317,57);
	
	return TRUE;
}

VOID RemoteRoleStateFrame::Render3D()
{
	if( P_VALID( m_pWndEquipView ) )
		m_pWndEquipView->Render3D();
	GameFrame::Render3D();
}

DWORD RemoteRoleStateFrame::EventHandler( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

	switch (pEvent->eEvent)
	{
	case EGUIE_MoveIntoCtrl:				// 鼠标移入该控件	
		{
			for(INT i = 0; i < EEP_MaxEquip; ++i)
			{
				if(pWnd == m_pBtnEquip[i] || pWnd ==m_pBlkEquip[i] || pWnd == m_pBlkEquipFb)
				{
					Equipment* pEquip = NULL;
					/*if (!m_bIsFashionDisplay || i <= EEP_Feet || i == EEP_RightHand || i == EEP_LeftHand)*/
					if(pWnd != m_pBlkEquipFb)
						pEquip = GetCurRemoteRoleEquip(GetEquipPos(i));
					else
						pEquip = GetCurRemoteRoleEquip(EEP_Face);	//获取法宝

					if(P_VALID(pEquip))
					{
						ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pEquip).c_str());
						ToolTipCreator::Inst()->ShowEquipCompare(pEquip->GetItemTypeID());
					}
					else
					{
						// 没有装备则显示装备部位
						TCHAR szTmp[X_SHORT_NAME];
						_sntprintf(szTmp, sizeof(szTmp)/sizeof(TCHAR), _T("EquipPos%d"), GetEquipPos(i)+1);
						tstring strTmp = g_StrTable[_T("TipsDefaultFont")];
						strTmp += g_StrTable[szTmp];
						ShowTip(pWnd, strTmp.c_str());
					}
					break;
				}
			}
		}
		break;
	case EGUIE_MoveOutOfCtrl:				// 鼠标移出该控件
		{
			ShowTip(pWnd, NULL);
		}
		break;
	default:
		return 0;
		break;
	}

	return 0;
}

DWORD RemoteRoleStateFrame::OnDeleRemoteRoleEquipInfo(tagGameEvent* pGameEvent)
{
	m_mapRemoteRoleEquip.clear();
	UpdateAllEquip();
	return 0;
}

DWORD RemoteRoleStateFrame::OnNS_GetRemoteRoleEquipInfo(tagNS_GetRemoteRoleEquipInfo* pMsg, DWORD dwParam)
{
	if( pMsg->dwErrorCode != E_Success && (SaveAccount::Inst()->GetLoginMode() == 7 || SaveAccount::Inst()->GetLoginMode() == 0) )	//台湾登陆模式才会报错
	{
		TCHAR szTmp[X_SHORT_NAME];
		_sntprintf(szTmp,sizeof(szTmp)/sizeof(TCHAR), _T("ShowRoleInfo_Code%d"), pMsg->dwErrorCode);
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[szTmp]);
		return 0;
	}
	else if( pMsg->dwRoleID == m_dwRemoteRoleID )
	{
		m_mapRemoteRoleEquip.clear();
		BYTE* pBytes = pMsg->byEquip;
		for( int i=0; i<pMsg->nEquipNum; ++i )
		{	

			if(*pBytes == EIT_FaBao)
			{
				tagFabao* equip = reinterpret_cast<tagFabao*>(++pBytes);
				if(equip->eConType == EICT_Equip)
				{		
					Equipment equipex(*equip);
					m_mapRemoteRoleEquip.insert(make_pair( equipex.GetPos(), equipex ));
				}
				pBytes += sizeof(tagFabao);
			}
			else if (*pBytes == EIT_SoulCrystal)
			{
				tagSoulCrystal* equip = reinterpret_cast<tagSoulCrystal*>(++pBytes);
				if(equip->eConType == EICT_Equip)
				{		
					Equipment equipex(*equip);
					m_mapRemoteRoleEquip.insert(make_pair( equipex.GetPos(), equipex ));
				}
				pBytes += sizeof(tagSoulCrystal);
			}
			else if (*pBytes == EIT_Holy)
			{
				tagHolyMan* equip = reinterpret_cast<tagHolyMan*>(++pBytes);
				if(equip->eConType == EICT_Equip)
				{
					Equipment equipex(*equip);
					m_mapRemoteRoleEquip.insert(make_pair(equipex.GetPos(), equipex));
				}
				pBytes += sizeof(tagHolyMan);
			}
			else if (*pBytes == EIT_HolyEquip)
			{
				tagHolyEquip* equip = reinterpret_cast<tagHolyEquip*>(++pBytes);
				if(equip->eConType == EICT_Equip)
				{
					Equipment equipex(*equip);
					m_mapRemoteRoleEquip.insert(make_pair(equipex.GetPos(), equipex));
				}
				pBytes += sizeof(tagHolyEquip);
			}
			else
			{
				tagEquip* equip = reinterpret_cast<tagEquip*>(++pBytes);
				if(equip->eConType == EICT_Equip)
				{		
					Equipment equipex(*equip);
					m_mapRemoteRoleEquip.insert(make_pair( equipex.GetPos(), equipex ));
				}
				pBytes += sizeof(tagEquip);
			}
		}				

		UpdateAllEquip();
	}
	return 0;
}

EEquipPos RemoteRoleStateFrame::GetEquipPos( INT nIndex )
{
	EEquipPos eRet = EEP_End;
	if(m_bIsFashionDisplay)
	{
		if(nIndex < EEP_End - EEP_MaxEquip)
		{
			eRet = (EEquipPos)(nIndex + EEP_MaxEquip);
		}
		else
		{
			eRet = (EEquipPos)nIndex;
		}
	}
	else
	{
		eRet = (EEquipPos)nIndex;
	}
	return eRet;
}

Equipment* RemoteRoleStateFrame::GetCurRemoteRoleEquip( EEquipPos pos/*=EEP_Null */ )
{
	map<INT16, Equipment>::iterator it = m_mapRemoteRoleEquip.find(pos);

	if( it!=m_mapRemoteRoleEquip.end() )
		return &it->second;

	return NULL;
}

void RemoteRoleStateFrame::UpdateAllEquip()
{
	for (INT i=0; i<EEP_MaxEquip; ++i)
	{
		UpdateEquipment((EEquipPos)i);
	}

	map<INT16, Equipment>::iterator it = m_mapRemoteRoleEquip.begin();
	for( ; it!=m_mapRemoteRoleEquip.end(); ++it )
	{
		INT16 n16Pos = it->first;
		if( n16Pos >= EEP_MaxEquip  )
			continue;
		BYTE byQuality = it->second.GetItemQuality();
		DWORD dwTypeID = it->second.GetItemTypeID();
		m_pBtnEquip[n16Pos]->RefreshItem( dwTypeID, 0, byQuality );
	}
}

void RemoteRoleStateFrame::UpdateEquipment( EEquipPos ePos )
{
	DWORD dwTypeID = GT_INVALID;
	BYTE byQuality = 0;
	m_pBtnEquip[ePos]->RefreshItem( dwTypeID, 0, byQuality );
}

void RemoteRoleStateFrame::UpdateEquipAvatar()
{
	Role* pRole = RoleMgr::Inst()->FindRole( m_dwRemoteRoleID );
	if( P_VALID( pRole ) && P_VALID( m_pWndEquipView ))
	{
		Player* pPlayer = dynamic_cast<Player*>(pRole);
		if( P_VALID(pPlayer) )
		{
			m_pWndEquipView->SetInvisible(false);
			m_pWndEquipView->UpdataAvatar( pPlayer );

			INT vipLevel = pPlayer->GetAttribute(ERA_VipLevel);
			m_pStcSilverVIP->SetInvisible(TRUE);
			m_pStcGoldVIP->SetInvisible(TRUE);
			m_pStcPlatinumVIP->SetInvisible(TRUE);
			if (vipLevel == 0)
			{
			}
			else if (vipLevel == 1)
			{
				m_pStcSilverVIP->SetInvisible(FALSE);
			}
			else if (vipLevel == 2)
			{
				m_pStcGoldVIP->SetInvisible(FALSE);
			}
			else if (vipLevel == 3)
			{
				m_pStcPlatinumVIP->SetInvisible(FALSE);
			}
		}
	}
	if (m_dwRemoteRoleID == GT_INVALID && P_VALID(m_pWndEquipView))
	{
		m_pWndEquipView->SetInvisible(true);
	}
	if (!P_VALID(pRole) && P_VALID(m_pWndEquipView))
	{
		m_pWndEquipView->SetInvisible(true);
	}
}

void RemoteRoleStateFrame::UpdataBlick()
{
	for(INT i=0; i<EEP_MaxEquip; ++i)
	{
		if ( i == EEP_FashionFace || i == EEP_FashionLegs)
			continue;

		Equipment* pEquip = GetCurRemoteRoleEquip(GetEquipPos(i));
		if(P_VALID(pEquip) && pEquip->GetEquipStrengthLevel() >= 12 )
		{
			if (P_VALID(m_pBlkEquip[i]))
			{
				if(20 <= pEquip->GetEquipStrengthLevel())
					m_pBlkEquip[i]->SetStartPoint(284,244);
				else
					m_pBlkEquip[i]->SetStartPoint(284,56);

				m_pBlkEquip[i]->StartBlink();
			}
		}
	}

	Equipment* pEquip = GetCurRemoteRoleEquip(GetEquipPos(EEP_Face));
	if(P_VALID(pEquip) )
	{
		const tagFabaoSpec *pTrumpSpe = pEquip->GetFaBaoSpe();
		if (P_VALID(pTrumpSpe) && pTrumpSpe->n16NativeIntelligence >= 80)
		{
			if(pTrumpSpe->n16NativeIntelligence > 100)
				m_pBlkEquipFb->SetStartPoint(284,244);
			else if (pTrumpSpe->n16NativeIntelligence >= 80)
				m_pBlkEquipFb->SetStartPoint(284,56);

			m_pBlkEquipFb->StartBlink();
		}
	}
	
}

VOID RemoteRoleStateFrame::Update()
{
	m_dwTimer += Kernel::Inst()->GetDeltaTimeDW();
	if(m_dwTimer > 3000)
	{
		UpdataBlick();
		m_dwTimer = 0;
	}
}