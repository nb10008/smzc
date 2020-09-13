#include "StdAfx.h"
#include "Item.h"
#include "MallMgr.h"
#include "ItemMgr.h"
#include "RoleMgr.h"
#include "ItemButton.h"
#include "PicStatic.h"
#include "LocalPlayer.h"
#include "ClothespressFrame.h"
#include "PlayerTryDressWnd.h"
#include "..\WorldDefine\msg_fashion.h"
#include "ClothespressData.h"
#include "SkillProtoData.h"
#include "RoleEvent.h"
#include "Container.h"
#include "ItemFrame.h"
#include "ItemEvent.h"
#include "ToolTipCreator.h"
#include "ToolTipFrame.h"

const TCHAR* szPicInit = _T("data\\ui\\common\\L_diban-b.bmp");
const TCHAR *szPicSele = _T("data\\ui\\Common\\L_icon-l2.bmp");
const INT MAX_LEVEL_CLOTHESPRESS  = 3;

ClothespressFrame::ClothespressFrame(void):
m_Trunk(this),
m_pWnd(NULL),
m_n16CurPage(0),
m_dwCurSuitIndex(0),
m_pWndTryDress(NULL)
{
	for( int i = 0; i < FASHION_SUIT_NUM_OF_ONE_PAGE; i++ )
	{
		for( int j = 0; j < EF_End; j++ )
		{
			m_pFashion[i][j] = NULL;
		}
	}
	for( int i = 0; i < FASHION_SUIT_NUM_OF_ONE_PAGE; i++ )
	{
		m_pStcClothesexpressLevel[i] = NULL;
		m_pStcSuitName[i] = NULL;
		m_pStcAttrativeValue[i] = NULL;
		m_pStcSuitTips[i] = NULL;
		m_pStcSuitState[i] = NULL;
		m_pDiyFashion[i] = NULL;
	}
}

ClothespressFrame::~ClothespressFrame()
{

}

BOOL ClothespressFrame::Init( GameFrameMgr *pMgr, GameFrame *pFather, DWORD dwParam )
{
	if( !GameFrame::Init( pMgr, pFather, dwParam) )
		return FALSE;

	m_pMgr->RegisterEventHandle(_T("ItemEvent_UpdateItem"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&ClothespressFrame::OnUpdateItemEvent));
	m_pMgr->RegisterEventHandle(_T("Close_ClothesPress"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&ClothespressFrame::OnEventHideUI));
	m_pMgr->RegisterEventHandle(_T("tagUpdateFashionInfo"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&ClothespressFrame::OnEventUpdateFashionInfo));
	m_pMgr->RegisterEventHandle(_T("LocalPlayerTransform"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&ClothespressFrame::OnEventTransform));
	m_pMgr->RegisterEventHandle(_T("LocalPlayerUnTransform"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&ClothespressFrame::OnEventTransform));
	m_pMgr->RegisterEventHandle(_T("OffFashionClothes"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&ClothespressFrame::OnOffFashionClothes));
	m_pCmdMgr->Register("NS_WardrobeLevelUp",					(NETMSGPROC)m_Trunk.sfp2(&ClothespressFrame::OnNS_WardrobeLevelUp),		_T("NS_WardrobeLevelUp") );
	m_pCmdMgr->Register("NS_Rehanding",						(NETMSGPROC)m_Trunk.sfp2(&ClothespressFrame::OnNS_NetRehanding),		_T("NS_NetRehanding") );
	return TRUE;
}

BOOL ClothespressFrame::Destroy()
{
	GameFrame::Destroy();
	if(P_VALID(m_pWnd))
		m_pGUI->AddToDestroyList(m_pWnd);

	m_pMgr->UnRegisterEventHandler(_T("ItemEvent_UpdateItem"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&ClothespressFrame::OnUpdateItemEvent));
	m_pMgr->UnRegisterEventHandler(_T("Close_ClothesPress"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&ClothespressFrame::OnEventHideUI));
	m_pMgr->UnRegisterEventHandler(_T("tagUpdateFashionInfo"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&ClothespressFrame::OnEventUpdateFashionInfo));
	m_pMgr->UnRegisterEventHandler(_T("LocalPlayerTransform"),	(FRAMEEVENTPROC)m_Trunk.sfp1(&ClothespressFrame::OnEventTransform));
	m_pMgr->UnRegisterEventHandler(_T("LocalPlayerUnTransform"),(FRAMEEVENTPROC)m_Trunk.sfp1(&ClothespressFrame::OnEventTransform));
	m_pMgr->UnRegisterEventHandler(_T("OffFashionClothes"),		(FRAMEEVENTPROC)m_Trunk.sfp1(&ClothespressFrame::OnOffFashionClothes));
	m_pCmdMgr->UnRegister("NS_WardrobeLevelUp",					(NETMSGPROC)m_Trunk.sfp2(&ClothespressFrame::OnNS_WardrobeLevelUp) );
	m_pCmdMgr->UnRegister("NS_Rehanding",					(NETMSGPROC)m_Trunk.sfp2(&ClothespressFrame::OnNS_NetRehanding));
	return TRUE;
}
BOOL ClothespressFrame::EscCancel()
{
	if(m_pWnd->IsInvisible() )
		return FALSE;

	m_pWnd->SetInvisible(TRUE);
	m_n16CurPage = 0;
	m_dwCurSuitIndex = 0;
	SetStrategy(EIST_ItemPocket);
	return TRUE;
}

