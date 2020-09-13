#include "StdAfx.h"
#include "PetStateFrame.h"
#include "StaticEx.h"
#include "PetDef.h"
#include "PetProtoData.h"
#include "PetManager.h"
#include "Item.h"
#include "ItemMgr.h"
#include "ItemProtoData.h"
#include "IconStatic.h"
#include "GameSet.h"
#include ".\ToolTipFrame.h"

PetStateFrame::PetStateFrame(void) :
    m_Trunk(this),
    m_dwStartTime(0)
{
}

PetStateFrame::~PetStateFrame(void)
{
}

BOOL PetStateFrame::Init( GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam )
{
	BOOL bRet = GameFrame::Init(pMgr, pFather, dwParam);

    return bRet;
}

BOOL PetStateFrame::Destroy()
{
    BOOL bRet = GameFrame::Destroy();

    //! 销毁窗体
    if( P_VALID(m_pWnd) )
    {
        m_pGUI->AddToDestroyList(m_pWnd);
        m_pWnd = NULL;
    }

    return bRet;
}

BOOL PetStateFrame::ReloadUI()
{
    XmlElement ele;
    tstring strPath = g_strLocalPath + _T("\\ui\\pet_state.xml");
    m_pGUI->LoadXml(&ele, "VFS_System", strPath.c_str());

    // 更改控件
    m_pGUI->ChangeXml(&ele, "pett\\pet\\facepic", "ClassName", "StaticEx");
	m_pGUI->ChangeXml(&ele, "pett\\pet\\feed", "ClassName", "StaticEx");
	m_pGUI->ChangeXml(&ele, "pett\\pet\\mood", "ClassName", "StaticEx");
	m_pGUI->ChangeXml(&ele, "pett\\pet\\learn", "ClassName", "StaticEx");

    // 创建
    m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &ele);
    //m_pStcName      = static_cast<GUIStatic*>   (m_pWnd->GetChild(_T("pet\\name")));
    m_pStcIcon      = static_cast<StaticEx*>    (m_pWnd->GetChild(_T("pet\\facepic")));
    m_pPgssEnergy   = static_cast<GUIProgress*> (m_pWnd->GetChild(_T("pet\\hp")));
    m_pPgssExp      = static_cast<GUIProgress*> (m_pWnd->GetChild(_T("pet\\mp")));

	m_pIconFeed      = static_cast<IconStatic*> (m_pWnd->GetChild(_T("pet\\feed")));
	m_pIconMood      = static_cast<IconStatic*> (m_pWnd->GetChild(_T("pet\\mood")));
	m_pIconFeed->SetPic(_T("data\\ui\\pet\\SM_fairy_feed.bmp"));
	m_pIconLearn      = static_cast<IconStatic*> (m_pWnd->GetChild(_T("pet\\learn")));
	m_pIconLearn->SetInvisible(TRUE);
	m_pStcSaveExp	= (GUIStatic*)m_pWnd->GetChild(_T("pet\\facepic\\sealpic"));

    // 注册事件处理函数
    m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(), m_Trunk.sfp1(&PetStateFrame::EventHandler));

    m_pPgssEnergy->SetSpeed(100000.0f);
	m_pPgssEnergy->SetMaxValue(2000.0f);
    m_pPgssExp->SetSpeed(100000.0f);

    return TRUE;
}

VOID PetStateFrame::Update()
{
    GameFrame::Update();

    if (Kernel::Inst()->GetAccumTimeDW() - m_dwStartTime > 500)
    {
        m_dwStartTime = Kernel::Inst()->GetAccumTimeDW();

        // 更新宠物数据
        UpdatePet();
    }
}

VOID PetStateFrame::Render3D()
{

}

BOOL PetStateFrame::EscCancel()
{
    return FALSE;
}

DWORD PetStateFrame::EventHandler( tagGUIEvent* pEvent )
{
    GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);

    switch (pEvent->eEvent)
    {
    case EGUIE_Click:
        {

        }
        break;
	case EGUIE_MoveIntoCtrl:
		{
			if(pWnd == m_pPgssEnergy)
			{
				TCHAR szBuff[X_SHORT_NAME] = {0};
				_stprintf( szBuff, _T("%d/%d"), m_nFairyRealMood, 2000 );
				tstring strTip = g_StrTable[_T("TipsDefaultFont")];
				strTip += szBuff;
				ShowTip( m_pPgssEnergy, strTip.c_str() );
			}
			else if( pWnd == m_pIconMood )
			{
				tstring strMood = GetFairyMoodState();
				ShowTip( m_pIconMood, strMood.c_str() );
			}
		}
		break;
    }

    return 0;
}

void PetStateFrame::ShowWnd( bool bVisable )
{
    m_pWnd->SetInvisible(!bVisable);
    if (bVisable)
    {

    }
    else
    {

    }
}