BOOL ClothespressFrame::ReloadUI()
{
	XmlElement e;
	tstring strPath = vEngine::g_strLocalPath + _T("\\ui\\clothshow.xml");
	if( !m_pGUI->LoadXml( &e, "VFS_System", strPath.c_str() ) )
		return FALSE;

	CHAR szTmp[X_LONG_NAME] = {0};
	for( int i = 0; i < FASHION_SUIT_NUM_OF_ONE_PAGE; i++ )
	{
		_snprintf(szTmp, sizeof(szTmp), "clothespress\\clothesback\\clothback\\cloth%d" ,i);
		m_pGUI->ChangeXml(&e, szTmp, "ClassName", "PicStatic");

		_snprintf(szTmp, sizeof(szTmp), "clothespress\\clothesback\\clothback\\cloth%d\\title" ,i);
		m_pGUI->ChangeXml(&e, szTmp, "ClassName", "PicStatic");

		_snprintf(szTmp, sizeof(szTmp), "clothespress\\clothesback\\clothback\\cloth%d\\title\\level" ,i);
		m_pGUI->ChangeXml(&e, szTmp, "ClassName", "PicStatic");

		_snprintf(szTmp, sizeof(szTmp), "clothespress\\clothesback\\clothback\\cloth%d\\title\\name" ,i);
		m_pGUI->ChangeXml(&e, szTmp, "ClassName", "PicStatic");

		_snprintf(szTmp, sizeof(szTmp), "clothespress\\clothesback\\clothback\\cloth%d\\fashionhat\\icon" ,i);
		m_pGUI->ChangeXml(&e, szTmp, "ClassName", "ItemButton");
		_snprintf(szTmp, sizeof(szTmp), "clothespress\\clothesback\\clothback\\cloth%d\\fashioncloth\\icon" ,i);
		m_pGUI->ChangeXml(&e, szTmp, "ClassName", "ItemButton");
		_snprintf(szTmp, sizeof(szTmp), "clothespress\\clothesback\\clothback\\cloth%d\\fashionglove\\icon" ,i);
		m_pGUI->ChangeXml(&e, szTmp, "ClassName", "ItemButton");
		_snprintf(szTmp, sizeof(szTmp), "clothespress\\clothesback\\clothback\\cloth%d\\fashionboot\\icon" ,i);
		m_pGUI->ChangeXml(&e, szTmp, "ClassName", "ItemButton");
		_snprintf(szTmp, sizeof(szTmp), "clothespress\\clothesback\\clothback\\cloth%d\\fashionback\\icon" ,i);
		m_pGUI->ChangeXml(&e, szTmp, "ClassName", "ItemButton");
	}

	m_pGUI->ChangeXml(&e, "clothespress\\clothesback\\clothback\\diycloth", "ClassName", "PicStatic");
	m_pGUI->ChangeXml(&e, "clothespress\\clothesback\\clothback\\diycloth\\title", "ClassName", "PicStatic");
	m_pGUI->ChangeXml(&e, "clothespress\\clothesback\\clothback\\diycloth\\title\\level", "ClassName", "PicStatic");
	m_pGUI->ChangeXml(&e, "clothespress\\clothesback\\clothback\\diycloth\\title\\name", "ClassName", "PicStatic");

	m_pGUI->ChangeXml(&e, "clothespress\\clothesback\\clothback\\diycloth\\fashionhat\\icon", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&e, "clothespress\\clothesback\\clothback\\diycloth\\fashioncloth\\icon", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&e, "clothespress\\clothesback\\clothback\\diycloth\\fashionglove\\icon", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&e, "clothespress\\clothesback\\clothback\\diycloth\\fashionboot\\icon", "ClassName", "ItemButton");
	m_pGUI->ChangeXml(&e, "clothespress\\clothesback\\clothback\\diycloth\\fashionback\\icon", "ClassName", "ItemButton");

	m_pGUI->ChangeXml(&e, "clothespress\\clothesback\\showback\\fittingroom", "ClassName", "PlayerTryDressWnd");

	m_pWnd = m_pGUI->CreateWnd( _T("\\desktop"), &e );
	if(!P_VALID(m_pWnd))
		return FALSE;

	m_pWndBack = (GUIPatch*)m_pWnd->GetChild(_T("clothesback"));
	m_pBtnMale = (GUIPushButton*)m_pWndBack->GetChild(_T("typechoose\\male"));
	m_pBtnMale->SetState(EGUIBS_PushDown);
	m_pBtnFemale = (GUIPushButton*)m_pWndBack->GetChild(_T("typechoose\\female"));
	TCHAR SzTemp[X_LONG_NAME] = {0};
	for( int i = 0; i < FASHION_SUIT_NUM_OF_ONE_PAGE; i++ )
	{
		_sntprintf(SzTemp, X_LONG_NAME, _T("clothback\\cloth%d") ,i);
		m_pIconSuit[i] = (PicStatic*)m_pWndBack->GetChild(SzTemp);

		_sntprintf(SzTemp , X_LONG_NAME, _T("clothback\\cloth%d\\title"), i);
		m_pStcTitle[i] = (PicStatic*)m_pWndBack->GetChild(SzTemp);

		_sntprintf(SzTemp , X_LONG_NAME, _T("clothback\\cloth%d\\title\\level"), i);
		m_pStcClothesexpressLevel[i] = (PicStatic*)m_pWndBack->GetChild(SzTemp);

		_sntprintf(SzTemp , X_LONG_NAME, _T("clothback\\cloth%d\\title\\name"), i);
		m_pStcSuitName[i] = (PicStatic*)m_pWndBack->GetChild(SzTemp);

		_sntprintf(SzTemp , X_LONG_NAME, _T("clothback\\cloth%d\\title\\wearingpic"), i);
		m_pStcWear[i] = (GUIStatic*)m_pWndBack->GetChild(SzTemp);
		m_pStcWear[i]->SetInvisible(TRUE);

		_sntprintf(SzTemp , X_LONG_NAME, _T("clothback\\cloth%d\\title\\friendpic"), i);
		m_pStcFriend[i] = (GUIStatic*)m_pWndBack->GetChild(SzTemp);
		m_pStcFriend[i]->SetInvisible(TRUE);

		_sntprintf(SzTemp , X_LONG_NAME, _T("clothback\\cloth%d\\word\\num"), i);
		m_pStcAttrativeValue[i] = (GUIStaticEx*)m_pWndBack->GetChild(SzTemp);

		_sntprintf(SzTemp , X_LONG_NAME, _T("clothback\\cloth%d\\wordback0\\word"), i);
		m_pStcSuitTips[i] = (GUIStaticEx*)m_pWndBack->GetChild(SzTemp);

		_sntprintf(SzTemp , X_LONG_NAME, _T("clothback\\cloth%d\\wordback1\\word"), i);
		m_pStcSuitState[i] = (GUIStaticEx*)m_pWndBack->GetChild(SzTemp);

		_sntprintf(SzTemp, sizeof(SzTemp), _T("clothback\\cloth%d\\fashionhat\\icon"),i);
		m_pFashion[i][EF_Hat] = (ItemButton*)m_pWndBack->GetChild(SzTemp);
		m_pFashion[i][EF_Hat]->RefreshItem();

		_sntprintf(SzTemp, sizeof(SzTemp), _T("clothback\\cloth%d\\fashioncloth\\icon"),i);
		m_pFashion[i][EF_Clothes] = (ItemButton*)m_pWndBack->GetChild(SzTemp);
		m_pFashion[i][EF_Clothes]->RefreshItem();

		_sntprintf(SzTemp, sizeof(SzTemp), _T("clothback\\cloth%d\\fashionglove\\icon"),i);
		m_pFashion[i][EF_Glove] = (ItemButton*)m_pWndBack->GetChild(SzTemp);
		m_pFashion[i][EF_Glove]->RefreshItem();

		_sntprintf(SzTemp, sizeof(SzTemp), _T("clothback\\cloth%d\\fashionboot\\icon"),i);
		m_pFashion[i][EF_Boot] = (ItemButton*)m_pWndBack->GetChild(SzTemp);
		m_pFashion[i][EF_Boot]->RefreshItem();

		_sntprintf(SzTemp, sizeof(SzTemp), _T("clothback\\cloth%d\\fashionback\\icon"),i);
		m_pFashion[i][EF_Back] = (ItemButton*)m_pWndBack->GetChild(SzTemp);
		m_pFashion[i][EF_Back]->RefreshItem();
	}

	m_pBtnReset = (GUIButton*)m_pWndBack->GetChild(_T("showback\\fittingroom\\refresh"));
	m_pStcCurPage = (GUIStatic*)m_pWndBack->GetChild(_T("clothback\\num"));
	m_pBtnPre = (GUIButton*)m_pWndBack->GetChild(_T("clothback\\pageback"));
	m_pBtnNext = (GUIButton*)m_pWndBack->GetChild(_T("clothback\\pagenext"));
	m_pBtnClose = (GUIButton*)m_pWndBack->GetChild(_T("close"));

	m_pIconDiySuit = (PicStatic*)m_pWndBack->GetChild(_T("clothback\\diycloth"));
	m_pStcDiyTitle = (PicStatic*)m_pWndBack->GetChild(_T("clothback\\diycloth\\title"));
	m_pStcDiyClothesexpressLevel = (PicStatic*)m_pWndBack->GetChild(_T("clothback\\diycloth\\title\\level"));
	m_pStcDiySuitName = (PicStatic*)m_pWndBack->GetChild(_T("clothback\\diycloth\\title\\name"));
	m_pStcDiyWear = (GUIStatic*)m_pWndBack->GetChild(_T("clothback\\diycloth\\title\\wearingpic"));
	m_pStcDiyWear->SetInvisible(TRUE);
	m_pStcDiyAttrativeValue = (GUIStaticEx*)m_pWndBack->GetChild(_T("clothback\\diycloth\\word\\num"));
	m_pStcDiySuitTips = (GUIStaticEx*)m_pWndBack->GetChild(_T("clothback\\diycloth\\wordback0\\word"));
	m_pStcDiySuitState = (GUIStaticEx*)m_pWndBack->GetChild(_T("clothback\\diycloth\\wordback1\\word"));

	m_pStcAttrativeTotalValue = (GUIStaticEx*)m_pWndBack->GetChild(_T("total\\num"));

	m_pDiyFashion[EF_Hat] = (ItemButton*)m_pWndBack->GetChild(_T("clothback\\diycloth\\fashionhat\\icon"));
	m_pDiyFashion[EF_Hat]->RefreshItem();
	m_pDiyFashion[EF_Clothes] = (ItemButton*)m_pWndBack->GetChild(_T("clothback\\diycloth\\fashioncloth\\icon"));
	m_pDiyFashion[EF_Clothes]->RefreshItem();
	m_pDiyFashion[EF_Glove] = (ItemButton*)m_pWndBack->GetChild(_T("clothback\\diycloth\\fashionglove\\icon"));
	m_pDiyFashion[EF_Glove]->RefreshItem();
	m_pDiyFashion[EF_Boot] = (ItemButton*)m_pWndBack->GetChild(_T("clothback\\diycloth\\fashionboot\\icon"));
	m_pDiyFashion[EF_Boot]->RefreshItem();
	m_pDiyFashion[EF_Back] = (ItemButton*)m_pWndBack->GetChild(_T("clothback\\diycloth\\fashionback\\icon"));
	m_pDiyFashion[EF_Back]->RefreshItem();


	m_pWndTryDress = (PlayerTryDressWnd*)m_pWndBack->GetChild(_T("showback\\fittingroom"));
	m_pStcCurSuitNum = (GUIStatic*)m_pWndBack->GetChild(_T("num"));
	m_pStcCollectRate = (GUIStatic*)m_pWndBack->GetChild(_T("percent\\num"));
	m_pBtnChangeSuit = (GUIButton*)m_pWndBack->GetChild(_T("change"));
	m_pBtnChangeSuit->SetEnable(FALSE);
	m_pBtnUpgradeClothes = (GUIButton*)m_pWndBack->GetChild(_T("upgrade"));
	m_pBtnUpgradeClothes->SetEnable(FALSE);
	m_pWnd->SetInvisible(TRUE);
	m_pWnd->FlipToTop();
	// 注册事件处理函数
	m_pGUI->RegisterEventHandler( m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&ClothespressFrame::EventHandler) );
	return TRUE;
}


//-----------------------------------------------------------------------------
// 界面消息事件处理
//-----------------------------------------------------------------------------
BOOL ClothespressFrame::EventHandler( tagGUIEvent* pEvent )
{
	GUIWnd* pWnd = m_pGUI->GetWnd( pEvent->dwWndID );
	switch( pEvent->eEvent )
	{
	case EGUIE_PushButtonDown:
		{
			if( pWnd == m_pBtnMale || pWnd == m_pBtnFemale )
			{
				m_n16CurPage = 0;
				UpdateCurPage();
				UpdateDIYFashion();
				INT16 n16MaxFashion = 0;
				if(m_pBtnMale->GetState() == EGUIBS_PushDown)
					n16MaxFashion = m_n16MaxMaleFashion;
				else if(m_pBtnFemale->GetState() == EGUIBS_PushDown)
					n16MaxFashion = m_n16MaxFemalFashion;
				TCHAR szTemp[X_SHORT_NAME] = {0};
				_sntprintf(szTemp, X_SHORT_NAME, _T("%d/%d"), m_n16CurPage+1, n16MaxFashion);
				m_pStcCurPage->SetText(szTemp);
				m_pBtnNext->SetEnable(TRUE);
				m_pBtnPre->SetEnable(TRUE);
				m_pBtnChangeSuit->SetEnable(FALSE);
				m_pBtnUpgradeClothes->SetEnable(FALSE);
			}
		}
		break;
	case EGUIE_Drag:
		{
			if(pWnd == m_pIconDiySuit || 
				pWnd == m_pStcDiyClothesexpressLevel ||
				pWnd == m_pStcDiySuitName ||
				pWnd == m_pStcDiyTitle )
			{
				InitBackPic();
				m_pIconDiySuit->SetPic(szPicSele);
				tagClothespressData Info = ClothespressData::Inst()->GetDIYFashion();
				m_dwCurSuitIndex = Info.nSuitId;
				TMap<DWORD, tagFashionInfo*> mapInfo = ItemMgr::Inst()->GetFashionMap();
				tagFashionInfo *pFashionInfo = mapInfo.Peek(m_dwCurSuitIndex);
				if(!P_VALID(pFashionInfo))
					break;
				m_pBtnChangeSuit->SetText(g_StrTable[_T("GetOnFashion")]);
				if(pFashionInfo->eState == EFS_Incomplete || EFS_Complete == pFashionInfo->eState || EFS_UseBySelf == pFashionInfo->eState )
				{
					m_pBtnChangeSuit->SetEnable(TRUE);
					if(EFS_UseBySelf == pFashionInfo->eState)
						m_pBtnChangeSuit->SetText(g_StrTable[_T("GetOffFashion")]);
				}
				else
					m_pBtnChangeSuit->SetEnable(FALSE);

				if(pFashionInfo->byLevel > 0 && pFashionInfo->byLevel < MAX_LEVEL_CLOTHESPRESS)
					m_pBtnUpgradeClothes->SetEnable(TRUE);
				else
					m_pBtnUpgradeClothes->SetEnable(FALSE);
				TryDress(TRUE);
				break;
			}
			else
			{
				for( int i = 0; i< FASHION_SUIT_NUM_OF_ONE_PAGE; i++)
				{
					if(pWnd == m_pIconSuit[i] || 
						m_pStcClothesexpressLevel[i] == pWnd || 
						m_pStcSuitName[i] == pWnd ||
						m_pStcTitle[i] == pWnd )
					{
						InitBackPic();
						m_pIconSuit[i]->SetPic(szPicSele);
						GetCurSuitID(i);
						TMap<DWORD, tagFashionInfo*> mapInfo = ItemMgr::Inst()->GetFashionMap();
						tagFashionInfo *pFashionInfo = mapInfo.Peek(m_dwCurSuitIndex);
						if(!P_VALID(pFashionInfo))
							break;
						m_pBtnChangeSuit->SetText(g_StrTable[_T("GetOnFashion")]);
						if(pFashionInfo->eState == EFS_Incomplete || EFS_Complete == pFashionInfo->eState || EFS_UseBySelf == pFashionInfo->eState )
						{
							m_pBtnChangeSuit->SetEnable(TRUE);
							if(EFS_UseBySelf == pFashionInfo->eState)
								m_pBtnChangeSuit->SetText(g_StrTable[_T("GetOffFashion")]);
						}
						else
							m_pBtnChangeSuit->SetEnable(FALSE);

						if(pFashionInfo->byLevel >=0 && pFashionInfo->byLevel < MAX_LEVEL_CLOTHESPRESS)
							m_pBtnUpgradeClothes->SetEnable(TRUE);
						else
							m_pBtnUpgradeClothes->SetEnable(FALSE);
						TryDress(FALSE);
						break;
					}
				}
			}
		}
		break;
	case EGUIE_Click:
		{
			if(pWnd == m_pBtnClose)
			{
				if(P_VALID(m_pWnd))
				{
					m_pWnd->SetInvisible(TRUE);
					m_n16CurPage = 0;
					m_dwCurSuitIndex = 0;
					SetStrategy(EIST_ItemPocket);
				}
				break;
			}
			else if( pWnd == m_pBtnUpgradeClothes )
			{
				tagNC_WardrobeLevelUp m;
				m.dwWardrobeLayerID = m_dwCurSuitIndex;
				m_pSession->Send(&m);
				m_pBtnUpgradeClothes->SetEnable(false);
				break;
			}
			else if(pWnd == m_pBtnChangeSuit)
			{
				TMap<DWORD, tagFashionInfo*> mapInfo = ItemMgr::Inst()->GetFashionMap();
				tagFashionInfo *pFashionInfo = mapInfo.Peek(m_dwCurSuitIndex);
				if(!P_VALID(pFashionInfo))
					break;

				bool bTakeOffFashion = false;
				if(EFS_UseBySelf == pFashionInfo->eState )
					bTakeOffFashion = true;
				tagNC_Rehanding m;
				m.dwWardrobeLayerID = m_dwCurSuitIndex;
				m.bTakeOffFashion = bTakeOffFashion;
				m_pSession->Send(&m);
				m_pBtnChangeSuit->SetEnable(false);
				break;
			}
			else if( pWnd == m_pBtnReset )
			{
				if( P_VALID(m_pWndTryDress) )
				{
					m_pWndTryDress->RotateYaw(0.0f);
					m_pWndTryDress->ReLoadTryEquip();
				}
				break;
			}
			else if( pWnd == m_pBtnPre )
			{
				m_n16CurPage--;
				if( m_n16CurPage <= 0 )
				{
					//m_pBtnPre->SetEnable(false);
					m_n16CurPage = 0;
				}
				//else
				//	m_pBtnNext->SetEnable(true);
				INT16 n16MaxFashion = 0;
				if(m_pBtnMale->GetState() == EGUIBS_PushDown)
					n16MaxFashion = m_n16MaxMaleFashion;
				else if(m_pBtnFemale->GetState() == EGUIBS_PushDown)
					n16MaxFashion = m_n16MaxFemalFashion;
				TCHAR szTemp[X_SHORT_NAME] = {0};
				_sntprintf(szTemp, X_SHORT_NAME, _T("%d/%d"), m_n16CurPage+1, n16MaxFashion);
				m_pStcCurPage->SetText(szTemp);
				m_dwCurSuitIndex = 0;
				UpdateCurPage();
				break;
			}
			else if( pWnd == m_pBtnNext )
			{
				m_n16CurPage++;
				INT16 n16MaxFashion = 0;
				if( m_pBtnMale->GetState() == EGUIBS_PushDown )
				{
					n16MaxFashion = m_n16MaxMaleFashion;
					if( m_n16CurPage >= m_n16MaxMaleFashion - 1 )
					{
						//m_pBtnNext->SetEnable(false);
						m_n16CurPage = m_n16MaxMaleFashion - 1;
					}
					//else
					//	m_pBtnPre->SetEnable(true);
				}
				else if( m_pBtnFemale->GetState() == EGUIBS_PushDown )
				{
					n16MaxFashion = m_n16MaxFemalFashion;
					if(m_n16CurPage >= m_n16MaxFemalFashion - 1)
					{
						//m_pBtnNext->SetEnable(false);
						m_n16CurPage = m_n16MaxFemalFashion - 1;
					}
					//else
					//	m_pBtnPre->SetEnable(true);
				}
				TCHAR szTemp[X_SHORT_NAME] = {0};
				_sntprintf(szTemp, X_SHORT_NAME, _T("%d/%d"), m_n16CurPage+1, n16MaxFashion);
				m_pStcCurPage->SetText(szTemp);
				m_dwCurSuitIndex = 0;
				UpdateCurPage();
				break;
			}
			else if(pWnd == m_pIconDiySuit || 
				pWnd == m_pStcDiyClothesexpressLevel ||
				pWnd == m_pStcDiySuitName ||
				pWnd == m_pStcDiyTitle )
			{
				InitBackPic();
				m_pIconDiySuit->SetPic(szPicSele);
				tagClothespressData Info = ClothespressData::Inst()->GetDIYFashion();
				m_dwCurSuitIndex = Info.nSuitId;
				TMap<DWORD, tagFashionInfo*> mapInfo = ItemMgr::Inst()->GetFashionMap();
				tagFashionInfo *pFashionInfo = mapInfo.Peek(m_dwCurSuitIndex);
				if(!P_VALID(pFashionInfo))
					break;
				m_pBtnChangeSuit->SetText(g_StrTable[_T("GetOnFashion")]);
				if(pFashionInfo->eState == EFS_Incomplete || EFS_Complete == pFashionInfo->eState || EFS_UseBySelf == pFashionInfo->eState )
				{
					m_pBtnChangeSuit->SetEnable(TRUE);
					if(EFS_UseBySelf == pFashionInfo->eState)
						m_pBtnChangeSuit->SetText(g_StrTable[_T("GetOffFashion")]);
				}
				else
					m_pBtnChangeSuit->SetEnable(FALSE);

				if(pFashionInfo->byLevel >=0 && pFashionInfo->byLevel < MAX_LEVEL_CLOTHESPRESS)
					m_pBtnUpgradeClothes->SetEnable(TRUE);
				else
					m_pBtnUpgradeClothes->SetEnable(FALSE);
				TryDress(TRUE);
				break;
			}
			else
			{
				for( int i = 0; i< FASHION_SUIT_NUM_OF_ONE_PAGE; i++)
				{
					if(pWnd == m_pIconSuit[i] || 
						m_pStcClothesexpressLevel[i] == pWnd || 
						m_pStcSuitName[i] == pWnd ||
						pWnd == m_pStcTitle[i] )
					{
						InitBackPic();
						m_pIconSuit[i]->SetPic(szPicSele);
						GetCurSuitID(i);
						TMap<DWORD, tagFashionInfo*> mapInfo = ItemMgr::Inst()->GetFashionMap();
						tagFashionInfo *pFashionInfo = mapInfo.Peek(m_dwCurSuitIndex);
						if(!P_VALID(pFashionInfo))
							break;

						m_pBtnChangeSuit->SetText(g_StrTable[_T("GetOnFashion")]);
						if(pFashionInfo->eState == EFS_Incomplete || EFS_Complete == pFashionInfo->eState || EFS_UseBySelf == pFashionInfo->eState )
						{
							m_pBtnChangeSuit->SetEnable(TRUE);
							if(EFS_UseBySelf == pFashionInfo->eState)
								m_pBtnChangeSuit->SetText(g_StrTable[_T("GetOffFashion")]);
						}
						else
							m_pBtnChangeSuit->SetEnable(FALSE);

						if(pFashionInfo->byLevel >=0 && pFashionInfo->byLevel < MAX_LEVEL_CLOTHESPRESS)
							m_pBtnUpgradeClothes->SetEnable(TRUE);
						else
							m_pBtnUpgradeClothes->SetEnable(FALSE);
						TryDress(FALSE);
						break;
					}
				}
			}
			
			INT64 n64ItemID = GT_INVALID;
			for( int i = 0; i < FASHION_SUIT_NUM_OF_ONE_PAGE; i++ )
			{
				bool bFlag = false;
				for( int j = 0; j < EF_End; j++ )
				{
					if(pWnd == m_pFashion[i][j] )
					{
						const BtnData &Data = m_pFashion[i][j]->GetBtnData();
						Equipment *pEquip = ItemMgr::Inst()->GetFashion(Data.GetData2());
						if(P_VALID(pEquip))
							n64ItemID = pEquip->GetItemId();
						bFlag = true;
						break;
					}
				}
				if(bFlag)
					break;
			}

			for( int i = 0; i< EF_End; i++ )
			{
				if(pWnd == m_pDiyFashion[i] )
				{
					const BtnData Data = m_pDiyFashion[i]->GetBtnData();
					Equipment *pEquip = ItemMgr::Inst()->GetFashion(Data.GetData2(), true);
					if(P_VALID(pEquip))
					{
						n64ItemID = pEquip->GetItemId();
						break;
					}
				}
			}

			if(n64ItemID != GT_INVALID)
			{
				tagNC_ItemPosChangeEx m;
				m.eConTypeSrc = EICT_Wardrobe;
				m.eConTypeDst = EICT_Bag;
				m.n16PosDst = GT_INVALID;
				m.n64Serial1 = n64ItemID;
				m.dwNPCID = GT_INVALID;
				m_pSession->Send(&m);
			}
		}
		break;
	case EGUISCB_Drop:
	case EGUISCB_DropCheck:
		{
			for( int i = 0; i < FASHION_SUIT_NUM_OF_ONE_PAGE; i++ )
			{
				bool bFlag = false;
				for( int j = 0; j < EF_End; j++ )
				{
					if(pWnd == m_pFashion[i][j] )
					{
						PutItem((INT16)LOWORD(pEvent->dwParam2), j,false, i);
						bFlag = true;
						break;
					}
				}
				if(bFlag)
					break;
			}
			for( int i = 0; i< EF_End; i++ )
			{
				if(pWnd == m_pDiyFashion[i] )
				{
					TMap<DWORD, tagFashionInfo*> mapInfo = ItemMgr::Inst()->GetFashionMap();
					TMap<DWORD, tagFashionInfo*>::TMapIterator iter = mapInfo.Begin();
					tagFashionInfo *pFashionInfo = iter->second;
					if(P_VALID(pFashionInfo) && pFashionInfo->eState == EFS_UseBySelf)
					{
						CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("FashionWearingNoPut")]);
						break;
					}
					PutItem((INT16)LOWORD(pEvent->dwParam2), i, true);
					break;
				}
			}
		}
		break;
	case EGUIE_MoveIntoCtrl:
		{
			for( int i = 0; i < FASHION_SUIT_NUM_OF_ONE_PAGE; i++ )
			{
				bool bFlag = false;
				for( int j = 0; j < EF_End; j++ )
				{
					if(pWnd == m_pFashion[i][j] )
					{
						const BtnData &Data = m_pFashion[i][j]->GetBtnData();
						DWORD dwTypeID = Data.GetData2();
						Equipment *pEquip = ItemMgr::Inst()->GetFashion(dwTypeID);
						if(P_VALID(pEquip))
							ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pEquip).c_str());
						else
							ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(dwTypeID).c_str());
						bFlag = true;
						break;
					}
				}
				if(bFlag)
					break;
			}
			for( int i = 0; i< EF_End; i++ )
			{
				if(pWnd == m_pDiyFashion[i] )
				{
					const BtnData &Data = m_pDiyFashion[i]->GetBtnData();
					DWORD dwTypeID = Data.GetData2();
					Equipment *pEquip = ItemMgr::Inst()->GetFashion(dwTypeID, TRUE);
					if(P_VALID(pEquip))
						ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(pEquip).c_str());
					else
						ShowTip(pWnd, ToolTipCreator::Inst()->GetItemTips(dwTypeID).c_str());
					break;
				}
			}
		}
		break;
	}
	return FALSE;
}