void PetStateFrame::UpdatePet()
{
    //const tagPet* pPet = PetManager::Inst()->GetPet(PetManager::Inst()->GetCurrPetID());
    //if (P_VALID(pPet))
    //{
    //      const tagPetProto* pPetProto = PetProtoData::Inst()->FindPetProto(pPet->dwTypeID);
    //      if (P_VALID(pPetProto))
    //      {
    //          // 名字
    //          m_pStcName->SetText(pPet->strName.c_str());
    //          m_pStcName->SetRefresh(TRUE);

    //          // 图标
    //          if (PetManager::Inst()->IsPetRiding(pPet->dwID))
    //          {
    //              m_pStcIcon->SetPicFile(pPetProto->strIconPath[EPMT_Mount]);
    //          }
    //          else
    //          {
    //              m_pStcIcon->SetPicFile(pPetProto->strIconPath[EPMT_Follow]);
    //          }

    //          // 精气
    //          m_pPgssEnergy->SetMaxValue(pPet->nAttrA[ECSPA_nSpiritMax]);
    //          m_pPgssEnergy->SetValue(pPet->nAttrA[ECSPA_nSpirit]);

    //          // 经验
    //          m_pPgssExp->SetMaxValue(pPet->nAttrA[ECSPA_nExpLevelUp]);
    //          m_pPgssExp->SetValue(pPet->nAttrA[ECSPA_nExpCurrent]);

    //          ShowWnd(true);
    //          return;
    //      }
    //}

	Equipment *pFairy = ItemMgr::Inst()->GetCurEquip(EEP_Face);
	if( !P_VALID(pFairy) )
	{
		ShowWnd(false);
		return;
	}

	const tagFabaoSpec *pFairySpe = pFairy->GetFaBaoSpe();
	const tagFabaoProto *pFairyProto = (tagFabaoProto*)pFairy->GetProto();
	if(!P_VALID(pFairySpe) && !P_VALID(pFairyProto) )
	{
		ShowWnd(false);
		return;
	}
		
	// 名字
	//if(P_VALID(pFairyProto->pDisplayInfo))
	//{
	//	m_pStcName->SetText(pFairyProto->pDisplayInfo->szName);
	//	m_pStcName->SetRefresh(TRUE);
	//}

	// 图标
	tstring strPicName = _T("data\\ui\\Icon\\");
	tstring strPic = ItemProtoData::Inst()->GetFairyPic(pFairyProto->dwTypeID, pFairySpe->n16Stage);
	strPicName += strPic;
	strPicName += _T(".tga");
	m_pStcIcon->SetPicFile(strPicName);

	// 心情
	if(pFairySpe->n16Stage >= 30)
	{
		m_nFairyRealMood = pFairySpe->nMood;
		m_pPgssEnergy->SetValue((float)m_nFairyRealMood);
		m_pPgssEnergy->SetInvisible(FALSE);
		SetFairyMoodPic(pFairySpe->nMood);
		if(GameSet::Inst()->Get(EGS_FairyAutoFeed))
			m_pIconFeed->SetInvisible(FALSE);
		else
			m_pIconFeed->SetInvisible(TRUE);
	}
	else
	{
		m_pIconFeed->SetInvisible(TRUE);
		m_pIconMood->SetInvisible(TRUE);
		m_pPgssEnergy->SetInvisible(TRUE);
	}

	if(pFairySpe->dwUnsureSkill != GT_INVALID)
		m_pIconLearn->SetInvisible(FALSE);
	else
		m_pIconLearn->SetInvisible(TRUE);

	if(pFairySpe->n16Stage >= MIN_STORE_EXP_FABAO_LEVEL && pFairySpe->bStoredExpOpen )
		m_pStcSaveExp->SetInvisible(FALSE);
	else
		m_pStcSaveExp->SetInvisible(TRUE);

	// 经验
	INT nCurExp = pFairySpe->nPracticeValue - ItemMgr::Inst()->GetTrumpCurExp( pFairySpe->n16Stage );
	INT nTotalExp = ItemMgr::Inst()->GetTrumpNextLevelExp( pFairySpe->n16Stage );
	m_pPgssExp->SetMaxValue((float)nTotalExp);
	m_pPgssExp->SetValue((float)nCurExp, true);
	ShowWnd(true);
	return;
}

VOID PetStateFrame::SetFairyMoodPic(INT nMood)
{
	INT nIndex = 0;
	if( nMood < 200 )
		nIndex = 1;
	else if( nMood < 400 )
		nIndex = 2;
	else if( nMood < 600 )
		nIndex = 3;
	else if( nMood < 800 )
		nIndex = 4;
	else if( nMood < 1000 )
		nIndex = 5;
	else
		nIndex = 6;
	TCHAR szTemp[X_LONG_NAME] = {0};
	_sntprintf( szTemp, X_LONG_NAME, _T("data\\ui\\pet\\SM_fairy_%d.bmp"), nIndex);
	m_pIconMood->SetPic(szTemp);
	m_pIconMood->SetInvisible(FALSE);
}

tstring PetStateFrame::GetFairyMoodState()
{
	if(m_nFairyRealMood > 1000)
		return g_StrTable[_T("FairyMood_High")];
	else if(m_nFairyRealMood >= 800)
		return g_StrTable[_T("FairyMood_Happey")];
	else if(m_nFairyRealMood >= 600)
		return g_StrTable[_T("FairyMood_Middle")];
	else if(m_nFairyRealMood >= 400)
		return g_StrTable[_T("FairyMood_depression")];
	else if(m_nFairyRealMood >= 200)
		return g_StrTable[_T("FairyMood_Low")];
	else
		return g_StrTable[_T("FairyMood_drearily")];
	return _T("");
}