VOID ClothespressFrame::PutItem(INT16 nPos, int nCol, bool bDiy /* = false */, int nRow /* = 0 */)
{
	LocalPlayer *pLP = RoleMgr::Inst()->GetLocalPlayer();
	if(!P_VALID(pLP))
		return;

	Item *pItem = ItemMgr::Inst()->GetPocketItem(nPos);
	if(!P_VALID(pItem))
		return;

	const tagItemProto* pItemProto = pItem->GetProto();
	if( !P_VALID(pItemProto) )
		return;

	const tagItem *pData = pItem->GetData();
	if( !P_VALID(pData) )
		return;

	if(!MIsEquipment(pItemProto->dwTypeID))
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("OnlyCanPutFashion")]);
		return;
	}

	if( pItemProto->eType != EIT_Fashion)
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("OnlyCanPutFashion")]);
		return;
	}

	EEquipPos ePos = Change2EquipPos(nCol);
	const tagEquipProto *pEquipProto = (tagEquipProto*)pItemProto;
	if( ePos != pEquipProto->eEquipPos )
	{
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PutFashion2WrongPos")]);
		return;
	}

	INT16 nDestPos = GT_INVALID;
	if(bDiy)
	{
		if(pItemProto->eSexLimit != pLP->GetPlayerSex())
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PutFashion2DIY")]);
			return;
		}
		nDestPos = (INT16)nCol;
	}
	else
	{
		const BtnData &Data = m_pFashion[nRow][nCol]->GetBtnData();
		DWORD dwTypeID = Data.GetData2();
		if(dwTypeID != pItemProto->dwTypeID)
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PutFashion2WrongRow")]);
			return;
		}
		if( ( pItemProto->eSexLimit == ESL_Woman && m_pBtnMale->GetState() == EGUIBS_PushDown ) ||
			(pItemProto->eSexLimit == ESL_Man && m_pBtnFemale->GetState() == EGUIBS_PushDown) )
		{
			CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("PutFashion2Clothespress")]);
			return;
		}
	}
	tagNC_ItemPosChangeEx m;
	m.eConTypeDst = EICT_Wardrobe;
	m.eConTypeSrc = EICT_Bag;
	m.n16PosDst = nDestPos;
	m.n64Serial1 = pData->n64Serial;
	m.dwNPCID = GT_INVALID;
	m_pSession->Send(&m);

	InitBackPic();
	m_dwCurSuitIndex = 0;
}

DWORD ClothespressFrame::OnEventHideUI(tagGameEvent* pGameEvent)
{
	if(P_VALID(m_pWnd))
	{
		m_pWnd->SetInvisible(TRUE);
		m_n16CurPage = 0;
		m_dwCurSuitIndex = 0;
		SetStrategy(EIST_ItemPocket);
	}
	return 0;
}

DWORD ClothespressFrame::OnEventUpdateFashionInfo(tagGameEvent* pGameEvent)
{
	if(m_pWnd->IsInvisible())
		return 0;
	UpdateCurPage();
	UpdateDIYFashion();
	RefreshCollectNum();
	return 0;
}

VOID ClothespressFrame::ShowUI()
{
	if(!P_VALID(m_pWnd))
		return;
	if(m_pWnd->IsInvisible())
	{
		SetStrategy(EIST_Clothespress);
		m_pWnd->SetInvisible(FALSE);
		UpdateCurPage();
		UpdateDIYFashion();
		RefreshCollectNum();
		UpdateAvatar();
		LocalPlayer *pLp = RoleMgr::Inst()->GetLocalPlayer();
		if(P_VALID(pLp))
		{
			if(pLp->GetPlayerSex())
				m_pBtnMale->SetState(EGUIBS_PushDown);
			else
				m_pBtnFemale->SetState(EGUIBS_PushDown);
		}
	}
	else
	{
		m_pWnd->SetInvisible(TRUE);
		m_n16CurPage = 0;
		m_dwCurSuitIndex = 0;
		SetStrategy(EIST_ItemPocket);
	}
}

DWORD ClothespressFrame::OnNS_WardrobeLevelUp( tagNS_WardrobeLevelUp* pNetCmd, DWORD dwParam )
{
	switch(pNetCmd->dwErrorCode)
	{
	case E_Success:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ClothespressLevelUpSucc")]);
		break;
	case EFEC_NoLevelUpItem:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ClothespressLevelUpFailNoItem")]);
		break;
	case E_WardrobeLevelUp_NoItem1:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ClothespressLevelUpFailNoItem1")]);
		break;
	case E_WardrobeLevelUp_NoItem2:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ClothespressLevelUpFailNoItem2")]);
		break;
	case E_WardrobeLevelUp_Fail:
		CombatSysUtil::Inst()->ShowScreenCenterMsg(g_StrTable[_T("ClothespressLevelUpFail")]);
		break;
	}

	TMap<DWORD, tagFashionInfo*> mapInfo = ItemMgr::Inst()->GetFashionMap();
	tagFashionInfo *pFashionInfo = mapInfo.Peek(m_dwCurSuitIndex);
	if(!P_VALID(pFashionInfo))
		return 0;
	if(pFashionInfo->byLevel > 0 && pFashionInfo->byLevel < MAX_LEVEL_CLOTHESPRESS)
		m_pBtnUpgradeClothes->SetEnable(TRUE);
	else
		m_pBtnUpgradeClothes->SetEnable(FALSE);
	return 0;
}

VOID ClothespressFrame::Update()
{
	
}

EEquipPos ClothespressFrame::Change2EquipPos(int nIndex)
{
	switch(nIndex)
	{
	case 0:
		return EEP_FashionHead;
		break;
	case 1:
		return EEP_FashionBody;
		break;
	case 2:
		return EEP_FashionWrist;
		break;
	case 3:
		return EEP_FashionFeet;
		break;
	case 4:
		return EEP_FashionBack;
		break;
	}
	return EEP_Null;
}

VOID ClothespressFrame::Render3D()
{
	if(P_VALID(m_pWndTryDress))
		m_pWndTryDress->Render3D();
	GameFrame::Render3D();
}

//更新试穿模型
void ClothespressFrame::UpdateAvatar()
{
	LocalPlayer* pLp = RoleMgr::Inst()->GetLocalPlayer();
	if(P_VALID(pLp) && P_VALID(m_pWndTryDress))
	{
		m_pWndTryDress->UpdataAvatar( pLp );
	}
}

void ClothespressFrame::JudgeTryDress( INT64 n64ID )
{
	Equipment *pEquip = ItemMgr::Inst()->GetFashion(n64ID);
	if(!P_VALID(pEquip))
		return;
	const tagItemProto* pItem = pEquip->GetProto();
	LocalPlayer* pLRole = RoleMgr::Inst()->GetLocalPlayer();
	if( P_VALID(pItem) && P_VALID(pLRole) )
	{
		AvatarEquipNode* pNode = (AvatarEquipNode*)pLRole->GetSceneNode();
		if( pItem->eType==EIT_Fashion && P_VALID(pNode) 
			&&( pNode->GetAtt().bySex==(BYTE)pItem->eSexLimit || -1==(BYTE)pItem->eSexLimit) )
		{
			tagEquipProto* pEquip = (tagEquipProto*)pItem;
			if( P_VALID(pEquip) )
			{
				EAvatarElement eAvat = (EAvatarElement)MallMgr::Inst()->GetEquipDisplayPos((INT16)pEquip->eEquipPos);
				tagEquipDisplay ep;
				ep.dwTypeID = pEquip->dwTypeID;
				ep.byDisplayPos = eAvat;
				ep.n8ColorID = EC_Null;
				m_pWndTryDress->SetEquip(eAvat, ep);
			}
		}
	}
}

void ClothespressFrame::InitBackPic()
{
	for( int i = 0; i < FASHION_SUIT_NUM_OF_ONE_PAGE; i++ )
		m_pIconSuit[i]->SetPic(szPicInit);
	m_pIconDiySuit->SetPic(szPicInit);
}

VOID ClothespressFrame::UpdateCurPage()
{
	for(int i = 0; i < FASHION_SUIT_NUM_OF_ONE_PAGE; i++)
	{
		m_pIconSuit[i]->SetInvisible(TRUE);
		m_pIconSuit[i]->SetPic(szPicInit);
		for(int j = 0; j < EF_End; j++)
			m_pFashion[i][j]->RefreshItem();
	}
	vector<tagClothespressData> vecFashion;
	if(m_pBtnFemale->GetState() == EGUIBS_PushDown)
	{
		vecFashion = ClothespressData::Inst()->GetFemaleFashionData();
		m_n16MaxFemalFashion = vecFashion.size() % FASHION_SUIT_NUM_OF_ONE_PAGE == 0 ? 
			vecFashion.size() / FASHION_SUIT_NUM_OF_ONE_PAGE : vecFashion.size() / 
			FASHION_SUIT_NUM_OF_ONE_PAGE + 1;
	}
	else
	{
		vecFashion = ClothespressData::Inst()->GetMaleFashionData();
		m_n16MaxMaleFashion = vecFashion.size() % FASHION_SUIT_NUM_OF_ONE_PAGE == 0 ? 
			vecFashion.size() / FASHION_SUIT_NUM_OF_ONE_PAGE : vecFashion.size() / 
			FASHION_SUIT_NUM_OF_ONE_PAGE + 1;
	}

	size_t nStart = m_n16CurPage * FASHION_SUIT_NUM_OF_ONE_PAGE;
	size_t nEnd = ( m_n16CurPage + 1 ) * FASHION_SUIT_NUM_OF_ONE_PAGE;
	TMap<DWORD, tagFashionInfo*> mapInfo = ItemMgr::Inst()->GetFashionMap();
	TCHAR szTemp[X_LONG_NAME] = {0};
	for(int i = 0; i < FASHION_SUIT_NUM_OF_ONE_PAGE; i++)
	{
		if( nStart >= vecFashion.size() )
			break;
		DWORD dwSuitIndex = vecFashion[nStart].nSuitId;
		if(!mapInfo.IsExist(dwSuitIndex))
			continue;
		m_pIconSuit[i]->SetInvisible(FALSE);
		tagFashionInfo *pFashionInfo = mapInfo.Peek(dwSuitIndex);
		_sntprintf(szTemp, X_LONG_NAME, g_StrTable[_T("ClothespressLevel")], pFashionInfo->byLevel);
		m_pStcClothesexpressLevel[i]->SetText(szTemp);
		tstring strName = ClothespressData::Inst()->GetSuitName(dwSuitIndex);
		m_pStcSuitName[i]->SetText(strName.c_str());

		const tagBuffProtoClient* pBuffProto = SkillProtoData::Inst()->FindBuffProto( vecFashion[nStart].dwBufferID[pFashionInfo->byLevel-1]);
		
		if(P_VALID(pBuffProto))
		{
			if(ISSuitComplete(dwSuitIndex))
				_sntprintf(szTemp, X_LONG_NAME, g_StrTable[_T("UsfulFairySkill")], pBuffProto->szDesc);
			else
				_sntprintf(szTemp, X_LONG_NAME, g_StrTable[_T("UnUsfulFairySkill")], pBuffProto->szDesc);
			m_pStcSuitTips[i]->SetText(szTemp);
		}
		
		m_pStcWear[i]->SetInvisible(TRUE);
		m_pStcFriend[i]->SetInvisible(TRUE);
		switch(pFashionInfo->eState)
		{
		case EFS_Complete:
			m_pStcSuitState[i]->SetText(g_StrTable[_T("ClothepressComplete")]);
			break;
		case EFS_None:
		case EFS_Incomplete:
			m_pStcSuitState[i]->SetText(g_StrTable[_T("ClothepressImcomplete")]);
			break;
		case EFS_UseBySelf:
			m_pStcSuitState[i]->SetText(g_StrTable[_T("ClothepressUseBySelf")]);
			m_pStcWear[i]->SetInvisible(FALSE);
			break;
		case EFS_UseForFriends:
			m_pStcSuitState[i]->SetText(g_StrTable[_T("ClothepressUseForFriends")]);
			m_pStcFriend[i]->SetInvisible(FALSE);
			break;
		}

		tagClothespressData *pData = ClothespressData::Inst()->GetFashionInfo(dwSuitIndex);
		if(P_VALID(pData))
		{
			INT nMettle = 0;
			for( int j = 0; j < EF_End; j++ )
			{
				Equipment* pEquip = ItemMgr::Inst()->GetFashion(pData->dwSuitTypeID[j], false);
				if (P_VALID(pEquip))
				{
					DWORD dwTypeID = pEquip->GetItemTypeID();
					BYTE byQuality = pEquip->GetItemQuality();
					m_pFashion[i][j]->RefreshItem(dwTypeID, 1, byQuality);
					nMettle += pEquip->GetEquip()->n16Appearance;
				}
				else
					m_pFashion[i][j]->SetBtnData2(pData->dwSuitTypeID[j]);
			}
			if(nMettle <= 9)
				_sntprintf(szTemp, X_LONG_NAME, g_StrTable[_T("Mettle_Common")], nMettle);
			else if(nMettle <= 15)
				_sntprintf(szTemp, X_LONG_NAME, g_StrTable[_T("Mettle_Yellow")], nMettle);
			else if(nMettle <= 20)
				_sntprintf(szTemp, X_LONG_NAME, g_StrTable[_T("Mettle_Gold")], nMettle);
			else
				_sntprintf(szTemp, X_LONG_NAME, g_StrTable[_T("Mettle_Pink")], nMettle);
			m_pStcAttrativeValue[i]->SetText(szTemp);
		}
		nStart++;
	}
	_sntprintf(szTemp, X_LONG_NAME,_T("%d") , GetTotalAttrativeValue());
	m_pStcAttrativeTotalValue->SetText(szTemp);
}

VOID ClothespressFrame::UpdateDIYFashion()
{
	for(int j = 0; j < EF_End; j++)
		m_pDiyFashion[j]->RefreshItem();
	m_pIconDiySuit->SetPic(szPicInit);
	TMap<DWORD, tagFashionInfo*> mapInfo = ItemMgr::Inst()->GetFashionMap();
	if(mapInfo.Empty())
		return;
	TMap<DWORD, tagFashionInfo*>::TMapIterator iter = mapInfo.Begin();
	tagFashionInfo *pFashionInfo = iter->second;

	TCHAR szTemp[X_LONG_NAME] = {0};
	_sntprintf(szTemp, X_LONG_NAME, g_StrTable[_T("ClothespressLevel")], pFashionInfo->byLevel);
	m_pStcDiyClothesexpressLevel->SetText(szTemp);
	tstring strName = ClothespressData::Inst()->GetSuitName(pFashionInfo->dwIndex);
	m_pStcDiySuitName->SetText(strName.c_str());

	const tagClothespressData DIYFashion = ClothespressData::Inst()->GetDIYFashion();
	const tagBuffProtoClient* pBuffProto = SkillProtoData::Inst()->FindBuffProto(DIYFashion.dwBufferID[pFashionInfo->byLevel-1]);

	if(P_VALID(pBuffProto))
	{
		if(IsDiySuitIsComplete())
			_sntprintf(szTemp, X_LONG_NAME, g_StrTable[_T("UsfulFairySkill")], pBuffProto->szDesc);
		else
			_sntprintf(szTemp, X_LONG_NAME, g_StrTable[_T("UnUsfulFairySkill")], pBuffProto->szDesc);
		m_pStcDiySuitTips->SetText(szTemp);
	}

	m_pStcDiyWear->SetInvisible(TRUE);
	switch(pFashionInfo->eState)
	{
	case EFS_Complete:
		m_pStcDiySuitState->SetText(g_StrTable[_T("ClothepressComplete")]);
		break;
	case EFS_None:
	case EFS_Incomplete:
		m_pStcDiySuitState->SetText(g_StrTable[_T("ClothepressImcomplete")]);
		break;
	case EFS_UseBySelf:
		m_pStcDiySuitState->SetText(g_StrTable[_T("ClothepressUseBySelf")]);
		m_pStcDiyWear->SetInvisible(FALSE);
		break;
	case EFS_UseForFriends:
		m_pStcDiySuitState->SetText(g_StrTable[_T("ClothepressUseForFriends")]);
		break;
	}

	INT nMettle = 0;
	for( int j = 0; j < EF_End; j++ )
	{
		Equipment *pEquip = (Equipment*)ItemMgr::Inst()->GetClothespress()->GetValue(j);
		DWORD dwTypeID = 0;
		if (P_VALID(pEquip))
		{
			dwTypeID = pEquip->GetItemTypeID();
			BYTE byQuality = pEquip->GetItemQuality();
			m_pDiyFashion[j]->RefreshItem(dwTypeID, 1, byQuality);
			nMettle += pEquip->GetEquip()->n16Appearance;
		}
		else
			m_pDiyFashion[j]->SetBtnData2(dwTypeID);
	}
	if(nMettle <= 9)
		_sntprintf(szTemp, X_LONG_NAME, g_StrTable[_T("Mettle_Common")], nMettle);
	else if(nMettle <= 15)
		_sntprintf(szTemp, X_LONG_NAME, g_StrTable[_T("Mettle_Yellow")], nMettle);
	else if(nMettle <= 20)
		_sntprintf(szTemp, X_LONG_NAME, g_StrTable[_T("Mettle_Gold")], nMettle);
	else
		_sntprintf(szTemp, X_LONG_NAME, g_StrTable[_T("Mettle_Pink")], nMettle);
	m_pStcDiyAttrativeValue->SetText(szTemp);
}

DWORD ClothespressFrame::GetCurCollectFashionNum()
{
	TMap<DWORD, tagFashionInfo*> mapInfo = ItemMgr::Inst()->GetFashionMap();
	mapInfo.ResetIterator();
	tagFashionInfo *pFashionInfo = NULL;
	DWORD dwSuitID;
	DWORD dwNum = 0;
	while(mapInfo.PeekNext(dwSuitID,pFashionInfo))
	{
		if( P_VALID(pFashionInfo) && pFashionInfo->eState != EFS_Incomplete
			&& pFashionInfo->eState != EFS_None )
			dwNum++;
	}
	return dwNum;
}

VOID ClothespressFrame::RefreshCollectNum()
{
	DWORD dwCollectNum = GetCurCollectFashionNum();
	TMap<DWORD, tagFashionInfo*> mapInfo = ItemMgr::Inst()->GetFashionMap();
	DWORD dwTotalNum = mapInfo.Size();
	if( dwTotalNum != 0 )
	{
		TCHAR szTemp[X_SHORT_NAME] = {0};
		_sntprintf(szTemp, X_SHORT_NAME, _T("%d/%d"), dwCollectNum, dwTotalNum);
		m_pStcCurSuitNum->SetText(szTemp);
		INT nRate = INT(( (float)dwCollectNum / (float)dwTotalNum ) * 100.0f);
		_sntprintf(szTemp, X_SHORT_NAME, _T("%d%s"), nRate, _T("%"));
		m_pStcCollectRate->SetText(szTemp);
	}
}

VOID ClothespressFrame::GetCurSuitID(INT nIndex)
{
	if(nIndex >=0 && nIndex < FASHION_SUIT_NUM_OF_ONE_PAGE)
	{
		BOOL bMan = GT_INVALID;
		if(m_pBtnFemale->GetState() == EGUIBS_PushDown )
			bMan = FALSE;
		if(m_pBtnMale->GetState() == EGUIBS_PushDown )
			bMan = TRUE;
		if(bMan != GT_INVALID)
		{
			vector<tagClothespressData> vecFashion;
			if(bMan)
				vecFashion = ClothespressData::Inst()->GetMaleFashionData();
			else if(bMan == FALSE)
				vecFashion = ClothespressData::Inst()->GetFemaleFashionData();
			DWORD nSuitID = m_n16CurPage == 0 ? nIndex : (m_n16CurPage * FASHION_SUIT_NUM_OF_ONE_PAGE + nIndex);
			if(nSuitID >=0 && nSuitID < vecFashion.size())
				m_dwCurSuitIndex = vecFashion[nSuitID].nSuitId;
		}
	}
}

DWORD ClothespressFrame::OnNS_NetRehanding( tagNS_Rehanding* pNetCmd, DWORD dwParam )
{
	tagRehanding eve(_T("tagRehanding"), NULL);
	eve.dwError = pNetCmd->dwErrorCode;
	TObjRef<GameFrameMgr>()->SendEvent(&eve);

	TMap<DWORD, tagFashionInfo*> mapInfo = ItemMgr::Inst()->GetFashionMap();
	tagFashionInfo *pFashionInfo = mapInfo.Peek(m_dwCurSuitIndex);
	if(!P_VALID(pFashionInfo))
		return 0;
	
	if(EFS_UseBySelf == pFashionInfo->eState )
		m_pBtnChangeSuit->SetText(g_StrTable[_T("GetOffFashion")]);
	else
		m_pBtnChangeSuit->SetText(g_StrTable[_T("GetOnFashion")]);

	m_dwCurSuitIndex = 0;
	return 0;
}


DWORD ClothespressFrame::OnUpdateItemEvent( tagUpdateLPItemEvent* pGameEvent )
{
	if(EICT_Wardrobe == pGameEvent->eType)
	{
		UpdateCurPage();
		UpdateDIYFashion();
	}
	return 0;
}

VOID ClothespressFrame::TryDress(BOOL bDIY /* = FALSE */)
{
	m_pWndTryDress->ReLoadTryEquip();
	tagClothespressData *pData = ClothespressData::Inst()->GetFashionInfo(m_dwCurSuitIndex);
	if(!P_VALID(pData))
		return;
	DWORD dwFashionID =0;
	for( int i = 0; i < EF_End; i++ )
	{
		if(bDIY)
			dwFashionID = m_pDiyFashion[i]->GetBtnData().GetData2();
		else
			dwFashionID = pData->dwSuitTypeID[i];
		Equipment *pEquip = (Equipment*)ItemMgr::Inst()->GetFashion(dwFashionID, bDIY);
		if(!P_VALID(pEquip))
			continue;
		const tagItem *pItemData = pEquip->GetData();
		if(!P_VALID(pItemData))
			continue;
		JudgeTryDress(pItemData->n64Serial);
	}
	UpdateAvatar();
}

BOOL ClothespressFrame::IsDiySuitIsComplete()
{
	ItemContainer *pClothes = ItemMgr::Inst()->GetClothespress();
	BOOL bComplete = TRUE;
	for( INT16 i = 0; i < 4; i++ )
	{
		Equipment *pEquip = (Equipment*)pClothes->GetValue(i);
		if(!P_VALID(pEquip))
		{
			bComplete = FALSE;
			return bComplete;
		}
	}
	return bComplete;
}

BOOL ClothespressFrame::ISSuitComplete(DWORD dwSuitID)
{
	tagClothespressData *pData = ClothespressData::Inst()->GetFashionInfo(dwSuitID);
	if(!P_VALID(pData))
		return FALSE;
	BOOL bComplete = TRUE;
	for( int i = 0; i < EF_End; i++ )
	{
		if( pData->dwSuitTypeID[i] != 0 && 
			pData->dwSuitTypeID[i] != GT_INVALID )
		{
			Equipment *pEquip = (Equipment*)ItemMgr::Inst()->GetFashion(pData->dwSuitTypeID[i], false);
			if(!P_VALID(pEquip))
			{
				bComplete = FALSE;
				return bComplete;
			}
		}
	}
	return bComplete;
}

DWORD ClothespressFrame::OnEventTransform(tagGameEvent* pEvent)
{
	if(pEvent->strEventName==_T("LocalPlayerTransform"))
	{
		m_pBtnChangeSuit->SetEnable(FALSE);
	}
	else if(pEvent->strEventName==_T("LocalPlayerUnTransform"))
	{
		m_pBtnChangeSuit->SetEnable(TRUE);
	}
	return 0;
}

DWORD ClothespressFrame::OnOffFashionClothes(tagGameEvent* pEvent)
{
	TMap<DWORD, tagFashionInfo*> mapInfo = ItemMgr::Inst()->GetFashionMap();
	tagFashionInfo *pFashionInfo;
	int nIndex = GT_INVALID;
	mapInfo.ResetIterator();
	while(mapInfo.PeekNext(pFashionInfo))
	{
		if(!P_VALID(pFashionInfo))
			continue;
		if(EFS_UseBySelf == pFashionInfo->eState)
		{
			nIndex = pFashionInfo->dwIndex;
			break;
		}
	}

	if(nIndex == GT_INVALID)
		return 0;
	tagNC_Rehanding m;
	m.dwWardrobeLayerID = nIndex;
	m.bTakeOffFashion = true;
	m_pSession->Send(&m);
	m_pBtnChangeSuit->SetEnable(false);
	return 0;
}

DWORD ClothespressFrame::GetTotalAttrativeValue()
{
	INT nMettle = 0;
	// DIY
	for( int j = 0; j < EF_End; j++ )
	{
		Equipment *pEquip = (Equipment*)ItemMgr::Inst()->GetClothespress()->GetValue(j);
		if (P_VALID(pEquip))
		{
			nMettle += pEquip->GetEquip()->n16Appearance;
		}
	}
	
	// Normal
	vector<tagClothespressData> vecFashion;
	vecFashion = ClothespressData::Inst()->GetFemaleFashionData();

	size_t nStart = 0;
	TMap<DWORD, tagFashionInfo*> mapInfo = ItemMgr::Inst()->GetFashionMap();
	for(int i = 0; i < vecFashion.size(); i++)
	{
		if( nStart >= vecFashion.size() )
			break;
		DWORD dwSuitIndex = vecFashion[nStart].nSuitId;
		if(!mapInfo.IsExist(dwSuitIndex))
			continue;

		tagClothespressData *pData = ClothespressData::Inst()->GetFashionInfo(dwSuitIndex);
		if(P_VALID(pData))
		{
			for( int j = 0; j < EF_End; j++ )
			{
				Equipment* pEquip = ItemMgr::Inst()->GetFashion(pData->dwSuitTypeID[j], false);
				if (P_VALID(pEquip))
				{
					nMettle += pEquip->GetEquip()->n16Appearance;
				}
			}
		}
		nStart++;
	}

	vecFashion = ClothespressData::Inst()->GetMaleFashionData();
	nStart = 0;
	mapInfo = ItemMgr::Inst()->GetFashionMap();
	for(int i = 0; i < vecFashion.size(); i++)
	{
		if( nStart >= vecFashion.size() )
			break;
		DWORD dwSuitIndex = vecFashion[nStart].nSuitId;
		if(!mapInfo.IsExist(dwSuitIndex))
			continue;

		tagClothespressData *pData = ClothespressData::Inst()->GetFashionInfo(dwSuitIndex);
		if(P_VALID(pData))
		{
			for( int j = 0; j < EF_End; j++ )
			{
				Equipment* pEquip = ItemMgr::Inst()->GetFashion(pData->dwSuitTypeID[j], false);
				if (P_VALID(pEquip))
				{
					nMettle += pEquip->GetEquip()->n16Appearance;
				}
			}
		}
		nStart++;
	}

	return nMettle;
}